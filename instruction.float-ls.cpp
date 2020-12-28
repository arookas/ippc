
// ========================================================================== //

// -------------------------------------------------------------------------- //
// floating-point load/store instructions
// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>

#include "instruction.hpp"
#include "processor.hpp"
#include "span.hpp"

// -------------------------------------------------------------------------- //

static CInstruction sInst_lfs {
  "lfs", "{FRT:fpr},{D:si},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    gPPC->fpr(frt) = CFPR {
      gPPC->lfs(gPPC->ea(d, ra))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lfsx {
  "lfsx", "{FRT:fpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    gPPC->fpr(frt) = CFPR {
      gPPC->lfs(gPPC->ea(ra, rb))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lfsu {
  "lfsu", "{FRT:fpr},{D:si},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->fpr(frt) = CFPR { gPPC->lfs(ea) };

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lfsux {
  "lfsux", "{FRT:fpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->fpr(frt) = CFPR { gPPC->lfs(ea) };

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lfd {
  "lfd", "{FRT:fpr},{D:si},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    gPPC->fpr(frt) = CFPR {
      gPPC->lfd(gPPC->ea(d, ra))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lfdx {
  "lfdx", "{FRT:fpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    gPPC->fpr(frt) = CFPR {
      gPPC->lfd(gPPC->ea(ra, rb))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lfdu {
  "lfdu", "{FRT:fpr},{D:si},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frt = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->fpr(frt) = CFPR { gPPC->lfd(ea) };

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_lfdux {
  "lfdux", "{FRT:fpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frt = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->fpr(frt) = CFPR { gPPC->lfd(ea) };

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stfs {
  "stfs", "{FRS:fpr},{D:si},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);
    gPPC->stfs(gPPC->ea(d, ra), gPPC->fpr(frs).f32());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stfsx {
  "stfsx", "{FRS:fpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    gPPC->stfs(gPPC->ea(ra, rb), gPPC->fpr(frs).f32());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stfsu {
  "stfsu", "{FRS:fpr},{D:si},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->stfs(ea, gPPC->fpr(frs).f32());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stfsux {
  "stfsux", "{FRS:fpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->stfs(ea, gPPC->fpr(frs).f32());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stfd {
  "stfd", "{FRS:fpr},{D:si},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);
    gPPC->stfd(gPPC->ea(d, ra), gPPC->fpr(frs).f64());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stfdx {
  "stfdx", "{FRS:fpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);
    gPPC->stfd(gPPC->ea(ra, rb), gPPC->fpr(frs).f64());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stfdu {
  "stfdu", "{FRS:fpr},{D:si},{RA:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frs = size_t(args[0]);
    auto d = int16_t(args[1]);
    auto ra = size_t(args[2]);

    size_t ea { gPPC->ea(d, ra) };
    gPPC->stfd(ea, gPPC->fpr(frs).f64());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_stfdux {
  "stfdux", "{FRS:fpr},{RA:gpr},{RB:gpr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    auto frs = size_t(args[0]);
    auto ra = size_t(args[1]);
    auto rb = size_t(args[2]);

    size_t ea { gPPC->ea(ra, rb) };
    gPPC->stfd(ea, gPPC->fpr(frs).f64());

    if (ra != 0) {
      gPPC->gpr(ra) = CGPR { uint32_t(ea) };
    }
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fmr {
  "fmr.", "{FRT:fpr},{FRB:fpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto frb = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };
    gPPC->fpr(frt) = gPPC->fpr(frb);
    // TODO: rc
  }
};

// -------------------------------------------------------------------------- //

// ========================================================================== //
