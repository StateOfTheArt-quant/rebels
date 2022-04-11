#include "rebels/data/basic.h"
#include "rebels/utilities/time.h"

#include <iostream>
using namespace data::basic;

MarketDepth::MarketDepth():data::MarketDepth(), csv_(){

}

MarketDepth::MarketDepth(std::string file_path): MarketDepth(){
    LoadCSV(file_path);
}

void MarketDepth::LoadCSV(std::string path){

    Reset();
    std::cout << "to open file" << std::endl;
    csv_.openFile(path);
    csv_.skip(1); // Ignore header
    std::cout << "to load next" << std::endl;
    LoadNext();
    std::cout << "LoadNext ok" << std::endl;
}

bool MarketDepth::_LoadRow(){
    while (row_.size() != 22 ){
        if (not csv_.hasData()){
            return false;
        }

        csv_.next(row_);
    }

    if (row_.size() == 22){
        return true;
    } else {
        return false;
    }

}

bool MarketDepth::_ParseRow(){
    record_next.date = std::stoi(row_.at(DATE));
    record_next.time = string_to_time(row_.at(TIME));

    for ( int i = 0; i < 5; i++){
        double nap = std::stof(row_.at(AP1 + i)) / 100,
               nbp = std::stof(row_.at(BP1 + i)) / 100;

        if (nap <= 0.0 or nbp <= 0.0){
            row_.clear();

            return false;
        }
        record_next.ask_prices[i] = nap;
        record_next.ask_volumes[i] = (std::stol(row_.at(AV1 + i)));

        record_next.bid_prices[i] = nbp;
        record_next.bid_volumes[i] = (std::stol(row_.at(BV1 + i)));
    }

    row_.clear();

    return true;
}

bool MarketDepth::_LoadNext(){

    if (not csv_.isOpen()){
        return false;
    }
    while (true){
        if (not _LoadRow()){
            break;
        }
        if (_ParseRow()){
            return true;
        }
    }
    return false;
}

long MarketDepth::_TimeLookAhead(){

    if (not _LoadRow()){
        return -1;
    } else {
        return string_to_time(row_.at(TIME));
    }
}


void MarketDepth::Reset(){
    Streamer::Reset();

    csv_.closeFile();
    row_.clear();
}


void MarketDepth::SkipN(long n){
    csv_.skip(n);
}

// -------------------------------------------------------------------------------

TimeAndSales::TimeAndSales(): data::TimeAndSales(), csv_() {

}

TimeAndSales::TimeAndSales(std::string file_path): data::TimeAndSales() {
    LoadCSV(file_path);
}

void TimeAndSales::LoadCSV(std::string path) {
    Reset();

    csv_.openFile(path);
    csv_.skip(1);

    LoadNext();
}


bool TimeAndSales::_LoadRow(){

    while (row_.size() != 4) {
        if (not csv_.hasData()){
            return false;
        }
        csv_.next(row_);
    }

    if (row_.size() == 4){
        return true;
    } else {
        return false;
    }

}


bool TimeAndSales::_ParseRow(){

    record_next.date = std::stoi(row_.at(DATE));
    record_next.time = string_to_time(row_.at(TIME));

    double price = std::stof(row_.at(PRICE)) /100;
    long size = std::stol(row_.at(SIZE));

    if (price > 0.0 and size > 0 ){
        record_next.transactions[price] += size;
    }
    row_.clear();

    return true;
}


bool TimeAndSales::_LoadNext() {
    if (not csv_.isOpen() or not _LoadRow()){
        return false;
    }

    //parse the row_
    long la = 0;
    long target  = _TimeLookAhead();
    do {
        if (la == -1 or (not _ParseRow())){
            return false;
        }

        la = _TimeLookAhead();
    } while (la <= target);

    return true;
}

long TimeAndSales::_TimeLookAhead(){
    if (not _LoadRow()){
        return -1;
    } else {
        return string_to_time(row_.at(TIME));
    }
}

void TimeAndSales::Reset(){
    Streamer::Reset();

    csv_.closeFile();
    row_.clear();
}

void TimeAndSales::SkipN(long n) {
    csv_.skip(n);
}


// --------------------------------------------------------------------------
Bar::Bar(): data::Bar(), csv_() {

}

Bar::Bar(std::string file_path): data::Bar() {
    LoadCSV(file_path);
}

void Bar::LoadCSV(std::string path) {
    Reset();

    csv_.openFile(path);
    csv_.skip(1);

    LoadNext();
}

bool Bar::_LoadRow(){

    while (row_.size() != 7) {
        if (not csv_.hasData()){
            return false;
        }
        csv_.next(row_);
    }

    if (row_.size() == 7){
        return true;
    } else {
        return false;
    }

}



bool Bar::_ParseRow(){

    std::string date_with_dash = row_.at(Date);
    remove_dash_in_date(date_with_dash);
    record_next.date = std::stoi(date_with_dash);
    record_next.time = std::stoi(date_with_dash);

    double open = std::stof(row_.at(Open));
    double high = std::stof(row_.at(High));
    double low = std::stof(row_.at(Low));
    double close = std::stof(row_.at(Close));
    double adj_close = std::stof(row_.at(Adj_close));
    double volume = std::stof(row_.at(Volume));

    if (close > 0.0){
        record_next.bar = std::make_tuple(open, high, low, close, adj_close, volume);
    }
    row_.clear();

    return true;
}

bool Bar::_LoadNext(){

    if (not csv_.isOpen()){
        return false;
    }
    while (true){
        if (not _LoadRow()){
            break;
        }
        if (_ParseRow()){
            return true;
        }
    }
    return false;
}

long Bar::_TimeLookAhead(){
    if (not _LoadRow()){
        return -1;
    } else {
        std::string date_with_dash = row_.at(Date);
        remove_dash_in_date(date_with_dash);
        return std::stoi(date_with_dash);
    }
}

void Bar::Reset(){
    Streamer::Reset();

    csv_.closeFile();
    row_.clear();
}

void Bar::SkipN(long n) {
    csv_.skip(n);
}
