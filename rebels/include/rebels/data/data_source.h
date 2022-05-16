#pragma once

#include "rebels/data/basic.h"
#include <map>
#include <memory>
#include <vector>
#include <unordered_map>

/// Base class of all different data sources,it contains 2 basic apis.
///
/// API intro:
/// history_bars       -- single instrument data if existed
/// multi_history_bars -- multi instruments data if existed
///
/// Data structure intro:
/// BAR :
/// {20000102: DATA1,
///  20000103: DATA2,
///  20000104: DATA3}
///
/// BAR_MULTI :
/// {"000001.XSHE": {20000102: DATA1,
///                  20000103: DATA2,
///                  20000104: DATA3},
///  "000002.XSHE": {20000101: DATA1,
///                  20000102: DATA2,
///                  20000103: DATA3}}
class AbstractDataSource {
public:
    // short declare of bar data type
    virtual std::map<int, std::tuple<double, double, double, double, double, double>> history_bars(
        std::string instrument_id, int bar_count, int end_dt)
        = 0;
    virtual std::map<std::string,
                     std::map<int, std::tuple<double, double, double, double, double, double>>>
    multi_history_bars(std::vector<std::string> instrument_ids, int bar_count, int end_dt) = 0;
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

    // TODO consider using struct to short define
    std::map<std::string,
             std::map<int, std::tuple<double, double, double, double, double, double>>>
        consumed_data;

    DataSource() = default;
    DataSource(std::map<std::string, std::string> path_map, int bar_count);

public:
    std::map<int, std::tuple<double, double, double, double, double, double>> history_bars(
        std::string instrument_id, int bar_count, int end_dt);
    std::map<std::string,
             std::map<int, std::tuple<double, double, double, double, double, double>>>
    multi_history_bars(std::vector<std::string> instrument_ids, int bar_count, int end_dt);

    // get previous date close price
    double get_previous_price(std::string instrument_id, int dt);
    // get current date close price
    double get_current_price(std::string instrument_id, int dt);
    // get next date close price
    double get_next_price(std::string instrument_id, int dt);

    // given trading date backward for N steps
    int get_previous_trading_date(int end_dt, int bar_count = 1);
    // given trading date forward for N steps
    int get_next_trading_date(int end_dt, int bar_count = 1);

    // back test only
    std::map<std::string,
             std::map<int, std::tuple<double, double, double, double, double, double>>>
    step();
    std::map<std::string,
             std::map<int, std::tuple<double, double, double, double, double, double>>>
    reset();
};
