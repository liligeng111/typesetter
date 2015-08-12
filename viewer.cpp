#include "viewer.h"
#include <qmessagebox.h>
#include <qpainter>
#include <QCloseEvent>
#include <QPoint>
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
	glwidget_ = nullptr;
	
	ui.setupUi(this);
	showMaximized();
}

void Viewer::init()
{
	QRect size = centralWidget()->geometry();
	settings_ = new QSettings("./settings/config.ini", QSettings::IniFormat);
	glwidget_ = new GLWidget(this);
	glwidget_->setGeometry(size.x(), size.y(), size.width(), size.height());
	glwidget_->show();

	textEdit = new QsciScintilla(this);
	textEdit->setGeometry(250, 50, 630, 891);
	textEdit->setWrapMode(QsciScintilla::WrapWord);
	textEdit->setMarginType(0, QsciScintilla::TextMarginRightJustified);
	textEdit->zoomTo(12);
	//textEdit->setFont(QFont());
	textEdit->show();
	readSettings();
	connect(textEdit, SIGNAL(textChanged()),
		this, SLOT(documentWasModified()));
	setCurrentFile("");

	textEdit->setMarginWidth(0, 50);
	textEdit->setMarginLineNumbers(0, true);

	connect(ui.actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));
	connect(ui.actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));
	connect(ui.actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));

	connect(textEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(jump(int, int)));
	connect(textEdit, SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)), this, SLOT(margin_clicked(int, int, Qt::KeyboardModifiers)));

	backup_marker_ = textEdit->markerDefine(QsciScintilla::Circle);
	textEdit->setMarkerBackgroundColor(QColor(255, 20, 20));
	textEdit->setMarginSensitivity(1, true);
}

Viewer::~Viewer()
{

}


void Viewer::margin_clicked(int margin, int line, Qt::KeyboardModifiers state)
{	
	//no alternative
	if (backups_[line].size() == 1)
		return;

	//delete marker and add back...wired slide issue
	textEdit->markerDelete(line);

	int new_index = (backups_index_[line] + 1) % backups_[line].size();
	textEdit->setCursorPosition(line, 0);
	textEdit->standardCommands()->find(QsciCommand::LineDelete)->execute();
	textEdit->insertAt((backups_[line])[new_index], line, 0);

	textEdit->markerAdd(line, backup_marker_);
	backups_index_[line] = new_index;
}

void Viewer::resizeEvent(QResizeEvent * event)
{
	if (glwidget_ == nullptr)
		return;
	QRect size = centralWidget()->geometry();
	textEdit->setGeometry(size.x(), size.y(), size.width() / 2, size.height());
	glwidget_->setGeometry(size.x() + size.width() / 2, size.y(), size.width() / 2, size.height());
}

void Viewer::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) 
	{
		writeSettings();
		event->accept();
	}
	else 
	{
		event->ignore();
	}
}

void Viewer::newFile()
{
	if (maybeSave()) 
	{
		textEdit->clear();
		setCurrentFile("");
		clear_backups();
	}
}

void Viewer::open()
{
	if (maybeSave()) 
	{
		QString fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty())
		{
			loadFile(fileName);
			clear_backups();
		}
	}
}

