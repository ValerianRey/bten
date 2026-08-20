#pragma once

#include <vector>
#include "index.h"

template<typename mV, typename mU>
requires IsIndex<mV> && IsIndex<mU>
class IndexMapping {
public:
    virtual ~IndexMapping() = default;
    virtual mU operator()(mV index) = 0;
};

class Stride : public IndexMapping<Multintdex, Intdex> {
private:
    std::vector<size_t> strides;

public:
    Stride(std::vector<size_t> strides) : strides(std::move(strides)) {}

    virtual Intdex operator()(Multintdex index) override {
        size_t result = 0;
        for (size_t i = 0; i < strides.size(); i++) {
            result += index[i] * strides[i];
        }
        return Intdex(result);
    }
};

template<typename mV, typename mU>
requires IsIndex<mV> && IsIndex<mU>
class PowersetMapping : public IndexMapping<mV, Powerset<mU>> {};

template<typename mV, typename mU>
requires IsIndex<mV> && IsIndex<mU>
class EmptyMapping : public PowersetMapping<mV, mU> {
public:
    virtual Powerset<mU> operator()(mV) override {
        return Powerset<mU>();
    }
};

class COO : public PowersetMapping<Multintdex, Intdex> {
private:
    std::vector<std::vector<size_t>> inverse_coords;
public:
    COO(std::vector<std::vector<size_t>> inverse_coords) : inverse_coords(std::move(inverse_coords)) {}

    virtual Powerset<Intdex> operator()(Multintdex index) override {
        Powerset<Intdex> result;
        for (size_t j = 0; j < inverse_coords.size(); j++) {
            bool match = true;
            for (size_t i = 0; i < inverse_coords[j].size(); i++) {
                if (inverse_coords[j][i] != index[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                result.insert(Intdex(j));
            }
        }
        return result;
    }
};

class Diagonal : public PowersetMapping<Multintdex, Intdex> {
public:
    virtual Powerset<Intdex> operator()(Multintdex index) override {
        Powerset<Intdex> result;
        size_t first = index[0];
        for (size_t i = 0; i < index.ndim(); i++) {
            if (index[i] != first) {
                return result;
            }
        }
        result.insert(Intdex(first));
        return result;
    }
};
