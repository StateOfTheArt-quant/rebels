#include "rebels/data/data_source.h"
#include <iostream>
#include <vector>
#include <algorithm>

/// Construct a new Data Source:: Data Source object
///
/// Parameters:
///     path_map: path dict like {"000001.XSHE": "/home/user/1.csv", "000002.XSHE":
///     "/home/user/2.csv"} bar_count: select init bar entries
///
/// Examples:
///     std::map<std::string, std::string> path_map;
///     path_map["000001.XSHE"] = "/home/user/1.csv";
///     path_map["000002.XSHE"] = "/home/user/1.csv";
///     auto ds = DataSource(path_map, 2);
DataSource::DataSource(std::map<std::string, std::string> path_map, int bar_count)
    : __bar_count(bar_count), path_dict(path_map) {
    // mapping dict for instrument id and streamer ptr
    for (auto& path_item : path_map) {
        bar_reader[path_item.first] = std::make_shared<data::basic::Bar>(path_item.second);
        std::cout << "load " << path_item.first << " from " << path_item.second << std::endl;
    }
}

// simple version for back test
DataSource::BAR DataSource::history_bars(std::string instrument_id, int bar_counts, int end_dt) {
    // trading data
    BAR bar;

    auto bar_it = consumed_data.find(instrument_id);

    // TODO it might have critical error when row info is unknown
    if (bar_it != consumed_data.end()) {
        // search dt from cache
        auto record_it = bar_it->second.find(end_dt);

        for (int i = 0; i < bar_counts; i++) {
            // if reach begin stop iteration
            if (record_it == bar_it->second.end()) {
                break;
            }
            bar[record_it->first] = (record_it->second);
            record_it--;
        }
    }

    return bar;
}

DataSource::BAR_MULTI DataSource::multi_history_bars(std::vector<std::string> instrument_ids,
                                                     int bar_count,
                                                     int end_dt) {
    BAR_MULTI bars;

    for (std::string& instrument_id : instrument_ids) {
        bars[instrument_id] = history_bars(instrument_id, bar_count, end_dt);
    }

    return bars;
}

DataSource::BAR_MULTI DataSource::step() {
    BAR_MULTI bar;
    int current_dt;
    data::BarRecord bar_rec;
    std::string instrument_id;

    // next bar for all storage reader
    for (auto& reader_item : bar_reader) {
        // next bar
        reader_item.second->LoadNext();

        bar_rec                                  = reader_item.second->Record();
        instrument_id                            = reader_item.first;
        current_dt                               = bar_rec.date;
        consumed_data[instrument_id][current_dt] = bar_rec.bar;

        bar[instrument_id] = history_bars(instrument_id, __bar_count, current_dt);

        std::cout << instrument_id << " current date is " << current_dt << std::endl;
    }

    return bar;
}

DataSource::BAR_MULTI DataSource::reset() {
    for (auto& reader_item : bar_reader) {
        reader_item.second->LoadCSV(path_dict.at(reader_item.first));
    }

    // clear dict
    decltype(consumed_data) empty;
    consumed_data.swap(empty);

    std::cout << "DataSource and consumed_data reset" << std::endl;

    // get first date then clear
    auto first_data = step();

    for (auto& reader_item : bar_reader) {
        reader_item.second->LoadCSV(path_dict.at(reader_item.first));
    }

    // clear dict
    // consumed_data.swap(empty);

    return first_data;
}

double DataSource::get_previous_price(std::string instrument_id, int dt) {
    auto it = bar_reader.find(instrument_id);

    if (it == bar_reader.end()) {
        return 0.0;
    }

    // instrument_id is not used yet
    data::BarRecord bar_last = it->second->record_last;

    // only if last date is not existed in raw data source
    if (bar_last.date != dt) {
        return 0.0;
    }

    // debug
    int last_date = bar_last.date;  // TODO remove
    std::cout << "last date is " << last_date << ", close price is " << std::get<3>(bar_last.bar)
              << std::endl;

    // last day close price
    return std::get<3>(bar_last.bar);
}

double DataSource::get_current_price(std::string instrument_id, int dt) {
    auto it = bar_reader.find(instrument_id);

    if (it == bar_reader.end()) {
        return 0.0;
    }

    // instrument_id is not used yet
    data::BarRecord bar_curr = it->second->record_curr;

    // debug
    int last_date = bar_curr.date;  // TODO remove
    std::cout << "[DataSouce]: get_current_price current date is " << last_date << std::endl;

    // only if last date is not existed in raw data source
    if (bar_curr.date != dt) {
        return 0.0;
    }

    // last day close price
    return std::get<3>(bar_curr.bar);
}

double DataSource::get_next_price(std::string instrument_id, int dt) {
    auto it = bar_reader.find(instrument_id);

    if (it == bar_reader.end()) {
        return 0.0;
    }

    // instrument_id is not used yet
    data::BarRecord bar_next = it->second->record_next;

    // only if last date is not existed in raw data source
    if (bar_next.date != dt) {
        return 0.0;
    }

    // debug
    std::cout << "next date is " << bar_next.date << ", close price is "
              << std::get<3>(bar_next.bar) << std::endl;

    // last day close price
    return std::get<3>(bar_next.bar);
}

int DataSource::get_previous_trading_date(int end_dt, int bar_count) {
    // get prevoius trading date via trading calendar
    // std::vector<int> calendar;
    // auto iter = std::find(calendar.begin(), calendar.end(), end_dt);
    // if (iter == calendar.end()) {
    //     return 0;
    // }
    // // get index
    // int idx = iter - calendar.begin();
    // // back ward
    // idx -= bar_count;
    // // if idx out of bound 0 as default
    // return calendar[idx];

    return 20200604;
}

int DataSource::get_next_trading_date(int end_dt, int bar_count) {
    // get next trading date via trading calendar
    // std::vector<int> calendar;
    // auto iter = std::find(calendar.begin(), calendar.end(), end_dt);
    // if (iter == calendar.end()) {
    //     return 0;
    // }
    // // get index
    // int idx = iter - calendar.begin();
    // // back ward
    // idx -= bar_count;
    // // if idx out of bound 0 as default
    // return calendar[idx];

    return 20200604;
}
