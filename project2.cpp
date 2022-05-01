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
description: Evaluate expressions using PDA
date: 04/24/22
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

pair<bool, float> CA(const string& s, int left, int right);
pair<bool, float> CP(const string& s, int left, int right);
pair<bool, float> T(const string& s, int left, int right);
pair<bool, float> F(const string& s, int left, int right);
pair<bool, float> M(const string& s, int left, int right);
pair<bool, float> CM(const string& s, int left, int right);
pair<bool, float> A(const string& s, int left, int right);
pair<bool, float> CA(const string& s, int left, int right);
pair<bool, float> OP(const string& s, int left, int right);
pair<bool, float> CSP(const string& s, int left, int right);
pair<bool, float> S(const string& s, int left, int right);

bool processdigits(const string& input, int& st, float& val, int& len) {
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

//prints pair
void print(const pair<bool, float>& p) {
	cout << p.first << ' ' << p.second << endl;
}

//function to compute 4 individual DFA's to see if any of them accept the input
pair<bool, float> computeDFA(const string& input) {
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

// Function to find precedence of
// operators.

pair<bool, float> CA(const string& s, int left, int right) {
    for(int i=left; i<right;++i) {
		auto a = A(s, left, i);
		auto t = T(s,i + 1,right);
		
        if(a.first && t.first) return make_pair(true, t.second);
    }
    
    return make_pair(false,0);
}

pair<bool, float> CP(const string& s, int left, int right) {
    return make_pair(left==right && s[left]==')',0);
}

pair<bool, float> T(const string& s, int left, int right) {
    //if(left==right) return make_pair(isdigit(s[left]), s[left] - '0');

    auto cd = computeDFA(s.substr(left,right-left+1));
	if(cd.first) return cd;

	for(int i=left; i<right;++i) {
		auto op = OP(s, left, i);
		auto csp = CSP(s, i + 1, right);
		if(op.first && csp.first) return make_pair(true, csp.second);
    }

    for(int i=left; i<right;++i) {
		auto t = T(s, left, i);
		auto cm = CM(s, i + 1, right);

        if(t.first && cm.first) return make_pair(true,t.second * cm.second);
    }
    
    return make_pair(false,0);
}

pair<bool, float> F(const string& s, int left, int right) {
    //if(left==right) return make_pair(isdigit(s[left]), s[left] - '0');

    auto cd = computeDFA(s.substr(left,right-left+1));
	if(cd.first) return cd;

    for(int i=left; i<right;++i) {
		auto op = OP(s, left, i);
		auto csp =  CSP(s,i+1,right);

        if(op.first && csp.first) return make_pair(true, csp.second);
    }
    
    return make_pair(false,0);
}

pair<bool, float> M(const string& s, int left, int right) {
    return make_pair(left==right && (s[left]=='*'),0);
}

pair<bool, float> CM(const string& s, int left, int right) {
    for(int i=left; i<right;++i) {
		auto m = M(s, left, i);
		auto f = F(s, i + 1, right);
        if(m.first && f.first) return make_pair(true,f.second);
    }
    
    return make_pair(false,0);
}

pair<bool, float> A(const string& s, int left, int right) {
    return make_pair(left==right && (s[left]=='+'),0);
}

pair<bool, float> OP(const string& s, int left, int right) {
    return make_pair(left==right && s[left]=='(',0);
}

pair<bool, float> CSP(const string& s, int left, int right) {
    for(int i=left; i<right;++i) {
		auto s0 = S(s, left, i);
		auto cp = CP(s, i + 1, right);

		if (s0.first && cp.first) return make_pair(true, s0.second);
    }
    
    return make_pair(false,0);
}

pair<bool, float> S(const string& s, int left, int right) {
    //if(left==right) return make_pair(isdigit(s[left]), s[left]-'0');

	auto cd = computeDFA(s.substr(left,right-left+1));
	if(cd.first) return cd;

	for(int i=left; i<right;++i) {
		auto op = OP(s, left, i);
		auto csp =  CSP(s,i+1,right);

		if(op.first && csp.first) return make_pair(true, csp.second);
	}

	for(int i=left; i<right;++i) {
		auto t = T(s, left, i);
		auto cm = CM(s,i+1,right);

		if(t.first && cm.first) return make_pair(true, t.second * cm.second);
	}

	for(int i=left; i<right;++i) {
		auto s0 = S(s, left, i);
		auto ca = CA(s, i+1,right);
		
        if(s0.first && ca.first) return make_pair(true, s0.second + ca.second);
	}

    return make_pair(false,0);
}

pair<bool, float> isvalid(const string & s) {
    return S(s,0,s.size()-1);
}

int main()
{
	string input = "0.9*(0.9e+10)+1.0";
	auto isv = isvalid(input);
	
	cout << isv.first << ' ' << isv.second << endl;

	return 0;
}