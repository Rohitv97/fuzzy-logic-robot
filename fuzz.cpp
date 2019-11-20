// fuzz.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include<vector>
#include<string>
using namespace std;

struct Member
{
	vector<pair<int, int>> linePoint;
	string memberName;
};

struct FuzzyClass
{
	vector<struct Member> allSets;
};

struct RuleTable
{
	vector<vector<string>> allRules;
};

vector<pair<string, double>> findMemVal(int x, FuzzyClass fuzz)
{
	vector<pair<string, double>> mems;
	for (unsigned int i = 0; i < fuzz.allSets.size(); i++)
	{
		int point1x = 0, point1y = 0;
		int point2x = 0, point2y = 0;
		int flag1 = 0, flag2 = 0, flag0 = 0;
		double memval;
		for (unsigned int j = 0; j < fuzz.allSets[i].linePoint.size(); j++)
		{
			if (x == fuzz.allSets[i].linePoint[j].first)
			{
				point1x = fuzz.allSets[i].linePoint[j].first;
				point1y = fuzz.allSets[i].linePoint[j].second;
				//cout << "Point is.....     " << point1x << "," << point1y << endl;
				flag0 = 1;
				break;
			}

			if (x > fuzz.allSets[i].linePoint[j].first)
			{
				point1x = fuzz.allSets[i].linePoint[j].first;
				point1y = fuzz.allSets[i].linePoint[j].second;
				//cout << "finding first point.....    " << point1x << "," << point1y << endl;
				flag1 = 1;
			}
			else
			{
				point2x = fuzz.allSets[i].linePoint[j].first;
				point2y = fuzz.allSets[i].linePoint[j].second;
				//cout << "finding second point.....    " << point2x << "," << point2y << endl;
				flag2 = 1;
				break;

			}
		}

		if (flag0 == 1)
		{
			memval = point1y;
			cout << fuzz.allSets[i].memberName << " Membership value: " << memval << endl;
			mems.push_back({ fuzz.allSets[i].memberName, memval });
		}

		else if (flag1 == 0 && flag2 == 1)
		{
			cout << fuzz.allSets[i].memberName << " Membership value: " << 0 << endl;
		}

		else if (flag1 == 1 && flag2 == 0)
		{
			cout << fuzz.allSets[i].memberName << " Membership value: " << 0 << endl;
		}
		else if (point2y > point1y)
		{
			memval = (double)(x - point1x) / (point2x - point1x);
			cout << fuzz.allSets[i].memberName << " Membership value: " << memval << endl;
			mems.push_back({ fuzz.allSets[i].memberName, memval });
		}
		else if (point2y < point1y)
		{
			memval = (double)(point2x - x) / (point2x - point1x);
			cout << fuzz.allSets[i].memberName << " Membership value: " << memval << endl;
			mems.push_back({ fuzz.allSets[i].memberName, memval });
		}
		else if (point1y == point2y)
		{
			memval = point1y;
			cout << fuzz.allSets[i].memberName << " Membership value: " << memval << endl;
			mems.push_back({ fuzz.allSets[i].memberName, memval });
		}
	}

	return mems;

}

vector<pair<string, double>> inter_y_val(RuleTable rule, vector<pair<string, double>> y1temp, vector<pair<string, double>> y2temp)
{
	double val;
	vector<pair<string, double>> out_pos;
	for (unsigned int i = 0; i < rule.allRules.size(); i++)
	{
		for (int j = 0; j < y1temp.size(); j++)
		{
			for (int k = 0; k < y2temp.size(); k++)
			{
				if ((y1temp[j].first == rule.allRules[i][0]) && (y2temp[k].first == rule.allRules[i][1]))
				{
					if (y1temp[j].second > y2temp[k].second)
					{
						val = y2temp[k].second;
					}
					else
					{
						val = y1temp[j].second;
					}
					out_pos.push_back({ rule.allRules[i][2], val });


				}
			}
		}
	}
	return out_pos;
}

double defuzz_y_val(vector<pair<string, double>> out_pos, FuzzyClass defuzz)
{
	double avg;
	double intermediate_result = 0;
	double result;
	double sum_of_mf = 0;
	for (unsigned int i = 0; i < defuzz.allSets.size(); i++)
	{
		for (unsigned int j = 0; j < out_pos.size(); j++)
		{
			if (defuzz.allSets[i].memberName == out_pos[j].first)
			{
				int n = defuzz.allSets[i].linePoint.size();
				avg = (defuzz.allSets[i].linePoint[0].first + defuzz.allSets[i].linePoint[n-1].first) / 2;
				intermediate_result += avg * out_pos[j].second;
				sum_of_mf += out_pos[j].second;
			}
		}
	}
	result = intermediate_result/sum_of_mf;
	return result;
}

int main()
{
	int x1, x2;
	vector<pair<int, int>> finalLines;
	Member m1;
	m1.memberName = "L";
	m1.linePoint = { { 0,1 },{ 10,1 },{ 20,0 } };
	Member m2;
	m2.memberName = "M";
	m2.linePoint = { { 10,0 },{ 20,1 },{ 30,1 },{ 40,0 } };
	Member m3;
	m3.memberName = "H";
	m3.linePoint = { { 30,0 },{ 45,1 },{ 60,1 } };

	FuzzyClass fuzz;
	fuzz.allSets.push_back(m1);
	fuzz.allSets.push_back(m2);
	fuzz.allSets.push_back(m3);

	Member o1;
	o1.memberName = "L";
	o1.linePoint = { {0,0}, {10,1}, {20, 0} };
	Member o2;
	o2.memberName = "M";
	o2.linePoint = { {20,0}, {50,1}, {80, 0} };
	Member o3;
	o3.memberName = "H";
	o3.linePoint = { {80,0}, {90,1}, {100, 0} };

	FuzzyClass defuzz;
	defuzz.allSets.push_back(o1);
	defuzz.allSets.push_back(o2);
	defuzz.allSets.push_back(o3);


	RuleTable rule;
	rule = { {{"L","L","L"}, {"L","M","L"}, { "L","H","M" } ,{ "M","L","L" } ,{ "M","M","M" } ,{ "M","H","H" } ,{ "H","L","M" } ,{ "H","M","H" } ,{ "H","H","H" } } };

	vector<pair<string, double>> y1temp, y2temp, out_pos;

	cout << "Enter x1 value: ";
	cin >> x1;

	y1temp = findMemVal(x1, fuzz);

	cout << "Enter x2 value: ";
	cin >> x2;

	y2temp = findMemVal(x2, fuzz);

	out_pos = inter_y_val(rule, y1temp, y2temp);

	
	cout << out_pos.size() << endl;
	for (int i = 0; i < out_pos.size(); i++)
	{
		cout << out_pos[i].first << " - " << out_pos[i].second << endl;
	}

	double res;

	res = defuzz_y_val(out_pos, defuzz);

	cout << "Defuzzified Result: " << res << endl;
	
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
