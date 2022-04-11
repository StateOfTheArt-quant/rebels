#include "rebels/core/executor.h"
#include <iostream>
#include "rebels/object/events.h"

Executor::Executor(std::shared_ptr<EventBus> event_bus):__event_bus(event_bus){

}

double Executor::send(std::vector<Order>& action) {

    // step1:
    //std::cout << "publish BeforeTradingEvent ..." << std::endl;
    //__event_bus->postpone(BeforeTradingEvent(EventType::BEFORE_TRADING));
    //__event_bus->process();

    std::cout << "publish BarEvent ..." << std::endl;
    __event_bus->postpone(BarEvent(EventType::BAR,action));
    __event_bus->process();

    //std::cout << "publish AfterTradingEvent ..." << std::endl;
    //__event_bus->postpone(AfterTradingEvent(EventType::AFTER_TRADING));
    //__event_bus->process();

    //std::cout << "publish SettlementEvent ..." << std::endl;
    //__event_bus->postpone(SettlementEvent(EventType::SETTLEMENT));
    //__event_bus->process();

    return 0.0;
}