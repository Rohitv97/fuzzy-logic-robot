// ariafollow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include "aria.h"
#include<iostream>
#include<stdlib.h>
#include<math.h>
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


vector<pair<string, double>> findMemVal(double x, FuzzyClass fuzz)
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

vector<pair<pair<string, string>, double>> inter_y_val_obstacle(RuleTable rule, vector<pair<string, double>> y1temp, vector<pair<string, double>> y2temp, vector<pair<string, double>> y3temp)
{
	double val;
	vector<pair<pair<string, string>, double>> out_pos;
	for (unsigned int i = 0; i < rule.allRules.size(); i++)
	{
		for (unsigned int j = 0; j < y1temp.size(); j++)
		{
			for (unsigned int k = 0; k < y2temp.size(); k++)
			{
				for (unsigned int l = 0; l < y3temp.size(); l++)
				{
					if ((y1temp[j].first == rule.allRules[i][0]) && (y2temp[k].first == rule.allRules[i][1]) && (y3temp[l].first == rule.allRules[i][2]))
					{
						if (y1temp[j].second > y2temp[k].second)
						{
							if (y2temp[k].second > y3temp[l].second)
							{
								val = y3temp[l].second;
							}
							else
							{
								val = y2temp[k].second;
							}
						}
						else
						{
							if (y1temp[j].second > y3temp[l].second)
							{
								val = y3temp[l].second;
							}
							else
							{
								val = y1temp[j].second;
							}
						}
						//out_pos.push_back({ rule.allRules[i][2], val });
						out_pos.push_back({ { rule.allRules[i][3], rule.allRules[i][4] }, val });

						cout << "Rules fired: " << rule.allRules[i][3] << " , " << rule.allRules[i][4] << " -- strength = " << val << endl;
					}
				}
			}
		}
	}
	return out_pos;
}

pair<double, double> defuzz_y_val(vector<pair<pair<string, string>, double>> out_pos, FuzzyClass defuzz)
{
	double avg;
	double intermediate_result = 0;
	double res;
	double sum_of_mf = 0;
	pair<double, double> result;
	for (unsigned int i = 0; i < defuzz.allSets.size(); i++)
	{
		for (unsigned int j = 0; j < out_pos.size(); j++)
		{
			if (defuzz.allSets[i].memberName == out_pos[j].first.first)
			{
				int n = defuzz.allSets[i].linePoint.size();
				avg = (defuzz.allSets[i].linePoint[0].first + defuzz.allSets[i].linePoint[n - 1].first) / 2;
				intermediate_result += avg * out_pos[j].second;
				sum_of_mf += out_pos[j].second;
			}
		}
	}
	//cout << "----LEFT----" << endl;
	//cout << "intermediate result---> " << intermediate_result << endl;
	//cout << "sum of mf ----> " << sum_of_mf << endl;
	res = intermediate_result / sum_of_mf;
	result.first = res;
	sum_of_mf = 0;
	intermediate_result = 0;
	for (unsigned int i = 0; i < defuzz.allSets.size(); i++)
	{
		for (unsigned int j = 0; j < out_pos.size(); j++)
		{
			if (defuzz.allSets[i].memberName == out_pos[j].first.second)
			{
				int n = defuzz.allSets[i].linePoint.size();
				avg = (defuzz.allSets[i].linePoint[0].first + defuzz.allSets[i].linePoint[n - 1].first) / 2;
				intermediate_result += avg * out_pos[j].second;
				sum_of_mf += out_pos[j].second;
			}
		}
	}
	//cout << "----RIGHT----" << endl;
	//cout << "intermediate result---> " << intermediate_result << endl;
	//cout << "sum of mf ----> " << sum_of_mf << endl;
	res = intermediate_result / sum_of_mf;
	result.second = res;

	return result;
}

