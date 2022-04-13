#pragma once

#include "rebels/core/executor.h"
#include "rebels/core/context.h"
#include "rebels/object/order.h"
#include <vector>
#include <tuple>
#include <memory>

class TradingEnvironment {
public:
    TradingEnvironment();
    std::vector<std::vector<double>> reset();
    std::tuple<std::vector<std::vector<double>>, double, bool> step(std::vector<Order>& action);

};