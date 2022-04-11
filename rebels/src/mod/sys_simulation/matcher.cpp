#include "rebels/mod/sys_simulation/matcher.h"
#include "rebels/const.h"
#include "rebels/object/events.h"

DefaultMatcher::DefaultMatcher(){

}

DefaultMatcher::DefaultMatcher(std::shared_ptr<EventBus> event_bus):__event_bus(event_bus){

}

void DefaultMatcher::match(Order& order){
    std::int64_t fill = order.unfilled_quantity();
    //todo: now only support limit order
    double deal_price = order.frozen_price();

    Trade trade{order.instrument_id(), deal_price, fill, order.side(), order.position_effect(), 0.0, 0.0, order.frozen_price()};
    order.fill(trade);

    __event_bus->postpone(TradeEvent(EventType::TRADE, order, trade));
    __event_bus->process();

    if(order.order_type() == OrderType::MARKET && order.unfilled_quantity() !=0){
        std::string cancelled__reason = "Order cancel: market order unfilled completely";
        order.mark_cancelled(cancelled__reason);
    }
}