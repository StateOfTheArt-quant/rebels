#include <iostream>
#include <vector>
#include <tuple>
#include <rebels/data/records.h>
#include <rebels/data/basic.h>
#include <rebels/utilities/csv.h>

void print_bar_record(data::BarRecord bar_record) {

    int date = bar_record.date;
    std::cout << "date: " << date << std::endl;

    std::tuple<double, double, double, double, double, double> bar = bar_record.bar;
    std::cout << std::get<0>(bar) << " " << std::get<1>(bar) << " " << std::get<2>(bar) << std::endl;
}

int main(){

    std::string bar_path = "./data/train/bar/FB.csv";

    CSV csv_md;
    csv_md.openFile(bar_path);
    bool is_open= csv_md.isOpen();
    std::cout << is_open << std::endl;
    csv_md.skip(1);
    bool has_data = csv_md.hasData();
    std::cout << has_data << std::endl;
    csv_md.closeFile();

    data::basic::Bar bar_reader;
    bar_reader.LoadCSV(bar_path);

    data::BarRecord bar_record_last = bar_reader.record_last;
    data::BarRecord bar_record_current = bar_reader.record_curr;
    data::BarRecord bar_record_next = bar_reader.record_next;

    std::cout << "when first load csv..................................";
    std::cout <<"record_last snapshot: \n";
    print_bar_record(bar_record_last);

    std::cout <<"record_current snapshot: \n";
    print_bar_record(bar_record_current);

    std::cout <<"record_next snapshot: \n";
    print_bar_record(bar_record_next);

    std::cout << "record_next.date: " << bar_record_next.date << std::endl;
    std::cout << "record_next.time: " << bar_record_next.time << std::endl;

    std::cout << "market_depth.NextDate(): " << bar_reader.NextDate() << std::endl;
    std::cout << "market_depth.NextTime(): " << bar_reader.NextTime() << std::endl;

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

    for (int i=0; i < 20; i++){
        std::cout << i << std::endl;
        int target_date = bar_reader.NextDate();      //获取下一条record的Date
        long target_time = bar_reader.NextTime();     //获取下一条record的Time

        // 此时的time_and_sales获得是 record_curr.date - record_curr.date 区间的逐笔成交数据
        if (not bar_reader.LoadUntil(target_date, target_time))     //一直导入数据直至next恰好<=target_time
            continue;
        std::cout << "target datetime : " << target_date << " " << target_time << std::endl;
        // 此时的time_and_sales.Record是target dateime的成交数据
        const data::BarRecord& bar_rec = bar_reader.Record();
        print_bar_record(bar_rec);

        while (true){
            if (not bar_reader.LoadNext()){
                continue;
            } else{
                break;
            }
        }

    }

    bar_reader.Reset();

    return 0;
}

