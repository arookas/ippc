
// ========================================================================== //

#ifndef INCLUDE_PROCESSOR_HPP
#define INCLUDE_PROCESSOR_HPP

// -------------------------------------------------------------------------- //

#include <cstddef>
#include <cstdint>

// -------------------------------------------------------------------------- //

enum ECR : uint8_t {

  ECR_LT = 0b00000001,
  ECR_GT = 0b00000010,
  ECR_EQ = 0b00000100,
  ECR_SO = 0b00001000,
  ECR_UN = ECR_SO,

};

enum EXER : uint8_t {

  EXER_S0 = 0b00000001,
  EXER_OV = 0b00000010,
  EXER_CA = 0b00000100,

};

enum EGQR : uint8_t {

  EGQR_F32 = 0,
  EGQR_U8  = 4,
  EGQR_U16 = 5,
  EGQR_S8  = 6,
  EGQR_S16 = 7,

};

// -------------------------------------------------------------------------- //

class CGPR {

  public:

  CGPR() = default;
  explicit CGPR(int32_t value);
  explicit CGPR(uint32_t value);

  int8_t s8() const;
  int16_t s16() const;
  int32_t s32() const;

  uint8_t u8() const;
  uint16_t u16() const;
  uint32_t u32() const;

  private:

  uint32_t mValue { 0 };

};

// -------------------------------------------------------------------------- //

class CFPR {

  public:

  CFPR();
  explicit CFPR(uint64_t u64);
  explicit CFPR(double f64);
  explicit CFPR(float f32);
  CFPR(float ps0, float ps1);

  uint64_t u64() const;
  double f64() const;
  float f32() const;
  float ps0() const;
  float ps1() const;

  private:

  union {

    uint64_t u64; // fctiwz
    double f64;

    struct {

      float ps0;
      float ps1;

    };

  } mValue;

};

// -------------------------------------------------------------------------- //

struct CGQR {

  EGQR stype { EGQR_F32 };
  int32_t sscale { 0 };
  EGQR ltype { EGQR_F32 };
  int32_t lscale { 0 };

};

// -------------------------------------------------------------------------- //

class CProcessor {

  public:

  CProcessor(size_t memory_size = { 24 * 1024 * 1024 });
  ~CProcessor();

  CGPR & gpr(size_t n);
  CGPR const & gpr(size_t n) const;

  CFPR & fpr(size_t n);
  CFPR const & fpr(size_t n) const;

  CGQR & gqr(size_t n);
  CGQR const & gqr(size_t n) const;

  uint32_t & ctr();
  uint32_t const & ctr() const;

  uint32_t & lr();
  uint32_t const & lr() const;

  uint8_t & cr(size_t n);
  uint8_t const & cr(size_t n) const;

  uint8_t & xer();
  uint8_t const & xer() const;

  size_t ea(int16_t d, size_t ra) const;
  size_t ea(size_t ra, size_t rb) const;

  uint8_t lbz(size_t addr) const;
  int16_t lha(size_t addr) const;
  uint16_t lhz(size_t addr) const;
  uint32_t lwz(size_t addr) const;
  float lfs(size_t addr) const;
  double lfd(size_t addr) const;

  void stb(size_t addr, uint8_t b);
  void sth(size_t addr, uint16_t h);
  void stw(size_t addr, uint32_t w);
  void stfs(size_t addr, float s);
  void stfd(size_t addr, double d);

  static uint32_t Mask(size_t mb, size_t me);
  static uint32_t Rot32(uint32_t value, size_t bits);
  static bool Carry(uint32_t lhs, uint32_t rhs);

  private:

  uint8_t * mMemory { nullptr };
  size_t mMemorySize { 0 };
  CGPR mGPR[32];
  CFPR mFPR[32];
  CGQR mGQR[8];
  uint32_t mCTR { 0 };
  uint32_t mLR { 0 };
  uint8_t mCR[8] { 0 };
  uint8_t mXER { 0 };

  uint8_t & ram(size_t addr);
  uint8_t const & ram(size_t addr) const;

};

// -------------------------------------------------------------------------- //

extern CProcessor * gPPC;

// -------------------------------------------------------------------------- //

// ========================================================================== //

#endif
