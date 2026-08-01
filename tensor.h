#pragma once

#include <memory>
#include <vector>
#include "mapping.h"

template<typename mU>
class Tensor {
    /*
     * Abstract class to represent a mapping from an index set to a value set.
     * The value set is fixed to the float numbers for now, for simplicity. 
     */
public:
    virtual float operator()(mU index) = 0;
};

class Physical : Tensor<int> {
private:
    std::vector<float> values;
public:
    Physical(const std::vector<float> values) : values(std::move(values)) {}
    virtual float operator()(int index) override;
    void print();
};


class Strided2D : public Tensor<std::pair<int, int>> {
private:
    std::shared_ptr<Physical> U;
    Stride2D f;
public:
    Strided2D(std::shared_ptr<Physical> U_ptr, Stride2D stride) : U(U_ptr), f(stride) {}
    virtual float operator()(std::pair<int, int> index) override;
    void print();
};