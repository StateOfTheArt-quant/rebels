#include "rebels/data/streamer.h"
#include <iostream>

using namespace data;

template<typename R>
Streamer<R>::Streamer():
    record_1(),
    record_2(),
    record_3(),

    record_last(record_1),
    record_curr(record_2),
    record_next(record_3){

    static_assert(std::is_base_of<data::Record, R>::value,
            "Streamer record type R must a subclass pf data::Record");
    }


template<typename R>
void Streamer<R>::_RotateRefs(){
    record_last = record_curr;
    record_curr = record_next;
    record_next = record_last;
}

template<typename R>
const R& Streamer<R>::Record(){
    return record_curr;
}

template<typename R>
void Streamer<R>::Reset(){
    record_1.clear();
    record_2.clear();
    record_3.clear();
}

template<typename R>
bool Streamer<R>::LoadNext(){
    //std::cout << "to rotate refs" << std::endl;
    _RotateRefs();
    //std::cout << "rotate successfully" << std::endl;
    record_next.clear();
    //std::cout << "record_next clear successfully" << std::endl;
    return _LoadNext();
}

template<typename R>
bool Streamer<R>::LoadUntil(const data::Record& rec){
    return LoadUntil(rec.date, rec.time);
}

template<typename R>
bool Streamer<R>::LoadUntil(int date, long time){
    if (date < record_curr.date or time < record_curr.time){
        throw std::runtime_error("[Streamer] Attempted to go backwards in time.");
    }

    if (date < record_next.date or time < record_next.time){
        record_curr.clear();

        record_curr.date = date;
        record_curr.time = time;

        return true;
    }

    while (record_next.date < date or _TimeLookAhead() <= time){
        if (not _LoadNext()){
            return false;
        }
    }

    if (date > record_next.date or time > record_next.time){
        record_next.date = date;
        record_next.time = time;
    }

    return LoadNext();
}

template<typename R>
bool Streamer<R>::SkipUntil(const data::Record& rec){
    return SkipUntil(rec.date, rec.time);
}

template<typename R>
bool Streamer<R>::SkipUntil(int date, long time){
    if (date < record_curr.date or time < record_curr.time){
        throw std::runtime_error("[Streamer] Attempted to go backwatds in time.");
    }

    while (record_next.date < date or record_next.time <= time){
        if (not LoadNext()){
            return false;
        }
    }

    if (date > record_next.date or time < record_next.time){
        record_curr.clear();

        record_curr.date = date;
        record_curr.time = time;
    }

    return true;
}

template<typename R>
bool Streamer<R>::HasTimeChanged(){
    return (record_curr.time != record_last.time);
}

template<typename R>
bool Streamer<R>::WillTimeChange(){
    return (record_curr.time != record_next.time);
}

template<typename R>
int Streamer<R>::NextDate(){
    return record_next.date;
}

template<typename R>
long Streamer<R>::NextTime(){
    return record_next.time;
}


// template specialisation
template class data::Streamer<data::MarketDepthRecord>;
template class data::Streamer<data::TimeAndSalesRecord>;
template class data::Streamer<data::BarRecord>;



