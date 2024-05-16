#pragma once
#include <math.h>
// https://nghiaho.com/?p=997
static inline double rajan_wang_joyal_may(double x) noexcept {
    // M_PI_4 0.785398163397448309616
    return M_PI_4 * x - x*(fabs(x) - 1)*(0.2447 + 0.0663*fabs(x));
}

// https://mazzo.li/posts/vectorized-atan2.html
inline float mazzoli_scalar(float x) {
    static const float a1  =  0.99997726f;
    static const float a3  = -0.33262347f;
    static const float a5  =  0.19354346f;
    static const float a7  = -0.11643287f;
    static const float a9  =  0.05265332f;
    static const float a11 = -0.01172120f;

    float x_sq = x*x;
    return x * (a1 + x_sq * (a3 + x_sq * (a5 + x_sq * (a7 + x_sq * (a9 + x_sq * a11)))));
}
