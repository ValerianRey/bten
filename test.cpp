#include <iostream>
#include <vector>
#include <array>
#include "tensor.h"
#include "mapping.h"


int main() {
    std::vector<float> v{1.F, 2.F, 3.F, 4.F, 5.F, 6.F};
    auto U_ptr = std::make_shared<Physical<6>>(v);
    constexpr std::array<size_t, 2> V_size {3, 2};
    constexpr std::array<size_t, 2> W_size {6, 5};

    // Index sets
    using mU = Intdex<6>;
    using mV = Multintdex<2, V_size>;
    using mW = Multintdex<2, W_size>;

    // Mappings
    Stride<mV, mU> stride({1, 3});
    auto coo = std::make_shared<COO<2, W_size, 6>>(std::array<std::array<size_t, 2>, 6>{{{0, 0}, {0, 3}, {4, 2}, {5, 4}, {1, 3}, {2, 0}}});

    // Tensors
    Strided<2, V_size, 6> V(U_ptr, stride);
    Sum<mW, mU> W(U_ptr, coo);
    Product<mW, mU> X(U_ptr, coo);
    Sup<mW, mU> Y(U_ptr, coo);

    // Printing
    std::cout<<"V:\n";
    V.print();
    std::cout<<"\nW:\n";
    W.print();
    std::cout<<"\nX:\n";
    X.print();
    std::cout<<"\nY:\n";
    Y.print();

    std::array<size_t, 2> index{2, 1};
    std::cout << "V(" << index[0] << ", " << index[1] << ") = " << V(index) << std::endl;

    return 0;
}