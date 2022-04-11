#include <rebels/const.h>
#include <rebels/mod/sys_account/account.h>
#include <rebels/object/order.h>
#include <rebels/mod/sys_simulation/simulation_broker.h>
#include <rebels/mod/sys_simulation/matcher.h>
#include <iostream>

#include <dexode/EventBus.hpp>
#include <rebels/object/events.h>

using EventBus = dexode::EventBus;

int main(){

    std::unique_ptr<Order> first_order_ptr = std::make_unique<Order>("000001", 300, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 10.0);
    std::cout << "order quantity: " << first_order_ptr -> quantity() << std::endl;
    //understand account
    std::shared_ptr<EventBus> event_bus = std::make_shared<EventBus>();
    // create_account
    Account account{"stock", 10000, event_bus};

    // create matcher
    //DefaultMatcher matcher{event_bus};
    //create broker
    SimulationBroker broker{event_bus};

    broker.submit_order(std::move(first_order_ptr));
    std::cout << "before update_last_price..." << std::endl;
    // when does not update_last_price, the last_price == 0, the market_value ==0;


    std::cout << "current equity is " << account.equity() << std::endl;
    std::cout << "current total cash is " << account.total_cash() << std::endl;
    std::cout << "account frozen_cash: " << account.frozen_cash() << std::endl;
    std::cout << "current total value is " << account.total_value() << std::endl;

    //
    std::vector<Order> empty;
    event_bus->postpone(BarEvent(EventType::BAR, empty));
    event_bus->process();

    std::cout << "after update_last_price..." << std::endl;
    std::cout << "current equity is " << account.equity() << std::endl;
    std::cout << "current total cash is " << account.total_cash() << std::endl;
    std::cout << "account frozen_cash: " << account.frozen_cash() << std::endl;
    std::cout << "current total value is " << account.total_value() << std::endl;

    return 0;
}


