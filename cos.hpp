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
static inline float milianw(float x) noexcept {
    x *= 0.15915494309189535f; // 1 / 2π
    x -= 0.25f + std::floor(x + 0.25f);
    x *= 16 * (std::abs(x) - 0.5f);
    return x;
}
static inline float milianw_precise(float x) noexcept {
    x *= 0.15915494309189535f; // 1 / 2π
    x -= 0.25f + std::floor(x + 0.25f);
    x *= 16 * (std::abs(x) - 0.5f);
    x += 0.225f * x * (std::abs(x) - 1);
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

// https://www.musicdsp.org/en/latest/Other/115-sin-cos-tan-approximation.html
float wildmagic0 (float fAngle) noexcept {
    float fASqr = fAngle * fAngle;
    float fResult = 3.705e-02f;
    fResult *= fASqr;
    fResult -= 4.967e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    return fResult;
}
float wildmagic1 (float fAngle) noexcept {
    float fASqr = fAngle * fAngle;
    float fResult = -2.605e-07f;
    fResult *= fASqr;
    fResult += 2.47609e-05f;
    fResult *= fASqr;
    fResult -= 1.3888397e-03f;
    fResult *= fASqr;
    fResult += 4.16666418e-02f;
    fResult *= fASqr;
    fResult -= 4.999999963e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    return fResult;
}

} // namespace cos
} // namespace fast
