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
    int32_t __min_commission;
    int32_t __tax_multiplier;
    int32_t __look_backward_window;
    int32_t __commission_multiplier;
    std::map<std::string, double> __starting_cash;

public:
    TradingEnvironment(std::shared_ptr<DataSource> datasource,
                       int32_t look_backward_window                = 1,
                       std::map<std::string, double> starting_cash = {{"STOCK", 1000000.0}},
                       std::string mode                            = "non-rl",
                       int32_t commission_multiplier               = 1,
                       int32_t min_commission                      = 5,
                       int32_t tax_multiplier                      = 1);

    std::map<std::string,
             std::map<int, std::tuple<double, double, double, double, double, double>>>
    reset();

    std::tuple<std::map<std::string,
                        std::map<int, std::tuple<double, double, double, double, double, double>>>,
               double,
               bool,
               std::map<std::string, double>>
    step(std::vector<Order>& action);
};
