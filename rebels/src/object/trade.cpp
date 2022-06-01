#include "rebels/object/trade.h"
#include "rebels/const.h"
#include "rebels/utilities/trade.h"
#include <stdexcept>

std::uint64_t Trade::__id_counter = 0;

Trade::Trade(std::string instrument_id,  double price, std::int64_t quantity, Side side, PositionEffect position_effect,
             double tax, double commission, double frozen_price, std::int64_t trade_time, int64_t close_today_amount):
        __instrument_id(instrument_id),
        __last_price(price),
        __last_quantity(quantity),
        __side(side),
        __position_effect(position_effect),
        __tax(tax),
        __commission(commission),
        __frozen_price(frozen_price),
        __trade_time(trade_time),
        __close_today_amount(close_today_amount),
        __trade_id(Trade::__id_counter){
    if (price < 0){
        throw std::runtime_error("Order price must be non-zero and positive.");
    }
    if (quantity < 0){
        throw std::runtime_error("Order size must be non-zero and positive.");
    }
    Trade::__id_counter++;

    __position_direction = get_position_direction(__side, __position_effect);
    __transaction_cost = tax + commission;
};

std::int64_t Trade::trade_id() const{
    return __trade_id;
}
std::int64_t Trade::order_id() const{
    return __order_id;
}
std::int64_t Trade::trade_time() const{
    return __trade_time;
}

std::string Trade::instrument_id() const{
    return __instrument_id;
}
double Trade::last_price() const{
    return __last_price;
}
std::int64_t Trade::last_quantity() const{
    return __last_quantity;
}

double Trade::frozen_price(){
    return __frozen_price;
}
std::int64_t Trade::close_today_amount(){
    return __close_today_amount;
}

//---------------
Side Trade::side() const{
    return __side;
}
PositionDirection Trade::position_direction() const{
    return __position_direction;
}
PositionEffect Trade::position_effect() const{
    return __position_effect;
}

double Trade::tax() const{
    return __tax;
}
double Trade::commission() const{
    return __commission;
}
double Trade::transaction_cost() const{
    return __transaction_cost;
}
