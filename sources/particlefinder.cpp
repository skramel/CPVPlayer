/***************************************************************************
 *   Copyright (C) 2007 by Thomas Glomann   *
 *   tglomann@wesleyan.edu   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "particlefinder.h"

using namespace std;

Particle::Particle()
{
	totalIntensity=0;radius=0;comx=0;comy=0;
	created++;
	id = created;
// 	qDebug() << "creating particle" << Id();
}
int Particle::created = 0;
int Particle::deleted = 0;

Particle::Particle(Particle *source)
{
	totalIntensity=source->totalIntensity;radius=source->radius;comx=source->comx;comy=source->comy;
	//copy the pixels of the particle 
	// !!! not like this !!!!   pixels=source->pixels;
	// !! also not like this !!!
// 	while (!source->pixels.isEmpty())
// 	{
// 		pixels.append(new PixelVector(source->pixels.takeFirst()));
// 	}
	for (int i=0;i<source->pixels.size();i++)
	{
		pixels.append(new PixelVector(source->pixels.at(i)));
	}
	created++;
	id = created;
// 	qDebug() << "creating particle" << Id() <<"as a copy of"<<source->Id();
}

Particle::~Particle()
{	
	// cleanly delete all pixels from the particle
// 	qDebug() << "deconstructing particle" << Id() << this;
	while (!pixels.isEmpty())
	{
// 		qDebug() << "deleting pixels from particle" << Id() << this;
		delete pixels.takeFirst();
	}
// 	qDebug() << "deleting particle" << Id();
	deleted++;
}

ParticleFinder::ParticleFinder(QObject *parent): QObject(parent){}
ParticleFinder::~ParticleFinder()
{
	while (!particles.isEmpty())
		delete particles.takeFirst();
	qDebug() << "created particles:"<<Particle::created<<"deleted particles:"<<Particle::deleted;
}

void ParticleFinder::findParticles(QList <PixelVector *> pixels, int perimeter, int hthreshold, int ithreshold, int mincenterI, int nthreshold)
{
	// delete all old particles
	while (!particles.isEmpty())
		delete particles.takeFirst();
	// sort the pixels list
	qSort(pixels.begin(), pixels.end(),qGreater<PixelVector>());  // sort the list for brightness. thx to Qt ;)
	Particle *p;
	while (!pixels.empty()) // let's find the particles within a Radius R
	{
		if (pixels.first()->brightness >= mincenterI) // only go on if our center pixel is geq mincenterintensity
		{
			int xm=0,ym=0,I=0;
			double comx=0,comy=0;
			cHistogram histo(perimeter,0,perimeter),histo2(perimeter,0,perimeter);
			QList <PixelVector *> tmplist;
			p = new Particle(); // the current particle we will work on
			// look at first brightest pixel and use it as center
			PixelVector *center = new PixelVector(pixels.first()); 
			// first stage: find all pixels that are inside the search perimeter 
			// make a histogram of the distance away from the brightest pixel
			// move them over to the tmplist
			
// 			qDebug() << "working on brightpixel at ("<<center->x<<","<<center->y<<") brightness "<<center->brightness<<endl;
			
			for (int i=pixels.size()-1; i>=0;i--)  // loop through all pixels reversely
			{
				PixelVector *ci = new PixelVector(pixels.at(i)); 
				double d = distance(center,ci);
				if (d < (double)perimeter) //within search perimeter 
				{
// 					const PixelVector ci(pixels.takeAt(i)); // take from pixelslist 
					if ( (ci->brightness >= hthreshold)) // above threshold
						histo.addValue(d);
					tmplist.append((pixels.takeAt(i)));	// add to tmplist
				}
				delete ci;
			}
			//tmplist contains all pixels withing searchperimeter incl. the center pixel itself !
		// second stage: find center of mass of the pixels withing distance zero of histogram
			int dzero = histo.zero();

// 			qDebug() << "\tadded "<<tmplist.size()<<" pixels to tmplist"<<endl;
// 			qDebug() << "\thistogram has "<<histo.values()<<" values. zero at radius bigger than "<<dzero<<endl;

			for (int i = tmplist.size()-1 ; i >= 0 ; i--)  // loop through all pixels reversely
			{
				PixelVector *ci = new PixelVector(tmplist.at(i)); 
				double d = distance(center,ci);
				if (d <= (double)dzero) 
				{
					int m = ci->brightness;
					I += m;
					xm += m * ci->x;
					ym += m * ci->y;
				}
				delete ci;
			}
			comx = (double)xm / (double)I;
			comy = (double)ym / (double)I;

// 			qDebug() << "\t1st center of mass at ("<<comx << "," << comy <<")."<< endl;

		// third stage: iterate again, now using the new COM position
			I = 0.;
			xm = 0;
			ym = 0;
			for (int i=tmplist.size()-1; i>=0;i--)  // loop through all pixels reversely
			{
				PixelVector *ci = new PixelVector(tmplist.at(i)); 
				double d = distance(comx,comy,ci);
				if (d < (double)perimeter) //within search perimeter 
				{
					if ( (ci->brightness >= hthreshold)) // above threshold
						histo2.addValue(d);
				}
				delete ci;
			}
			dzero = histo2.zero();

// 			qDebug() << "\t2. histogram has "<<histo2.values()<<" values. zero at radius bigger than "<<dzero<<endl;

		// now recalculate center of mass
			for (int i=tmplist.size()-1; i>=0;i--)  // loop through all pixels reversely
			{
				PixelVector *ci = new PixelVector(tmplist.at(i)); 
				double d = distance(comx,comy,ci);
				if (d <= (double)dzero) 
				{
					int m = ci->brightness;
					I += m;
					xm += m * ci->x;
					ym += m * ci->y;
				}
				delete ci;
			}
			comx = (double)xm / (double)I;
			comy = (double)ym / (double)I;

// 			qDebug() << "\t2nd center of mass at ("<<comx << "," << comy <<")."<< endl;

		// now that we have the right COM, we can determine the particle
			I = 0;
			double radius = 0.;
			for (int i=tmplist.size()-1; i>=0;i--)  // loop through tmplist reversely
			{
				PixelVector *ci = tmplist.takeAt(i); // remove from our tmplist and process it
				double d = distance(comx, comy, ci);
				if (d <= (double)dzero) // if it is withing bounds of our determinded radius
				{
					I += (int)ci->brightness;
					radius += d * (int)ci->brightness;
					PixelVector *newpixel = new PixelVector(ci);
					p->pixels.append(newpixel); // add pixel to pixellist of particle
				}
				else // add back to our original pixels list cos it was not part of our current particle
					pixels.append(ci); // put to the back of the list
			}
			delete center; 
			if (tmplist.size() != 0 )
				cout << "\tsizeof tmplist is not 0 !!!" << endl;
			// add the particle to the particles list ..
			if ((p->pixels.size() >= nthreshold) && (I >= ithreshold)) // decide if we recognize it as a real particle
			{
				radius /= (double)I; // calc average radius
				// write particle info
				p->comx = comx;
				p->comy = comy;
				p->radius = radius;
				p->totalIntensity = I;
				// calc eccentricity (translated from IDL code)
				double theta,cos2t,sin2t,cossumsq=0,sinsumsq=0;
				for (int i=0;i<p->pixels.size();i++)
				{
					theta = atan2((double)p->pixels.at(i)->y-comy , (double)p->pixels.at(i)->x-comx);
	// 				cout << "angle theta is "<<theta*180/M_PI<<endl;
					cos2t = cos(2*theta);
					sin2t = sin(2*theta);
					cossumsq += pow(cos2t * p->pixels.at(i)->brightness,2);
					sinsumsq += pow(sin2t * p->pixels.at(i)->brightness,2);
				}
				
				p->eccentricity = sqrt(cossumsq + sinsumsq) / (p->totalIntensity);
// 				qDebug() << "\teccentricity of particle at ("<<p->comx<<","<<p->comy<<") is "<<p->eccentricity;
// 				qDebug() << "\tweighted radius of particle at ("<<p->comx<<","<<p->comy<<") is "<<p->radius;
// add the particle to the list		
// 				qDebug() << "\tadding Particle. tot_I= "<<p->totalIntensity<<"\tsize of particle = "<<p->pixels.size();
				particles.append(new Particle(p));
			}
			else
			{
// 				qDebug() << "\tNot adding Pseudo Particle. tot_I= "<<p->totalIntensity<<"\tsize of particle = "<<p->pixels.size();
			}
			delete p;
		}
		else // there are leftover pixels in the list, either merely noise, or still particles -> resort and redo everything
		{
			bool end=true;
			// check if we have only noise left, or if there are still potential particles
			for (int i=0;i<pixels.size();i++)
				if (pixels.at(i)->brightness >= mincenterI)
					end = false;
			if (end) // all remaining pixels must be noise, delete them !!!
			{
// 				while (!pixels.isEmpty())
// 					delete pixels.takeFirst();
				break;
			}
			else
				qSort(pixels.begin(), pixels.end(),qGreater<PixelVector>());
		}
	}	

// 	qDebug() << "I found " <<particles.size()<< " particle clusters!" << endl;	
}

void ParticleFinder::clear()
{
	while (!particles.isEmpty())
		delete particles.takeFirst();
}

double ParticleFinder::distance(const PixelVector *c, const PixelVector *p)
{
	return sqrt(pow((double)(c->x-p->x),2)+pow((double)(c->y-p->y),2));
}
double ParticleFinder::distance(double xc,double yc, const PixelVector *p)
{
	return sqrt(pow(xc-p->x,2)+pow(yc-p->y,2));
}

