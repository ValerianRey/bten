#pragma once

#include <vector>
#include <utility>

template<typename mV, typename mU>
class IndexMapping {
public:
    virtual mU operator()(mV index) = 0;
};


class Stride2D : public IndexMapping<std::pair<int, int>, int> {
private:
    std::pair<int, int> strides;

public:
    Stride2D(std::pair<int, int> strides) : strides(strides) {}

    virtual int operator()(std::pair<int, int> index) override;
};