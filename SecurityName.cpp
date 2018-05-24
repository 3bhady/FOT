//
// Created by mohamed on 5/22/18.
//

#include "SecurityName.h"

SecurityName::SecurityName(string _name,double _medianDailyTraded,double _monthlyMedian,int _month,int _tradedDays) {
    name = std::move(_name);
    medianDailyTraded = _medianDailyTraded;
    monthlyMedian = _monthlyMedian;
    month=_month;
    tradedDays=_tradedDays;
    tradedDaysVec.push_back(tradedDays);
}