#pragma once


enum OrderType{
    LIMIT,
    MARKET
};

enum class Side {
    BUY,
    SELL
};

enum class PositionDirection {
    LONG,
    SHORT
};

enum class PositionEffect {
    OPEN,
    CLOSE,
    CLOSE_TODAY,
    EXERCISE,
    MATCH
};

enum class OrderStatus {
    PENDING_NEW,
    ACTIVE,
    FILLED,
    REJECTED,
    PENDING_CANCEL,
    CANCELLED
};