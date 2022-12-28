#include "./common.hpp"
#include "./pow.hpp"
#include "./exp.hpp"
// 10^x or pow(10, x)

namespace fast {
namespace exp10 {

static inline float powx_stl(float x) { return pow::stl(10.0f, x); }
static inline float powx_ekmett_fast(float x) { return pow::ekmett_fast(10.0f, x); }
// static inline float powx_ekmett_fast_lb(float x) { return pow::ekmett_fast_lb(10.0f, x); }
static inline float powx_ekmett_fast_lb(float b) noexcept {
    int i = (int)(b * 27262975 + 1064631197);
    return convert_type<int, float>(i);
}
static inline float powx_ekmett_fast_ub(float x) { return pow::ekmett_fast_ub(10.0f, x); }
static inline float powx_ekmett_fast_precise(float x) { return pow::ekmett_fast_precise(10.0f, x); }
static inline float powx_ekmett_fast_better_precise(float x) { return pow::ekmett_fast_better_precise(10.0f, x); }



static constexpr float ln10 = 2.30258509299404568402f;

static inline float exp_stl(float x) noexcept { return exp::stl(x * ln10); }
static inline float exp_ekmett_ub(float x) noexcept { return exp::ekmett_ub(x * ln10); }
static inline float exp_ekmett_lb(float x) noexcept { return exp::ekmett_lb(x * ln10); }
// static inline float exp_schraudolph(float x) noexcept { return exp::schraudolph(x * ln10); }
static inline float exp_schraudolph(float a) noexcept {
//   int i = (int) (12102203 * a + 1064866805);
  int i = (int) (27866352.22018782f * a + 1064866805);
    return convert_type<int, float>(i);

}
// static inline float exp_mineiro(float x) noexcept { return exp::mineiro(x * ln10); }
static inline float exp_mineiro(float x) noexcept {
    // float p = 1.442695040f * ln10 * x;
    // float p = log2(10) * x;
    float p = 3.3219280948873626f * x;
    float offset = (p < 0) ? 1.0f : 0.0f;
    float clipp = (p < -126) ? -126.0f : p;
    int w = (int)clipp;
    float z = clipp - w + offset;
    union { uint32_t i; float f; } v = { static_cast<uint32_t> ( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

    return v.f;
}
static inline float exp_mineiro_faster(float x) noexcept { return exp::mineiro_faster(x * ln10); }




} // namespace exp10
} // namespace fast
