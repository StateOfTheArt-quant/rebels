#include "rebels/mod/sys_account/account.h"
#include "rebels/object/events.h"
#include <iostream>

Account::Account(){
}

Account::Account(std::string account_type, double total_cash, std::shared_ptr<EventBus> event_bus):
    account_type(account_type),
    __total_cash(total_cash),
    __listener{event_bus}{
    std::cout << "creating a account ..." << std::endl;
    register_event();
}

void Account::register_event() {
    std::cout << "Account::register_event ..." << std::endl;
    __listener.listen<TradeEvent>(std::bind(&Account::__apply_trade, this, std::placeholders::_1));

    __listener.listen<OrderPendingNewEvent>(std::bind(&Account::__on_order_pending_new, this, std::placeholders::_1));
    __listener.listen<OrderCreationRejectEvent>(std::bind(&Account::__on_order_unsolicited_update<OrderCreationRejectEvent>, this, std::placeholders::_1));
    __listener.listen<OrderUnsolicitedUpdateEvent>(std::bind(&Account::__on_order_unsolicited_update<OrderUnsolicitedUpdateEvent>, this, std::placeholders::_1));
    __listener.listen<OrderCancelPassEvent>(std::bind(&Account::__on_order_unsolicited_update<OrderCancelPassEvent>, this, std::placeholders::_1));

    __listener.listen<PreBeforeTradingEvent>(std::bind(&Account::__on_before_trading<PreBeforeTradingEvent>, this, std::placeholders::_1));
    __listener.listen<SettlementEvent>(std::bind(&Account::__on_settlement, this, std::placeholders::_1));

    //__listener.listen<BarEvent>(std::bind(&Account::__update_last_price<BarEvent>, this, std::placeholders::_1));
}

void Account::__apply_trade(TradeEvent event) {
    std::cout << "call Account::__apply_trade" << std::endl;

    Order& order = event.order;
    Trade& trade = event.trade;
    std::string instrument_id = trade.instrument_id();

    if (trade.position_effect() != PositionEffect::MATCH){
        //std::cout << "call trade.position_effect() != PositionEffect::MATCH" << std::endl;
        if (trade.last_quantity() != order.quantity()){
            double delta_frozen_cash = order.frozen_cash() * trade.last_quantity() / order.quantity();
            std::cout << "delta frozen cash: " << delta_frozen_cash << std::endl;
            __frozen_cash -= delta_frozen_cash;
        } else {
            __frozen_cash -= order.frozen_cash();
        }
    }

    if (trade.position_effect() == PositionEffect::MATCH){
        std::cout << "call trade.position_effect() == PositionEffect::MATCH" << std::endl;
        double delta_cash = __get_or_create_position(instrument_id, PositionDirection::LONG) -> apply_trade(trade) + __get_or_create_position(instrument_id, PositionDirection::SHORT) -> apply_trade(trade);
        __total_cash += delta_cash;
    } else {
        std::cout << "call trade.position_effect() != PositionEffect::MATCH" << std::endl;
        double delta_cash = __get_or_create_position(instrument_id, trade.position_direction()) -> apply_trade(trade);
        __total_cash += delta_cash;
    }

}

void Account::__on_order_pending_new(OrderPendingNewEvent event) {
    std::cout << "call Account::__on_order_pending_new" << std::endl;
    Order& order = event.order;
    double cost = __frozen_cash_of_order(order);
    order.set_frozen_cash(cost);
    std::cout << "local variable order frozen_cash() : "<< order.frozen_cash() << std::endl;
    __frozen_cash += order.frozen_cash();
}

template<typename T>
void Account::__on_order_unsolicited_update(T event) {
    std::cout << "call Account::__on_order_unsolicited_update" << std::endl;
    Order& order = event.order;
    if (order.filled_quantity() == 0){
        __frozen_cash -= order.frozen_cash();
    } else {
        __frozen_cash -= order.frozen_cash() * (double)order.unfilled_quantity() / (double)order.quantity();
    }
}

template<typename T>
void Account::__on_before_trading(T event) {
    std::cout << "call Account::__on_before_trading" << std::endl;

    //todo
    int trading_dt = 20220101;
    std::vector<Position*> position_container = __iter_pos();
    for (auto it = position_container.begin(); it != position_container.end(); it++){
        double delta_cash = (*it) -> before_trading(trading_dt);
        __total_cash += delta_cash;
    }

}

void Account::__on_settlement(SettlementEvent event) {
    std::cout << "call Account::__on_settlement" << std::endl;

    int trading_dt = 20220101;
    //step1: on_settlement
    std::vector<Position*> position_container = __iter_pos();
    for (auto it = position_container.begin(); it != position_container.end(); it++){
        double delta_cash = (*it) -> settlement(trading_dt);
        __total_cash += delta_cash;
    }
    //step2: remove position whose quantity is zero
    for(auto it = __positions.begin(); it != __positions.end(); it++){
        std::vector<bool> bool_vec;
        for(auto iit = it->second.begin(); iit != it->second.end(); iit++){
            bool_vec.push_back(iit -> second ->quantity() == 0);
        }
        if (std::all_of(bool_vec.begin(), bool_vec.end(), [](bool v){return v;})){
            __positions.erase(it->first);
        }
    }

}

