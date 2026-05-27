#ifndef JAGGED_ARRAY_CONSTRUCTIONS_FLAT_VECTOR_H
#define JAGGED_ARRAY_CONSTRUCTIONS_FLAT_VECTOR_H
#include <vector>

namespace flat_vector {

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

    void emplace_back(int x, T && y) {
        *(current_ends[x]) = std::move(y);
        ++current_ends[x];
    }

    const T * operator[](int x) const {
        return view_as_nested[x];
    }
};

template <typename T>
[[nodiscard]] inline JaggedArray<T> construct(int n, const std::vector<int> & x, const std::vector<T> & y) {
    JaggedArray<T> result(n, x);
    for (int i = 0; i < static_cast<int>(x.size()); i++) {
        result.push_back(x[i], y[i]);
    }
    return result;
}

}  // namespace flat_vector

#endif  // JAGGED_ARRAY_CONSTRUCTIONS_FLAT_VECTOR_H
