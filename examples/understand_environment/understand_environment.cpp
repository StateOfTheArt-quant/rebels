/**
 * @file understand_environment.cpp
 * @author zhongziyang (hankknight@live.com)
 * @brief Quick start of environment
 * @date 2022-05-10 15:05:74
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>

#include "rebels/core/context.h"
#include "rebels/data/data_source.h"
#include "rebels/environment/environment.h"

using namespace std;

void buy_and_hold() {
    // 初始化数据集
    std::string bar_path                        = "data/FB.csv";
    std::map<std::string, std::string> path_map = {{"000001", bar_path}, {"000002", bar_path}};
    auto ds_ptr                                 = std::make_shared<DataSource>(path_map, 1);

    // 初始化回测环境
    auto env     = TradingEnvironment(/*datasource=*/ds_ptr,
                                  /*look_backward_window=*/1,
                                  /*starting_cash=*/{{"User001", 30000.0}});
    auto state   = env.reset();
    Context& ctx = Context::Instance();

    // 以 225元 买入 "000001" 标的100股
    Order first_act
        = Order("000001", 100, Side::BUY, PositionEffect::OPEN, OrderType::LIMIT, 225.0);
    // 订单配置
    std::vector<Order> action;
    action.push_back(first_act);

    cout << "---------------------------------split line------------------------------" << endl;
    // 第一个交易日
    ctx.trading_dt = 20200605;
    env.step(action);

    // 第二个交易日
    ctx.trading_dt = 20200608;
    // 不做任何操作，买入并持有
    action.clear();

    cout << "---------------------------------split line------------------------------" << endl;
    auto ret        = env.step(action);
    auto next_state = std::get<0>(ret);
    auto reward     = std::get<1>(ret);
    auto done       = std::get<2>(ret);

    cout << "reward is " << reward << ", done is " << boolalpha << done << endl;
}

int main(int argc, char* argv[]) {
    buy_and_hold();

    return 0;
}
