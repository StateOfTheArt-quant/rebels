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
    std::vector<std::tuple<double, double, double, double, double, double>> reset();
    std::tuple<std::vector<std::tuple<double, double, double, double, double, double>>,
               double,
               bool>
    step(std::vector<Order>& action);
};
