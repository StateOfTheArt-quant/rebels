#include <rebels/const.h>
#include <rebels/object/trade.h>
#include <rebels/mod/sys_account/position.h>
#include <rebels/mod/sys_account/account.h>
#include <rebels/object/order.h>
#include <iostream>

#include <dexode/EventBus.hpp>
#include <rebels/object/events.h>

using EventBus = dexode::EventBus;

int main(){

    Order first_order{"000001", 300, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 10.0};
    std::cout << "order quantity: " << first_order.quantity() << std::endl;


    Trade first_trade{"000001",  202201011500, 10.0, 100, Side::BUY, PositionEffect::OPEN, 0.0, 0.0};
    //Trade trade2{"000002",  202201011500,10.0, 100, Side::BUY, PositionEffect::OPEN, 0.0, 0.0};
    std::cout << "trade last_quantity: " << first_trade.last_quantity() << std::endl;
    /*
    double init_price = 0;
    std::int64_t init_quantity = 0;
    Position pos{"000001", init_price, init_quantity, PositionDirection::LONG};

    std::cout << "position quantity " << pos.quantity() << std::endl;

    double trade_cost = pos.apply_trade(trade);
    std::cout << "cash changed: " << trade_cost << std::endl;
    */

    //understand account
    std::shared_ptr<EventBus> event_bus = std::make_shared<EventBus>();

    Account account{"stock", 10000, event_bus};

    //Position*  first_position = account.get_or_create_position("000001", PositionDirection::LONG, 100, 10.22);
    //std::cout << first_position -> quantity() << std::endl;

    //
    //Order first_order{};

    event_bus->postpone(OrderPendingNewEvent(EventType::ORDER_PENDING_NEW, first_order));
    event_bus->process();

    std::cout << "order frozen_cash: " << first_order.frozen_cash() << std::endl;
    std::cout << "account frozen_cash: " << account.frozen_cash() << std::endl;

    first_order.fill(first_trade);

    event_bus->postpone(TradeEvent(EventType::TRADE, first_order, first_trade));
    event_bus->process();
    std::cout << "after trade ..." << std::endl;
    std::cout << "account frozen_cash: " << account.frozen_cash() << std::endl;

    std::vector<Position*> pos_vec = account.iter_pos();
    for(auto it=pos_vec.begin(); it != pos_vec.end(); it++){
        std::cout << "instrument_id: "<<(*it) -> instrument_id() << " side: " << (*it) -> market_value() << " quantity:" << (*it) -> quantity() << std::endl;
    }

    std::cout << "before update_last_price..." << std::endl;
    // when does not update_last_price, the last_price == 0, the market_value == 0;


    std::cout << "current equity is " << account.equity() << std::endl;
    std::cout << "current total cash is " << account.total_cash() << std::endl;

    //
    event_bus->postpone(BarEvent(EventType::BAR));
    event_bus->process();

    std::cout << "after update_last_price..." << std::endl;

    for(auto it=pos_vec.begin(); it != pos_vec.end(); it++){
        std::cout << "instrument_id: "<<(*it) -> instrument_id() << " position_direction: " << int((*it) -> position_direction()) << " quantity:" << (*it) -> quantity() << std::endl;
    }

    std::cout << "current equity is " << account.equity() << std::endl;
    std::cout << "current total cash is " << account.total_cash() << std::endl;
    std::cout << "account frozen_cash: " << account.frozen_cash() << std::endl;
    std::cout << "current total value is " << account.total_value() << std::endl;

    return 0;
}
