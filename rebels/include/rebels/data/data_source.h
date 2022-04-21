#pragma once

#include "rebels/data/basic.h"
#include <unordered_map>
#include <map>
#include <vector>

class AbstractDataSource {
    virtual std::vector<std::tuple<double, double, double, double, double, double>> history_bars(
        std::string instrument_id, int bar_count, int end_dt)
        = 0;
};

class DataSource : public AbstractDataSource {
public:
    int bar_count;
    std::string bar_path;
    // 包含 last current next
    data::basic::Bar bar_reader;
    std::map<std::string,
             std::map<int, std::tuple<double, double, double, double, double, double>>>
        toyDict;

    std::map<int, std::tuple<double, double, double, double, double, double>> consumedData;

    DataSource();
    DataSource(std::string data_Path, int bar_count);

public:
    std::vector<std::tuple<double, double, double, double, double, double>> history_bars(
        std::string instrument_id, int bar_count, int end_dt);
    std::vector<std::tuple<double, double, double, double, double, double>> step();
    std::vector<std::tuple<double, double, double, double, double, double>> reset();

    double get_last_price(std::string instrument_id);
    double get_next_price(std::string instrument_id);
};
