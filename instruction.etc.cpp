
// ========================================================================== //

// -------------------------------------------------------------------------- //
// miscellaneous instructions
// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>

#include "instruction.hpp"
#include "processor.hpp"
#include "span.hpp"

// -------------------------------------------------------------------------- //

static CInstruction sInst_mtctr {
  "mtctr", "{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto ra = size_t(args[0]);
    gPPC->ctr() = gPPC->gpr(ra).u32();
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_mfctr {
  "mfctr", "{RD:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rd = size_t(args[0]);
    gPPC->gpr(rd) = CGPR { gPPC->ctr() };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_mtlr {
  "mtlr", "{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto ra = size_t(args[0]);
    gPPC->lr() = gPPC->gpr(ra).u32();
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_mflr {
  "mflr", "{RD:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rd = size_t(args[0]);
    gPPC->gpr(rd) = CGPR { gPPC->lr() };
  }
};

// -------------------------------------------------------------------------- //

// ========================================================================== //
