#include "rebels/utilities/accumulator.h"

#include <math.h>
#include <numeric>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;

// ------------ Accumulator ------------------
template <typename T>
Accumulator<T>::Accumulator(size_t window_size):
        window_size(window_size),
        window()
{}

template <typename T>
void Accumulator<T>::push(T val)
{
    _sum += val;
    window.push_front(val);

    if (size() > window_size) {
        _sum -= window.back();
        window.pop_back();
    }
}

template <typename T>
T Accumulator<T>::sum()
{
    return _sum;
}

template <typename T>
T Accumulator<T>::at(size_t i)
{
    if (size() > i) return window[i];
    else return -1;
}

template <typename T>
T Accumulator<T>::front()
{
    if (size() == 0)
        throw runtime_error("Attempting to access `front` of an empty accumulator...");

    return window.front();
}

template <typename T>
T Accumulator<T>::back()
{
    if (size() == 0)
        throw runtime_error("Attempting to access `back` of an empty accumulator...");

    return window.back();
}

template <typename T>
void Accumulator<T>::clear()
{
    window.clear();
}

template <typename T>
bool Accumulator<T>::full()
{
    return (window.size() == window_size);
}

template <typename T>
size_t Accumulator<T>::size()
{
    return window.size();
}

// ------------ Rolling Mean --------------------

template <typename T>
RollingMean<T>::RollingMean(size_t window_size):
        Accumulator<T>(window_size),
        _mean(T(0)), _s(T(0))
{}

template <typename T>
void RollingMean<T>::push(T val)
{
    this->_sum += val;
    this->window.push_front(val);

    T n = this->size();
    T _old_mean = _mean;

    _mean += (val - _mean) / n;
    _s += (val - _mean) * (val - _old_mean);

    if (this->size() > this->window_size) {
        T old = this->window.back();
        this->window.pop_back();
        this->_sum -= old;

        T n = this->size();
        T _old_mean = _mean;

        _mean -= (old - _mean) / n;
        _s -= (old - _mean) * (old - _old_mean);
    }
}

template <typename T>
T RollingMean<T>::mean()
{
    return _mean;
}

template <typename T>
T RollingMean<T>::var()
{
    return _s / (T) (this->size() - 1);
}

template <typename T>
T RollingMean<T>::std()
{
    T v = var();
    if (v > 0)
        return sqrt(v);
    else
        return T(0);
}

template <typename T>
T RollingMean<T>::zscore(T val)
{
    T _std = std();
    return _std > 0 ? (val - mean()) / _std : 0;
}

template <typename T>
T RollingMean<T>::last_zscore()
{
    return zscore(this->window.front());
}


//------------ 自定义指标容器 ------------------
bool DataBuffer::isInited() {
    return this->inited;
}

void DataBuffer::clear() {
    this->inited = false;
    this->count = 0;
}

void DataBuffer::push(const data::MarketDepthRecord& rec){
    AP_Last = AP;
    BP_Last = BP;
    BV_Last = BV;
    AV_Last = AV;
    last_av1_5 = av1_5;
    last_bv1_5 = bv1_5;
    time = rec.time;

    copy(rec.ask_prices.cbegin(),rec.ask_prices.cend(),AP.begin());
    copy(rec.bid_prices.cbegin(),rec.bid_prices.cend(),BP.begin());
    copy(rec.ask_volumes.cbegin(),rec.ask_volumes.cend(),AV.begin());
    copy(rec.bid_volumes.cbegin(),rec.bid_volumes.cend(),BV.begin());

    av1_5 = AV[0]+AV[1]+AV[2]+AV[3]+AV[4];
    bv1_5 = BV[0]+BV[1]+BV[2]+BV[3]+BV[4];

    if (!this->inited){
        this->count++;
        if(this->count==10){
            this->inited=true;
        }
    }
}

void DataBuffer::printInfo() {
    printf("Time:%d\nabv_diff:%d\tabv_diff_change:%2f\tabv_diff_ratio:%2f\n",
           time,abv_diff(),abv_diff_change(),abv_diff_ratio());
    printf("av_consume:%d\tbv_consume:%d\tav_consume_5d:%d\tbv_consume_5d:%d\n",
           av_consume(),bv_consume(),av_consume_5d(),bv_consume_5d());
    printf("\n");
}

int DataBuffer::priceInLastAp(int price) {
    int index = find(AP_Last.cbegin(),AP_Last.cend(),price)-AP_Last.cbegin();
    return index;
}
int DataBuffer::priceInLastBp(int price) {
    int index = find(BP_Last.cbegin(),BP_Last.cend(),price)-BP_Last.cbegin();
    return index;
}
int DataBuffer::priceInCurrAp(int price) {
    int index = find(AP.cbegin(),AP.cend(),price)-AP.cbegin();
    return index;
}
int DataBuffer::priceInCurrBp(int price) {
    int index = find(BP.cbegin(),BP.cend(),price)-BP.cbegin();
    return index;
}

int DataBuffer::abv_diff() {
    return av1_5-bv1_5;
}

double DataBuffer::abv_diff_change() {
    return (double)(av1_5-bv1_5-(last_av1_5-last_bv1_5))/(double)(av1_5+bv1_5);
}

double DataBuffer::abv_diff_ratio() {
    return ((double)abs(av1_5-bv1_5)/(double)min(av1_5,bv1_5));
}

