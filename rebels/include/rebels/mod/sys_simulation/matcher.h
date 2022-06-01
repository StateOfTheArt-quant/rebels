#pragma once

#include "rebels/object/order.h"
#include "rebels/eventbus.h"

class DefaultMatcher {
private:
    std::shared_ptr<EventBus> __event_bus;

public:
    DefaultMatcher();
    DefaultMatcher(std::shared_ptr<EventBus> event_bus);

    void match(Order& order);
};