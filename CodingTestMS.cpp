// CodingTestMS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "Generator.h"

using namespace std;

int main()
{

	vector<string> dist = { "u", "n", "c" };

	// user inputs
	string distType;
	int accuracy_level;
	int N;
	

	cout << "Type of Distribution (Uniform : u, Standard Normal : n, Chi-square : c) - choose from u|n|c: ";
	do {
		std::getline(cin, distType); cout << endl;
		if (std::find(dist.begin(), dist.end(), distType) != dist.end()) break;
		cout << "Not a valid input. Please choose again (u|n|c) : "  ;
	} while (true);


	cout << "Level of accuracy (1~5): " ;
	do {
		cin >> accuracy_level; cout << endl;
		if (accuracy_level > 0 && accuracy_level <= 5) break;
		cout << "Not a valid input. Please choose again (1 ~ 5): ";
	} while (true);


	cout << "Number of random numbers (1~1000): " ;
	do {
		cin >> N; cout << endl << endl;
		if (N > 0 && N <= 1000) break;
		cout << "Not a valid input. Please choose again (1 ~ 1000): ";
	} while (true);
	
	cout << "Generating 5 random numbers from " << distType << " distribution and rounding to " << pow(10, -accuracy_level) << endl;

	Generator* g_ptr;

	if (distType == "u") g_ptr = new Uniform(accuracy_level);
	else if (distType == "c") g_ptr = new ChiSquared(accuracy_level);
	else if (distType == "n") g_ptr = new Normal(accuracy_level);
	else {
		g_ptr = new Generator();
		cout << "this distribution is not supported!" << endl; 
	}

	vector<double> v = g_ptr->generateN(N);

	cout << endl;

	//write_csv("data.csv", "values", v);  // this is in case we want to save the sample

	system("pause");
	
	return 0;
}
