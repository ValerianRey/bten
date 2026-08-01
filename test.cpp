#include <iostream>
#include <vector>
#include "tensor.h"
#include "mapping.h"


int main() {
    std::vector<float> v{1.F, 2.F, 3.F, 4.F, 5.F, 6.F};
    auto U_ptr = std::make_shared<Physical>(v);
    Stride2D s({1, 3});
    Strided2D V(U_ptr, s);
    
    std::pair<int, int> index{2, 1};
    std::cout << "V(" << index.first << ", " << index.second << ") = " << V(index) << std::endl;

    return 0;
}