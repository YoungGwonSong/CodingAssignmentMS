#pragma once
#include <vector>

using namespace std;

class Generator {
	/*
	Parent class
	*/
protected:
	int accuracy_level;

public:
	Generator() : accuracy_level(1) {};
	Generator(int a) : accuracy_level(a) {};

	virtual double generate() { return 0; };  // runtime polymorphism

	vector<double> generateN(int N) ;   // generate N samples
	
	virtual ~Generator() {};
};

class Uniform : public Generator {
	/* Class for uniform sampling */
public:
	Uniform() {};
	Uniform(int a) : Generator(a) { };

	double generate();

	virtual ~Uniform() {};
};

class Normal : public Uniform {
	/* Class for standard normal sampling */
private:
	double cdf(double x);
	double pdf(double x);
	double Newton(double u_target, double x_0, double precision);

public:

	Normal() {};
	Normal(int a) : Uniform(a) {};

	double generate();

	virtual ~Normal() {};
};

class ChiSquared : public Uniform {
	/* Class for Chi-squared w/ degree of freedom 2 sampling */
public:
	ChiSquared() {};
	ChiSquared(int a) : Uniform(a) {};

	double generate();

	virtual ~ChiSquared() {};
};

double rnd(double var, int accuracy_level); // function for rounding

void write_csv(string filename, string colname, std::vector<double> vals);
// function for saving the samples to a file