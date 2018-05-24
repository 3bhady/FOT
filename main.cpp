//
// Created by mohamed on 5/23/18.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include "csv.h"
#include "SecurityName.h"
#include "utility.h"
using namespace std;
map<string,SecurityName*>securityNamesMap;


bool readDirectory(const string &location,vector<string>& vec)
{    string s;
    s = "ls -1 ";
    s += "\""+location+"\"";

    string str = GetStdoutFromCommand(s);
    std::stringstream ss(str);
    std::string to;

    if (!str.empty())
    {
        while(std::getline(ss,to,'\n')){
            vec.push_back(to);
        }
    }
return !vec.empty();
}

void readSecurityNameCSV(string name){


    io::CSVReader<3> in("historical data/"+name);
    //in.set_header( "Company name", "Security name","dummy1","dummy2","dummy3", "Final NOS","Final FFR","un");
    try {
        in.read_header(io::ignore_missing_column | io::ignore_extra_column, "Date", "Close", "Volume");
        in.set_header("Date", "Close", "Volume");
    }
    catch(...) {
        cout<<"file " + name+ " isn't in the right format"<<endl;
        return;
    }
    string date,close,volume;
    int tradedDays=0;
    vector<double>dailyTraded;
        try{
    while(in.read_row(date,close,volume)){

        if(volume.empty()) {
            continue;
        }
        double closing=toDouble(close);
        int volumeTraded = stoi(volume);
        dailyTraded.push_back(volumeTraded*closing);
        tradedDays++;
    }
    double medianDailyTraded= findMedian(dailyTraded);
    double monthlyMedian = medianDailyTraded*tradedDays;
    securityNamesMap[name]=new SecurityName(name,medianDailyTraded,monthlyMedian);
            cout<<monthlyMedian<<endl;
    } catch(...){//this is for new lines at the end of the csv file
             }
}

void readCountryCSV(string country){
    io::CSVReader<7> in("countries on present quarter/"+country);
    //Company name;Security name;;;;Final number of shares;Final free floating rate
    //try{
    in.read_header(io::ignore_missing_column|io::ignore_extra_column, "Company name", "Security name","dummy1","dummy2","dummy3", "Final NOS","Final FFR");
    in.set_header("Company name","Security name","dummy1","dummy2","dummy3" , "Final NOS","Final FFR");

    string company,securityName,shares,floatingRate,dummy;

    while(in.read_row(company,securityName,dummy,dummy,dummy,shares,floatingRate)){
        securityName = iso_8859_1_to_utf8(securityName);
        company = iso_8859_1_to_utf8(company);
        securityName+=".csv";
//        cout<<company<<endl;
//        cout<<securityName<<endl;
//        cout<<shares<<endl;
//        cout<<floatingRate<<endl;

        double freeFloatingRate=toDouble(floatingRate);

        if(securityNamesMap.count(securityName)!=0) {
            securityNamesMap[securityName]->monthlyMedianTradedRatio =
                     securityNamesMap[securityName]->monthlyMedian/freeFloatingRate;

        }
        else {
            //cout<<"security Name: "+securityName+" not found in any files!"<<endl;
//            exit(1);
        }
    }
   // }
   // catch(...) {
    //    cout<<"file " + country+ " isn't in the right format"<<endl;
   //     return;

    //}
    }

int main()
{
    /*
    vector<string> countries;
    readDirectory("countries on present quarter",countries);
    for (const auto &i : countries) {

        io::CSVReader<4> in("countries on present quarter/"+i);
        //in.set_header( "Company name", "Security name","dummy1","dummy2","dummy3", "Final NOS","Final FFR","un");
        in.read_header(io::ignore_extra_column, "Company name", "Security name", "Final NOS","Final FFR");
        string companyName,SecurityName,finalNOS,finalFFR,dummy,val;
        while(in.read_row(companyName,SecurityName,finalNOS,finalFFR)){
            cout<<companyName<<SecurityName<<finalNOS<<finalFFR<<dummy<<endl;

        }
    }*/
//
    vector<string>securityNames;
    readDirectory("historical data",securityNames);
    for (const auto &i : securityNames) {
        if(endsWith(i,".csv"))
        readSecurityNameCSV(i);
    }
    vector<string> countries;
    readDirectory("countries on present quarter",countries);
    for (const auto &i : countries) {
        if(endsWith(i,".csv"))
        readCountryCSV(i);
    }
    return 0;
}



