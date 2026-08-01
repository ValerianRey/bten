#pragma once

#include <array>
#include <iostream>
#include <sstream>
#include <string>

namespace print_utils {

inline std::string format_value(float value) {
    std::ostringstream oss;
    oss << value;
    std::string s = oss.str();
    if (s.find('.') == std::string::npos && s.find('e') == std::string::npos) {
        s += ".";
    }
    return s;
}

template<int NDIM, typename Accessor>
void print_recursive(const std::array<size_t, NDIM>& shape, std::array<size_t, NDIM>& index,
                      size_t dim, const std::string& indent, const Accessor& accessor) {
    std::cout << "[";
    for (size_t i = 0; i < shape[dim]; i++) {
        index[dim] = i;
        if (dim == static_cast<size_t>(NDIM) - 1) {
            std::cout << format_value(accessor(index));
        } else {
            print_recursive<NDIM>(shape, index, dim + 1, indent + " ", accessor);
        }
        if (i + 1 < shape[dim]) {
            if (dim == static_cast<size_t>(NDIM) - 1) {
                std::cout << ", ";
            } else {
                std::cout << ",\n";
                if (dim + 2 < static_cast<size_t>(NDIM)) {
                    std::cout << "\n" << indent;
                }
                std::cout << indent;
            }
        }
    }
    std::cout << "]";
}

template<int NDIM, typename Accessor>
void print_tensor(const std::array<size_t, NDIM>& shape, const Accessor& accessor) {
    std::array<size_t, NDIM> index{};
    std::cout << "tensor(";
    print_recursive<NDIM>(shape, index, 0, std::string(8, ' '), accessor);
    std::cout << ")\n";
}

}
