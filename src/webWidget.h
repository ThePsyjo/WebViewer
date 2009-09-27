/************************************************************************
 * webWidget.h                                                          *
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

#ifndef WEBWIDGET_H
#define WEBWIDGET_H

#include <QDockWidget>
#include <QWebView>
#include <QUrl>

class WebWidget : public QDockWidget
{
Q_OBJECT
public:
	WebWidget(QString, QWidget * parent = 0);
	~WebWidget();
protected:
	QWebView *webView;
	QUrl *url;
public slots:
	void webViewDone(bool);
	virtual void reload(){};
signals:
	void done();
};

#endif
