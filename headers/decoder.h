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
#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <QtCore>
#include <QtDebug>
#include "iostream"

#define BUFFERSIZE 4

/**
	@author Thomas Glomann <tglomann@wesleyan.edu>
*/
enum position {first,current,tmp};

class PixelVector
{
	public:
		PixelVector(quint8 B=0, quint16 X=0, quint16 Y=0);
		PixelVector(PixelVector *p);// copy constructor
		PixelVector(const PixelVector *p);// copy constructor
// 		PixelVector(PixelVector &p);// copy constructor
// 		PixelVector(const PixelVector &p);// copy constructor
		~PixelVector();
		quint8 brightness;
		quint16 x,y;
		static int created,deleted;
};


bool operator<(const PixelVector &v1,const PixelVector &v2);
std::ostream& operator<<(std::ostream& os,const PixelVector &v);

class Decoder : public QObject
{
Q_OBJECT
public:
	Decoder(QObject* parent=0);
    	~Decoder();
    	bool openFile(const QString &fileName);
	quint32 decodeNextFrame(QList <PixelVector*> &pixels); // return the current framenumber
	void decodeHeader();
	int getVersion();
	int getNumLines();
	quint32 getCurrentFrame();
	int getNumPixels();
	int getExposure();
	int getRate();
	int getGain();
	int getFifoOv();
	int getLineOv();
	int getThreshold();
	quint64 getFilePosition();
	quint64 getCurFilePosition();
	QSize getDimensions();
	void seekToPosition(quint64 pos);
	void seekToCurrentFrame();
	void seekToFirstFrame();
	quint32 seekToNextFrame(); //file pointer is at BEGINNING of next frame (i.e. returnedframe + 1). CurPointer is at returned frame.
	
private:
	
// 	int decodeFrame(qint64 frameNum=1);
	char buffer[BUFFERSIZE]; // group of 4 bytes
// 	QPixmap *pixmap;
	QFile file;
	QDataStream data;
	int fifoov,lineov;
	qint8 threshold;
	quint8 version,numLines;
	quint16 xdim,ydim;
	qint32 exposure,rate,gain;
	quint32 currentFrameNum, numPixels;
	QVector <quint64> filePos;

public slots:
	
private slots:
};

#endif
