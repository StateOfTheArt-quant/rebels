#pragma once
#include <string>


struct Reward {
    std::string measure_method = "pnl_damped";
    double damping_factor = 0.15;
    int pnl_lookback = 0;
    double pos_weight = 0.0;
    double trd_weight = 0.0;
    double pnl_weight = 1.0;
};

struct Data{
    std::string symbol = "000661.SZ";

    std::string train_md_path = "./data/train/md/000661.SZ/md_20200525.csv";
    std::string train_tas_path = "./data/train/tas/000661.SZ/td_20200525.csv";
};

struct Market {
    double TRANSACTION_FEE = 0.0007;
    int pos_ub = 600;
    int pos_lb = -600;
    int order_size = 100;
};

struct Lookback_window {
    int mpm = 15;
    int vlt = 5;
    int abv_diff = 10;
};

struct State {
    std::vector<std::string> variables = {"pos","spd", "abv_diff"};
    Lookback_window lookback_window;
};


struct Config{
    Reward reward;
    Data data;
    Market market;
    State state;
 };