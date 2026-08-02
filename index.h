#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>


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
    bool operator==(const Multintdex& other) const { return value == other.value; }
};

template<typename T>
struct is_multintdex : std::false_type {};
template<int NDIM, std::array<size_t, NDIM> SIZE>
struct is_multintdex<Multintdex<NDIM, SIZE>> : std::true_type {};

template<typename T>
concept IsMultintdex = is_multintdex<T>::value;

namespace std {
    template<int NDIM, std::array<size_t, NDIM> SIZE>
    struct hash<Multintdex<NDIM, SIZE>> {
        std::size_t operator()(const Multintdex<NDIM, SIZE>& idx) const noexcept {
            std::size_t seed = 0;
            for (int i = 0; i < NDIM; ++i) {
                seed ^= std::hash<size_t>{}(idx[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}


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

namespace std {
    template<size_t SIZE>
    struct hash<Intdex<SIZE>> {
        std::size_t operator()(const Intdex<SIZE>& idx) const noexcept {
            return std::hash<size_t>{}(idx.get());
        }
    };
}

template<typename T>
requires IsIndex<T>
class Powerset : public Index {
private:
    std::unordered_set<T> values;
public:
    Powerset() = default;
    Powerset(std::initializer_list<T> init) : values(init) {}

    bool contains(const T& value) const { return values.contains(value); }
    size_t size() const { return values.size(); }
    void insert(const T& value) { values.insert(value); }

    auto begin() const { return values.begin(); }
    auto end() const { return values.end(); }

    bool operator==(const Powerset& other) const { return values == other.values; }
};

namespace std {
    template<typename T>
    struct hash<Powerset<T>> {
        std::size_t operator()(const Powerset<T>& s) const noexcept {
            std::size_t seed = 0;
            for (const auto& elem : s) {
                seed ^= std::hash<T>{}(elem);
            }
            return seed;
        }
    };
}