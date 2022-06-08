#include "rebels/core/context.h"
#include "rebels/utilities/output.h"

void Context::update_trading_dt(int trading_dt) { this->trading_dt = trading_dt; }

void Context::set_data_source(std::shared_ptr<DataSource> data_source_ptr) {
    this->data_source_ptr = data_source_ptr;
}

void Context::set_account(std::shared_ptr<Account> account_ptr) {
    this->account_ptr = account_ptr;
}

void Context::set_portfolio(std::shared_ptr<Portfolio> portfolio_ptr) {
    this->portfolio_ptr = portfolio_ptr;
}

void Context::set_broker(std::shared_ptr<SimulationBroker> broker_ptr) {
    this->broker_ptr = broker_ptr;
}

void Context::set_strategy(std::shared_ptr<Strategy> strategy_ptr) {
    this->strategy_ptr = strategy_ptr;
}

void Context::set_analyzer(std::shared_ptr<Analyzer> analyzer_ptr) {
    this->analyzer_ptr = analyzer_ptr;
}

void Context::set_executor(std::shared_ptr<Executor> executor_ptr) {
    this->executor_ptr = executor_ptr;
}

void Context::set_mode(std::string mode_str) { this->mode = mode_str; }

void Context::set_transaction_cost_decider(
    std::string instrument_type, std::shared_ptr<CNStockTransactionCostDecider> decider) {
    this->__transaction_cost_decider_dict[instrument_type] = decider;
}

std::shared_ptr<CNStockTransactionCostDecider> Context::get_transaction_cost_decider(
    std::string instrument_id) {
    // TODO: get type from datasource
    (void) instrument_id;
    std::string instrument_type = "CS";
    auto iter                   = __transaction_cost_decider_dict.find(instrument_type);

    if (iter != __transaction_cost_decider_dict.end()) {
        return iter->second;
    }

    return nullptr;
}

double Context::get_trade_tax(Trade& trade) {
    // to check key existed?
    std::string instrument_id = trade.instrument_id();
    auto decider_ptr          = get_transaction_cost_decider(instrument_id);

    if (decider_ptr == nullptr) {
        DEBUG_MSG("Context", "get_trade_tax decider missing {}", instrument_id);
        return 0.0;
    }

    return decider_ptr->get_trade_tax(trade);
}

double Context::get_trade_commission(Trade& trade) {
    // to check key existed?
    std::string instrument_id = trade.instrument_id();
    auto decider_ptr          = get_transaction_cost_decider(instrument_id);

    if (decider_ptr == nullptr) {
        DEBUG_MSG("Context", "get_trade_commission decider missing {}", instrument_id);
        return 0.0;
    }

    return decider_ptr->get_trade_commission(trade);
}

double Context::get_order_transaction_cost(Order& order) {
    // to check key existed?
    std::string instrument_id = order.instrument_id();
    auto iter                 = __transaction_cost_decider_dict.find(instrument_id);

    if (iter == __transaction_cost_decider_dict.end()) {
        DEBUG_MSG("Context", "get_order_transaction_cost decider missing {}", instrument_id);
        return 0.0;
    }

    return iter->second->get_order_transaction_cost(order);
}
