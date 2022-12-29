#pragma once
#define SIGN_MASK_32 0x80000000


template <typename T, typename D>
D convert_type(T v) {
    union { T a; D b; } o { v };
    return o.b;
}
