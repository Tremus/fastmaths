#pragma once

#include "log.hpp"

namespace fast {
namespace log10 {

static inline float stl(float x) noexcept { return std::log10f(x); }

// https://www.johndcook.com/blog/2021/03/24/log10-trick/
constexpr float jcook(float x) noexcept { return (x - 1) / (x + 1); }

// https://stackoverflow.com/a/41416894
constexpr float __newton_next(float r, float x) noexcept {
    // static float one_over_ln2 = 1.4426950408889634f;
    return r - static_cast<float>(M_LOG2E) * (1 - x / (1 << static_cast<int>(r)));
}

constexpr float __newton_log2(float x) noexcept {
    // const float epsilon = 0.00001f; // change this to change accuracy
    float r = x / 2; // better first guesses converge faster
    float r2 = __newton_next(r, x);
    // float delta = r - r2;
    // while (delta * delta > epsilon) {
    //     r = r2;
    //     r2 = __newton_next(r, x);
    //     delta = r - r2;
    // }
    return r2;
}

static inline float newton(float x) noexcept {
    static float log2_10 = __newton_log2(10);
    return __newton_log2(x) / log2_10;
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastlog.h
// adapted from pmineiro's log2

// 1 / log10(e)
static constexpr float log10e = 0.434294481903251827651f;

static inline float log1_njuffa(float x) noexcept { return log::njuffa(x) * log10e; }
static inline float log1_njuffa_faster(float x) noexcept { return log::njuffa_faster(x) * log10e; }
// static inline float log1_ankerl32(float x) noexcept { return log::ankerl32(x) * log10e; }
static inline float log1_ankerl32(float a) noexcept {
    union { float f; int x; } u = { a };
    // return (u.x - 1064866805) * 8.262958405176314e-8f * log10e; /* 1 / 12102203.0; */
    return (u.x - 1064866805) * 3.5885572395641675e-8f; /* 1 / 12102203.0; */
}
static inline float log1_ekmett_ub(float x) noexcept { return log::ekmett_ub(x) * log10e; }
static inline float log1_ekmett_lb(float x) noexcept { return log::ekmett_lb(x) * log10e; }
static inline float log1_jenkas(float x) noexcept { return log::jenkas(x) * log10e; }

// adapted from log2::mineiro
constexpr float log2_mineiro (float x) noexcept {
    union { float f; uint32_t i; } vx = { x };
    union { uint32_t i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
    float y = vx.i;
    // log21_10 = 0.3010299956639812
    // y *= log21_10 * 1.1920928955078125e-7f;
    y *= 3.588557191657796e-8f;

    // return log21_10 * (y - 124.22551499f
    //                      - 1.498030302f * mx.f
    //                      - 1.72587999f  / (0.3520887068f + mx.f));
    return y - 37.39560623879553f
             - 0.4509520553155725f * mx.f
             - 0.5195416459062518f / (0.3520887068f + mx.f);
}

// adapted from log2::mineiro_faster
static inline float log2_mineiro_faster(float x) noexcept {
    union { float f; uint32_t i; } vx = { x };
    float y = vx.i;
    // log21_10 = 0.3010299956639812
    // y *= log21_10 * 1.1920928955078125e-7f;
    y *= 3.588557191657796e-8f;
    // return y - log21_10 * 126.94269504f;
    // return y - 38.21355893746529f;
    return y - 38.21355894f;
}


} // namespace log10
} // namespace fast
