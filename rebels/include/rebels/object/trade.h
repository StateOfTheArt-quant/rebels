#pragma once

#include <cstdint>
#include <string>
#include "rebels/const.h"

class Trade {

private:
    static std::uint64_t __id_counter;


private:
    std::int64_t __trade_id;
    std::int64_t __order_id;
    std::int64_t __trade_time;

    std::string __instrument_id;
    double __last_price;
    std::int64_t __last_quantity;

    double __frozen_price;
    std::int64_t __close_today_amount;

    Side __side;
    PositionDirection __position_direction;
    PositionEffect __position_effect;

    double __tax{0.0};
    double __commission{0.0};
    double __transaction_cost{0.0};

public:
    Trade(std::string instrument_id, double price, std::int64_t quantity, Side side, PositionEffect position_effect,
          double tax=0.0, double commission=0.0, double frozen_price=0.0,  std::int64_t trade_time=20000, std::int64_t close_today_amount=0);

    std::int64_t trade_id() const;
    std::int64_t order_id() const;
    std::int64_t trade_time() const;

    std::string instrument_id() const;
    double last_price() const;
    std::int64_t last_quantity() const;

    double frozen_price();
    std::int64_t close_today_amount();

    Side side() const;
    PositionDirection position_direction() const;
    PositionEffect position_effect() const;

    double tax() const;
    double commission() const;
    double transaction_cost() const;

};
