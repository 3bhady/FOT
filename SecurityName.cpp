//
// Created by mohamed on 5/24/18.
//

#include "SecurityName.h"

SecurityName::SecurityName(string _name,double _medianDailyTraded,double _monthlyMedian) {
    name = std::move(_name);
    medianDailyTraded = _medianDailyTraded;
    monthlyMedian = _monthlyMedian;
}