#pragma once

#include <memory>
#include <vector>
#include <array>
#include <stdexcept>
#include "mapping.h"
#include "index_set.h"
#include "print_utils.h"

template<typename mU>
class Tensor {
    /*
     * Abstract class to represent a mapping from an index set to a value set.
     * The value set is fixed to the float numbers for now, for simplicity. 
     */
private:
    std::unique_ptr<IndexSet<mU>> index_set_ptr;
    virtual float call(mU index) = 0;
public:
    Tensor(std::unique_ptr<IndexSet<mU>> index_set_ptr) : index_set_ptr(std::move(index_set_ptr)) {}
    virtual ~Tensor() = default;
    virtual float operator()(mU index) final {
        if (! this->contains(index)) {
            throw std::invalid_argument("Index out of bounds");
        }
        return this->call(index);
    }
    bool contains(mU index) {return this->index_set_ptr->contains(index);}
};

class Physical : public Tensor<size_t> {
private:
    std::vector<float> values;
public:
    Physical(std::vector<float> values)
        : Tensor<size_t>(std::make_unique<IndexRange>(values.size())),
          values(std::move(values)) {}
    virtual float call(size_t index) override {return this->values[index];}
    void print() {
        print_utils::print_tensor<1>({this->values.size()}, [this](const std::array<size_t, 1>& index) {
            return this->values[index[0]];
        });
    }
};

template<int NDIM>
class Strided : public Tensor<std::array<size_t, NDIM>> {
private:
    std::shared_ptr<Physical> U_ptr;
    Stride<NDIM> f;
    std::array<size_t, NDIM> shape;

public:
    Strided(std::shared_ptr<Physical> U_ptr, Stride<NDIM> stride, IndexBox<NDIM> index_box)
        : Tensor<std::array<size_t, NDIM>>(std::make_unique<IndexBox<NDIM>>(index_box)),
          U_ptr(U_ptr),
          f(stride),
          shape(index_box.shape()) {}
    virtual float call(std::array<size_t, NDIM> index) override {
        return this->U_ptr.get()->operator()(this->f(index));
    }

    void print() {
        print_utils::print_tensor<NDIM>(this->shape, [this](const std::array<size_t, NDIM>& index) {
            return (*this)(index);
        });
    }
};