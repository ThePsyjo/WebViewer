
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
	bool tagCreated, doSave, change;
	QTimer *timer;
	void saveBool(QString, QString, bool);
	bool loadBool(QString, QString, QString);
public:
	ConfigHandler(QString, QString);
	virtual ~ConfigHandler();

	QString loadStyleSheet();

	QString loadStyle();
	void saveStyle(QString);
	
	bool loadIsVisible();
	void saveIsVisible(bool);

	void saveState(QByteArray);
	QByteArray loadState();

	void saveUserName(QString);
	QString loadUserName();

	void savePassword(QString);
	QString loadPassword();

	void saveReloadInterval(int);
	int loadReloadInterval();

	void saveLink(Link);
	void saveLink(QString, QUrl, QString);
	void saveLink(QString, QString, QString, QString, QString);
	void loadLink(QList<Link>*);
	void delLink(QString);

public slots:
	void saveFile();
};

#endif

