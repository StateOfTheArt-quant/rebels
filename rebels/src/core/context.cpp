#include "rebels/core/context.h"

void Context::update_trading_dt(int trading_dt){
    this -> trading_dt = trading_dt;
}
void Context::set_data_source(std::shared_ptr<DataSource> data_source_ptr){
    this -> data_source_ptr = data_source_ptr;
}
void Context::set_account(std::shared_ptr<Account> account_ptr){
    this -> account_ptr = account_ptr;
}
void Context::set_portfolio(std::shared_ptr<Portfolio> portfolio_ptr){
    this -> portfolio_ptr = portfolio_ptr;
}
void Context::set_broker(std::shared_ptr<SimulationBroker> broker_ptr){
    this -> broker_ptr = broker_ptr;
}

void Context::set_strategy(std::shared_ptr<Strategy> strategy_ptr){
    this -> strategy_ptr = strategy_ptr;
}

void Context::set_analyzer(std::shared_ptr<Analyzer> analyzer_ptr){
    this -> analyzer_ptr = analyzer_ptr;
}

void Context::set_executor(std::shared_ptr<Executor> executor_ptr){
    this -> executor_ptr = executor_ptr;
}