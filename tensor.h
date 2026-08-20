#pragma once

#include <algorithm>
#include <concepts>
#include <limits>
#include <memory>
#include <string>
#include <vector>
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

class Physical : public Tensor<Intdex> {
private:
    std::vector<float> values;
public:
    Physical(std::vector<float>&& values) : values(std::move(values)) {}
    virtual float operator()(Intdex index) override { return this->values.at(index.get()); }
    std::string str() {
        return print_utils::format_tensor({values.size()}, [this](const std::vector<size_t>& index) {
            return this->values[index[0]];
        });
    }
};

class Strided : public Tensor<Multintdex> {
private:
    std::shared_ptr<Physical> U_ptr;
    Stride f;
    std::vector<size_t> shape;

public:
    Strided(std::shared_ptr<Physical> U_ptr, Stride stride, std::vector<size_t> shape)
        : U_ptr(std::move(U_ptr)), f(std::move(stride)), shape(std::move(shape)) {}
    virtual float operator()(Multintdex index) override {
        Intdex i = f(index);
        return U_ptr->operator()(i);
    }

    std::string str() {
        return print_utils::format_tensor(shape, [this](const std::vector<size_t>& index) {
            return (*this)(Multintdex(index));
        });
    }
};

struct SumReduction {
    static constexpr float neutral = 0.F;
    static float combine(float a, float b) { return a + b; }
};

struct ProductReduction {
    static constexpr float neutral = 1.F;
    static float combine(float a, float b) { return a * b; }
};

struct SupReduction {
    static constexpr float neutral = -std::numeric_limits<float>::infinity();
    static float combine(float a, float b) { return std::max(a, b); }
};

struct InfReduction {
    static constexpr float neutral = std::numeric_limits<float>::infinity();
    static float combine(float a, float b) { return std::min(a, b); }
};

template<typename R>
concept IsReduction = requires(float a, float b) {
    { R::neutral } -> std::convertible_to<float>;
    { R::combine(a, b) } -> std::convertible_to<float>;
};

template<typename mV, typename mU, typename R>
requires IsIndex<mV> && IsIndex<mU> && IsReduction<R>
class ReductionTensor : public Tensor<mV> {
private:
    std::shared_ptr<Tensor<mU>> U_ptr;
    std::shared_ptr<PowersetMapping<mV, mU>> mapping;
    std::vector<size_t> shape;

public:
    ReductionTensor(std::shared_ptr<Tensor<mU>> U_ptr, std::shared_ptr<PowersetMapping<mV, mU>> mapping,
                     std::vector<size_t> shape)
        : U_ptr(std::move(U_ptr)), mapping(std::move(mapping)), shape(std::move(shape)) {}

    virtual float operator()(mV index) override {
        Powerset<mU> indices = mapping->operator()(index);
        float result = R::neutral;
        for (const mU& i : indices) {
            result = R::combine(result, U_ptr->operator()(i));
        }
        return result;
    }

    std::string str() {
        return print_utils::format_tensor(shape, [this](const std::vector<size_t>& index) {
            return (*this)(mV(index));
        });
    }
};

template<typename mV, typename mU>
using Sum = ReductionTensor<mV, mU, SumReduction>;

template<typename mV, typename mU>
using Product = ReductionTensor<mV, mU, ProductReduction>;

template<typename mV, typename mU>
using Sup = ReductionTensor<mV, mU, SupReduction>;

template<typename mV, typename mU>
using Inf = ReductionTensor<mV, mU, InfReduction>;
