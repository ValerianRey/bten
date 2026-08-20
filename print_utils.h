#pragma once

#include <algorithm>
#include <iomanip>
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
void format_recursive(std::ostringstream& out, const std::vector<size_t>& shape, std::vector<size_t>& index,
                       size_t dim, const std::string& indent, size_t width, const Accessor& accessor) {
    out << "[";
    for (size_t i = 0; i < shape[dim]; i++) {
        index[dim] = i;
        if (dim == shape.size() - 1) {
            out << std::right << std::setw(static_cast<int>(width)) << format_value(accessor(index));
        } else {
            format_recursive(out, shape, index, dim + 1, indent + " ", width, accessor);
        }
        if (i + 1 < shape[dim]) {
            if (dim == shape.size() - 1) {
                out << ", ";
            } else {
                out << ",\n";
                if (dim + 2 < shape.size()) {
                    out << "\n";
                }
                out << indent;
            }
        }
    }
    out << "]";
}

template<typename Accessor>
std::string format_tensor(const std::vector<size_t>& shape, const Accessor& accessor) {
    std::vector<size_t> index(shape.size(), 0);
    size_t width = max_width(shape, index, 0, accessor);
    std::ostringstream out;
    out << "tensor(";
    format_recursive(out, shape, index, 0, std::string(8, ' '), width, accessor);
    out << ")";
    return out.str();
}

}
