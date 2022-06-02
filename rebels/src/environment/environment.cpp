#include "rebels/environment/environment.h"

TradingEnvironment::TradingEnvironment(std::shared_ptr<DataSource> datasource,
                                       int look_backward_window,
                                       std::map<std::string, double> starting_cash,
                                       std::string mode,
                                       double commission_multiplier,
                                       double min_commission,
                                       double tax_multiplier)
    : __look_backward_window(look_backward_window),
      __mode(std::move(mode)),
      __starting_cash(std::move(starting_cash)),
      __commission_multiplier(commission_multiplier),
      __min_commission(min_commission),
      __tax_multiplier(tax_multiplier) {
    // create context
    Context& context                    = Context::Instance();
    std::shared_ptr<EventBus> event_bus = std::make_shared<EventBus>();

    // partial eventbus
    {
        // mode
        context.set_mode(__mode);

        // datasource
        context.set_data_source(std::move(datasource));

        // broker
        auto broker_ptr = std::make_shared<SimulationBroker>(event_bus);
        context.set_broker(broker_ptr);

        // portfolio
        auto portfolio_ptr = std::make_shared<Portfolio>(__starting_cash, event_bus);
        context.set_portfolio(portfolio_ptr);

        // strategy
        auto strategy_ptr = std::make_shared<Strategy>(event_bus);
        context.set_strategy(strategy_ptr);

        // analyzer
        auto analyzer_ptr = std::make_shared<Analyzer>(event_bus);
        context.set_analyzer(analyzer_ptr);

        // executor
        auto executor_ptr = std::make_shared<Executor>(event_bus);
        context.set_executor(executor_ptr);

        // transaction cost decider
        auto decider_ptr = std::make_shared<CNStockTransactionCostDecider>(
            /*commission_multiplier =*/__commission_multiplier,
            /*min_commission=*/__min_commission,
            /*tax_multiplier=*/__tax_multiplier);
        context.set_transaction_cost_decider("CS", decider_ptr);
    }
}

std::map<std::string, std::map<int, std::tuple<double, double, double, double, double, double>>>
TradingEnvironment::reset() {
    // reload csv stream to the beginning
    return Context::Instance().data_source_ptr->reset();
}

std::tuple<std::map<std::string,
                    std::map<int, std::tuple<double, double, double, double, double, double>>>,
           double,
           bool,
           std::map<std::string, double>>
TradingEnvironment::step(std::vector<Order>& action) {
    double reward;
    std::map<std::string, double> info;

    std::map<std::string,
             std::map<int, std::tuple<double, double, double, double, double, double>>>
        next_state         = Context::Instance().data_source_ptr->step();
    std::tie(reward, info) = Context::Instance().executor_ptr->send(action);

    return {next_state, reward, false, info};
}

/// property
int TradingEnvironment::trading_dt() { return Context::Instance().trading_dt; }