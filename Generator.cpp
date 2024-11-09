#define _USE_MATH_DEFINES   // for math constants

#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include "Generator.h"

using std::cout; using std::endl;
using std::vector;

std::vector<double> Generator::generateN(int N) {
	/*
	generate N random variables,
	print and return the vector	
	*/

	vector<double> v;
	double x;
	for (int i = 0; i < N; ++i) {
		x = generate();
		while (isnan(x)) { x = generate(); };  // in case we get uncoverged newtonian
		v.push_back(x);
		cout << x << ((i != N - 1) ? ", " : "\n");
	}
	return v;
}

double Uniform::generate() {
	/*
	Generate one uniform random variable by fliping coins.
	We use the fact that a number between 0 and 10^accuracy_level 
	can be expressed as a binary number, so each digit is a coin flip.

	However, by doing this, it's possible to get a number greater than 10^accuracy_level.
	So we take accept-reject approach. If the generated number is greater than 10^accuracy_level
	we reject the sample, and sample it again.

	Lastly, we can divide the generated number by 10^accuracy_level	to simulate uniform(0, 1) 
	*/

	double decimal = pow(10, accuracy_level);
	
	// find how many coin flips needed to achieve this accuracy level
	int required_coin_flips = 1;
	while (pow(2, required_coin_flips) < decimal) required_coin_flips++;  

	// coin flip generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::bernoulli_distribution coin(0.5);

	// initialize the vector for coin flip results
	vector<bool> v(required_coin_flips, false);
	double x = 0;
	bool accept = false;

	while (!accept) {
		// coin flips
		for (auto itr = v.begin(); itr != v.end(); ++itr) {
			*itr = coin(gen);
		}

		// translate the binary number to decimal number
		x = 0;
		int i = 0;
		for (auto itr = v.begin(); itr != v.end(); ++itr) {
			x = x + *itr * pow(2, i++);
		}

		// accept x as a sample if it's less than 10 ** accuracy_level
		// reject and sample it again otherwise
		accept = x < decimal;
	}
	return x / decimal;
}

double ChiSquared::generate() {
	/*
	Generate one Chi-Squared distributed random variable with degree of freedom 2.
	Closed form formula for inverse cdf is available!
	We can generate uniform variable and transform it to Chi-Squared using this formula.
	*/

	double u = Uniform::generate();
	double x = -2 * log(1 - u);  // inverse cdf
	return rnd(x, accuracy_level);
}

double Normal::generate() {
	/*
	Generate one standard normal random variable.
	No closed form formula for inverse cdf here.
	But we know pdf, which is the derivative of cdf, 
	so we can use Newton-Raphson root search algorithm.
	*/

	double u = Uniform::generate();
	double precision = pow(10, -accuracy_level - 2);
	double x = Newton(u, 0, precision);
	
	return rnd(x, accuracy_level);
}

double Normal::cdf(double x) {
	/* standard normal cdf */
	double P = (1 + std::erf(x / sqrt(2))) / 2;
	return P;
}

double Normal::pdf(double x) {
	/* standard normal pdf */
	double p = exp(-pow(x, 2) / 2) / sqrt(2 * M_PI);
	return p;
}

double Normal::Newton(double u_target, double x_0, double precision) {
	/*
	Newton root search method.
	x_0 = 0 would be a reasonable initial guess
	*/

	double u = Normal::cdf(x_0);
	double x = x_0;
	int count = 0;
	while (abs(u_target - u) > precision) {
		x = (u_target - u) / Normal::pdf(x) + x;
		u = Normal::cdf(x);
		if (count++ > 30) return NAN; // in case it does not converge 
	}

	return x;
}

double rnd(double var, int accuracy_level) {
	/* function for rounding */
	double decimal = pow(10, accuracy_level);
	double value = (int)(var * decimal + 0.5);
	return (double)(value / decimal);
}

void write_csv(string filename, string colname, std::vector<double> vals) {
	/* function for streaming out the generated samples to a csv file */

	ofstream myFile(filename);

	myFile << colname << "\n";
	for (auto itr = vals.begin(); itr != vals.end(); ++itr) {
		myFile << *itr << "\n";
	}

	myFile.close();
}