int DataBuffer::av_consume() {
    int avc = 0;
    int index = -1;
    if(AP[0]==AP_Last[0]){
        avc = AV_Last[0]-AV[0];
    }
    else if(AP[0]>AP_Last[0]){
        index = priceInLastAp(AP[0]);
        if(index==5)
            avc += (-AV[0]);
        else
            avc += (AV_Last[index]-AV[0]);
        for(int i=0;i!=5;i++){
            if(AP_Last[i]<AP[0])
                avc += AV_Last[i];
        }
    }
    else {
        //AP下移
        index = priceInCurrAp(AP_Last[0]);
        if (index != 5)
            //t0 ap出现在t1的五档中
            avc += (AV_Last[0] - AV[index]);
        else {
            if (AP[4] >= AP_Last[0])
                avc = AV_Last[0];
        }
        for (int i = 0; i != 4; i++) {
            if (AP[i] < AP_Last[0])
                avc -= (AV[i]);
        }
    }
    return avc;
}

int DataBuffer::bv_consume() {
    int bvc = 0;
    int index = -1;
    if(BP[0]==BP_Last[0]){
        bvc = BV_Last[0]-BV[0];
    }
    else if(BP[0]<BP_Last[0]){
        index = priceInLastBp(BP[0]);
        if(index==5)
            bvc += (-BV[0]);
        else
            bvc += (BV_Last[index]-BV[0]);
        for(int i=0;i!=5;i++){
            if(BP_Last[i]>BP[0])
                bvc += BV_Last[i];
        }
    }
    else {
        //BP下移
        index = priceInCurrBp(BP_Last[0]);
        if (index != 5)
            //t0 BP出现在t1的五档中
            bvc += (BV_Last[0] - BV[index]);
        else {
            if (BP[4] <= BP_Last[0])
                bvc = BV_Last[0];
        }
        for (int i = 0; i != 4; i++) {
            if (BP[i] > BP_Last[0])
                bvc -= (BV[i]);
        }
    }
    return bvc;
}

int DataBuffer::av_consume_5d() {
    int avc=0;
    int index=-1;
    if(AP[4]==AP_Last[4]){
        for(int i=0;i!=5;i++){
            //t0的价格是否消失
            index = priceInCurrAp(AP_Last[i]);
            if(index==5)
                avc += AV_Last[i];
            else
                avc += (AV_Last[i]-AV[index]);
        }
        for(int i=0;i!=5;i++){
            //考虑t1的新的价格
            index = priceInLastAp(AP[i]);
            if(index==5)
                avc -= AV[i];
        }
    }
    else if(AP[4]>AP_Last[4]){
        //如果五档价格上升了
        for(int i=0;i!=5;i++){
            index = priceInCurrAp(AP_Last[i]);
            if(index==5)
                avc += (AV_Last[i]);
            else
                avc += (AV_Last[i]-AV[index]);
        }
        for(int i=0;i!=5;i++){
            index = priceInLastAp(AP[i]);
            if(index==5 && AP[i]<=AP_Last[4])
                avc -= (AV[i]);
        }
    }
    else{
        //如果五档价格下降了
        for(int i=0;i!=5;i++) {
            index = priceInCurrAp(AP_Last[i]);
            if (AP_Last[i] <= AP[4]) {
                if (index == 5)
                    avc += (AV_Last[i]);
                else
                    avc += (AV_Last[i] - AV[index]);
            }
        }
        for(int i=0;i!=5;i++){
            index = priceInLastAp(AP[i]);
            if(index==5)
                avc -= (AV[i]);
        }
    }
    return avc;
}

int DataBuffer::bv_consume_5d() {
    int bvc=0;
    int index=-1;
    if(BP[4]==BP_Last[4]){
        for(int i=0;i!=5;i++){
            //t0的价格是否消失
            index = priceInCurrBp(BP_Last[i]);
            if(index==5)
                bvc += BV_Last[i];
            else
                bvc += (BV_Last[i]-BV[index]);
        }
        for(int i=0;i!=5;i++){
            //考虑t1的新的价格
            index = priceInLastBp(BP[i]);
            if(index==5)
                bvc -= BV[i];
        }
    }
    else if(BP[4]<BP_Last[4]){
        //如果五档价格上升了
        for(int i=0;i!=5;i++){
            index = priceInCurrBp(BP_Last[i]);
            if(index==5)
                bvc += (BV_Last[i]);
            else
                bvc += (BV_Last[i]-BV[index]);
        }
        for(int i=0;i!=5;i++){
            index = priceInLastBp(BP[i]);
            if(index==5 && BP[i]>=BP_Last[4])
                bvc -= (BV[4]);
        }
    }
    else{
        //如果五档价格下降了
        for(int i=0;i!=5;i++) {
            index = priceInCurrBp(BP_Last[i]);
            if (BP_Last[i] >= BP[4]) {
                if (index == 5)
                    bvc += (BV_Last[i]);
                else
                    bvc += (BV_Last[i] - BV[index]);
            }
        }
        for(int i=0;i!=5;i++){
            index = priceInLastBp(BP[i]);
            if(index==5)
                bvc -= (BV[i]);
        }
    }
    return bvc;
}

// Explicit implementations
template class Accumulator<int>;
template class Accumulator<float>;
template class Accumulator<double>;

template class RollingMean<int>;
template class RollingMean<float>;
template class RollingMean<double>;
