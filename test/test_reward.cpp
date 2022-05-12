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
 * 单账户资金  :    30000元
 * 第一个交易日:    买入000001标的100股，每股单价225元，收盘价为230.77元
 * 第二个交易日:    不进行任何交易，收盘价为231.4元
 * 账户结算情况:    剩余资金->7500元  账户价值->23140元
 * 账户总收益率:    (23140 + 7500) / 30000 - 1 = 0.0213
 */
void single_target_reward() {
    std::string bar_path = "data/FB.csv";
    std::map<std::string, std::string> path_map;
    path_map["000001"] = bar_path;
    auto ds_ptr        = std::make_shared<DataSource>(path_map, 1);

    // init trading env
    auto env   = TradingEnvironment(/*datasource=*/ds_ptr, 1, {{"User001", 30000.0}});
    auto state = env.reset();

    // 添加时间，ctx默认的td为0？
    Context& ctx   = Context::Instance();
    ctx.trading_dt = 20200605;

    // 第一个订单交易日，模拟买入非自动化操作
    Order first_act
        = Order("000001", 100, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 225.0);
    std::vector<Order> action{first_act};

    cout << "---------------------------------day 1------------------------------" << endl;
    // process
    env.step(action);

    // 第二个订单的交易日
    ctx.trading_dt = 20200608;
    // 不做任何操作，买入并持有
    action.clear();

    cout << "---------------------------------day 2------------------------------" << endl;

    auto ret        = env.step(action);
    auto next_state = std::get<0>(ret);
    auto reward     = std::get<1>(ret);
    auto done       = std::get<2>(ret);

    cout << "Account total reward is " << reward << " ,is finished " << boolalpha << done << endl;
}

/**
 * 单账户资金  :    60000元
 * 第一个交易日:    买入000001标的100股，每股单价225元，收盘价为230.77元
 *                  买入000002标的100股，每股单价228元，收盘价为230.77元
 * 第二个交易日:    不进行任何交易，2支标的收盘价均为231.4元
 * 账户结算情况:    剩余资金->14700元  账户价值->46280元
 * 账户总收益率:    (46280 + 14700) / 60000 - 1 = 0.0163
 */
void multi_targets_reward() {
    std::string bar_path = "data/FB.csv";
    std::map<std::string, std::string> path_map;
    path_map["000001"] = bar_path;
    path_map["000002"] = bar_path;
    auto ds_ptr        = std::make_shared<DataSource>(path_map, 1);

    // init trading env
    auto env   = TradingEnvironment(/*datasource=*/ds_ptr, 1, {{"User001", 60000.0}});
    auto state = env.reset();

    // 添加时间，ctx默认的td为0？
    Context& ctx   = Context::Instance();
    ctx.trading_dt = 20200605;

    // 第一个订单交易日，模拟买入非自动化操作
    Order first_act
        = Order("000001", 100, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 225.0);
    Order second_act
        = Order("000002", 100, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 228.0);
    std::vector<Order> action{first_act, second_act};

    cout << "---------------------------------day 1------------------------------" << endl;
    // process
    env.step(action);

    // 第二个订单的交易日
    ctx.trading_dt = 20200608;
    // 不做任何操作，买入并持有
    action.clear();

    cout << "---------------------------------day 2------------------------------" << endl;

    auto ret        = env.step(action);
    auto next_state = std::get<0>(ret);
    auto reward     = std::get<1>(ret);
    auto done       = std::get<2>(ret);

    cout << "Account total reward is " << reward << " ,is finished " << boolalpha << done << endl;
}

int main(int argc, char* argv[]) {
    // finish your code
    single_target_reward();
    std::cout << "*****************************************************************" << std::endl;
    // multi_targets_reward();

    return 0;
}
