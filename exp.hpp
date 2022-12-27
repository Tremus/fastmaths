#pragma once
#include "common.hpp"

namespace fast {
namespace exp {

template<typename T>
constexpr T stl(T x) noexcept { return std::exp(x); }


// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* 1065353216 + 1 */
static inline float ekmett_ub(float a) noexcept {
    union { float f; int x; } u;
    u.x = (int) (12102203 * a + 1065353217);
    return u.f;
}

// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* 1065353216 - 486411 = 1064866805 */
static inline float schraudolph(float a) noexcept {
  union { float f; int x; } u;
  u.x = (int) (12102203 * a + 1064866805);
  return u.f;
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastexp.h
static inline float mineiro (float x) noexcept {
    // return pow2::mineiro (1.442695040f * p);
    float p = 1.442695040f * x;
    float offset = (p < 0) ? 1.0f : 0.0f;
    float clipp = (p < -126) ? -126.0f : p;
    int w = (int)clipp;
    float z = clipp - w + offset;
    union { uint32_t i; float f; } v = { static_cast<uint32_t> ( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

    return v.f;
}

static inline float mineiro_faster (float x) noexcept {
    // return pow2::mineiro_faster (1.442695040f * p);
    float p = 1.442695040f * x;
    float clipp = (p < -126) ? -126.0f : p;
    union { uint32_t i; float f; } v = { static_cast<uint32_t> ( (1 << 23) * (clipp + 126.94269504f) ) };
    return v.f;
}

} // namespace exp
} // namespace fast
