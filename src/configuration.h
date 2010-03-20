
/************************************************************************
 * configuration.h							*
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QtXml>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QByteArray>
#include <QList>
#include <QDomNodeList>
#include <QUrl>

// containerclass for links
class Link
{
public:
	QUrl url;
	QString name, widgetType;
};

class ConfigHandler : public QObject
{
Q_OBJECT
private:
	QDomDocument *doc;
	QFile *f;
	QDomElement genTag(QDomElement, QString);
	QDomElement *tmpE;
	bool tagCreated, doSave, change;
	QTimer *timer;
	void saveBool(QString, QString, bool);
	bool loadBool(QString, QString, QString);
	bool findLink(QString);
	QDomNode* findLinkR(QString);
	int cnt;
	void repairLinks();
	void createLink(QString);
public:
	ConfigHandler(QString, QString);
	virtual ~ConfigHandler();

	QString loadStyleSheet();

	QString loadStyle();
	void saveStyle(QString);
	
	bool loadIsVisible();
	void saveIsVisible(bool);

	bool loadOntop();
	void saveOntop(bool);

	bool loadShowTray();
	void saveShowTray(bool);

	bool loadCloseToTray();
	void saveCloseToTray(bool);

	void saveState(QByteArray);
	QByteArray loadState();

	void saveReloadInterval(int);
	int loadReloadInterval();

	void saveLink(Link);
	void saveLink(QString, QUrl, QString);
	void saveLink(QString, QString, QString, QString, QString);
	void loadLink(QList<Link>*);
	void delLink(QString);

	bool loadCloseToTrayTip();
	void saveCloseToTrayTip(bool);

	QSize loadWinSize();
	void saveWinSize(QSize);

	qreal loadZoomFactor(QString);
	void saveZoomFactor(QString, qreal);

public slots:
	void saveFile();
};

#endif

