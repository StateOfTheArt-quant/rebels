#pragma once

#include "rebels/object/events.h"
#include "rebels/object/order.h"
#include "rebels/common/eventbus.h"

class Strategy {
private:
    Listener __listener;

public:
    Strategy(std::shared_ptr<EventBus> event_bus);
    void handle_bar(BarEvent event);
};
