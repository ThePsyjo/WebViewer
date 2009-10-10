/************************************************************************
 * webWidget.cpp                                                        *
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

#include "webWidget.h"

WebWidget::WebWidget(QString name, QWidget * parent)
        : QDockWidget(name, parent)
{
	setObjectName(name);

	webView = new QWebView(this);
	connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(webViewDone(bool)));

	url = new QUrl;

	setWidget(webView);

//	webView->setZoomFactor(config->loadZoomFactor(objectName()));
}

void WebWidget::keyPressEvent(QKeyEvent* e)
{
	if( e->text() == "+" || e->text() == "-" || e->text() == "0" )
	{
		if( e->text() == "+" ) webView->setZoomFactor(webView->zoomFactor() + .05);
		else
		if( e->text() == "-" ) webView->setZoomFactor(webView->zoomFactor() - .05);
		else
		if( e->text() == "0" ) webView->setZoomFactor(1);

		config->saveZoomFactor(objectName(), webView->zoomFactor());
	}
}

WebWidget::~WebWidget()
{}

void WebWidget::webViewDone(bool ok)
{
	emit done();
}
