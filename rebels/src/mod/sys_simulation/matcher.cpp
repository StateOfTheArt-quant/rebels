#include "rebels/const.h"
#include "rebels/core/context.h"
#include "rebels/object/events.h"
#include "rebels/mod/sys_simulation/matcher.h"

DefaultMatcher::DefaultMatcher() {}

DefaultMatcher::DefaultMatcher(std::shared_ptr<EventBus> event_bus) : __event_bus(event_bus) {}

void DefaultMatcher::match(Order& order) {
    // fetch global record from context
    Context& context  = Context::Instance();
    std::int64_t fill = order.unfilled_quantity();

    // todo: now only support limit order
    double deal_price = order.frozen_price();

    Trade trade{/*instrument_id=*/order.instrument_id(),
                /*price=*/deal_price,
                /*quantity=*/fill,
                /*side=*/order.side(),
                /*position_effect=*/order.position_effect(),
                // /*tax=*/0.0,
                // /*commission=*/0.0,
                /*frozen_price=*/order.frozen_price()};
    trade.set_commission(context.get_trade_commission(trade));
    trade.set_tax(context.get_trade_tax(trade));

    order.fill(trade);

    __event_bus->postpone(TradeEvent(EventType::TRADE, order, trade));
    __event_bus->process();

    if (order.order_type() == OrderType::MARKET && order.unfilled_quantity() != 0) {
        std::string cancelled__reason = "Order cancel: market order unfilled completely";
        order.mark_cancelled(cancelled__reason);
    }
}