#pragma once

#include "rebels/object/order.h"
#include "rebels/eventbus.h"

class DefaultMatcher {
public:
    std::shared_ptr<EventBus> __event_bus;

    DefaultMatcher();
    DefaultMatcher(std::shared_ptr<EventBus> event_bus);
    void match(Order& order);
};