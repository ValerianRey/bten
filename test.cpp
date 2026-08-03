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
    using mD = Multintdex<2, uniform_size<2>(6)>;
    using mE = Multintdex<3, uniform_size<3>(6)>;
    using mI = Intdex<0>;

    // Mappings
    Stride<mV, mU> stride({1, 3});
    auto coo = std::make_shared<COO<2, W_size, 6>>(std::array<std::array<size_t, 2>, 6>{{{0, 0}, {0, 3}, {4, 2}, {5, 4}, {1, 3}, {2, 0}}});
    auto diag = std::make_shared<Diagonal<2, 6>>();
    auto diag3 = std::make_shared<Diagonal<3, 6>>();
    auto empty_map = std::make_shared<EmptyMapping<mU, mI>>();

    // Tensors
    Strided<2, V_size, 6> V(U_ptr, stride);
    Sum<mW, mU> W(U_ptr, coo);
    Product<mW, mU> X(U_ptr, coo);
    Sup<mW, mU> Y(U_ptr, coo);
    Sum<mD, mU> D(U_ptr, diag);
    Sum<mE, mU> E(U_ptr, diag3);
    auto empty_ptr = std::make_shared<Physical<0>>(std::vector<float>{});
    auto inner_ptr = std::make_shared<Product<mU, mI>>(empty_ptr, empty_map);
    Sum<mD, mU> Identity(inner_ptr, diag);

    // Printing
    std::cout<<"V:\n";
    V.print();
    std::cout<<"\nW:\n";
    W.print();
    std::cout<<"\nX:\n";
    X.print();
    std::cout<<"\nY:\n";
    Y.print();
    std::cout<<"\nD:\n";
    D.print();
    std::cout<<"\nE:\n";
    E.print();
    std::cout<<"\nIdentity:\n";
    Identity.print();

    std::array<size_t, 2> index{2, 1};
    std::cout << "V(" << index[0] << ", " << index[1] << ") = " << V(index) << std::endl;

    return 0;
}