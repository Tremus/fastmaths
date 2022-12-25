#pragma once
#include <cmath>
#include "sin.hpp"

namespace fast {
namespace cos {

template<typename T>
constexpr T stl(T x) noexcept { return std::cos(x); }

// JUCE
// https://github.com/juce-framework/JUCE/blob/master/modules/juce_dsp/maths/juce_FastMathApproximations.h
template <typename T>
constexpr T pade (T x) noexcept {
    auto x2 = x * x;
    auto numerator = -(-39251520 + x2 * (18471600 + x2 * (-1075032 + 14615 * x2)));
    auto denominator = 39251520 + x2 * (1154160 + x2 * (16632 + x2 * 127));
    return numerator / denominator;
}

// https://stackoverflow.com/a/28050328
template<typename T>
constexpr T milianw(T x) noexcept
{
    constexpr T tp = 1./(2.*M_PI);
    x *= tp;
    x -= T(.25) + std::floor(x + T(.25));
    x *= T(16.) * (std::abs(x) - T(.5));
    #if EXTRA_PRECISION
    x += T(.225) * x * (std::abs(x) - T(1.));
    #endif
    return x;
}

// https://stackoverflow.com/a/71674578
static inline float juha(float x) noexcept {
    return 4 * (0.5f - 0.31830988618f * x) * (1 - std::abs(0.5f - 0.31830988618f * x));
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fasttrig.h
static inline float mineiro (float x) {
    static const float halfpi = 1.5707963267948966f;
    static const float halfpiminustwopi = -4.7123889803846899f;
    float offset = (x > halfpi) ? halfpiminustwopi : halfpi;
    return sin::mineiro (x + offset);
}

static inline float mineiro_faster (float x)
{
    static const float twooverpi = 0.63661977236758134f;
    static const float p = 0.54641335845679634f;

    union { float f; uint32_t i; } vx = { x };
    vx.i &= 0x7FFFFFFF;

    float qpprox = 1.0f - twooverpi * vx.f;

    return qpprox + p * qpprox * (1.0f - qpprox * qpprox);
}


} // namespace cos
} // namespace fast
