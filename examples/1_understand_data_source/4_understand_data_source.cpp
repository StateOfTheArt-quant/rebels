/**
 * @file 4_understand_data_source.cpp
 * @author zzy (hankknight@live.com)
 * @brief quick start of how to use data source module
 * @version 0.1
 * @date 2022-04-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>

#include "rebels/data/data_source.h"

void print_bar(DataSource::BAR bar) {
    std::cout << "  date\t\t open \t high" << std::endl;
    for (auto& items : bar) {
        std::cout << items.first << "\t" << std::get<0>(items.second) << "\t"
                  << std::get<1>(items.second) << std::endl;
    }
}

void print_bars(DataSource::BAR_MULTI bars) {
    std::cout << "instrument\t date\t\t open\t high" << std::endl;
    for (auto& bar : bars) {
        for (auto& items : bar.second) {
            std::cout << bar.first << "\t" << items.first << "\t" << std::get<0>(items.second)
                      << "\t" << std::get<1>(items.second) << std::endl;
        }
    }
}

void single_instrument() {
    // step 1: init mapping path dict,like {"000001.XSHE": "/path/to/file"}
    std::string bar_path = "./data/train/bar/FB.csv";
    std::map<std::string, std::string> path_map;
    path_map["000001.XSHE"] = bar_path;
    // step 2: init data source module with 1 day
    DataSource data_source(path_map, 1);
    // step 3: moving streamer pointer and put 6 days data into cache
    for (int i = 0; i < 6; i++) {
        data_source.step();
    }
    // step 4: get 5 trading data before 2020-06-11
    DataSource::BAR response = data_source.history_bars("000001.XSHE", 5, 20200611);
    // step 5: print to terminal
    print_bar(response);
}

void multi_instruments() {
    // step 1: init mapping path dict,like {"000001.XSHE": "/path/to/file"}
    std::string bar_path = "./data/train/bar/FB.csv";
    std::map<std::string, std::string> path_map;
    path_map["000001.XSHE"] = bar_path;
    path_map["000002.XSHE"] = bar_path;
    // step 2: init data source module with 1 day
    DataSource data_source(path_map, 1);
    // step 3: moving streamer pointer and put 6 days data into cache
    for (int i = 0; i < 6; i++) {
        data_source.step();
    }
    // step 4: get 5 trading data before 2020-06-11
    DataSource::BAR_MULTI response
        = data_source.multi_history_bars({"000001.XSHE", "000002.XSHE"}, 5, 20200611);
    // step 5: print to terminal
    print_bars(response);
}

// clang-format off
void get_price() {
    // step 1: init mapping path dict,like {"000001.XSHE": "/path/to/file"}
    std::string bar_path = "./data/train/bar/FB.csv";
    std::map<std::string, std::string> path_map;
    path_map["000001.XSHE"] = bar_path;
    // step 2: init data source module with 1 day
    DataSource data_source(path_map, 1);
    // step 3: moving streamer pointer and put 6 days data into cache
    for (int i = 0; i < 6; i++) {
        data_source.step();
    }
    // step 4: last and next close price
    double last_price = data_source.get_last_price("000001.XSHE", /*dt=*/20200610);
    double next_price = data_source.get_next_price("000001.XSHE", /*dt=*/20200612);
    // step 5: print to terminal
    std::cout << "20200611 last date close price is " << last_price << std::endl;
    std::cout << "20200611 next date close price is " << next_price << std::endl;
}

void get_trading_date() {
    // step 1: init mapping path dict,like {"000001.XSHE": "/path/to/file"}
    std::string bar_path = "./data/train/bar/FB.csv";
    std::map<std::string, std::string> path_map;
    path_map["000001.XSHE"] = bar_path;
    // step 2: init data source module with 1 day
    DataSource data_source(path_map, 1);
    // step 3: moving streamer pointer and put 6 days data into cache
    for (int i = 0; i < 6; i++) {
        data_source.step();
    }
    // step 4: last and next trading date
    int last_td = data_source.get_previous_trading_date(20200610, /*bar_count=*/1);
    int next_td = data_source.get_next_trading_date(20200610, /*bar_count=*/1);
    // step 5: print to terminal
    std::cout << "20200610 last trading date is " << last_td << std::endl;
    std::cout << "20200610 next trading date is " << next_td << std::endl;
}
// clang-format on

int main(int argc, char* argv[]) {
    single_instrument();
    std::cout << "********************************************" << std::endl;
    multi_instruments();
    std::cout << "********************************************" << std::endl;
    get_price();
    std::cout << "********************************************" << std::endl;
    // get_trading_date();

    return 0;
}
