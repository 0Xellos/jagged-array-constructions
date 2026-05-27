#ifndef JAGGED_ARRAY_CONSTRUCTIONS_FLAT_RESULT_TWO_STEP_CONSTRUCTION_H
#define JAGGED_ARRAY_CONSTRUCTIONS_FLAT_RESULT_TWO_STEP_CONSTRUCTION_H
#include <span>
#include <vector>

namespace flat_result_two_step_construction {

template <typename T>
class JaggedArray {
    std::vector<int> sizes, offsets;
    std::vector<T> storage;
    std::vector<T *> view_as_nested, current_ends;

public:
    JaggedArray(int n, const std::vector<int> & indices) : sizes(n, 0), offsets{0} {
        for (const auto x : indices) { sizes[x]++; }
        offsets.reserve(n + 1);
        for (int i = 0; i < n; i++) {
            offsets.push_back(offsets[i] + sizes[i]);
        }
        storage.resize(offsets[n]);
        view_as_nested.resize(n);
        for (int i = 0; i < n; i++) {
            view_as_nested[i] = storage.data() + offsets[i];
        }
        current_ends = view_as_nested;
    }

    void push_back(int x, const T & y) {
        *(current_ends[x]) = y;
        ++current_ends[x];
    }

    std::span<const T> operator[](int x) const {
        return {view_as_nested[x], static_cast<std::size_t>(sizes[x])};
    }
};

template <typename T, int GROUP_WIDTH = 1000>
[[nodiscard]] inline JaggedArray<T> construct(int n, const std::vector<int> & x, const std::vector<T> & y) {
    struct Group {
        std::vector<std::pair<int, T>> xy;
    };

    const int groups_count = (n - 1) / GROUP_WIDTH + 1;

    std::vector<int> group_sizes(groups_count, 0);
    for (const int i : x) { ++group_sizes[i / GROUP_WIDTH]; }

    std::vector<Group> groups(groups_count);
    for (int i = 0; i < groups_count; i++) {
        groups[i].xy.reserve(group_sizes[i]);
    }
    for (int i = 0; i < static_cast<int>(x.size()); i++) {
        groups[x[i] / GROUP_WIDTH].xy.emplace_back(x[i], y[i]);
    }

    JaggedArray<T> result(n, x);
    for (int group = 0; group < groups_count; group++) {
        for (auto [px, py] : groups[group].xy) { result.push_back(px, py); }
    }
    return result;
}

}  // namespace flat_result_two_step_construction

#endif  // JAGGED_ARRAY_CONSTRUCTIONS_FLAT_RESULT_TWO_STEP_CONSTRUCTION_H
