// https://stackoverflow.com/a/18662665
float bigtailwolf(float x)
{
    unsigned int i = *(unsigned int*) &x;

    // adjust bias
    i  += 127 << 23;
    // approximation of square root
    i >>= 1;

    return *(float*) &i;
}

// https://stackoverflow.com/a/43176496
float nimig18(float x) noexcept {
    union {float f; int i; } X, Y;
    float ScOff;
    uint8_t e;

    X.f = x;
    e = (X.i >> 23);           // f.SFPbits.e;

    if(x <= 0) return(0.0f);

    ScOff = ((e & 1) != 0) ? 1.0f : 0x1.6a09e6p0;  // NOTE: If exp=EVEN, b/c (exp-127) a (EVEN - ODD) := ODD; but a (ODD - ODD) := EVEN!!

    e = ((e + 127) >> 1);                            // NOTE: If exp=ODD,  b/c (exp-127) then flr((exp-127)/2)
    X.i = (X.i & ((1uL << 23) - 1)) | (0x7F << 23);  // Mask mantissa, force exponent to zero.
    Y.i = (((uint32_t) e) << 23);

    // Error grows with square root of the exponent. Unfortunately no work around like inverse square root... :(
    // Y.f *= ScOff * (0x9.5f61ap-4 + X.f*(0x6.a09e68p-4));        // Error = +-1.78e-2 * 2^(flr(log2(x)/2))
    // Y.f *= ScOff * (0x7.2181d8p-4 + X.f*(0xa.05406p-4 + X.f*(-0x1.23a14cp-4)));      // Error = +-7.64e-5 * 2^(flr(log2(x)/2))
    // Y.f *= ScOff * (0x5.f10e7p-4 + X.f*(0xc.8f2p-4 +X.f*(-0x2.e41a4cp-4 + X.f*(0x6.441e6p-8))));     // Error =  8.21e-5 * 2^(flr(log2(x)/2))
    // Y.f *= ScOff * (0x5.32eb88p-4 + X.f*(0xe.abbf5p-4 + X.f*(-0x5.18ee2p-4 + X.f*(0x1.655efp-4 + X.f*(-0x2.b11518p-8)))));   // Error = +-9.92e-6 * 2^(flr(log2(x)/2))
    // Y.f *= ScOff * (0x4.adde5p-4 + X.f*(0x1.08448cp0 + X.f*(-0x7.ae1248p-4 + X.f*(0x3.2cf7a8p-4 + X.f*(-0xc.5c1e2p-8 + X.f*(0x1.4b6dp-8))))));   // Error = +-1.38e-6 * 2^(flr(log2(x)/2))
    // Y.f *= ScOff * (0x4.4a17fp-4 + X.f*(0x1.22d44p0 + X.f*(-0xa.972e8p-4 + X.f*(0x5.dd53fp-4 + X.f*(-0x2.273c08p-4 + X.f*(0x7.466cb8p-8 + X.f*(-0xa.ac00ep-12)))))));    // Error = +-2.9e-7 * 2^(flr(log2(x)/2))
    Y.f *= ScOff * (0x3.fbb3e8p-4 + X.f*(0x1.3b2a3cp0 + X.f*(-0xd.cbb39p-4 + X.f*(0x9.9444ep-4 + X.f*(-0x4.b5ea38p-4 + X.f*(0x1.802f9ep-4 + X.f*(-0x4.6f0adp-8 + X.f*(0x5.c24a28p-12 ))))))));   // Error = +-2.7e-6 * 2^(flr(log2(x)/2))

    return(Y.f);
}
