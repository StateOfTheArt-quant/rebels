#include "rebels/mod/sys_transaction_cost/deciders.h"

StockTransactionCostDecider::StockTransactionCostDecider(double commission_rate,
                                                         double commission_multiplier,
                                                         double min_commission)
    : __commission_rate(commission_rate),
      __commission_multiplier(commission_multiplier),
      __min_commission(min_commission) {}

StockTransactionCostDecider::~StockTransactionCostDecider() {}

double StockTransactionCostDecider::get_order_commission(std::string instrument_id,
                                                         Side side,
                                                         double price,
                                                         long quantity) {
    // unused var
    (void) instrument_id;
    (void) side;
    double commission = price * quantity * __commission_rate * __commission_multiplier;
    return std::max(commission, __min_commission);
}

double StockTransactionCostDecider::get_trade_commission(Trade& trade) {
    double commission      = 0.0;
    double cost_commission = trade.last_price() * static_cast<double>(trade.last_quantity())
                             * __commission_rate * __commission_multiplier;

    // find instrument id from map
    std::string instrument_id = trade.instrument_id();
    auto iter                 = __commission_map.find(instrument_id);

    // check commission is existed otherwise insert a new commission
    if (iter != __commission_map.end()) {
        commission = iter->second;
    } else {
        __commission_map[instrument_id] = commission;
    }

    // compare
    if (cost_commission > commission) {
        __commission_map[instrument_id] = 0.0;
        return commission == __min_commission ? cost_commission : (cost_commission - commission);
    } else {
        __commission_map[instrument_id] -= cost_commission;
        return commission == __min_commission ? commission : 0.0;
    }
}

double StockTransactionCostDecider::get_trade_tax(Trade& trade) {
    return get_tax(trade.instrument_id(), trade.side(), trade.last_price());
}

double StockTransactionCostDecider::get_order_transaction_cost(Order& order) {
    double commission = get_order_commission(
        order.instrument_id(), order.side(), order.frozen_price(), order.quantity());
    double tax = get_tax(order.instrument_id(), order.side(), order.frozen_price());
    return tax + commission;
}

// ---------------------------------------------------------------------
CNStockTransactionCostDecider::CNStockTransactionCostDecider(double commission_multiplier,
                                                             double min_commission,
                                                             double tax_multiplier)
    : __tax_multiplier(tax_multiplier),
      StockTransactionCostDecider(/*commission_rate=*/0.0005,
                                  /*commission_multiplier=*/commission_multiplier,
                                  /*min_commission=*/min_commission) {}

CNStockTransactionCostDecider::~CNStockTransactionCostDecider() {}

double CNStockTransactionCostDecider::get_tax(std::string instrument_id,
                                              Side side,
                                              double cost_money) {
    // TODO get instrument type from data source
    std::string instrument_type = "CS";

    if (side != Side::SELL || instrument_type != "CS") {
        return 0.0;
    }

    return cost_money * __tax_rate * __tax_multiplier;
}
