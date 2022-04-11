#pragma once

#include <set>
#include <deque>
#include <array>
#include <cstddef>
#include "rebels/data/records.h"

template <typename T>
class Accumulator{

protected:
    const std::size_t window_size;

    std::deque<T> window;

    T _sum = T(0);

public:
    Accumulator(std::size_t window_size);

    virtual void push(T val);

    T sum();

    T at(std::size_t i);
    T front();
    T back();

    void clear();

    bool full();
    std::size_t size();

};

template <typename T>
class RollingMean: public Accumulator<T> {

private:
    T _mean;
    T _s;

public:
    RollingMean(std::size_t window_size);

    void push(T val);

    T mean();
    T var();
    T std();

    T zscore(T val);
    T last_zscore();

};


class DataBuffer {
public:

    typedef std::array<int, 5> arr;

    DataBuffer()=default;

    bool isInited();

    //reset
    void clear();
    void printInfo();

    void push(const data::MarketDepthRecord&);

    //挂单倾斜程度变化率
    int abv_diff();
    double abv_diff_change();
    double abv_diff_ratio();

    //一档挂单量变化
    int av_consume();
    int bv_consume();

    //五档挂单量变化
    int av_consume_5d();
    int bv_consume_5d();

private:
    arr AP, BP, AV, BV;
    arr AP_Last, BP_Last, AV_Last, BV_Last;
    int av1_5, bv1_5, last_av1_5, last_bv1_5;
    int count=0;
    bool inited=false;
    int time;

    int priceInLastAp(int);
    int priceInLastBp(int);
    int priceInCurrAp(int);
    int priceInCurrBp(int);

};