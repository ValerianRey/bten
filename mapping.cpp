#include "mapping.h"

int Stride2D::operator()(std::pair<int, int> index) {
    return index.first * strides.first + index.second * strides.second;
}
