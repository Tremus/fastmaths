#pragma once
#include <cmath>
#include "common.hpp"

namespace fast {
namespace log {

static inline  float stl(float x) noexcept { return std::logf(x); }

// JUCE
/** Provides a fast approximation of the function log(x+1) using a Pade approximant
    continued fraction, calculated sample by sample.
    Note: This is an approximation which works on a limited range. You are
    advised to use input values only between -0.8 and +5 for limiting the error.
*/
template <typename FloatType>
static FloatType logNPlusOne (FloatType x) noexcept
{
    auto numerator = x * (7560 + x * (15120 + x * (9870 + x * (2310 + x * 137))));
    auto denominator = 7560 + x * (18900 + x * (16800 + x * (6300 + x * (900 + 30 * x))));
    return numerator / denominator;
}

// https://stackoverflow.com/a/39822314
/* compute natural logarithm, maximum error 0.85089 ulps */
static float njuffa (float a) noexcept {
    float i, m, r, s, t;
    uint32_t e;

#if PORTABLE
    m = frexpf (a, &e);
    if (m < 0.666666667f) {
        m = m + m;
        e = e - 1;
    }
    i = (float)e;
#else // PORTABLE
    i = 0.0f;
    if (a < 1.175494351e-38f){ // 0x1.0p-126
        a = a * 8388608.0f; // 0x1.0p+23
        i = -23.0f;
    }
    e = (convert_type<float, uint32_t> (a) - convert_type<float, uint32_t> (0.666666667f)) & 0xff800000;
    m = convert_type<uint32_t, float> (convert_type<float, uint32_t> (a) - e);
    i = fmaf ((float)e, 1.19209290e-7f, i); // 0x1.0p-23
#endif // PORTABLE
    /* m in [2/3, 4/3] */
    m = m - 1.0f;
    s = m * m;
    /* Compute log1p(m) for m in [-1/3, 1/3] */
    r =             -0.130310059f;  // -0x1.0ae000p-3
    t =              0.140869141f;  //  0x1.208000p-3
    r = fmaf (r, s, -0.121483512f); // -0x1.f198b2p-4
    t = fmaf (t, s,  0.139814854f); //  0x1.1e5740p-3
    r = fmaf (r, s, -0.166846126f); // -0x1.55b36cp-3
    t = fmaf (t, s,  0.200120345f); //  0x1.99d8b2p-3
    r = fmaf (r, s, -0.249996200f); // -0x1.fffe02p-3
    r = fmaf (t, m, r);
    r = fmaf (r, m,  0.333331972f); //  0x1.5554fap-2
    r = fmaf (r, m, -0.500000000f); // -0x1.000000p-1
    r = fmaf (r, s, m);
    r = fmaf (i,  0.693147182f, r); //  0x1.62e430p-1 // log(2)
    if (!((a > 0.0f) && (a < INFINITY))) {
        r = a + a;  // silence NaNs if necessary
        if (a  < 0.0f) r = INFINITY - INFINITY; //  NaN
        if (a == 0.0f) r = -INFINITY;
    }
    return r;
}

// https://stackoverflow.com/a/39822314
/* natural log on [0x1.f7a5ecp-127, 0x1.fffffep127]. Maximum relative error 9.4529e-5 */
float njuffa_faster (float a) noexcept {
    float m, r, s, t, i, f;
    uint32_t e;

    e = (convert_type<float, uint32_t> (a) - 0x3f2aaaab) & 0xff800000;
    m = convert_type<uint32_t, float> (convert_type<float, uint32_t> (a) - e);
    i = (float)e * 1.19209290e-7f; // 0x1.0p-23
    /* m in [2/3, 4/3] */
    f = m - 1.0f;
    s = f * f;
    /* Compute log1p(f) for f in [-1/3, 1/3] */
    r = fmaf (0.230836749f, f, -0.279208571f); // 0x1.d8c0f0p-3, -0x1.1de8dap-2
    t = fmaf (0.331826031f, f, -0.498910338f); // 0x1.53ca34p-2, -0x1.fee25ap-2
    r = fmaf (r, s, t);
    r = fmaf (r, s, f);
    r = fmaf (i, 0.693147182f, r); // 0x1.62e430p-1 // log(2)
    return r;
}

// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* Ankerl's inversion of Schraudolph's published algorithm, converted to explicit multiplication */
static inline double ankerl64(double a) noexcept {
    union { double d; int x[2]; } u = { a };
    return (u.x[1] - 1072632447) * 6.610368362777016e-7; /* 1 / 1512775.0; */
}

// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* 1065353216 - 486411 = 1064866805 */
static inline float ankerl32(float a) noexcept {
    union { float f; int x; } u = { a };
    return (u.x - 1064866805) * 8.262958405176314e-8f; /* 1 / 12102203.0; */
}

// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* 1065353216 + 1 */
static inline float ekmett_lb(float a) noexcept {
    union { float f; int x; } u = { a };
    return (u.x - 1065353217) * 8.262958405176314e-8f; /* 1 / 12102203.0 */
}

// https://stackoverflow.com/a/74585982
// assumes x > 0 and that it's not a subnormal.
// Results for 0 or negative x won't be -Infinity or NaN
inline float jenkas(float x)
{
    //fast_log abs(rel) : avgError = 2.85911e-06(3.32628e-08), MSE = 4.67298e-06(5.31012e-08), maxError = 1.52588e-05(1.7611e-07)
    const float s_log_C0 = -19.645704f;
    const float s_log_C1 = 0.767002f;
    const float s_log_C2 = 0.3717479f;
    const float s_log_C3 = 5.2653985f;
    const float s_log_C4 = -(1.0f + s_log_C0) * (1.0f + s_log_C1) / ((1.0f + s_log_C2) * (1.0f + s_log_C3)); //ensures that log(1) == 0
    const float s_log_2 = 0.6931472f;

    // uint32_t ux = std::bit_cast<uint32_t, float>(x);
    union { float f; uint32_t i; } ux = { x };
    int e = static_cast<int>(ux.i - 0x3f800000) >> 23; //e = exponent part can be negative
    ux.i |= 0x3f800000;
    ux.i &= 0x3fffffff; // 1 <= x < 2  after replacing the exponent field
    // x = std::bit_cast<float, uint32_t>(ux);
    x = ux.f;
    float a = (x + s_log_C0) * (x + s_log_C1);
    float b = (x + s_log_C2) * (x + s_log_C3);
    float c = (float(e) + s_log_C4);
    float d = a / b;
    return (c + d) * s_log_2;
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastlog.h
constexpr float mineiro (float x) noexcept {
    union { float f; uint32_t i; } vx = { x };
    union { uint32_t i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
    float y = vx.i;
    y *= 1.1920928955078125e-7f;

    float log2_x = y - 124.22551499f - 1.498030302f * mx.f - 1.72587999f / (0.3520887068f + mx.f);

    return 0.69314718f * log2_x;
}

constexpr float mineiro_faster (float x) noexcept {
    //  return 0.69314718f * mineiro (x);
    union { float f; uint32_t i; } vx = { x };
    float y = vx.i;
    y *= 8.2629582881927490e-8f;
    return y - 87.989971088f;
}

} // namespace log
} // namespace fast
