
// ========================================================================== //

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

#include "directive.hpp"
#include "interpreter.hpp"
#include "processor.hpp"

// -------------------------------------------------------------------------- //

CDirective * CDirective::sFirst { nullptr };

// -------------------------------------------------------------------------- //

static bool PrintRegistre(
  std::ostringstream & echo,
  std::string_view key,
  std::string_view style
);

// -------------------------------------------------------------------------- //

CDirective::CDirective(
  std::string_view const key,
  FCallback const callback
) :
  key { key },
  callback { callback },
  next { sFirst }
{
  sFirst = this;
}

// -------------------------------------------------------------------------- //

static CDirective sDir_exit {
  ".exit",
  [] () {
    return false;
  }
};

// -------------------------------------------------------------------------- //

static CDirective sDir_echo {
  ".echo",
  [] () {
    std::optional<std::string> string_opt {
      gInterpreter->readString()
    };

    if (string_opt == std::nullopt) {
      return false;
    }

    std::string_view cursor {
      string_opt->data(),
      string_opt->size()
    };

    std::ostringstream echo;

    do {
      auto start = std::find(
        cursor.begin(), cursor.end(), '{'
      );

      if (start != cursor.begin()) {
        echo << cursor.substr(
          0, std::distance(cursor.begin(), start)
        );
      }

      if (start == cursor.end()) {
        break;
      }

      ++start;

      if (start == cursor.end()) {
        gInterpreter->error();
        std::cerr << "bad print sequence." << std::endl;
        return false;
      }

      if (*start == '{') {
        cursor = cursor.substr(
          (std::distance(cursor.begin(), start) + 1)
        );

        echo << '{';
        continue;
      }

      cursor = cursor.substr(
        std::distance(cursor.begin(), start)
      );

      auto const end = std::find(
        cursor.begin(), cursor.end(), '}'
      );

      if (end == cursor.end()) {
        gInterpreter->error();
        std::cerr << "bad print sequence." << std::endl;
        return false;
      }

      auto const middle = std::find(
        cursor.begin(), end, ':'
      );

      std::string_view const key {
        cursor.substr(0, std::distance(cursor.begin(), middle))
      };

      if (key.empty()) {
        gInterpreter->error();
        std::cerr << "bad print sequence." << std::endl;
        return false;
      }

      std::string_view style;

      if (middle != end) {
        style = cursor.substr(
          (std::distance(cursor.begin(), middle) + 1),
          (std::distance(middle, end) - 1)
        );

        if (style.empty()) {
          gInterpreter->error();
          std::cerr << "bad print sequence." << std::endl;
          return false;
        }
      }

      if (!PrintRegistre(echo, key, style)) {
        return false;
      }

      cursor = cursor.substr(
        (std::distance(cursor.begin(), end) + 1)
      );
    } while (!cursor.empty());

    std::cout << echo.str() << std::endl;
    return true;
  }
};

// -------------------------------------------------------------------------- //

CDirective const *
CDirective::Fetch(
  std::string_view const key
) {
  CDirective const * it;

  for (it = sFirst; it != nullptr; it = it->next) {
    if (it->key == key) {
      return it;
    }
  }

  return nullptr;
}

// -------------------------------------------------------------------------- //

