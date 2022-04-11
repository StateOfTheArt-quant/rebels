#include "rebels/data/data_source.h"
#include <iostream>
#include <vector>
#include <algorithm>

DataSource::DataSource(){

}

DataSource::DataSource(std::string bar_path, int bar_counts) : bar_reader(),bar_path(bar_path), bar_count(bar_counts) { //, bar_count(bar_count)
    bar_reader.LoadCSV(bar_path);
    std::cout << "load csv successfully : " << bar_path << std::endl;
}

std::vector<std::tuple<double, double, double, double, double, double>>
DataSource::history_bars(std::string instrument_id, int bar_counts, int end_dt) {

    std::vector<std::tuple<double, double, double, double, double, double>> v;
    std::map<int, std::tuple<double, double, double, double, double, double>>::iterator it;
    it = consumedData.find(end_dt);

    for(int i=0; i< bar_counts; i++){
        v.push_back(it -> second);
        it--;
    }

    std::reverse(v.begin(), v.end());
    return v;
}

std::vector<std::tuple<double, double, double, double, double, double>>
DataSource::step() {
    bar_reader.LoadNext();
    data::BarRecord bar_rec = bar_reader.Record();
    consumedData[bar_rec.date] = bar_rec.bar;
    int current_dt = bar_rec.date;
    std::cout << "current dt: " << current_dt << std::endl;

    std::vector<std::tuple<double, double, double, double, double, double>> v;
    v = history_bars("fb", bar_count, current_dt);
    return v;
}


void DataSource::reset() {

    std::cout << "DataSource reset" << std::endl;
    //bar_reader.Reset();
    bar_reader.LoadCSV(bar_path);

    for (int i =0; i < bar_count; i++){
        std::cout << "i: " << i << std::endl;
        bar_reader.LoadNext();
        //bar_reader.record();
        data::BarRecord bar_rec = bar_reader.Record();
        std::cout << bar_rec.date << std::endl;
        consumedData[bar_rec.date] = bar_rec.bar;
    }
}



