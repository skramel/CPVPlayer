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


//#include <QtGui>
#include <QtWidgets>
#include "cpvplayer.h"

#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>


cpvplayer::cpvplayer()
{

	mainWidget = new DecoderWidget();
	setCentralWidget(mainWidget);
	createActions();
     createMenus();
     createToolBars();
     createStatusBar();
     setCurrentFile("");
     setCurrentDir("c:\\");

}

void cpvplayer::closeEvent(QCloseEvent *event)
{
      event->accept();
}

void cpvplayer::open()
{
//	QString fileName = QFileDialog::getOpenFileName(this, "Select CPV file",QString(),"*.cpv");
//            if (!fileName.isEmpty()){
//                loadFile(fileName);
//                QFileDialog::setDirectory(QFileDialog::directory());
//            }



    QFileDialog f;
    QString fileName;
    QDir lastDir;

    fileName=f.getOpenFileName(this,"Select a CPV file",getCurrentDir(),"*.cpv");
    if(!fileName.isEmpty()){
        loadFile(fileName);
        lastDir=f.directory();
        QFileInfo fileInfo(fileName);
        setCurrentDir(fileInfo.absolutePath());
    }


}

void cpvplayer::about()
{
      QMessageBox::about(this,"About cpv player",
				"This is a player for cpv compressed videos.<br><br>"
				"(c) 2007, Thomas Glomann, Wesleyan University<br><br>"
				"This program is distributed under the terms of the GPL v2.");

}

void cpvplayer::createActions()
{
      openAct = new QAction(QIcon(":/fileopen.png"), tr("&Open..."), this);
      openAct->setShortcut(tr("Ctrl+O"));
      openAct->setStatusTip(tr("Open an existing file"));
      connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	 exitAct = new QAction(QIcon(":/exit.png"),tr("E&xit"), this);
      exitAct->setShortcut(tr("Ctrl+Q"));
      exitAct->setStatusTip(tr("Exit the application"));
      connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	 
	 nextAct = new QAction(QIcon(":/next.png"),tr("&Next frame"), this);
	 nextAct->setShortcut(tr("Ctrl+N"));
	 nextAct->setStatusTip(tr("decode next frame"));
	 
	 playAct = new QAction(QIcon(":/player_play.png"),tr("&Play"), this);
	 playAct->setShortcut(tr("Ctrl+P"));
	 playAct->setStatusTip(tr("Animate"));
	 
	 stopAct = new QAction(QIcon(":/player_stop.png"),tr("&Stop"), this);
	 stopAct->setShortcut(tr("Ctrl+P"));
	 stopAct->setStatusTip(tr("Stop Animation"));
	 
	 rewindAct = new QAction(QIcon(":/player_start.png"),tr("&Rewind"), this);
	 rewindAct->setShortcut(tr("Ctrl+R"));
	 rewindAct->setStatusTip(tr("Rewind to beginning"));
	 	 
	 seekAct = new QAction(QIcon(":/seek.png"),tr("S&eek"), this);
	 seekAct->setShortcut(tr("Ctrl+E"));
	 seekAct->setStatusTip(tr("Seek to frame"));

	 shotAct = new QAction(QIcon(":/ss.png"),tr("&Screenshot"), this);
	 shotAct->setShortcut(tr("Ctrl+S"));
	 shotAct->setStatusTip(tr("take a screenshot of current frame"));
	 
	 aboutAct = new QAction(tr("&About"), this);
      aboutAct->setStatusTip(tr("Show the application's About box"));
      connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

      aboutQtAct = new QAction(tr("About &Qt"), this);
      aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
      connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}

void cpvplayer::createMenus()
{
      fileMenu = menuBar()->addMenu(tr("&File"));
      fileMenu->addAction(openAct);
      fileMenu->addSeparator();
      fileMenu->addAction(exitAct);

      menuBar()->addSeparator();

      helpMenu = menuBar()->addMenu(tr("&Help"));
      helpMenu->addAction(aboutAct);
      helpMenu->addAction(aboutQtAct);
}

void cpvplayer::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(exitAct);

	editToolBar = addToolBar(tr("Player"));
	editToolBar->addAction(rewindAct);
	editToolBar->addAction(nextAct);
	editToolBar->addAction(playAct);
	editToolBar->addAction(stopAct);
	editToolBar->addAction(seekAct);
	editToolBar->addAction(shotAct);
	
}

void cpvplayer::createStatusBar()
{
      statusBar()->showMessage(tr("Ready"));
}

void cpvplayer::loadFile(const QString &fileName)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);	
	mainWidget->openFile(fileName);
	connect(nextAct, SIGNAL(triggered()), mainWidget, SLOT(decodeNextFrame()));
	connect(playAct, SIGNAL(triggered()), mainWidget->timer, SLOT(start()));
	connect(stopAct, SIGNAL(triggered()), mainWidget->timer, SLOT(stop()));
	connect(rewindAct, SIGNAL(triggered()), mainWidget, SLOT(rewind()));
	connect(seekAct, SIGNAL(triggered()), mainWidget, SLOT(seek()));
	connect(shotAct, SIGNAL(triggered()), mainWidget, SLOT(shot()));
     QApplication::restoreOverrideCursor();
     setCurrentFile(fileName);
     statusBar()->showMessage(tr("File loaded"), 2000);
	// skip first frame automatically
	mainWidget->decodeNextFrame();
}

void cpvplayer::setCurrentFile(const QString &fileName)
{
      curFile = fileName;
      setWindowModified(false);

      QString shownName;
      if (curFile.isEmpty())
            shownName = "please load a cpv file first";
      else
            shownName = strippedName(curFile);

      setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("CPV Player")));
}
void cpvplayer::setCurrentDir(const QString &dirName){
    curDir=dirName;
}
QString cpvplayer::getCurrentDir(){
    return curDir;
}

QString cpvplayer::strippedName(const QString &fullFileName)
{
      return QFileInfo(fullFileName).fileName();
}

cpvplayer::~cpvplayer()
{

}
