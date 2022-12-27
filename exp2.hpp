// 2^x or pow(2, x)

namespace fast {
namespace exp2 {

template<typename T>
static inline T stl(T a) noexcept { return std::exp2(a); }

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastexp.h
static inline float mineiro (float p)
{
    float offset = (p < 0) ? 1.0f : 0.0f;
    float clipp = (p < -126) ? -126.0f : p;
    int w = (int)clipp;
    float z = clipp - w + offset;
    union { uint32_t i; float f; } v = { static_cast<uint32_t> ( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

    return v.f;
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastexp.h
static inline float mineiro_faster (float p)
{
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

} // namespace exp2
} // namespace fast
