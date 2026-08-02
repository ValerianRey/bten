#pragma once

#include <array>
#include <cstddef>
#include <type_traits>


class Index {
public:
    virtual ~Index() = default;
};

template<typename T>
concept IsIndex = std::is_base_of_v<Index, T>;


template<int NDIM, std::array<size_t, NDIM> SIZE>
class Multintdex : public Index {
private:
    std::array<size_t, NDIM> value;
public:
    static constexpr int ndim = NDIM;

    Multintdex(std::array<size_t, NDIM> value) : value(value) {
        for (int i = 0; i < NDIM; i++) {
            if(value[i] >= SIZE[i]) {
                throw std::invalid_argument("Index out of range");
            }
        }
    }
    size_t operator[](int i) const { return value[i]; }
};

template<typename T>
struct is_multintdex : std::false_type {};
template<int NDIM, std::array<size_t, NDIM> SIZE>
struct is_multintdex<Multintdex<NDIM, SIZE>> : std::true_type {};

template<typename T>
concept IsMultintdex = is_multintdex<T>::value;


template<size_t SIZE>
class Intdex : public Multintdex<1, std::array<size_t, 1>{SIZE}> {
    using Base = Multintdex<1, std::array<size_t, 1>{SIZE}>;
public:
    Intdex(size_t value) : Base({value}) {}
    size_t get() const { return (*this)[0]; }
};

template<typename T>
struct is_intdex : std::false_type {};
template<size_t SIZE>
struct is_intdex<Intdex<SIZE>> : std::true_type {};

template<typename T>
concept IsIntdex = is_intdex<T>::value;
