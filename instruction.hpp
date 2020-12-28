
// ========================================================================== //

#ifndef INCLUDE_INSTRUCTION_HPP
#define INCLUDE_INSTRUCTION_HPP

// -------------------------------------------------------------------------- //

#include <cstdint>
#include <functional>
#include <string_view>

#include "span.hpp"

// -------------------------------------------------------------------------- //

enum EBit : uint8_t {

  EBIT_RC = 0b0000'0001, // record bit
  EBIT_OE = 0b0000'0010, // overflow-exception bit
  EBIT_AA = 0b0000'0100, // absolute-address bit
  EBIT_LK = 0b0000'1000, // link bit
  EBIT_Y  = 0b0001'0000, // branch is likely to be taken

};

// -------------------------------------------------------------------------- //

struct CInstruction {

  using FCallback = std::function<void (CSpanT<int32_t>, uint8_t)>;

  std::string_view const key;
  std::string_view const signature;
  FCallback const callback;

  CInstruction(
    std::string_view key,
    FCallback callback
  );

  CInstruction(
    std::string_view key,
    std::string_view signature,
    FCallback callback
  );

  static CInstruction const * Fetch(
    std::string_view key,
    uint8_t * bits = nullptr
  );

  private:

  CInstruction * next { nullptr };

  static CInstruction * sFirst;

};

// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //

// ========================================================================== //

#endif
