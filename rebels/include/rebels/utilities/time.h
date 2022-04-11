#pragma once

#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>

static long time2sec(int time) {
    //hour*3600+min*60+sec
    time = time / 1000;
    return ((int)(time / 3600) * 10000 + (int)((time % 3600) / 60) * 100 + (time % 60));
}

static long add_hours(long update, long time)
{
    return time + update*3600000;
}

static long add_minutes(long update, long time)
{
    return time + update*60000;
}

static long add_seconds(long update, long time)
{
    return time + update*1000;
}

static long add_millis(long update, long time)
{
    return time + update;
}

static long string_to_time(std::string s)
{
    int hour,min,sec,mil;
    if(s.length()==8) {
        hour = stoi(s.substr(0, 1));
        min = stoi(s.substr(1, 2));
        sec = stoi(s.substr(3, 2));
        mil = stoi(s.substr(5, 3));
    }
    else{
        hour = stoi(s.substr(0, 2));
        min = stoi(s.substr(2, 2));
        sec = stoi(s.substr(4, 2));
        mil = stoi(s.substr(6, 3));
    }

    return add_hours(hour,
                     add_minutes(min,
                                 add_seconds(sec,
                                             add_millis(mil, 0))));
}

static std::string time_to_string(long t)
{
    int mil, sec, min;

    mil = t % 1000;
    t /= 1000;

    sec = t % 60;
    t /= 60;

    min = t % 60;
    t /= 60;

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2);
    ss << t << ":";
    ss << std::setfill('0') << std::setw(2);
    ss << min << ":";
    ss << std::setfill('0') << std::setw(2);
    ss << sec << ".";
    ss << mil;

    std::string s;
    ss >> s;

    return s;
}

bool IsParenthesesOrDash(char c)
{
    switch(c)
    {
        case '(':
        case ')':
        case '-':
            return true;
        default:
            return false;
    }
}

void remove_dash_in_date(std::string& date){
    date.erase(std::remove_if(date.begin(), date.end(), &IsParenthesesOrDash), date.end());
}