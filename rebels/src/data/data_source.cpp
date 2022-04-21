/* Single company. */

#include "rebels/data/data_source.h"
#include <iostream>
#include <vector>
#include <algorithm>

DataSource::DataSource() {}

/// 多个csv读取并加载
DataSource::DataSource(std::string bar_path, int bar_counts)
    : bar_reader(), bar_path(bar_path), bar_count(bar_counts) {  //, bar_count(bar_count)
    bar_reader.LoadCSV(bar_path);
    std::cout << "load csv successfully : " << bar_path << std::endl;
}

std::vector<std::tuple<double, double, double, double, double, double>> DataSource::history_bars(
    std::string instrument_id, int bar_counts, int end_dt) {
    std::vector<std::tuple<double, double, double, double, double, double>> v;
    std::map<int, std::tuple<double, double, double, double, double, double>>::iterator it;
    it = consumedData.find(end_dt);

    for (int i = 0; i < bar_counts; i++) {
        v.push_back(it->second);
        it--;
    }

    std::reverse(v.begin(), v.end());
    return v;
}

std::vector<std::tuple<double, double, double, double, double, double>> DataSource::step() {
    bar_reader.LoadNext();
    data::BarRecord bar_rec    = bar_reader.Record();
    consumedData[bar_rec.date] = bar_rec.bar;
    int current_dt             = bar_rec.date;
    std::cout << "current dt: " << current_dt << std::endl;

    std::vector<std::tuple<double, double, double, double, double, double>> v;
    v = history_bars("fb", bar_count, current_dt);
    return v;
}

std::vector<std::tuple<double, double, double, double, double, double>> DataSource::reset() {
    // reload csv file from beginning
    bar_reader.LoadCSV(bar_path);

    // clear dict and release memory
    decltype(consumedData) empty;
    consumedData.swap(empty);

    std::cout << "DataSource and consumedData reset" << std::endl;

    return step();
}

double DataSource::get_last_price(std::string instrument_id) {
    // instrument_id is not used yet
    data::BarRecord bar_last = bar_reader.record_last;

    // only if last date is not existed in raw data source
    if (bar_last.date == 0) {
        return 0.0;
    }

    // debug
    std::cout << "last date is " << bar_last.date << ", close price is "
              << std::get<3>(bar_last.bar) << std::endl;

    // last day close price
    return std::get<3>(bar_last.bar);
}

double DataSource::get_next_price(std::string instrument_id) {
    // instrument_id is not used yet
    data::BarRecord bar_next = bar_reader.record_next;

    // only if last date is not existed in raw data source
    if (bar_next.date == 0) {
        return 0.0;
    }

    // debug
    std::cout << "last date is " << bar_next.date << ", close price is "
              << std::get<3>(bar_next.bar) << std::endl;

    // last day close price
    return std::get<3>(bar_next.bar);
}
