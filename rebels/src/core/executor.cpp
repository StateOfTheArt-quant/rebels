#include <iostream>

#include "rebels/core/context.h"
#include "rebels/core/executor.h"
#include "rebels/object/events.h"

Executor::Executor(std::shared_ptr<EventBus> event_bus) : __event_bus(event_bus) {
    std::cout << "[Executor]: Init publish PostSystemInitEvent ..." << std::endl;
    __event_bus->postpone(PostSystemInitEvent(EventType::POST_SYSTEM_INIT));
    __event_bus->process();
}

std::tuple<double, std::map<std::string, double>> Executor::send(std::vector<Order>& action) {
    std::cout << "[Executor]: publish PreBeforeTradingEvent ..." << std::endl;
    __event_bus->postpone(PreBeforeTradingEvent(EventType::PREBEFORETRADING));
    __event_bus->process();

    std::cout << "[Executor]: publish BeforeTradingEvent ..." << std::endl;
    __event_bus->postpone(BeforeTradingEvent(EventType::BEFORE_TRADING));
    __event_bus->process();

    // error: when event_bus->postpone in event_bus->postpone(when nested event_bus->postpone)
    std::cout << "[Executor]: publish BarEvent ..." << std::endl;
    std::cout << "[Executor]: action size " << action.size() << ", is empty " << std::boolalpha
              << action.empty() << std::endl;
    /// Note: Strategy and Portfolio receieve BarEvent at same time,we must ensure Strategy execute
    /// first, consider using queue?
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

    Context& context = Context::Instance();

    std::vector<double> bar_returns = context.analyzer_ptr->bar_returns();
    std::vector<double> bar_pnl     = context.analyzer_ptr->bar_pnl();

    double reward, pnl;

    if (bar_returns.empty()) {
        std::cout << "[Executor]: bar returns is empty..." << std::endl;
        reward = 0.0;
    } else {
        reward = bar_returns.back();
    }

    if (bar_pnl.empty()) {
        std::cout << "[Executor]: bar pnl is empty..." << std::endl;
        pnl = 0.0;
    } else {
        pnl = bar_pnl.back();
    }

    std::map<std::string, double> info{{"profit_and_loss", pnl}};

    return {reward, info};
}