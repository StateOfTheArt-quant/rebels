#pragma once

#include <cstdint>
#include <string>
#include "rebels/const.h"
#include "rebels/object/trade.h"

class Position {
private:
    std::string __instrument_id;

    std::int64_t __old_quantity{0};
    std::int64_t __logical_old_quantity{0};
    std::int64_t __today_quantity{0};

    double __avg_price;
    double __trade_cost;
    double __transaction_cost;

    double __prev_close;
    double __last_price;

    PositionDirection __position_direction;
    int __direction_factor;

public:
    std::string instrument_id();
    //double price;
    //std::int64_t quantity;
    double avg_price();
    double trade_cost();
    double transaction_cost();
    double prev_close();
    double last_price();

    PositionDirection position_direction();
    int direction_factor();



public:
    Position(std::string instrument_id, double init_price, std::int64_t init_quantity,  PositionDirection position_direction);

    std::int64_t quantity();
    std::int64_t closable();
    double margin();

    double market_value();
    double equity();

    double position_pnl();
    double trading_pnl();
    double pnl();

    double apply_trade(const Trade& trade);
    double before_trading(int trading_dt);
    double settlement(int trading_dt);
    void update_last_price(double price);
};
