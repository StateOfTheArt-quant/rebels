#include <rebels/data/basic.h>
#include <rebels/data/records.h>
#include <rebels/data/streamer.h>
#include <rebels/utilities/csv.h>
#include <rebels/utilities/comparison.h>
#include <iostream>

#include <rebels/market/book.h>
#include <rebels/market/measures.h>

using namespace market::measure;

void print_md_record(const data::MarketDepthRecord& md_rec){

    std::cout << "the record.date: " << md_rec.date << "  the recored.time" << md_rec.time << std::endl;

    std::array<double, 5> ask_prices = md_rec.ask_prices;
    std::array<double, 5> bid_prices = md_rec.bid_prices;
    std::array<long, 5> ask_volumes = md_rec.ask_volumes;
    std::array<long, 5> bid_volumes = md_rec.bid_volumes;

    //for(auto itr = ask_prices.rbegin(); itr != ask_prices.rend(); itr++){
    //    std::cout << *itr << std::endl;
    //}
    for (int m = ask_prices.size() -1; m >=0; m--){
        std::cout << ask_prices[m] << "   " << ask_volumes [m] << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;
    //for (auto i : bid_prices){
    //    std::cout << i << std::endl;
    //}
    for (int n = 0; n <= ask_prices.size() - 1; n++){
        std::cout << bid_prices[n] << "   " << bid_volumes [n] << std::endl;
    }
}



bool update_book_profile(market::AskBook<>& ask_book,
                         market::BidBook<>& bid_book,
                         data::MarketDepth& market_depth,
                         const std::map<double, long, FloatComparator<>>& transactions = {}){
    /*cannot bind non-const lvalue reference of type ‘std::map<double, long int, FloatComparator<> >&’ to an rvalue of type ‘std::map<double, long int, FloatComparator<> >’*/
    ask_book.StashState();
    bid_book.StashState();

    while (true) {
        if (not market_depth.LoadNext())
            return false;

        const data::MarketDepthRecord& rec_md = market_depth.Record();

        ask_book.ApplyChanges(rec_md.ask_prices,
                              rec_md.ask_volumes,
                              transactions);

        bid_book.ApplyChanges(rec_md.bid_prices,
                              rec_md.bid_volumes,
                              transactions);

        if (not market_depth.WillTimeChange()) continue;

        // Use a try as a bit of a shortcut. If the books haven't seen
        // enough (2) states to have a midprice move then it will clearly
        // throw an exception when we try to access it. So, instead, we just
        // use a try and start again if we couldn't.
        try {
            if (market::BookUtils::IsValidState(ask_book, bid_book))
                break;
        } catch (std::runtime_error &e) {
            continue;
        }
    }

    return true;
}

void print_book(market::AskBook<>& ask_book, market::BidBook<>& bid_book){
    std::cout << std::endl << "The books:" << std::endl;
    std::cout << "Ask (ov == " << ask_book.observed_volume() << ")" << std::endl;

    int d = ask_book.depth();
    for (int i = d-1; i >= 0; i--) {
        double price = ask_book.price(i);
        std::cout << "\tLevel " << i+1 << ": " << price << "\t"
                  << ask_book.volume(price) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "\tMid price (-): " << market::measure::midprice(ask_book, bid_book) << std::endl;
    std::cout << "\tMicro price (-): " << market::measure::microprice(ask_book, bid_book) << std::endl << std::endl;

    std::cout << "Bid (ov == " << bid_book.observed_volume() << ")" << std::endl;
    d = bid_book.depth();
    for (int i = 0; i <= d-1; i++) {
        double price = bid_book.price(i);
        std::cout << "\tLevel " << i+1 << ": " << price << "\t"
                  << bid_book.volume(price) << std::endl;
    }

}






int main() {

    std::string md_path = "./data/train/md/000661.SZ/md_20200525.csv";
    std::string tas_path = "./data/train/tas/000661.SZ/td_20200525.csv";

     /*
      *  for order
      *
      * */
    int ORDER_SIZE = 200;

    // to only but once
    bool fired = false;

    data::MarketDepthRecord mdr;
    data::TimeAndSalesRecord tsr;
    mdr.clear();
    tsr.clear();

    data::basic::MarketDepth market_depth;
    data::basic::TimeAndSales time_and_sales;
    market_depth.LoadCSV(md_path);
    time_and_sales.LoadCSV(tas_path);

    market::AskBook<> ask_book_;
    market::BidBook<> bid_book_;
    ask_book_.Reset();
    bid_book_.Reset();

    // first initilazation
    bool first_init = update_book_profile(ask_book_, bid_book_, market_depth);
    if (not first_init){
        return false;
    } else {
        std::cout << "initialization successfully" << std::endl;
    }

    double mp = market::measure::midprice(ask_book_, bid_book_);
    double to_bid_price = (double) mp * (1.0 - 100.0/10000.0);

    // next_state
    for (int i = 0; i < 20; i++) {
        std::cout << i << std::endl;



        int target_date = market_depth.NextDate();      //获取下一条record的Date
        long target_time = market_depth.NextTime();     //获取下一条record的Time

        if (not time_and_sales.LoadUntil(target_date, target_time))     //一直导入数据直至next恰好<=target_time
            continue;
        std::cout << "current datetime : " << target_date << target_time << std::endl;
        const data::TimeAndSalesRecord rec_ts = time_and_sales.Record();
        const data::MarketDepthRecord rec_md = market_depth.Record();
        print_md_record(rec_md);

        if (not fired){
            bid_book_.PlaceOrder(to_bid_price, ORDER_SIZE, 'B');
            fired = true;
        }

        if (bid_book_.HasOpenOrder(to_bid_price)){
            bid_book_.PrintOrders();
        } else {
            std::cout << "the order is executed" << std::endl;
        }

        std::map<double, long, FloatComparator<>> transactions = rec_ts.transactions;
        std::cout << "the quantities of transaction at diffetent price: " << transactions.size() << std::endl;
        for (auto &x: transactions) {
            std::cout << x.first << ": " << x.second << std::endl;
        }

        mp = market::measure::midprice(ask_book_, bid_book_);
        // there is no order ,so there is nothing happens here temporarily
        auto au = ask_book_.ApplyTransactions(rec_ts.transactions, mp),
             bu = bid_book_.ApplyTransactions(rec_ts.transactions, mp);

        update_book_profile(ask_book_, bid_book_, market_depth, transactions);

        print_book(ask_book_, bid_book_);


    }
    return 0;
}
