#include "rebels/object/events.h"

Event::Event(EventType event_type):event_type(event_type) {
}

PreBeforeTradingEvent::PreBeforeTradingEvent(EventType event_type): Event(event_type) {
}
BeforeTradingEvent::BeforeTradingEvent(EventType event_type): Event(event_type) {
}
PostBeforeTradingEvent::PostBeforeTradingEvent(EventType event_type): Event(event_type) {
}

PreBarEvent::PreBarEvent(EventType event_type): Event(event_type) {
}
BarEvent::BarEvent(EventType event_type, std::vector<Order> orders): Event(event_type), orders(orders) {
}

PostBarEvent::PostBarEvent(EventType event_type): Event(event_type) {
}

PreAfterTradingEvent::PreAfterTradingEvent(EventType event_type): Event(event_type) {
}
AfterTradingEvent::AfterTradingEvent(EventType event_type): Event(event_type) {
}
PostAfterTradingEvent::PostAfterTradingEvent(EventType event_type): Event(event_type) {
}

OrderPendingNewEvent::OrderPendingNewEvent(EventType event_type, Order& order): Event(event_type),order(order){
}

OrderCreationPassEvent::OrderCreationPassEvent(EventType event_type, Order& order): Event(event_type),order(order){
}

OrderCreationRejectEvent::OrderCreationRejectEvent(EventType event_type, Order& order): Event(event_type),order(order){
}

OrderPendingCancelEvent::OrderPendingCancelEvent(EventType event_type, Order& order): Event(event_type),order(order){
}

OrderCancelPassEvent::OrderCancelPassEvent(EventType event_type, Order& order): Event(event_type),order(order){
}

OrderCancelRejectEvent::OrderCancelRejectEvent(EventType event_type, Order& order): Event(event_type),order(order){
}

OrderUnsolicitedUpdateEvent::OrderUnsolicitedUpdateEvent(EventType event_type, Order& order): Event(event_type),order(order){
}

TradeEvent::TradeEvent(EventType event_type, Order& order, Trade& trade): Event(event_type), order(order), trade(trade){
}


PreSettlementEvent::PreSettlementEvent(EventType event_type):Event(event_type){
}
SettlementEvent::SettlementEvent(EventType event_type):Event(event_type){
}
PostSettlementEvent::PostSettlementEvent(EventType event_type):Event(event_type){
}