#include "rebels/core/strategy.h"
#include <memory>
#include <functional>
#include "rebels/object/order.h"
#include "rebels/core/context.h"

Strategy::Strategy(std::shared_ptr<EventBus> event_bus) : __listener{event_bus} {
    /// conflit to Portfolio
    // __listener.listen<BarEvent>(std::bind(&Strategy::handle_bar, this, std::placeholders::_1));
}

void Strategy::handle_bar(BarEvent event) {
    // debug message
    std::cout << "[Strategy]: execute handle bar ..." << std::endl;

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

    std::cout << "[Strategy]: submit order done ..." << std::endl;
}
