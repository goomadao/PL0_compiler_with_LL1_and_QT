/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//! [0]
#pragma execution_character_set("utf-8")

#include <QtWidgets>
#include <QTextStream>

#include "mainwindow.h"
#include "codeeditor.h"
//! [0]

//! [1]
MainWindow::MainWindow()
	: textEdit(new CodeEditor), parser(".")/*, lineEdit(new QLineEdit)*/
	//! [1] //! [2]
{
	//QLineEdit *lineEdit = new QLineEdit();
	parser = Parser(".");
	setCentralWidget(textEdit);

	createActions();
	createStatusBar();

	readSettings();

	connect(textEdit->document(), &QTextDocument::contentsChanged,
		this, &MainWindow::documentWasModified);

#ifndef QT_NO_SESSIONMANAGER
	QGuiApplication::setFallbackSessionManagementEnabled(false);
	connect(qApp, &QGuiApplication::commitDataRequest,
		this, &MainWindow::commitData);
#endif

	setCurrentFile(QString());
	setUnifiedTitleAndToolBarOnMac(true);
}
//! [2]

//! [3]
void MainWindow::closeEvent(QCloseEvent *event)
//! [3] //! [4]
{
	if (maybeSave()) {
		writeSettings();
		event->accept();
	}
	else {
		event->ignore();
	}
}
//! [4]

//! [5]
void MainWindow::newFile()
//! [5] //! [6]
{
	if (maybeSave()) {
		textEdit->clear();
		setCurrentFile(QString());
	}
}
//! [6]

//! [7]
void MainWindow::open()
//! [7] //! [8]
{
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty())
			loadFile(fileName);
	}
}
//! [8]

//! [9]
bool MainWindow::save()
//! [9] //! [10]
{
	if (curFile.isEmpty()) {
		return saveAs();
	}
	else {
		return saveFile(curFile);
	}
}
//! [10]

//! [11]
bool MainWindow::saveAs()
//! [11] //! [12]
{
	QFileDialog dialog(this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() != QDialog::Accepted)
		return false;
	return saveFile(dialog.selectedFiles().first());
}
//! [12]

//! [13]
void MainWindow::about()
//! [13] //! [14]
{
	QMessageBox::about(this, tr("About Application"),
		tr("这是一个PL/0编译器，其中语法部分采用了LL1分析法"));
}
//! [14]

