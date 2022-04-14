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

const int DEBUG_PRINT = 1;
const int DETAIL_DEBUG_PRINT = 0;

using namespace std;

//function to process digits separately
bool processdigits(const string& input, int& st, float& val, int& len) {
	bool accept = false;
	int state = 1;

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

pair<bool, float> mdfa1(const string& input)
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

				dead = !processdigits(input, i, value, len);
				len = 0;
			}
			else if (state == 2)
			{
				state = 2;
				accept = true;

				dead = !processdigits(input, i, frac, len);
				frac = frac / pow(10, len);
			}
			else if (state == 3 || state == 4 || state == 5)
			{
				state = 5;
				accept = true;
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

pair<bool, float> mdfa2(const string& input)
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

				dead = !processdigits(input, i, value, len);
				value = value / pow(10, len);
			}
			else if (state == 4 || state == 5) {
				accept = true;
				state = 5;

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

pair<bool, float> mdfa3(const string& input)
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

				dead = !processdigits(input, i, value, len);
			}
			else if (state == 4 || state == 3) {
				state = 4;

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

pair<bool, float> mdfa4(const string& input)
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
				dead = !processdigits(input, i, value, len);
			}
			else if (state == 4 || state == 3) {
				state = 4;
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

/*{
	1. User input
	2. Main loop 
	5. Add comments
	6. Explain how doing 4 DFA is same as one DFA
	8. Write READ me
	9. Create text file to submit for github link
}*/

void print(const pair<bool, float>& p) {
	cout << p.first << ' ' << p.second << endl;
}

pair<bool, float> computeDFA(const string& input) {
	pair<bool, float> p[4] = { mdfa1(input), mdfa2(input), mdfa3(input), mdfa4(input) };

	bool accept = false;
	float v = 0.0f;

	for (int i = 0; i < 4; ++i) {
		if (DETAIL_DEBUG_PRINT) {
			cout << "DFA " << i + 1 << ": ";
			print(p[i]);
		}

		if (p[i].first) {
			accept = p[i].first;
			v = p[i].second;
		}
	}

	return make_pair(accept, v);
}

void test_cases() {
	if (!DEBUG_PRINT) return;

	vector<string> test_nums = { "", "123", "123f", "123e", "123e1", "123e1f", "+123f", "-123f",
				"123.", ".", "123..2", "123.2.e1", "_", "_1__2.", "1__2.", "1__2_.",
				"123._2", "123.2_e1", "123.2_e_1", "123.3e1" };

	for (int i = 0; i < test_nums.size(); ++i) {
		cout << "For string " << test_nums[i] << " ";
		auto cd = computeDFA(test_nums[i]);

		cout << "Final result: ";
		print(cd);
	}
}

int main()
{
	test_cases();

    

	return 0;
}
