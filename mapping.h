#pragma once

#include <array>

template<typename mV, typename mU>
class IndexMapping {
public:
    virtual ~IndexMapping() = default;
    virtual mU operator()(mV index) = 0;
};

template<unsigned int NDIM>
class Stride : public IndexMapping<std::array<size_t, NDIM>, size_t> {
private:
    std::array<size_t, NDIM> strides;

public:
    Stride(std::array<size_t, NDIM> strides) : strides(strides) {}

    virtual size_t operator()(std::array<size_t, NDIM> index) override {
        size_t result = 0;
        for (size_t i = 0; i < NDIM; i++) {
            result += index[i] * strides[i];
        }
        return result;
    }
};