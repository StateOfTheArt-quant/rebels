#pragma once

#include "rebels/data/data_source.h"
#include "rebels/mod/sys_account/account.h"
#include "rebels/mod/sys_account/portfolio.h"
#include "rebels/mod/sys_simulation/simulation_broker.h"
#include "rebels/mod/sys_transaction_cost/deciders.h"
#include "rebels/mod/sys_analyzer/analyzer.h"
#include "rebels/core/strategy.h"
#include "rebels/core/executor.h"
#include "rebels/common/eventbus.h"

#include <memory>

// Context is a singleton
class Context {
public:
    static Context& Instance() {
        static std::unique_ptr<Context> instance_ptr(
            new Context());  // static local variable only exec once
        return *instance_ptr;
    }

public:
    Context(const Context&) = delete;
    Context(Context&&)      = delete;
    Context& operator=(const Context&) = delete;
    Context& operator=(Context&&) = delete;
    ~Context()                    = default;

private:
    Context() = default;

private:
    std::map<std::string, std::shared_ptr<CNStockTransactionCostDecider>>
        __transaction_cost_decider_dict;
    std::shared_ptr<CNStockTransactionCostDecider> get_transaction_cost_decider(
        std::string instrument_id);

    //---- for singleton
public:
    int trading_dt{0};
    std::string mode{"rl"};

    /// pointer
    std::shared_ptr<DataSource> data_source_ptr;
    std::shared_ptr<Account> account_ptr;
    std::shared_ptr<Portfolio> portfolio_ptr;
    std::shared_ptr<SimulationBroker> broker_ptr;
    std::shared_ptr<Strategy> strategy_ptr;
    std::shared_ptr<Analyzer> analyzer_ptr;
    std::shared_ptr<Executor> executor_ptr;

public:
    void update_trading_dt(int trading_dt);
    void set_data_source(std::shared_ptr<DataSource> data_source_ptr);
    void set_account(std::shared_ptr<Account> account_ptr);
    void set_portfolio(std::shared_ptr<Portfolio> portfolio_ptr);
    void set_broker(std::shared_ptr<SimulationBroker> broker_ptr);
    void set_analyzer(std::shared_ptr<Analyzer> analyzer_ptr);
    void set_strategy(std::shared_ptr<Strategy> strategy_ptr);
    void set_executor(std::shared_ptr<Executor> executor_ptr);
    void set_mode(std::string mode_str);
    void set_transaction_cost_decider(std::string instrument_type,
                                      std::shared_ptr<CNStockTransactionCostDecider> decider);

    // transcation cost related
    double get_trade_tax(Trade& trade);
    double get_trade_commission(Trade& trade);
    double get_order_transaction_cost(Order& order);
};