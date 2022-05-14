#include <rebels/core/context.h>
#include <rebels/core/strategy.h>
#include <rebels/core/executor.h>
#include <rebels/object/order.h>

#include <dexode/EventBus.hpp>
#include <rebels/object/events.h>
#include <vector>

using EventBus = dexode::EventBus;
using Listener = dexode::EventBus::Listener;

int main(){
    auto event_bus = std::make_shared<EventBus>();
    //Context context{event_bus};
    Strategy strategy{event_bus};
    Executor executor{event_bus};

    std::vector<Order> action;
    Order first_order{};
    action.push_back(first_order);

    // double reward = executor.send(action);
    //event_bus->postpone(BarEvent(EventType::BAR));
    //event_bus->process();

    return 0;
}
