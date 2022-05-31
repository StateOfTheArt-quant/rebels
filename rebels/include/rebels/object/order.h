#pragma once

#include <cstdint>
#include <string>
#include "rebels/const.h"
#include "rebels/object/trade.h"

class Order {

private:
    static std::uint64_t __id_counter;

private:
    std::int64_t __order_id;
    std::int64_t __insert_time;
    std::string __instrument_id;

    /// attention: init parameter with default values otherwise error will occur
    double __frozen_price{0.0};
    std::int64_t __quantity{0};
    std::int64_t __filled_quantity{0};

    double __frozen_cash;

    OrderType __order_type;
    Side __side;
    PositionDirection __position_direction;
    PositionEffect __position_effect;
    OrderStatus __status;

    double __avg_price{0.0};
    double __transaction_cost{0.0};

    std::string __message;

public:
    Order();
    Order(std::string instrument_id, std::int64_t quantity, Side side, PositionEffect position_effect, OrderType order_type, double price=0.0);

    void fill(Trade trade);
    void set_frozen_cash(double value);

    //
    bool is_final();
    void active();

    //
    void mark_rejected(std::string reject_reason);
    void mark_cancelled(std::string cancelled_reason);


public:
    std::int64_t order_id();
    std::int64_t insert_time();
    std::string instrument_id();

    double frozen_price();
    std::int64_t quantity();
    std::int64_t filled_quantity();
    std::int64_t unfilled_quantity();

    double frozen_cash();

    OrderType order_type();
    Side side();
    PositionEffect position_effect();
    PositionDirection position_direction();
    OrderStatus status();

    double avg_price();
    double transaction_cost();

    std::string message();


};