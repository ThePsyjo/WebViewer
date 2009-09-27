
/************************************************************************
 * linkInput.h								*
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

#ifndef LINK_INPUT_H
#define LINK_INPUT_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
//why ever ...
//#include <qmap.h>
#include <QGridLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QList>
#include <QLabel>
#include <QValidator>
#include <QRegExp>

#include "configuration.h"

class StringInput : public QDialog
{
Q_OBJECT
public:
	StringInput(QStringList, QWidget* = 0);
	virtual ~StringInput();
	QString getVal();
private:
	QValidator *validator;
	QStringList list;
	QString val;
	QLabel *text;
	QLineEdit *e;
	QPushButton *okButton, *cancelButton;
	QGridLayout *layout;
	private slots:
	void onOkClick();
};

class LinkInput : public QDialog
{
Q_OBJECT
public:
	LinkInput(ConfigHandler*, QWidget* = 0);
	virtual ~LinkInput();
private:
	ConfigHandler *config;
	QList<Link> *links;
	QLineEdit *name, *pass, *urlEdit;
	QPushButton *finButton, *newButton, *delButton, *savButton;
	QComboBox *typeSelect, *nameSelect;
	QGridLayout *layout;
	QLabel *lUserName, *lPass, *lUrl, *lType, *lWidgetName;
	QStringList genNameList();
	QFont *defFont;
private slots:
	void onNewClick();
	void onDelClick();
	void onSavClick();
	void onFinClick();
	void onNameSelectChange();
	void handleChange();
};

#endif

