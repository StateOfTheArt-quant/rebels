#include "rebels/mod/sys_analyzer/analyzer.h"
#include "rebels/core/context.h"
#include "rebels/mod/sys_account/portfolio.h"
#include <iostream>

Analyzer::Analyzer(std::shared_ptr<EventBus> event_bus)
    : __listener{event_bus}, __rl_static_unit_net_value{1.0} {
    __listener.listen<PostSystemInitEvent>(
        std::bind(&Analyzer::__register_events, this, std::placeholders::_1));

    // init total value
    __rl_static_total_value = Context::Instance().portfolio_ptr->total_value();
}

void Analyzer::__register_events(PostSystemInitEvent event) {
    __listener.listen<TradeEvent>(
        std::bind(&Analyzer::__collect_trade, this, std::placeholders::_1));

    __listener.listen<OrderCreationPassEvent>(
        std::bind(&Analyzer::__collect_order, this, std::placeholders::_1));

    __listener.listen<PostSettlementEvent>(
        std::bind(&Analyzer::__collect_daily, this, std::placeholders::_1));
}

void Analyzer::__collect_daily(PostSettlementEvent event) {
    // step 1: get portfolio
    Portfolio& portfolio = *Context::Instance().portfolio_ptr;

    // step 2: mode select
    if (Context::Instance().mode == "rl") {
        // calculate
        double rl_unv = rl_unit_net_value();
        double total_value = portfolio.total_value();
        double reward = rl_unv / __rl_static_unit_net_value;
        double pnl = total_value - __rl_static_total_value;
        // put it in history reward vector
        __portfolio_current_bar_returns.push_back(reward);
        __portfolio_current_bar_pnl.push_back(pnl);
        // important to update value
        __rl_static_unit_net_value = rl_unv;
        __rl_static_total_value = total_value;

        std::cout << "[Analyzer]: rl mode in __collect_daily" << std::endl;
    } else {
        // daily statistic
        __portfolio_current_bar_returns.push_back(portfolio.daily_returns());
        __portfolio_current_bar_pnl.push_back(portfolio.daily_pnl());
        std::cout << "[Analyzer]: none rl mode in __collect_daily" << std::endl;
    }

    // step 3(optional): record info(complete in future release)
    // portfolio record, performance record
}

// clang-format off
void Analyzer::__collect_order(OrderCreationPassEvent event) { 
    orders.push_back(event.order); 
}
// clang-format on

void Analyzer::__collect_trade(TradeEvent event) {
    // optional step: record info
    // trade info
}

// clang-format off
double Analyzer::rl_static_unit_net_value() {
    return __rl_static_unit_net_value;
}
// clang-format on

double Analyzer::rl_unit_net_value() {
    Portfolio& portfolio = *Context::Instance().portfolio_ptr;
    return portfolio.total_value() / portfolio.units();
}

// clang-format off
std::vector<double> Analyzer::bar_returns() {
    std::cout << "bar returns is: [";
    for (const auto& val : __portfolio_current_bar_returns) {
        std::cout << val << ", ";
    }
    std::cout << "]" << std::endl;

    return __portfolio_current_bar_returns;
}

std::vector<double> Analyzer::bar_pnl() {
    return __portfolio_current_bar_pnl;
}
// clang-format on
