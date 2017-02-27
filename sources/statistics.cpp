#include "statistics.h"

using namespace std;

double cDistributions::Binomial(int N, int k, double p)
{
	if (k == 0)
		return (pow(1.-p,N));
	else
		return Binomial(N,k-1,p)*(1+((N+1)*p-k)/k/(1-p));
}

double cDistributions::Poisson(int k,double lambda)
{
	return (pow(lambda,k)/fac(k)*exp(-lambda));
}

double cDistributions::Gaussian(double x, double mean, double sigma2)
{
	return (1./sqrt(TWO_PI*sigma2)*exp(-(x-mean)*(x-mean)/(2*sigma2)));
}

bool cDistributions::Gaussian(char *fname, double mean, double sigma2, int a, int b, int steps)
{
	ofstream file;
	file.open(fname);
	if (file.is_open())
	{
// 		int scaler;
// 		if ((b-a)<100)
// 		{
// 			scaler = (100/(b-a));
// 			for (int i=a;i<=(b-a)*scaler;i++)
// 			{
// 				double x = (double)(a+((double)i/scaler));
// 				file << x <<"\t"<<Gaussian(x,mean,sigma2)<<endl;
// 			}	
// 		}
// 		else
// 		{
// 			for (int i=a;i<=b;i++)
// 			{
// 				file << i <<"\t"<<Gaussian((double)i,mean,sigma2)<<endl;
// 			}
// 		}
		
		double stepwidth = (double)(b-a)/(double)steps;
		double x=(double)a;
		while (x<(double)b)
		{
			file <<  x <<"\t"<<Gaussian(x,mean,sigma2)<<endl;
			x+=stepwidth;
		}
		return true;
	}
	else return false;
}

long double cDistributions::fac(int n) 			//   n!
{
	if (n == 0) return 1.;
	else return fac(n-1)*n;
}


cHistogram::cHistogram(int Bins, int A, int B) : bins(Bins),a(A),b(B)
{
	bin = new double[bins];
	for (int i=0;i<bins;i++) bin[i]=0; // empty all bins
	delta = ((double)b-(double)a)/(double)bins;
	totalvalues = 0; // reset counter
}

cHistogram::~cHistogram()
{
	delete []bin;
}	
	
void cHistogram::addValue(double value)
{
	// bin of form ]a,b]
	value-=(double)a;
	value/=delta;
	int index = value;
	// Note THAT 0.0 will be out of range then
	if ( (index >= 0) && (index < bins))
	{
		bin[index]++; // add to Bin if within range
		totalvalues++; // increase counter
	}
	else cerr << value <<" is out of bounds in ["<<a<<"," <<b<<"]"<<endl;
}

void cHistogram::reset()
{
	for (int i=0;i<bins;i++)
		bin[i] = 0;
	totalvalues = 0;
}

int cHistogram::zero()
{
	int prev=0;
	bool a=false;
	for (int i=0;i<bins;i++)
	{
		if (bin[i])
// 			prev = i;
			a=true;
// 		if ( (bin[i] == 0) && (i == prev+1))
		if ( (bin[i] == 0) && (a))
			return i;
	}
	return bins;
}

bool cHistogram::printToFile()
{
	ofstream file;
	file.open("histogram");
	if (!file.is_open())
	{
		cerr << "error creating histogram file !" << endl;
		return false;
	}
	else
	{
		for (int i=0;i<bins;i++)
		{
			bin[i]=(double)bin[i]/totalvalues/delta; // normalize 
			file << (double)a + delta*(double)i + delta/2.<< "\t" << bin[i] << endl;
		}
		return true;
	}
	file.close();
}

bool cHistogram::printToFile(char * fname)
{
	ofstream file;
	file.open(fname);
	if (!file.is_open())
	{
		cerr << "error creating histogram file \""<<fname<<"\"!" << endl;
		return false;
	}
	else
	{
		for (int i=0;i<bins;i++)
		{
			bin[i]=(double)bin[i]/totalvalues/delta; // normalize 
			file << (double)a + delta*(double)i + delta/2. << "\t" << bin[i] << endl;
		}
		return true;
	}
	file.close();
}

cStatistics::cStatistics()
{
	meansum=0;
	mean2sum=0;
	counter=0;
	overflow=0;
}

void cStatistics::addValue(double x)
{
	meansum+=x;
	mean2sum+=(x*x);
	counter++;
	// prevent counter overflow for very large ensembles (>2 Billions)
	if (counter == INT_MAX) 
	{
		cout << "overflow !!!"<<endl;
		overflow++;
		counter=0;
	}
}

double cStatistics::Mean()
{
	if (counter)
	{
		if (!overflow)
			return ( meansum/counter );
		else
			return (meansum/((INT_MAX*overflow)+counter));
			
	}
	else 
		return (0);
}

double cStatistics::Sigma2()
{
	if (counter)
	{
		mean=Mean();
		if (!overflow)
			mean2=mean2sum/counter;
		else
			mean2=mean2sum/((INT_MAX*overflow)+counter);
		return (mean2 - (mean*mean));
		
	}
	else 
		return (0);
}

double cStatistics::StdDeviation()
{
	return (sqrt(Sigma2()));
}

double cStatistics::RelError()
{
	if (counter)
	{
		return (StdDeviation()/Mean());
	}
	else 
		return (0);
}

int cStatistics::numberOfElements()
{
	return (counter);
}

void cStatistics::reset() 
{
	meansum=0;
	mean2sum=0;
	counter=0;
	overflow=0;
}


