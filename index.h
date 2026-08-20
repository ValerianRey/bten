#pragma once

#include <cstddef>
#include <functional>
#include <type_traits>
#include <unordered_set>
#include <vector>


class Index {
public:
    virtual ~Index() = default;
};

template<typename T>
concept IsIndex = std::is_base_of_v<Index, T>;


class Multintdex : public Index {
private:
    std::vector<size_t> value;
public:
    Multintdex(std::vector<size_t> value) : value(std::move(value)) {}
    size_t ndim() const { return value.size(); }
    size_t operator[](size_t i) const { return value.at(i); }
    bool operator==(const Multintdex& other) const { return value == other.value; }
};

namespace std {
    template<>
    struct hash<Multintdex> {
        std::size_t operator()(const Multintdex& idx) const noexcept {
            std::size_t seed = 0;
            for (size_t i = 0; i < idx.ndim(); ++i) {
                seed ^= std::hash<size_t>{}(idx[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}


class Intdex : public Multintdex {
public:
    Intdex(size_t value) : Multintdex({value}) {}
    size_t get() const { return (*this)[0]; }
};

namespace std {
    template<>
    struct hash<Intdex> {
        std::size_t operator()(const Intdex& idx) const noexcept {
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
