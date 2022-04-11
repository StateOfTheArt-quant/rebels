#include "rebels/data/basic.h"
#include <unordered_map>
#include <map>
#include <vector>

class AbstractDataSource {

    virtual std::vector<std::tuple<double, double, double, double, double, double>> history_bars(std::string instrument_id, int bar_count, int end_dt) = 0;

};


class DataSource : public AbstractDataSource{

public:
    std::string bar_path;
    data::basic::Bar  bar_reader;
    int bar_count;


    std::map<int, std::tuple<double, double, double, double, double,double>> consumedData;
    std::vector<std::tuple<double, double, double, double, double, double>> history_bars(std::string instrument_id, int bar_count, int end_dt);

public:
    DataSource();
    DataSource(std::string data_Path, int bar_count);
    std::vector<std::tuple<double, double, double, double, double, double>> step();
    void reset();


};