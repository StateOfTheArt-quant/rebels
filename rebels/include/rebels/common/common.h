/**
 * @file common.h
 * @date 2022-06-06 10:06:20
 * @author zhongziyang (hankknight@live.com)
 * @brief common head and marco file
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <string>
#include <cstdlib>

/** define smart pointer **/
#define DECLARE_PTR(X) using X##_ptr = std::shared_ptr<X>;

/** forward declaration smart ptr **/
#define FORWARD_DECLARE_PTR(X) \
    class X;                   \
    DECLARE_PTR(X)
