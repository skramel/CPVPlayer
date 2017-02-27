#ifndef RNG_H
#define RNG_H

#include <climits>
#include <cmath>

#define EXPm1m1  exp(-1.)-1.


class cRng
{
	private:
		int a,m,q,r;
		double ran;
		
		
	public:
	cRng(int seed=16807);
	double ran0();
	int ran0(int);
	int ran0(int,int);	
	double ran0(double,double);
	double px_poisson();
};

class cLogisticMap
{
	private:
		double xn;
	public:
		cLogisticMap(double x0);
		double next();
};

#endif

