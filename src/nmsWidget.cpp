/************************************************************************
 * nmsWidget.cpp                                                        *
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

#include "nmsWidget.h"

NmsWidget::NmsWidget(QString name, QUrl u, QWidget * parent)
        : WebWidget(name, parent)
{
	*url = u;
}

NmsWidget::~NmsWidget()
{}

void NmsWidget::reload()
{
//	if(! conf->loadUserName().isEmpty())
//		url->setUserName(conf->loadUserName());
//	if(! conf->loadPassword().isEmpty())
//		url->setPassword(conf->loadPassword());
	webView->load(*url);
}
