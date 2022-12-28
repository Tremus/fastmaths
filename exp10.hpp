// 10^x or pow(10, x)

namespace fast {
namespace exp10 {

template<typename T>
static inline T stl(T a) noexcept { return std::exp10(a); }

static inline powx_stl(float x) { return pow::stl(10.0f, x); }
static inline powx_ekmett_fast(float x) { return pow::ekmett_fast(10.0f, x); }
static inline powx_ekmett_fast_lb(float x) { return pow::ekmett_fast_lb(10.0f, x); }
static inline powx_ekmett_fast_ub(float x) { return pow::powx_ekmett_fast_ub(10.0f, x); }
static inline powx_ekmett_fast_precise(float x) { return pow::powx_ekmett_fast_precise(10.0f, x); }
static inline powx_ekmett_fast_better_precise(float x) { return pow::powx_ekmett_fast_better_precise(10.0f, x); }


} // namespace exp10
} // namespace fast
