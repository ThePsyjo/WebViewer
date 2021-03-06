/************************************************************************
 * genericWidget.cpp                                                    *
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

#include "genericWidget.h"

GenericWidget::GenericWidget(QString name, QUrl u, ConfigHandler* c, QWidget * parent)
        : WebWidget(name, parent)
{
	config = c;
	*url = u;
	webView->load(u);
	webView->setZoomFactor(config->loadZoomFactor(objectName()));
}

GenericWidget::~GenericWidget()
{}

void GenericWidget::reload()
{
//	if(! conf->loadUserName().isEmpty())
//		url->setUserName(conf->loadUserName());
//	if(! conf->loadPassword().isEmpty())
//		url->setPassword(conf->loadPassword());
	webView->reload();
}
