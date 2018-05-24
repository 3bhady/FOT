//
// Created by mohamed on 5/24/18.
//

#ifndef FOT_SECURITY_H
#define FOT_SECURITY_H

#include <string>
#include <utility>

using namespace std;
class SecurityName {
public:
    string name;
    string company;
    double medianDailyTraded;
    double monthlyMedian;
    double monthlyMedianTradedRatio;
    explicit SecurityName(string _name,double _medianDailyTraded,double _monthlyMedian );

};


#endif //FOT_SECURITY_H
