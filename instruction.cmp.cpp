
// ========================================================================== //

// -------------------------------------------------------------------------- //
// compare instructions
// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>

#include "instruction.hpp"
#include "processor.hpp"
#include "span.hpp"

// -------------------------------------------------------------------------- //

void cmpwi(size_t bf, size_t ra, int16_t si);

// -------------------------------------------------------------------------- //

static CInstruction sInst_cmpw {
  "cmpw", "[{BF:cr},]{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto ra = size_t(args[args.size() - 2]);
    auto rb = size_t(args[args.size() - 1]);
    size_t bf { 0 };

    if (args.size() > 2) {
      bf = size_t(args[0]);
    }

    int32_t lhs { gPPC->gpr(ra).s32() };
    int32_t rhs { gPPC->gpr(rb).s32() };
    uint8_t cr { 0 };

    if (lhs < rhs) {
      cr |= ECR_LT;
    }

    if (lhs > rhs) {
      cr |= ECR_GT;
    }

    if (lhs == rhs) {
      cr |= ECR_EQ;
    }

    gPPC->cr(bf) = cr;
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_cmpwi {
  "cmpwi", "[{BF:cr},]{RA:gpr},{SIMM:si}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto ra = size_t(args[args.size() - 2]);
    auto si = int16_t(args[args.size() - 1]);
    size_t bf { 0 };

    if (args.size() > 2) {
      bf = size_t(args[0]);
    }

    cmpwi(bf, ra, si);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_cmplw {
  "cmplw", "[{BF:cr},]{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto ra = size_t(args[args.size() - 2]);
    auto rb = size_t(args[args.size() - 1]);
    size_t bf { 0 };

    if (args.size() > 2) {
      bf = size_t(args[0]);
    }

    uint32_t lhs { gPPC->gpr(ra).u32() };
    uint32_t rhs { gPPC->gpr(rb).u32() };
    uint8_t cr { 0 };

    if (lhs < rhs) {
      cr |= ECR_LT;
    }

    if (lhs > rhs) {
      cr |= ECR_GT;
    }

    if (lhs == rhs) {
      cr |= ECR_EQ;
    }

    gPPC->cr(bf) = cr;
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_cmplwi {
  "cmplwi", "[{BF:cr},]{RA:gpr},{UIMM:ui}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto ra = size_t(args[args.size() - 2]);
    auto rb = size_t(args[args.size() - 1]);
    size_t bf { 0 };

    if (args.size() > 2) {
      bf = size_t(args[0]);
    }

    uint32_t lhs { gPPC->gpr(ra).u32() };
    uint32_t rhs { gPPC->gpr(rb).u32() };
    uint8_t cr { 0 };

    if (lhs < rhs) {
      cr |= ECR_LT;
    }

    if (lhs > rhs) {
      cr |= ECR_GT;
    }

    if (lhs == rhs) {
      cr |= ECR_EQ;
    }

    gPPC->cr(bf) = cr;
  }
};

// -------------------------------------------------------------------------- //

void cmpwi(
  size_t bf,
  size_t ra,
  int16_t si
) {
  uint8_t cr { 0 };
  int32_t lhs { gPPC->gpr(ra).s32() };

  if (lhs < si) {
    cr |= ECR_LT;
  }

  if (lhs > si) {
    cr |= ECR_GT;
  }

  if (lhs == si) {
    cr |= ECR_EQ;
  }

  gPPC->cr(bf) = cr;
}

// -------------------------------------------------------------------------- //

// ========================================================================== //
