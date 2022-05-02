#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <utility>
#include <stack>

/*
Project 2 - CS 3110
Group: Omar Suede, Abhinav Neelam
Description: Evaluate expressions using CFG and PDA
Date: 05/02/22
*/

using namespace std;

/*
	Function to process digits separately	
		- Handles underscores(_)
	Parameters
	1. input - input string to parse
	2. st - index of starting point
	3. val - value to store 
	4. len - returns length of digits (used for calculating fractional component)
*/

pair<bool, float> S(const string& str, int left, int right);
pair<bool, float> T(const string& str, int left, int right);
pair<bool, float> F(const string& str, int left, int right);

void print(const pair<bool, float>& p) //prints pair
{
	cout << p.first << ' ' << p.second << endl;
}

string trim(const string& str) //to trim whitespaces at front and end of string
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool processdigits(const string& input, int& st, float& val, int& len) //to process digits in DFA
{
	bool accept = false;//boolean to choose accept states
	int state = 1;//varaiable to keep track of what state we are in

	for (; st < input.size(); ++st) {
		switch (input[st]) {
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			len++;
			if (state == 1) {
				state = 2;
				accept = true;
			}
			else if (state == 2) {
				state = 4;
				accept = true;
			}
			else if (state == 3 || state == 4) {
				state = 4;
				accept = true;
			}
			else {
				return false;
			}
			val *= 10;
			val += input[st] - '0';
			break;
		case '_':
			if (state == 2) {
				state = 3;
			}
			else if (state == 3) {
				state = 3;
			}
			else if (state == 4) {
				state = 3;
			}
			else {
				return false;
			}
			break;
		default:
			st--;
			if (state == 2 || state == 4) {
				return true;
			}
			else {
				return false;
			}
			break;
		}
	}

	return accept;
}

/*
	DFA 1 - Digits . [Digits] [ExponentPart] [FloatTypeSuffix]
*/

pair<bool, float> dfa1(const string& input)
{
	int state = 0;
	bool accept = false, dead = false;
	int sign = 1;
	float value = 0.0f;
	float exp = 0;
	float frac = 0.0f;
	int len = 0;

	for (int i = 0; i < input.size() && !dead; ++i) {
		switch (input[i]) {
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			if (state == 0 || state == 1)
			{
				state = 1;
				
				//calculate whole value
				dead = !processdigits(input, i, value, len);
				len = 0;
			}
			else if (state == 2)
			{
				state = 2;
				accept = true;

				//calculate fractional value
				dead = !processdigits(input, i, frac, len);
				frac = frac / pow(10, len);
			}
			else if (state == 3 || state == 4 || state == 5)
			{
				state = 5;
				accept = true;
				//calculate exponent
				dead = !processdigits(input, i, exp, len);
			}
			else {
				dead = true;
			}
			break;
		case '.':
			if (state == 1)
			{
				state = 2;
				accept = true;
			}
			else {
				dead = true;
			}
			break;
		case 'e':case 'E':
			if (state == 2)
			{
				state = 3;
			}
			else {
				dead = true;
			}
			break;
		case '+':
		case '-':
			if (state == 3) {
				state = 4;
				sign = (input[i] == '-') ? -1 : 1;
			}
			else {
				dead = true;
			}
			break;
		case 'f':case 'F':case 'd':case 'D':
			if (state == 2 || state == 5) {
				state = 6;
				accept = true;
			}
			else {
				dead = true;
			}
			break;
		case '_':
			dead = true;
			break;
		default:
			dead = true;
			break;
		}
	}

	value += frac;
	value *= pow(10, sign * exp);
	accept &= !dead;

	return make_pair(accept, value);
}

/*
	DFA 2 - . Digits [ExponentPart] [FloatTypeSuffix] 
*/

pair<bool, float> dfa2(const string& input)
{
	int state = 0;
	bool accept = false, dead = false;
	int sign = 1;
	float value = 0.0f;
	float exp = 0;

	int len = 0;

	for (int i = 0; i < input.size() && !dead; ++i) {
		switch (input[i]) {
		case '.':
			if (state == 0)
			{
				state = 1;
			}
			else {
				dead = true;
			}
			break;
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			if (state == 1 || state == 2)
			{
				accept = true;
				state = 2;

				//calculate fractional value
				dead = !processdigits(input, i, value, len);
				value = value / pow(10, len);
			}
			else if (state == 4 || state == 5) {
				accept = true;
				state = 5;

				//calculate exponent
				dead = !processdigits(input, i, exp, len);
			}
			else {
				dead = true;
			}
			break;
		case 'e': case 'E':
			if (state == 2)
			{
				state = 4;
			}
			else {
				dead = true;
			}
			break;
		case '+': case '-':
			if (state == 4) {
				sign = (input[i] == '-') ? -1 : 1;
				state = 5;
			}
			else {
				dead = true;
			}
			break;
		case 'f': case 'F': case 'd': case 'D':
			if (state == 5 || state == 2) {
				state = 6;
				accept = true;
			}
			else {
				dead = true;
			}
			break;
		case '_':
			dead = true;
			break;
		default:
			dead = true;
			break;
		}
	}

	value *= pow(10, sign * exp);
	accept &= !dead;

	return make_pair(accept, value);
}

/*
	DFA 3 - Digits ExponentPart [FloatTypeSuffix] 
*/

