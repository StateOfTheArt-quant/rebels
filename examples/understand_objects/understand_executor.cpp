#include <rebels/const.h>
#include <rebels/mod/sys_account/account.h>
#include <rebels/mod/sys_account/portfolio.h>
#include <rebels/object/order.h>
#include <rebels/mod/sys_simulation/simulation_broker.h>
#include <rebels/mod/sys_simulation/matcher.h>
#include <rebels/core/executor.h>
#include <rebels/core/strategy.h>
#include <rebels/core/context.h>

#include <iostream>
#include <string>
#include <dexode/EventBus.hpp>
#include <rebels/object/events.h>

using EventBus = dexode::EventBus;

int main(){

    Order first_order{"000001", 300, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 10.0};
    //std::cout << "order quantity: " << first_order.quantity() << std::endl;
    std::unique_ptr<Order> first_order_ptr = std::make_unique<Order>("000001", 300, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 10.0);
    std::vector<Order> order_vectors;
    order_vectors.push_back(first_order);

    //create context
    Context& context = Context::Instance();

    //understand account
    std::shared_ptr<EventBus> event_bus = std::make_shared<EventBus>();


    // create account
    Account account{"stock", 10000, event_bus};
    //std::shared_ptr<Account> account_ptr = std::make_shared<Account>("stock",10000.00, event_bus);

    //create portfolio
    //std::map<std::string, double> account_start_cash;
    //account_start_cash.emplace("stock", 10000.00);
    //Portfolio portfolio{account_start_cash,event_bus}; //
    //std::shared_ptr<Portfolio> portfolio_ptr = std::make_shared<Portfolio>(account_start_cash, event_bus);
    //context.set_portfolio(portfolio_ptr);

    // create strategy
    //Strategy strategy{event_bus};
    std::shared_ptr<Strategy> strategy_ptr = std::make_shared<Strategy>(event_bus);
    context.set_strategy(strategy_ptr);

    //create broker
    //SimulationBroker broker{event_bus};
    //broker.submit_order(std::move(first_order_ptr));

    std::shared_ptr<SimulationBroker> broker_ptr = std::make_shared<SimulationBroker>(event_bus);
    context.set_broker(broker_ptr);
    //std::cout << "broker_ptr ->submit_order" << std::endl;
    //broker_ptr ->submit_order(std::move(first_order_ptr));

    //Context::Instance().broker_ptr ->submit_order(std::move(first_order_ptr));
    //std::shared_ptr<Analyzer> analyzer_ptr = std::make_shared<Analyzer>(event_bus);
    //context.set_analyzer(analyzer_ptr);

    //std::cout << "strategy_ptr ->handle_bar" << std::endl;
    //std::vector<Order> empty;
    //strategy_ptr ->handle_bar(BarEvent(EventType::BAR, empty));

    //{
    //    std::cout << "event_bus->postpone(BarEvent" << std::endl;
    //    event_bus->postpone(BarEvent(EventType::BAR, empty));
    //    event_bus->process();
    //}
    //event_bus->process();
    //event_bus->postpone(OrderPendingNewEvent(EventType::ORDER_PENDING_NEW, first_order)); //pass-by-reference
    //event_bus->process();

    //event_bus->postpone(OrderCreationPassEvent(EventType::ORDER_CREATION_PASS, first_order)); //pass-by-reference
    //event_bus->process();

    Executor executor{event_bus};
    executor.send(order_vectors);

    //std::cout << "after update_last_price..." << std::endl;
    //std::cout << "current equity is " << account.equity() << std::endl;
    //std::cout << "current total cash is " << account.total_cash() << std::endl;
    //std::cout << "account frozen_cash: " << account.frozen_cash() << std::endl;
    //std::cout << "current total value is " << account.total_value() << std::endl;

    return 0;
}
