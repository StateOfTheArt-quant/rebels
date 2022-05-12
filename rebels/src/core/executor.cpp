#include <iostream>

#include "rebels/core/context.h"
#include "rebels/core/executor.h"
#include "rebels/object/events.h"

Executor::Executor(std::shared_ptr<EventBus> event_bus) : __event_bus(event_bus) {
    std::cout << "[Executor]: Init publish PostSystemInitEvent ..." << std::endl;
    __event_bus->postpone(PostSystemInitEvent(EventType::POST_SYSTEM_INIT));
    __event_bus->process();
}

double Executor::send(std::vector<Order>& action) {

    std::cout << "[Executor]: publish PreBeforeTradingEvent ..." << std::endl;
    __event_bus->postpone(PreBeforeTradingEvent(EventType::PREBEFORETRADING));
    __event_bus->process();

    std::cout << "[Executor]: publish BeforeTradingEvent ..." << std::endl;
    __event_bus->postpone(BeforeTradingEvent(EventType::BEFORE_TRADING));
    __event_bus->process();

    // error: when event_bus->postpone in event_bus->postpone(when nested event_bus->postpone)
    std::cout << "[Executor]: publish BarEvent ..." << std::endl;
    std::cout << "[Executor]: action size " << action.size() << ", is empty " << std::boolalpha << action.empty() << std::endl;
    /// Note: Strategy and Portfolio receieve BarEvent at same time,we must ensure Strategy execute first, consider using
    /// queue?
    Context::Instance().strategy_ptr->handle_bar(BarEvent(EventType::BAR, action));
    __event_bus->postpone(BarEvent(EventType::BAR, action));
    __event_bus->process();

    std::cout << "[Executor]: publish AfterTradingEvent ..." << std::endl;
    __event_bus->postpone(AfterTradingEvent(EventType::AFTER_TRADING));
    __event_bus->process();

    std::cout << "[Executor]: publish SettlementEvent ..." << std::endl;
    __event_bus->postpone(SettlementEvent(EventType::SETTLEMENT));
    __event_bus->process();

    std::cout << "[Executor]: publish PostSettlementEvent ..." << std::endl;
    __event_bus->postpone(PostSettlementEvent(EventType::POSTSETTLEMENT));
    __event_bus->process();

    std::vector<double> bar_returns = Context::Instance().analyzer_ptr->bars_returns();

    if (bar_returns.empty()) {
        std::cout << "[Executor]: bar returns is empty..." << std::endl;
        return 0.0;
    }

    return bar_returns.back();
}