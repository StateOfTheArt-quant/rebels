#pragma once

#include "rebels/data/basic.h"
#include <map>
#include <memory>
#include <vector>
#include <unordered_map>

/// Base class of all different data sources,it contains 2 basic apis.
/// history_bars       -- single instrument data if existed
/// multi_history_bars -- multi instruments data if existed
class AbstractDataSource {
public:
    // short declare of bar data type
    using RECORD  = std::tuple<double, double, double, double, double, double>;
    using BAR = std::map<std::string, std::map<int, RECORD>>;

    virtual std::map<int, RECORD> history_bars(std::string instrument_id, int bar_count, int end_dt) = 0;
    // virtual BAR multi_history_bars(std::vector<std::string> instrument_ids,
    //                                        int bar_count,
    //                                        int end_dt)
    //     = 0;
};

class DataSource : public AbstractDataSource {
private:
    int __bar_count;

public:
    /// bar_reader structure:
    /// {
    ///  "000001.XSHE": <object Bar at 0x0151172>,
    ///  "000002.XSHE": <object Bar at 0x0161315>
    /// }
    std::map<std::string, std::shared_ptr<data::basic::Bar>> bar_reader;
    std::map<std::string, std::string> path_dict;
    /// consumed_data structure:
    /// {
    ///  "000001.XSHE": {20000102: BAR1,
    ///                  20000103: BAR2,
    ///                  20000104: BAR3},
    ///  "000002.XSHE": {20000101: BAR1,
    ///                  20000102: BAR2,
    ///                  20000103: BAR3}
    /// }
    BAR consumed_data;

    DataSource() = default;
    DataSource(std::map<std::string, std::string> path_map, int bar_count);

public:
    std::map<int, RECORD> history_bars(std::string instrument_id, int bar_count, int end_dt);
    // BAR multi_history_bars(std::vector<std::string> instrument_ids,
    //                                        int bar_count,
    //                                        int end_dt);

    // get last date close price
    double get_last_price(std::string instrument_id, int dt);
    // get next date close price
    double get_next_price(std::string instrument_id, int dt);

    // previous date range
    int get_previous_trading_date(int end_dt, int bar_count = 1);

    // back test only
    BAR step();
    BAR reset();
};
