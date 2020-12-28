
// ========================================================================== //

// -------------------------------------------------------------------------- //
// fixed-point arithmetic instructions
// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>

#include "instruction.hpp"
#include "processor.hpp"
#include "span.hpp"

// -------------------------------------------------------------------------- //

void cmpwi(size_t bf, size_t ra, int16_t si);

// -------------------------------------------------------------------------- //

void addi(size_t rt, size_t ra, int16_t si, bool rc);
void addis(size_t rt, size_t ra, int16_t si, bool rc);
void addic(size_t rt, size_t ra, int16_t si, bool rc);
void subf(size_t rt, size_t rb, size_t ra, bool rc);

// -------------------------------------------------------------------------- //

static CInstruction sInst_add {
  "add.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      gPPC->gpr(ra).u32() + gPPC->gpr(rb).u32()
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_addi {
  "addi.", "{RT:gpr},{RA:gpr},{SI:si}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto si = int16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    addi(rt, ra, si, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_addis {
  "addis.", "{RT:gpr},{RA:gpr},{SI:si}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto si = int16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    addis(rt, ra, si, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_addic {
  "addic.", "{RT:gpr},{RA:gpr},{SI:si}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto si = int16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    addic(rt, ra, si, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_addze {
  "addze.", "{RT:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      gPPC->gpr(ra).u32() +
      ((gPPC->xer() & EXER_CA) ? 1Ui32 : 0Ui32)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_adde {
  "adde.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      gPPC->gpr(ra).u32() +
      gPPC->gpr(rb).u32() +
      ((gPPC->xer() & EXER_CA) ? 1Ui32 : 0Ui32)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_sub {
  "sub.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    subf(rt, rb, ra, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_subi {
  "subi.", "{RT:gpr},{RA:gpr},{SI:si}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto si = int16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    addi(rt, ra, -si, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_subis {
  "subis.", "{RT:gpr},{RA:gpr},{SI:si}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto si = int16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    addis(rt, ra, -si, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_subic {
  "subic.", "{RT:gpr},{RA:gpr},{SI:si}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto si = int16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    addic(rt, ra, -si, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_subf {
  "subf.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    subf(rt, ra, rb, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_subfc {
  "subfc.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    auto lhs = gPPC->gpr(rb).u32();
    auto rhs = gPPC->gpr(ra).u32();
    gPPC->gpr(rt) = CGPR { lhs - rhs };

    if (rhs == 0 || CProcessor::Carry(lhs, (0 - rhs))) {
      gPPC->xer() |= EXER_CA;
    } else {
      gPPC->xer() &= ~EXER_CA;
    }

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_subfic {
  "subfic.", "{RT:gpr},{RA:gpr},{SI:si}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto si = int16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      int32_t(si) - gPPC->gpr(ra).s32()
    };

    auto rhs = gPPC->gpr(ra).u32();

    if ((rhs == 0) || CProcessor::Carry((0 - rhs), uint32_t(si))
    ) {
      gPPC->xer() |= EXER_CA;
    } else {
      gPPC->xer() &= ~EXER_CA;
    }

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_subfe {
  "subfe.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    auto lhs = ~gPPC->gpr(ra).u32();
    auto rhs = gPPC->gpr(rb).u32();
    auto ca = ((gPPC->xer() & EXER_CA) ? 1Ui32 : 0Ui32);
    gPPC->gpr(rt) = CGPR { lhs + rhs + ca };

    if (
      CProcessor::Carry(lhs, rhs) ||
      CProcessor::Carry((lhs + rhs), ca)
    ) {
      gPPC->xer() |= EXER_CA;
    } else {
      gPPC->xer() &= ~EXER_CA;
    }

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_subfme {
  "subfme.", "{RT:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };
    auto lhs = gPPC->gpr(ra).u32();
    auto ca = ((gPPC->xer() & EXER_CA) ? 1Ui32 : 0Ui32);
    gPPC->gpr(rt) = CGPR { lhs + ca - 1 };

    if (CProcessor::Carry(~lhs, (ca - 1))) {
      gPPC->xer() |= EXER_CA;
    } else {
      gPPC->xer() &= ~EXER_CA;
    }

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_subfze {
  "subfze.", "{RT:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };
    auto lhs = gPPC->gpr(ra).u32();
    auto ca = ((gPPC->xer() & EXER_CA) ? 1Ui32 : 0Ui32);
    gPPC->gpr(rt) = CGPR { lhs + ca };

    if (CProcessor::Carry(~lhs, ca)) {
      gPPC->xer() |= EXER_CA;
    } else {
      gPPC->xer() &= ~EXER_CA;
    }

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_mullw {
  "mullw.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    auto lhs = int64_t(gPPC->gpr(ra).s32());
    auto rhs = int64_t(gPPC->gpr(rb).s32());

    gPPC->gpr(rt) = CGPR {
      uint32_t((lhs * rhs) & 0xFFFFFFFFUi32)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_mulhw {
  "mulhw.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    auto lhs = int64_t(gPPC->gpr(ra).s32());
    auto rhs = int64_t(gPPC->gpr(rb).s32());

    gPPC->gpr(rt) = CGPR {
      uint32_t(((lhs * rhs) >> 32) & 0xFFFFFFFFUi32)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_mullwu {
  "mullwu.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    auto lhs = uint64_t(gPPC->gpr(ra).u32());
    auto rhs = uint64_t(gPPC->gpr(rb).u32());

    gPPC->gpr(rt) = CGPR {
      uint32_t((lhs * rhs) & 0xFFFFFFFFUi32)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_mulhwu {
  "mulhwu.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    auto lhs = uint64_t(gPPC->gpr(ra).u32());
    auto rhs = uint64_t(gPPC->gpr(rb).u32());

    gPPC->gpr(rt) = CGPR {
      uint32_t(((lhs * rhs) >> 32) & 0xFFFFFFFFUi32)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_mulli {
  "mulli.", "{RT:gpr},{RA:gpr},{SI:si}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto si = int16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      uint32_t((int64_t(gPPC->gpr(ra).s32()) * si) & 0xFFFFFFFFUi32)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_divw {
  "divw.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      gPPC->gpr(ra).s32() / gPPC->gpr(rb).s32()
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_divwu {
  "divwu.", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      gPPC->gpr(ra).u32() / gPPC->gpr(rb).u32()
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_abs {
  "abs.", "{RT:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      std::abs(gPPC->gpr(ra).s32())
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_nabs {
  "nabs.", "{RT:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      -std::abs(gPPC->gpr(ra).s32())
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_neg {
  "neg.", "{RT:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      uint32_t(~gPPC->gpr(ra).u32() + 1)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

void addi(
  size_t rt,
  size_t ra,
  int16_t si,
  bool rc
) {
  if (ra != 0) {
    gPPC->gpr(rt) = CGPR {
      int32_t(gPPC->gpr(ra).s32() + si)
    };
  } else {
    gPPC->gpr(rt) = CGPR { int32_t(si) };
  }

  if (rc) {
    cmpwi(0, rt, 0);
  }
}

// -------------------------------------------------------------------------- //

void addis(
  size_t rt,
  size_t ra,
  int16_t si,
  bool rc
) {
  int32_t simm { int32_t(si) << 16 };

  if (ra != 0) {
    gPPC->gpr(rt) = CGPR {
      int32_t(gPPC->gpr(ra).s32() + simm)
    };
  } else {
    gPPC->gpr(rt) = CGPR { simm };
  }

  if (rc) {
    cmpwi(0, rt, 0);
  }
}

// -------------------------------------------------------------------------- //

void addic(
  size_t rt,
  size_t ra,
  int16_t si,
  bool rc
) {
  auto lhs = gPPC->gpr(ra).u32();
  auto rhs = (uint32_t)(int32_t)si;

  gPPC->gpr(rt) = CGPR {
    uint32_t(lhs + rhs)
  };

  if (CProcessor::Carry(lhs, rhs)) {
    gPPC->xer() |= EXER_CA;
  } else {
    gPPC->xer() &= ~EXER_CA;
  }

  if (rc) {
    cmpwi(0, rt, 0);
  }
}

// -------------------------------------------------------------------------- //

void subf(
  size_t rt,
  size_t ra,
  size_t rb,
  bool rc
) {
  gPPC->gpr(rt) = CGPR {
    gPPC->gpr(rb).s32() - gPPC->gpr(ra).s32()
  };

  if (rc) {
    cmpwi(0, rt, 0);
  }
}

// -------------------------------------------------------------------------- //

// ========================================================================== //
