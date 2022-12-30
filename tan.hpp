namespace fast {
namespace tan {

static inline float stl(float x) noexcept { return std::tanf(x); }

// https://github.com/juce-framework/JUCE/blob/master/modules/juce_dsp/maths/juce_FastMathApproximations.h
static inline float pade (float x) noexcept {
    auto x2 = x * x;
    auto numerator = x * (-135135 + x2 * (17325 + x2 * (-378 + x2)));
    auto denominator = -135135 + x2 * (62370 + x2 * (-3150 + 28 * x2));
    return numerator / denominator;
}

// https://www.musicdsp.org/en/latest/Other/115-sin-cos-tan-approximation.html
static inline float wildmagic0 (float fAngle) noexcept {
    float fASqr = fAngle * fAngle;
    float fResult = 2.033e-01f;
    fResult *= fASqr;
    fResult += 3.1755e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= fAngle;
    return fResult;
}
static inline float wildmagic1 (float fAngle) noexcept {
    float fASqr = fAngle * fAngle;
    float fResult = 9.5168091e-03f;
    fResult *= fASqr;
    fResult += 2.900525e-03f;
    fResult *= fASqr;
    fResult += 2.45650893e-02f;
    fResult *= fASqr;
    fResult += 5.33740603e-02f;
    fResult *= fASqr;
    fResult += 1.333923995e-01f;
    fResult *= fASqr;
    fResult += 3.333314036e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= fAngle;
    return fResult;
}

// https://observablehq.com/@jrus/fasttan
// NOTE: this function is normalised to take normalised halfpi values
// Regular tan functions will cycle after halfpi (1.57), this cycles
// after 1.0
// This is ideal for calculating tan(πfc/fs)
static inline float jrus_alt(float x) noexcept {
    // 3 add, 3 mult, 1 div
    float y = 1 - x * x;
    return x * (-0.0187108f * y + 0.31583526f + 1.27365776f / y);
}

static inline float jrus_alt_denorm(float x) noexcept {
    x *= 0.6366197723675814f;
    float y = 1 - x * x;
    return x * (-0.0187108f * y + 0.31583526f + 1.27365776f / y);
}


static inline float jrus_denorm(float x) noexcept {
    x *= 0.6366197723675814;
    float y = (1 - x*x);
    return x * (((-0.000221184f * y + 0.0024971104f) * y - 0.02301937096f) * y + 0.3182994604f + 1.2732402998f / y);
}

static inline float jrus_full_denorm(float x) noexcept {
    x *= 0.6366197723675814f;

    // 2**54 = 18014398509481984
    float S = 18014398509481984.0 * ((x > 0) - (x < 0));
    x -= (x - S) + S;  // reduce range to -1...1
    float y = (1 - x * x);
    return x * (((-0.00023552f * y + 0.002530368f) * y - 0.023045536f) * y
      + 0.3183073636f + 1.2732395912f / y);
}

// license = Public domain
// https://andrewkay.name/blog/post/efficiently-approximating-tan-x/
static inline float kay (float x) noexcept {
    // 3 mult, 2 sub, 1 div
    static const float pisqby4 = 2.4674011002723397f;
    static const float oneminus8bypisq = 0.1894305308612978f;
    float xsq = x*x;
    return x * (pisqby4 - oneminus8bypisq * xsq) / (pisqby4 - xsq);
}

// approximation of tan with lower relative error
// https://andrewkay.name/blog/post/efficiently-approximating-tan-x/
static inline float kay_precise (float x) noexcept {
    // 3 mult, 2 sub, 1 div
    static const float pisqby4 = 2.4674011002723397f;
    static const float adjpisqby4 = 2.471688400562703f;
    static const float adj1minus8bypisq = 0.189759681063053f;
    float xsq = x * x;
    return x * (adjpisqby4 - adj1minus8bypisq * xsq) / (pisqby4 - xsq);
}

} // namespace tan
} // namespace fast
