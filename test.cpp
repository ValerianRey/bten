#include <iostream>
#include <vector>
#include "tensor.h"
#include "mapping.h"


int main() {
    // Data
    std::vector<float> data{1.F, 2.F, 3.F, 4.F, 5.F, 6.F};

    // Mappings
    Stride stride({1, 3});
    auto coo = std::make_shared<COO>(std::vector<std::vector<size_t>>{{0, 0}, {0, 3}, {4, 2}, {5, 4}, {1, 3}, {2, 0}});
    auto diagonalize = std::make_shared<Diagonal>();
    auto empty_map = std::make_shared<EmptyMapping<Intdex, Intdex>>();

    // Tensors
    auto physical = std::make_shared<Physical>(data);
    Strided strided(physical, stride, {3, 2});
    Sum<Multintdex, Intdex> sparse_coo(physical, coo, {6, 5});
    Product<Multintdex, Intdex> prod_coo(physical, coo, {6, 5});
    Sup<Multintdex, Intdex> sup_coo(physical, coo, {6, 5});
    Sum<Multintdex, Intdex> diag2d(physical, diagonalize, {6, 6});
    Sum<Multintdex, Intdex> diag3d(physical, diagonalize, {6, 6, 6});
    auto empty_ptr = std::make_shared<Physical>(std::vector<float>{});
    auto inner_ptr = std::make_shared<Product<Intdex, Intdex>>(empty_ptr, empty_map, std::vector<size_t>{6});
    Sum<Multintdex, Intdex> identity(inner_ptr, diagonalize, {6, 6});

    // Printing
    std::cout<<"strided:\n";
    strided.print();
    std::cout<<"\nsparse_coo:\n";
    sparse_coo.print();
    std::cout<<"\nprod_coo:\n";
    prod_coo.print();
    std::cout<<"\nsup_coo:\n";
    sup_coo.print();
    std::cout<<"\ndiag2d:\n";
    diag2d.print();
    std::cout<<"\ndiag3d:\n";
    diag3d.print();
    std::cout<<"\nidentity:\n";
    identity.print();

    std::vector<size_t> index{2, 1};
    std::cout << "strided(" << index[0] << ", " << index[1] << ") = " << strided(index) << std::endl;

    return 0;
}
