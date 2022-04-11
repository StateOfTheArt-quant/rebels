#include <rebels/data/basic.h>
#include <rebels/data/records.h>
#include <rebels/data/streamer.h>
#include <rebels/utilities/csv.h>
#include <rebels/utilities/comparison.h>
#include <iostream>


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

void print_ts_record(const data::TimeAndSalesRecord& ts_record){
    std::cout << "the ts record.date: " << ts_record.date << "  the recored.time " << ts_record.time << std::endl;
    std::cout << "the quantities of transaction at diffetent price: " << ts_record.transactions.size() << std::endl;
    for(auto &x:ts_record.transactions){
        std::cout << x.first << ": " << x.second << std::endl;
    }
}


int main(){

    std::string md_path = "./data/train/md/000661.SZ/md_20200525.csv";
    std::string tas_path = "./data/train/tas/000661.SZ/td_20200525.csv";

    CSV csv_md;
    csv_md.openFile(md_path);
    bool is_open= csv_md.isOpen();
    std::cout << is_open << std::endl;
    csv_md.skip(1);
    bool has_data = csv_md.hasData();
    std::cout << has_data << std::endl;

    CSV csv_td;
    csv_td.openFile(tas_path);
    is_open= csv_td.isOpen();
    std::cout << is_open << std::endl;

    data::MarketDepthRecord mdr;
    data::TimeAndSalesRecord tsr;
    mdr.clear();
    tsr.clear();

    data::basic::MarketDepth market_depth;
    data::basic::TimeAndSales time_and_sales;
    //data::basic::MarketDepth market_depth{md_path};
    //data::basic::TimeAndSales time_and_sales{tas_path};
    //market_depth.Reset();
    //time_and_sales.Reset();

    market_depth.LoadCSV(md_path);
    time_and_sales.LoadCSV(tas_path);

    //std::string symbol = ticker.substr(0, ticker.find_first_of('.'));
    //std::string venue  = ticker.substr(ticker.find_first_of('.') + 1);
    data::MarketDepthRecord md_record_last = market_depth.record_last;
    data::MarketDepthRecord md_record_current = market_depth.record_curr;
    data::MarketDepthRecord md_record_next = market_depth.record_next;

    std::cout << "when first load csv..................................";
    std::cout <<"record_last snapshot: \n";
    print_md_record(md_record_last);

    std::cout <<"record_current snapshot: \n";
    print_md_record(md_record_current);

    std::cout <<"record_next snapshot: \n";
    print_md_record(md_record_next);

    std::cout << "record_next.date: " << md_record_next.date << std::endl;
    std::cout << "record_next.time: " << md_record_next.time << std::endl;

    std::cout << "market_depth.NextDate(): " << market_depth.NextDate() << std::endl;
    std::cout << "market_depth.NextTime(): " << market_depth.NextTime() << std::endl;

    /*
     *  template<typename R>
        int Streamer<R>::NextDate(){
            return record_next.date;
        }

         template<typename R>
         long Streamer<R>::NextTime(){
            return record_next.time;
        }
     */


    data::TimeAndSalesRecord ts_record_last = time_and_sales.record_last;
    data::TimeAndSalesRecord ts_record_current = time_and_sales.record_curr;
    data::TimeAndSalesRecord ts_record_next = time_and_sales.record_next;

    std::cout << "when first load csv..................................";
    std::cout <<"ts_record_last snapshot: \n";
    print_ts_record(ts_record_last);

    std::cout <<"ts_record_current snapshot: \n";
    print_ts_record(ts_record_current);

    std::cout <<"ts_record_next snapshot: \n";
    print_ts_record(ts_record_next);

    std::cout << "ts_record_next.date: " << ts_record_next.date << std::endl;
    std::cout << "ts_record_next.time: " << ts_record_next.time << std::endl;

    std::cout << "time_and_sales.NextDate(): " << time_and_sales.NextDate() << std::endl;
    std::cout << "ime_and_sales.NextTime(): " << time_and_sales.NextTime() << std::endl;



    for (int i=0; i < 20; i++){
        std::cout << i << std::endl;
        int target_date = market_depth.NextDate();      //获取下一条record的Date
        long target_time = market_depth.NextTime();     //获取下一条record的Time

        // 此时的time_and_sales获得是 record_curr.date - record_curr.date 区间的逐笔成交数据
        if (not time_and_sales.LoadUntil(target_date, target_time))     //一直导入数据直至next恰好<=target_time
            continue;
        std::cout << "target datetime : " << target_date << target_time << std::endl;
        // 此时的time_and_sales.Record是target dateime的成交数据
        const data::TimeAndSalesRecord& rec_ts = time_and_sales.Record();

        //此时的time_and_sales.Record是上一个时刻的快照数据
        const data::MarketDepthRecord md_rec = market_depth.Record();  // return record_curr
        /*
         *  template<typename R>
            const R& Streamer<R>::Record()
            {
            return record_curr;
            }
         */

        std::map<double, long, FloatComparator<>> transactions = rec_ts.transactions;
        print_ts_record(rec_ts);
        //std::cout << "the quantities of transaction at diffetent price: " << transactions.size() << std::endl;
        //for(auto &x:transactions){
        //    std::cout << x.first << ": " << x.second << std::endl;
        //}
        std::cout << "the current snapshot market_depth.Record() is :" << std::endl;
        print_md_record(md_rec);

        double mean_price = rec_ts.mean_price();
        std::cout << "middle price: " << mean_price << std::endl;

        while (true){
            if (not market_depth.LoadNext()){
                continue;
            } else{
                break;
            }
        }

    }

    return 0;
}
