
// ========================================================================== //

#include <cstdint>
#include <string_view>

#include "instruction.hpp"

// -------------------------------------------------------------------------- //

CInstruction * CInstruction::sFirst { nullptr };

// -------------------------------------------------------------------------- //

CInstruction::CInstruction(
  std::string_view const key,
  FCallback const callback
) :
  CInstruction { key, "", callback }
{}

// -------------------------------------------------------------------------- //

CInstruction::CInstruction(
  std::string_view const key,
  std::string_view const signature,
  FCallback const callback
) :
  key { key },
  signature { signature },
  callback { callback },
  next { sFirst }
{
  sFirst = this;
}

// -------------------------------------------------------------------------- //

CInstruction const *
CInstruction::Fetch(
  std::string_view const key,
  uint8_t * const bits
) {
  CInstruction const * it;

  for (it = sFirst; it != nullptr; it = it->next) {
    std::string_view rhs { it->key };

    if (bits != nullptr) {
      *bits = 0;
    }

    if (rhs.back() == '.') {
      if (key.back() != '.') {
        rhs = rhs.substr(0, (rhs.size() - 1));
      } else if (bits != nullptr) {
        *bits |= EBIT_RC;
      }
    }

    if (rhs.back() == 'o') {
      if (key.back() != 'o') {
        rhs = rhs.substr(0, (rhs.size() - 1));
      } else if (bits != nullptr) {
        *bits |= EBIT_OE;
      }
    }

    if (key == rhs) {
      return it;
    }
  }

  return nullptr;
}

// -------------------------------------------------------------------------- ///

// ========================================================================== //
