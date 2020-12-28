
// ========================================================================== //

// -------------------------------------------------------------------------- //
// floating-point arithmetic instructions
// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>

#include "instruction.hpp"
#include "processor.hpp"
#include "span.hpp"

// -------------------------------------------------------------------------- //

static CInstruction sInst_fadds {
  "fadds.", "{FRT:fpr},{FRA:fpr},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      static_cast<float>(
        gPPC->fpr(fra).f64() + gPPC->fpr(frb).f64()
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fsubs {
  "fsubs.", "{FRT:fpr},{FRA:fpr},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      static_cast<float>(
        gPPC->fpr(fra).f64() - gPPC->fpr(frb).f64()
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fmuls {
  "fmuls.", "{FRT:fpr},{FRA:fpr},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      static_cast<float>(
        gPPC->fpr(fra).f64() * gPPC->fpr(frb).f64()
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fdivs {
  "fdivs.", "{FRT:fpr},{FRA:fpr},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      static_cast<float>(
        gPPC->fpr(fra).f64() / gPPC->fpr(frb).f64()
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fmadds {
  "fmadds.", "{FRT:fpr},{FRA:fpr},{FRC:frb},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frc = size_t(args[2]);
    auto frb = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      static_cast<float>(
        gPPC->fpr(fra).f64() *
        gPPC->fpr(frc).f64() +
        gPPC->fpr(frb).f64()
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fmsubs {
  "fmsubs.", "{FRT:fpr},{FRA:fpr},{FRC:frb},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frc = size_t(args[2]);
    auto frb = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      static_cast<float>(
        gPPC->fpr(fra).f64() *
        gPPC->fpr(frc).f64() -
        gPPC->fpr(frb).f64()
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fnmadds {
  "fnmadds.", "{FRT:fpr},{FRA:fpr},{FRC:frb},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frc = size_t(args[2]);
    auto frb = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      static_cast<float>(-(
        gPPC->fpr(fra).f64() *
        gPPC->fpr(frc).f64() +
        gPPC->fpr(frb).f64()
      ))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fnmsubs {
  "fnmsubs.", "{FRT:fpr},{FRA:fpr},{FRC:frb},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frc = size_t(args[2]);
    auto frb = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      static_cast<float>(-(
        gPPC->fpr(fra).f64() *
        gPPC->fpr(frc).f64() -
        gPPC->fpr(frb).f64()
      ))
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fsqrts {
  "fsqrts.", "{FRT:fpr},{FRB:fpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto frb = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      static_cast<float>(
        std::sqrt(gPPC->fpr(frb).f64())
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fadd {
  "fadd.", "{FRT:fpr},{FRA:fpr},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      gPPC->fpr(fra).f64() + gPPC->fpr(frb).f64()
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fsub {
  "fsub.", "{FRT:fpr},{FRA:fpr},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      gPPC->fpr(fra).f64() - gPPC->fpr(frb).f64()
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fmul {
  "fmul.", "{FRT:fpr},{FRA:fpr},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      gPPC->fpr(fra).f64() * gPPC->fpr(frb).f64()
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fdiv {
  "fdiv.", "{FRT:fpr},{FRA:fpr},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frb = size_t(args[2]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      gPPC->fpr(fra).f64() / gPPC->fpr(frb).f64()
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fmadd {
  "fmadd.", "{FRT:fpr},{FRA:fpr},{FRC:frb},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frc = size_t(args[2]);
    auto frb = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      gPPC->fpr(fra).f64() *
      gPPC->fpr(frc).f64() +
      gPPC->fpr(frb).f64()
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fmsub {
  "fmsub.", "{FRT:fpr},{FRA:fpr},{FRC:frb},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frc = size_t(args[2]);
    auto frb = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      gPPC->fpr(fra).f64() *
      gPPC->fpr(frc).f64() -
      gPPC->fpr(frb).f64()
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fnmadd {
  "fnmadd.", "{FRT:fpr},{FRA:fpr},{FRC:frb},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frc = size_t(args[2]);
    auto frb = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      -(
        gPPC->fpr(fra).f64() *
        gPPC->fpr(frc).f64() +
        gPPC->fpr(frb).f64()
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fnmsub {
  "fnmsub.", "{FRT:fpr},{FRA:fpr},{FRC:frb},{FRB:frb}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto fra = size_t(args[1]);
    auto frc = size_t(args[2]);
    auto frb = size_t(args[3]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      -(
        gPPC->fpr(fra).f64() *
        gPPC->fpr(frc).f64() -
        gPPC->fpr(frb).f64()
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fsqrt {
  "fsqrt.", "{FRT:fpr},{FRB:fpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto frb = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      std::sqrt(gPPC->fpr(frb).f64())
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fabs {
  "fabs.", "{FRT:fpr},{FRB:fpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto frb = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      uint64_t(
        gPPC->fpr(frb).u64() & ~0x80000000'00000000Ui64
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fnabs {
  "fnabs.", "{FRT:fpr},{FRB:fpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto frb = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      uint64_t(
        gPPC->fpr(frb).u64() | 0x80000000'00000000Ui64
      )
    };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fneg {
  "fneg.", "{FRT:fpr},{FRB:fpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto frb = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };
    gPPC->fpr(frt) = CFPR { -gPPC->fpr(frb).f64() };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_fres {
  "fres.", "{FRT:fpr},{FRB:fpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto frb = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };
    gPPC->fpr(frt) = CFPR { 1.0F / gPPC->fpr(frb).f32() };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_frsp {
  "frsp.", "{FRT:fpr},{FRB:fpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto frb = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };
    gPPC->fpr(frt) = CFPR { gPPC->fpr(frb).f32() };
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_frsqrte {
  "frsqrte.", "{FRT:fpr},{FRB:fpr}",
  [] (CSpanT<int32_t> args, uint8_t bits) {
    auto frt = size_t(args[0]);
    auto frb = size_t(args[1]);
    bool rc { !!(bits & EBIT_RC) };

    gPPC->fpr(frt) = CFPR {
      1.0F / std::sqrt(gPPC->fpr(frb).f64())
    };
  }
};

// -------------------------------------------------------------------------- //

// ========================================================================== //
