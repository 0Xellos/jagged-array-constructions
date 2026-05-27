#include <random>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_get_random_seed.hpp>
#include "flat-two-step-construction.hpp"
#include "flat-vector.hpp"
#include "nested-vector.hpp"
#include "two-step-construction-split-inputs.hpp"
#include "two-step-construction.hpp"
#include "flat-result-two-step-construction.hpp"

namespace {

using DataT = std::int32_t;

struct JaggedArrayConstructionInput {
    int n;
    std::vector<int> x;
    std::vector<DataT> y;
};

JaggedArrayConstructionInput makeInput(const int n, const int q) {
    std::vector<int> x;
    std::vector<DataT> y;
    x.reserve(q);
    y.reserve(q);

    std::mt19937 rng(Catch::getSeed());
    std::uniform_int_distribution<int> x_distribution(0, n - 1);
    std::uniform_int_distribution<DataT> y_distribution;
    for (int i = 0; i < q; i++) {
        x.push_back(x_distribution(rng));
        y.push_back(y_distribution(rng));
    }

    return {.n = n, .x = std::move(x), .y = std::move(y)};
}

TEST_CASE("Jagged array constructions using baseline nested vector", "[benchmark][nested][onestep]") {
    const auto [n, x, y] = makeInput(1'000'000, 100'000'000);

    BENCHMARK("Nested vector") {
        return nested_vector::construct<DataT>(n, x, y);
    };
}

TEST_CASE("Jagged array constructions using flat vector", "[benchmark][flat][onestep]") {
    const auto [n, x, y] = makeInput(1'000'000, 100'000'000);

    BENCHMARK("Flat vector") {
        return flat_vector::construct<DataT>(n, x, y);
    };
}

TEST_CASE("Jagged array constructions using two-step method (with nested vector)", "[benchmark][nested][twostep]") {
    const auto [n, x, y] = makeInput(1'000'000, 100'000'000);

    BENCHMARK("Two-step with nested vector, original") {
        return two_step_construction::constructOriginal(n, x, y);
    };

    BENCHMARK("Two-step with nested vector, variation with split x, y") {
        return two_step_construction_split_inputs::construct<DataT>(n, x, y);
    };

    BENCHMARK("Two-step with nested vector, variation with zipped x, y") {
        return two_step_construction::construct<DataT>(n, x, y);
    };
}

TEST_CASE("Jagged array constructions using two-step method (with flat vector)", "[benchmark][flat][twostep]") {
    const auto [n, x, y] = makeInput(1'000'000, 100'000'000);

    BENCHMARK("Two-step with flat vector") {
        return flat_two_step_construction::construct<DataT>(n, x, y);
    };

    BENCHMARK("Two-step with flat vector, uses custom code for flat groups") {
        return flat_two_step_construction::constructCustom<DataT>(n, x, y);
    };

    BENCHMARK("Two-step with flat result vector only") {
        return flat_result_two_step_construction::construct<DataT>(n, x, y);
    };
}

}