//! [15]
void MainWindow::documentWasModified()
//! [15] //! [16]
{
	setWindowModified(textEdit->document()->isModified());
}
void MainWindow::originGrammar()
{
	list<list<vector<int> > >::iterator i1 = parser.getGrammarList();
	list<vector<int> >::iterator i2 = i1->begin();
	vector<int>::iterator i3 = i2->begin();
	string out;
	/*for (; i1 != parser.getGrammarListEnd(); ++i1)
	{
		for (i2 = i1->begin(); i2 != i1->end(); ++i2)
		{
			for (int i = 0; i < i2->size(); ++i)
			{
				if (i2 == i1->begin())
				{
					out += GrammarDefinition::GrammarSymTypes[i2->at(i)];
					for (int js = 0; js < 16 - GrammarDefinition::GrammarSymTypes[i2->at(i)].size(); ++js)
						out += " ";
				}
				else
				{
					out += GrammarDefinition::GrammarSymTypes[i2->at(i)];
				}
				out += " ";
			}
			list<vector<int> >::iterator temp = i2;
			++temp;
			if (i2 == i1->begin())
				out +=  "==>";
			else if (temp != i1->end())
			{
				out += " | ";
			}
		}
		out += "\n";
	}*/





	int djt = 1;
	list< list< vector<int> > >::iterator it = parser.getGrammarList();//it:list <list< vector<int> > >迭代器，指向某个list< vector<int> >
	for (; it != parser.getGrammarListEnd(); ++it, ++djt)
	{
		//QMessageBox::about(this, tr("about"), QString::fromStdString( "1"));
		//out += djt;
		//out += "\t";
		list< vector<int> >::iterator iter = it->begin();//iter:list< vector<int> >迭代器，指向某个vector<int>
		for (; iter != it->end(); ++iter)
		{
			for (int i = 0; i < iter->size(); ++i)
			{
				if (iter == it->begin())
				{
					out += GrammarDefinition::GrammarSymTypes[iter->at(i)];
					/*for (int js = 0; js < 25 - GrammarDefinition::GrammarSymTypes[iter->at(i)].size(); ++js)
					{
						out += " ";
					}*/
					out += "    ";
				}
				else
					out += GrammarDefinition::GrammarSymTypes[iter->at(i)];
				out += " ";
			}
			list< vector<int> >::iterator temp = iter;
			++temp;
			if (iter == it->begin())
			{
				out += "==>";
			}
			else if (temp != it->end())
			{
				out += "|";
			}
		}
		out += "\n";
	}





	QMessageBox::about(this, tr("化简文法"), QString::fromStdString(out));
}
void MainWindow::first()
{
	QTableWidget *table = new QTableWidget();
	table->setRowCount(95);
	table->setColumnCount(1);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	QVector<QString> types;
	for (int i = 0; i < 95; ++i)
	{
		types.push_back(QString::fromStdString(GrammarDefinition::GrammarSymTypes[i]));
	}
	table->setVerticalHeaderLabels(QStringList::fromVector(types));
	table->setHorizontalHeaderLabels({ "First集" });
	table->showMaximized();
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	map<int, set<int> >::iterator i1;
	set<int>::iterator i2;
	int r = 0;
	for (i1 = parser.getFirstBegin(); i1 != parser.getFirstEnd(); ++i1, ++r)
	{
		string two;
		for (i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
		{
			two += GrammarDefinition::GrammarSymTypes[*i2];
			set<int>::iterator tmp = i2;
			++tmp;
			if (tmp != i1->second.end())
			{
				two += ", ";
			}
		}
		QTableWidgetItem *new2 = new QTableWidgetItem(QString::fromStdString(two));
		table->setItem(r, 0, new2);
	}
	table->setWindowTitle("first集");
	table->show();
}
void MainWindow::follow()
{
	QTableWidget *table = new QTableWidget();
	table->setRowCount(95);
	table->setColumnCount(1);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	QVector<QString> types;
	for (int i = 0; i < 95; ++i)
	{
		types.push_back(QString::fromStdString(GrammarDefinition::GrammarSymTypes[i]));
	}
	table->setVerticalHeaderLabels(QStringList::fromVector(types));
	table->setHorizontalHeaderLabels({ "Follow集" });
	table->showMaximized();
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	map<int, set<int> >::iterator i1;
	set<int>::iterator i2;
	int r = 0;
	for (i1 = parser.getFollowBegin(); i1 != parser.getFollowEnd(); ++i1, ++r)
	{
		string two;
		for (i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
		{
			two += GrammarDefinition::GrammarSymTypes[*i2];
			set<int>::iterator tmp = i2;
			++tmp;
			if (tmp != i1->second.end())
			{
				two += ", ";
			}
		}
		QTableWidgetItem *new2 = new QTableWidgetItem(QString::fromStdString(two));
		table->setItem(r, 0, new2);
	}
	table->setWindowTitle("follow集");
	table->show();
}
void MainWindow::parsingTable()
{
	QTableWidget *table = new QTableWidget();
	table->setRowCount(59);
	table->setColumnCount(36);
	//table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	QVector<QString>rr, cc;
	for (int i = 0; i < 59; ++i)
	{
		rr.push_back(QString::fromStdString(GrammarDefinition::GrammarSymTypes[i]));
	}
	for (int i = 59; i < 95; ++i)
	{
		cc.push_back(QString::fromStdString(GrammarDefinition::GrammarSymTypes[i]));
	}
	table->setVerticalHeaderLabels(QStringList::fromVector(rr));
	table->setHorizontalHeaderLabels(QStringList::fromVector(cc));
	table->showMaximized();
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	map<parsingNode, vector<int> >::iterator i1;
	vector<int>::iterator i2;
	int r = 0, c = 0;
	for (i1 = parser.getParsingTableBegin(); i1 != parser.getParsingTableEnd(); ++i1, ++r)
	{
		if (!i1->second.size())
			continue;
		string out;
		for (i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
		{
			out += GrammarDefinition::GrammarSymTypes[*i2];
			out += " ";
		}
		QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(out));
		table->setItem(i1->first.nonTerminal, i1->first.terminal - 59, item);
	}
	table->setWindowTitle("预测分析表");
	table->show();
}
void MainWindow::errorList()
{
	parser = Parser(".");
	QString code = textEdit->toPlainText();
	try
	{
		parser = Parser(code.toStdString());
	}
	catch (...)
	{
		return;
		//QMessageBox::about(this, "1", "exception");
	}
	if (parser.errorEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("没有检查到错误"));
		return;
	}
	QTableWidget *table = new QTableWidget();
	table->setRowCount(parser.errorList.size());
	table->setColumnCount(2);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->setHorizontalHeaderLabels({ "错误信息","行" });
	table->showMaximized();
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	priority_queue<errorMessage>tmpQueue;
	int r = -1, errRow = -1, errCol = -1, count = 0;
	while (!parser.errorList.empty())
	{
		errorMessage tmp = parser.errorList.top();
		if (errRow == tmp.getRow() && errCol == tmp.getColumn())
		{
			tmpQueue.push(parser.errorList.top());
			parser.errorList.pop();
			continue;
		}
		++count;
		errRow = tmp.getRow();
		errCol = tmp.getColumn();
		QTableWidgetItem *i1 = new QTableWidgetItem(QString::fromStdString(tmp.getMessage())),
			*i2 = new QTableWidgetItem(QString::number(tmp.getRow())),
			*i3 = new QTableWidgetItem(QString::number(tmp.getColumn()));
		table->setItem(++r, 0, i1);
		table->setItem(r, 1, i2);
		//table->setItem(r, 2, i3);
		tmpQueue.push(parser.errorList.top());
		parser.errorList.pop();
	}
	while (!tmpQueue.empty())
	{
		parser.errorList.push(tmpQueue.top());
		tmpQueue.pop();
	}
	table->setWindowTitle("错误");
	table->setRowCount(count);
	table->show();

}
void MainWindow::parsingProc()
{
	parser = Parser(".");
	QString code = textEdit->toPlainText();
	try
	{
		parser = Parser(code.toStdString());
	}
	catch (...)
	{
		return;
		//QMessageBox::about(this, "1", "exception");
	}
	if (!parser.errorEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("请先修正错误再查看LL1分析过程!"));
		errorList();
		return;
	}
	//QMessageBox::about(this, "!", QString::number(parser.proc.size()));
	QTableWidget *table = new QTableWidget();
	table->setRowCount(parser.proc.size());
	table->setColumnCount(3);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->setHorizontalHeaderLabels(QStringList({ "符号栈","下一个输入符号","选择的产生式" }));
	table->showMaximized();
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	int r = 0, c = 0;
	for (int i = 0; i < parser.proc.size(); ++i, ++r)
	{
		QString out, out2 = QString::fromStdString(GrammarDefinition::GrammarSymTypes[parser.proc[i].s.top()]) + " ==> ";
		QStack<int>tmp;
		while (!parser.proc[i].s.empty())
		{
			out += QString::fromStdString(GrammarDefinition::GrammarSymTypes[parser.proc[i].s.top()]);
			parser.proc[i].s.pop();
			if (!parser.proc[i].s.empty())
				out += ", ";
		}
		/*while (!parser.proc[i].s.empty())
		{
			tmp.push(parser.proc[i].s.top());
			parser.proc[i].s.pop();
		}
		while (!tmp.empty())
		{
			out += QString::fromStdString(GrammarDefinition::GrammarSymTypes[tmp.top()]);
			tmp.pop();
			out += ", ";
		}*/
		QTableWidgetItem *item0 = new QTableWidgetItem(out);
		table->setItem(r, 0, item0);
		QTableWidgetItem *item1 = new QTableWidgetItem(QString::fromStdString(parser.proc[i].t.getVal()));
		table->setItem(r, 1, item1);
		for (int j = 0; j < parser.proc[i].v.size(); ++j)
		{
			out2 += QString::fromStdString(GrammarDefinition::GrammarSymTypes[parser.proc[i].v[j]]);
			out2 += " ";
		}
		if (parser.proc[i].v.empty())
		{
			out2 = "";
		}
		QTableWidgetItem *item2 = new QTableWidgetItem(out2);
		table->setItem(r, 2, item2);
	}
	table->setWindowTitle("LL1分析过程");
	table->show();
}
void MainWindow::pcode()
{
	parser = Parser(".");
	QString code = textEdit->toPlainText();
	try
	{
		parser = Parser(code.toStdString());
	}
	catch (...)
	{
		return;
		//QMessageBox::about(this, "1", "exception");
	}
	if (!parser.errorEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("请先修正错误再生成pcode!"));
		errorList();
		return;
	}
	QTableWidget *table = new QTableWidget();
	table->setRowCount(parser.getPCodeEnd() - parser.getPCodeBegin());
	table->setColumnCount(1);
	QVector<QString>row;
	for (int i = 0; i < table->rowCount(); ++i)
		row.push_back(QString::number(i));
	table->setVerticalHeaderLabels(QStringList::fromVector(row));
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->setHorizontalHeaderLabels(QStringList({ "pcode" }));
	table->showMaximized();
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	vector<pCode>::iterator i1;
	int r = 0, c = 0;
	for (i1 = parser.getPCodeBegin(); i1 != parser.getPCodeEnd(); ++i1, ++r)
	{
		QString out;
		out += QString::fromStdString(i1->op);
		out += ", ";
		out += QString::number(i1->a1);
		out += ", ";
		out += QString::number(i1->a2);
		QTableWidgetItem *item = new QTableWidgetItem(out);
		table->setItem(r, 0, item);
	}
	table->setWindowTitle("pcode");
	table->show();
}
void MainWindow::run()
{
	//plainEdit->showMaximized();
	parser = Parser(".");
	QString code = textEdit->toPlainText();
	try
	{
		parser = Parser(code.toStdString());
	}
	catch (...)
	{
		return;
		//QMessageBox::about(this, "1", "exception");
	}
	if (!parser.errorEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("请先修正错误再生成pcode!"));
		errorList();
		return;
	}
	QPlainTextEdit *plainEdit = new QPlainTextEdit();
	plainEdit->setWindowTitle("运行结果");
	plainEdit->show();
	QVector<int>s;
	QString out;
	int p = 0, b = 0, t = 0;
	s.push_back(-1);
	s.push_back(-1);
	s.push_back(-1);
	bool flag;
	while (1)
	{
		plainEdit->setPlainText(out);


		QString bug;
		bug += QString::fromStdString(parser.pcode[p].op) +
			"," +
			QString::number(parser.pcode[p].a1) +
			"," +
			QString::number(parser.pcode[p].a2) +
			"   b:" +
			QString::number(b) +
			"   p:" +
			QString::number(p) +
			"\n";
		for (int i = s.size() - 1; i > -1; --i)
		{
			bug += QString::number(s[i]);
			bug += "\n";
		}
		//QMessageBox::about(this, "1", bug);
		if (parser.pcode[p].op == "OPR" && parser.pcode[p].a2 == 0 && s[b] == -1)
		{
			break;
		}
		if (parser.pcode[p].op == "LIT")
		{
			s.push_back(parser.pcode[p].a2);
			++p;
		}
		else if (parser.pcode[p].op == "OPR")
		{
			if (parser.pcode[p].a2 == 0)
			{
				int start = b, end = s.size();
				p = s[b + 2];
				b = s[b];
				for (int i = start; i < end; ++i)
					s.pop_back();
				continue;
			}
			int a = s.last();
			s.pop_back();
			if (parser.pcode[p].a2 == 1)
			{
				s.push_back(-a);
				++p;
				continue;
			}
			if (parser.pcode[p].a2 == 6)
			{
				flag = a % 2;
				++p;
				continue;
			}
			int b = s.last();
			s.pop_back();
			swap(a, b);
			if (parser.pcode[p].a2 == 2)
			{
				s.push_back(a + b);
			}
			else if (parser.pcode[p].a2 == 3)
			{
				s.push_back(a - b);
			}
			else if (parser.pcode[p].a2 == 4)
			{
				s.push_back(a * b);
			}
			else if (parser.pcode[p].a2 == 5)
			{
				s.push_back(a / b);
			}
			else if (parser.pcode[p].a2 == 8)
			{
				flag = (a == b);
			}
			else if (parser.pcode[p].a2 == 9)
			{
				flag = (a != b);
			}
			else if (parser.pcode[p].a2 == 10)
			{
				flag = (a < b);
			}
			else if (parser.pcode[p].a2 == 11)
			{
				flag = (a >= b);
			}
			else if (parser.pcode[p].a2 == 12)
			{
				flag = (a > b);
			}
			else if (parser.pcode[p].a2 == 13)
			{
				flag = (a <= b);
			}
			++p;
		}
		else if (parser.pcode[p].op == "LOD")
		{
			int js = parser.pcode[p].a1;
			int tmp = b;
			while (js--)
			{
				tmp = s[tmp];
			}
			s.push_back(s[tmp + parser.pcode[p].a2]);
			++p;
		}
		else if (parser.pcode[p].op == "STO")
		{
			int stackTop = s.last();
			s.pop_back();
			int js = parser.pcode[p].a1;
			int tmp = b;
			while (js--)
			{
				tmp = s[tmp];
			}
			s[tmp + parser.pcode[p].a2] = stackTop;
			++p;
		}
		else if (parser.pcode[p].op == "CAL")
		{
			int tmp = b;
			s.push_back(b);
			s.push_back(b);
			b = s.size() - 2;
			s.push_back(p + 1);
			p = parser.pcode[p].a2;
		}
		else if (parser.pcode[p].op == "INT")
		{
			for (int i = 0; i < parser.pcode[p].a2 - 3; ++i)
			{
				s.push_back(0x3f3f3f);
			}
			++p;
		}
		else if (parser.pcode[p].op == "JMP")
		{
			p = parser.pcode[p].a2;
		}
		else if (parser.pcode[p].op == "JPC")
		{
			if (flag)
			{
				++p;
			}
			else
			{
				p = parser.pcode[p].a2;
			}
		}
		else if (parser.pcode[p].op == "RED")
		{
			int js = parser.pcode[p].a1;
			int tmp = b;
			while (js--)
			{
				tmp = s[tmp];
			}

			//QWidget *w = new QWidget();
			//QLineEdit *edit = new QLineEdit(this);


			//QIntValidator* validator = new QIntValidator;
			//validator->setRange(INT_MIN, INT_MAX);
			//edit->setValidator(validator);
			////connect(edit, &QLineEdit::returnPressed, this, &MainWindow::input);
			//edit->show();
			////connect(saveAct, &QAction::triggered, this, &MainWindow::save);
			////改成读取输入

			////while (!flag);
			////flag = false;
			////inputNum = edit->text().toInt();

			bool ok;
			int inputNum = QInputDialog::getInt(this, tr("Input"), "", 0, INT_MIN, INT_MAX, 1, &ok);
			if (ok)
				s[tmp + parser.pcode[p].a2] = inputNum;
			else
			{
				QMessageBox::warning(this, tr("Warning"), tr("由于取消输入，程序终止，请重新运行"));
				return;
			}


			++p;
		}
		else if (parser.pcode[p].op == "WRT")
		{
			int stackTop = s.last();
			s.pop_back();
			out += QString::number(stackTop);
			out += ("\n");
			++p;
		}
	}
	out += "程序运行结束...";
	plainEdit->setPlainText(out);
}
//void MainWindow::run()
//{
//	QString out;
//	int ret = 0x3f3f3f, p = 0;
//	bool flag;
//	QStack<sta> s;
//	sta *b = nullptr, *t = nullptr;
//	sta *temp = new sta(nullptr);
//	s.push(*temp);
//	b = &s.top();
//	QMessageBox::about(this, "1", QString::number(s.top().num));
//	QMessageBox::about(this, "1", QString::number((int)s.top().it));
//	QMessageBox::about(this, "1", QString::number((int)(b->it == nullptr)));
//	temp = new sta(nullptr);
//	s.push(*temp);
//	temp = new sta(nullptr);
//	s.push(*temp);
//	int count = 0;
//	while (1)
//	{
//		QMessageBox::about(this, "1", QString::number(count) /*+ QString::number((long)(b->it))*/);
//		QMessageBox::about(this, "1", QString::fromStdString(parser.pcode[p].op));
//		//if (b->it==nullptr)
//			QMessageBox::about(this, "1", QString::number((long)(b->it)));
//		if (parser.pcode[p].op == "OPR" && parser.pcode[p].a2 == 0 && b->it == nullptr)
//		{
//			break;
//		}
//		if (parser.pcode[p].op == "LIT")
//		{
//			s.push(sta(parser.pcode[p].a2));
//			++p;
//		}
//		else if (parser.pcode[p].op == "OPR")
//		{
//			if (parser.pcode[p].a2 == 0)
//			{
//				sta* temp = b;
//				++temp; ++temp;
//				p = temp->num;
//				b = b->it;
//				continue;
//			}
//			int a = s.top().num;
//			s.pop();
//			if (parser.pcode[p].a2 == 1)
//			{
//				s.push(sta(-a));
//				++p;
//				continue;
//			}
//			if (parser.pcode[p].a2 == 6)
//			{
//				flag = a % 2;
//				++p;
//				continue;
//			}
//			int b = s.top().num;
//			s.pop();
//			if (parser.pcode[p].a2 == 2)
//			{
//				s.push(sta(a + b));
//			}
//			else if (parser.pcode[p].a2 == 3)
//			{
//				s.push(sta(a - b));
//			}
//			else if (parser.pcode[p].a2 == 4)
//			{
//				s.push(sta(a * b));
//			}
//			else if (parser.pcode[p].a2 == 5)
//			{
//				s.push(sta(a / b));
//			}
//			else if (parser.pcode[p].a2 == 8)
//			{
//				flag = (a == b);
//			}
//			else if (parser.pcode[p].a2 == 9)
//			{
//				flag = (a != b);
//			}
//			else if (parser.pcode[p].a2 == 10)
//			{
//				flag = (a < b);
//			}
//			else if (parser.pcode[p].a2 == 11)
//			{
//				flag = (a >= b);
//			}
//			else if (parser.pcode[p].a2 == 12)
//			{
//				flag = (a > b);
//			}
//			else if (parser.pcode[p].a2 == 13)
//			{
//				flag = (a <= b);
//			}
//			++p;
//		}
//		else if (parser.pcode[p].op == "LOD")
//		{
//			int js = parser.pcode[p].a1;
//			sta *tmp = b;
//			while (js--)
//			{
//				tmp = tmp->it;
//			}
//			js = parser.pcode[p].a2;
//			while (js--)
//			{
//				++tmp;
//			}
//			s.push(tmp->num);
//			++p;
//		}
//		else if (parser.pcode[p].op == "STO")
//		{
//			int stackTop = s.top().num;
//			s.pop();
//			int js = parser.pcode[p].a1;
//			sta* tmp = b;
//			while (js--)
//			{
//				tmp = tmp->it;
//			}
//			js = parser.pcode[p].a2;
//			while (js--)
//			{
//				++tmp;
//			}
//			tmp->num = stackTop;
//			++p;
//		}
//		else if (parser.pcode[p].op == "CAL")
//		{
//			sta* tmp = b;
//			s.push(sta(b));
//			b = &s.top();
//			s.push(sta(tmp));
//			s.push(sta(p + 1));
//			p = parser.pcode[p].a2;
//		}
//		else if (parser.pcode[p].op == "INT")
//		{
//			for (int i = 0; i < parser.pcode[p].a2 - 3; ++i)
//			{
//				s.push(sta(0x3f3f3f));
//			}
//			++p;
//		}
//		else if (parser.pcode[p].op == "JMP")
//		{
//			p = parser.pcode[p].a2;
//		}
//		else if (parser.pcode[p].op == "JPC")
//		{
//			if (flag)
//			{
//				++p;
//			}
//			else
//			{
//				p = parser.pcode[p].a2;
//			}
//		}
//		else if (parser.pcode[p].op == "RED")
//		{
//			int stackTop = s.top().num;
//			s.pop();
//			int js = parser.pcode[p].a1;
//			sta* tmp = b;
//			while (js--)
//			{
//				tmp = tmp->it;
//			}
//			js = parser.pcode[p].a2;
//			while (js--)
//			{
//				++tmp;
//			}
//			tmp->num = 9;
//
//
//			++p;
//		}
//		else if (parser.pcode[p].op == "WRT")
//		{
//			int stackTop = s.top().num;
//			s.pop();
//			out += QString::number(stackTop);
//			out += ("\n");
//			++p;
//		}
//
//
//
//	}
//	out += "程序运行结束";
//	QMessageBox::about(this, "1", out);
//}
//! [16]

