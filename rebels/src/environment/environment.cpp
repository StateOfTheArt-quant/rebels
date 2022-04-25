#include "rebels/environment/environment.h"

TradingEnvironment::TradingEnvironment() {
    // create context
    Context& context                    = Context::Instance();
    std::shared_ptr<EventBus> event_bus = std::make_shared<EventBus>();

    {
        // create portfolio
        std::map<std::string, double> account_start_cash;
        account_start_cash.emplace("stock", 10000.00);
        std::shared_ptr<Portfolio> portfolio_ptr
            = std::make_shared<Portfolio>(account_start_cash, event_bus);
        context.set_portfolio(portfolio_ptr);

        // create strategy
        // Strategy strategy{event_bus};
        std::shared_ptr<Strategy> strategy_ptr = std::make_shared<Strategy>(event_bus);
        context.set_strategy(strategy_ptr);

        // create broker
        // SimulationBroker broker{event_bus};
        // broker.submit_order(std::move(first_order_ptr));

        std::shared_ptr<SimulationBroker> broker_ptr
            = std::make_shared<SimulationBroker>(event_bus);
        context.set_broker(broker_ptr);

        std::shared_ptr<Analyzer> analyzer_ptr = std::make_shared<Analyzer>(event_bus);
        context.set_analyzer(analyzer_ptr);

        std::shared_ptr<Executor> executor_ptr = std::make_shared<Executor>(event_bus);
        context.set_executor(executor_ptr);
    }
}

DataSource::BAR_MULTI TradingEnvironment::reset() {
    // reload csv stream to the beginning
    return Context::Instance().data_source_ptr->reset();
}

std::tuple<DataSource::BAR_MULTI, double, bool> TradingEnvironment::step(
    std::vector<Order>& action) {
    DataSource::BAR_MULTI next_state = Context::Instance().data_source_ptr->step();
    double step_reward               = Context::Instance().executor_ptr->send(action);

    return {next_state, step_reward, false};
}
