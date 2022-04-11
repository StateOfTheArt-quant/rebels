#pragma once

#include "rebels/const.h"

inline PositionDirection get_position_direction(Side side, PositionEffect position_direction){
    if ((side == Side::BUY && position_direction ==PositionEffect::OPEN) || (side == Side::SELL && position_direction == PositionEffect::CLOSE)){
        return PositionDirection::LONG;
    } else {
        return PositionDirection::SHORT;
    }
}