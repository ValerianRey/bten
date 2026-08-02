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

template<typename mV, typename mU>
requires IsIndex<mV> && IsIndex<mU>
class PowersetMapping : public IndexMapping<mV, Powerset<mU>> {};

template<int NNZ, int NDIM, std::array<size_t, NDIM> SIZE_V, size_t SIZE_U>
class COO : public PowersetMapping<Multintdex<NDIM, SIZE_V>, Intdex<SIZE_U>> {
private:
    std::array<std::array<size_t, NDIM>, NNZ> inverse_coords;
public:
    COO(std::array<std::array<size_t, NDIM>, NNZ> inverse_coords) : inverse_coords(inverse_coords) {}

    virtual Powerset<Intdex<SIZE_U>> operator()(Multintdex<NDIM, SIZE_V> index) override {
        Powerset<Intdex<SIZE_U>> result;
        for (int j = 0; j < NNZ; j++) {
            bool match = true;
            for (int i = 0; i < NDIM; i++) {
                if (inverse_coords[j][i] != index[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                result.insert(Intdex<SIZE_U>(j));
            }
        }
        return result;
    }
};