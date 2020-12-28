
// ========================================================================== //

// -------------------------------------------------------------------------- //
// bitwise shift/rotate instructions
// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>

#include "instruction.hpp"
#include "processor.hpp"
#include "span.hpp"

// -------------------------------------------------------------------------- //

void cmpwi(size_t bf, size_t ra, int16_t si);

// -------------------------------------------------------------------------- //

void rlwinm(size_t ra, size_t rs, size_t sh, size_t mb, size_t me, bool rc);
void rlwnm(size_t ra, size_t rs, size_t rb, size_t mb, size_t me, bool rc);
void rlwimi(size_t ra, size_t rs, size_t sh, size_t mb, size_t me, bool rc);

// -------------------------------------------------------------------------- //

static CInstruction sInst_extsb {
  "extsb.", "{RT:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      int8_t(gPPC->gpr(ra).u32() & 0xFF)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_extsh {
  "extsh.", "{RT:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(rt) = CGPR {
      int16_t(gPPC->gpr(ra).u32() & 0xFFFF)
    };

    if (rc) {
      cmpwi(0, rt, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_cntlzw {
  "cntlzw.", "{RS:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rs = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    uint32_t count { 0 };

    for (int i = 0; i < 32; ++i, ++count) {
      if (gPPC->gpr(ra).u32() & (0x80000000Ui32 >> i)) {
        break;
      }
    }

    gPPC->gpr(rs) = CGPR { count };

    if (rc) {
      cmpwi(0, rs, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_and {
  "and.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() & gPPC->gpr(rb).u32()
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_andc {
  "andc.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() & ~gPPC->gpr(rb).u32()
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_andi {
  "andi.", "{RA:gpr},{RS:gpr},{UI:ui}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto ui = uint16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() & ui
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_andis {
  "andis.", "{RA:gpr},{RS:gpr},{UI:ui}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto ui = uint16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() & uint32_t(ui << 16)
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_or {
  "or.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() | gPPC->gpr(rb).u32()
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_orc {
  "orc.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() | ~gPPC->gpr(rb).u32()
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_ori {
  "ori.", "{RA:gpr},{RS:gpr},{UI:ui}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto ui = uint16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() | ui
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_oris {
  "oris.", "{RA:gpr},{RS:gpr},{UI:ui}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto ui = uint16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() | uint32_t(ui << 16)
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_xor {
  "xor.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() ^ gPPC->gpr(rb).u32()
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_xori {
  "xori.", "{RA:gpr},{RS:gpr},{UI:ui}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto ui = uint16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() ^ ui
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_xoris {
  "xoris.", "{RA:gpr},{RS:gpr},{UI:ui}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto ui = uint16_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).u32() ^ uint32_t(ui << 16)
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_eqv {
  "eqv.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      ~(gPPC->gpr(rs).u32() ^ gPPC->gpr(rb).u32())
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_nand {
  "nand.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      ~(gPPC->gpr(rs).u32() & gPPC->gpr(rb).u32())
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_nor {
  "nor.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      ~(gPPC->gpr(rs).u32() | gPPC->gpr(rb).u32())
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_rlwinm {
  "rlwinm.", "{RA:gpr},{RS:gpr},{SH:bit},{MB:bit},{ME:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto sh = size_t(args[2]);
    auto mb = size_t(args[3]);
    auto me = size_t(args[4]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, sh, mb, me, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_rlwnm {
  "rlwnm.", "{RA:gpr},{RS:gpr},{RB:gpr},{MB:bit},{ME:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    auto mb = size_t(args[3]);
    auto me = size_t(args[4]);
    bool rc { !!(bits & EBIT_RC) };
    rlwnm(ra, rs, rb, mb, me, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_rlwimi {
  "rlwimi.", "{RA:gpr},{RS:gpr},{SH:bit},{MB:bit},{ME:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto sh = size_t(args[2]);
    auto mb = size_t(args[3]);
    auto me = size_t(args[4]);
    bool rc { !!(bits & EBIT_RC) };
    auto result = CProcessor::Rot32(gPPC->gpr(rs).u32(), sh);
    auto mask = CProcessor::Mask(mb, me);

    gPPC->gpr(ra) = CGPR {
      (result & mask) | (gPPC->gpr(ra).u32() & ~mask)
    };

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_extlwi {
  "extlwi.", "{RA:gpr},{RS:gpr},{N:bit},{B:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    auto b = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, b, 0, (n - 1), rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_extrwi {
  "extrwi.", "{RA:gpr},{RS:gpr},{N:bit},{B:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    auto b = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, (b + n), (32 - n), 31, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_inslwi {
  "inslwi.", "{RA:gpr},{RS:gpr},{N:bit},{B:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    auto b = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };
    rlwimi(ra, rs, (32 - b), b, ((b + n) - 1), rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_insrwi {
  "insrwi.", "{RA:gpr},{RS:gpr},{N:bit},{B:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    auto b = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };
    rlwimi(ra, rs, (32 - (b + n)), b, ((b + n) - 1), rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_rotlwi {
  "rotlwi.", "{RA:gpr},{RS:gpr},{N:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, n, 0, 31, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_rotrwi {
  "rotrwi.", "{RA:gpr},{RS:gpr},{N:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, (32 - n), 0, 31, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_rotlw {
  "rotlw.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    rlwnm(ra, rs, rb, 0, 31, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_clrlwi {
  "clrlwi.", "{RA:gpr},{RS:gpr},{N:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, 0, n, 31, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_clrrwi {
  "clrrwi.", "{RA:gpr},{RS:gpr},{N:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, 0, 0, (31 - n), rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_clrlslwi {
  "clrlslwi.", "{RA:gpr},{RS:gpr},{B:bit},{N:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto b = size_t(args[2]);
    auto n = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, n, (b - n), (31 - n), rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_slw {
  "slw.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    if ((gPPC->gpr(rb).u32() & 0x20) == 0) {
      gPPC->gpr(ra) = CGPR {
        gPPC->gpr(rs).u32() << (gPPC->gpr(rb).u32() & 0x1F)
      };
    } else {
      gPPC->gpr(ra) = CGPR { uint32_t(0) };
    }

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_slwi {
  "slwi.", "{RA:gpr},{RS:gpr},{N:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, n, 0, (31 - n), rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_srw {
  "srw.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    if ((gPPC->gpr(rb).u32() & 0x20) == 0) {
      gPPC->gpr(ra) = CGPR {
        gPPC->gpr(rs).u32() >> (gPPC->gpr(rb).u32() & 0x1F)
      };
    } else {
      gPPC->gpr(ra) = CGPR { uint32_t(0) };
    }

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_srwi {
  "srwi.", "{RA:gpr},{RS:gpr},{N:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto n = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };
    rlwinm(ra, rs, (32 - n), n, 31, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_sraw {
  "sraw.", "{RA:gpr},{RS:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto rb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    if ((gPPC->gpr(rb).u32() & 0x20) != 0) {
      if ((gPPC->gpr(rs).u32() & 0x80000000Ui32) != 0) {
        gPPC->gpr(ra) = CGPR { 0xFFFFFFFFUi32 };
        gPPC->xer() |= EXER_CA;
      } else {
        gPPC->gpr(ra) = CGPR { 0Ui32 };
        gPPC->xer() &= ~EXER_CA;
      }
    } else {
      auto sh = (gPPC->gpr(rb).u32() & 0x1F);

      gPPC->gpr(ra) = CGPR {
        gPPC->gpr(rs).s32() >> sh
      };

      if (
        (gPPC->gpr(rs).s32() < 0) &&
        (sh > 0) &&
        ((gPPC->gpr(rs).u32() << (32 - sh)) != 0)
      ) {
        gPPC->xer() |= EXER_CA;
      } else {
        gPPC->xer() &= ~EXER_CA;
      }
    }

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_srawi {
  "srawi.", "{RA:gpr},{RS:gpr},{SH:bit}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto ra = size_t(args[0]);
    auto rs = size_t(args[1]);
    auto sh = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->gpr(ra) = CGPR {
      gPPC->gpr(rs).s32() >> sh
    };

    if (
      (gPPC->gpr(rs).s32() < 0) &&
      (sh > 0) &&
      ((gPPC->gpr(rs).u32() << (32 - sh)) != 0)
    ) {
      gPPC->xer() |= EXER_CA;
    } else {
      gPPC->xer() &= ~EXER_CA;
    }

    if (rc) {
      cmpwi(0, ra, 0);
    }
  }
};

// -------------------------------------------------------------------------- //

void rlwinm(
  size_t ra,
  size_t rs,
  size_t sh,
  size_t mb,
  size_t me,
  bool rc
) {
  auto result = CProcessor::Rot32(gPPC->gpr(rs).u32(), sh);
  auto mask = CProcessor::Mask(mb, me);
  gPPC->gpr(ra) = CGPR { result & mask };

  if (rc) {
    cmpwi(0, ra, 0);
  }
}

// -------------------------------------------------------------------------- //

void rlwnm(
  size_t ra,
  size_t rs,
  size_t rb,
  size_t mb,
  size_t me,
  bool rc
) {
  auto sh = size_t(gPPC->gpr(rb).u32() & 0x1F);
  rlwinm(ra, rs, sh, mb, me, rc);
}

// -------------------------------------------------------------------------- //

void rlwimi(
  size_t ra,
  size_t rs,
  size_t sh,
  size_t mb,
  size_t me,
  bool rc
) {
  auto result = CProcessor::Rot32(gPPC->gpr(rs).u32(), sh);
  auto mask = CProcessor::Mask(mb, me);

  gPPC->gpr(ra) = CGPR {
    (result & mask) | (gPPC->gpr(ra).u32() & ~mask)
  };

  if (rc) {
    cmpwi(0, ra, 0);
  }
}

// -------------------------------------------------------------------------- //

// ========================================================================== //
