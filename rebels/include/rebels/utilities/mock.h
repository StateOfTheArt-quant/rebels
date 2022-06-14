/**
 * @file mock.h
 * @date 2022-06-13 13:06:75
 * @author zhongziyang (hankknight@live.com)
 * @brief generate mock data for example or demo
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <ctime>
#include <random>
#include <string>
#include <vector>
#include <chrono>

using namespace std::chrono;

/**
 * @brief accept integer numbers as input
 *
 * uint_fast32_t:
 * 64bit -> unsigned long int
 * 32bit -> unsigned int
 */
template <typename T>
std::vector<T> randn(T low, T high, T length, uint_fast32_t seed = 0U) {
    std::vector<T> temp_vec;

    // default rnd seed
    if (seed == 0U) {
        auto now = steady_clock::now();
        auto now_int
            = duration_cast<nanoseconds>(now.time_since_epoch()).count();
        seed = static_cast<uint32_t>(now_int);
        // might generate same output
        // seed       = time(nullptr);
    }

    std::default_random_engine rnd_engine(seed);
    std::uniform_int_distribution<T> rnd(low, high);

    // generate random number
    for (T i = 0; i < length; i++) {
        temp_vec.push_back(rnd(rnd_engine));
    }

    return temp_vec;
}

/**
 * @brief accept double or float numbers as input, note if we call function at same second,
 *        the first number is always same, because default seed is not changed.
 *
 */
template <typename T>
std::vector<T> rand(T low, T high, T length, uint_fast32_t seed = 0U) {
    std::vector<T> temp_vec;

    // default rnd seed
    if (seed == 0) {
        auto now = steady_clock::now();
        auto now_int
            = duration_cast<nanoseconds>(now.time_since_epoch()).count();
        seed = static_cast<uint32_t>(now_int);
        // might generate same output
        // seed = time(nullptr);
    }

    std::default_random_engine rnd_engine(seed);
    std::uniform_real_distribution<T> rnd(low, high);

    // generate random number
    for (T i = 0; i < length; i++) {
        temp_vec.push_back(rnd(rnd_engine));
    }

    return temp_vec;
}

/**
 * @brief generate date range
 *
 * @param start_date
 * @param length
 * @return std::vector<tm>
 */
std::vector<tm> date_range(const std::string& start_date, uint32_t length);

/**
 * @brief Create a toy data object
 *
 * @param companies csv file numbers
 * @param start_date toy data start date
 * @param date_n day range length
 * @param out_dir output directory
 * @param seed random seed
 */
void create_toy_data(int32_t companies,
                     const std::string& start_date,
                     uint32_t date_n,
                     const std::string& out_dir);