//
// Created by mohamed on 5/24/18.
//

#ifndef FOT_UTILITY_H
#define FOT_UTILITY_H

using namespace std;
string months[13] = {"0","January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
//string GetStdoutFromCommand(string cmd) {
//
//    string data;
//    FILE * stream;
//    const int max_buffer = 256;
//    char buffer[max_buffer];
//    cmd.append(" 2>&1");
//
//    stream = popen(cmd.c_str(), "r");
//    if (stream != nullptr) {
//        while (feof(stream) == 0)
//            if (fgets(buffer, max_buffer, stream) != nullptr) data.append(buffer);
//        pclose(stream);
//    }
//    return data;
//}
// Function for calculating median
double findMedian(vector<double> a)
{
    int n=a.size();
    // First we sort the array
    sort(a.begin(),a.end());

    // check for even case
    if (n % 2 != 0)
        return (double)a[n/2];

    return (a[(n-1)/2] + a[n/2])/2.0;
}
bool endsWith(const std::string &mainStr, const std::string &toMatch)
{
    return mainStr.size() >= toMatch.size() &&
           mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0;
}

double toDouble(string str)
{
    unsigned long i= str.find_first_of(',');
    if(i!=string::npos) {
        str[i]='.';
    }
    return stod(str);
}

std::string iso_8859_1_to_utf8(std::string &str)
{
    string strOut;
    for (std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
        uint8_t ch = *it;
        if (ch < 0x80) {
            strOut.push_back(ch);
        }
        else {
            strOut.push_back(0xc0 | ch >> 6);
            strOut.push_back(0x80 | (ch & 0x3f));
        }
    }
    return strOut;
}
#endif //FOT_UTILITY_H
