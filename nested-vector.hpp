#ifndef JAGGED_ARRAY_CONSTRUCTIONS_NESTED_VECTOR_H
#define JAGGED_ARRAY_CONSTRUCTIONS_NESTED_VECTOR_H
#include <vector>

namespace nested_vector {

template <typename T>
[[nodiscard]] inline std::vector< std::vector<T> > construct(int n, const std::vector<int> & x, const std::vector<T> & y) {
    std::vector< std::vector<T> > result(n);
    for (int i = 0; i < static_cast<int>(x.size()); i++) {
        result[x[i]].push_back(y[i]);
    }
    return result;
}

}  // namespace nested_vector

#endif  // JAGGED_ARRAY_CONSTRUCTIONS_NESTED_VECTOR_H
