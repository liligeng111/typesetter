#include "viewer.h"
#include <qmessagebox.h>
#include <qpainter>
#include <QCloseEvent>
#include <QPoint>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QSize>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>
#include "settings.h"
#include <fstream>
#include <streambuf>
#include "container.h"



Viewer::Viewer(QWidget *parent)
	: QMainWindow(parent)
{
	textEdit = new QsciScintilla(this);
	textEdit->setGeometry(250, 50, 630, 891); 
	textEdit->show();
	textEdit->SendScintilla(textEdit->SCI_SETHSCROLLBAR, 0);
	
	readSettings();
	connect(textEdit, SIGNAL(textChanged()),
		this, SLOT(documentWasModified()));
	setCurrentFile("");

	ui.setupUi(this);
	content = "input/sample.txt";
	showMaximized();

	update_UI();

	glwidget_ = new GLWidget(this);
	glwidget_->setGeometry(1050, 50, 630, 891);
	glwidget_->show();
}

Viewer::~Viewer()
{

}


void Viewer::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) {
		writeSettings();
		event->accept();
	}
	else {
		event->ignore();
	}
}

void Viewer::newFile()
{
	if (maybeSave()) {
		textEdit->clear();
		setCurrentFile("");
	}
}

void Viewer::open()
{
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty())
			loadFile(fileName);
	}
}

bool Viewer::save()
{
	if (curFile.isEmpty()) {
		return saveAs();
	}
	else {
		return saveFile(curFile);
	}
}

bool Viewer::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return false;

	return saveFile(fileName);
}

void Viewer::documentWasModified()
{
	setWindowModified(textEdit->isModified());
}

void Viewer::readSettings()
{
	QSettings settings("Trolltech", "Application Example");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}

void Viewer::writeSettings()
{
	QSettings settings("Trolltech", "Application Example");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

bool Viewer::maybeSave()
{
	if (textEdit->isModified()) {
		int ret = QMessageBox::warning(this, tr("Application"),
			tr("The document has been modified.\n"
			"Do you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::Default,
			QMessageBox::No,
			QMessageBox::Cancel | QMessageBox::Escape);
		if (ret == QMessageBox::Yes)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

void Viewer::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}

	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEdit->setText(in.readAll());
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}

bool Viewer::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << textEdit->text();
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void Viewer::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	textEdit->setModified(false);
	setWindowModified(false);

	QString shownName;
	if (curFile.isEmpty())
		shownName = "untitled.txt";
	else
		shownName = strippedName(curFile);

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}

QString Viewer::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void Viewer::update_UI()
{
	ui.fontSizeBox->setValue(settings::font_size_);
	ui.expansionBox->setCurrentIndex(settings::expansion_mode_);
	ui.expansionSpinBox->setValue(settings::max_expansion_);

	ui.magicSpinBox->setValue(settings::max_magic_amount_);
	ui.magicGainSpinBox->setValue(settings::min_magic_gain_);
}

void Viewer::Message(const string& msg)
{
	QMessageBox::critical(nullptr, "Error", QString::fromLocal8Bit(msg.c_str()));
}

void Viewer::on_renderButton_clicked()
{
	typesetter.set_content(content);
	typesetter.Typeset();
	//typesetter.render(Typesetter::SVG);

	cout << "Total Page Count:" << typesetter.page_count() << endl;
	ui.pageSlider->setMaximum(typesetter.page_count() - 1);
	ui.pageSlider->setValue(0);
	//on_pageSlider_valueChanged(0);
	glwidget_->render_page(typesetter.page(0), 0);
}


void Viewer::on_pageSlider_valueChanged(int value)
{
	if (typesetter.page_count() == 0)
		return;
	glwidget_->render_page(typesetter.page(value), value);
	QString tip = "Page: ";
	tip.append(QString::number(value, 10));
	statusBar()->showMessage(tip);
}

void Viewer::on_spaceBorderButton_clicked(bool checked)
{
	settings::border_[Item::GLUE] = checked;
}

void Viewer::on_charBorderButton_clicked(bool checked)
{
	settings::border_[Item::BOX] = checked;
}

void Viewer::on_wordBorderButton_clicked(bool checked)
{
	settings::border_[Container::WORD] = checked;
}

void Viewer::on_lineBorderButton_clicked(bool checked)
{
	settings::border_[Container::LINE] = checked;
}

void Viewer::on_pageBorderButton_clicked(bool checked)
{
	settings::border_[Container::PAGE] = checked;
}

void Viewer::on_riverButton_clicked(bool checked)
{
	settings::show_river_ = checked;
}

void Viewer::on_fontSizeBox_valueChanged(int arg1)
{
	settings::font_size_ = arg1;
	typesetter.LoadFace();
}

void Viewer::on_riverThresholdSpinBox_valueChanged(double arg1)
{
	settings::river_threshold_ = arg1;
}

void Viewer::on_actionOpen_File_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",tr("*"));
	
	//ifstream t(fileName);
	//content = string((std::istreambuf_iterator<char>(t)),
	//				 std::istreambuf_iterator<char>());
	//ui.plainTextEdit->setPlainText(QString::fromStdString(content));
	//for now
	string file = fileName.toUtf8().constData();
	cout << "Reading file: " << file << endl;
	content = file;
}

void Viewer::on_markdownBox_currentIndexChanged(int index)
{
	settings::markdown_type_ = index;
	//nvpr_renderer.render_page(typesetter.page(ui.pageSlider->value()), ui.pageSlider->value());
}

void Viewer::on_paperBox_currentIndexChanged(QString page)
{
	if (page == "A7")
	{
		settings::page_width_ = 74;
		settings::page_height_ = 105;
	}
	else if (page == "A6")
	{
		settings::page_width_ = 105;
		settings::page_height_ = 148;
	}
	else if (page == "A5")
	{
		settings::page_width_ = 148;
		settings::page_height_ = 210;
	}
	else if (page == "A4")
	{
		settings::page_width_ = 210;
		settings::page_height_ = 297;
	}
	else if (page == "A3")
	{
		settings::page_width_ = 297;
		settings::page_height_ = 420;
	}
	else if (page == "A2")
	{
		settings::page_width_ = 420;
		settings::page_height_ = 594;
	}
	else if (page == "B7")
	{
		settings::page_width_ = 88;
		settings::page_height_ = 125;
	}
	else if (page == "B6")
	{
		settings::page_width_ = 125;
		settings::page_height_ = 176;
	}
	else if (page == "B5")
	{
		settings::page_width_ = 176;
		settings::page_height_ = 250;
	}
	else if (page == "B4")
	{
		settings::page_width_ = 250;
		settings::page_height_ = 353;
	}
	else if (page == "B3")
	{
		settings::page_width_ = 353;
		settings::page_height_ = 500;
	}
	else if (page == "B2")
	{
		settings::page_width_ = 500;
		settings::page_height_ = 707;
	}
}

void Viewer::on_expansionBox_currentIndexChanged(int index)
{
	settings::expansion_mode_ = index;
}

void Viewer::on_expansionSpinBox_valueChanged(double arg1)
{
	settings::max_expansion_ = arg1;
}


void Viewer::on_useMagicBox_clicked(bool checked)
{
	settings::use_magic_ = checked;
}

void Viewer::on_magicSpinBox_valueChanged(double arg1)
{
	settings::max_magic_amount_ = arg1;
}

void Viewer::on_magicGainSpinBox_valueChanged(double arg1)
{
	settings::min_magic_gain_ = arg1;
}