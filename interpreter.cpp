
// ========================================================================== //

// -------------------------------------------------------------------------- //

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <string_view>

#include "directive.hpp"
#include "instruction.hpp"
#include "interpreter.hpp"
#include "processor.hpp"
#include "span.hpp"

// -------------------------------------------------------------------------- //

std::ifstream * gStream { nullptr };
CInterpreter * gInterpreter { nullptr };

// -------------------------------------------------------------------------- //

bool CInterpreter::interpret() {
  if (gStream == nullptr) {
    return false;
  }

  ++mLineNo;
  char input_buffer[1024];

  if (!gStream->getline(input_buffer, 1024)) {
    if (mBranchAhead) {
      std::cerr << "missing branch target '" << mLabel << "'" << std::endl;
    }

    return false;
  }

  mLine = { std::data(input_buffer), std::strlen(input_buffer) };

  auto const comment = std::find(
    mLine.begin(), mLine.end(), ';'
  );

  if (comment != mLine.end()) {
    mLine = mLine.substr(
      0, std::distance(mLine.begin(), comment)
    );
  }

  if (mLine.empty()) {
    return true;
  }

  mCursor = mLine;
  skipSpace();

  if (mCursor.empty()) {
    return false;
  }

  std::string_view const key {
    readWord()
  };

  if (key.empty()) {
    return false;
  }

  skipSpace();

  if (!mCursor.empty() && mCursor[0] == ':') {
    std::string label { key };
    mLabels[label] = gStream->tellg();

    if (mLabel == label) {
      mBranchAhead = false;
    }

    return true;
  }

  if (mBranchAhead) {
    return true;
  }

  mArgNo = 0;
  CDirective const * directive { nullptr };
  CInstruction const * instruction { nullptr };
  uint8_t bits { 0 };

  if ((directive = CDirective::Fetch(key)) != nullptr) {
    if (!directive->callback()) {
      return false;
    }
  } else if ((instruction = CInstruction::Fetch(key, &bits)) != nullptr) {
    if (!parseSignature(instruction->signature)) {
      return false;
    }

    instruction->callback({ mArgs, mArgNo }, bits);
  } else {
    error();
    std::cerr << "unknown operation" << std::endl;
    return false;
  }

  return true;
}

// -------------------------------------------------------------------------- //

void CInterpreter::branch() {
  if (gStream == nullptr || mLabel.empty()) {
    return;
  }

  if (mLabels.count(mLabel) == 1) {
    gStream->seekg(mLabels[mLabel]);
  } else {
    mBranchAhead = true;
  }
}

// -------------------------------------------------------------------------- //

void CInterpreter::seek(
  CStreamPos const position
) {
  if (gStream == nullptr) {
    return;
  }

  gStream->seekg(position);
}

// -------------------------------------------------------------------------- //

CInterpreter::CStreamPos
CInterpreter::tell() const {
  if (gStream == nullptr) {
    return 0;
  }

  return gStream->tellg();
}

// -------------------------------------------------------------------------- //

void CInterpreter::error() {
  std::cerr << "ERROR on line " << mLineNo << ":" << std::endl;
}

// -------------------------------------------------------------------------- //

bool CInterpreter::skip(
  size_t const count
) {
  if (count >= mCursor.size()) {
    mCursor = {};
  } else {
    mCursor = mCursor.substr(count);
  }

  return (!mCursor.empty());
}

// -------------------------------------------------------------------------- //

bool CInterpreter::skipSpace() {
  size_t count { 0 };

  while (count < mCursor.size() && mCursor[count] == ' ') {
    ++count;
  }

  return skip(count);
}

// -------------------------------------------------------------------------- //

std::string_view
CInterpreter::readWord() {
  size_t count { 0 };

  while (
    (count < mCursor.size()) &&
    (mCursor[count] != ' ') &&
    (mCursor[count] != ':')
  ) {
    ++count;
  }

  std::string_view const word {
    mCursor.substr(0, count)
  };

  skip(count);
  return word;
}

// -------------------------------------------------------------------------- //

std::optional<int32_t>
CInterpreter::readInt(
  int8_t base
) {
  if (mCursor.empty()) {
    return std::nullopt;
  }

  bool negative { false };

  if (mCursor[0] == '-') {
    negative = true;
    skip(1);

    if (mCursor.empty()) {
      return std::nullopt;
    }
  }

  size_t min_digits { 0 };

  if (mCursor[0] == '0') {
    skip(1);

    if (!mCursor.empty()) {
      switch (mCursor[0]) {
        case 'x':
        case 'X': {
          if (base == -1) {
            base = 16;
          }

          if (base == 16) {
            min_digits = 1;
            skip(1);
          }

          break;
        }
        case 'b':
        case 'B': {
          if (base == -1) {
            base = 2;
          }

          if (base == 2) {
            min_digits = 1;
            skip(1);
          }

          break;
        }
      }
    }

    if (base == -1) {
      base = 8;
    }
  }

  if (base == -1) {
    min_digits = 1;
    base = 10;
  }

  int32_t value { 0 };
  size_t digits { 0 };

  while (!mCursor.empty()) {
    int32_t digit { mCursor[0] };

    if ('0' <= digit && digit <= '9') {
      digit = (digit - '0');
    } else if ('a' <= digit && digit <= 'z') {
      digit = (10 + (digit - 'a'));
    } else if ('A' <= digit && digit <= 'Z') {
      digit = (10 + (digit - 'A'));
    }

    if (digit >= base) {
      break;
    }

    value = (value * base + digit);
    ++digits;
    skip(1);
  }

  if (digits < min_digits) {
    return std::nullopt;
  }

  if (negative) {
    value = -value;
  }

  return value;
}

