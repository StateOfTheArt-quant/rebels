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

void example_of_unbalance_data_process() {
    // APPLE.csv date range is: 2020-06-04 ~ 2020-06-12
    // AMZN.csv date range is : 2020-06-08 ~ 2020-06-16
    // default using csv file first and last readable data to fill previous or next rows
    // set output dir to current path
    AligmentProcess ap(/*files=*/{"data/APPLE.csv", "data/AMZN.csv"}, /*out_dir=*/"./");
    ap.process();

    std::cout << "-- fin --" << std::endl;
}

int main(int argc, char *argv[]) {
    // example
    example_of_unbalance_data_process();

    return 0;
}
