/**
 * @file common.h
 * @date 2022-06-06 11:06:08
 * @author zhongziyang (hankknight@live.com)
 * @brief forward declare common head file for core
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "rebels/common/common.h"
#include "rebels/common/eventbus.h"

namespace rebels {
namespace core {

FORWARD_DECLARE_PTR(DataSource)

FORWARD_DECLARE_PTR(Account)

FORWARD_DECLARE_PTR(Portfolio)

FORWARD_DECLARE_PTR(SimulationBroker)

FORWARD_DECLARE_PTR(Analyzer)

FORWARD_DECLARE_PTR(Executor)

}  // namespace core
}  // namespace rebels
