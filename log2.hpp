#pragma once
#include <cmath>

#include "log.hpp"

namespace fast {
namespace log2 {

template<typename T>
constexpr T stl(T x) noexcept { return std::log2(x); }

// ~58% faster than std::log2
// https://stackoverflow.com/questions/9411823/fast-log2float-x-implementation-c/28730362#28730362
static inline float lgeoffroy(float val) {
    union { float val; int32_t x; } u = { val };
    float log_2 = (float)(((u.x >> 23) & 255) - 128);
    u.x   &= ~(255 << 23);
    u.x   += 127 << 23;
    log_2 += ((-0.3358287811f) * u.val + 2.0f) * u.val  -0.65871759316667f;
    return (log_2);
}

static inline float lgeoffroy_accurate(float val) {
    union { float val; int32_t x; } u = { val };
    float log_2 = (float)(((u.x >> 23) & 255) - 128);
    u.x   &= ~(255 << 23);
    u.x   += 127 << 23;
    log_2 += ((-0.34484843f) * u.val + 2.02466578f) * u.val - 0.67487759f;
    return (log_2);
}

// https://www.johndcook.com/blog/2021/03/24/log10-trick/
static inline float jcook(float x) noexcept {
    return 2.9142f * (x - 1)/(x + 1);
}

// https://stackoverflow.com/a/41416894
constexpr float __newton_next(float r, float x) noexcept {
    // static float one_over_ln2 = 1.4426950408889634f;
    return r - static_cast<float>(M_LOG2E) * (1 - x / (1 << static_cast<int>(r)));
}

static inline float newton(float x) noexcept {
    // static float epsilon = 0.000001f; // change this to change accuracy
    float r = x / 2; // better first guesses converge faster
    float r2 = __newton_next(r, x);

    // NOTE: sometimes gets stuck in infinite loops
    // float delta = r - r2;
    // while (delta * delta > epsilon) {
    //     r = r2;
    //     r2 = next(r, x);
    //     delta = r - r2
    // }
    return r2;
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastlog.h
static inline float mineiro(float x)
{
  union { float f; uint32_t i; } vx = { x };
  union { uint32_t i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
  float y = vx.i;
  y *= 1.1920928955078125e-7f;

  return y - 124.22551499f
           - 1.498030302f * mx.f
           - 1.72587999f / (0.3520887068f + mx.f);
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastlog.h
static inline float mineiro_faster (float x)
{
    union { float f; uint32_t i; } vx = { x };
    float y = vx.i;
    y *= 1.1920928955078125e-7f;
    return y - 126.94269504f;
}

static constexpr float log2e = 1.4426950408888495f;

static inline float log1_njuffa(float x) noexcept { return log::njuffa(x) * log2e; }
static inline float log1_njuffa_faster(float x) noexcept { return log::njuffa_faster(x) * log2e; }
static inline float log1_ankerl32(float x) noexcept { return log::ankerl32(x) * log2e; }
static inline float log1_ekmett_lb(float x) noexcept { return log::ekmett_lb(x) * log2e; }
static inline float log1_jenkas(float x) noexcept { return log::jenkas(x) * log2e; }
static inline float log1_mineiro(float x) noexcept { return log::mineiro(x) * log2e; }
static inline float log1_mineiro_faster(float x) noexcept { return log::mineiro_faster(x) * log2e; }


} // namespace log2
} // namespace fast
