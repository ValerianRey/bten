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
    using mU = Intdex<SIZE>;
private:
    std::vector<float> values;
public:
    Physical(std::vector<float> values) : values(std::move(values)) {}
    virtual float operator()(mU index) override {return this->values[index.get()];}
    void print() {
        print_utils::print_tensor<1>({SIZE}, [this](const std::array<size_t, 1>& index) {
            return this->values[index[0]];
        });
    }
};

template<int NDIM, std::array<size_t, NDIM> SIZE_V, size_t SIZE_U>
class Strided : public Tensor<Multintdex<NDIM, SIZE_V>> {
    using mV = Multintdex<NDIM, SIZE_V>;
    using mU = Intdex<SIZE_U>;
private:
    std::shared_ptr<Physical<SIZE_U>> U_ptr;
    Stride<mV, mU> f;

public:
    Strided(std::shared_ptr<Physical<SIZE_U>> U_ptr, Stride<mV, mU> stride)
        : U_ptr(U_ptr), f(stride) {}
    virtual float operator()(mV index) override {
        mU i = f(index);
        return U_ptr.get()->operator()(i);
    }

    void print() {
        print_utils::print_tensor<NDIM>(SIZE_V, [this](const std::array<size_t, NDIM>& index) {
            return (*this)(index);
        });
    }
};

template<typename mV, typename mU>
requires IsIndex<mV> && IsIndex<mU>
class Sum : public Tensor<mV> {
private:
    std::shared_ptr<Tensor<mU>> U_ptr;
    std::shared_ptr<PowersetMapping<mV, mU>> mapping;

public:
    Sum(std::shared_ptr<Tensor<mU>> U_ptr, std::shared_ptr<PowersetMapping<mV, mU>> mapping)
        : U_ptr(U_ptr), mapping(mapping) {}

    virtual float operator()(mV index) override {
        Powerset<mU> indices = mapping.get()->operator()(index);
        float result = 0.F;
        for (const mU& i : indices) {
            result += U_ptr.get()->operator()(i);
        }
        return result;
    }

    void print() {
        print_utils::print_tensor<mV::ndim>(mV::size, [this](const std::array<size_t, mV::ndim>& index) {
            return (*this)(index);
        });
    }
};