void Viewer::clear_backups()
{
	for (auto backup : backups_)
	{
		backup.clear();
	}
	backups_.clear();
	backups_index_.clear();

	for (int i = 0; i < textEdit->lines(); i++)
	{
		backups_.push_back(vector<QString>());
		backups_[i].push_back(textEdit->text(i));
		backups_index_.push_back(0);
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

void Viewer::about()
{
	QMessageBox::about(this, tr("About Application"), tr("This is a typesetting editor..."));
}

void Viewer::previous()
{
	int* line = new int(-1);
	int* index = new int(-1);

	textEdit->getCursorPosition(line, index);
	textEdit->setCursorPosition(typesetter_.get_prev_magic(*line), 0);
	textEdit->setFocus();

	delete line;
	delete index;
}

void Viewer::next()
{
	int* line = new int(-1);
	int* index = new int(-1);

	textEdit->getCursorPosition(line, index);
	textEdit->setCursorPosition(typesetter_.get_next_magic(*line), 0);
	textEdit->setFocus();

	delete line;
	delete index;
}

void Viewer::documentWasModified()
{
	setWindowModified(textEdit->isModified());
}

void Viewer::jump(int line, int index)
{
	Page* page = typesetter_.get_page_number(line);
	if (page != nullptr)
		glwidget_->render_page(page);
}

void Viewer::readSettings()
{
	QPoint pos = settings_->value("window/pos", QPoint(200, 200)).toPoint();
	QSize size = settings_->value("window/size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);

	//load settings
	bool m_auto_typeset = settings_->value("typesetting/auto_typeset", false).toBool();
	ui.actionAuto_Typeset->setChecked(m_auto_typeset);

	settings::font_size_ = settings_->value("font/font_size", 18).toInt();

	settings::page_width_ = settings_->value("page/width", 210).toFloat();
	settings::page_height_ = settings_->value("page/height", 297).toFloat();
	settings::margin_top_ = settings_->value("page/margin_top", 19).toFloat();
	settings::margin_left_ = settings_->value("page/margin_left", 19).toFloat();
	settings::margin_right_ = settings_->value("page/margin_right", 13.2).toFloat();
	settings::margin_bottom_ = settings_->value("page/margin_bottom", 36.7).toFloat();


	settings::letter_space_stretch_ratio_ = settings_->value("typesetting/letter_space_stretch_ratio", 0).toFloat();
	settings::letter_space_shrink_ratio_ = settings_->value("typesetting/letter_space_shrink_ratio", 0).toFloat();
	settings::font_expansion_ = settings_->value("typesetting/font_expansion", 0).toFloat();

	settings::min_magic_gain_ = settings_->value("magic/min_magic_gain", 10000).toFloat();
	settings::max_magic_amount_ = settings_->value("magic/max_magic_amount", 1.0).toFloat();
}

void Viewer::writeSettings()
{
	settings_->setValue("window/pos", pos());
	settings_->setValue("window/size", size());
}

bool Viewer::maybeSave()
{
	if (textEdit->isModified())
	{
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

void Viewer::commit()
{
	int* line = new int(-1);
	int* index = new int(-1);
	textEdit->getCursorPosition(line, index);

	//add only once
	if (backups_[*line].size() == 1)
		textEdit->markerAdd(*line, backup_marker_);

	//cout << *line << endl;
	//cout << textEdit->text(*line).toStdString() << endl;
	backups_[*line].push_back(textEdit->text(*line));
	backups_index_[*line] = backups_[*line].size() - 1;
	
	delete line;
	delete index;
}

void Viewer::Message(const string& msg)
{
	QMessageBox::critical(nullptr, "Error", QString::fromLocal8Bit(msg.c_str()));
}

void Viewer::auto_typeset(bool checked)
{
	settings_->setValue("typesetting/auto_typeset", checked);
	if (checked)
		connect(textEdit, SIGNAL(textChanged()), this, SLOT(typeset()));
	else
		disconnect(textEdit, SIGNAL(textChanged()), this, SLOT(typeset()));
}

void Viewer::typeset()
{
	//TODO::why do I have to do this
	typesetter_.Typeset(textEdit->text().append("\n"));
	//typesetter.render(Typesetter::SVG);

	cout << "Total Page Count:" << typesetter_.page_count() << endl;
	ui.pageSlider->setMaximum(typesetter_.page_count() - 1);
	ui.pageSlider->setValue(0);
	//on_pageSlider_valueChanged(0);
	//glwidget_->render_page(typesetter_.page(0));

	int* line = new int(-1);
	int* index = new int(-1);
	textEdit->getCursorPosition(line, index);
	
	jump(*line, *index);

	delete line;
	delete index;
}

void Viewer::on_pageSlider_valueChanged(int value)
{
	if (typesetter_.page_count() == 0)
		return;
	glwidget_->render_page(typesetter_.page(value));
	QString tip = "Page: ";
	tip.append(QString::number(value, 10));
	statusBar()->showMessage(tip);
}

void Viewer::wheelEvent(QWheelEvent * event)
{
	int change;
	if (event->delta() > 0)
	{
		change = -1;
	}
	else
	{
		change = 1;
	}

	ui.pageSlider->setValue(ui.pageSlider->value() + change);
	event->accept();
}

void Viewer::setMarkdownDemerits(bool checked)
{
	ui.actionR->setChecked(false);
	ui.actionPenalty->setChecked(false);
	ui.actionSpaceR->setChecked(false);
	ui.actionFontR->setChecked(false);
	ui.actionLetterSpaceR->setChecked(false);
	settings::markdown_type_ = 0;
	glwidget_->update();
}

void  Viewer::setMarkdownR(bool checked)
{
	ui.actionDemerits->setChecked(false);
	ui.actionPenalty->setChecked(false);
	ui.actionSpaceR->setChecked(false);
	ui.actionFontR->setChecked(false);
	ui.actionLetterSpaceR->setChecked(false);
	settings::markdown_type_ = 1;
	glwidget_->update();
}

void Viewer::setMarkdownPenalty(bool checked)
{
	ui.actionDemerits->setChecked(false);
	ui.actionR->setChecked(false);
	ui.actionSpaceR->setChecked(false);
	ui.actionFontR->setChecked(false);
	ui.actionLetterSpaceR->setChecked(false);
	settings::markdown_type_ = 2;
	glwidget_->update();
}

void Viewer::setMarkdownSpaceR(bool checked)
{
	ui.actionDemerits->setChecked(false);
	ui.actionR->setChecked(false);
	ui.actionPenalty->setChecked(false);
	ui.actionFontR->setChecked(false);
	ui.actionLetterSpaceR->setChecked(false);
	settings::markdown_type_ = 3;
	glwidget_->update();
}

void Viewer::setMarkdownFontR(bool checked)
{
	ui.actionDemerits->setChecked(false);
	ui.actionR->setChecked(false);
	ui.actionPenalty->setChecked(false);
	ui.actionSpaceR->setChecked(false);
	ui.actionLetterSpaceR->setChecked(false);
	settings::markdown_type_ = 4;
	glwidget_->update();
}

void Viewer::setMarkdownLetterSpaceR(bool checked)
{
	ui.actionDemerits->setChecked(false);
	ui.actionR->setChecked(false);
	ui.actionPenalty->setChecked(false);
	ui.actionSpaceR->setChecked(false);
	ui.actionFontR->setChecked(false);
	settings::markdown_type_ = 5;
	glwidget_->update();
}