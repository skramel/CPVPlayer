//
// C++ Implementation: gdf
//
// Description: 
//
//
// Author: Thomas Glomann <tglomann@wesleyan.edu>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//

//
#include "gdf.h"

using namespace std;

GDF::GDF()
{
	fileCounter = 1;
	particleCounter = 0;
// 	if (node)
// 		cout << "cluster mode. node "<<node<<endl;
// 	else
// 		cout << "non cluster mode."<<endl;
}


GDF::~GDF() 
{
}

bool GDF::openFile(const QString name, int Node)
{
	fname = name;
	node = Node;
	QFileInfo fileinfo(fname);
	QString numstr,filestr;
	QTextStream ts(&numstr),fs(&filestr);
	ts.setFieldWidth(4);
	ts.setPadChar('0');
	ts << right << fileCounter;
	QDir::setCurrent(fileinfo.absolutePath());
	if (node) // clustermode
		fs << fileinfo.baseName() << "_featdat_" << node << ".gdf";
	else
		fs << fileinfo.baseName() << "_featdat_" << numstr << ".gdf";
    qDebug(filestr.toLatin1());
	file.setFileName(filestr);
	if (file.open(QIODevice::WriteOnly))
	{	
		out.setDevice(&file);
		out.setByteOrder(QDataStream::LittleEndian);
		writeHeader();
		return true;
	}
	else 
	{
		cerr << " couldn't open gdf file for writing !!!"<<endl;
		return false;
	}
}

void GDF::writeHeader()
{
	out << (quint32)82991; // a magic number
	out << (quint32)2 << (quint32)6 ;
	posN = file.pos(); // store position of the value N
	out << (quint32)0 << (quint32)0 << (quint32)0; // write dummy values. need to fill in later ->fixHeader()
}

void GDF::closeFile(bool done) // fixup the header and close the file and increase filecounter
{
	if (!file.isOpen()) // prevent multiple closing
		return;
	//fixup the header
	file.seek(posN);
	out << (quint32)particleCounter << (quint32)4 << (quint32)particleCounter*6;
	//close the file
	file.close();
	cout << "closed the file"<<endl;
	// reopen the file if neccessary
	if (!done)
	{
		particleCounter = 0;
		fileCounter++;
		openFile(fname,node);
	}
}

void GDF::writeData(const QList<Particle*> &particles, int framenum)
{
	if (framenum)
	{
		if (particleCounter > MAXPARTICLES)
			closeFile(false); // close and open a new file
		for (int i=0;i<particles.size();i++) 
		{// loop over all particles
			out << (float)particles.at(i)->comx;
			out << (float)particles.at(i)->comy;
			out << (float)particles.at(i)->totalIntensity;
			out << (float)particles.at(i)->radius;
			out << (float)particles.at(i)->eccentricity;
			out << (float)framenum;
			particleCounter++;
		}
	}
	else
		closeFile(true); // end of cpv file. we close it for good and call it a day.
}
