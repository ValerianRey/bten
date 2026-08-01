#pragma once

#include <memory>
#include <vector>
#include <array>
#include "mapping.h"
#include "index_set.h"

template<typename mU>
class Tensor {
    /*
     * Abstract class to represent a mapping from an index set to a value set.
     * The value set is fixed to the float numbers for now, for simplicity. 
     */
private:
    std::unique_ptr<IndexSet<mU>> index_set_ptr;
public:
    Tensor(std::unique_ptr<IndexSet<mU>> index_set_ptr) : index_set_ptr(std::move(index_set_ptr)) {}
    virtual ~Tensor() = default;
    virtual float operator()(mU index) = 0;
};

class Physical : Tensor<size_t> {
private:
    std::vector<float> values;
public:
    Physical(std::vector<float> values)
        : Tensor<size_t>(std::make_unique<IndexRange>(values.size())),
          values(std::move(values)) {}
    virtual float operator()(size_t index) override;
    void print();
};

template<int NDIM>
class Strided : public Tensor<std::array<size_t, NDIM>> {
private:
    std::shared_ptr<Physical> U;
    Stride<NDIM> f;
public:
    Strided(std::shared_ptr<Physical> U_ptr, Stride<NDIM> stride, IndexBox<NDIM> index_box)
        : Tensor<std::array<size_t, NDIM>>(std::make_unique<IndexBox<NDIM>>(index_box)),
          U(U_ptr),
          f(stride) {}
    virtual float operator()(std::array<size_t, NDIM> index) override {
        return this->U.get()->operator()(this->f(index));
    }
};