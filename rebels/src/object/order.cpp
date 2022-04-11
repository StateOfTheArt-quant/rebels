#include "rebels/object/order.h"
#include "rebels/const.h"
#include <stdexcept>
#include "rebels/utilities/trade.h"

std::uint64_t Order::__id_counter = 0;

Order::Order(){
}

Order::Order(std::string instrument_id, std::int64_t quantity, Side side, PositionEffect position_effect, OrderType order_type, double price):
            __instrument_id(instrument_id),
            __quantity(quantity),
            __side(side),
            __position_effect(position_effect),
            __order_type(order_type),
            __status(OrderStatus::PENDING_NEW),
            __order_id(Order::__id_counter){
    if (price < 0){
        throw std::runtime_error("Order price must be non-zero and positive.");
    }
    if (__quantity < 0){
        throw std::runtime_error("Order size must be non-zero and positive.");
    }
    Order::__id_counter++;

    if(__order_type == OrderType::LIMIT){
        __frozen_price = price;
    } else{
        __frozen_price = 0.0;
    }
    __position_direction = get_position_direction(side, position_effect);
};

void Order::fill(Trade trade){
    std::int64_t  quantity = trade.last_quantity();
    std::int64_t  new_quantity = filled_quantity() + quantity;

    __transaction_cost = trade.commission() + trade.tax();
    if (trade.position_effect() != PositionEffect::MATCH){
        __avg_price = (__avg_price * filled_quantity() + trade.last_price() * quantity) / new_quantity;
    }
    __filled_quantity = new_quantity;
    if (unfilled_quantity() == 0){
        __status = OrderStatus::FILLED;
    }
}


void Order::set_frozen_cash(double value) {
    __frozen_cash = value;
}

bool Order::is_final(){
    if ( __status == OrderStatus::PENDING_NEW || __status == OrderStatus::ACTIVE || __status == OrderStatus::PENDING_CANCEL){
        return false;
    } else {
        return true;
    }
}

void Order::active(){
    __status = OrderStatus::ACTIVE;
}

void Order::mark_rejected(std::string reject_reason){
    if  (not is_final()){
        __message = reject_reason;
        __status = OrderStatus::REJECTED;
    }
}

void Order::mark_cancelled(std::string cancelled_reason){
    if  (not is_final()){
        __message = cancelled_reason;
        __status = OrderStatus::CANCELLED;
    }
}


//-------------------
std::int64_t Order::order_id(){
    return __order_id;
}
std::int64_t Order::insert_time(){
    return __insert_time;
}
std::string Order::instrument_id(){
    return __instrument_id;
}

double Order::frozen_price(){
    return __frozen_price;
}
std::int64_t Order::quantity(){
    return __quantity;
}
std::int64_t Order::filled_quantity(){
    return __filled_quantity;
}

std::int64_t Order::unfilled_quantity() {
    return __quantity - __filled_quantity;
}

double Order::frozen_cash(){
    return __frozen_cash;
}

OrderType Order::order_type(){
    return __order_type;
}
Side Order::side(){
    return __side;
}
PositionEffect Order::position_effect(){
    return __position_effect;
}
PositionDirection Order::position_direction(){
    return __position_direction;
}
OrderStatus Order::status(){
    return __status;
}

double Order::avg_price(){
    return __avg_price;
}
double Order::transaction_cost(){
    return __transaction_cost;
}

std::string Order::message(){
    return __message;
}