template<typename T>
void Account::__update_last_price(T event) {
    std::cout << "call Account::__update_last_price" << std::endl;

    for(auto it = __positions.begin(); it != __positions.end(); it++){
        double price = 11.11; // env.get_last_price(it->first)
        for(auto iit = it->second.begin(); iit != it->second.end(); iit++){
            iit -> second ->update_last_price(price);
        }
    }
}

//--------------------------------------------
Position* Account::__get_or_create_position(std::string instrument_id, PositionDirection position_direction, std::int64_t init_quantity, double init_price){
    PMMI itr = __positions.find(instrument_id);
    if (itr == __positions.end()){
        std::int64_t long_quantity;
        std::int64_t short_quantity;
        if (position_direction == PositionDirection::LONG){
            long_quantity = init_quantity;
            short_quantity = 0;
        } else {
            long_quantity = 0;
            short_quantity = init_quantity;
        }
        std::map<PositionDirection, std::unique_ptr<Position>> position_map;
        position_map.emplace(PositionDirection::LONG, std::make_unique<Position>(instrument_id, init_price, long_quantity, PositionDirection::LONG));
        position_map.emplace(PositionDirection::SHORT, std::make_unique<Position>(instrument_id, init_price, long_quantity, PositionDirection::SHORT));
        std::pair<PMMI, bool> pit = __positions.emplace(instrument_id, std::move(position_map));
        if(not pit.second){
            std::runtime_error("the key in Account:__positions already exist");
        }
    }
    return __positions[instrument_id][position_direction].get();
}

Position* Account::get_or_create_position(std::string instrument_id, PositionDirection position_direction, std::int64_t init_quantity, double init_price){
    return __get_or_create_position(instrument_id, position_direction, init_quantity, init_price);
}


std::vector<Position*> Account::__iter_pos(){
    std::vector<Position*> position_container;

    for(auto it = __positions.begin(); it != __positions.end(); it++){
        for(auto iit = it->second.begin(); iit != it->second.end(); iit++){
            position_container.push_back(iit -> second.get());
        }
    }
    return position_container;
}

std::vector<Position*> Account::iter_pos(){
    return __iter_pos();
}


double Account::__frozen_cash_of_order(Order& order) {
    double cost = 0.0;
    if(order.position_effect() == PositionEffect::OPEN){
        std::cout << "order quantity " <<order.quantity() << std::endl;
        std::cout << "order frozen_price " << order.frozen_price() << std::endl;
        cost = double(order.quantity()) * order.frozen_price();
        std::cout << "frozen cash " << cost << std::endl;
    }
    return cost;
}

//----------------------------------------
double Account::frozen_cash(){
    return __frozen_cash;
}

double Account::margin() {
    double total_margin = 0.0;
    std::vector<Position*> position_container = __iter_pos();
    for (auto it = position_container.begin(); it != position_container.end(); it++){
        double position_margin = (*it) -> margin();
        total_margin += position_margin;
    }
    return total_margin;

}

double Account::available_cash() {
    return __total_cash - __frozen_cash - margin();
}

double Account::total_cash(){
    return __total_cash - margin();
}

//----------------
double Account::equity() {
    double total_equity;
    std::vector<Position*> position_container = __iter_pos();
    for (auto it = position_container.begin(); it != position_container.end(); it++){
        double position_equity = (*it) -> equity();
        total_equity += position_equity;
    }
    return total_equity;
}

double Account::total_value(){
    return __total_cash + equity();
}
//------------------
double Account::trading_pnl(){
    double total_trading_pnl;
    std::vector<Position*> position_container = __iter_pos();
    for (auto it = position_container.begin(); it != position_container.end(); it++){
        double position_trading_pnl = (*it) -> trading_pnl();
        total_trading_pnl += position_trading_pnl;
    }
    return total_trading_pnl;
}

double Account::position_pnl(){
    double total_position_pnl;
    std::vector<Position*> position_container = __iter_pos();
    for (auto it = position_container.begin(); it != position_container.end(); it++){
        double position_trading_pnl = (*it) -> trading_pnl();
        total_position_pnl += position_trading_pnl;
    }
    return total_position_pnl;
}

double Account::transaction_cost(){
    double total_transaction_cost;
    std::vector<Position*> position_container = __iter_pos();
    for (auto it = position_container.begin(); it != position_container.end(); it++){
        double position_transaction_cost = (*it) -> trading_pnl();
        total_transaction_cost += position_transaction_cost;
    }
    return total_transaction_cost;
}

double Account::daily_pnl(){
    return trading_pnl() + position_pnl() - transaction_cost();
}