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


#ifndef CPVPLAYER_H
#define CPVPLAYER_H

#include <QMainWindow>
#include <QCloseEvent>
//#include <QtGui>
#include <QtWidgets>
#include<QDir>
// #include "decoder.h"
#include "decoderwidget.h"

class QAction;
class QMenu;
class Decoder;
class PicWidget;

class cpvplayer:public QMainWindow
{
      Q_OBJECT

public:
      cpvplayer();
      ~cpvplayer();



protected:
      void closeEvent(QCloseEvent *event);

private slots:
      void open();
      void about();

private:
      void createActions();
      void createMenus();
      void createToolBars();
      void createStatusBar();
      void loadFile(const QString &fileName);
      void setCurrentFile(const QString &fileName);
      QString strippedName(const QString &fullFileName);
      void setCurrentDir(const QString &dirName);
      QString getCurrentDir();

      QString curFile;
      QString curDir;
     DecoderWidget *mainWidget;
	QMenu *fileMenu;
      QMenu *editMenu;
      QMenu *helpMenu;
      QToolBar *fileToolBar;
      QToolBar *editToolBar;
      QAction *openAct;
	 QAction *stopAct;
	 QAction *playAct;
	 QAction *nextAct;
	 QAction *rewindAct;
      QAction *exitAct;
      QAction *cutAct;
      QAction *aboutAct;
      QAction *aboutQtAct;
	 QAction *seekAct;
	 QAction *shotAct;
};

#endif
