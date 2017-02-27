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
#include "decoder.h"

PixelVector::PixelVector(quint8 B, quint16 X, quint16 Y) : brightness(B),x(X),y(Y)  {created++;}
PixelVector::PixelVector(PixelVector *p) {brightness=p->brightness;x=p->x;y=p->y;created++;};
PixelVector::PixelVector(const PixelVector *p){brightness=p->brightness;x=p->x;y=p->y;created++;};
int PixelVector::created = 0;
int PixelVector::deleted = 0;
PixelVector::~PixelVector()
{
// 	qDebug() << "destroying Pixelvector" << this;
	deleted++;
}

std::ostream& operator<<(std::ostream& os,const PixelVector &v) 
{
	os << (int)v.brightness;
	return os;
}

bool operator<(const PixelVector &v1,const PixelVector &v2)
{
	if (v1.brightness < v2.brightness) return true;
	else return false;
}


Decoder::Decoder(QObject* parent): QObject(parent)
{	filePos.resize(3);}

Decoder::~Decoder(){}

void Decoder::decodeHeader()
{
	file.seek((quint64)0);
	data >> version >> numLines;
	file.seek((quint64)4); // skip byte 3 and 4 (unused)
	data >> xdim >> ydim ;
	if (version >= 33) 
		data >> exposure >> rate >> gain;
	else // compatibility with older versions of cpv file
	{
		exposure = -1;
		rate = -1;
		gain = -1;
		fifoov = 0;
		lineov = -1;
	}
	filePos[first] = file.pos();
	// store position of beginning of current frame
	filePos[current] = filePos[first];
}

quint32 Decoder::decodeNextFrame(QList <PixelVector*> &pixels)
{
	lineov=0;
	fifoov=0;
	// 	pixels.clear();
// 	qDebug() << "decoding next frame";
// already done, before this function is called////////
	// 	while (!pixels.isEmpty())
// 		delete pixels.takeFirst();
	/////////////
// 	qDebug() << "created particles:"<<PixelVector::created<<"deleted particles:"<<PixelVector::deleted;
	// 	qDebug() << "deleted pixels";
	filePos[current] = file.pos();
	// first read the frame number
	data >> currentFrameNum;
	if (currentFrameNum == 0) // end of frame
		return (0);
	data.readRawData(buffer,4);
	if (version >= 33)
		threshold = (uchar)buffer[0];
	else
		threshold = -1;
	numPixels = ((uchar)buffer[3] << 14) + ((uchar)buffer[2] << 6) + ((uchar)buffer[1] >> 2);
	if (version >= 33)
	{
		if (buffer[1] & 1)	fifoov=1;
		if (buffer[1] & 2)	lineov=1;
	}
	else 
		if (buffer[0] + ((buffer[1] & 03) << 8) != 0)
			fifoov=1;
	// now read all pixels into our list
	for (quint32 i=0; i < numPixels; i++) 
	{
		data.readRawData(buffer,4);
// 		PixelVector p((uchar)buffer[0],(uchar)buffer[1] + (((uchar)buffer[2] & 07) << 8),((uchar)buffer[2] >> 3) + ((uchar)buffer[3] << 5));
// 		pixels.append(p);
		pixels.append(new PixelVector((uchar)buffer[0],(uchar)buffer[1] + (((uchar)buffer[2] & 07) << 8),((uchar)buffer[2] >> 3) + ((uchar)buffer[3] << 5)));
	}
	return (currentFrameNum);
}

bool Decoder::openFile(const QString &fileName) 
{
	file.setFileName(fileName);
	if (!file.open(QFile::ReadOnly)) 
		return false;
	data.setDevice(&file);
	data.setByteOrder(QDataStream::LittleEndian);
	return true;
} 

int Decoder::getVersion(){return version;}
int Decoder::getNumLines(){return numLines;}
quint32 Decoder::getCurrentFrame(){return currentFrameNum;}
int Decoder::getNumPixels(){return numPixels;}
QSize Decoder::getDimensions(){return QSize(xdim,ydim);}
void Decoder::seekToCurrentFrame(){file.seek(filePos[current]);}
void Decoder::seekToPosition(quint64 pos){file.seek(pos);filePos[current]=pos;}
quint64 Decoder::getFilePosition(){return file.pos();}
quint64 Decoder::getCurFilePosition(){return filePos[current];}
quint32 Decoder::seekToNextFrame()
{
	filePos[current] = file.pos(); // store pos of current frame
	data >> currentFrameNum;
	if (currentFrameNum == 0) // end of frame
		return (0);
	data.readRawData(buffer,4);
	numPixels = ((uchar)buffer[3] << 14) + ((uchar)buffer[2] << 6) + ((uchar)buffer[1] >> 2);
	filePos[tmp] = file.pos();
	file.seek(filePos[tmp] + (numPixels*4));
	return (currentFrameNum);
}
void Decoder::seekToFirstFrame(){file.seek(filePos[first]);}
int Decoder::getExposure(){return exposure;}
int Decoder::getRate(){return rate;}
int Decoder::getGain(){return gain;}
int Decoder::getFifoOv(){return fifoov;}
int Decoder::getLineOv(){return lineov;}
int Decoder::getThreshold(){return threshold;}


