#pragma once

#include <array>
#include <cstddef>
#include <type_traits>


class Index {
public:
    virtual ~Index() = default;
};


template<int NDIM, std::array<size_t, NDIM> SIZE>
class Multintdex {
private:
    std::array<size_t, NDIM> value;
public:
    Multintdex(std::array<size_t, NDIM> value) : value(value) {
        for (int i = 0; i < NDIM; i++) {
            if(value[i] >= SIZE[i]) {
                throw std::invalid_argument("Index out of range");
            }
        }
    }
    size_t operator[](int i) const { return value[i]; }
};


template<size_t SIZE>
class Intdex : public Multintdex<1, std::array<size_t, 1>{SIZE}> {
public:
    Intdex(size_t value) : Multintdex<1, std::array<size_t, 1>{SIZE}>({value}) {};
    size_t get() const { return (*this)[0]; }
};
