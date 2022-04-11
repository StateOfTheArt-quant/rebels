#pragma once

#include <string>
#include <map>
#include <memory> //for std:unique_ptr
#include <vector>
#include "rebels/mod/sys_account/account.h"
#include "rebels/object/events.h"
#include "rebels/const.h"

#include <dexode/EventBus.hpp>
using EventBus = dexode::EventBus;
using Listener = dexode::EventBus::Listener;


class Portfolio{
private:
    Listener __listener;

    double __static_unit_net_value{1.0};
    double __units{0.0};

    std::map<std::string, std::shared_ptr<Account>> __account_container;

    void __pre_before_trading(PreBeforeTradingEvent event);

public:
    Portfolio();
    Portfolio(std::map<std::string, double> starting_cash, std::shared_ptr<EventBus> event_bus); //std::map<std::string, int> starting_cash,

    double unit_net_value();
    double total_value();
    double bar_returns();
    double total_returns();

};