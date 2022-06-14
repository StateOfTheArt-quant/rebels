/**
 * @file mock.cpp
 * @date 2022-06-13 17:06:55
 * @author zhongziyang (hankknight@live.com)
 * @brief private achieve of mock.h
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <sstream>
#include <iomanip>
#include <fstream>

#include "rebels/utilities/mock.h"
#include "rebels/utilities/output.h"

using namespace std::chrono;

constexpr int32_t g_base_month = 1;
constexpr int32_t g_base_year  = 1900;
constexpr time_t g_day_unit    = 24 * 60 * 60;

std::vector<tm> date_range(const std::string& start_date, uint32_t length) {
    std::vector<tm> temp_vec;

    // init tm sturct(important!!!)
    tm time_info = {}, temp_info;

    // format input and convert to seconds
    strptime(start_date.c_str(), "%Y-%m-%d %Y%m%d %Y/%m/%d", &time_info);
    time_t start_time = mktime(&time_info);

    for (int i = 0; i < length; i++) {
        start_time += g_day_unit;
        temp_info = *localtime(&start_time);
        temp_vec.push_back(temp_info);
    }

    return temp_vec;
}

void create_toy_data(int32_t companies,
                     const std::string& start_date,
                     uint32_t date_n,
                     const std::string& out_dir) {
    // cache
    std::ofstream fp;
    std::string file, temp;
    std::vector<tm> date_vec;
    std::vector<float> temp_vec;
    std::stringstream file_ss, date_ss;

    // create date range
    date_vec = date_range(start_date, date_n);

    for (int32_t i = 1; i <= companies; i++) {
        // auto fill 0 brefor instrument id, like "000001.XSHE"
        file_ss << std::setw(6) << std::setfill('0') << i << ".XSHE"
                << ".csv";

        // create output file
        file = out_dir + "/" + file_ss.str();
        fp.open(file);

        if (!fp.is_open()) {
            DEBUG_MSG("Mock Data", "Can't open create {}.", file);
            continue;
        }

        // head columns
        fp << "Date,Open,High,Low,Close,Adj Close,Volume\n";

        for (const auto& date : date_vec) {
            // random data number
            temp_vec = rand<float>(/*low=*/10.0, /*high=*/200.0, /*length=*/7.0);

            date_ss << date.tm_year + g_base_year << "-" << date.tm_mon + g_base_month << "-"
                    << date.tm_mday;

            temp = FORMAT_MSG("{},{},{},{},{},{},{},{}\n",
                              date_ss.str(),
                              temp_vec[0],
                              temp_vec[1],
                              temp_vec[2],
                              temp_vec[3],
                              temp_vec[4],
                              temp_vec[5],
                              std::round(temp_vec[6]) * 10.0);
            fp << temp;

            // clear cache
            date_ss.clear();
            date_ss.str("");
        }

        // clear cache
        file_ss.clear();
        file_ss.str("");
        fp.close();
    }
}