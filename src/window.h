
/************************************************************************
 * window.h								*
 * Copyright (C) 2008,2009  Psyjo					*
 *									*
 * This program is free software; you can redistribute it and/or modify	*
 * it under the terms of the GNU General Public License as published by	*
 * the Free Software Foundation; either version 3 of the License,	*
 * or (at your option) any later version.				*
 *									*
 * This program is distributed in the hope that it will be useful, but	*
 * WITHOUT ANY WARRANTY; without even the implied warranty of		*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.			*
 * See the GNU General Public License for more details.			*
 *									*
 * You should have received a copy of the GNU General Public License	*
 * along with this program; if not, see <http://www.gnu.org/licenses/>. *
 ************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QTimer>
#include <QAction>
#include <QMenuBar>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QStatusBar>
#include <QProgressBar>
#include <QStack>
#include <QStyleFactory>
#include <QInputDialog>
#include <QMap>
#include <QList>
#include "nagVisWidget.h"
#include "nmsWidget.h"
#include "genericWidget.h"
#include "configuration.h"
#include "linkInput.h"
#include "trayManager.h"

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
	MainWindow( QWidget * parent =0, Qt::WFlags f =0 );
	virtual ~MainWindow();
protected:
        void closeEvent ( QCloseEvent *event );
private:
	QMap<QString, QString> *credMap;
	ConfigHandler *config;
	bool reloadStackEmpty;
	QWidget *dummy;
	QStack<WebWidget*> *reloadStack;
	QTimer *reloadTimer, *statTimer;
	QAction *ontopAction, *showTrayAction, *closeToTrayAction, *reloadAction;
	QMenu *mAction, *trayIconMenu, *mAbout;
	QSystemTrayIcon *trayIcon;
	TrayManager *trayMgr;
	QProgressBar *pBar;
	QStatusBar *statusBar;
	QMenu *mOption, *mStyle;
	void pushToReload(WebWidget*);
	QList<Link> *links;
	QList<WebWidget*> *widgetList;
	void createWidgets();
	void createWidget(int);
	void delWidget(int);
	bool created;
private slots:
	void reload();
	void popReload();
	void handleTrayIcon(QSystemTrayIcon::ActivationReason);
	void handleExit();
	void stat();
	void onStyleMenu(QAction*);
	void handleReloadTime();
	void handleNewLink();
	void handleAboutAppAction();
	void handleAboutQtAction();
	void onOntopAction(bool);
	void onShowTrayAction(bool);
	void onCloseToTrayAction(bool);
	void handleMinimizedTip();
	void disconnectMinimizedTip();
};

#endif

