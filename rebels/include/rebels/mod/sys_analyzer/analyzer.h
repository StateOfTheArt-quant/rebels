#pragma once

#include <string>
#include <map>
#include <memory>  //for std:unique_ptr
#include <vector>
#include "rebels/mod/sys_account/portfolio.h"
#include "rebels/object/events.h"
#include "rebels/const.h"
#include "rebels/common/eventbus.h"

class Analyzer {
private:
    Listener __listener;

    double __rl_static_total_value;
    double __rl_static_unit_net_value{1.0};

    std::vector<double> __portfolio_current_bar_pnl;
    std::vector<double> __portfolio_current_bar_returns;

private:
    void __register_events(PostSystemInitEvent event);
    void __collect_daily(PostSettlementEvent event);
    void __collect_trade(TradeEvent event);
    void __collect_order(OrderCreationPassEvent event);

public:
    Analyzer(std::shared_ptr<EventBus> event_bus);

    std::vector<Order> orders;
    std::vector<double> bar_returns();
    std::vector<double> bar_pnl();

    double rl_static_unit_net_value();
    double rl_unit_net_value();

    // get next day total value
    double get_next_total_value();
};