//! [17]
void MainWindow::createActions()
//! [17] //! [18]
{

	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	QToolBar *fileToolBar = addToolBar(tr("File"));
	const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
	QAction *newAct = new QAction(newIcon, tr("&New"), this);
	newAct->setShortcuts(QKeySequence::New);
	newAct->setStatusTip(tr("Create a new file"));
	connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
	fileMenu->addAction(newAct);
	fileToolBar->addAction(newAct);

	//! [19]
	const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
	QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, &QAction::triggered, this, &MainWindow::open);
	fileMenu->addAction(openAct);
	fileToolBar->addAction(openAct);
	//! [18] //! [19]

	const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
	QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, &QAction::triggered, this, &MainWindow::save);
	fileMenu->addAction(saveAct);
	fileToolBar->addAction(saveAct);

	const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
	QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));

	//! [20]

	fileMenu->addSeparator();

	const QIcon exitIcon = QIcon::fromTheme("application-exit");
	QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
	exitAct->setShortcuts(QKeySequence::Quit);
	//! [20]
	exitAct->setStatusTip(tr("Exit the application"));

	//! [21]
	QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
	QToolBar *editToolBar = addToolBar(tr("Edit"));
	//!
#ifndef QT_NO_CLIPBOARD
	const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
	QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
	//! [21]
	cutAct->setShortcuts(QKeySequence::Cut);
	cutAct->setStatusTip(tr("Cut the current selection's contents to the "
		"clipboard"));
	connect(cutAct, &QAction::triggered, textEdit, &QPlainTextEdit::cut);
	editMenu->addAction(cutAct);
	editToolBar->addAction(cutAct);

	const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
	QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
	copyAct->setShortcuts(QKeySequence::Copy);
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
		"clipboard"));
	connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
	editMenu->addAction(copyAct);
	editToolBar->addAction(copyAct);

	const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
	QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
	pasteAct->setShortcuts(QKeySequence::Paste);
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
		"selection"));
	connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
	editMenu->addAction(pasteAct);
	editToolBar->addAction(pasteAct);

	menuBar()->addSeparator();


	QMenu *LL1Menu = menuBar()->addMenu(tr("&LL1"));

	QAction *originGrammar = LL1Menu->addAction(tr("化简文法"), this, &MainWindow::originGrammar);
	originGrammar->setStatusTip(tr("显示化简后的符合LL1的PL/0文法"));

	QAction *firstAct = LL1Menu->addAction(tr("&first集"), this, &MainWindow::first);
	firstAct->setStatusTip(tr("显示PL/0的First集"));

	QAction *followAct = LL1Menu->addAction(tr("&follow集"), this, &MainWindow::follow);
	followAct->setStatusTip(tr("显示PL/0的Follow集"));

	QAction *parsingTableAct = LL1Menu->addAction(tr("&预测分析表"), this, &MainWindow::parsingTable);
	parsingTableAct->setStatusTip(tr("显示PL/0的预测分析表"));


	QMenu *compileMenu = menuBar()->addMenu(tr("&compile"));

	QAction *errorAct = compileMenu->addAction(tr("错误分析"), this, &MainWindow::errorList);
	errorAct->setStatusTip(tr("显示语法及语义错误"));

	QAction *parsingAct = compileMenu->addAction(tr("&LL1语法分析过程"), this, &MainWindow::parsingProc);
	parsingAct->setStatusTip(tr("显示LL1语法分析过程"));

	compileMenu->addSeparator();

	QAction *pcodeAct = compileMenu->addAction(tr("&pcode"), this, &MainWindow::pcode);
	pcodeAct->setStatusTip(tr("显示生成的pcode"));

	QAction *runAct = compileMenu->addAction(tr("run"), this, &MainWindow::run);
	runAct->setStatusTip(tr("运行代码"));

