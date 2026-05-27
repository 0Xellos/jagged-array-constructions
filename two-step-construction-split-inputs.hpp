#ifndef JAGGED_ARRAY_CONSTRUCTIONS_TWO_STEP_CONSTRUCTION_SPLIT_INPUTS_H
#define JAGGED_ARRAY_CONSTRUCTIONS_TWO_STEP_CONSTRUCTION_SPLIT_INPUTS_H
#include <vector>

namespace two_step_construction_split_inputs {

template <typename T, int GROUP_WIDTH = 1000>
[[nodiscard]] inline std::vector<std::vector<T>> construct(int n, const std::vector<int> & x, const std::vector<T> & y) {
    struct Group {
        std::vector<int> x;
        std::vector<T> y;
    };

    const int groups_count = (n - 1) / GROUP_WIDTH + 1;

    std::vector<int> group_sizes(groups_count, 0);
    for (const int i : x) { ++group_sizes[i / GROUP_WIDTH]; }

    std::vector<Group> groups(groups_count);
    for (int i = 0; i < groups_count; i++) {
        groups[i].x.reserve(group_sizes[i]);
        groups[i].y.reserve(group_sizes[i]);
    }
    for (int i = 0; i < static_cast<int>(x.size()); i++) {
        groups[x[i] / GROUP_WIDTH].x.push_back(x[i]);
        groups[x[i] / GROUP_WIDTH].y.push_back(y[i]);
    }

    std::vector<std::vector<T>> result(n);

    for (int group = 0; group < groups_count; group++) {
        const auto & g = groups[group];
        for (int i = 0; i < static_cast<int>(g.x.size()); i++) {
            result[g.x[i]].push_back(g.y[i]);
        }
    }

    return result;
}

}  // namespace two_step_construction_split_inputs

#endif  // JAGGED_ARRAY_CONSTRUCTIONS_TWO_STEP_CONSTRUCTION_SPLIT_INPUTS_H
