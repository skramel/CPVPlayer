#ifndef STATISTICS_H
#define STATISTICS_H

#include <cmath>
#include <iostream>
#include <fstream>

#define TWO_PI M_PI*2
#define INT_MAX 20000
class cDistributions
{
	public:
		double Binomial(int N, int k, double p);	// P_k^N(p)
		double Poisson(int k,double lambda);	// P_k
		double Gaussian(double x, double mean, double sigma2);
		bool Gaussian(char *fname, double mean, double sigma2,int a, int b,int steps=100);
		long double fac(int n); 		// n!
	private:
};

class cStatistics
{
	public:
		cStatistics();
		void addValue(double x);
		double Mean();
		double Sigma2();
		double StdDeviation();
		double RelError();
		int numberOfElements();
		void reset();

	private:
		double meansum,mean2sum,mean,mean2;
		int counter,overflow;
};

class cHistogram
{
	public:
		cHistogram(int Bins, int A, int B);
		~cHistogram();
		void addValue(double value);
		bool printToFile(char * fname);
		bool printToFile();
		int zero();
		void reset();
		int values(){return totalvalues;};
	
	private:
		int bins, a, b, totalvalues;;
		double delta;
		double *bin;
};

#endif


