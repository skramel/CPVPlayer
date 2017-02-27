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
/**
	@author Thomas Glomann <tglomann@wesleyan.edu>
 */

#ifndef PARTICLEFINDER_H
#define PARTICLEFINDER_H

#include <QObject>
#include "decoder.h"
#include "statistics.h"
#include <cmath>
#include <iostream>
#include <QtDebug>

// #define QT_NO_DEBUG_OUTPUT

class Particle
{
	public:
		Particle();
		Particle(Particle *source);
		~Particle();
		int Id() {return id;};
		int totalIntensity;
		double eccentricity;
		double radius;
		double comx,comy; 
		QList <PixelVector *> pixels;
		static int created,deleted;
	private:
		int id;
};



class ParticleFinder : public QObject
{
	Q_OBJECT
	public:
    		ParticleFinder(QObject *parent = 0);
		~ParticleFinder();
		void findParticles(QList <PixelVector *> pixels, int perimeter=30, int hthreshold=30,int ithreshold=500, int mincenterI=30, int nthreshold=2);
		QList<Particle*> & getParticles(){return particles;};
		void clear();
	private:
		double distance(const PixelVector *c, const PixelVector *p);
		double distance(double xc,double yc, const PixelVector *p);
		QList <Particle*> particles;
};

#endif
