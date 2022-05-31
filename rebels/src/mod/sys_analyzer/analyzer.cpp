#include "rebels/mod/sys_analyzer/analyzer.h"
#include "rebels/core/context.h"
#include "rebels/mod/sys_account/portfolio.h"
#include "rebels/utilities/output.h"

Analyzer::Analyzer(std::shared_ptr<EventBus> event_bus)
    : __listener{event_bus}, __rl_static_unit_net_value{1.0} {
    __listener.listen<PostSystemInitEvent>(
        std::bind(&Analyzer::__register_events, this, std::placeholders::_1));

    // init total value
    __rl_static_total_value = Context::Instance().portfolio_ptr->total_value();
}

void Analyzer::__register_events(PostSystemInitEvent event) {
    __listener.listen<TradeEvent>(
        std::bind(&Analyzer::__collect_trade, this, std::placeholders::_1));

    __listener.listen<OrderCreationPassEvent>(
        std::bind(&Analyzer::__collect_order, this, std::placeholders::_1));

    __listener.listen<PostSettlementEvent>(
        std::bind(&Analyzer::__collect_daily, this, std::placeholders::_1));
}

void Analyzer::__collect_daily(PostSettlementEvent event) {
    // step 1: get portfolio
    Context& context = Context::Instance();
    Portfolio& portfolio = *context.portfolio_ptr;

    // step 2: mode select
    if (Context::Instance().mode == "rl") {
        // calculate
        double next_total_value       = get_next_total_value();
        double current_unit_net_value = portfolio.unit_net_value();  // 当期账户单位净值
        double next_unit_net_value    = next_total_value / portfolio.units();  // 下期账户单位净值
        double pnl                    = next_total_value - __rl_static_total_value;
        double reward                 = next_unit_net_value / current_unit_net_value - 1.0;

        // put it in history reward vector
        __portfolio_current_bar_returns.push_back(reward);
        __portfolio_current_bar_pnl.push_back(pnl);
        // important to update value
        __rl_static_unit_net_value = next_unit_net_value;
        __rl_static_total_value = next_total_value;

        DEBUG_MSG("Analyzer", "rl mode in __collect_daily triggered.");
    } else {
        // daily statistic
        __portfolio_current_bar_returns.push_back(portfolio.daily_returns());
        __portfolio_current_bar_pnl.push_back(portfolio.daily_pnl());

        DEBUG_MSG("Analyzer", "none rl mode in __collect_daily triggered.");
    }

    // step 3(optional): record info(complete in future release)
    // portfolio record, performance record
}

// clang-format off
void Analyzer::__collect_order(OrderCreationPassEvent event) { 
    orders.push_back(event.order); 
}
// clang-format on

void Analyzer::__collect_trade(TradeEvent event) {
    // optional step: record info
    // trade info
}

// clang-format off
double Analyzer::rl_static_unit_net_value() {
    return __rl_static_unit_net_value;
}
// clang-format on

double Analyzer::rl_unit_net_value() {
    Portfolio& portfolio = *Context::Instance().portfolio_ptr;
    return portfolio.total_value() / portfolio.units();
}

// clang-format off
std::vector<double> Analyzer::bar_returns() {
    /// debug message
    DEBUG_MSG("Analyzer", "bar return is {}", __portfolio_current_bar_returns);

    return __portfolio_current_bar_returns;
}

std::vector<double> Analyzer::bar_pnl() {
    return __portfolio_current_bar_pnl;
}
// clang-format on


double Analyzer::get_next_total_value() {
    double next_price, next_market_value, total_value{0.0};

    // get context
    Context& context = Context::Instance();

    auto datasource_ptr = context.data_source_ptr;
    auto accounts_map = context.portfolio_ptr->account_container();

    // std::map<std::string, std::shared_ptr<Account>>
    for (const auto& item: accounts_map) {
        // init
        next_market_value = 0.0;
        auto account_ptr = item.second;

        // query each account buy position
        for (const auto& position : account_ptr->iter_pos()) {         
            // get next trading price
            next_price = datasource_ptr->get_next_price(position->instrument_id(), 0);
            next_market_value += next_price * position->quantity();
        }

        total_value += account_ptr->total_cash() + next_market_value;
        /// debug
        DEBUG_MSG("Analyzer", "{} rl total value is {}", item.first, total_value);
    }

    return total_value;
}