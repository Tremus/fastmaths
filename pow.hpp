#pragma once

namespace fast {
namespace pow {

static inline float stl(float a, float b) noexcept { return std::powf(a, b); }

// https://martin.ankerl.com/2007/10/04/optimized-pow-approximation-for-java-and-c-c/
// meant for doubles
static inline double ankerl64(double a, double b) {
    union { double d; int x[2]; } u = { a };
    u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;
    return u.d;
}

// NOTE: while loop can cause infinite loops. Not recommended!
// https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
// should be much more precise with large b
static inline double ankerl_precise64(double a, double b) {
    // calculate approximation with fraction of the exponent
    int e = (int) b;
    union { double d; int x[2]; } u = { a };
    u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;

    // exponentiation by squaring with the exponent's integer part
    // double r = u.d makes everything much slower, not sure why
    double r = 1.0;
    while (e) {
        if (e & 1) {
            r *= a;
        }
        a *= a;
        e >>= 1;
    }

    return r * u.d;
}

// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* 1065353216 + 1      = 1065353217 ub */
/* 1065353216 - 486411 = 1064866805 min RMSE */
/* 1065353216 - 722019 = 1064631197 lb */
static inline float ekmett_fast(float a, float b) noexcept {
    union { float d; int x; } u = { a };
    u.x = (int)(b * (u.x - 1064866805) + 1064866805);
    return u.d;
}

static inline float ekmett_fast_lb(float a, float b) noexcept {
    union { float d; int x; } u = { a };
    u.x = (int)(b * (u.x - 1065353217) + 1064631197);
    return u.d;
}

static inline float ekmett_fast_ub(float a, float b) noexcept {
    union { float d; int x; } u = { a };
    u.x = (int)(b * (u.x - 1064631197) + 1065353217);
    return u.d;
}

// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* should be much more precise with large b */
// slower than stl
static inline float ekmett_fast_precise(float a, float b) {
    int flipped = 0;
    if (b < 0) {
        flipped = 1;
        b = -b;
    }

    /* calculate approximation with fraction of the exponent */
    int e = (int) b;
    union { float f; int x; } u = { a };
    u.x = (int)((b - e) * (u.x - 1065353216) + 1065353216);

    float r = 1.0f;
    while (e) {
        if (e & 1) {
            r *= a;
        }
        a *= a;
        e >>= 1;
    }

    r *= u.f;
    return flipped ? (1.0f / r) : r;
}

// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* should be much more precise with large b */
// slower than stl
static inline float __better_expf_fast(float a) {
    union { float f; int x; } u, v;
    u.x = (int)(6051102 * a + 1056478197);
    v.x = (int)(1056478197 - 6051102 * a);
    return u.f / v.f;
}
static inline float ekmett_fast_better_precise(float a, float b) {
    int flipped = 0;
    if (b < 0) {
        flipped = 1;
        b = -b;
    }

    /* calculate approximation with fraction of the exponent */
    int e = (int) b;
    float f = __better_expf_fast(b - e);

    float r = 1.0f;
    while (e) {
        if (e & 1) {
            r *= a;
        }
        a *= a;
        e >>= 1;
    }

    r *= f;
    return flipped ? 1.0f/r : r;
}


} // namespace pow
} // namespace fast
