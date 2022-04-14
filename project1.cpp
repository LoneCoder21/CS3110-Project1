#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <utility>

/*
Project 1 - CS 3110
Group: Omar Suede, Abhinav Neelam
description: Recognize floating point literals using dfa's
date: 04/13/22
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

int main()
{
	string str;
	pair<bool, float> dfa;
	cout << "Enter a string (type 'q' to quit): ";
	cin >> str;

	while (str != "q") {
		dfa = computeDFA(str);
		if (dfa.first) {
			cout << "Number is: " << setprecision(7) << dfa.second << endl;
		}
		else {
			cout << "Invalid input" << endl;
		}//if error exists then we report and move on to next input
		cout << "Enter a string (type 'q' to quit): ";
		cin >> str;
	}
    
	return 0;
}