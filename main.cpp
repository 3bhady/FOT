//
// Created by mohamed on 5/22/18.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <map>
#include <dirent.h>
#include "csv.h"
#include "SecurityName.h"
#include "utility.h"

using namespace std;
map<string, SecurityName *> securityNamesMap;

int currMonth = 0;
string resultFile = "";
string threeMonthFile ="";

int maxDays=-1;
bool noOld=false;
bool readDirectory(const string &location, vector<string> &vec) {
/*    string s;
    s = "ls -1 ";
    s += "\"" + location + "\"";

    string str = GetStdoutFromCommand(s);
    std::stringstream ss(str);
    std::string to;

    if (!str.empty()) {
        while (std::getline(ss, to, '\n')) {
            vec.push_back(to);
        }
    }
    return !vec.empty();
    */
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (location.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            string temp(ent->d_name);
            vec.push_back(temp);
//            printf ("%s\n", ent->d_name);
        }
        closedir (dir);
        return true;
    } else {
        /* could not open directory */
        perror ("");
        return false;
    }
}

void readSecurityNameCSV(string name) {


    io::CSVReader<3> in("historical data/" + name);
    //in.set_header( "Company name", "Security name","dummy1","dummy2","dummy3", "Final NOS","Final FFR","un");
    try {
        in.read_header(io::ignore_missing_column | io::ignore_extra_column, "Date", "Close", "Volume");
        in.set_header("Date", "Close", "Volume");
    }
    catch (...) {
        cout << "file " + name + " isn't in the right format" << endl;
        return;
    }
    string date, close, volume;
    int tradedDays = 0;
    vector<double> dailyTraded;
    try {
        while (in.read_row(date, close, volume)) {

            if (volume.empty()) {
                continue;
            }
            double closing = toDouble(close);
            int volumeTraded = stoi(volume);
            if(volumeTraded!=0)
                tradedDays++;
            dailyTraded.push_back(volumeTraded * closing);

        }
        double medianDailyTraded = findMedian(dailyTraded);
        double monthlyMedian = medianDailyTraded * tradedDays;
        string month = "00";
        month[0] = (date[3]);
        month[1] = date[4];
        currMonth = stoi(month);

        securityNamesMap[name] = new SecurityName(name, medianDailyTraded, monthlyMedian, stoi(month), tradedDays);

    } catch (...) {//this is for new lines at the end of the csv file
    }
}

void readCountryCSV(string country) {
    io::CSVReader<7> in("countries on present quarter/" + country);
    //Company name;Security name;;;;Final number of shares;Final free floating rate
    try {
        in.read_header(io::ignore_missing_column | io::ignore_extra_column, "Company name", "Security name", "dummy1",
                       "dummy2", "dummy3", "Final NOS", "Final FFR");
        in.set_header("Company name", "Security name", "dummy1", "dummy2", "dummy3", "Final NOS", "Final FFR");
    } catch (...) {
        cout << "file " + country + " isn't in the right format" << endl;
//         return;
    }

    string company, securityName, shares, floatingRate, dummy;
    while (in.read_row(company, securityName, dummy, dummy, dummy, shares, floatingRate)) {
        securityName = iso_8859_1_to_utf8(securityName);
        company = iso_8859_1_to_utf8(company);
        securityName += ".csv";

//        cout<<company<<endl;
//        cout<<securityName<<endl;
//        cout<<shares<<endl;
//        cout<<floatingRate<<endl;

        double freeFloatingRate = toDouble(floatingRate);

        if (securityNamesMap.count(securityName) != 0) {
            securityNamesMap[securityName]->monthlyMedianTradedRatio =
                    securityNamesMap[securityName]->monthlyMedian / freeFloatingRate;

            resultFile +=
                    securityName + ";" + to_string(securityNamesMap[securityName]->monthlyMedianTradedRatio) +
                    ";" + to_string(securityNamesMap[securityName]->tradedDays) + "\r\n";
            securityNamesMap[securityName]->monthlyMedianTradedRatioVec.push_back(securityNamesMap[securityName]->monthlyMedianTradedRatio);
            if (securityNamesMap[securityName]->month % 3 == 0) {
                //it uses the previous monthly median traded value ratios from earlier months
                //to calculate the 12 month indicator and the 3 month indicator.
            } else {
                //the monthly median traded value ratios are saved as history for the month.

            }

        } else {
            //cout<<"security Name: "+securityName+" not found in any files!"<<endl;
//            exit(1);
        }
    }
}
void readOldFile(string name){

    try {
        io::CSVReader<3> in(name);
        in.read_header(io::ignore_missing_column | io::ignore_extra_column, "Security Name","Monthly Median Traded Ratio","Trading Days");


    string securityName,monthlyMedianTradedRatio,tradingDays;
    while (in.read_row(securityName,monthlyMedianTradedRatio,tradingDays)) {

        if(securityNamesMap.count(securityName)==0) {
            cout<<"old security name not found in this month"<<endl;
            cout<<securityName<<endl;
            continue;
        }
        securityNamesMap[securityName]->tradedDaysVec.push_back(stoi(tradingDays));
        securityNamesMap[securityName]->monthlyMedianTradedRatioVec.push_back(stod(monthlyMedianTradedRatio));
    }
    } catch (...) {
        cout << "file " + name + " doesn't exist" << endl;
        noOld=true;
    }
}
void calculateIndicator(){
    for(const auto &i : securityNamesMap) {
        double twelve=0;
        double three=0;
        double daysOfTrade=0;
        int numOfMonth=0;
        auto &medianRatioVec = i.second->monthlyMedianTradedRatioVec;
        auto &tradedDaysVec = i.second->tradedDaysVec;
        if(medianRatioVec.empty())
        {
//            cout<<""<<endl;
            continue;
        }
        for(int j=0;j<medianRatioVec.size();j++) {
            daysOfTrade+=tradedDaysVec[j];
            if(j<2) {
                three+=medianRatioVec[j];
            }
            twelve+=medianRatioVec[j];
        }
        if(medianRatioVec.size()>3)
        {
            three/=3;
        }
        else{
            three/=medianRatioVec.size();
        }
        twelve/=medianRatioVec.size();
        three*=12;
        twelve*=12;
        threeMonthFile+=i.first+";"+to_string(three)+";"+to_string(twelve)+to_string(daysOfTrade/maxDays)+"\r\n";
//        twelveMonthFile+=i.first+"\r\n";
    }
}
int main() {

    while(maxDays<0){
        cout<<"Please enter the number of business days for this quarter"<<endl;
        cin>>maxDays;
    }


    resultFile = "Security Name;Monthly Median Traded Ratio;Trading Days\r\n";
    vector<string> securityNames;
    readDirectory("historical data", securityNames);
    for (const auto &i : securityNames) {
        if (endsWith(i, ".csv"))
            readSecurityNameCSV(i);
    }

    vector<string> countries;
    readDirectory("countries on present quarter", countries);
    for (const auto &i : countries) {
        if (endsWith(i, ".csv"))
            readCountryCSV(i);
    }
    ofstream outfile;
    outfile.open(months[currMonth] + ".csv");
    outfile << resultFile;
    outfile.close();
    if(currMonth%3==0){

        for(int i=1;i<currMonth;i++) {
            readOldFile(months[i]+".csv");
        }
        if(!noOld) {
            calculateIndicator();
            threeMonthFile="Security Name;Three-Month Indicator;Twelve-Month Indicator;FOT\r\n";
            outfile.open("quarterly.csv");
            outfile << threeMonthFile;
            outfile.close();

        }
    }
    return 0;
}