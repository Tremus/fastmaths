#pragma once
#include <cmath>
#include "./common.hpp"

namespace fast {
namespace sin {

static inline float stl(float x) noexcept { return std::sinf(x); }

// based on https://stackoverflow.com/questions/18662261/fastest-implementation-of-sine-cosine-and-square-root-in-c-doesnt-need-to-b
template<typename T, int N = 15>
constexpr T taylor(T x) noexcept {
    T sum       = 0;
    T power     = x;
    T sign      = 1;
    const T x2  = x * x;
    T Fact      = 1.0;
    for (unsigned int i=1; i<N; i+=2)
    {
        sum     += sign * power / Fact;
        power   *= x2;
        Fact    *= (i + 1) * (i + 2);
        sign    *= static_cast<T>(-1);
    }
    return sum;
}

// own implementation
// slower than above method when N is low, faster when N is >= 3
template<typename T, int N = 1>
constexpr T taylorN(T x) noexcept {
    // sin(x) = x - x^3 / 3! + x^5 / 5!
    T x2 = x * x;
    T x3 = x * x2;
    T x5 = x3 * x2;
    T y = x;
    for (int i = 0; i < N; i++) {
        auto d3 = i * 4 + 3;
        auto d5 = i * 4 + 5;
        y -= x3 / d3 + x5 / d5;
        x3 = x5;
        x5 = x5 * x2;
    }
    return y;
}

// https://www.musicdsp.org/en/latest/Other/93-hermite-interpollation.html
constexpr float __hermiteInterpolate(float v0, float v1, float v2, float v3, float offset) {
    float slope0 = (v2 - v0) * 0.5f;
    float slope1 = (v3 - v1) * 0.5f;

    float v = v1 - v2;
    float w = slope0 + v;
    float a = w + v + slope1;
    float b_neg = w + a;
    float stage1 = a * offset - b_neg;
    float stage2 = stage1 * offset + slope0;
    float result = stage2 * offset + v1;
    return result;
}

// fairly accurate in values between -pi & pi, but not beyond that
// ~ 32% faster than std::sin
// JUCE uses this
// https://github.com/juce-framework/JUCE/blob/master/modules/juce_dsp/maths/juce_FastMathApproximations.h
template<typename T>
constexpr T pade (T x) noexcept {
    T x2 = x * x;
    T numerator = -x * (-11511339840 + x2 * (1640635920 + x2 * (-52785432 + x2 * 479249)));
    T denominator = 11511339840 + x2 * (277920720 + x2 * (3177720 + x2 * 18361));
    return numerator / denominator;
}

// not sure where i found this
// fairly accurate in values between -pi & pi, but not beyond that
// ~28% faster than std::sin
template<typename T>
constexpr T sin_approx(T x) noexcept {
    T pi_major = static_cast<T>(3.1415927);
    T pi_minor = static_cast<T>(-0.00000008742278);
    T x2 = x*x;
    T p11 = static_cast<T>(0.00000000013291342);
    T p9  = p11*x2 + static_cast<T>(-0.000000023317787);
    T p7  = p9*x2  + static_cast<T>(0.0000025222919);
    T p5  = p7*x2  + static_cast<T>(-0.00017350505);
    T p3  = p5*x2  + static_cast<T>(0.0066208798);
    T p1  = p3*x2  + static_cast<T>(-0.10132118);
    return (x - pi_major - pi_minor) *
           (x + pi_major + pi_minor) * p1 * x;
}

// https://en.wikipedia.org/wiki/Bhaskara_I's_sine_approximation_formula
template<typename T>
constexpr T bhaskara_degrees(T x) noexcept {
    return 4 * x * (180 - x) / (40500 - x * (180 - x));
}
// very very fast
// ~43% faster than stl
template<typename T>
constexpr T bhaskara_radians(T x) noexcept {
    return 16 * x * (static_cast<T>(M_PI) - x) /
        (25 * static_cast<T>(M_PI) * static_cast<T>(M_PI) - 4 * x * (static_cast<T>(M_PI) - x));
}

// https://web.archive.org/web/20141220225551/http://forum.devmaster.net/t/fast-and-accurate-sine-cosine/9648
template<typename T>
constexpr T slaru(T x) noexcept {
    const T B = 4/static_cast<T>(M_PI);
    const T C = -4/(static_cast<T>(M_PI)*static_cast<T>(M_PI));

    T y = B * x
        + C * x * abs(x);

    // const float Q = 0.775;
    const T P = static_cast<T>(0.225);

    y = P * (y * abs(y) - y) + y;   // Q * y + P * y * abs(y)
    return y;
}

// very very fast
// ~46% faster than stl
// https://stackoverflow.com/a/71674578
template<typename T>
constexpr T juha(float x) noexcept {
    return 4 * static_cast<T>(0.31830988618) * x * (1 - static_cast<T>(0.31830988618) * std::abs(x));
}

// I edited juha to cycle well over +/- pi
static inline float juha_fmod(float x) noexcept {
    int times = (int)(x / static_cast<float>(M_PI));

    // correct sign
    union { float f; int i; } y = { juha<float>(std::fmodf(x, static_cast<float>(M_PI))) };
    y.i ^= times << 31;
    return y.f;
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fasttrig.h
static inline float mineiro (float x) noexcept {
    static const float fouroverpi = 1.2732395447351627f;
    static const float fouroverpisq = 0.40528473456935109f;
    static const float q = 0.78444488374548933f;
    union { float f; uint32_t i; } p = { 0.20363937680730309f };
    union { float f; uint32_t i; } r = { 0.015124940802184233f };
    union { float f; uint32_t i; } s = { -0.0032225901625579573f };

    union { float f; uint32_t i; } vx = { x };
    uint32_t sign = vx.i & 0x80000000;
    vx.i = vx.i & 0x7FFFFFFF;

    float qpprox = fouroverpi * x - fouroverpisq * x * vx.f;
    float qpproxsq = qpprox * qpprox;

    p.i |= sign;
    r.i |= sign;
    s.i ^= sign;

    return q * qpprox + qpproxsq * (p.f + qpproxsq * (r.f + qpproxsq * s.f));
}

static inline float mineiro_faster (float x) noexcept {
    static const float fouroverpi = 1.2732395447351627f;
    static const float fouroverpisq = 0.40528473456935109f;
    static const float q = 0.77633023248007499f;
    union { float f; uint32_t i; } p = { 0.22308510060189463f };

    union { float f; uint32_t i; } vx = { x };
    uint32_t sign = vx.i & 0x80000000;
    vx.i &= 0x7FFFFFFF;

    float qpprox = fouroverpi * x
                 - fouroverpisq * x * vx.f;

    p.i |= sign;

    return qpprox * (q + p.f * qpprox);
}

static inline float mineiro_full (float x) noexcept {
    static const float twopi = 6.2831853071795865f;
    static const float invtwopi = 0.15915494309189534f;

    int k = (int)(x * invtwopi);
    float half = (x < 0) ? -0.5f : 0.5f;
    return mineiro ((half + k) * twopi - x);
}

static inline float mineiro_full_faster (float x) noexcept {
    static const float twopi = 6.2831853071795865f;
    static const float invtwopi = 0.15915494309189534f;

    int k = (int)(x * invtwopi);
    float half = (x < 0) ? -0.5f : 0.5f;
    return mineiro_faster ((half + k) * twopi - x);
}

// This is slower than stl for 32/64/128 floats
// https://stackoverflow.com/a/11575574
template <typename T>
constexpr T __rint (T x) noexcept {
  T t = floor(std::fabs(x) + static_cast<T>(0.5));
  return (x < 0) ? -t : t;
}
template <typename T>
constexpr T __cos_core (T x) noexcept {
    T x8, x4, x2;
    x2 = x * x;
    x4 = x2 * x2;
    x8 = x4 * x4;
    /* evaluate polynomial using Estrin's scheme */
    return (static_cast<T>(-2.7236370439787708e-7) * x2 + static_cast<T>(2.4799852696610628e-5)) * x8 +
           (static_cast<T>(-1.3888885054799695e-3) * x2 + static_cast<T>(4.1666666636943683e-2)) * x4 +
           (static_cast<T>(-4.9999999999963024e-1) * x2 + static_cast<T>(1.0000000000000000e+0));
}
/* minimax approximation to sin on [-pi/4, pi/4] with rel. err. ~= 5.5e-12 */
template <typename T>
constexpr T __sin_core (T x) noexcept {
    T x4, x2;
    x2 = x * x;
    x4 = x2 * x2;
    /* evaluate polynomial using a mix of Estrin's and Horner's scheme */
    return ((static_cast<T>(2.7181216275479732e-6) * x2 - static_cast<T>(1.9839312269456257e-4)) * x4 +
            (static_cast<T>(8.3333293048425631e-3) * x2 - static_cast<T>(1.6666666640797048e-1))) * x2 * x + x;
}

/* relative error < 7e-12 on [-50000, 50000] */
template <typename T>
constexpr T njuffa (T x) noexcept {
    T q, t;
    int quadrant;
    /* Cody-Waite style argument reduction */
    q = __rint<T> (x * static_cast<T>(6.3661977236758138e-1));
    quadrant = (int)q;
    t = x - q * static_cast<T>(1.5707963267923333e+00);
    t = t - q * static_cast<T>(2.5633441515945189e-12);
    if (quadrant & 1) {
        t = __cos_core<T>(t);
    } else {
        t = __sin_core<T>(t);
    }
    return (quadrant & 2) ? -t : t;
}

// https://www.musicdsp.org/en/latest/Other/115-sin-cos-tan-approximation.html
static inline float wildmagic0 (float fAngle) noexcept {
    float fASqr = fAngle*fAngle;
    float fResult = 7.61e-03f;
    fResult *= fASqr;
    fResult -= 1.6605e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= fAngle;
    return fResult;
}
//----------------------------------------------------------------------
static inline float wildmagic1 (float fAngle) noexcept {
    float fASqr = fAngle*fAngle;
    float fResult = -2.39e-08f;
    fResult *= fASqr;
    fResult += 2.7526e-06f;
    fResult *= fASqr;
    fResult -= 1.98409e-04f;
    fResult *= fASqr;
    fResult += 8.3333315e-03f;
    fResult *= fASqr;
    fResult -= 1.666666664e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= fAngle;
    return fResult;
}

// in [-1,1], out [-0.25, 0.25]
// https://bmtechjournal.wordpress.com/2020/05/27/super-fast-quadratic-sinusoid-approximation/
static inline float bluemangoo_original(float x)
{
    return -x * fabsf(x) + x;
}
// in [-pi,pi], out [-1,1]
static inline float bluemangoo(float x)
{
    float x2 = x * float(M_1_PI);
    float y = -x2 * fabsf(x2) + x2;
    return 4 * y;
}

// [0, 2] corresponding to [0, pi]
// https://github.com/LancePutnam/Gamma/blob/0ab245147c8bbebe1e96eb301e52ebb47c8c1c60/Gamma/scl.h#L963
static float lanceputnam_gamma_original(float x)
{
    float y = x * (2.0f - x);
	return y * (0.775f + 0.225f * y);
}
// More forgiving inputs
// [-pi, pi]
static float lanceputnam_gamma(float x)
{
    float ax = fabsf(x * float(M_2_PI));
    float y = ax * (2.0f - ax);
	y = y * (0.775f + 0.225f * y);
    return x < 0 ? -y : y;
}

} // namespace sin
} // namespace fast
