/**
 * @file output.h
 * @author zhongziyang (hankknight@live.com)
 * @brief print function achieve in fmt
 * @date 2022-05-16 16:05:20
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>

// marco to controll debug message
#define ENABLE_DEBUG true

/// format message with given tag and message
#define FORMAT_MSG(tag, msg) "[" + std::string(tag) + "]: " + std::string(msg) + "\n"
/// wrap fmt library to print debug message
#define DEBUG_MSG(tag, msg, ...) fmt::print(FORMAT_MSG(tag, msg), ##__VA_ARGS__)
/// wrap fmt library to print normal message
#define PRINT_MSG(msg, ...) fmt::print(msg, ##__VA_ARGS__)
