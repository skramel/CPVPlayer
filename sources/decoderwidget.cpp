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
#include "decoderwidget.h"

using namespace std;

DecoderWidget::DecoderWidget(QWidget *parent)
 : QWidget(parent)
{
	QGridLayout *mainlayout = new QGridLayout();
	piclabel = new QLabel("no file loaded");
	piclabel->setAlignment(Qt::AlignCenter);
	piclabel->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
	timer = new QTimer;
	timer->setInterval(40);
	connect(timer,SIGNAL(timeout()),this,SLOT(decodeNextFrame()));
	pixmap = 0;
	// scaling is easy but it is slow
	piclabel->setScaledContents(false);
	piclabel->setMinimumSize(50,50);
	decoder = new Decoder();
	for (int i=0;i<34;i++)
		labels.append(new QLabel());
	labels[0]->setText("filename");
	labels[1]->setText("codec version");
	labels[2]->setText("number of lines");
	labels[3]->setText("resolution");
	labels[19]->setText("exposure time [ms]");
	labels[20]->setText("capture rate [fps]");
	labels[21]->setText("gain");
	labels[22]->setText("N/A");
	labels[23]->setText("N/A");
	labels[24]->setText("N/A");
	labels[4]->setText("N/A");
	labels[5]->setText("N/A");
	labels[6]->setText("N/A");
	labels[7]->setText("N/A");
	labels[8]->setText("x");
	labels[9]->setText("N/A");
	labels[10]->setText("frame number");
	labels[11]->setText("bright pixels");
	labels[12]->setText("FIFO overflow");
	labels[13]->setText("line overflow");
	labels[18]->setText("threshold");
	labels[25]->setText("N/A");
	labels[14]->setText("N/A");
	labels[15]->setText("N/A");
	labels[16]->setText("N/A");
	labels[17]->setText("N/A");
	cbTrace = new QCheckBox("trajectories");
	cbParticle = new QCheckBox("find Particles");
	labels[26]->setText("seek to frame");
	seekTo = new QSpinBox();
	seekTo->setRange(0,2000000000);
	labels[27]->setText("search perimeter");
	searchPerimeter = new QSpinBox();
	searchPerimeter->setRange(0,100);
	searchPerimeter->setValue(30);
	labels[28]->setText("histogram threshold");
	hthreshold = new QSpinBox();
	hthreshold->setRange(0,100);
	hthreshold->setValue(30);
	labels[29]->setText("Min Total Intensity ");
	ithreshold = new QSpinBox();
	ithreshold->setRange(1,100000);
	ithreshold->setValue(500);
	labels[30]->setText("Min Number of Pixels");
	nthreshold = new QSpinBox();
	nthreshold->setRange(1,100);
	nthreshold->setValue(2);
	labels[31]->setText("frame range");
	framerange = new QSpinBox();
	framerange->setRange(1,100000);
	framerange->setValue(1000);
	singleGdf = new QPushButton("create GDF\nframe range");
	labels[32]->setText("fames per node");
	fpn = new QSpinBox();
	fpn->setRange(100,10000000);
	fpn->setValue(1000000);
	clusterGdf = new QPushButton("create the \ncluster script");
	labels[33]->setText("min Center Intensity");
	mincenterI = new QSpinBox();
	mincenterI->setRange(1,255);
	mincenterI->setValue(100);
	allGdf = new QPushButton("convert whole cpv file to gdf");
	QGroupBox *g1 = new QGroupBox(tr("Video Information"));
	QGroupBox *g2 = new QGroupBox(tr("Frame Information"));
	QGroupBox *g3 = new QGroupBox(tr("Playback Options"));
	QGroupBox *g4 = new QGroupBox(tr("Particle Finding Parameters"));
	QGroupBox *g5 = new QGroupBox(tr("write GDF file"));
	QHBoxLayout *g1m = new QHBoxLayout;	
	QHBoxLayout *g2m = new QHBoxLayout;	
	QVBoxLayout *g1l = new QVBoxLayout;
	g1l->addWidget(labels[0]);
	g1l->addWidget(labels[1]);
	g1l->addWidget(labels[2]);
	g1l->addWidget(labels[3]);
	g1l->addWidget(labels[18]);
	g1l->addWidget(labels[19]);
	g1l->addWidget(labels[20]);
	g1l->addWidget(labels[21]);
	QVBoxLayout *g1r = new QVBoxLayout;
	g1r->addWidget(labels[4]);
	g1r->addWidget(labels[5]);
	g1r->addWidget(labels[6]);
	QHBoxLayout *g1rh = new QHBoxLayout;
	g1rh->addWidget(labels[7]);
	g1rh->addWidget(labels[8]);
	g1rh->addWidget(labels[9]);
	g1rh->addStretch(0);
	g1r->addLayout(g1rh);
	g1r->addWidget(labels[25]); // thres
	g1r->addWidget(labels[22]); // exp
	g1r->addWidget(labels[23]); // fps
	g1r->addWidget(labels[24]); //gain
	g1m->addLayout(g1l);
	g1m->addLayout(g1r);
	g1m->addStretch(0);
	g1->setLayout(g1m);
			
	QVBoxLayout *g2l = new QVBoxLayout;
	g2l->addWidget(labels[10]);
	g2l->addWidget(labels[11]);
	g2l->addWidget(labels[12]);
	g2l->addWidget(labels[13]);
	QVBoxLayout *g2r = new QVBoxLayout;
	g2r->addWidget(labels[14]);
	g2r->addWidget(labels[15]);
	g2r->addWidget(labels[16]);
	g2r->addWidget(labels[17]);
	g2m->addLayout(g2l);
	g2m->addLayout(g2r);
	g2m->addStretch(0);
	g2->setLayout(g2m);
	
	QVBoxLayout *g3v = new QVBoxLayout;
	QHBoxLayout *g3h = new QHBoxLayout;
	QHBoxLayout *g3h2 = new QHBoxLayout;
	QHBoxLayout *g3h3 = new QHBoxLayout;
	g3h->addWidget(labels[26]);
	g3h->addWidget(seekTo);
	g3h->addStretch(0);
	g3h2->addWidget(cbTrace);
	g3h2->addWidget(cbParticle);
	g3h2->addStretch(0);
	g3v->addLayout(g3h2);
	g3v->addLayout(g3h);
// 	g3v->addStretch(0);
	g3->setLayout(g3v);
	
	QVBoxLayout *g4v1 = new QVBoxLayout;
	QVBoxLayout *g4v2 = new QVBoxLayout;
	QHBoxLayout *g4h = new QHBoxLayout;
	g4v1->addWidget(labels[27]);
	g4v1->addWidget(labels[28]);
	g4v1->addWidget(labels[29]);
	g4v1->addWidget(labels[33]);
	g4v1->addWidget(labels[30]);
	g4v2->addWidget(searchPerimeter);
	g4v2->addWidget(hthreshold);
	g4v2->addWidget(ithreshold);
	g4v2->addWidget(mincenterI);
	g4v2->addWidget(nthreshold);
	g4h->addLayout(g4v1);
	g4h->addLayout(g4v2);
	g4h->addStretch(0);
	g4->setLayout(g4h);	
	
	QVBoxLayout *g5v1 = new QVBoxLayout;
	QVBoxLayout *g5v2 = new QVBoxLayout;
	QVBoxLayout *g5v = new QVBoxLayout;
	QHBoxLayout *g5h0 = new QHBoxLayout;
	QHBoxLayout *g5h1 = new QHBoxLayout;
	QHBoxLayout *g5h2 = new QHBoxLayout;
	g5v1->addWidget(labels[31]);
	g5v1->addWidget(framerange);
	g5v1->addStretch(0);
	g5v2->addWidget(labels[32]);
	g5v2->addWidget(fpn);
	g5v2->addStretch(0);
// 	g5h0->addStretch(0);
	g5h0->addWidget(allGdf);
	g5h1->addLayout(g5v1);
	g5h1->addWidget(singleGdf);
	g5h2->addLayout(g5v2);
	g5h2->addWidget(clusterGdf);
	g5v->addLayout(g5h0);
	g5v->addLayout(g5h1);
	g5v->addLayout(g5h2);
	g5v->addStretch(0);
	g5->setLayout(g5v);
	g5x = g5->width();
	g5y = g5->height()+g4->height()+g3->height()+g2->height()+g1->height();
	
	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addWidget(g1);
	vlayout->addWidget(g2);
	vlayout->addWidget(g3);
	vlayout->addWidget(g4);
	vlayout->addWidget(g5);
	vlayout->addStretch(0);
	mainlayout->addWidget(piclabel,0,0);
	mainlayout->addLayout(vlayout,0,1);
	mainlayout->setColumnStretch(0,5);
	setLayout(mainlayout);
	// non layouting things
	font.setFamily("Arial");
	font.setPointSize(40);
	thread = new cpv2gdfThread(this);
	connect(thread,SIGNAL(finished()),this,SLOT(gdfDone()));
	
	resize(this->sizeHint());
}


