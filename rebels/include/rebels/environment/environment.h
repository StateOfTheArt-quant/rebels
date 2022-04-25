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
    DataSource::BAR_MULTI reset();
    std::tuple<DataSource::BAR_MULTI, double, bool> step(std::vector<Order>& action);
};
