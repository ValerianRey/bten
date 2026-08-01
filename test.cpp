#include <iostream>
#include <vector>
#include "tensor.h"
#include "mapping.h"


int main() {
    std::vector<float> v{1.F, 2.F, 3.F, 4.F, 5.F, 6.F};
    auto U_ptr = std::make_shared<Physical>(v);
    Stride<2> s({1, 3});
    IndexBox<2> shape({3, 2});
    Strided<2> V(U_ptr, s, shape);
    
    std::array<size_t, 2> index{2, 1};
    std::cout << "V(" << index[0] << ", " << index[1] << ") = " << V(index) << std::endl;

    return 0;
}