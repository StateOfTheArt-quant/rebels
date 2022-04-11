#pragma once

#include <string>
#include <map>
#include <memory> //for std:unique_ptr
#include <vector>
#include "rebels/mod/sys_account/portfolio.h"
#include "rebels/object/events.h"
#include "rebels/const.h"

#include <dexode/EventBus.hpp>
using EventBus = dexode::EventBus;
using Listener = dexode::EventBus::Listener;


class Analyzer{

private:
    Listener __listener;
    void __register_events(PostSystemInitEvent event);
    void __collect_bars(PostSettlementEvent event);

public:
    std::vector<double> bar_returns;

    Analyzer(std::shared_ptr<EventBus> event_bus);
};
