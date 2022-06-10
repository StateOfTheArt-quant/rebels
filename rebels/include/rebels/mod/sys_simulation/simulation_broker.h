#pragma once

#include <vector>
#include <memory>
#include "rebels/object/events.h"
#include "rebels/mod/sys_simulation/matcher.h"
#include "rebels/common/eventbus.h"

class SimulationBroker {
private:
    Listener __listener;
    std::shared_ptr<EventBus> __event_bus;

    DefaultMatcher __matcher;  //

private:
    bool __match_immediately{true};
    std::vector<std::unique_ptr<Order>> __open_orders;

    void __before_trading(BeforeTradingEvent event);
    void __after_trading(AfterTradingEvent event);
    void __on_bar(BarEvent event);
    void __pre_settlement(PreSettlementEvent event);

    void __match();

public:
    SimulationBroker();
    SimulationBroker(std::shared_ptr<EventBus> event_bus);
    void register_event();
    void submit_order(std::unique_ptr<Order> order_ptr);
    // void cancel_order(std::unique_ptr<Order> order_ptr);
};