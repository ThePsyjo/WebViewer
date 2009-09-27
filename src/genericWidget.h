/************************************************************************
 * gegericWidget.h                                                      *
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

#ifndef GENERICWIDGET_H
#define GENERICWIDGET_H

#include "webWidget.h"

class GenericWidget : public WebWidget
{
Q_OBJECT
public:
	GenericWidget(QString, QUrl, QWidget * parent = 0);
	~GenericWidget();
public slots:
	void reload();
};

#endif
