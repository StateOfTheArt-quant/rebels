#include <cmath>
#include <iostream>

#include "rebels/mod/sys_account/portfolio.h"

Portfolio::Portfolio() {}

Portfolio::Portfolio(std::map<std::string, double> starting_cash,
                     std::shared_ptr<EventBus> event_bus)
    : __listener{event_bus} {
    for (auto it = starting_cash.begin(); it != starting_cash.end(); it++) {
        std::cout << it->first << " " << it->second << std::endl;
        std::shared_ptr<Account> account_ptr
            = std::make_shared<Account>(it->first, it->second, event_bus);
        std::cout << "create " << it->first
                  << " account, total value is: " << account_ptr->total_value() << std::endl;
        __account_container.emplace(it->first, account_ptr);
        __units += it->second;
    }
    __listener.listen<PreBeforeTradingEvent>(
        std::bind(&Portfolio::__pre_before_trading, this, std::placeholders::_1));

    std::cout << "portfolio total value: " << this->total_value() << std::endl;
}

void Portfolio::__pre_before_trading(PreBeforeTradingEvent event) {
    __static_unit_net_value = unit_net_value();
}

double Portfolio::total_value() {
    double total_values = 0.0;
    for (auto it = __account_container.begin(); it != __account_container.end(); it++) {
        total_values += it->second->total_value();
    }
    return total_values;
}

double Portfolio::unit_net_value() { return total_value() / __units; }

double Portfolio::bar_returns() { return unit_net_value() / __static_unit_net_value - 1.0; }

double Portfolio::total_returns() { return unit_net_value() - 1.0; }

double Portfolio::daily_returns() {
    if (__static_unit_net_value == 0) {
        // NAN is declared in math.h header
        return NAN;
    } else {
        return __static_unit_net_value - 1.0;
    }
}

double Portfolio::daily_pnl() {
    double sum_pnl = 0.0;

    for (auto it = __account_container.begin(); it != __account_container.end(); it++) {
        sum_pnl += it->second->daily_pnl();
    }

    return sum_pnl;
}

double Portfolio::units() { return __units; }
