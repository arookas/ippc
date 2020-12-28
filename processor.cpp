
// ========================================================================== //

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>

#include "processor.hpp"

// -------------------------------------------------------------------------- //

CProcessor * gPPC { nullptr };

// -------------------------------------------------------------------------- //

CGPR::CGPR(
  int32_t const value
) :
  mValue { static_cast<uint32_t>(value) }
{ }

// -------------------------------------------------------------------------- //

CGPR::CGPR(
  uint32_t const value
) :
  mValue { value }
{ }

// -------------------------------------------------------------------------- //

int8_t CGPR::s8() const {
  return static_cast<int8_t>(mValue & 0xFF);
}

// -------------------------------------------------------------------------- //

int16_t CGPR::s16() const {
  return static_cast<int16_t>(mValue & 0xFFFF);
}

// -------------------------------------------------------------------------- //

int32_t CGPR::s32() const {
  return static_cast<int32_t>(mValue);
}

// -------------------------------------------------------------------------- //

uint8_t CGPR::u8() const {
  return static_cast<uint8_t>(mValue & 0xFF);
}

// -------------------------------------------------------------------------- //

uint16_t CGPR::u16() const {
  return static_cast<uint16_t>(mValue & 0xFFFF);
}

// -------------------------------------------------------------------------- //

uint32_t CGPR::u32() const {
  return mValue;
}

// -------------------------------------------------------------------------- //

CFPR::CFPR() {
  mValue.u64 = 0;
}

// -------------------------------------------------------------------------- //

CFPR::CFPR(
  uint64_t const u64
) {
  mValue.u64 = u64;
}

// -------------------------------------------------------------------------- //

CFPR::CFPR(
  double const f64
) {
  mValue.f64 = f64;
}

// -------------------------------------------------------------------------- //

CFPR::CFPR(
  float const f32
) {
  mValue.f64 = f32;
}

// -------------------------------------------------------------------------- //

CFPR::CFPR(
  float const ps0,
  float const ps1
) {
  mValue.ps0 = ps0;
  mValue.ps1 = ps1;
}

// -------------------------------------------------------------------------- //

uint64_t CFPR::u64() const {
  return mValue.u64;
}

// -------------------------------------------------------------------------- //

double CFPR::f64() const {
  return mValue.f64;
}

// -------------------------------------------------------------------------- //

float CFPR::f32() const {
  return static_cast<float>(mValue.f64);
}

// -------------------------------------------------------------------------- //

float CFPR::ps0() const {
  return mValue.ps0;
}

// -------------------------------------------------------------------------- //

float CFPR::ps1() const {
  return mValue.ps1;
}

// -------------------------------------------------------------------------- //

CProcessor::CProcessor(
  size_t const memory_size
) {
  if (memory_size > 0) {
    mMemory = reinterpret_cast<uint8_t *>(
      operator new(memory_size)
    );

    mMemorySize = memory_size;
    std::memset(mMemory, 0, mMemorySize);
  }
}

// -------------------------------------------------------------------------- //

CProcessor::~CProcessor() {
  operator delete(mMemory);
}

// -------------------------------------------------------------------------- //

CGPR &
CProcessor::gpr(
  size_t const n
) {
  return mGPR[n];
}

// -------------------------------------------------------------------------- //

CGPR const &
CProcessor::gpr(
  size_t const n
) const {
  return mGPR[n];
}

// -------------------------------------------------------------------------- //

CFPR &
CProcessor::fpr(
  size_t const n
) {
  return mFPR[n];
}

// -------------------------------------------------------------------------- //

CFPR const &
CProcessor::fpr(
  size_t const n
) const {
  return mFPR[n];
}

// -------------------------------------------------------------------------- //

CGQR &
CProcessor::gqr(
  size_t const n
) {
  return mGQR[n];
}

// -------------------------------------------------------------------------- //

CGQR const &
CProcessor::gqr(
  size_t const n
) const {
  return mGQR[n];
}

// -------------------------------------------------------------------------- //

uint32_t &
CProcessor::ctr() {
  return mCTR;
}

// -------------------------------------------------------------------------- //

uint32_t const &
CProcessor::ctr() const {
  return mCTR;
}

// -------------------------------------------------------------------------- //

uint32_t &
CProcessor::lr() {
  return mLR;
}

// -------------------------------------------------------------------------- //

uint32_t const &
CProcessor::lr() const {
  return mLR;
}

// -------------------------------------------------------------------------- //

uint8_t &
CProcessor::cr(
  size_t const n
) {
  return mCR[n];
}

// -------------------------------------------------------------------------- //

uint8_t const &
CProcessor::cr(
  size_t const n
) const {
  return mCR[n];
}

// -------------------------------------------------------------------------- //

uint8_t &
CProcessor::xer() {
  return mXER;
}

// -------------------------------------------------------------------------- //

uint8_t const &
CProcessor::xer() const {
  return mXER;
}

// -------------------------------------------------------------------------- //

size_t CProcessor::ea(
  int16_t const d,
  size_t const ra
) const {
  if (ra == 0) {
    return static_cast<size_t>(d);
  }

  return static_cast<size_t>(
    mGPR[ra].u16() + d
  );
}

// -------------------------------------------------------------------------- //

