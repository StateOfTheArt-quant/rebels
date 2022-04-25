#include "rebels/core/executor.h"
#include <iostream>
#include "rebels/object/events.h"
#include "rebels/core/context.h"

Executor::Executor(std::shared_ptr<EventBus> event_bus):__event_bus(event_bus){

}

double Executor::send(std::vector<Order>& action) {

    // step1:
    std::cout << "publish BeforeTradingEvent ..." << std::endl;
    __event_bus->postpone(BeforeTradingEvent(EventType::BEFORE_TRADING));
    __event_bus->process();

    // error: when event_bus->postpone in event_bus->postpone(when nested event_bus->postpone)
    //std::cout << "publish BarEvent ..." << std::endl;
    //__event_bus->postpone(BarEvent(EventType::BAR,action));
    //__event_bus->process();
    std::vector<Order> empty;
    Context::Instance().strategy_ptr ->handle_bar(BarEvent(EventType::BAR, empty));

    std::cout << "publish BarEvent ..." << std::endl;
    __event_bus->postpone(BarEvent(EventType::BAR,action));
    __event_bus->process();

    std::cout << "publish AfterTradingEvent ..." << std::endl;
    __event_bus->postpone(AfterTradingEvent(EventType::AFTER_TRADING));
    __event_bus->process();

    std::cout << "publish SettlementEvent ..." << std::endl;
    __event_bus->postpone(SettlementEvent(EventType::SETTLEMENT));
    __event_bus->process();

    std::vector<double> bar_returns = Context::Instance().analyzer_ptr->bars_returns();
    if (bar_returns.empty()) {
        std::cout << "bar returns is empty..." << std::endl;
        return 0.0;
    } else {
        return bar_returns.back();
    }
}