#pragma once

#include <array>

template<typename T>
class IndexSet {
public:
    virtual bool contains(const T& index) = 0;
    virtual ~IndexSet() = default;
};

template<int NDIM>
class IndexBox : public IndexSet<std::array<size_t, NDIM>> {
private:
    std::array<size_t, NDIM> size;
public:
    IndexBox(const std::array<size_t, NDIM>& size) : size(size) {}
    const std::array<size_t, NDIM>& shape() const { return size; }
    virtual bool contains(const std::array<size_t, NDIM>& index) override {
        for (unsigned int i = 0; i < index.size(); i++) {
            if (index[i] < 0 || index[i] >= this->size[i]) {
                return false;
            }
        }
        return true;
    }
};

class IndexRange : public IndexSet<size_t> {
private:
    IndexBox<1> box;
public:
    IndexRange(const size_t& size) : box(std::array<size_t, 1>({size})) {}
    virtual bool contains(const size_t& index) override {
        return this->box.contains(std::array<size_t, 1>({index}));
    }
};