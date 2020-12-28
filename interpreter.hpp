
// ========================================================================== //

#ifndef INCLUDE_INTERPRETER_HPP
#define INCLUDE_INTERPRETER_HPP

// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <map>
#include <string>
#include <string_view>
#include <optional>

#include "processor.hpp"

// -------------------------------------------------------------------------- //

class CInterpreter {

  public:

  using CStreamPos = std::ifstream::pos_type;

  bool interpret();

  void branch();

  void seek(CStreamPos position);
  CStreamPos tell() const;

  inline std::string_view cursor() const {
    return mCursor;
  }

  void error();

  bool skip(size_t);
  bool skipSpace();

  std::string_view readWord();
  std::optional<int32_t> readInt(int8_t base = -1);
  std::optional<std::string> readString(bool silent = false);

  private:

  std::map<std::string, CStreamPos> mLabels;
  std::string_view mLine;
  std::string_view mCursor;
  size_t mLineNo { 0 };
  int32_t mArgs[8];
  size_t mArgNo { 0 };
  std::string mLabel;
  bool mBranchAhead { false };

  bool readArg(
    std::string_view signature,
    bool silent = false
  );

  bool parseSignature(
    std::string_view signature,
    bool silent = false
  );

};

// -------------------------------------------------------------------------- //

extern std::ifstream * gStream;
extern CInterpreter * gInterpreter;

// -------------------------------------------------------------------------- //

// ========================================================================== //

#endif
