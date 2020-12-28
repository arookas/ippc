
// ========================================================================== //

// -------------------------------------------------------------------------- //
// fixed-point load/store instructions
// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>

#include "instruction.hpp"
#include "processor.hpp"
#include "span.hpp"

// -------------------------------------------------------------------------- //

void addi(size_t rt, size_t ra, int16_t si, bool rc);
void addis(size_t rt, size_t ra, int16_t si, bool rc);

// -------------------------------------------------------------------------- //

static CInstruction li {
  "li", "{RT:gpr},{SIMM:si}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto si = int16_t(args[1]);
    addi(rt, 0, si, false);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lis {
  "lis", "{RT:gpr},{SIMM:si}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto si = int16_t(args[1]);
    addis(rt, 0, si, false);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_mr {
  "mr.", "{RT:gpr},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };
    addi(rt, ra, 0, rc);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lbz {
  "lbz", "{RT:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    gPPC->gpr(rt) = CGPR {
      gPPC->lbz(gPPC->ea(d, ra))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lbzx {
  "lbzx", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    gPPC->gpr(rt) = CGPR {
      gPPC->lbz(gPPC->ea(ra, rb))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lbzu {
  "lbzu", "{RT:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->gpr(rt) = CGPR { gPPC->lbz(ea) };

    if (ra != rt && ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lbzux {
  "lbzux", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->gpr(rt) = CGPR { gPPC->lbz(ea) };

    if (ra != rt && ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lhz {
  "lhz", "{RT:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    gPPC->gpr(rt) = CGPR {
      gPPC->lhz(gPPC->ea(d, ra))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lhzx {
  "lhzx", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    gPPC->gpr(rt) = CGPR {
      gPPC->lhz(gPPC->ea(ra, rb))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lhzu {
  "lhzu", "{RT:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->gpr(rt) = CGPR { gPPC->lhz(ea) };

    if (ra != rt && ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lhzux {
  "lhzux", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->gpr(rt) = CGPR { gPPC->lhz(ea) };

    if (ra != rt && ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lwz {
  "lwz", "{RT:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    gPPC->gpr(rt) = CGPR {
      gPPC->lwz(gPPC->ea(d, ra))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lwzx {
  "lwzx", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    gPPC->gpr(rt) = CGPR {
      gPPC->lwz(gPPC->ea(ra, rb))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lwzu {
  "lwzu", "{RT:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->gpr(rt) = CGPR { gPPC->lwz(ea) };

    if (ra != rt && ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lwzux {
  "lwzux", "{RT:gpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->gpr(rt) = CGPR { gPPC->lwz(ea) };

    if (ra != rt && ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lmw {
  "lmw", "{RT:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);
    size_t ea { gPPC->ea(d, ra) };

    for (size_t i { 0 }; i < (32 - rt); ++i, ea += 4) {
      gPPC->gpr(rt + i) = CGPR { gPPC->lwz(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stb {
  "stb", "{RS:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);
    gPPC->stb(gPPC->ea(d, ra), gPPC->gpr(rs).u8());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stbu {
  "stbu", "{RS:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->stb(ea, gPPC->gpr(rs).u8());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stbx {
  "stbx", "{RS:gpr},{RA:gpr}({RB:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    gPPC->stb(gPPC->ea(ra, rb), gPPC->gpr(rs).u8());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stbux {
  "stbux", "{RS:gpr},{RA:gpr}({RB:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->stb(gPPC->ea(ra, rb), gPPC->gpr(rs).u8());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_sth {
  "sth", "{RS:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);
    gPPC->sth(gPPC->ea(d, ra), gPPC->gpr(rs).u16());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_sthu {
  "sthu", "{RS:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->sth(ea, gPPC->gpr(rs).u16());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_sthx {
  "sthx", "{RS:gpr},{RA:gpr}({RB:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    gPPC->sth(gPPC->ea(ra, rb), gPPC->gpr(rs).u16());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_sthux {
  "sthux", "{RS:gpr},{RA:gpr}({RB:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->sth(gPPC->ea(ra, rb), gPPC->gpr(rs).u16());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stw {
  "stw", "{RS:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);
    gPPC->stw(gPPC->ea(d, ra), gPPC->gpr(rs).u32());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stwu {
  "stwu", "{RS:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->stw(ea, gPPC->gpr(rs).u32());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stwx {
  "stwx", "{RS:gpr},{RA:gpr}({RB:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    gPPC->stw(gPPC->ea(ra, rb), gPPC->gpr(rs).u32());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stwux {
  "stwux", "{RS:gpr},{RA:gpr}({RB:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->stw(gPPC->ea(ra, rb), gPPC->gpr(rs).u32());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stmw {
  "stmw", "{RS:gpr},{D:si}({RA:gpr})",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto rs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);
    size_t ea { gPPC->ea(d, ra) };

    for (size_t i { 0 }; i < (32 - rs); ++i) {
      gPPC->stw(ea, gPPC->gpr(rs + i).u32());
      ea += 4;
    }
  }
};

// -------------------------------------------------------------------------- //

// ========================================================================== //
