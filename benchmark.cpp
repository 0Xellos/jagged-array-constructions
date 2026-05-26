#include <random>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "flat-vector.hpp"
#include "nested-vector.hpp"
#include "two-step-construction.hpp"

namespace {

using DataT = std::int32_t;

TEST_CASE("Benchmark jagged array constructions") {
    constexpr int N = 1'000'00;
    constexpr int Q = 10'000'000;

    std::mt19937 rng(123);

    std::vector<int> x(Q);
    std::vector<DataT> y(Q);
    for (int i = 0; i < Q; i++) {
        x[i] = rng() % N;
        y[i] = rng();
    }

    SECTION("Nested vector", "[benchmark][nested][onestep]") {
        BENCHMARK("Nested vector") {
            auto ret = nested_vector::construct<DataT>(N, x, y);
        };
    }

    SECTION("Flat vector", "[benchmark][flat][onestep]") {
        BENCHMARK("Flat vector") {
            auto ret = flat_vector::construct<DataT>(N, x, y);
        };
    }

    SECTION("Two-step (with nested vector)", "[benchmark][nested][twostep]") {
        BENCHMARK("Two-step (with nested vector), original") {
            two_step_construction::construct_original(N, x, y);
        };

        // BENCHMARK("Two-step (with nested vector)") {
        //     auto ret = two_step_construction::construct<DataT>(N, x, y);
        // };
    }
}

}
