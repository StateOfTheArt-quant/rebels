#pragma once
#include "rebels/object/order.h"
#include "rebels/object/trade.h"
#include <vector>

enum class EventType{
    POST_SYSTEM_INIT,
    BEFORE_TRADING,
    BAR,
    AFTER_TRADING,
    ORDER_PENDING_NEW,
    ORDER_CREATION_PASS,
    ORDER_CREATION_REJECT,
    ORDER_PENDING_CANCEL,
    ORDER_CANCEL_PASS,
    ORDER_CANCEL_REJECT,
    ORDER_UNSOLICITED_UPDATE, // order_status_change
    TRADE,
    SETTLEMENT,
    POSTSETTLEMENT
};

class Event{
public:
    EventType event_type;
    Event(EventType event_type);
};
// --------------------
class PostSystemInitEvent : public Event {
public:
    PostSystemInitEvent(EventType event_type);
};
//----------------
class PreBeforeTradingEvent : public Event {
public:
    PreBeforeTradingEvent(EventType event_type);
};

class BeforeTradingEvent : public Event {
public:
    BeforeTradingEvent(EventType event_type);
};
class PostBeforeTradingEvent : public Event {
public:
    PostBeforeTradingEvent(EventType event_type);
};

//--------------------
class PreBarEvent : public Event {
public:
    PreBarEvent(EventType event_type);
};

class BarEvent : public Event {
public:
    std::vector<Order> orders;
    BarEvent(EventType event_type, std::vector<Order> orders); //here we can pass-by-value, think why?
};

class PostBarEvent : public Event {
public:
    PostBarEvent(EventType event_type);
};

// -----------------
class PreAfterTradingEvent : public Event {
public:
    PreAfterTradingEvent(EventType event_type);
};

class AfterTradingEvent : public Event {
public:
    AfterTradingEvent(EventType event_type);
};

class PostAfterTradingEvent : public Event {
public:
    PostAfterTradingEvent(EventType event_type);
};

//------------------------------------------------------------------
class OrderPendingNewEvent : public Event {
public:
    Order& order;

public:
    OrderPendingNewEvent(EventType event_type, Order& order);
};

class OrderCreationPassEvent : public Event {
public:
    Order& order;

public:
    OrderCreationPassEvent(EventType event_type, Order& order);
};

class OrderCreationRejectEvent : public Event {
public:
    Order& order;

public:
    OrderCreationRejectEvent(EventType event_type, Order& order);
};

class OrderPendingCancelEvent : public Event {
public:
    Order& order;

public:
    OrderPendingCancelEvent(EventType event_type, Order& order);
};

class OrderCancelPassEvent : public Event {
public:
    Order& order;  //here we have to pass-by-reference, think why?

public:
    OrderCancelPassEvent(EventType event_type, Order& order); //here we have to pass-by-reference, think why?
};

class OrderCancelRejectEvent : public Event {
public:
    Order& order;

public:
    OrderCancelRejectEvent(EventType event_type, Order& order);
};

class OrderUnsolicitedUpdateEvent : public Event {
public:
    Order& order;

public:
    OrderUnsolicitedUpdateEvent(EventType event_type, Order& order);
};

//------------------------------------------------------------------

class TradeEvent : public Event {
public:
    Order& order;
    Trade& trade;

public:
    TradeEvent(EventType event_type, Order& order, Trade& trade);
};

// ----------
class PreSettlementEvent : public Event {
public:
    PreSettlementEvent(EventType event_type);
};

class SettlementEvent : public Event {
public:
    SettlementEvent(EventType event_type);
};

class PostSettlementEvent : public Event {
public:
    PostSettlementEvent(EventType event_type);
};