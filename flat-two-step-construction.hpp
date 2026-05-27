#ifndef JAGGED_ARRAY_CONSTRUCTIONS_FLAT_TWO_STEP_CONSTRUCTION_H
#define JAGGED_ARRAY_CONSTRUCTIONS_FLAT_TWO_STEP_CONSTRUCTION_H
#include <span>
#include <vector>

namespace flat_two_step_construction {

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

    void push_back(int x, T && y) {
        *(current_ends[x]) = std::move(y);
        ++current_ends[x];
    }

    template <typename... Args>
    void emplace_back(int x, Args &&... args) {
        *(current_ends[x]) = T{std::forward<Args>(args)...};
        ++current_ends[x];
    }

    std::span<const T> operator[](int x) const {
        return {view_as_nested[x], static_cast<std::size_t>(sizes[x])};
    }
};

template <typename T, int GROUP_WIDTH = 1024>
[[nodiscard]] inline JaggedArray<T> construct(int n, const std::vector<int> & x, const std::vector<T> & y) {
    const int groups_count = (n - 1) / GROUP_WIDTH + 1;

    std::vector<int> group_ids;
    group_ids.reserve(x.size());
    for (const int i : x) { group_ids.push_back(i / GROUP_WIDTH); }

    JaggedArray<std::pair<int, T>> groups(groups_count, group_ids);
    for (int i = 0; i < static_cast<int>(x.size()); i++) {
        groups.emplace_back(group_ids[i], x[i], y[i]);
    }

    JaggedArray<T> result(n, x);
    for (int group = 0; group < groups_count; group++) {
        for (auto [px, py] : groups[group]) { result.push_back(px, py); }
    }
    return result;
}

template <typename T, int GROUP_WIDTH = 1024>
[[nodiscard]] inline JaggedArray<T> construct_custom(int n, const std::vector<int> & x, const std::vector<T> & y) {
    struct Group {
        std::vector<std::pair<int, T>> xy;
    };

    const int groups_count = (n - 1) / GROUP_WIDTH + 1;

    std::vector<int> group_sizes(groups_count, 0);
    for (const int i : x) { ++group_sizes[i / GROUP_WIDTH]; }
    std::vector<int> offsets(groups_count + 1);
    offsets[0] = 0;
    for (int i = 0; i < groups_count; i++) { offsets[i + 1] = offsets[i] + group_sizes[i]; }

    std::vector<std::pair<int, T>> groups(x.size());
    for (int i = 0; i < static_cast<int>(x.size()); i++) {
        groups[offsets[x[i] / GROUP_WIDTH]++] = {x[i], y[i]};
    }

    JaggedArray<T> result(n, x);
    for (int group = 0, i = 0; group < groups_count; group++) {
        for (int j = 0; j < group_sizes[group]; j++, i++) {
            auto [px, py] = groups[i];
            result.push_back(px, py);
        }
    }
    return result;
}

}  // namespace flat_two_step_construction

#endif  // JAGGED_ARRAY_CONSTRUCTIONS_FLAT_TWO_STEP_CONSTRUCTION_H
