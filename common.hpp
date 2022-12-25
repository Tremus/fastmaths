#pragma once
template <typename T, typename D>
D convert_type(T v) {
    union { T a; D b; } o { v };
    return o.b;
}
