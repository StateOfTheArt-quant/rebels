#include "rebels/core/strategy.h"
#include <memory>
#include <functional>
#include "rebels/object/order.h"
#include "rebels/core/context.h"

Strategy::Strategy(std::shared_ptr<EventBus> event_bus):__listener{event_bus}{

    //__listener.listen<BarEvent>(std::bind(&Strategy::handle_bar, this, std::placeholders::_1));
}

void Strategy::handle_bar(BarEvent event){
    std::cout << "exec handle_bar" << std::endl;
    //std::vector<Order>& orders_ = event.orders;

    std::unique_ptr<Order> order_ptr = std::make_unique<Order>("000001", 300, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 10.0);
    Context::Instance().broker_ptr -> submit_order(std::move(order_ptr));   //transfer ownership of order into broker
    std::cout << "submit order done..." << std::endl;
    /*
    for(auto it = orders_.begin(); it != orders_.end(); it++){
        //std::unique_ptr<Order> order_ptr = std::make_unique<Order>(*it); //call copy constructor
        std::cout << "to submit order..." << std::endl;
        //std::cout << "order instrument_id: " << order_ptr->instrument_id() << "quantity: " <<  order_ptr->quantity() << std::endl;
        Order first_order{"000001", 300, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 10.0};
        std::unique_ptr<Order> order_ptr = std::make_unique<Order>(first_order);
        Context::Instance().broker_ptr -> submit_order(std::move(order_ptr));   //transfer ownership of order into broker
        std::cout << "submit order done..." << std::endl;
    }
    */
}

