#include "rebels/mod/sys_analyzer/analyzer.h"
#include "rebels/core/context.h"
#include "rebels/mod/sys_account/portfolio.h"

Analyzer::Analyzer(std::shared_ptr<EventBus> event_bus): __listener{event_bus} {
    __listener.listen<PostSystemInitEvent>(std::bind(&Analyzer::__register_events, this, std::placeholders::_1));
}

void Analyzer::__register_events(PostSystemInitEvent event) {
    __listener.listen<PostSettlementEvent>(std::bind(&Analyzer::__collect_bars, this, std::placeholders::_1));

}


void Analyzer::__collect_bars(PostSettlementEvent event){
    // step 1: get portfolio
    Portfolio& portfolio = *Context::Instance().portfolio_ptr;
    // step 2:
    bar_returns.push_back(portfolio.bar_returns());
}