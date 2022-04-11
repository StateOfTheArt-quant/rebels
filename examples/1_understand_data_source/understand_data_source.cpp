#include <rebels/data/data_source.h>
#include <map>
#include <iostream>

void print_map(std::map<int, std::tuple<double, double, double, double, double,double>> a_map){
    for(auto i = a_map.begin(); i!=a_map.end(); i++){
        std::cout <<i->first << ":" << std::get<0>(i->second) << std::endl;
    }
}

void print_vector(std::vector<std::tuple<double, double, double, double, double,double>> v){
    for(auto i = v.begin(); i!=v.end(); i++){
        std::cout << std::get<0>(*i) << " " << std::get<1>(*i) << std::endl;
    }
}


int main(){

    std::string bar_path = "./data/train/bar/FB.csv";
    DataSource data_source{bar_path, 5};
    data_source.reset();

    std::map<int, std::tuple<double, double, double, double, double,double>> consumed_data = data_source.consumedData;
    print_map(consumed_data);


    std::vector<std::tuple<double, double, double, double, double,double>> history_data = data_source.history_bars("fb", 3, 20200610);
    print_vector(history_data);

    std::vector<std::tuple<double, double, double, double, double,double>> state;
    state = data_source.step();
    print_vector(state);

    state = data_source.step();
    print_vector(state);

    return 0;
}
