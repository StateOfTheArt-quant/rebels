#pragma once

#include "rebels/data/data_source.h"
#include "rebels/mod/sys_account/account.h"
#include "rebels/mod/sys_account/portfolio.h"
#include "rebels/mod/sys_simulation/simulation_broker.h"
#include "rebels/mod/sys_analyzer/analyzer.h"
#include <dexode/EventBus.hpp>
#include <memory>

using EventBus = dexode::EventBus;
using Listener = dexode::EventBus::Listener;

//Context is a singleton
class Context{
public:
    static Context& Instance(){
        static std::unique_ptr<Context> instance_ptr(new Context()); //static local variable only exec once
        return *instance_ptr;
    }

public:
    Context(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(const Context&) = delete;
    Context& operator=(Context&&) = delete;
    ~Context() = default;

private:
    Context() = default;
//---- for singleton
public:
    int trading_dt{0};
    std::shared_ptr<DataSource> data_source_ptr;
    std::shared_ptr<Account> account_ptr;
    std::shared_ptr<Portfolio> portfolio_ptr;
    std::shared_ptr<SimulationBroker> broker_ptr;
    std::shared_ptr<Analyzer> analyzer_ptr;

public:
    void update_trading_dt(int trading_dt);
    void set_data_source(std::shared_ptr<DataSource> data_source_ptr);
    void set_account(std::shared_ptr<Account> account_ptr);
    void set_portfolio(std::shared_ptr<Portfolio> portfolio_ptr);
    void set_broker(std::shared_ptr<SimulationBroker> broker_ptr);
    void set_analyzer(std::shared_ptr<Analyzer> analyzer_ptr);
    //Account account{0};
    //void set_account(Account my_account){
    //    account = my_account;
    //}
};