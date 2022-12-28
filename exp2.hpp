#include "./pow.hpp"
#include "./exp.hpp"
// 2^x or pow(2, x)

namespace fast {
namespace exp2 {

static inline float stl(float x) noexcept { return std::exp2f(x); }

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastexp.h
static inline float mineiro (float p) noexcept {
    float offset = (p < 0) ? 1.0f : 0.0f;
    float clipp = (p < -126) ? -126.0f : p;
    int w = (int)clipp;
    float z = clipp - w + offset;
    union { uint32_t i; float f; } v = { static_cast<uint32_t> ( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

    return v.f;
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastexp.h
static inline float mineiro_faster (float p) noexcept {
    float clipp = (p < -126) ? -126.0f : p;
    union { uint32_t i; float f; } v = { static_cast<uint32_t> ( (1 << 23) * (clipp + 126.94269504f) ) };
    return v.f;
}

static inline float schraudolph(float a) noexcept {
    union { float f; int x; } u;
    u.x = (int) (8388607.888014112f * a + 1064866805);
    return u.f;
}

// https://www.musicdsp.org/en/latest/Other/50-base-2-exp.html
static double desoras (const double val) noexcept {
    int    e;
    double ret;

    if (val >= 0) {
        e = int (val);
        ret = val - (e - 1);
        ((*(1 + (int *) &ret)) &= ~(2047 << 20)) += (e + 1023) << 20;
    }
    else {
        e = int (val + 1023);
        ret = val - (e - 1024);
        ((*(1 + (int *) &ret)) &= ~(2047 << 20)) += e << 20;
    }
    return (ret);
}

static double desoras_pos (const double val) noexcept {
    int    e;
    double ret;

    e = int (val);
    ret = val - (e - 1);
    ((*(1 + (int *) &ret)) &= ~(2047 << 20)) += (e + 1023) << 20;
    return (ret);
}

static inline float powx_stl(float x) noexcept { return pow::stl(2.0f, x); }
static inline float powx_ekmett_fast(float x) noexcept { return pow::ekmett_fast(2.0f, x); }
static inline float powx_ekmett_fast_lb(float x) noexcept { return pow::ekmett_fast_lb(2.0f, x); }
static inline float powx_ekmett_fast_ub(float x) noexcept { return pow::ekmett_fast_ub(2.0f, x); }
static inline float powx_ekmett_fast_precise(float x) noexcept { return pow::ekmett_fast_precise(2.0f, x); }
static inline float powx_ekmett_fast_better_precise(float x) noexcept { return pow::ekmett_fast_better_precise(2.0f, x); }

static constexpr float ln2 = 0.693147180559945309417f;

static inline float exp_stl(float x) noexcept { return exp::stl(x * ln2); }
static inline float exp_ekmett_ub(float x) noexcept { return exp::ekmett_ub(x * ln2); }
static inline float exp_ekmett_lb(float x) noexcept { return exp::ekmett_lb(x * ln2); }
static inline float exp_schraudolph(float x) noexcept { return exp::schraudolph(x * ln2); }
static inline float exp_mineiro(float x) noexcept { return exp::mineiro(x * ln2); }
static inline float exp_mineiro_faster(float x) noexcept { return exp::mineiro_faster(x * ln2); }

} // namespace exp2
} // namespace fast
