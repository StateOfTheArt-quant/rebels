#pragma once

#include <iostream>

#include "rebels/eventbus.h"
#include "rebels/object/events.h"
#include "rebels/object/order.h"

class Strategy {
private:
    Listener __listener;

public:
    Strategy(std::shared_ptr<EventBus> event_bus);
    void handle_bar(BarEvent event);
};
