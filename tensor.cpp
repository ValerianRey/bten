#include <iostream>
#include "tensor.h"

float Physical::operator()(size_t index) {
    return this->values[index];
}

void Physical::print() {
    std::cout<<"Physical([";
    bool is_first = true;
    for (float value: this->values) {
        if (!is_first) {
            std::cout<<", ";
        }
        is_first = false;
        std::cout<<value;
    }
    std::cout<<"])\n";
}