#endif // !QT_NO_CLIPBOARD

	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
	aboutAct->setStatusTip(tr("Show the application's About box"));

	//! [22]

	/*QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));*/
	//! [22]

	//! [23]
#ifndef QT_NO_CLIPBOARD
	cutAct->setEnabled(false);
	//! [23] //! [24]
	copyAct->setEnabled(false);
	connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
	connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}
//! [24]

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
	statusBar()->showMessage(tr("Ready"));
}
//! [33]

//! [34] //! [35]
void MainWindow::readSettings()
//! [34] //! [36]
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
	if (geometry.isEmpty()) {
		const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
		resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
		move((availableGeometry.width() - width()) / 2,
			(availableGeometry.height() - height()) / 2);
	}
	else {
		restoreGeometry(geometry);
	}
}
//! [35] //! [36]

//! [37] //! [38]
void MainWindow::writeSettings()
//! [37] //! [39]
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	settings.setValue("geometry", saveGeometry());
}
//! [38] //! [39]

//! [40]
bool MainWindow::maybeSave()
//! [40] //! [41]
{
	if (!textEdit->document()->isModified())
		return true;
	const QMessageBox::StandardButton ret
		= QMessageBox::warning(this, tr("Application"),
			tr("The document has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	switch (ret) {
	case QMessageBox::Save:
		return save();
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}
	return true;
}
//! [41]

//! [42]
void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(QDir::toNativeSeparators(fileName), file.errorString()));
		return;
	}

	QTextStream in(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [43]

//! [44]
bool MainWindow::saveFile(const QString &fileName)
//! [44] //! [45]
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(QDir::toNativeSeparators(fileName),
				file.errorString()));
		return false;
	}

	QTextStream out(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}
//! [45]

//! [46]
void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
	curFile = fileName;
	textEdit->document()->setModified(false);
	setWindowModified(false);

	QString shownName = curFile;
	if (curFile.isEmpty())
		shownName = "untitled.txt";
	setWindowFilePath(shownName);
}
//! [47]

//! [48]
QString MainWindow::strippedName(const QString &fullFileName)
//! [48] //! [49]
{
	return QFileInfo(fullFileName).fileName();
}
//! [49]
#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{
	if (manager.allowsInteraction()) {
		if (!maybeSave())
			manager.cancel();
	}
	else {
		// Non-interactive: save without asking
		if (textEdit->document()->isModified())
			save();
	}
}
#endif
