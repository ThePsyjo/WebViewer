
/************************************************************************
 * configuration.cpp							*
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

#include "configuration.h"

ConfigHandler::ConfigHandler(QString fileLocation, QString fileName)
{
	QFileInfo i(fileLocation);
	if(! i.dir().exists())
	{
		QDir d;		// only create if not existing
		d.mkpath(i.path());
	}

	doc = new QDomDocument ( fileName );
	f = new QFile (fileLocation);
	timer = new QTimer(this);
	tmpE = new QDomElement;
	doSave = true;

	f->open( QIODevice::ReadOnly );

	QString errorStr;
	int errorLine;
	int errorColumn;

	// load if (file exist AND file is not empty)
	if (f->exists() && f->size() > 0 && !doc->setContent(f, true, &errorStr, &errorLine, &errorColumn))
	{
		QMessageBox::warning(NULL, tr("parse error"),
						tr("Parse error in file %4:\nat line %1, column %2:\n\"%3\"\n\nconfig will not be written")
						.arg(errorLine)
						.arg(errorColumn)
						.arg(errorStr)
						.arg(fileLocation));
		doSave = false;
	}

	if ( ! (doc->documentElement().tagName() == fileName) )
	{
		QDomElement root = doc->createElement(fileName);
		doc->appendChild(root);
		QMessageBox::information(NULL, "info", tr("%1 created in %2.").arg(fileName).arg(f->fileName()));
	}
	if(doSave)
	{
		f->remove(fileLocation + ".bak");
		f->copy(fileLocation + ".bak");
	}
	f->close();
	repairLinks();
};

void ConfigHandler::saveFile()
{
	if(doSave)
	{
		f->open( QIODevice::WriteOnly );
		QTextStream stream( f );
		doc->save(stream,3);
		f->close();
	}
}

ConfigHandler::~ConfigHandler()
{
	saveFile();
};

QDomElement ConfigHandler::genTag(QDomElement inMe, QString tagInMe)
{
	QDomElement tag = inMe.firstChildElement(tagInMe);
	if ( tag.tagName() != tagInMe )
	{
		inMe.appendChild(doc->createElement(tagInMe));
		tag = inMe.firstChildElement(tagInMe);
		tagCreated = true;
		saveFile();
	}
	else tagCreated = false;
	return tag;
}

QString ConfigHandler::loadStyleSheet()
{
	return genTag(doc->documentElement(), "Style").text();
}

void ConfigHandler::repairLinks()
{
	QDomNodeList list;
	QDomNode n;
	bool hit = true;
	list = genTag ( doc->documentElement(), "Links" ).childNodes();

	while(hit)
	{
		hit = false;
		for (cnt=0;cnt<list.size();cnt++)
		{
			if( ! list.at(cnt).toElement().hasAttribute("name"))
			{
				n = genTag(doc->documentElement(), "Links").appendChild(doc->createElement("Link"));
				n.toElement().setAttribute("data", list.at(cnt).toElement().attribute("data"));
				n.toElement().setAttribute("type", list.at(cnt).toElement().attribute("type"));
				n.toElement().setAttribute("name", list.at(cnt).nodeName());
				
				genTag(doc->documentElement(), "Links").removeChild(list.at(cnt));
				hit = true;
			}
		}
	}
}

bool ConfigHandler::findLink(QString name)
{
	QDomNodeList l;
	l = genTag ( doc->documentElement(), "Links" ).childNodes();
	bool found = false;

	for (cnt=0;cnt<l.size();cnt++)
	{
		if(l.at(cnt).toElement().attribute("name", "") == name)
		{
			*tmpE = l.at(cnt).toElement();
			found = true;
		}
	}
	if(found) return true;
	else return false;
}

QDomNode* ConfigHandler::findLinkR(QString name)
{
	findLink(name);
	return tmpE;
}

//WindowStyle//////////////////////////////////////////////////////////////////////
QString ConfigHandler::loadStyle()
{
	return genTag(doc->documentElement(), "WindowStyle").attribute("value", "default");
}

void ConfigHandler::saveStyle(QString s)
{
	genTag(doc->documentElement(), "WindowStyle").setAttribute("value", s);
}

//Bool-Operations//////////////////////////////////////////////////////////////////
void ConfigHandler::saveBool(QString tag, QString attribute, bool b)
{
	b ? genTag ( doc->documentElement(), tag ).setAttribute(attribute, 1) : genTag ( doc->documentElement(), tag ).setAttribute(attribute, 0);
}

bool ConfigHandler::loadBool(QString tag, QString attribute, QString defaultValue)
{
	return genTag ( doc->documentElement(), tag ).attribute(attribute, defaultValue).toInt();
}

//Visibility///////////////////////////////////////////////////////////////////////
//! Sichtbarkeit des Hauptfensters speichern.
void ConfigHandler::saveIsVisible(bool b)
{
	saveBool( "Options", "visible", b);
}

//! Sichtbarkeit des Hauptfensters laden.
bool ConfigHandler::loadIsVisible()
{
	return loadBool( "Options", "visible", "1");
}

//ontop////////////////////////////////////////////////////////////////////////////
//! Option 'immer im Vordergung' speichern.
void ConfigHandler::saveOntop(bool b)
{
	saveBool( "Options", "ontop", b);
}

//! Option 'immer im Vordergung' laden.
bool ConfigHandler::loadOntop()
{
	return loadBool( "Options", "ontop", "0");
}

//ShowTray////////////////////////////////////////////////////////////////////////
//! Option 'TraySymbol anzeigen' speichern.
void ConfigHandler::saveShowTray(bool b)
{
	saveBool( "Options", "tray_visible", b);
}

//! Option 'TraySymbol anzeigen' laden.
bool ConfigHandler::loadShowTray()
{
	return loadBool( "Options", "tray_visible", "1");
}

//CloseToTray//////////////////////////////////////////////////////////////////////
//! Option 'in SystemTray schließen' speichern.
void ConfigHandler::saveCloseToTray(bool b)
{
	saveBool( "Options", "close2tray", b);
}

//! Option 'in SystemTray schließen' laden.
bool ConfigHandler::loadCloseToTray()
{
	return loadBool( "Options", "close2tray", "1");
}

//widgetState//////////////////////////////////////////////////////////////////////
void ConfigHandler::saveState(QByteArray d)
{	
	//compression stats
	//qDebug() << d.toHex().size() << " +z " << qCompress(d).toHex().size();
	genTag ( doc->documentElement(), "WindowSettings" ).setAttribute("state", qCompress(d).toHex().data());
}

QByteArray ConfigHandler::loadState()
{
	QByteArray ba;
	ba = qUncompress(QByteArray::fromHex(QByteArray( genTag ( doc->documentElement(), "WindowSettings" ).attribute("state").toAscii())));
	if(ba.isEmpty())
		ba = QByteArray::fromHex(QByteArray( genTag ( doc->documentElement(), "WindowSettings" ).attribute("state").toAscii()));
	return ba;
}

//CloseToTrayTip///////////////////////////////////////////////////////////////////
//! Option 'in SystemTray schlieÃen - Tipp' speichern.
void ConfigHandler::saveCloseToTrayTip(bool b)
{
	saveBool( "Options", "close2trayTip", b);
}

//! Option 'in SystemTray schlieÃen - Tipp' laden.
bool ConfigHandler::loadCloseToTrayTip()
{
	return loadBool( "Options", "close2trayTip", "1");
}

//WidgetSize///////////////////////////////////////////////////////////////////////
void ConfigHandler::saveWinSize(QSize s)
{	
	genTag ( doc->documentElement(), "WindowSettings" ).setAttribute("height", s.rheight());
	genTag ( doc->documentElement(), "WindowSettings" ).setAttribute("width", s.rwidth());
}

QSize ConfigHandler::loadWinSize()
{
	return QSize (genTag ( doc->documentElement(), "WindowSettings" ).attribute("width").toInt(), genTag ( doc->documentElement(), "WindowSettings" ).attribute("height").toInt() );
}

//reloadInterval///////////////////////////////////////////////////////////////////
void ConfigHandler::saveReloadInterval(int i)
{
	genTag ( doc->documentElement(), "Options" ).setAttribute("reloadInterval", i);
}

int ConfigHandler::loadReloadInterval()
{
	return genTag ( doc->documentElement(), "Options" ).attribute("reloadInterval", "60").toInt();
}

//Links////////////////////////////////////////////////////////////////////////////

void ConfigHandler::createLink(QString name)
{
	QDomNode n = genTag(doc->documentElement(), "Links").appendChild(doc->createElement("Link"));
	n.toElement().setAttribute("name", name);
	findLink(name);
}

void ConfigHandler::saveLink(Link l)
{
	if(!findLink(l.name)) createLink(l.name);
	tmpE->toElement().setAttribute("name", l.name);
	tmpE->toElement().setAttribute("data", qCompress(l.url.toString().toAscii()).toHex().data());
	tmpE->toElement().setAttribute("type", qCompress(l.widgetType.toAscii()).toHex().data());
}
void ConfigHandler::saveLink(QString name, QUrl u, QString widgetType)
{
	if(!findLink(name)) createLink(name);
	tmpE->toElement().setAttribute("name", name);
	tmpE->toElement().setAttribute("data", qCompress(u.toString().toAscii()).toHex().data());
	tmpE->toElement().setAttribute("type", qCompress(widgetType.toAscii()).toHex().data());
}

void ConfigHandler::saveLink(QString name, QString url, QString userName, QString pass, QString widgetType)
{
	QUrl u = url;
	u.setUserInfo(QString("%1:%2").arg(userName).arg(pass));
	if(!findLink(name)) createLink(name);
	tmpE->toElement().setAttribute("name", name);
	tmpE->toElement().setAttribute("data", qCompress(u.toString().toAscii()).toHex().data());
	tmpE->toElement().setAttribute("type", qCompress(widgetType.toAscii()).toHex().data());
}

void ConfigHandler::loadLink(QList<Link> *l)
{
	QByteArray ba;
	QDomNodeList nl =  genTag ( doc->documentElement(), "Links" ).childNodes();
	Link link;

	l->clear();

	for (int i = 0; i < nl.count(); i++ )
	{

		link.name = nl.at(i).toElement().attribute("name");

		ba = qUncompress(QByteArray::fromHex(QByteArray( nl.at(i).toElement().attribute("data").toAscii())));
		link.url = QString::fromAscii(ba);
		
		ba = qUncompress(QByteArray::fromHex(QByteArray( nl.at(i).toElement().attribute("type").toAscii())));
		link.widgetType = QString::fromAscii(ba);

		l->insert(i, link);
	}
}

void ConfigHandler::delLink(QString s)
{
	genTag(doc->documentElement(), "Links").removeChild(findLinkR(s)->toElement());
}

//ZoomFactor///////////////////////////////////////////////////////////////////////

qreal ConfigHandler::loadZoomFactor(QString name)
{
	if(findLink(name))
		return tmpE->attribute("zoom", "1").toDouble();
	else
		return 1;
}

void ConfigHandler::saveZoomFactor(QString name, qreal val)
{
	if(findLink(name))
		tmpE->setAttribute("zoom", val);
}
