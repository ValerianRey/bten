#pragma once

#include <array>
#include "index.h"

template<typename mV, typename mU>
requires IsIndex<mV> && IsIndex<mU>
class IndexMapping {
public:
    virtual ~IndexMapping() = default;
    virtual mU operator()(mV index) = 0;
};

// TODO: enforce SIZE_U = prod(SIZE_V) with a concept
template<typename mV, typename mU>
requires IsMultintdex<mV> && IsIntdex<mU>
class Stride : public IndexMapping<mV, mU> {
private:
    std::array<size_t, mV::ndim> strides;

public:
    Stride(std::array<size_t, mV::ndim> strides) : strides(strides) {}

    virtual mU operator()(mV index) override {
        size_t result = 0;
        for (size_t i = 0; i < mV::ndim; i++) {
            result += index[i] * strides[i];
        }
        // TODO: note that there could be a way to enforce that check at instanciation time:
        //       we could enforce that the strides are such that the max possible result is less
        //       than SIZE_V.
        return mU(result);
    }
};