size_t CProcessor::ea(
  size_t const ra,
  size_t const rb
) const {
  if (ra == 0) {
    return static_cast<size_t>(
      mGPR[rb].u32()
    );
  }

  return static_cast<size_t>(
    mGPR[ra].u32() + mGPR[rb].u32()
  );
}

// -------------------------------------------------------------------------- //

uint8_t CProcessor::lbz(
  size_t const addr
) const {
  return ram(addr);
}

// -------------------------------------------------------------------------- //

int16_t CProcessor::lha(
  size_t const addr
) const {
  return static_cast<int16_t>(lhz(addr));
}

// -------------------------------------------------------------------------- //

uint16_t CProcessor::lhz(
  size_t const addr
) const {
  return static_cast<uint16_t>(
    (static_cast<uint16_t>(ram(addr)) << 8) |
    static_cast<uint16_t>(ram(addr + 1))
  );
}

// -------------------------------------------------------------------------- //

uint32_t CProcessor::lwz(
  size_t const addr
) const {
  return static_cast<uint32_t>(
    (static_cast<uint32_t>(ram(addr)) << 24) |
    (static_cast<uint32_t>(ram(addr + 1)) << 16) |
    (static_cast<uint32_t>(ram(addr + 2)) << 8) |
    static_cast<uint32_t>(ram(addr + 3))
  );
}

// -------------------------------------------------------------------------- //

float CProcessor::lfs(
  size_t const addr
) const {
  union {
    uint32_t u32;
    float f32;
  } bits;

  bits.u32 = lwz(addr);
  return bits.f32;
}

// -------------------------------------------------------------------------- //

double CProcessor::lfd(
  size_t const addr
) const {
  uint64_t const hi { lwz(addr) };
  uint64_t const lo { lwz(addr + 4) };

  union {
    uint64_t u64;
    double f64;
  } bits;

  bits.u64 = ((hi << 32) | lo);
  return bits.f64;
}

// -------------------------------------------------------------------------- //

void CProcessor::stb(
  size_t const addr,
  uint8_t const b
) {
  ram(addr) = b;
}

// -------------------------------------------------------------------------- //

void CProcessor::sth(
  size_t const addr,
  uint16_t const h
) {
  ram(addr) = static_cast<uint8_t>((h >> 8) & 0xFF);
  ram(addr + 1) = static_cast<uint8_t>(h & 0xFF);
}

// -------------------------------------------------------------------------- //

void CProcessor::stw(
  size_t const addr,
  uint32_t w
) {
  ram(addr) = static_cast<uint8_t>((w >> 24) & 0xFF);
  ram(addr + 1) = static_cast<uint8_t>((w >> 16) & 0xFF);
  ram(addr + 2) = static_cast<uint8_t>((w >> 8) & 0xFF);
  ram(addr + 3) = static_cast<uint8_t>(w & 0xFF);
}

// -------------------------------------------------------------------------- //

void CProcessor::stfs(
  size_t const addr,
  float const s
) {
  union {
    uint32_t u32;
    float f32;
  } bits;

  bits.f32 = s;
  stw(addr, bits.u32);
}

// -------------------------------------------------------------------------- //

void CProcessor::stfd(
  size_t const addr,
  double const d
) {
  union {
    uint64_t u64;
    double f64;
  } bits;

  bits.f64 = d;
  stw(addr, static_cast<uint32_t>(bits.u64 >> 32));
  stw((addr + 4), static_cast<uint32_t>(bits.u64));
}

// -------------------------------------------------------------------------- //

uint32_t CProcessor::Mask(
  size_t const mb,
  size_t const me
) {
  uint32_t mask {
    (0xFFFFFFFFui32 >> mb) ^
    (0x7FFFFFFFui32 >> me)
  };

  if (me < mb) {
    mask = ~mask;
  }

  return mask;
}

// -------------------------------------------------------------------------- //

uint32_t CProcessor::Rot32(
  uint32_t const value,
  size_t const bits
) {
  return uint32_t(
    (value << bits) | (value >> (32 - bits))
  );
}

// -------------------------------------------------------------------------- //

bool CProcessor::Carry(
  uint32_t const lhs,
  uint32_t const rhs
) {
  return (rhs > ~lhs);
}

// -------------------------------------------------------------------------- //

uint8_t &
CProcessor::ram(
  size_t const addr
) {
  if (addr < 0x80000000) {
    std::cerr << "segfault" << std::endl;
    std::terminate();
  }

  size_t const physical_addr {
    addr & ~0xC0000000
  };

  if (physical_addr >= mMemorySize) {
    std::cerr << "segfault" << std::endl;
    std::terminate();
  }

  return mMemory[physical_addr];
}

// -------------------------------------------------------------------------- //

uint8_t const &
CProcessor::ram(
  size_t const addr
) const {
  if (addr < 0x80000000) {
    std::cerr << "segfault" << std::endl;
    std::terminate();
  }

  size_t const physical_addr {
    addr & ~0xC0000000
  };

  if (physical_addr >= mMemorySize) {
    std::cerr << "segfault" << std::endl;
    std::terminate();
  }

  return mMemory[physical_addr];
}

// -------------------------------------------------------------------------- //

// ========================================================================== //
