#include "rebels/mod/sys_simulation/simulation_broker.h"
#include "rebels/object/events.h"
#include "rebels/const.h"

#include <iterator> //for std::back_inserter
#include <iostream>

SimulationBroker::SimulationBroker(){

}
SimulationBroker::SimulationBroker(std::shared_ptr<EventBus> event_bus): __listener{event_bus}, __event_bus(event_bus){
    DefaultMatcher matcher{event_bus};
    __matcher = matcher;
    register_event();
}

void SimulationBroker::register_event() {
    std::cout << "SimulationBroker::register_event ..." << std::endl;
    __listener.listen<BeforeTradingEvent>(std::bind(&SimulationBroker::__before_trading, this, std::placeholders::_1));
    __listener.listen<AfterTradingEvent>(std::bind(&SimulationBroker::__after_trading, this, std::placeholders::_1));
    __listener.listen<BarEvent>(std::bind(&SimulationBroker::__on_bar, this, std::placeholders::_1));
    __listener.listen<PreSettlementEvent>(std::bind(&SimulationBroker::__pre_settlement, this, std::placeholders::_1));
}


void SimulationBroker::__before_trading(BeforeTradingEvent event){
    std::cout << "SimulationBroker::__before_trading ..." << std::endl;
    for(auto it = __open_orders.begin(); it != __open_orders.end(); it++){
        (*it)->active();
        __event_bus->postpone(OrderCreationPassEvent(EventType::ORDER_CREATION_PASS, **it));
        __event_bus->process();
    }
}

void SimulationBroker::__after_trading(AfterTradingEvent event){
    for(auto it = __open_orders.begin(); it != __open_orders.end(); it++){
        (*it)->mark_rejected("Order rejected: market close");
        __event_bus->postpone(OrderUnsolicitedUpdateEvent(EventType::ORDER_UNSOLICITED_UPDATE, **it));
        __event_bus->process();
    }
    __open_orders.clear();
}

void SimulationBroker::__pre_settlement(PreSettlementEvent event){
    // pass
}

void SimulationBroker::__on_bar(BarEvent event){
    __match();
}

void SimulationBroker::__match(){
    std::cout << "SimulationBroker::__match() ... " << std::endl;
    // step1 match orders in __open_orders
    for(auto it = __open_orders.begin(); it != __open_orders.end(); it++) {
        __matcher.match(**it);
    }

    // step2: get final orders
    std::vector<std::unique_ptr<Order>> final_orders;
    std::copy_if(std::make_move_iterator(__open_orders.begin()), std::make_move_iterator(__open_orders.end()), std::back_inserter(final_orders), [](const std::unique_ptr<Order>& ptr){return (ptr->is_final());});

    // step3: remove order whose status is not_final()
    // (attention) remove the order in __open_orders, but the Resource has not be released. when to release??
    __open_orders.erase(std::remove_if(__open_orders.begin(), __open_orders.end(),[](const std::unique_ptr<Order>& ptr){return (ptr == nullptr);}), __open_orders.end());

    //step4: for final order
    for(auto it = final_orders.begin(); it !=final_orders.end(); it++){
        if ((*it)->status() == OrderStatus::REJECTED || (*it)->status() == OrderStatus::CANCELLED){
            __event_bus->postpone(OrderUnsolicitedUpdateEvent(EventType::ORDER_UNSOLICITED_UPDATE, **it));
            __event_bus->process();
        }
        (*it).reset(); // release the object points to.
    }
}

void SimulationBroker::submit_order(std::unique_ptr<Order> order_ptr){
    // step 1: publish ORDER_PENDING_NEW event

    std::cout << "publish OrderPendingNewEvent... " << std::endl;
    std::cout << "order instrument_id: " << order_ptr->instrument_id() << " quantity: " <<  order_ptr->quantity() << std::endl;
    __event_bus->postpone(OrderPendingNewEvent(EventType::ORDER_PENDING_NEW, *order_ptr)); //pass-by-reference
    __event_bus->process();

    if(order_ptr -> is_final()){
        return;
    }

    // step2:
    order_ptr -> active();
    std::cout << "publish OrderCreationPassEvent... " << std::endl;
    __event_bus->postpone(OrderCreationPassEvent(EventType::ORDER_CREATION_PASS, *order_ptr)); //pass-by-reference
    __event_bus->process();

    // step3: move it into __open_orders
    __open_orders.push_back(std::move(order_ptr));
    if (__match_immediately){
        __match();
    }
}


