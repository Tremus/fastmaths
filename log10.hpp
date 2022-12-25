#pragma once

#include "log.hpp"

namespace fast {
namespace log10 {

template<typename T>
constexpr T stl(T x) noexcept { return std::log10(x); }

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
    float delta = r - r2;
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

// 1 / log10(e)
static constexpr float log10e = 0.434294481903251827651f;

static inline float log1_stl(float x) noexcept { return log::stl(x) * log10e; }
static inline float log1_njuffa(float x) noexcept { return log::njuffa(x) * log10e; }
static inline float log1_njuffa_faster(float x) noexcept { return log::njuffa_faster(x) * log10e; }
static inline float log1_ankerl32(float x) noexcept { return log::ankerl32(x) * log10e; }
static inline float log1_ekmett_lb(float x) noexcept { return log::ekmett_lb(x) * log10e; }
static inline float log1_jenkas(float x) noexcept { return log::jenkas(x) * log10e; }
static inline float log1_mineiro(float x) noexcept { return log::mineiro(x) * log10e; }
static inline float log1_mineiro_faster(float x) noexcept { return log::mineiro_faster(x) * log10e; }


} // namespace log10
} // namespace fast