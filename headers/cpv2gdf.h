//
// C++ Interface: cpv2gdf
//
// Description: 
//
//
// Author: Thomas Glomann <tglomann@wesleyan.edu>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CPV2GDF_H
#define CPV2GDF_H

#include <QObject>
#include <QCoreApplication>
#include <iostream>
#include <QtCore>
//#include <QtGui>
#include <QtWidgets>
#include "decoder.h"
#include "particlefinder.h"
#include "statistics.h"
#include "gdf.h"

/**
	@author Thomas Glomann <tglomann@wesleyan.edu>
*/

class cpv2gdf : public QObject
{
	Q_OBJECT
	public:
    		cpv2gdf(QObject *parent = 0);
		bool writeGDF(QString &fname, int sp, int ht,int it,int mc,int nt);
		bool writeGDF(QString &fname, int sp, int ht,int it,int mc,int nt,int cn, quint64 fs, quint64 fe);
		bool writeGDFcluster(QString &fname, int sp, int ht,int it,int mc,int nt,int fs);
	private:
		Decoder decoder;
		GDF gdf;
		ParticleFinder finder;
		QList <PixelVector*> list;

};

#endif
