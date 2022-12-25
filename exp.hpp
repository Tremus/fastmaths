#include "pow2.hpp"

namespace fast {
namespace exp {

template<typename T>
constexpr T stl(T x) noexcept { return std::exp(x); }


// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* 1065353216 + 1 */
static inline float ekmett_ub(float a) noexcept {
    union { float f; int x; } u;
    u.x = (int) (12102203 * a + 1065353217);
    return u.f;
}

// https://github.com/ekmett/approximate/blob/master/cbits/fast.c
/* 1065353216 - 486411 = 1064866805 */
static inline float schraudolph(float a) noexcept {
  union { float f; int x; } u;
  u.x = (int) (12102203 * a + 1064866805);
  return u.f;
}

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fastexp.h
static inline float mineiro (float p) noexcept {
    return pow2::mineiro (1.442695040f * p);
}

static inline float mineiro_faster (float p) noexcept {
    return pow2::mineiro (1.442695040f * p);
}

} // namespace exp
} // namespace fast