// -------------------------------------------------------------------------- //

std::optional<std::string>
CInterpreter::readString(
  bool const silent
) {
  skipSpace();

  if (mCursor.empty()) {
    if (!silent) {
      error();
      std::cerr << "expected string." << std::endl;
    }

    return std::nullopt;
  }

  if (mCursor[0] != '"' && mCursor[0] != '\'') {
    if (!silent) {
      error();
      std::cerr << "expected string." << std::endl;
    }

    return std::nullopt;
  }

  char const terminator { mCursor[0] };
  skip(1);

  std::string string;
  string.reserve(mCursor.size());

  while (!mCursor.empty()) {
    if (mCursor[0] == terminator) {
      return string;
    } else if (mCursor[0] == '\\') {
      skip(1);

      if (mCursor.empty()) {
        if (!silent) {
          error();
          std::cerr << "bad escape sequence." << std::endl;
        }

        return std::nullopt;
      }

      switch (mCursor[0]) {
        case '\\':
        case '\'':
        case '"': {
          string.push_back(mCursor[0]);
          skip(1);
          continue;
        }
        case 'r': {
          string.push_back('\r');
          skip(1);
          continue;
        }
        case 'n': {
          string.push_back('\n');
          skip(1);
          continue;
        }
        case 't': {
          string.push_back('\t');
          skip(1);
          continue;
        }
        default: {
          if (!silent) {
            error();
            std::cerr << "bad escape sequence." << std::endl;
          }

          return std::nullopt;
        }
        // TODO: add unicode escape sequences (u, U, x)?
      }
    } else {
      string.push_back(mCursor[0]);
      skip(1);
    }
  }

  if (!silent) {
    error();
    std::cerr << "unterminated string." << std::endl;
  }

  return std::nullopt;
}

// -------------------------------------------------------------------------- //

bool CInterpreter::readArg(
  std::string_view signature,
  bool const silent
) {
  size_t i { 0 };

  while (signature[i] != ':') {
    ++i;
  }

  std::string_view const name {
    signature.substr(0, i)
  };

  if (mCursor.empty()) {
    if (!silent) {
      error();
      std::cerr << "missing argument '" << name << "'" << std::endl;
    }

    return false;
  }

  std::string_view const type {
    signature.substr(i + 1)
  };

  int8_t base { - 1 };

  if (type == "gpr") {
    base = 10;

    if (mCursor[0] != 'r') {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }

    skip(1);

    if (mCursor.empty()) {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }
  } else if (type == "fpr") {
    base = 10;

    if (mCursor[0] != 'f') {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }

    skip(1);

    if (mCursor.empty()) {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }
  } else if (type == "cr") {
    base = 10;

    if (mCursor.size() < 2 || mCursor.substr(0, 2) != "cr") {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }

    skip(2);

    if (mCursor.empty()) {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }
  } else if (type == "addr") {
    mLabel = readWord();

    if (mLabel.empty()) {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }

    return true;
  }

  std::optional<int32_t> value_opt = readInt(base);

  if (value_opt == std::nullopt) {
    if (!silent) {
      error();
      std::cerr << "bad argument '" << name << "'" << std::endl;
    }

    return false;
  }

  int32_t const value { *value_opt };

  if (type == "gpr" || type == "fpr") {
    if (value < 0 || value > 31) {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }
  } else if (type == "si") {
    if (
      (value < std::numeric_limits<int16_t>::lowest()) ||
      (value > std::numeric_limits<int16_t>::max())
    ) {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }
  } else if (type == "ui") {
    if (
      (value < std::numeric_limits<uint16_t>::lowest()) ||
      (value > std::numeric_limits<uint16_t>::max())
    ) {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }
  } else if (type == "cr") {
    if (value < 0 || value > 7) {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }
  } else if (type == "bit") {
    if (value < 0 || value > 31) {
      if (!silent) {
        error();
        std::cerr << "bad argument '" << name << "'" << std::endl;
      }

      return false;
    }
  }

  mArgs[mArgNo++] = value;
  return true;
}

// -------------------------------------------------------------------------- //

bool CInterpreter::parseSignature(
  std::string_view signature,
  bool const silent
) {
  size_t i { 0 };

  while (i < signature.size()) {
    switch (signature[i]) {
      case ' ': {
        skipSpace();
        ++i;
        break;
      }
      case '{': {
        size_t size { 0 };
        ++i;

        while (signature[i + size] != '}') {
          ++size;
        }

        std::string_view const subsignature {
          signature.substr(i, size)
        };

        if (!readArg(subsignature, silent)) {
          return false;
        }

        skipSpace();
        i += (size + 1);
        break;
      }
      case '[': {
        size_t scope { 1 };
        size_t size { 0 };
        ++i;

        for (;;) {
          switch (signature[i + size]) {
            case '[': {
              ++scope;
              break;
            }
            case ']': {
              --scope;
              break;
            }
          }

          if (scope == 0) {
            break;
          }

          ++size;
        }

        std::string_view const subsignature {
          signature.substr(i, size)
        };

        std::string_view const save_cursor { mCursor };
        size_t const save_argno { mArgNo };

        if (!parseSignature(subsignature, true)) {
          mCursor = save_cursor;
          mArgNo = save_argno;
        }

        i += (size + 1);
        break;
      }
      default: {
        if (mCursor.empty() || mCursor[0] != signature[i]) {
          if (!silent) {
            error();
            std::cerr << "expected '" << signature[i] << "'" << std::endl;
          }

          return false;
        }

        skip(1);
        skipSpace();
        ++i;
        break;
      }
    }
  }

  return true;
}

// -------------------------------------------------------------------------- //

// ========================================================================== //
