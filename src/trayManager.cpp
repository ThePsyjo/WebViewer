
/************************************************************************
 * trayManager.cpp							*
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

#include "trayManager.h"

//! Standard Konstruktor.
TrayManager::TrayManager(ConfigHandler* c, QSystemTrayIcon* ico)
{
	trayIcon = ico;
	config = c;
	busy = false;

	messageStack = new QStack<MessageData>;
	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(freeStack()));
}

//! Standard Destruktor.
TrayManager::~TrayManager(){}

//! schiebt die Nachricht auf den internen messageStack.
void TrayManager::showMessage(QString title, QString message, QSystemTrayIcon::MessageIcon icon , int time )
{
	MessageData d;
	d.title = title;
	d.message = message;
	d.icon = icon;
	d.time = time;

	messageStack->push(d);
	checkMessageStack();
}

//! Ruft die aktuellste Nachricht vom Stack ab uns stellt sie dar.
void TrayManager::checkMessageStack()
{
	if(!busy)
	{
		if(! messageStack->isEmpty())
		{
			busy = true;
			MessageData d;
			d = messageStack->pop();
			trayIcon->showMessage(d.title, d.message, d.icon, d.time);
			QTimer::singleShot(d.time, this, SLOT(freeStack()));
		}
	}
}

//! Stellt busy auf false und ruft wieder checkMessageStack() auf.
void TrayManager::freeStack()
{
	busy = false;
	checkMessageStack();
}
