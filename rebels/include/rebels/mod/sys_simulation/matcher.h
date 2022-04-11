#pragma once

#include "rebels/object/order.h"
#include <dexode/EventBus.hpp>

using EventBus = dexode::EventBus;
using Listener = dexode::EventBus::Listener;

class DefaultMatcher{
public:
    std::shared_ptr<EventBus> __event_bus;

    DefaultMatcher();
    DefaultMatcher(std::shared_ptr<EventBus> event_bus);
    void match(Order& order);
};