bool PrintRegistre(
  std::ostringstream & echo,
  std::string_view const key,
  std::string_view style
) {
  if (key[0] == 'r') {
    size_t index;

    if (
      (key.size() == 2) &&
      ('0' <= key[1] && key[1] <= '9')
    ) {
      index = static_cast<size_t>(key[1] - '0');
    } else if (
      (key.size() == 3) &&
      ('0' <= key[1] && key[1] <= '9') &&
      ('0' <= key[2] && key[2] <= '9')
    ) {
      index = static_cast<size_t>(
        (key[1] - '0') * 10 + (key[2] - '0')
      );
    } else {
      gInterpreter->error();
      std::cerr << "bad print sequence." << std::endl;
      return false;
    }

    if (index > 32) {
      gInterpreter->error();
      std::cerr << "bad print sequence." << std::endl;
      return false;
    }

    CGPR const & gpr { gPPC->gpr(index) };

    if (style.empty()) {
      echo << gpr.s32();
    } else {
      bool next { true };

      while (!style.empty() && next) {
        switch (style[0]) {
          case '-': {
            echo << std::right;
            break;
          }
          case '+': {
            echo << std::showpos;
            break;
          }
          case '#': {
            echo << std::showbase;
            break;
          }
          case '0': {
            echo << std::setfill('0');
            break;
          }
          default: {
            next = false;
            break;
          }
        }

        if (next) {
          style = style.substr(1);
        }
      }

      size_t width { 0 };

      while (
        !style.empty() &&
        ('0' <= style[0] && style[0] <= '9')
      ) {
        width = (width * 10 + (style[0] - '0'));
        style = style.substr(1);
      }

      echo.width(width);

      if (style.size() != 1) {
        gInterpreter->error();
        std::cerr << "bad print sequence." << std::endl;
        return false;
      }

      switch (style[0]) {
        case 'd':
        case 'i': {
          echo << gpr.s32();
          break;
        }
        case 'u': {
          echo << gpr.u32();
          break;
        }
        case 'x':
        case 'X': {
          if (style[0] == 'X') {
            echo << std::uppercase;
          }

          echo << std::hex << gpr.u32();
          break;
        }
        default: {
          gInterpreter->error();
          std::cerr << "bad print sequence." << std::endl;
          return false;
        }
      }
    }
  } else if (key[0] == 'f') {
    size_t index;

    if (
      (key.size() == 2) &&
      ('0' <= key[1] && key[1] <= '9')
    ) {
      index = static_cast<size_t>(key[1] - '0');
    } else if (
      (key.size() == 3) &&
      ('0' <= key[1] && key[1] <= '9') &&
      ('0' <= key[2] && key[2] <= '9')
    ) {
      index = static_cast<size_t>(
        (key[1] - '0') * 10 + (key[2] - '0')
      );
    } else {
      gInterpreter->error();
      std::cerr << "bad print sequence." << std::endl;
      return false;
    }

    if (index > 32) {
      gInterpreter->error();
      std::cerr << "bad print sequence." << std::endl;
      return false;
    }

    CFPR const & fpr { gPPC->fpr(index) };

    if (style.empty()) {
      echo << fpr.f64();
    } else {
      bool next { true };

      while (!style.empty() && next) {
        switch (style[0]) {
          case '-': {
            echo << std::right;
            break;
          }
          case '+': {
            echo << std::showpos;
            break;
          }
          case '#': {
            echo << std::showpoint;
            break;
          }
          case '0': {
            echo << std::setfill('0');
            break;
          }
          default: {
            next = false;
            break;
          }
        }

        if (next) {
          style = style.substr(1);
        }
      }

      size_t width { 0 };

      while (
        !style.empty() &&
        ('0' <= style[0] && style[0] <= '9')
      ) {
        width = (width * 10 + (style[0] - '0'));
        style = style.substr(1);
      }

      echo.width(width);

      if (!style.empty() && style[0] == '.') {
        style = style.substr(1);
        size_t precision { 0 };

        while (
          !style.empty() &&
          ('0' <= style[0] && style[0] <= '9')
        ) {
          precision = (precision * 10 + (style[0] - '0'));
          style = style.substr(1);
        }

        echo.precision(precision);
      }

      if (style.size() != 1) {
        gInterpreter->error();
        std::cerr << "bad print sequence." << std::endl;
        return false;
      }

      switch (style[0]) {
        case 'f': {
          echo << fpr.f64();
          break;
        }
        case 'h': {
          echo << fpr.ps0();
          break;
        }
        case 'l': {
          echo << fpr.ps1();
          break;
        }
        case 'u': {
          echo << fpr.u64();
          break;
        }
        case 'x':
        case 'X': {
          if (style[0] == 'X') {
            echo << std::uppercase;
          }

          echo << std::hex << fpr.u64();
          break;
        }
        default: {
          gInterpreter->error();
          std::cerr << "bad print sequence." << std::endl;
          return false;
        }
      }
    }
  } else {
    gInterpreter->error();
    std::cerr << "bad print sequence." << std::endl;
    return false;
  }

  return true;
}

// -------------------------------------------------------------------------- //

// ========================================================================== //
