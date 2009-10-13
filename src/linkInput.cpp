
/************************************************************************
 * linkInput.cpp							*
 * Copyright (C) 2009  Psyjo						*
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

#include "linkInput.h"

///////////////////////////////////////////////////
//StringInput//////////////////////////////////////
///////////////////////////////////////////////////

StringInput::StringInput(QStringList l, QWidget* parent)
{
	setParent(parent);
	setWindowFlags(Qt::Dialog);
	setWindowTitle(tr("new Widget"));
	setModal(1);
	list = l;

	QRegExp rx(".*");
	//QRegExp rx("^[a-zA-Z][a-zA-Z0-9-_.]*");
	validator = new QRegExpValidator(rx, this);
	e = new QLineEdit(this);
	e->show();
	e->setValidator(validator);

	okButton = new QPushButton("&OK", this);
	cancelButton = new QPushButton(tr("&cancel"), this);
	connect(okButton,	SIGNAL(clicked()), this, SLOT(onOkClick()));
	connect(cancelButton,	SIGNAL(clicked()), this, SLOT(reject()));  
	layout = new QGridLayout(this);

	layout->addWidget(e, 0,0,1,2);
	layout->addWidget(okButton, 1,0);
	layout->addWidget(cancelButton, 1,1);
}

void StringInput::onOkClick()
{
	if(e->text().isEmpty())
	{
		e->setText(tr("insert_widgetname_here"));
		e->selectAll();
	}
	else
	{
		foreach(QString s, list)
			if(s == e->text())
			{
				e->setText(tr("name_exists"));
				e->selectAll();
				return;
			}
		val = e->text();
		accept();
	}
}

QString StringInput::getVal()
{
	return val;
}

StringInput::~StringInput(){};

///////////////////////////////////////////////////

///////////////////////////////////////////////////
//LinkInput////////////////////////////////////////
///////////////////////////////////////////////////

LinkInput::LinkInput(ConfigHandler* c, QWidget* parent)
{
	setParent(parent);
	setWindowFlags(Qt::Dialog);
	setWindowTitle(tr("manage links"));
	setModal(1);

	config = c;

	links = new QList<Link>;
	c->loadLink(links);

	typeSelect = new QComboBox (this);
	QStringList list;
	list << "generic" << "NmsWidget" << "NagVisWidget";
	typeSelect->addItems(list);

	nameSelect = new QComboBox(this);
	foreach (Link l, *links)
		nameSelect->addItem(l.name);

	connect(nameSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(onNameSelectChange()));

	lUserName = new QLabel(tr("userName"), this);
	lPass = new QLabel(tr("password"), this);
	lUrl = new QLabel(tr("Url"), this);
	lType = new QLabel(tr("widgetType"), this);
	lWidgetName = new QLabel(tr("widgetName"), this);

        name = new QLineEdit(this);
        pass = new QLineEdit(this);
        pass->setEchoMode(QLineEdit::Password);
        urlEdit = new QLineEdit();

	connect(typeSelect, SIGNAL(activated(int)),this,SLOT(handleChange()));
	connect(urlEdit, SIGNAL(textEdited(QString)),this,SLOT(handleChange()));
	connect(name, SIGNAL(textEdited(QString)),this,SLOT(handleChange()));
	connect(pass,SIGNAL(textEdited(QString)),this,SLOT(handleChange()));

	newButton = new QPushButton(tr("&new"), this);
	delButton = new QPushButton(tr("&del"), this);
	savButton = new QPushButton(tr("&save"), this);
	finButton = new QPushButton(tr("&finish"), this);

	defFont = new QFont();

	connect(newButton, SIGNAL(clicked()), this, SLOT(onNewClick()));
	connect(delButton, SIGNAL(clicked()), this, SLOT(onDelClick()));  
	connect(savButton, SIGNAL(clicked()), this, SLOT(onSavClick()));
	connect(finButton, SIGNAL(clicked()), this, SLOT(onFinClick()));

	layout = new QGridLayout(this);

	layout->addWidget(lWidgetName,	0, 0);
	layout->addWidget(lType,	2, 0);
	layout->addWidget(lUrl,		3, 0);
	layout->addWidget(lUserName,	4, 0);
	layout->addWidget(lPass,	5, 0);

	layout->addWidget(nameSelect, 0, 1);
	layout->addWidget(typeSelect, 2, 1);
	layout->addWidget(urlEdit   , 3, 1);
	layout->addWidget(name	    , 4, 1);
	layout->addWidget(pass	    , 5, 1);

	layout->addWidget(newButton, 1, 2);
	layout->addWidget(delButton, 2, 2);
	layout->addWidget(savButton, 3, 2);
	layout->addWidget(finButton, 4, 2);

	setMinimumWidth(500);
}

void LinkInput::handleChange()
{
	savButton->setFont(QFont(defFont->defaultFamily(), -1, QFont::Bold));
//	savButton->setFocus(Qt::OtherFocusReason); // no good idea
}

void LinkInput::onSavClick()
{
	Link l;
/*	if(name->text().isEmpty())
	{
		name->setText(tr("insert name here"));
		name->selectAll();
	}
	else if(pass->text().isEmpty())
	{
		pass->setText(tr("insert password here"));
		pass->selectAll();
	}
	else*/ if (urlEdit->text().isEmpty())
	{
		urlEdit->setText(tr("insert URL here"));
		urlEdit->selectAll();
	}
	else
	{
		l.url.setUrl(urlEdit->text());
		l.url.setUserName(name->text());
		l.url.setPassword(pass->text());
		l.widgetType = typeSelect->currentText();
		l.name = nameSelect->currentText();

		config->saveLink(l);
		config->loadLink(links);
		savButton->setFont(QFont());
	}
}

void LinkInput::onNewClick()
{
	StringInput in(genNameList(), this);
	if(in.exec())
	{
		name->clear();
		pass->clear();
		urlEdit->clear();
		nameSelect->addItem(in.getVal());
		nameSelect->setCurrentIndex(nameSelect->findText(in.getVal()));
	}
}

void LinkInput::onDelClick()
{
	if(	QMessageBox(    QMessageBox::Question,
				tr("del_link"),
				tr("ask_del_link %1 ?").arg(nameSelect->currentText()),
				QMessageBox::Yes | QMessageBox::No,
				this)
			.exec() == QMessageBox::Yes
	)
	{
		config->delLink(nameSelect->currentText());
		config->loadLink(links);
		nameSelect->removeItem(nameSelect->currentIndex());
	}
}

void LinkInput::onFinClick()
{	accept();	}

QStringList LinkInput::genNameList()
{
	QStringList list;
	foreach(Link l, *links)
		list << l.name;
	return list;
}

void LinkInput::onNameSelectChange()
{
	Link l;
	foreach(Link l_tmp, *links)
		if(l_tmp.name == nameSelect->currentText())
		{
			l = l_tmp;
			break;
		}
	name->setText(l.url.userName());
	pass->setText(l.url.password());
	urlEdit->setText(l.url.toString(QUrl::RemoveUserInfo));
	typeSelect->setCurrentIndex(typeSelect->findText(l.widgetType));
}

LinkInput::~LinkInput(){};
