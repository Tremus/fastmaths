#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <random>

// #include "sin.hpp"
// #include "cos.hpp"
// #include "tanh.hpp"
// #include "log.hpp"
#include "log2.hpp"
// #include "log10.hpp"
// #include "pow.hpp"
// #include "exp.hpp"


template <typename F, typename S>
void log_hz_to_midi (F func, S name) {
    constexpr float frequencies[] = {
        1.0f, 2.0f, 5.0f,
        10.0f, 20.0f, 50.0f,
        100.0f, 200.0f, 500.0f,
        1000, 2000.0f, 5000.0f,
        10000.0f, 20000.0f
    };
    std::cout << name << " = [";

    auto hz_to_midi = [=](float Hz) { return 69 + func(Hz / 440) * 12; };

    for (const float Hz : frequencies) {
        std::cout << std::setprecision(12) << hz_to_midi(Hz) << ",";
    }
    std::cout << "]" << std::endl;

}

// https://stackoverflow.com/questions/18662261/fastest-implementation-of-sine-cosine-and-square-root-in-c-doesnt-need-to-b

float gen_random() noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1.0, 1.0);
    return dis(gen);
}

volatile float sink{}; // ensures a side effect

int main() {
    // for (const auto x : {0.001f, 0.1f, 0.5f, 1.0f, 2.0f, 3.14f, 4.f, 5.f, 6.f, 9.f}) {
    //     const float sin_x = std::sinf(x);
    //     const float fastSin_x = fastSin(x);
    //     const float sin_approx_x = sin_approx(x);

    //     const float diff_fastSin = sin_x - fastSin_x;
    //     const float diff_sin_approx = sin_x - sin_approx_x;

    //     std::cout
    //         << std::setprecision(12)
    //         << "x = " << x << '\n'
    //         << "fastSin_x diff - " << diff_fastSin << '\n'
    //         << "sin_approx_x diff - " << diff_sin_approx << '\n'
    //         << "winner - " << (std::abs(diff_sin_approx) < std::abs(diff_fastSin)  ? "sin_approx" : "fastSin") << '\n' << std::endl;
    // }

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

    // float hz_to_midi = [](auto log2_func, const float Hz) { return 69 + log2_func(Hz / 440) * 12; };

    /** SINE */
    // benchmark(fast::sin::stl<float>, "stl");
    // benchmark(fast::sin::taylor<float, 5>, "taylor 5");
    // benchmark(fast::sin::taylor<float, 9>, "taylor 9");
    // benchmark(fast::sin::taylor<float, 13>, "taylor 13");
    // benchmark(fast::sin::taylor<float, 17>, "taylor 13");
    // benchmark(fast::sin::taylorN<float, 1>, "taylorN 1");
    // benchmark(fast::sin::taylorN<float, 2>, "taylorN 2");
    // benchmark(fast::sin::taylorN<float, 3>, "taylorN 3");
    // benchmark(fast::sin::taylorN<float, 4>, "taylorN 3");
    // benchmark(fast::sin::tremus_lookup, "tremus_lookup");
    // benchmark(fast::sin::bhaskara_radians<float>, "bhaskara_radians");
    // benchmark(fast::sin::pade<float>, "pade");
    // benchmark(fast::sin::sin_approx<float>, "sin_approx");
    // benchmark(fast::sin::slaru<float>, "slaru");
    // benchmark(fast::sin::juha<float>, "juha");
    // benchmark(fast::sin::mineiro, "mineiro");
    // benchmark(fast::sin::mineiro_faster, "mineiro_faster");
    // benchmark(fast::sin::mineiro_fast_full, "mineiro_fast_full");
    // benchmark(fast::sin::mineiro_faster_full, "mineiro_faster_full");
    // benchmark(fast::sin::njuffa<long double>, "njuffa");

    /** COS */
    // benchmark(fast::cos::stl<float>, "stl");
    // benchmark(fast::cos::pade<float>, "pade");
    // benchmark(fast::cos::milianw<float>, "milianw");
    // benchmark(fast::cos::juha, "juha");
    // benchmark(fast::cos::mineiro, "mineiro");
    // benchmark(fast::cos::mineiro_faster, "mineiro_faster");

    /** TANH */
    // benchmark(fast::tanh::stl<float>, "stl");
    // benchmark(fast::tanh::pade<float>, "pade");
    // benchmark(fast::tanh::c3, "c3");
    // benchmark(fast::tanh::big_bang, "big_bang");
    // benchmark(fast::tanh::spagnum_moss, "spagnum_moss");
    // benchmark(fast::tanh::mineiro, "mineiro");
    // benchmark(fast::tanh::mineiro_faster, "mineiro_faster");

    /** LOG */
    // benchmark(fast::log::stl<float>, "stl");
    // benchmark(fast::log::logNPlusOne<float>, "logNPlusOne");
    // benchmark(fast::log::njuffa, "njuffa");
    // benchmark(fast::log::njuffa_faster, "njuffa_faster");
    // benchmark(fast::log::ankerl32, "ankerl32");
    // benchmark(fast::log::ekmett_lb, "ekmett_lb");
    // benchmark(fast::log::mineiro, "mineiro");
    // benchmark(fast::log::mineiro_faster, "mineiro_faster");

    /** LOG2 */
    benchmark(fast::log2::stl<float>, "stl");
    benchmark(fast::log2::lgeoffroy, "lgeoffroy");
    benchmark(fast::log2::lgeoffroy_accurate, "lgeoffroy_accurate");
    benchmark(fast::log2::jcook, "jcook");
    benchmark(fast::log2::mineiro, "mineiro");
    benchmark(fast::log2::mineiro_faster, "mineiro_faster");
    benchmark(fast::log2::newton, "newton");
    benchmark(fast::log2::log1_njuffa, "log1_njuffa");
    benchmark(fast::log2::log1_njuffa_faster, "log1_njuffa_faster");
    benchmark(fast::log2::log1_ankerl32, "log1_ankerl32");
    benchmark(fast::log2::log1_ekmett_lb, "log1_ekmett_lb");
    benchmark(fast::log2::log1_mineiro, "log1_mineiro");
    benchmark(fast::log2::log1_mineiro_faster, "log1_mineiro_faster");

/**
    log_hz_to_midi(fast::log2::stl<float>, "stl");
    log_hz_to_midi(fast::log2::lgeoffroy, "lgeoffroy");
    log_hz_to_midi(fast::log2::lgeoffroy_accurate, "lgeoffroy_accurate");
    log_hz_to_midi(fast::log2::jcook, "jcook");
    log_hz_to_midi(fast::log2::mineiro, "mineiro");
    log_hz_to_midi(fast::log2::mineiro_faster, "mineiro_faster");
    log_hz_to_midi(fast::log2::newton, "newton");
    log_hz_to_midi(fast::log2::log1_njuffa, "log1_njuffa");
    log_hz_to_midi(fast::log2::log1_njuffa_faster, "log1_njuffa_faster");
    log_hz_to_midi(fast::log2::log1_ankerl32, "log1_ankerl32");
    log_hz_to_midi(fast::log2::log1_ekmett_lb, "log1_ekmett_lb");
    log_hz_to_midi(fast::log2::log1_mineiro, "log1_mineiro");
    log_hz_to_midi(fast::log2::log1_mineiro_faster, "log1_mineiro_faster");
*/

    /** LOG10 */
    // benchmark(fast::log10::stl<float>, "stl");
    // benchmark(fast::log10::jcook, "jcook");
    // benchmark(fast::log10::newton, "newton");
    // benchmark(fast::log10::log1_stl, "log1_stl");
    // benchmark(fast::log10::log1_njuffa, "log1_njuffa");
    // benchmark(fast::log10::log1_njuffa_faster, "log1_njuffa_faster");
    // benchmark(fast::log10::log1_ankerl32, "log1_ankerl32");
    // benchmark(fast::log10::log1_ekmett_lb, "log1_ekmett_lb");
    // benchmark(fast::log10::log1_mineiro, "log1_mineiro");
    // benchmark(fast::log10::log1_mineiro_faster, "log1_mineiro_faster");

    /** POW */
    // benchmark([](double a) { return fast::pow::stl<double>(2.0, a); }, "pow");
    // benchmark([](double a) { return fast::pow::ankerl(2.0, a); }, "ankerl");
    // benchmark([](double a) { return fast::pow::ankerl_precise(2.0, a); }, "ankerl_precise");
    // benchmark([](float a) { return fast::pow::stl<float>(2.f, a); }, "stl");
    // benchmark([](float a) { return fast::pow::ekmett_fast(2.f, a); }, "ekmett_fast");
    // benchmark([](float a) { return fast::pow::ekmett_fast_lb(2.f, a); }, "ekmett_fast_lb");
    // benchmark([](float a) { return fast::pow::ekmett_fast_ub(2.f, a); }, "ekmett_fast_ub");
    // benchmark([](float a) { return fast::pow::ekmett_fast_precise(2.f, a); }, "ekmett_fast_precise");
    // benchmark([](float a) { return fast::pow::ekmett_fast_better_precise(2.f, a); }, "ekmett_fast_better_precise");

    /** EXP */
    // benchmark(fast::exp::stl<float>, "stl");
    // benchmark(fast::exp::ekmett_ub, "ekmett_ub");
    // benchmark(fast::exp::schraudolph, "schraudolph");
    // benchmark(fast::exp::mineiro, "mineiro");
    // benchmark(fast::exp::mineiro_faster, "mineiro_faster");

}