
/************************************************************************
 * window.cpp								*
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

#include "window.h"

MainWindow::MainWindow( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{
	setWindowTitle(QApplication::applicationName() +" "+ QApplication::applicationVersion());

	config = new ConfigHandler(QDir::toNativeSeparators(QDir::homePath ()  + "/.w3v/w3v.xml"), "WWWebViewer_configuration");

        setStyle(QStyleFactory::create(config->loadStyle()));
        if(! config->loadStyleSheet().isEmpty())
                setStyleSheet(config->loadStyleSheet());
        else    setStyleSheet("* {}"); // without any stylesheet, windowstyles won't apply
	
	setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs);

	dummy = new QWidget(this);
	setCentralWidget(dummy);

	links = new QList<Link>;
	config->loadLink(links);
	widgetList = new QList<WebWidget*>;

	createWidgets();

	reloadStack = new QStack<WebWidget*>;
	reloadStackEmpty = true;

	pBar = new QProgressBar(this);
	pBar->setRange(0,config->loadReloadInterval());
	pBar->setFormat("%v Sekunden");

	statusBar = new QStatusBar(this);
	setStatusBar(statusBar);
	statusBar->addWidget(pBar);

	reloadTimer = new QTimer(this);
	reloadTimer->start(config->loadReloadInterval() * 1000);
	connect(reloadTimer, SIGNAL(timeout()), this, SLOT(reload()));

	statTimer = new QTimer(this);
	statTimer->start(1000); // 1 sec
	connect(statTimer, SIGNAL(timeout()), this, SLOT(stat()));

	mAction = menuBar()->addMenu(tr("&Action"));
	mAction->addAction(tr("&update"), this, SLOT(reload()), Qt::Key_F5);
	mAction->addAction(tr("e&xit"), this, SLOT(handleExit()), Qt::CTRL + Qt::Key_Q);

	mStyle = new QMenu(tr("&Style"), this);
        foreach(QString s, QStyleFactory::keys())       // fill in all available Styles
		mStyle->addAction(s);                   //
	connect(mStyle, SIGNAL(triggered(QAction*)), this, SLOT(onStyleMenu(QAction*)));

	mOption = menuBar()->addMenu(tr("&Options"));
	mOption->addAction(tr("reloadinterval"), this, SLOT(handleReloadTime()), Qt::Key_F8);
	mOption->addAction(tr("add link"), this, SLOT(handleNewLink()), Qt::Key_F6);
	mOption->addSeparator();

	ontopAction = new QAction(tr("always on &top"), this);
	showTrayAction = new QAction(tr("show tray &icon"), this);
	closeToTrayAction = new QAction(tr("close to &tray"), this);

	ontopAction->setCheckable(true);
	showTrayAction->setCheckable(true);
	closeToTrayAction->setCheckable(true);

	showTrayAction->setChecked(config->loadShowTray());
	ontopAction->setChecked(config->loadOntop());
	closeToTrayAction->setChecked(config->loadCloseToTray());

	connect(ontopAction, SIGNAL(toggled(bool)), this, SLOT(onOntopAction(bool)));
	connect(showTrayAction, SIGNAL(toggled(bool)), this, SLOT(onShowTrayAction(bool)));
	connect(closeToTrayAction, SIGNAL(toggled(bool)), this, SLOT(onCloseToTrayAction(bool)));
	
	mOption->addAction(ontopAction);
	mOption->addAction(showTrayAction);
	mOption->addAction(closeToTrayAction);
	mOption->addSeparator();
	mOption->addMenu(mStyle);

	trayIcon = new QSystemTrayIcon(QIcon(":/appicon"), this);
	trayMgr = new TrayManager (config, trayIcon);
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleTrayIcon(QSystemTrayIcon::ActivationReason)));
	trayIcon->show();

	trayIconMenu = new QMenu;
	trayIconMenu->addAction(tr("exit"), this, SLOT(handleExit()));
	trayIcon->setContextMenu(trayIconMenu);

	mAbout = menuBar()->addMenu(tr("&about"));
	mAbout->addAction(QApplication::applicationName(), this,  SLOT(handleAboutAppAction()));
	mAbout->addAction("Qt", this,  SLOT(handleAboutQtAction()));

	restoreState(config->loadState());

	resize(config->loadWinSize());
	show();
	reload();
}

void MainWindow::onStyleMenu(QAction* a)
{
	QApplication::setStyle(QStyleFactory::create(a->text()));
	setStyle(QStyleFactory::create(a->text()));
	config->saveStyle(a->text());
}

void MainWindow::pushToReload(WebWidget* w)
{
	if(w->isVisible())
		reloadStack->push(w);
}

void MainWindow::reload()
{
	for (int i = 0; i < links->count(); i++ )
		if(widgetList->at(i) != NULL)
			pushToReload(widgetList->at(i));

	statusBar->showMessage(QString("noch %1...").arg(reloadStack->count()));
		
	if(reloadStackEmpty)
		popReload();
	reloadStackEmpty = false; // lock here on first call, release below

	pBar->setValue(config->loadReloadInterval());
	reloadTimer->start(config->loadReloadInterval()*1000);
}

void MainWindow::popReload()
{
	reloadStack->count() > 0
		? statusBar->showMessage(QString("noch %1...").arg(reloadStack->count()))
		: statusBar->clearMessage();

	if (reloadStack->count() > 0)
		reloadStack->pop()->reload();
	else	reloadStackEmpty = true; // release lock if queue is empty again
}

void MainWindow::stat()
{
	pBar->setValue(pBar->value()-1);
}

void MainWindow::handleTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
        switch(reason)
        {
                case QSystemTrayIcon::Trigger:
                        if(isVisible())
			{
				config->saveWinSize(size());
                                hide();
			}
                        else
			{
                                show();
				resize(config->loadWinSize());
			}
                        break;
                case QSystemTrayIcon::Context:
                        break;
                case QSystemTrayIcon::DoubleClick:
                        break;
                case QSystemTrayIcon::MiddleClick:
                        break;
                case QSystemTrayIcon::Unknown:
                        break;
        }
}

void MainWindow::handleExit()
{
	config->saveWinSize(size());
	QApplication::exit();
}

//! Reaktion auf Fentser-schließen-X, ALT+F4 o.Ä.
void MainWindow::closeEvent ( QCloseEvent *event )
{
	config->saveWinSize(size());
	// tray is visible -> close to tray
	// else close app
	if(trayIcon->isVisible())
		event->accept();
	else
	{
		QApplication::exit();
		return;
	}
	// if close-to-tray is set, do so
	if(config->loadCloseToTray())
		event->accept();
	else
	{
		QApplication::exit();
		return;
	}
	// save this state
	trayIcon->isVisible() ? config->saveIsVisible(false) : config->saveIsVisible(true);

	if(config->loadCloseToTray() && trayIcon->isVisible())
	{
		if(config->loadCloseToTrayTip())
		{
			connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(handleMinimizedTip()));
			// watch for click on message
			QTimer::singleShot(10000, this, SLOT(disconnectMinimizedTip()));
			// disconnect watch after 10 secs
			trayMgr->showMessage("", tr("%1 minimized.").arg(QApplication::applicationName()));
		}
	}
}

void MainWindow::handleReloadTime()
{
	bool ok;
	int tmp = QInputDialog::getInteger(this,
					"Nachladeintervall", // title
					"Zeit in Sekunden eingeben", // text
					config->loadReloadInterval(), // default
					10, // min
					86400, //at least once a day
					1, // step
					&ok);
	if(ok)
	{
		config->saveReloadInterval(tmp);
		pBar->setRange(0,config->loadReloadInterval());
		reload();
	}
}

void MainWindow::handleNewLink()
{
	LinkInput in(config, this);
	if(in.exec())
	{	
		config->loadLink(links);
		bool found;
		while (widgetList->count() != links->count())
		{
			// qDebug() << widgetList->count()  << " <- wl | l -> " << links->count() ;
			/////////// 
			/*
		        QStringList llist;
		        foreach(Link l, *links)
		                llist << l.name;
		        QStringList wlist;
		        foreach(WebWidget *w, *widgetList)
				wlist << w->objectName();
			qDebug () << "WL - " << llist;
			qDebug () << "links - " << wlist;
			*///////////////
		
			for (int i = 0; i < widgetList->count(); i++)
			{
				found = false;
				for (int j = 0; j < links->count(); j++)
				{
		//			qDebug() << "d " << links->at(j).name << "==" << widgetList->at(i)->objectName();
					if( links->at(j).name == widgetList->at(i)->objectName() )
					//if( links->count() > i && links->at(j).name == widgetList->at(i)->objectName() )
					{
						found = true;
						break;
					}
				}
				if (!found) delWidget(i);
			}
	
			///////////
		        /*
			llist.clear();
			        foreach(Link l, *links)
				                llist << l.name;
		        wlist.clear();
			        foreach(WebWidget *w, *widgetList)
				                wlist << w->objectName();
			qDebug () << "WL - " << llist;
			qDebug () << "links - " << wlist;
			*/////////////
	
			for (int i = 0; i < links->count(); i++)
			{
				found = false;
				for (int j = 0; j < widgetList->count(); j++)
				{
	//				qDebug() << "c "<< links->at(i).name << "==" << widgetList->at(j)->objectName();
					if( links->at(i).name == widgetList->at(j)->objectName() )
					{
						found = true;
						break;
					}
				}	
				if (!found) createWidget(i);
			}
			///////////
			/*
			llist.clear();
			foreach(Link l, *links)
				llist << l.name;
	        	wlist.clear();
				foreach(WebWidget *w, *widgetList)
			wlist << w->objectName();
			qDebug () << "WL - " << llist;
			qDebug () << "links - " << wlist;
			*/////////////
	
		} // while
	} // if
}

