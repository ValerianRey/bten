#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

template<typename Accessor>
size_t max_width(const std::vector<size_t>& shape, std::vector<size_t>& index,
                  size_t dim, const Accessor& accessor) {
    size_t width = 0;
    for (size_t i = 0; i < shape[dim]; i++) {
        index[dim] = i;
        if (dim == shape.size() - 1) {
            width = std::max(width, format_value(accessor(index)).size());
        } else {
            width = std::max(width, max_width(shape, index, dim + 1, accessor));
        }
    }
    return width;
}

template<typename Accessor>
void print_recursive(const std::vector<size_t>& shape, std::vector<size_t>& index,
                      size_t dim, const std::string& indent, size_t width, const Accessor& accessor) {
    std::cout << "[";
    for (size_t i = 0; i < shape[dim]; i++) {
        index[dim] = i;
        if (dim == shape.size() - 1) {
            std::cout << std::right << std::setw(static_cast<int>(width)) << format_value(accessor(index));
        } else {
            print_recursive(shape, index, dim + 1, indent + " ", width, accessor);
        }
        if (i + 1 < shape[dim]) {
            if (dim == shape.size() - 1) {
                std::cout << ", ";
            } else {
                std::cout << ",\n";
                if (dim + 2 < shape.size()) {
                    std::cout << "\n";
                }
                std::cout << indent;
            }
        }
    }
    std::cout << "]";
}

template<typename Accessor>
void print_tensor(const std::vector<size_t>& shape, const Accessor& accessor) {
    std::vector<size_t> index(shape.size(), 0);
    size_t width = max_width(shape, index, 0, accessor);
    std::cout << "tensor(";
    print_recursive(shape, index, 0, std::string(8, ' '), width, accessor);
    std::cout << ")\n";
}

}
