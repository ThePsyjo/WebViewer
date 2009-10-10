/************************************************************************
 * nagVisWidget.cpp                                                     *
 * Copyright (C) 2008,2009  Psyjo                                       *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 3 of the License,       *
 * or (at your option) any later version.                               *
 *                                                                      *
 * This program is distributed in the hope that it will be useful, but  *
 * WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                 *
 * See the GNU General Public License for more details.                 *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program; if not, see <http://www.gnu.org/licenses/>. *
 ************************************************************************/

#include "nagVisWidget.h"

NagVisWidget::NagVisWidget(QString name, QUrl u, ConfigHandler* c, QWidget * parent)
        : WebWidget(name, parent)
{
	config = c;
	urlString = u.toString(QUrl::RemoveUserInfo);
	un = u.userName();
	pa = u.password();
	webView->setZoomFactor(config->loadZoomFactor(objectName()));
}

NagVisWidget::~NagVisWidget()
{}

void NagVisWidget::reload()
{
	// set url including widgetsize
	*url = QString("%1&width=%2&height=%3").arg(urlString).arg(width()-15).arg(height()-25);
	url->setUserName(un);
	url->setPassword(pa);

	webView->load(*url);
}
