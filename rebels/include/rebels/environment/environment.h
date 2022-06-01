#pragma once

#include <vector>
#include <tuple>
#include <memory>

#include "rebels/core/executor.h"
#include "rebels/core/context.h"
#include "rebels/object/order.h"

class TradingEnvironment {
private:
    std::string __mode;
    double __min_commission;
    double __tax_multiplier;
    int __look_backward_window;
    double __commission_multiplier;
    std::map<std::string, double> __starting_cash;

public:
    TradingEnvironment(std::shared_ptr<DataSource> datasource,
                       int look_backward_window                    = 1,
                       std::map<std::string, double> starting_cash = {{"STOCK", 1000000.0}},
                       std::string mode                            = "non-rl",
                       double commission_multiplier                = 1.0,
                       double min_commission                       = 5.0,
                       double tax_multiplier                       = 1.0);

    std::map<std::string,
             std::map<int, std::tuple<double, double, double, double, double, double>>>
    reset();

    std::tuple<std::map<std::string,
                        std::map<int, std::tuple<double, double, double, double, double, double>>>,
               double,
               bool,
               std::map<std::string, double>>
    step(std::vector<Order>& action);

    /// basic property
    int trading_dt();
};
