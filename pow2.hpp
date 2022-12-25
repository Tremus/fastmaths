// 2^x or pow(2, x)

namespace fast {
namespace pow2 {

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastexp.h
static inline float mineiro (float p)
{
    float offset = (p < 0) ? 1.0f : 0.0f;
    float clipp = (p < -126) ? -126.0f : p;
    int w = clipp;
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

} // namespace pow2
} // namespace fast