void MainWindow::delWidget(int i)
{
	removeDockWidget(widgetList->at(i));
	widgetList->at(i)->deleteLater();
	widgetList->removeAt(i);
}

void MainWindow::createWidget(int pos)
{
	created = false;
	if (links->at(pos).widgetType == "NagVisWidget" )
	{
		widgetList->insert(pos, new NagVisWidget (links->at(pos).name, links->at(pos).url, config, this));
		created = true;
	}
	if (links->at(pos).widgetType == "NmsWidget" )
	{
		widgetList->insert(pos, new NmsWidget (links->at(pos).name, links->at(pos).url, config, this));
		created = true;
	}
	if (links->at(pos).widgetType == "generic" )
	{
		widgetList->insert(pos, new GenericWidget (links->at(pos).name, links->at(pos).url, config, this));
		created = true;
	}

	if(created)
	{
		addDockWidget(Qt::TopDockWidgetArea, widgetList->at(pos));
		widgetList->at(pos)->setObjectName(links->at(pos).name);
		connect(widgetList->at(pos), SIGNAL(done()), this, SLOT(popReload()));
	}
}


void MainWindow::createWidgets()
{
	for (int i = 0; i < links->count(); i++ )
		createWidget(i);
}

void MainWindow::handleAboutAppAction()
{
	QMessageBox::about( this, tr("about"), tr("<html>%1 %2<br><br>Copyright (C) 2009 Psyjo<br><br><br>This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.<br><br>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.<br>See the GNU General Public License for more details.<br><br>You should have received a copy of the GNU General Public License along with this program; if not, see <a href=\"http://www.gnu.org/licenses/\">this link</a>.</html>").arg(QApplication::applicationName()).arg(QApplication::applicationVersion()));
}

