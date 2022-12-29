#include "exp.hpp"

namespace fast {
namespace tanh {

template<typename T>
constexpr T stl(T x) noexcept { return std::tanh(x); }

// https://github.com/juce-framework/JUCE/blob/master/modules/juce_dsp/maths/juce_FastMathApproximations.h
template <typename T>
constexpr T pade (T x) noexcept {
    auto x2 = x * x;
    auto numerator = x * (135135 + x2 * (17325 + x2 * (378 + x2)));
    auto denominator = 135135 + x2 * (62370 + x2 * (3150 + 28 * x2));
    return numerator / denominator;
}

// https://math.stackexchange.com/a/3485944
static float c3(float v) noexcept {
    const float c1 = 0.03138777F;
    const float c2 = 0.276281267F;
    const float c_log2f = 1.442695022F;
    v *= c_log2f;
    int intPart = (int)v;
    float x = (v - intPart);
    float xx = x * x;
    float v1 = c_log2f + c2 * xx;
    float v2 = x + xx * c1 * x;
    float v3 = (v2 + v1);
    *((int*)&v3) += intPart << 24;
    float v4 = v2 - v1;
    return (v3 + v4) / (v3 - v4);
}

// these are slower
// https://stackoverflow.com/q/29239343
// static float big_bang(float x) noexcept {
//     return 2 / (1 + std::exp(-2 * x)) - 1;
// }
// // https://stackoverflow.com/a/60505422
// static float spagnum_moss(float x) noexcept {
//     return 1 - (2 * (1 / (1 + std::exp(x * 2))));
// }

static inline float exp_ekmett_ub (float p) noexcept { return -1 + 2 / (1 + exp::ekmett_ub(-2 * p)); }
static inline float exp_ekmett_lb (float p) noexcept { return -1 + 2 / (1 + exp::ekmett_lb(-2 * p)); }
static inline float exp_schraudolph (float p) noexcept { return -1 + 2 / (1 + exp::schraudolph(-2 * p)); }

// https://github.com/romeric/fastapprox/blob/master/fastapprox/src/fasthyperbolic.h
static inline float exp_mineiro   (float p) noexcept { return -1 + 2 / (1 + exp::mineiro  (-2 * p)); }
static inline float exp_mineiro_faster (float p) noexcept { return -1 + 2 / (1 + exp::mineiro_faster(-2 * p)); }



} // namespace tanh
} // namespace fast
