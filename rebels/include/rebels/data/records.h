#pragma once

#include <map>
#include <array>
#include <tuple>
#include "rebels/utilities/comparison.h"


namespace data{

    struct Record{
        int date;
        long time;

        void clear();
    };

    struct MarketDepthRecord: Record{
        std::array<double, 5> ask_prices;
        std::array<long, 5> ask_volumes;

        std::array<double, 5> bid_prices;
        std::array<long, 5> bid_volumes;

        void clear();
    };

    struct TimeAndSalesRecord: Record{

        std::map<double, long, FloatComparator<> > transactions;

        double mean_price() const;

        void clear();
    };

    struct BarRecord: Record{
        //[open, high, low, close, adj_close, volume]
        std::tuple<double, double, double, double, double, double> bar;

        void clear();
    };

}