DecoderWidget::~DecoderWidget(){}

void DecoderWidget::paint(QPainter &p)
{
	if (thread->isRunning())
	{
		p.setFont(font);
		p.setPen(QColor(128,128,255,225));
		p.drawText(QPoint(20,60),QString("CPV2GDF conversion thread is running")); //QString("# Particles: ")+
	}
	else
	{
		// draw all pixels
		for (int i=0; i < pixels.size(); i++) 
		{
			int b = pixels.at(i)->brightness;
			p.setPen(QColor(b,b,b));
			p.drawPoint(pixels.at(i)->x,pixels.at(i)->y);
		}
		// draw particle
		
		{	
			int num = pf.getParticles().size();
			for (quint32 i=0; i < num; i++) 
			{
			// draw all pixels that belong to a particle
				p.setPen(QColor(255,0,0,100));
				for (quint32 j=0; j < pf.getParticles().at(i)->pixels.size(); j++) 
				{
					p.drawPoint(pf.getParticles().at(i)->pixels.at(j)->x,pf.getParticles().at(i)->pixels.at(j)->y);
				}
				double r = (pf.getParticles().at(i)->radius);
				double x = (pf.getParticles().at(i)->comx);
				double y = (pf.getParticles().at(i)->comy);
			// draw circle
				p.setPen(QColor(0,0,255,128));
	// 			p.setBrush(QBrush(QColor(0,0,255,128)));
				QRectF ellipse(x-8.*r,y-8.*r,r*16.+1,r*16.+1);
		// 		QRectF ellipse(x-r,y-r,r*2.,r*2.);
				p.drawEllipse(ellipse);
			// draw COM
				p.setPen(QColor(0,255,0,255));
				p.drawPoint(QPointF(round(x),round(y)));
			}
			// draw number of particles
			if (cbParticle->isChecked() && !zoom)
			{
				p.setFont(font);
				p.setPen(QColor(255,255,255,125));
				p.drawText(QPoint(20,60),QString::number(num)); //QString("# Particles: ")+
			}
		}
	}
	
}

