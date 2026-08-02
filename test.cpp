#include <iostream>
#include <vector>
#include <array>
#include "tensor.h"
#include "mapping.h"


int main() {
    std::vector<float> v{1.F, 2.F, 3.F, 4.F, 5.F, 6.F};
    auto U_ptr = std::make_shared<Physical<6>>(v);
    constexpr std::array<size_t, 2> V_size {3, 2};
    using mU = Intdex<6>;
    using mV = Multintdex<2, V_size>;
    Stride<mV, mU> stride({1, 3});
    Strided<2, V_size, 6> V(U_ptr, stride);
    V.print();

    std::array<size_t, 2> index{2, 1};
    std::cout << "V(" << index[0] << ", " << index[1] << ") = " << V(index) << std::endl;

    return 0;
}