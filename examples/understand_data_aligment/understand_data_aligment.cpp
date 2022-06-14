/**
 * @file understand_data_aligment.cpp
 * @date 2022-06-10 17:06:13
 * @author zhongziyang (hankknight@live.com)
 * @brief Quick start of how to align unbalance data
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>

#include "rebels/utilities/csv.h"
#include "rebels/utilities/mock.h"

void example_of_unbalance_data_process() {
    // APPLE.csv date range is: 2020-06-04 ~ 2020-06-12
    // AMZN.csv date range is : 2020-06-08 ~ 2020-06-16
    // default using csv file first and last readable data to fill previous or next rows
    // set output dir to current path
    AligmentProcess ap(/*files=*/{"data/APPLE.csv", "data/AMZN.csv"}, /*out_dir=*/"./");
    ap.process();

    std::cout << "-- unbalance process fin --" << std::endl;
}

void example_of_mock_util() {
    // 2 companies, trade day from 2022-06-14 to 2022-06-18, output file locate at runtime
    // directory
    create_toy_data(/*companies=*/2, /*start_date=*/"2022-06-14", /*date_n=*/5, /*out_dir=*/"./");

    std::cout << "-- mock util fin --" << std::endl;
}

int main(int argc, char *argv[]) {
    // example 1
    example_of_unbalance_data_process();
    // example 2
    example_of_mock_util();

    return 0;
}