void DecoderWidget::paintEvent ( QPaintEvent * event )
{
	// draw the pixmap on the screen 
	if (pixmap)
	{

		if (zoom)
		{
			double fx = (double)decoder->getDimensions().width()/piclabel->width();
			double fy = (double)decoder->getDimensions().height()/piclabel->height();
			QPixmap screenshot(decoder->getDimensions());
			QRect window(QPoint(1,1),decoder->getDimensions());
			QPainter p(&screenshot);
			p.fillRect(window,QBrush(Qt::black)); // clear screen	
			paint(p);
			QPixmap pix(screenshot.copy(fx*a.x(),fy*a.y(),fx*(b.x()-a.x()),fy*(b.y()-a.y())));
			piclabel->setPixmap(pix.scaled(piclabel->size()));
		}
		else
		{
			QPainter p(pixmap);
// 			QRect window(0,0,decoder->getDimensions().width()-1,decoder->getDimensions().height()-1);
			QRect window(QPoint(1,1),decoder->getDimensions());
			p.setWindow(window);
			if (!cbTrace->isChecked())	
				p.fillRect(window,QBrush(Qt::black)); // clear screen
			paint(p);
			piclabel->setPixmap(*pixmap);
		}
	}
	
}

void DecoderWidget::resizeEvent ( QResizeEvent * event )
{
	if (pixmap)
	{
		if (timer->isActive())
		{
			timer->stop();
			delete pixmap;
			pixmap = new QPixmap(piclabel->size());
			// redraw last frame
			cbTrace->setCheckState(Qt::Unchecked);
			timer->start();
		}
		else
		{
			delete pixmap;
			pixmap = new QPixmap(piclabel->size());
			cbTrace->setCheckState(Qt::Unchecked);
		}
		update();
	}
}

