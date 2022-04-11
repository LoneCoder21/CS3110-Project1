#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iterator>

/*
Project 1 - CS 3110
Group: Omar Suede, Abhinav Neelam
Description: Recognize floating point literals
Date: 04/13/22
*/

using namespace std;

bool error=0;
//global error flag for whether string is valid floating point or not
//gets reset to 0 after every string input

//check if character is a sign
bool checkSign(char c) {
    return c == '-' || c == '+';
}

//convert number to literal (without signs/exponent/etc)
double convertToDouble(string str) {
    double d=0.0;

    for(int i=0;i<str.size();++i) {
        if(str[i] > '9' || str[i] < '0') {
            error=1;
            break;
        }//mark error flag if character is non numeric

        d*=10;
        d+=str[i]-'0';
    }

    return d;
}

//increases t by 1 if s is a sign and returns sign value
int processSign(char s, int & t) {
    if(checkSign(s)) {
        t++;
    }
    return (s == '-') ? -1 : 1;
}

//function to parse whole floating point literal
double parseDoubleExp(string str)
{
    double ans = 0;
    double exponent = 1;

    str.erase(remove(str.begin(), str.end(), '_'), str.end());//removes underscores
    str.erase(remove_if(str.begin(), str.end(), [](char c){return toupper(c)=='F' || toupper(c)=='D';}), str.end());//removes floating point suffix at end

    auto e = find_if(str.begin(),str.end(),[](char c){return toupper(c)=='E';});//finds exponents

    if(e != str.end()) {
        int dist = distance(str.begin(), e);
        int ti = dist + 1;

        int sign = processSign(str[ti],ti);
        double exp = convertToDouble(str.substr(ti));
        
        exponent = pow(10, sign * exp);
        
        str = str.substr(0,dist);
    }//if e exists in string then we process the exponent value

    int ti = 0;
    int sign = processSign(str[ti],ti);
    int dot = str.find('.');

    double leftpart=0, rightpart=0;

    //dot is in string
    if(dot != string::npos) { 
        rightpart = convertToDouble(str.substr(dot+1)) / pow(10,str.size()-dot-1);
        leftpart = convertToDouble(str.substr(ti, dot - ti));
    }else{ //dot doesn't exist
        leftpart = convertToDouble(str.substr(ti));
    }//computes whole number and decimal value

    ans += leftpart + rightpart;
    ans *= exponent * sign;

    return ans;
}

int main()
{
    string str;
    
    cout << "Enter a string (type 'q' to quit): ";
    cin >> str;

    while(str != "q") {
        double v = parseDoubleExp(str);

        if(!error) {
            cout << "Number is: " <<setprecision(15) << v << endl;
        }else{
            cout << "Invalid input" << endl;
        }//if error exists then we report and move on to next input

        cout << "Enter a string (type 'q' to quit): ";
        cin >> str;
        error=0;
    }

    return 0;
}