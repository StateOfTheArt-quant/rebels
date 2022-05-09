#pragma once

#include "rebels/object/order.h"
#include "rebels/eventbus.h"

#include <vector>
#include <memory>

class Executor {
private:
    std::shared_ptr<EventBus> __event_bus;

public:
    Executor(std::shared_ptr<EventBus> event_bus);
    double send(std::vector<Order>& action);
};