int main(int argc, char **argv)
{
	//Initialise
	//create instances
	Aria::init();
	ArRobot robot;

	//parse command line arguments
	ArArgumentParser argParser(&argc, argv);
	argParser.loadDefaultArguments();

	//Connect to robot
	ArRobotConnector robotConnector(&argParser, &robot);

	if (robotConnector.connectRobot())
		std::cout << "Robot connected!" << std::endl;

	robot.runAsync(false);
	robot.lock();
	robot.enableMotors();
	robot.unlock();

	//Initialize fuzzy membership values now

	vector<pair<int, int>> finalLines;
	Member m1;

	m1.memberName = "L";
	m1.linePoint = { { 0,1 },{ 250,1 },{ 400,0 } };
	Member m2;
	m2.memberName = "M";
	m2.linePoint = { { 250,0 },{ 400,1 },{ 500,1 },{ 650,0 } };
	Member m3;
	m3.memberName = "H";
	m3.linePoint = { { 500,0 },{ 650,1 },{ 2000,1 } };

	FuzzyClass fuzz;
	fuzz.allSets.push_back(m1);
	fuzz.allSets.push_back(m2);
	fuzz.allSets.push_back(m3);

	Member o1;
	o1.memberName = "L";
	o1.linePoint = { { 25,0 },{ 50,1 },{ 75, 0 } };
	Member o2;
	o2.memberName = "M";
	o2.linePoint = { { 75,0 },{ 125,1 },{ 150, 0 } };
	Member o3;
	o3.memberName = "H";
	o3.linePoint = { { 150,0 },{ 175,1 },{ 200, 0 } };

	FuzzyClass defuzz;
	defuzz.allSets.push_back(o1);
	defuzz.allSets.push_back(o2);
	defuzz.allSets.push_back(o3);

	RuleTable rule;
	rule = { { { "L","L","L","L","H" },{ "L","L","M","H","L" },{ "L","L","H","H","L" },{ "L","M","L","H","L" },{ "L","M","M","H","L" },
	{ "L","M","H","H","L" },{ "L","H","L","L","H" },{ "L","H","M","M","L" },{ "L","H","H","H","L" },{ "M","L","L","L","H" },{ "M","L","M","L","H" },
	{ "M","L","H","H","L" },{ "M","M","L","L","H" },{ "M","M","M","M","H" },{ "M","M","H","H","M" },{ "M","H","L","L","H" },{ "M","H","M","M","M" },
	{ "M","H","H","H","M" },{ "H","L","L","L","H" },{ "H","L","M","L","H" },{ "H","L","H","L","H" },{ "H","M","L","L","H" },{ "H","M","M","L","H" },
	{ "H","M","H","L","M" },{ "H","H","L","L","H" },{ "H","H","M","M","H" },{ "H","H","H","M","M" } } };

	//rule = { { { "L","L","M","H" },{ "L","M","M","H" },{ "L","H","M","H" } ,{ "M","L","H","M" } ,{ "M","M","M","M" } ,{ "M","H","M","H" } ,{ "H","L","H","M" } ,{ "H","M","H","M" } ,{ "H","H","H","M" } } };


	vector<pair<string, double>> y1temp, y2temp, y3temp;

	vector<pair<pair<string, string>, double>> out_pos;

	double lfs, fs, rfs;

	while (true)
	{
		//----------Get Sonar Readings----------

		ArSensorReading *sonarSensor[8];

		int sonarRange[8];
		for (int i = 0; i < 8; i++)
		{
			sonarSensor[i] = robot.getSonarReading(i);
			sonarRange[i] = sonarSensor[i]->getRange();
		}


		double min2 = sonarRange[2];

		for (int i = 0; i < 5; i++)
		{
			double val2 = robot.getSonarReading(2)->getRange();
			if (min2 > val2)
			{
				min2 = val2;
			}
		}

		double min3 = sonarRange[3];

		for (int i = 0; i < 5; i++)
		{
			double val3 = robot.getSonarReading(3)->getRange();
			if (min3 > val3)
			{
				min3 = val3;
			}
		}

		double min4 = sonarRange[4];

		for (int i = 0; i < 5; i++)
		{
			double val4 = robot.getSonarReading(4)->getRange();
			if (min4 > val4)
			{
				min4 = val4;
			}
		}

		double min5 = sonarRange[5];

		for (int i = 0; i < 5; i++)
		{
			double val5 = robot.getSonarReading(5)->getRange();
			if (min5 > val5)
			{
				min5 = val5;
			}
		}

		lfs = min2;
		fs = (min3 + min4) / 2;
		rfs = min5;

		if (lfs > 2000)
		{
			lfs = 1900;
		}
		if (fs > 2000)
		{
			fs = 1900;
		}
		if (rfs > 2000)
		{
			rfs = 1900;
		}

		y1temp = findMemVal(lfs, fuzz);
		y2temp = findMemVal(fs, fuzz);
		y3temp = findMemVal(rfs, fuzz);

		out_pos = inter_y_val_obstacle(rule, y1temp, y2temp, y3temp);

		pair<double, double> res;

		res = defuzz_y_val(out_pos, defuzz);

		//---------Setting speed----------

		double l, r;
		l = res.first;
		r = res.second;
		robot.setVel2(l, r);
		cout << "Left, Right ----->  " << res.first << "," << res.second << endl;
		//robot.setVel2(base - output, base);

		ArUtil::sleep(100);

	}

	//termination
	//stop the robot
	robot.lock();
	robot.stop();
	robot.unlock();

	//terminate all threads
	Aria::exit();


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