void DecoderWidget::decodeNextFrame()
{
	// clear everything
	while (!pixels.isEmpty())
		delete pixels.takeFirst();
	pf.clear();
// 	qDebug() << "created pixels:"<<PixelVector::created<<"deleted pixels:"<<PixelVector::deleted;
// 	qDebug() << "created particles:"<<Particle::created<<"deleted particles:"<<Particle::deleted;
	int frame = decoder->decodeNextFrame(pixels);
	if (frame) // first framenumber of cpv file is 1.
	{
		labels[14]->setNum(frame);
		labels[15]->setNum(decoder->getNumPixels());
		labels[16]->setNum(decoder->getFifoOv());
		labels[17]->setNum(decoder->getLineOv());
		seekTo->setValue(frame);
	}
	else // if endof cpv is reached, then the current frame number is 0
	{
		if (timer->isActive())timer->stop();
		labels[15]->setText("end of file");
		labels[16]->setText("was reached");
		labels[17]->setText("have a good day ;)");
	}
	
	// find the particles
	if (cbParticle->isChecked())	
	{
		pf.findParticles(pixels,searchPerimeter->value(),hthreshold->value(),ithreshold->value(),mincenterI->value(),nthreshold->value());	
	}
	else
	{
		pf.clear();
	}
	update();
}



void DecoderWidget::rewind()
{
	decoder->seekToFirstFrame();
	decodeNextFrame();
}

void DecoderWidget::shot()
{
	QPixmap screenshot(decoder->getDimensions());
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Screenshot"),0,tr("Images (*.png *.xpm *.jpg)"));
	QRect window(QPoint(0,0),decoder->getDimensions());
	QPainter p(&screenshot);
	p.fillRect(window,QBrush(Qt::black)); // clear screen	
	paint(p);
	screenshot.save(fileName);
}

void DecoderWidget::seek()
{
	quint32 frame,frameTo;
	frame = decoder->getCurrentFrame();
	frameTo= seekTo->value();
	if (( frame == 0 ) || (frameTo < frame))
		decoder->seekToFirstFrame();
	else if (frameTo == frame)
		return;
	do 
	{
		frame = decoder->seekToNextFrame();
		labels[14]->setNum((int)frame);
	}
	while ((frame < (frameTo-1)) && frame);
	decodeNextFrame();
}

