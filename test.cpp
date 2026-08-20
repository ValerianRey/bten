#include <iostream>
#include <vector>
#include "tensor.h"
#include "mapping.h"


int main() {
    std::vector<float> v{1.F, 2.F, 3.F, 4.F, 5.F, 6.F};
    auto U_ptr = std::make_shared<Physical>(v);
    std::vector<size_t> V_size {3, 2};
    std::vector<size_t> W_size {6, 5};

    // Mappings
    Stride stride({1, 3});
    auto coo = std::make_shared<COO>(std::vector<std::vector<size_t>>{{0, 0}, {0, 3}, {4, 2}, {5, 4}, {1, 3}, {2, 0}});
    auto diag = std::make_shared<Diagonal>();
    auto empty_map = std::make_shared<EmptyMapping<Intdex, Intdex>>();

    // Tensors
    Strided V(U_ptr, stride, V_size);
    Sum<Multintdex, Intdex> W(U_ptr, coo, W_size);
    Product<Multintdex, Intdex> X(U_ptr, coo, W_size);
    Sup<Multintdex, Intdex> Y(U_ptr, coo, W_size);
    Sum<Multintdex, Intdex> D(U_ptr, diag, {6, 6});
    Sum<Multintdex, Intdex> E(U_ptr, diag, {6, 6, 6});
    auto empty_ptr = std::make_shared<Physical>(std::vector<float>{});
    auto inner_ptr = std::make_shared<Product<Intdex, Intdex>>(empty_ptr, empty_map, std::vector<size_t>{6});
    Sum<Multintdex, Intdex> Identity(inner_ptr, diag, {6, 6});

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

    std::vector<size_t> index{2, 1};
    std::cout << "V(" << index[0] << ", " << index[1] << ") = " << V(index) << std::endl;

    return 0;
}
