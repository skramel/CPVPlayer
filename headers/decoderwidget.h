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
#ifndef DECODERWIDGET_H
#define DECODERWIDGET_H

//#include <QtGui>
#include <QtWidgets>
#include <QtCore>
#include "decoder.h"
#include "particlefinder.h"
#include "cpv2gdf.h"
#include "rng.h"
#include <cmath>

/**
	@author Thomas Glomann <tglomann@wesleyan.edu>
*/

class cpv2gdfThread;

class DecoderWidget : public QWidget
{
	Q_OBJECT
	public:
		DecoderWidget(QWidget *parent = 0);
		~DecoderWidget();
		void openFile(const QString &fileName);
		QTimer *timer;
		
	private:
		cpv2gdfThread *thread;
		int g5x,g5y;
		bool zoom;
		double sx,sy;
		QString filename,filepath;
		QFont font;
		QPoint a,b;
		ParticleFinder pf;
		QList <PixelVector *> pixels;
		QLabel *piclabel;
		QPixmap *pixmap;
		QCheckBox *cbTrace,*cbParticle,*cbDouble;
		QSpinBox *seekTo,*searchPerimeter,*hthreshold,*ithreshold,*nthreshold,*fpn,*framerange,*mincenterI;
		QPushButton *clusterGdf,*singleGdf,*allGdf;
		Decoder *decoder;
		QVector <QLabel* > labels;
		virtual void paintEvent ( QPaintEvent * event );
		virtual void resizeEvent ( QResizeEvent * event );
		virtual void mousePressEvent ( QMouseEvent * event );
		virtual void mouseReleaseEvent ( QMouseEvent * event );
		virtual QSize sizeHint () const;
		int round(double n){if (n - floor(n) >=.5) return (int)ceil(n);else return (int)floor(n);};
		void paint(QPainter &p);
	private slots:
		void writeGDF();
		void writeGDFall();
		void writeClusterJob();
		void gdfDone();
				
	friend class cpv2gdfThread;
		
	public slots:
		void decodeNextFrame();
		void rewind();
		void shot();
		void seek();
		void reDecode(){decoder->seekToCurrentFrame();decodeNextFrame();};
};

class cpv2gdfThread : public QThread
{
	public:
		cpv2gdfThread(DecoderWidget *p);
		void run();
	private: 
		DecoderWidget *parent;
};

#endif
