/**
 * @file test_reward.cpp
 * @author zhongziyang (hankknight@live.com)
 * @brief unittest for single or multi target reward accuracy(googletest support later)
 * @date 2022-05-12 13:05:78
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>

#include "rebels/core/context.h"
#include "rebels/data/data_source.h"
#include "rebels/environment/environment.h"

using namespace std;

/**
 * 计算收益模式:    non-rl
 * 单账户资金  :    30000元
 * 第一个交易日:    买入000001标的100股，每股单价226.29元，收盘价为230.77元
 * 第二个交易日:    不进行任何交易，收盘价为231.4元
 * 期望收益率  :    [0, 0.149333, 0.002069]
 */
void single_target_reward() {
    std::string bar_path = "data/FB.csv";
    std::map<std::string, std::string> path_map;
    path_map["000001"] = bar_path;
    auto ds_ptr        = std::make_shared<DataSource>(path_map, 1);

    // init trading env
    auto env   = TradingEnvironment(/*datasource=*/ds_ptr,
                                  /*look_backward_window=*/1,
                                  /*starting_cash=*/{{"User001", 30000.0}},
                                  /*mode=*/"non-rl");
    auto state = env.reset();

    // 获取上下文信息
    Context& ctx = Context::Instance();

    // 模拟买入
    Order first_act = Order(
        "000001", 100, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, /*price= */ 226.29);
    std::vector<Order> action{first_act};

    cout << "---------------------------------day 1------------------------------" << endl;
    // 第一个订单交易日
    ctx.trading_dt = 20200604;
    auto fret      = env.step(action);
    std::cout << ctx.trading_dt << " Account total reward is " << std::get<1>(fret) << std::endl;

    cout << "---------------------------------day 2------------------------------" << endl;
    // 第二个订单的交易日
    ctx.trading_dt = 20200605;
    // 不做任何操作，买入并持有
    action.clear();
    auto sret = env.step(action);
    cout << ctx.trading_dt << " Account total reward is " << std::get<1>(sret) << endl;

    cout << "---------------------------------day 3------------------------------" << endl;
    ctx.trading_dt = 20200608;
    auto tret      = env.step(action);
    std::cout << ctx.trading_dt << " Account total reward is " << std::get<1>(tret) << std::endl;
}

/**
 * 计算收益模式:    non-rl
 * 单账户资金  :    60000元
 * 第一个交易日:    买入000001标的100股，每股单价226.29元，收盘价为230.77元
 *                  买入000002标的100股，每股单价226.29元，收盘价为230.77元
 * 第二个交易日:    不进行任何交易，2支标的收盘价均为231.4元
 * 期望收益率  :    [0, 0.149333, 0.002069]
 */
void multi_targets_reward() {
    std::string bar_path = "data/FB.csv";
    std::map<std::string, std::string> path_map;
    path_map["000001"] = bar_path;
    path_map["000002"] = bar_path;
    auto ds_ptr        = std::make_shared<DataSource>(path_map, 1);

    // init trading env
    auto env   = TradingEnvironment(/*datasource=*/ds_ptr,
                                  /*look_backward_window=*/1,
                                  /*starting_cash=*/{{"User001", 60000.0}},
                                  /*mode=*/"non-rl");
    auto state = env.reset();

    // 获取上下文信息
    Context& ctx = Context::Instance();

    // 模拟买入
    Order first_act
        = Order("000001", 100, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 226.29);
    Order second_act
        = Order("000002", 100, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 226.29);
    std::vector<Order> action{first_act, second_act};

    cout << "---------------------------------day 1------------------------------" << endl;
    // 第一个订单交易日
    ctx.trading_dt = 20200604;
    auto fret      = env.step(action);
    std::cout << ctx.trading_dt << " Account total reward is " << std::get<1>(fret) << std::endl;

    cout << "---------------------------------day 2------------------------------" << endl;
    // 第二个订单的交易日
    ctx.trading_dt = 20200605;
    // 不做任何操作，买入并持有
    action.clear();
    auto sret = env.step(action);
    std::cout << ctx.trading_dt << " Account total reward is " << std::get<1>(sret) << std::endl;

    cout << "---------------------------------day 3------------------------------" << endl;
    ctx.trading_dt = 20200608;
    auto tret      = env.step(action);
    std::cout << ctx.trading_dt << " Account total reward is " << std::get<1>(tret) << std::endl;
}

int main(int argc, char* argv[]) {
    // finish your code
    std::cout << "*********************************single********************************"
              << std::endl;
    single_target_reward();
    std::cout << "*********************************multi********************************"
              << std::endl;
    // multi_targets_reward();

    return 0;
}
