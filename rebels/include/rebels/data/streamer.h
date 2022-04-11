#pragma once

#include <map>
#include <string>
#include <vector>

#include "rebels/data/records.h"
#include "rebels/utilities/csv.h"

namespace data {

    template<typename R>
    class Streamer{
    //protected:
    public:
        R record_1;
        R record_2;
        R record_3;

        R& record_last;
        R& record_curr;
        R& record_next;

        Streamer();

        void _RotateRefs();

        virtual bool _LoadNext() = 0;
        virtual long _TimeLookAhead() =0;

    public:
        bool LoadNext();

        bool LoadUntil(const data::Record& rec);
        bool LoadUntil(int date, long time);

        bool SkipUntil(const data::Record& rec);
        bool SkipUntil(int date, long time);

        const R& Record();

        virtual void Reset();
        virtual void SkipN(long n = 1L) = 0;

        bool HasTimeChanged();
        bool WillTimeChange();

        int NextDate();
        long NextTime();
    };

    //template specialization
    class MarketDepth: public Streamer<MarketDepthRecord> {};
    class TimeAndSales: public Streamer<TimeAndSalesRecord> {};
    class Bar: public Streamer<BarRecord> {};
}