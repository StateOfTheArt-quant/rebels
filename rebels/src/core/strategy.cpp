#include "rebels/core/strategy.h"
#include "rebels/object/order.h"
#include "rebels/core/context.h"
#include "rebels/utilities/output.h"

Strategy::Strategy(std::shared_ptr<EventBus> event_bus) : __listener{event_bus} {
    /// conflit to Portfolio
    // __listener.listen<BarEvent>(std::bind(&Strategy::handle_bar, this, std::placeholders::_1));
}

void Strategy::handle_bar(BarEvent event) {
    // debug message
    DEBUG_MSG("Strategy", "execute handle bar ...");

    // temporary parameter to fetch ev orders
    std::vector<Order> orders = event.orders;

    // query all bar orders
    if (orders.empty()) {
        // do nothing when empty orders
        return;
    }

    // load context
    Context& context = Context::Instance();

    for (auto order : orders) {
        // transfer ownership to broker
        std::unique_ptr<Order> order_ptr = std::make_unique<Order>(order);
        context.broker_ptr->submit_order(std::move(order_ptr));
    }

    DEBUG_MSG("Strategy", "submit order done ...");
}
