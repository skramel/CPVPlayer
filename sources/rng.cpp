#include "rng.h"

cRng::cRng(int seed)
{
	a= seed;
	m=INT_MAX;
	q = 127773;
	r=2836;		
};

double cRng::ran0()
{
	static int x = 7531736;
	
	x = a*(x%q) - r*(x/q);
	while (x<0) {x+=m;}
	ran=(double)x/m;
	return ran;
}

int cRng::ran0(int max)
{
	return (int)(ran0()*max);
}

double cRng::ran0(double min, double max)
{
	return (min+ran0()*(max-min));
}

int cRng::ran0(int min,int max)
{
	return (min+(int)(ran0()*(max-min)));
}

double cRng::px_poisson()
{
	double r=ran0();
	return ( -log(r*(EXPm1m1) + 1.) );
}

cLogisticMap::cLogisticMap(double x0) : xn(x0) {}
double cLogisticMap::next() 
{
	xn = 4.*xn*(1.-xn);
	return (xn);
}

