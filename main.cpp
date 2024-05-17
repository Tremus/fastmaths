#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

#include "common.hpp"

#include "cos.hpp"
#include "exp.hpp"
#include "exp2.hpp"
#include "exp10.hpp"
#include "log.hpp"
#include "log2.hpp"
#include "log10.hpp"
#include "pow.hpp"
#include "sin.hpp"
#include "sqrt.hpp"
#include "tan.hpp"
#include "tanh.hpp"


template <typename F, typename S>
void log_hz_to_midi (F log2_func, S name) {
    constexpr float frequencies[] = {
        1.0f, 2.0f, 5.0f,
        10.0f, 20.0f, 50.0f,
        100.0f, 200.0f, 500.0f,
        1000, 2000.0f, 5000.0f,
        10000.0f, 20000.0f
    };
    std::cout << name << " = [";

    auto hz_to_midi = [=](float Hz) { return 69 + log2_func(Hz / 440) * 12; };

    for (const float Hz : frequencies) {
        std::cout << std::setprecision(12) << hz_to_midi(Hz) << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename F, typename S>
void log_ratio_to_midi_offset (F log2_func, S name) {
    constexpr float ratios[] = {
        0.125f, 0.25f, 0.5f,
        1.0f,  2.0f,  3.0f,  4.0f,  5.0f,  6.0f,  7.0f,  8.0f, 9.0f, 10.0f,
        11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f,
        21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f,
        31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f,
        41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f
    };
    std::cout << name << " = [";

    auto ratio_to_midi_offset = [=](float r) { return log2_func(r) * 12; };

    for (const float r : ratios) {
        std::cout << std::setprecision(12) << ratio_to_midi_offset(r) << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename F, typename S>
void log_midi_to_hz (F pow2_func, S name) {
    constexpr float midi_notes[] = {
        -36.376312f, // 1Hz
        -24.376312f, // 2Hz
         -8.513184f, // 5Hz
          3.486816f, // 10Hz
         15.486820f, // 20Hz
         31.349960f, // 50Hz
         43.349960f, // 100Hz
         55.349960f, // 200Hz
         71.213097f, // 500Hz
         83.213097f, // 1kHz
         95.213097f, // 2kHz
        111.076233f, // 5kHz
        123.076233f, // 10kHz
        135.076233f, // 20kHz
    };
    std::cout << name << " = [";

    for (const float midi : midi_notes) {
        const float hz = 440.0f * pow2_func((midi - 69.0f) * 0.083333f);
        std::cout << std::setprecision(12) << hz << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename F, typename S>
void log_normalised_freq(F log_func, S name) noexcept {
    constexpr float frequencies[] = {
                  20.0f,   50.0f,
        100.0f,   200.0f,  500.0f,
        1000,     2000.0f, 5000.0f,
        10000.0f, 20000.0f
    };
    std::cout << name << " = [";

    for (const float Hz : frequencies) {
        float normalised = log_func(Hz * 0.05f) * 0.14426950408889633f;
        std::cout << std::setprecision(12) << normalised << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename F, typename S>
void log_denormalised_freq(F exp_func, S name) noexcept {
    constexpr float values[] = {
        0.0f, 0.025f, 0.05f, 0.075f,
        0.1f, 0.125f, 0.15f, 0.175f,
        0.2f, 0.225f, 0.25f, 0.275f,
        0.3f, 0.325f, 0.35f, 0.375f,
        0.4f, 0.425f, 0.45f, 0.475f,
        0.5f, 0.525f, 0.55f, 0.575f,
        0.6f, 0.625f, 0.65f, 0.675f,
        0.7f, 0.725f, 0.75f, 0.775f,
        0.8f, 0.825f, 0.85f, 0.875f,
        0.9f, 0.925f, 0.95f, 0.975f,
        1.0f};
    std::cout << name << " = [";

    for (const float v : values) {
        float denormalised = 20 * exp_func(v * 10);
        std::cout << std::setprecision(12) << denormalised << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename F, typename S>
void log_db_to_gain(F exp_func, S name) noexcept {
    constexpr float decibels[] = {
       -84.0f, -81.0f, -78.0f, -75.0f,
       -72.0f, -69.0f, -66.0f, -63.0f,
       -60.0f, -57.0f, -54.0f, -51.0f,
       -48.0f, -45.0f, -42.0f, -39.0f,
       -36.0f, -33.0f, -30.0f, -27.0f,
       -24.0f, -21.0f, -18.0f, -15.0f,
       -12.0f, -9.0f, -6.0f, -3.0f,
        0.0f, 3.0f, 6.0f, 9.0f,
        12.0f};
    std::cout << name << " = [";

    for (const float dB : decibels) {
        // pow (10, dB / 20)
        // exp10 (dB * 0.05)
        float gain = exp_func(dB * 0.05f);
        std::cout << std::setprecision(12) << gain << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename F, typename S>
void log_gain_to_db(F log_func, S name) noexcept {
    constexpr float gains[] = {
        6.309573444801929e-05f, // -84
        8.912509381337459e-05f, // -81
        0.00012589254117941674f, // -78
        0.00017782794100389227f, // -75
        0.00025118864315095795f, // -72
        0.0003548133892335753f, // -69
        0.0005011872336272725f, // -66
        0.000707945784384138f, // -63
        0.001f, // -60
        0.001412537544622754f, // -57
        0.001995262314968879f, // -54
        0.002818382931264455f, // -51
        0.003981071705534973f, // -48
        0.005623413251903491f, // -45
        0.007943282347242814f, // -42
        0.011220184543019636f, // -39
        0.015848931924611134f, // -36
        0.0223872113856834f, // -33
        0.03162277660168379f, // -30
        0.0446683592150963f, // -27
        0.06309573444801933f, // -24
        0.08912509381337455f, // -21
        0.12589254117941673f, // -18
        0.1778279410038923f, // -15
        0.251188643150958f, // -12
        0.35481338923357547f, // -9
        0.5011872336272722f, // -6
        0.7079457843841379f, // -3
        1.0f, // 0
        1.4125375446227544f, // 3
        1.9952623149688795f, // 6
        2.8183829312644537f, // 9
        3.9810717055349722f, // 12
    };
    std::cout << name << " = [";

    for (const float g : gains) {
        // log10(gain) * 20
        float gain = log_func(g) * 20;
        std::cout << std::setprecision(12) << gain << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename F, typename S>
void log_sin(F sin_func, S name) noexcept {
    constexpr float radians[] = {
        -12.566370614359172f, -12.173671532660448f, -11.780972450961723f, -11.388273369263f, -10.995574287564276f, -10.602875205865551f, -10.210176124166829f, -9.817477042468104f,
        -9.42477796076938f, -9.032078879070655f, -8.63937979737193f, -8.246680715673207f, -7.853981633974483f, -7.461282552275758f, -7.0685834705770345f, -6.675884388878311f,
        -6.283185307179586f, -5.890486225480862f, -5.497787143782138f, -5.105088062083414f, -4.71238898038469f, -4.319689898685965f, -3.9269908169872414f, -3.5342917352885173f,
        -3.141592653589793f, -2.748893571891069f, -2.356194490192345f, -1.9634954084936207f, -1.5707963267948966f, -1.1780972450961724f, -0.7853981633974483f, -0.39269908169872414f,
        0.0f, 0.39269908169872414f, 0.7853981633974483f, 1.1780972450961724f, 1.5707963267948966f, 1.9634954084936207f, 2.356194490192345f, 2.748893571891069f,
        3.141592653589793f, 3.5342917352885173f, 3.9269908169872414f, 4.319689898685965f, 4.71238898038469f, 5.105088062083414f, 5.497787143782138f, 5.890486225480862f,
        6.283185307179586f, 6.675884388878311f, 7.0685834705770345f, 7.461282552275758f, 7.853981633974483f, 8.246680715673207f, 8.63937979737193f, 9.032078879070655f,
        9.42477796076938f, 9.817477042468104f, 10.210176124166829f, 10.602875205865551f, 10.995574287564276f, 11.388273369263f, 11.780972450961723f, 12.173671532660448f,
        12.566370614359172f
    };

    std::cout << name << " = [";

    for (const float r : radians) {
        float s = sin_func(r);
        std::cout << std::setprecision(12) << s << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename F, typename S>
void log_tanh(F tanh_func, S name) noexcept {
    constexpr float values[] = {
        -4.0f, -3.9f, -3.8f, -3.7f, -3.6f, -3.5f, -3.4f, -3.3f, -3.2f, -3.1f,
        -3.0f, -2.9f, -2.8f, -2.7f, -2.6f, -2.5f, -2.4f, -2.3f, -2.2f, -2.1f,
        -2.0f, -1.9f, -1.8f, -1.7f, -1.6f, -1.5f, -1.4f, -1.3f, -1.2f, -1.1f,
        -1.0f, -0.9f, -0.8f, -0.7f, -0.6f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f,
         0.0f,  0.1f,  0.2f,  0.3f,  0.4f,  0.5f,  0.6f,  0.7f,  0.8f,  0.9f,
         1.0f,  1.1f,  1.2f,  1.3f,  1.4f,  1.5f,  1.6f,  1.7f,  1.8f,  1.9f,
         2.0f,  2.1f,  2.2f,  2.3f,  2.4f,  2.5f,  2.6f,  2.7f,  2.8f,  2.9f,
         3.0f,  3.1f,  3.2f,  3.3f,  3.4f,  3.5f,  3.6f,  3.7f,  3.8f,  3.9f,
         4.0f,
    };

    std::cout << name << " = [";

    for (const float v : values) {
        float s = tanh_func(v);
        std::cout << std::setprecision(12) << s << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename F, typename S>
void log_tan(F tan_func, S name) noexcept {
    constexpr float frequencies[] = {
        10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f,
        100.0f, 200.0f, 300.0f, 400.0f, 500.0f, 600.0f, 700.0f, 800.0f, 900.0f,
        1000.0f, 2000.0f, 3000.0f, 4000.0f, 5000.0f, 6000.0f, 7000.0f, 8000.0f, 9000.0f,
        10'000.0f, 11'000.0f, 12'000.0f, 13'000.0f, 14'000.0f, 15'000.0f, 16'000.0f, 17'000.0f, 18'000.0f, 19'000.0f,
        20'000.0f,
    };

    std::cout << name << " = [";

    const float sampleRate = 44100.0f;
    for (const float fc : frequencies) {
        // wc = pi * fc / fs
        float wc = static_cast<float>(M_PI) * fc / sampleRate;
        float g = tan_func(wc);
        std::cout << std::setprecision(12) << g << ",";
    }
    std::cout << "]" << std::endl;
}


template <typename F, typename S>
void log_sqrt(F sqrt_func, S name) noexcept {
    constexpr float values[] = {
         0.0f,  0.1f,  0.2f,  0.3f,  0.4f,  0.5f,  0.6f,  0.7f,  0.8f,  0.9f,
         1.0f,  1.1f,  1.2f,  1.3f,  1.4f,  1.5f,  1.6f,  1.7f,  1.8f,  1.9f,
         2.0f,  2.1f,  2.2f,  2.3f,  2.4f,  2.5f,  2.6f,  2.7f,  2.8f,  2.9f,
         3.0f,  3.1f,  3.2f,  3.3f,  3.4f,  3.5f,  3.6f,  3.7f,  3.8f,  3.9f,
         4.0f,
    };

    std::cout << name << " = [";

    for (const float v : values) {
        float s = sqrt_func(v);
        std::cout << std::setprecision(12) << s << ",";
    }
    std::cout << "]" << std::endl;
}

float gen_random() noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1.0, 1.0);
    return dis(gen);
}

volatile float sink{}; // ensures a side effect

int main() {
    auto benchmark = [](auto fun, auto rem) {
        const auto start = std::chrono::high_resolution_clock::now();
        for (auto size{1ULL}; size != 10'000'000ULL; ++size) {
            sink = fun(gen_random());
        }
        const std::chrono::duration<double> diff =
            std::chrono::high_resolution_clock::now() - start;
        std::cout << "Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << rem << std::endl;
    };

    benchmark([](float x) { return x; }, "pass");
    benchmark([](float x) { return x * x; }, "x^2");

    /** SINE */
    benchmark(fast::sin::stl, "stl");
    // benchmark(fast::sin::taylor<float, 5>, "taylor 5");
    // benchmark(fast::sin::taylor<float, 9>, "taylor 9");
    // benchmark(fast::sin::taylor<float, 13>, "taylor 13");
    // benchmark(fast::sin::taylor<float, 17>, "taylor 13");
    // benchmark(fast::sin::taylorN<float, 1>, "taylorN 1");
    // benchmark(fast::sin::taylorN<float, 2>, "taylorN 2");
    // benchmark(fast::sin::taylorN<float, 3>, "taylorN 3");
    // benchmark(fast::sin::taylorN<float, 4>, "taylorN 3");
    benchmark(fast::sin::bhaskara_radians<float>, "bhaskara_radians");
    benchmark(fast::sin::pade<float>, "pade");
    benchmark(fast::sin::sin_approx<float>, "sin_approx");
    benchmark(fast::sin::slaru<float>, "slaru");
    benchmark(fast::sin::juha<float>, "juha");
    benchmark(fast::sin::juha_fmod, "juha_fmod");
    benchmark(fast::sin::mineiro, "mineiro");
    benchmark(fast::sin::mineiro_faster, "mineiro_faster");
    benchmark(fast::sin::mineiro_full, "mineiro_full");
    benchmark(fast::sin::mineiro_full_faster, "mineiro_full_faster");
    benchmark(fast::sin::njuffa<float>, "njuffa");
    benchmark(fast::sin::wildmagic0, "wildmagic0");
    benchmark(fast::sin::wildmagic1, "wildmagic1");
    benchmark(fast::sin::bluemangoo, "bluemangoo");
    benchmark(fast::sin::lanceputnam_gamma, "lanceputnam_gamma");

    log_sin(fast::sin::stl, "stl");
    log_sin(fast::sin::bhaskara_radians<float>, "bhaskara_radians");
    log_sin(fast::sin::pade<float>, "pade");
    log_sin(fast::sin::sin_approx<float>, "sin_approx");
    log_sin(fast::sin::slaru<float>, "slaru");
    log_sin(fast::sin::juha<float>, "juha");
    log_sin(fast::sin::juha_fmod, "juha_fmod");
    log_sin(fast::sin::mineiro, "mineiro");
    log_sin(fast::sin::mineiro_faster, "mineiro_faster");
    log_sin(fast::sin::mineiro_full, "mineiro_full");
    log_sin(fast::sin::mineiro_full_faster, "mineiro_full_faster");
    log_sin(fast::sin::njuffa<float>, "njuffa");
    log_sin(fast::sin::wildmagic0, "wildmagic0");
    log_sin(fast::sin::wildmagic1, "wildmagic1");
    log_sin(fast::sin::bluemangoo, "bluemangoo");
    log_sin(fast::sin::lanceputnam_gamma, "lanceputnam_gamma");

    /** COS */
    /**
    benchmark(fast::cos::stl<float>, "stl");
    benchmark(fast::cos::pade<float>, "pade");
    benchmark(fast::cos::milianw, "milianw");
    benchmark(fast::cos::milianw_precise, "milianw_precise");
    benchmark(fast::cos::juha, "juha");
    benchmark(fast::cos::mineiro, "mineiro");
    benchmark(fast::cos::mineiro_faster, "mineiro_faster");
    benchmark(fast::cos::wildmagic0, "wildmagic0");
    benchmark(fast::cos::wildmagic1, "wildmagic1");

    log_sin(fast::cos::stl<float>, "stl");
    log_sin(fast::cos::pade<float>, "pade");
    log_sin(fast::cos::milianw, "milianw");
    log_sin(fast::cos::milianw_precise, "milianw_precise");
    log_sin(fast::cos::juha, "juha");
    log_sin(fast::cos::mineiro, "mineiro");
    log_sin(fast::cos::mineiro_faster, "mineiro_faster");
    log_sin(fast::cos::wildmagic0, "wildmagic0");
    log_sin(fast::cos::wildmagic1, "wildmagic1");
    */

    /** TAN */
    /**
    benchmark(fast::tan::stl, "stl");
    benchmark(fast::tan::pade, "pade");
    benchmark(fast::tan::wildmagic0, "wildmagic0");
    benchmark(fast::tan::wildmagic1, "wildmagic1");
    benchmark(fast::tan::jrus_alt, "jrus_alt");
    benchmark(fast::tan::jrus_alt_denorm, "jrus_alt_denorm");
    benchmark(fast::tan::jrus_denorm, "jrus_denorm");
    benchmark(fast::tan::jrus_full_denorm, "jrus_full_denorm");
    benchmark(fast::tan::kay, "kay");
    benchmark(fast::tan::kay_precise, "kay_precise");

    log_tan(fast::tan::stl, "stl");
    log_tan(fast::tan::pade, "pade");
    log_tan(fast::tan::wildmagic0, "wildmagic0");
    log_tan(fast::tan::wildmagic1, "wildmagic1");
    log_tan(fast::tan::jrus_alt_denorm, "jrus_alt_denorm");
    log_tan(fast::tan::jrus_denorm, "jrus_denorm");
    log_tan(fast::tan::jrus_full_denorm, "jrus_full_denorm");
    log_tan(fast::tan::kay, "kay");
    log_tan(fast::tan::kay_precise, "kay_precise");
    */

    /** TANH */
    /**
    benchmark(fast::tanh::stl<float>, "stl");
    benchmark(fast::tanh::pade<float>, "pade");
    benchmark(fast::tanh::c3, "c3");
    benchmark(fast::tanh::exp_ekmett_ub, "exp_ekmett_ub");
    benchmark(fast::tanh::exp_ekmett_lb, "exp_ekmett_lb");
    benchmark(fast::tanh::exp_schraudolph, "exp_schraudolph");
    benchmark(fast::tanh::exp_mineiro, "exp_mineiro");
    benchmark(fast::tanh::exp_mineiro_faster, "exp_mineiro_faster");

    log_tanh(fast::tanh::stl<float>, "stl");
    log_tanh(fast::tanh::pade<float>, "pade");
    log_tanh(fast::tanh::c3, "c3");
    log_tanh(fast::tanh::exp_ekmett_ub, "exp_ekmett_ub");
    log_tanh(fast::tanh::exp_ekmett_lb, "exp_ekmett_lb");
    log_tanh(fast::tanh::exp_schraudolph, "exp_schraudolph");
    log_tanh(fast::tanh::exp_mineiro, "exp_mineiro");
    log_tanh(fast::tanh::exp_mineiro_faster, "exp_mineiro_faster");
    */

    /** LOG */
    /**
    benchmark(fast::log::stl<float>, "stl");
    benchmark(fast::log::logNPlusOne<float>, "logNPlusOne");
    benchmark(fast::log::njuffa, "njuffa");
    benchmark(fast::log::njuffa_faster, "njuffa_faster");
    benchmark(fast::log::ankerl32, "ankerl32");
    benchmark(fast::log::ekmett_lb, "ekmett_lb");
    benchmark(fast::log::mineiro, "mineiro");
    benchmark(fast::log::mineiro_faster, "mineiro_faster");
    log_normalised_freq(fast::log::stl<float>, "stl");
    log_normalised_freq(fast::log::logNPlusOne<float>, "logNPlusOne");
    log_normalised_freq(fast::log::njuffa, "njuffa");
    log_normalised_freq(fast::log::njuffa_faster, "njuffa_faster");
    log_normalised_freq(fast::log::ankerl32, "ankerl32");
    log_normalised_freq(fast::log::ekmett_lb, "ekmett_lb");
    log_normalised_freq(fast::log::mineiro, "mineiro");
    log_normalised_freq(fast::log::mineiro_faster, "mineiro_faster");
    */

    /** LOG2 */
    /**
    benchmark(fast::log2::stl, "stl");
    benchmark(fast::log2::lgeoffroy, "lgeoffroy");
    benchmark(fast::log2::lgeoffroy_accurate, "lgeoffroy_accurate");
    benchmark(fast::log2::jcook, "jcook");
    benchmark(fast::log2::mineiro, "mineiro");
    benchmark(fast::log2::mineiro_faster, "mineiro_faster");
    benchmark(fast::log2::newton, "newton");
    benchmark(fast::log2::desoras, "desoras");
    benchmark(fast::log2::log1_njuffa, "log1_njuffa");
    benchmark(fast::log2::log1_njuffa_faster, "log1_njuffa_faster");
    benchmark(fast::log2::log1_ankerl32, "log1_ankerl32");
    benchmark(fast::log2::log1_ekmett_lb, "log1_ekmett_lb");

    log_hz_to_midi(fast::log2::stl, "stl");
    log_hz_to_midi(fast::log2::lgeoffroy, "lgeoffroy");
    log_hz_to_midi(fast::log2::lgeoffroy_accurate, "lgeoffroy_accurate");
    log_hz_to_midi(fast::log2::jcook, "jcook");
    log_hz_to_midi(fast::log2::mineiro, "mineiro");
    log_hz_to_midi(fast::log2::mineiro_faster, "mineiro_faster");
    log_hz_to_midi(fast::log2::newton, "newton");
    log_hz_to_midi(fast::log2::desoras, "desoras");
    log_hz_to_midi(fast::log2::log1_njuffa, "log1_njuffa");
    log_hz_to_midi(fast::log2::log1_njuffa_faster, "log1_njuffa_faster");
    log_hz_to_midi(fast::log2::log1_ankerl32, "log1_ankerl32");
    log_hz_to_midi(fast::log2::log1_ekmett_lb, "log1_ekmett_lb");
    */

    /*
    log_ratio_to_midi_offset(fast::log2::stl, "stl");
    log_ratio_to_midi_offset(fast::log2::lgeoffroy, "lgeoffroy");
    log_ratio_to_midi_offset(fast::log2::lgeoffroy_accurate, "lgeoffroy_accurate");
    log_ratio_to_midi_offset(fast::log2::jcook, "jcook");
    log_ratio_to_midi_offset(fast::log2::mineiro, "mineiro");
    log_ratio_to_midi_offset(fast::log2::mineiro_faster, "mineiro_faster");
    log_ratio_to_midi_offset(fast::log2::newton, "newton");
    log_ratio_to_midi_offset(fast::log2::desoras, "desoras");
    log_ratio_to_midi_offset(fast::log2::log1_njuffa, "log1_njuffa");
    log_ratio_to_midi_offset(fast::log2::log1_njuffa_faster, "log1_njuffa_faster");
    log_ratio_to_midi_offset(fast::log2::log1_ankerl32, "log1_ankerl32");
    log_ratio_to_midi_offset(fast::log2::log1_ekmett_lb, "log1_ekmett_lb");
    log_ratio_to_midi_offset(fast::log2::log1_mineiro_faster, "log1_mineiro_faster");
    */

    /** LOG10 */
    /*
    benchmark(fast::log10::stl, "stl");
    benchmark(fast::log10::jcook, "jcook");
    benchmark(fast::log10::newton, "newton");
    benchmark(fast::log10::log1_njuffa, "log1_njuffa");
    benchmark(fast::log10::log1_njuffa_faster, "log1_njuffa_faster");
    benchmark(fast::log10::log1_ankerl32, "log1_ankerl32");
    benchmark(fast::log10::log1_ekmett_ub, "log1_ekmett_ub");
    benchmark(fast::log10::log1_ekmett_lb, "log1_ekmett_lb");
    benchmark(fast::log10::log2_mineiro, "log2_mineiro");
    benchmark(fast::log10::log2_mineiro_faster, "log2_mineiro_faster");

    log_gain_to_db(fast::log10::stl, "stl");
    log_gain_to_db(fast::log10::jcook, "jcook");
    log_gain_to_db(fast::log10::newton, "newton");
    log_gain_to_db(fast::log10::log1_njuffa, "log1_njuffa");
    log_gain_to_db(fast::log10::log1_njuffa_faster, "log1_njuffa_faster");
    log_gain_to_db(fast::log10::log1_ankerl32, "log1_ankerl32");
    log_gain_to_db(fast::log10::log1_ekmett_ub, "log1_ekmett_ub");
    log_gain_to_db(fast::log10::log1_ekmett_lb, "log1_ekmett_lb");
    log_gain_to_db(fast::log10::log2_mineiro, "log2_mineiro");
    log_gain_to_db(fast::log10::log2_mineiro_faster, "log2_mineiro_faster");
    */

    /** EXP2 */
    /**
    benchmark([](float x) { return fast::exp2::stl(x); }, "exp2");
    benchmark([](float x) { return fast::exp2::mineiro(x); }, "mineiro");
    benchmark([](float x) { return fast::exp2::mineiro_faster(x); }, "mineiro_faster");
    benchmark([](float x) { return fast::exp2::schraudolph(x); }, "schraudolph");
    benchmark([](float x) { return (float)fast::exp2::desoras(x); }, "desoras");
    benchmark([](float x) { return (float)fast::exp2::desoras_pos(x); }, "desoras_pos");
    benchmark([](float x) { return fast::exp2::powx_stl(x); }, "powx_stl");
    benchmark([](float x) { return fast::exp2::powx_ekmett_fast(x); }, "powx_ekmett_fast");
    benchmark([](float x) { return fast::exp2::powx_ekmett_fast_lb(x); }, "powx_ekmett_fast_lb");
    benchmark([](float x) { return fast::exp2::powx_ekmett_fast_ub(x); }, "powx_ekmett_fast_ub");
    benchmark([](float x) { return fast::exp2::powx_ekmett_fast_precise(x); }, "powx_ekmett_fast_precise");
    benchmark([](float x) { return fast::exp2::powx_ekmett_fast_better_precise(x); }, "powx_ekmett_fast_better_precise");
    benchmark([](float x) { return fast::exp2::exp_stl(x); }, "exp_stl");
    benchmark([](float x) { return fast::exp2::exp_ekmett_ub(x); }, "exp_ekmett_ub");
    benchmark([](float x) { return fast::exp2::exp_schraudolph(x); }, "exp_schraudolph");
    benchmark([](float x) { return fast::exp2::exp_mineiro(x); }, "exp_mineiro");
    benchmark([](float x) { return fast::exp2::exp_mineiro_faster(x); }, "exp_mineiro_faster");

    log_midi_to_hz([](float x) { return fast::exp2::stl(x); }, "exp2");
    log_midi_to_hz([](float x) { return fast::exp2::mineiro(x); }, "mineiro");
    log_midi_to_hz([](float x) { return fast::exp2::mineiro_faster(x); }, "mineiro_faster");
    log_midi_to_hz([](float x) { return fast::exp2::schraudolph(x); }, "schraudolph");
    log_midi_to_hz([](float x) { return (float)fast::exp2::desoras(x); }, "desoras");
    log_midi_to_hz([](float x) { return fast::exp2::powx_stl(x); }, "powx_stl");
    log_midi_to_hz([](float x) { return fast::exp2::powx_ekmett_fast(x); }, "powx_ekmett_fast");
    log_midi_to_hz([](float x) { return fast::exp2::powx_ekmett_fast_lb(x); }, "powx_ekmett_fast_lb");
    log_midi_to_hz([](float x) { return fast::exp2::powx_ekmett_fast_ub(x); }, "powx_ekmett_fast_ub");
    log_midi_to_hz([](float x) { return fast::exp2::powx_ekmett_fast_precise(x); }, "powx_ekmett_fast_precise");
    log_midi_to_hz([](float x) { return fast::exp2::powx_ekmett_fast_better_precise(x); }, "powx_ekmett_fast_better_precise");
    log_midi_to_hz([](float x) { return fast::exp2::exp_stl(x); }, "exp_stl");
    log_midi_to_hz([](float x) { return fast::exp2::exp_ekmett_ub(x); }, "exp_ekmett_ub");
    log_midi_to_hz([](float x) { return fast::exp2::exp_schraudolph(x); }, "exp_schraudolph");
    log_midi_to_hz([](float x) { return fast::exp2::exp_mineiro(x); }, "exp_mineiro");
    log_midi_to_hz([](float x) { return fast::exp2::exp_mineiro_faster(x); }, "exp_mineiro_faster");

    log_denormalised_freq([](float x) { return fast::exp2::stl(x); }, "exp2");
    log_denormalised_freq([](float x) { return fast::exp2::mineiro(x); }, "mineiro");
    log_denormalised_freq([](float x) { return fast::exp2::mineiro_faster(x); }, "mineiro_faster");
    log_denormalised_freq([](float x) { return fast::exp2::schraudolph(x); }, "schraudolph");
    log_denormalised_freq([](float x) { return (float)fast::exp2::desoras(x); }, "desoras");
    log_denormalised_freq([](float x) { return fast::exp2::powx_stl(x); }, "powx_stl");
    log_denormalised_freq([](float x) { return fast::exp2::powx_ekmett_fast(x); }, "powx_ekmett_fast");
    log_denormalised_freq([](float x) { return fast::exp2::powx_ekmett_fast_lb(x); }, "powx_ekmett_fast_lb");
    log_denormalised_freq([](float x) { return fast::exp2::powx_ekmett_fast_ub(x); }, "powx_ekmett_fast_ub");
    log_denormalised_freq([](float x) { return fast::exp2::powx_ekmett_fast_precise(x); }, "powx_ekmett_fast_precise");
    log_denormalised_freq([](float x) { return fast::exp2::powx_ekmett_fast_better_precise(x); }, "powx_ekmett_fast_better_precise");
    log_denormalised_freq([](float x) { return fast::exp2::exp_stl(x); }, "exp_stl");
    log_denormalised_freq([](float x) { return fast::exp2::exp_ekmett_ub(x); }, "exp_ekmett_ub");
    log_denormalised_freq([](float x) { return fast::exp2::exp_schraudolph(x); }, "exp_schraudolph");
    log_denormalised_freq([](float x) { return fast::exp2::exp_mineiro(x); }, "exp_mineiro");
    log_denormalised_freq([](float x) { return fast::exp2::exp_mineiro_faster(x); }, "exp_mineiro_faster");
    */

    /** EXP */
    // benchmark(fast::exp::stl<float>, "stl");
    // benchmark(fast::exp::ekmett_ub, "ekmett_ub");
    // benchmark(fast::exp::schraudolph, "schraudolph");
    // benchmark(fast::exp::mineiro, "mineiro");
    // benchmark(fast::exp::mineiro_faster, "mineiro_faster");

    /** EXP10 */
    /**
    benchmark([](float x) { return fast::exp10::powx_stl(x); }, "powx_stl");
    benchmark([](float x) { return fast::exp10::powx_ekmett_fast(x); }, "powx_ekmett_fast");
    benchmark([](float x) { return fast::exp10::powx_ekmett_fast_lb(x); }, "powx_ekmett_fast_lb");
    benchmark([](float x) { return fast::exp10::powx_ekmett_fast_ub(x); }, "powx_ekmett_fast_ub");
    benchmark([](float x) { return fast::exp10::powx_ekmett_fast_precise(x); }, "powx_ekmett_fast_precise");
    benchmark([](float x) { return fast::exp10::powx_ekmett_fast_better_precise(x); }, "powx_ekmett_fast_better_precise");
    benchmark([](float x) { return fast::exp10::exp_stl(x); }, "exp_stl");
    benchmark([](float x) { return fast::exp10::exp_ekmett_lb(x); }, "exp_ekmett_lb");
    benchmark([](float x) { return fast::exp10::exp_ekmett_ub(x); }, "exp_ekmett_ub");
    benchmark([](float x) { return fast::exp10::exp_schraudolph(x); }, "exp_schraudolph");
    benchmark([](float x) { return fast::exp10::exp_mineiro(x); }, "exp_mineiro");
    benchmark([](float x) { return fast::exp10::exp_mineiro_faster(x); }, "exp_mineiro_faster");

    log_db_to_gain([](float x) { return fast::exp10::powx_stl(x); }, "powx_stl");
    log_db_to_gain([](float x) { return fast::exp10::powx_ekmett_fast(x); }, "powx_ekmett_fast");
    log_db_to_gain([](float x) { return fast::exp10::powx_ekmett_fast_lb(x); }, "powx_ekmett_fast_lb");
    log_db_to_gain([](float x) { return fast::exp10::powx_ekmett_fast_ub(x); }, "powx_ekmett_fast_ub");
    log_db_to_gain([](float x) { return fast::exp10::powx_ekmett_fast_precise(x); }, "powx_ekmett_fast_precise");
    log_db_to_gain([](float x) { return fast::exp10::powx_ekmett_fast_better_precise(x); }, "powx_ekmett_fast_better_precise");
    log_db_to_gain([](float x) { return fast::exp10::exp_stl(x); }, "exp_stl");
    log_db_to_gain([](float x) { return fast::exp10::exp_ekmett_lb(x); }, "exp_ekmett_lb");
    log_db_to_gain([](float x) { return fast::exp10::exp_ekmett_ub(x); }, "exp_ekmett_ub");
    log_db_to_gain([](float x) { return fast::exp10::exp_schraudolph(x); }, "exp_schraudolph");
    log_db_to_gain([](float x) { return fast::exp10::exp_mineiro(x); }, "exp_mineiro");
    log_db_to_gain([](float x) { return fast::exp10::exp_mineiro_faster(x); }, "exp_mineiro_faster");
    */

    /** SQRT */
    /**
    benchmark(fast::sqrt::stl, "stl");
    benchmark(fast::sqrt::bigtailwolf, "bigtailwolf");
    benchmark(fast::sqrt::nimig18, "nimig18"); // awful

    log_sqrt(fast::sqrt::stl, "stl");
    log_sqrt(fast::sqrt::bigtailwolf, "bigtailwolf");
    log_sqrt(fast::sqrt::nimig18, "nimig18");
    */
}
