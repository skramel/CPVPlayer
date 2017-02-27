//
// C++ Interface: gdf
//
// Description: 
//
//
// Author: Thomas Glomann <tglomann@wesleyan.edu>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GDF_H
#define GDF_H

#include <QObject>
#include "particlefinder.h"
#include <iostream>
//#include <QtGui>
#include <QtWidgets>

// set limit after when to split files 
#define MAXPARTICLES 10000000


/**
writes gdf file

	@author Thomas Glomann <tglomann@wesleyan.edu>
*/
class GDF : public QObject
{
	Q_OBJECT
	public:
		GDF();
		~GDF();
		void writeData(const QList<Particle*> &particles, int framenum);
		bool openFile(QString name, int Node=0);
	private:
		QFile file;
		QString fname;
		qint64 posN;
		quint32 particleCounter,fileCounter;
		QDataStream out;
		int node;
		
		void writeHeader();
		void closeFile(bool done=false);
};

#endif
