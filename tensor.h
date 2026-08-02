#pragma once

#include <memory>
#include <vector>
#include <array>
#include <stdexcept>
#include "mapping.h"
#include "index.h"
#include "print_utils.h"

template<typename mU>
requires IsIndex<mU>
class Tensor {
    /*
     * Abstract class to represent a mapping from an index set to a value set.
     * The value set is fixed to the float numbers for now, for simplicity. 
     */
private:
public:
    virtual ~Tensor() = default;
    virtual float operator()(mU index) = 0;
};

template<int SIZE>
class Physical : public Tensor<Intdex<SIZE>> {
private:
    std::vector<float> values;
public:
    Physical(std::vector<float> values) : values(std::move(values)) {}
    virtual float operator()(Intdex<SIZE> index) override {return this->values[index.get()];}
    void print() {
        print_utils::print_tensor<1>({SIZE}, [this](const std::array<size_t, 1>& index) {
            return this->values[index[0]];
        });
    }
};

template<int NDIM, std::array<size_t, NDIM> SIZE_V, size_t SIZE_U>
class Strided : public Tensor<Multintdex<NDIM, SIZE_V>> {
private:
    std::shared_ptr<Physical<SIZE_U>> U_ptr;
    Stride<Multintdex<NDIM, SIZE_V>, Intdex<SIZE_U>> f;

public:
    Strided(std::shared_ptr<Physical<SIZE_U>> U_ptr, Stride<Multintdex<NDIM, SIZE_V>, Intdex<SIZE_U>> stride)
        : U_ptr(U_ptr), f(stride) {}
    virtual float operator()(Multintdex<NDIM, SIZE_V> index) override {
        return this->U_ptr.get()->operator()(this->f(index));
    }

    void print() {
        print_utils::print_tensor<NDIM>(SIZE_V, [this](const std::array<size_t, NDIM>& index) {
            return (*this)(index);
        });
    }
};