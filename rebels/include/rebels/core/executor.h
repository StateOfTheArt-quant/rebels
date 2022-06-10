#pragma once

#include "rebels/object/order.h"
#include "rebels/common/eventbus.h"

#include <vector>
#include <memory>

class Executor {
private:
    std::shared_ptr<EventBus> __event_bus;

public:
    Executor(std::shared_ptr<EventBus> event_bus);
    // consider using struct
    std::tuple<double, std::map<std::string, double>> send(std::vector<Order>& action);
};
