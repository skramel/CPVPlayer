//
// C++ Implementation: cpv2gdf
//
// Description: 
//
//
// Author: Thomas Glomann <tglomann@wesleyan.edu>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QtCore>
//#include <QtGui>
#include <QtWidgets>

#include "cpv2gdf.h"
#include <QMessageBox>

using namespace std;

cpv2gdf::cpv2gdf(QObject *parent) : QObject(parent)
{}

bool cpv2gdf::writeGDF(QString &fname, int sp, int ht,int it,int mc,int nt)
{ // whole file
	if (!decoder.openFile(fname))
	{
		cerr << "couldn't open cpv file for reading !!!" <<endl;
		return false;
	}
	gdf.openFile(fname);
	decoder.decodeHeader();
	int frame;
	do 
	{
		// first delete everything from the prev frame
		while (!list.isEmpty())
			delete list.takeFirst();
		finder.clear();
		// now decode next frame, find particles and write to gdf file
		frame = decoder.decodeNextFrame(list);
		if (!(decoder.getLineOv() || decoder.getFifoOv())) // if we don't have an overflow find the particles
		{
			finder.findParticles(list,sp,ht,it,mc,nt);
			gdf.writeData(finder.getParticles(),frame); // closes the file automatically if frame 0 is reached
		}
	}
	while (frame);
	gdf.writeData(finder.getParticles(),0); // redundant, should not be neccessary, but doesn't hurt either
	return true;
}

bool cpv2gdf::writeGDF(QString &fname, int sp, int ht,int it,int mc,int nt,int cn, quint64 fs, quint64 fe)
{ // cluster mode and/or parts of file
	if (!decoder.openFile(fname))
	{
		cerr << "couldn't open cpv file for reading !!!" <<endl;
		return false;
	}
	gdf.openFile(fname,cn);
	decoder.seekToPosition(fs);
	quint64 beginoflastframepos = fe;
	while (decoder.getFilePosition() <= beginoflastframepos)
	{
		// first delete everything from the prev frame
		while (!list.isEmpty())
			delete list.takeFirst();
		finder.clear();
		// now decode next frame, find particles and write to gdf file
		int frame = decoder.decodeNextFrame(list); 
		finder.findParticles(list,sp,ht,it,mc,nt);
		gdf.writeData(finder.getParticles(),frame);
	}
	gdf.writeData(finder.getParticles(),0); // close the file 
	return true;
}

bool cpv2gdf::writeGDFcluster(QString &fname, int sp, int ht,int it,int mc,int nt,int fs)
{
	quint32 frame=1,jobID=1;
	quint64 startFrame;
	QFile file;
	QTextStream ts(&file);
	
	decoder.openFile(fname);
	QFileInfo fileinfo(fname);
	QDir::setCurrent(fileinfo.absolutePath());
        file.setFileName("Runjobs.sh");

	if (file.open(QIODevice::WriteOnly)) //opens pathofcpvile/runjobs.sh
	{	
		ts << "#!/bin/bash" << endl;
	}
	else 
	{
		cout << "error writing jobs file" <<endl;
		return false;
	}
	decoder.decodeHeader();
	startFrame = decoder.getCurFilePosition(); // at frame 1
	// create jobs file
	frame = decoder.seekToNextFrame(); // at frame 2
	while(frame)
	{
		if ( (frame%fs) == 0 ) // at frame split + 1
		{
// 			cout << "writing job " <<jobID<<" starting at pos "<<startFrame<<" ending with frame "<<frame<< "at pos " << decoder.getCurFilePosition()<<endl;
                        ts << "./cpv2gdf_project_new_8-2007 "<< fname << " " << sp << " " << ht << " " << it<< " " << mc  << " " << nt << " " << jobID << " " << startFrame << " " << decoder.getCurFilePosition() << " &"<<endl;
			jobID++;
			startFrame = decoder.getFilePosition(); // file pointer is already at begin of next frame
		}
		frame = decoder.seekToNextFrame(); // at split + 2
	}
	
	// we are at frame 0 = lastframe, lets write the last job
	
	if (startFrame != decoder.getCurFilePosition())
// 		cout << "writing job " <<jobID<<" starting at pos "<<startFrame<<" ending with frame "<<frame<< " at pos " << decoder.getCurFilePosition()<<endl;
		ts << "./cpv2gdf "<< fname << " " << sp << " " << ht << " " << it<< " " << mc  << " " << nt << " " << jobID << " " << startFrame << " " << decoder.getCurFilePosition() << " &"<<endl;
        file.flush();
        file.close();
        QMessageBox::information(0,tr("Cluster Sctrip Creation"),tr("Successfully created the file"));
        //QMessageBox::information(0,tr("title here"),tr("message here"));

}