pair<bool, float> dfa3(const string& input)
{
	int state = 1;
	bool accept = false, dead = false;
	int sign = 1;
	float value = 0.0f;
	float exp = 0;
	int len = 0;

	for (int i = 0; i < input.size() && !dead; ++i) {
		switch (input[i]) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			if (state == 1 || state == 2)
			{
				state = 2;

				//calculate whole value
				dead = !processdigits(input, i, value, len);
			}
			else if (state == 4 || state == 3) {
				state = 4;

				//calculate exponent
				dead = !processdigits(input, i, exp, len);
				accept = true;
			}
			else {
				dead = true;
			}
			break;
		case 'e': case 'E':
			if (state == 2)
			{
				state = 3;
			}
			else {
				dead = true;
			}
			break;
		case '+': case '-':
			if (state == 3) {
				sign = (input[i] == '-') ? -1 : 1;
				state = 4;
			}
			else {
				dead = true;
			}
			break;
		case 'f': case 'F': case 'd': case 'D':
			if (state == 4) {
				state = 5;
				accept = true;
			}
			else {
				dead = true;
			}
			break;
		case '_':
			dead = true;
			break;
		default:
			dead = true;
			break;
		}
	}

	value *= pow(10, sign * exp);

	accept &= !dead;
	return make_pair(accept, value);
}

/*
	DFA 4 - Digits [ExponentPart] FloatTypeSuffix
*/

pair<bool, float> dfa4(const string& input)
{
	int state = 1;
	bool dead = false;
	bool accept = false;
	int sign = 1;
	float value = 0.0f;
	float exp = 0;
	int len = 0;

	for (int i = 0; i < input.size() && !dead; ++i) {
		switch (input[i]) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			if (state == 1 || state == 2)
			{
				state = 2;
				//calculate whole value
				dead = !processdigits(input, i, value, len);
			}
			else if (state == 4 || state == 3) {
				state = 4;
				//calculate exponent
				dead = !processdigits(input, i, exp, len);
			}
			else {
				dead = true;
			}
			break;
		case 'e': case 'E':
			if (state == 2)
			{
				state = 3;
			}
			else {
				dead = true;
			}
			break;
		case '+': case '-':
			if (state == 3) {
				sign = (input[i] == '-') ? -1 : 1;
				state = 4;
			}
			else {
				dead = true;
			}
			break;
		case 'f': case 'F': case 'd': case 'D':
			if (state != 4 && state != 2) {
				dead = true;
			}
			if (state == 2 || state == 4) {
				state = 5;
				accept = true;
			}
			break;
		case '_':
			dead = true;
			break;
		default:
			dead = true;
			break;
		}
	}

	value *= pow(10, sign * exp);

	accept &= !dead;
	return make_pair(accept, value);
}

//function to compute 4 individual DFA's to see if any of them accept the input
pair<bool, float> computeDFA(const string& s) {
	string input = trim(s);

	pair<bool, float> p[4] = { dfa1(input), dfa2(input), dfa3(input), dfa4(input) };

	bool accept = false;
	float v = 0.0f;

	for (int i = 0; i < 4; ++i) {
		if (p[i].first) {
			accept = p[i].first;
			v = p[i].second;
		}
	}

	return make_pair(accept, v);
}

//computes parenthesized expressions or evaluates expression as floating point
pair<bool, float> F(const string& str, int left, int right) {
    string s = trim(str.substr(left,right-left+1));
    auto cd = computeDFA(s);
	if(cd.first) return cd;
	//evaluate expression if string is floating point value

	if(s[0]=='(' && s.back()==')') { //see whether string is surrounded by parenthesis
		auto csp = S(s,1,s.size()-2);
		
        if(csp.first) return make_pair(true, csp.second);
	}

    //failure case
    return make_pair(false,0);
}

//computes multiplication and division expressions or evaluates expression as F
pair<bool, float> T(const string& str, int left, int right) {
    auto f = F(str,left,right);
	if(f.first) return f;
	//evaluate expression if string is floating point value

	for (int i = left; i < right; ++i) {
		if (str[i] == '*') { //compute multiplication if both t and f are valid expressions
			auto t = T(str, left, i-1);
			auto f = F(str, i + 1, right);
			
			if(t.first && f.first) return make_pair(true,t.second*f.second);
			
		}else if(str[i]=='/') { //compute division if both t and f are valid expressions
		    auto t = T(str, left, i-1);
			auto f = F(str, i + 1, right);
			
			float tdf = t.second/f.second;
			
			if(t.first && f.first) return make_pair(isnormal(tdf),tdf);
			//isnormal <cmath> function checks whether floating point value is a good value
			//necessary to handle division by zero, overflows, etc...
		}
	}
	
	//failure case
	return make_pair(false,0);
}

//computes addition and subtraction expressions or evaluates expression as T
pair<bool, float> S(const string& str, int left, int right) {
	auto t = T(str,left,right);
	if(t.first) return t;
	//evaluate expression if string is floating point value

	for (int i = left; i < right; ++i) {
		if (str[i] == '+') { //compute addition if both s and t are valid expressions
			auto s = S(str, left, i-1);
			auto t = T(str, i + 1, right);
			
			if(s.first && t.first) return make_pair(true, s.second + t.second);
		}else if(str[i]=='-') { //compute subtraction if both s and t are valid expressions
		    auto s = S(str, left, i-1);
			auto t = T(str, i + 1, right);
			
			if(s.first && t.first) return make_pair(true, s.second - t.second);
		}
	}
	
	//failure case
	return make_pair(false,0);
}

//tries to compute good expression into single value and fails otherwise
pair<bool, float> computeExpression(const string & s) {
    return S(s,0,s.size()-1);
}

int main() {
    string str;
	cout << "Enter a string (type 'q' to quit): ";
	getline(cin,str);

	while (str != "q") {
		pair<bool, float> expr = computeExpression(str);
		if (expr.first) {
			cout << "Number is: " << setprecision(7) << expr.second << endl;
		}else {
			cout << "Invalid input" << endl;
		}//if error exists then we report and move on to next input
		cout << "Enter a string (type 'q' to quit): ";
		getline(cin,str);
	}
    
	return 0;
}