void MainWindow::handleAboutQtAction()
{
        QMessageBox::aboutQt ( this, tr("about"));
}

void MainWindow::onOntopAction(bool b)
{
	b ? setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint) : setWindowFlags(Qt::Dialog);
	setWindowIcon(QIcon(":/appicon"));
	show();
	config->saveOntop(b);
}

void MainWindow::onShowTrayAction(bool b)
{
	b ? trayIcon->show() : trayIcon->hide();
	config->saveShowTray(b);
}

void MainWindow::onCloseToTrayAction(bool b)
{
	config->saveCloseToTray(b);
}
//
//! HilfsSlot
void MainWindow::disconnectMinimizedTip() // is needed as slot, see top function
{
//	disconnect(trayIcon, SIGNAL(messageClicked()), 0, 0);
	disconnect(trayIcon, 0, this, 0);
}

//! Reaktion auf klick auf Minimiert-Tipp
void MainWindow::handleMinimizedTip()
{
	// buttons need to be localized
	QMessageBox(	QMessageBox::Question,
			tr("tip"),
			tr("show this tip further ?"),
			QMessageBox::Yes | QMessageBox::No,
			this)
	.exec() == QMessageBox::No ? config->saveCloseToTrayTip(0) : config->saveCloseToTrayTip(1);
	disconnectMinimizedTip();
}

MainWindow::~MainWindow()
{
	config->saveState(saveState());
	delete config;
}