void DecoderWidget::openFile(const QString &fileName)
{
	if (decoder->openFile(fileName))
	{
		filename = fileName;
		QFileInfo fileinfo(filename);
		filepath = fileinfo.absolutePath();
		decoder->decodeHeader();
		labels[4]->setText(fileinfo.fileName());
		labels[5]->setNum(decoder->getVersion());
		labels[6]->setNum(decoder->getNumLines());
		labels[7]->setNum(decoder->getDimensions().width());
		labels[9]->setNum(decoder->getDimensions().height());
		labels[22]->setNum((double)decoder->getExposure()/1000.);
		labels[23]->setNum(1./(double)decoder->getRate()*1000000.);
		labels[24]->setNum(decoder->getGain());
		pixmap = new QPixmap(piclabel->size());
		decodeNextFrame();
		labels[25]->setNum(decoder->getThreshold());
		hthreshold->setValue(decoder->getThreshold());
		mincenterI->setValue((decoder->getThreshold()*2)%256);
		
		connect(seekTo,SIGNAL(editingFinished()),this,SLOT(seek()));
	// redraw whenever u change a parameter 
		connect(cbParticle,SIGNAL(released()),this,SLOT(reDecode()));
		connect(searchPerimeter,SIGNAL(editingFinished()),this,SLOT(reDecode()));
		connect(ithreshold,SIGNAL(editingFinished()),this,SLOT(reDecode()));
		connect(hthreshold,SIGNAL(editingFinished()),this,SLOT(reDecode()));
		connect(nthreshold,SIGNAL(editingFinished()),this,SLOT(reDecode()));
		connect(mincenterI,SIGNAL(editingFinished()),this,SLOT(reDecode()));
		connect(singleGdf,SIGNAL(clicked()),this,SLOT(writeGDF()));
		connect(allGdf,SIGNAL(clicked()),this,SLOT(writeGDFall()));
		connect(clusterGdf,SIGNAL(clicked()),this,SLOT(writeClusterJob()));
	}
	else
	{
		filename = "";
		QMessageBox::warning(0, tr("Application"), tr("Cannot read file %1") .arg(fileName));
	}
}

void DecoderWidget::mousePressEvent ( QMouseEvent * event )
{
	if (event->button() == Qt::LeftButton)
	{
		a = event->pos();
		a-=piclabel->pos();
	}
	if (event->button() == Qt::RightButton)
	{
		zoom = false;
		update();
	}
}
void DecoderWidget::mouseReleaseEvent ( QMouseEvent * event )
{
	if (event->button() == Qt::LeftButton)
	{
		b = event->pos();
		b-=piclabel->pos();
		if (b == a)
			return;
		zoom = true;
		update();
	}
}

QSize DecoderWidget::sizeHint () const
{
	return QSize(g5y*4/3+g5x,g5y);
}

//  << " " << 

void DecoderWidget::writeClusterJob()
{
	cpv2gdf c2g;
	c2g.writeGDFcluster(filename,searchPerimeter->value(),hthreshold->value(),ithreshold->value(),mincenterI->value(),nthreshold->value(),fpn->value());	
}

void DecoderWidget::writeGDFall()
{		
	// multithreaded
	if (!thread->isRunning())
		thread->start();
	else 
		QMessageBox::warning(this, tr("cpv2gdf converter"),
							tr("cpv2gdf conversion thread is still running!"),QMessageBox::Ok);
	update();
}

void DecoderWidget::writeGDF()
{
	cpv2gdf c2g;
	quint32 frame,frameTo;
	quint64 curpos,endpos;

	frame = decoder->getCurrentFrame();
	frameTo= framerange->value() + frame;
	curpos = decoder->getCurFilePosition();
	do 
	{
		frame = decoder->seekToNextFrame();
	}
	while ((frame < frameTo) && frame);
	endpos = decoder->getCurFilePosition();
// seek back to original framepos
	decoder->seekToPosition(curpos);
	// write gdf
	c2g.writeGDF(filename,searchPerimeter->value(),hthreshold->value(),ithreshold->value(),mincenterI->value(),nthreshold->value(),0,curpos,endpos);
	
	QMessageBox::information(this, tr("cpv2gdf converter"),
						tr("cpv2gdf conversion is done"),QMessageBox::Ok); // show messagebos
}

void DecoderWidget::gdfDone()
{
	QMessageBox::information(this, tr("cpv2gdf converter"),
					 tr("cpv2gdf conversion is done"),QMessageBox::Ok);
	
}

cpv2gdfThread::cpv2gdfThread(DecoderWidget *p) : parent(p){}

void cpv2gdfThread::run()
{
	cpv2gdf c2g;
	c2g.writeGDF(parent->filename,parent->searchPerimeter->value(),parent->hthreshold->value(),parent->ithreshold->value(),parent->mincenterI->value(),parent->nthreshold->value());
}


