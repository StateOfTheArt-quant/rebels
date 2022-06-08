#pragma once
#include <map>

#include "rebels/const.h"
#include "rebels/object/order.h"
#include "rebels/object/trade.h"

/// attention: don't call get_trade_tax and get_order_transaction_cost when using base class
/// because base class not achieve get_tax method.
class StockTransactionCostDecider {
private:
    double __commission_rate{0.0};
    double __commission_multiplier{0.0};
    double __min_commission{0.0};
    std::map<std::string, double> __commission_map{};

public:
    StockTransactionCostDecider(double commission_rate,
                                double commission_multiplier,
                                double min_commission);
    ~StockTransactionCostDecider();

protected:
    virtual double get_order_commission(std::string instrument_id,
                                        Side side,
                                        double price,
                                        long quantity);
    virtual double get_tax(std::string instrument_id, Side side, double cost_money) = 0;

public:
    double get_trade_commission(Trade& trade);
    double get_trade_tax(Trade& trade);
    double get_order_transaction_cost(Order& order);
};

class CNStockTransactionCostDecider : public StockTransactionCostDecider {
private:
    double __tax_rate{0.001};
    double __tax_multiplier{0.0};

public:
    CNStockTransactionCostDecider(double commission_multiplier = 1.0,
                                  double min_commission        = 5.0,
                                  double tax_multiplier        = 1.0);
    ~CNStockTransactionCostDecider();

protected:
    double get_tax(std::string instrument_id, Side side, double cost_money) override;
};