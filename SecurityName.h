//
// Created by mohamed on 5/24/18.
//

#ifndef FOT_SECURITY_H
#define FOT_SECURITY_H

#include <string>
#include <utility>
#include <vector>

using namespace std;
class SecurityName {
public:
    string name;
    string company;
    double medianDailyTraded;
    double monthlyMedian;
    double monthlyMedianTradedRatio;
    int month;
    int tradedDays;
    vector<double> monthlyMedianTradedRatioVec;
    vector<int>tradedDaysVec;
    explicit SecurityName(string _name,double _medianDailyTraded,double _monthlyMedian ,int _month,int _tradedDays);

};


#endif //FOT_SECURITY_H
