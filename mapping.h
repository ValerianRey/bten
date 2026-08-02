#pragma once

#include <array>
#include "index.h"

// TODO: enforce mV and mU <= Index.
template<typename mV, typename mU>
class IndexMapping {
public:
    virtual ~IndexMapping() = default;
    virtual mU operator()(mV index) = 0;
};

// TODO: enforce SIZE_U = prod(SIZE_V) with a concept
template<size_t NDIM, std::array<size_t, NDIM> SIZE_V, size_t SIZE_U>
class Stride : public IndexMapping<Multintdex<NDIM, SIZE_V>, Intdex<SIZE_U>> {
private:
    std::array<size_t, NDIM> strides;

public:
    Stride(std::array<size_t, NDIM> strides) : strides(strides) {}

    virtual Intdex<SIZE_U> operator()(Multintdex<NDIM, SIZE_V> index) override {
        size_t result = 0;
        for (size_t i = 0; i < NDIM; i++) {
            result += index[i] * strides[i];
        }
        // TODO: note that there could be a way to enforce that check at instanciation time:
        //       we could enforce that the strides are such that the max possible result is less
        //       than SIZE_V.
        return Intdex<SIZE_U>(result);
    }
};