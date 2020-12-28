
// ========================================================================== //

// -------------------------------------------------------------------------- //
// branching instructions
// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>
#include <optional>

#include "instruction.hpp"
#include "interpreter.hpp"
#include "processor.hpp"
#include "span.hpp"

// -------------------------------------------------------------------------- //

enum : uint8_t {

  LT,
  GT,
  EQ,

};

// -------------------------------------------------------------------------- //

void b(bool lk, std::optional<uint32_t> ll);
void bc(uint8_t bo, uint8_t bi, bool lk, std::optional<uint32_t> bd);

// -------------------------------------------------------------------------- //

static CInstruction sInst_b {
  "b", "{LL:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    b(false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_blr {
  "blr",
  [] (CSpanT<int32_t> args, uint8_t) {
    b(false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bctr {
  "bctr",
  [] (CSpanT<int32_t> args, uint8_t) {
    b(false, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bl {
  "bl", "{LL:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    b(true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_blrl {
  "blrl",
  [] (CSpanT<int32_t> args, uint8_t) {
    b(true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bctrl {
  "bctrl",
  [] (CSpanT<int32_t> args, uint8_t) {
    b(true, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_blt {
  "blt", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + LT), false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bltlr {
  "bltlr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + LT), false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bltctr {
  "bltctr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + LT), false, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bltl {
  "bltl", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + LT), true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bltlrl {
  "bltlrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + LT), true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bltctrl {
  "bltctrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + LT), true, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_ble {
  "ble", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_blelr {
  "blelr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_blectr {
  "blectr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), false, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_blel {
  "blel", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_blelrl {
  "blelrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_blectrl {
  "blectrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), true, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_beq {
  "beq", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + EQ), false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_beqlr {
  "beqlr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + EQ), false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_beqctr {
  "beqctr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + EQ), false, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_beql {
  "beql", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + EQ), true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_beqlrl {
  "beqlrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + EQ), true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_beqctrl {
  "beqctrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + EQ), true, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bge {
  "bge", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgelr {
  "bgelr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgectr {
  "bgectr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), false, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgel {
  "bgel", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgelrl {
  "bgelrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgectrl {
  "bgectrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), true, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgt {
  "bgt", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + GT), false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgtlr {
  "bgtlr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + GT), false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgtctr {
  "bgtctr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + GT), false, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgtl {
  "bgtl", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + GT), true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgtlrl {
  "bgtlrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + GT), true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bgtctrl {
  "bgtctrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b01100, (4 * cr + GT), true, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnl {
  "bnl", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnllr {
  "bnllr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnlctr {
  "bnlctr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), false, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnll {
  "bnll", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnllrl {
  "bnllrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnlctrl {
  "bnlctrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + LT), true, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bne {
  "bne", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + EQ), false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnelr {
  "bnelr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + EQ), false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnectr {
  "bnectr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + EQ), false, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnel {
  "bnel", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + EQ), true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnelrl {
  "bnelrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + EQ), true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnectrl {
  "bnectrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + EQ), true, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bng {
  "bng", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnglr {
  "bnglr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bngctr {
  "bngctr", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), false, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bngl {
  "bngl", "[{CR:cr},]{BD:addr}",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bnglrl {
  "bnglrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bngctrl {
  "bngctrl", "[{CR:cr}]",
  [] (CSpanT<int32_t> args, uint8_t) {
    uint8_t cr { 0 };

    if (!args.empty()) {
      cr = uint8_t(args[0]);
    }

    bc(0b00100, (4 * cr + GT), true, gPPC->ctr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bdz {
  "bdz", "{BD:addr}",
  [] (CSpanT<int32_t>, uint8_t) {
    bc(0b10010, 0, false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bdzl {
  "bdzl", "{BD:addr}",
  [] (CSpanT<int32_t>, uint8_t) {
    bc(0b10010, 0, true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bdnz {
  "bdnz", "{BD:addr}",
  [] (CSpanT<int32_t>, uint8_t) {
    bc(0b10000, 0, false, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bdnzl {
  "bdnzl", "{BD:addr}",
  [] (CSpanT<int32_t>, uint8_t) {
    bc(0b10000, 0, true, std::nullopt);
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bdzlr {
  "bdzlr",
  [] (CSpanT<int32_t>, uint8_t) {
    bc(0b10010, 0, false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bdzlrl {
  "bdzlrl",
  [] (CSpanT<int32_t>, uint8_t) {
    bc(0b10010, 0, true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bdnzlr {
  "bdnzlr",
  [] (CSpanT<int32_t>, uint8_t) {
    bc(0b10000, 0, false, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

static CInstruction sInst_bdnzlrl {
  "bdnzlrl",
  [] (CSpanT<int32_t>, uint8_t) {
    bc(0b10000, 0, true, gPPC->lr());
  }
};

// -------------------------------------------------------------------------- //

void b(
  bool lk,
  std::optional<uint32_t> ll
) {
  if (lk) {
    gPPC->lr() = static_cast<uint32_t>(
      gInterpreter->tell()
    );
  }

  if (ll == std::nullopt) {
    gInterpreter->branch();
  } else {
    gInterpreter->seek(*ll);
  }
}

// -------------------------------------------------------------------------- //

void bc(
  uint8_t bo,
  uint8_t bi,
  bool lk,
  std::optional<uint32_t> bd
) {
  // BO[0] BO[1] BO[2] BO[3] BO[4]
  // 0x10  0x08  0x04  0x02  0x01

  if (lk) {
    gPPC->lr() = static_cast<uint32_t>(
      gInterpreter->tell()
    );
  }

  if (!(bo & 0x04)) {
    --gPPC->ctr();

    if (!((gPPC->ctr() == 0) ^ ((bo & 0x02) == 0))) {
      return;
    }
  }

  if (!(bo & 0x10)) {
    uint8_t lhs { gPPC->cr(bi >> 2) };
    auto rhs = static_cast<uint8_t>(1 << (bi & 0x3));

    if (!(((lhs & rhs) != 0) ^ ((bo & 0x08) == 0))) {
      return;
    }
  }

  if (bd == std::nullopt) {
    gInterpreter->branch();
  } else {
    gInterpreter->seek(*bd);
  }
}

// -------------------------------------------------------------------------- //

// ========================================================================== //
