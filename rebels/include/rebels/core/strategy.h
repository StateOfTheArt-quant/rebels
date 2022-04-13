#pragma once

#include <dexode/EventBus.hpp>
#include <iostream>

#include "rebels/object/events.h"
#include "rebels/object/order.h"

using EventBus = dexode::EventBus;
using Listener = dexode::EventBus::Listener;


class Strategy{

private:
    Listener __listener;

public:
    Strategy(std::shared_ptr<EventBus> event_bus);
    void handle_bar(BarEvent event);
};
