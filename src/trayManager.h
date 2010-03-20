
/************************************************************************
 * trayManager.h							*
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

#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H

#include <QSystemTrayIcon>
#include <QMap>
#include <QStack>
#include "configuration.h"

/*!
 * \brief Containerklasse für Systemtraypopups.
 */
class MessageData
{
public:
	QString title, message;
	QSystemTrayIcon::MessageIcon icon;
	int time;
};

/*!
 * \brief WrapperKlasse um SystemTrayPopups zu stapeln und nacheinander auszugeben.
 */
class TrayManager : public QObject
{
Q_OBJECT
public:
	TrayManager(ConfigHandler*, QSystemTrayIcon*);
	virtual ~TrayManager();
private:
	QSystemTrayIcon *trayIcon;
	ConfigHandler *config;
	QStack<MessageData> *messageStack;
	void checkMessageStack();
	bool busy;
private slots:
	void freeStack();
public slots:
	void showMessage(QString, QString, QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information, int time = 10000 );
};

#endif
