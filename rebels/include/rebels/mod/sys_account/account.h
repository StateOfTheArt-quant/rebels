#pragma once

#include <string>
#include <map>
#include <memory> //for std:unique_ptr
#include <vector>
#include "rebels/mod/sys_account/position.h"
#include "rebels/object/events.h"
#include "rebels/const.h"

#include <dexode/EventBus.hpp>
using EventBus = dexode::EventBus;
using Listener = dexode::EventBus::Listener;


class Account {

private:
    Listener __listener;

private:
    using PositionMap = std::map<std::string, std::unique_ptr<Position>>;
    using PositionMapMap = std::map<std::string, std::map<PositionDirection, std::unique_ptr<Position>>>;
    using PMI = std::map<std::string, std::unique_ptr<Position>>::iterator;
    using PMMI = PositionMapMap::iterator;


private:
    PositionMapMap __positions;

    double __total_cash{0.0};
    double __frozen_cash{0.0};

    // event callback functions
    void __on_order_pending_new(OrderPendingNewEvent event);

    template<typename T>
    void __on_order_unsolicited_update(T event);

    void __apply_trade(TradeEvent event);

    template<typename T>
    void __on_before_trading(T event);

    void __on_settlement(SettlementEvent event);

    template<typename T>
    void __update_last_price(T event);

private:
    Position* __get_or_create_position(std::string instrument_id, PositionDirection position_direction, std::int64_t init_quantity=0, double init_price=0.0);
    std::vector<Position*> __iter_pos();
    double __frozen_cash_of_order(Order& order);
public:

    std::string account_type;
    double total_cash();
    double frozen_cash();
    double margin();
    double available_cash();

    double market_value();
    double equity();
    double total_value();

    double trading_pnl();
    double position_pnl();
    double transaction_cost();
    double daily_pnl();

public:
    Account();
    Account(std::string account_type, double total_cash, std::shared_ptr<EventBus> event_bus);
    void register_event();

    //just to test
    Position* get_or_create_position(std::string instrument_id, PositionDirection position_direction, std::int64_t init_quantity=0, double init_price=0.0);
    std::vector<Position*> iter_pos();
};