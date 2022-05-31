#include "rebels/mod/sys_account/position.h"
#include "rebels/utilities/output.h"
#include "rebels/core/context.h"
#include <algorithm>
#include <stdexcept>
// #include <iostream>

std::int64_t zero_ll = 0;

Position::Position(std::string instrument_id,
                   double init_price,
                   std::int64_t init_quantity,
                   PositionDirection position_direction)
    : __instrument_id(instrument_id),
      __avg_price(init_price),
      __old_quantity(init_quantity),
      __position_direction(position_direction) {
    if (position_direction == PositionDirection::LONG) {
        __direction_factor = 1;
    } else {
        __direction_factor = -1;
    }
}

std::int64_t Position::quantity() { return __old_quantity + __today_quantity; }

double Position::market_value() {
    if (this->quantity() != 0) {
        return last_price() * quantity();
        // return __last_price * quantity();
    } else {
        return 0;
    }
}

double Position::equity() {
    if (this->quantity() != 0) {
        return last_price() * quantity();
        // return __last_price * quantity();
    } else {
        return 0;
    }
}

std::int64_t Position::closable() {
    // todo
    return 0;
}

double Position::margin() { return 0.0; }

double Position::position_pnl() {
    return __logical_old_quantity * (__last_price - __prev_close) * __direction_factor;
}

double Position::trading_pnl() {
    std::int64_t trade_quantity = __today_quantity + __old_quantity - __logical_old_quantity;
    return (trade_quantity * __last_price - __trade_cost) * __direction_factor;
}

double Position::pnl() { return (__last_price - __avg_price) * quantity() * __direction_factor; }

double Position::apply_trade(const Trade& trade) {
    // return the change of total cash
    __transaction_cost = trade.transaction_cost();

    /// debug
    PRINT_MSG("[Position]: transaction cost: {}, ", __transaction_cost);

    if (trade.position_effect() == PositionEffect::OPEN) {
        if (quantity() < 0) {
            if (quantity() + trade.last_quantity() > 0) {
                __avg_price = trade.last_price();
            } else {
                __avg_price = 0;
            }
        } else {
            double cost = quantity() * __avg_price + trade.last_quantity() * trade.last_price();
            /// debug
            PRINT_MSG("cost {}, ", cost);

            __avg_price = cost / (quantity() + trade.last_quantity());
        }

        /// debug
        PRINT_MSG("__avg_price {}\n", __avg_price);

        __today_quantity += trade.last_quantity();
        __trade_cost += trade.last_price() * trade.last_quantity();
        return (-1 * trade.last_price() * trade.last_quantity()) - trade.transaction_cost();
    } else if (trade.position_effect() == PositionEffect::CLOSE) {
        __today_quantity -= std::max(trade.last_quantity() - __old_quantity, zero_ll);
        __old_quantity -= std::min(trade.last_quantity(), __old_quantity);
        __trade_cost -= trade.last_price() * trade.last_quantity();
        return trade.last_price() * trade.last_quantity() - trade.transaction_cost();
    } else {
        std::runtime_error("does not support this position_effect");
        // empty return to get rid of:
        // warning: control reaches end of non-void function [-Wreturn-type]
        return 0.0;
    }
}

double Position::before_trading(int trading_dt) {
    __prev_close       = __last_price;
    __transaction_cost = 0.0;
    return 0.0;
}
void Position::update_last_price(double price) { __last_price = price; }

double Position::settlement(int trading_dt) {
    __old_quantity += __today_quantity;
    __logical_old_quantity = __old_quantity;
    __today_quantity       = 0;
    __trade_cost           = 0;
    // non_closable = 0;
    return 0.0;
}

//--------------------
std::string Position::instrument_id() { return __instrument_id; }
// double price;
// std::int64_t quantity;
double Position::avg_price() { return __avg_price; }
double Position::trade_cost() { return __trade_cost; }
double Position::transaction_cost() { return __transaction_cost; }
double Position::prev_close() { return __prev_close; }
double Position::last_price() {
    if (__last_price == INFINITY) {
        // get last price from datasource
        Context& context = Context::Instance();
        __last_price
            = context.data_source_ptr->get_current_price(instrument_id(), context.trading_dt);
        // __last_price = 226.29;
    }

    return __last_price;
}

PositionDirection Position::position_direction() { return __position_direction; }
int Position::direction_factor() { return __direction_factor; }