#include "viewer.h"
#include <qmessagebox.h>
#include <qpainter>
#include <QCloseEvent>
#include <QPoint>
#include <QFile>
#include <QFileInfo>
#include <QColorDialog>
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
	textEdit->setUtf8(true);
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

		suggestions_.clear();
		suggestions_index_ = -1;
		typesetter_.typeset(textEdit->text().append("\n"), false);
		suggestions_ = typesetter_.get_suggestions();
		sort(suggestions_.begin(), suggestions_.end(), [](const pair<int, float> & a, const pair<int, float> & b)
		{
			return a.second > b.second;
		});
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
	if (suggestions_.size() == 0)
		return;

	int* line = new int(-1);
	int* index = new int(-1);

	suggestions_index_--;
	suggestions_index_ += suggestions_.size();
	suggestions_index_ %= suggestions_.size();
	auto suggestion = suggestions_[suggestions_index_];
	textEdit->setCursorPosition(suggestion.first, 0);
	textEdit->setFocus();
	QString msg = QString("Importance: %1").arg(suggestion.second, 0, 'f', 2);
	statusBar()->showMessage(msg, 2000);

	delete line;
	delete index;
}

void Viewer::next()
{
	if (suggestions_.size() == 0)
		return;

	int* line = new int(-1);
	int* index = new int(-1);

	suggestions_index_++;
	suggestions_index_ %= suggestions_.size();
	auto suggestion = suggestions_[suggestions_index_];
	textEdit->setCursorPosition(suggestion.first, 0);
	textEdit->setFocus();
	QString msg = QString("Importance: %1").arg(suggestion.second, 0, 'f', 2);
	statusBar()->showMessage(msg, 2000);

	delete line;
	delete index;
}

void Viewer::documentWasModified()
{
	setWindowModified(textEdit->isModified());
}

void Viewer::jump(int line, int index)
{
	//typesetter_.typeset(textEdit->text(line), true);
	//Page* page = typesetter_.get_page_number(0);
	//if (page != nullptr)
	//	glwidget_->render_page(page);
	typesetter_.reset_magic_index();
	typeset();
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
	settings::max_magic_amount_ = settings_->value("magic/max_magic_amount", 1.0f).toFloat();
	settings::use_magic_ = settings_->value("magic/use_magic", false).toBool();
	ui.actionUse_Magic->setChecked(settings::use_magic_);

	settings::shrink_color_ = settings_->value("view/shrink_color", QColor(0, 0, 0)).value<QColor>();
	settings::stretch_color_ = settings_->value("view/stretch_color", QColor(0, 0, 0)).value<QColor>();

	bool m_show_only_one_suggestion = settings_->value("view/show_only_one_suggestion", false).toBool();
	ui.actionShow_Only_One_Suggestion->setChecked(m_show_only_one_suggestion);
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

void Viewer::use_magic(bool checked)
{
	settings_->setValue("magic/use_magic", checked);
	settings::use_magic_ = checked;
	typeset();
}

void Viewer::typeset()
{
	int* line = new int(-1);
	int* index = new int(-1);

	textEdit->getCursorPosition(line, index);
	//TODO::why do I have to do this
	//typesetter_.Typeset(textEdit->text().append("\n"));
	//typesetter_.Typeset(backups_[*line][backups_index_[*line]]);
	glwidget_->reset();
	typesetter_.typeset(textEdit->text(*line), true);
	//typesetter.render(Typesetter::SVG);

	cout << "Total Page Count:" << typesetter_.page_count() << endl;

	Page* page = typesetter_.get_page_number(0);
	if (page != nullptr)
		glwidget_->render_page(page);

	//jump(0, 0);
	//jump(*line, *index);

	delete line;
	delete index;
}

void Viewer::wheelEvent(QWheelEvent * event)
{
	if (event->delta() > 0)
	{
		if (settings::use_magic_)
		{
			QString result = typesetter_.magic_index_increase();
			typeset();
			statusBar()->showMessage(result, 5000);
		}
		event->accept();
	}
	else
	{
		if (settings::use_magic_)
		{
			QString result = typesetter_.magic_index_decrease();
			typeset();
			statusBar()->showMessage(result, 5000);
		}
		event->accept();
	}
}

void Viewer::keyReleaseEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_J)
	{
	}
	else if (event->key() == Qt::Key_K)
	{
	}
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

void Viewer::show_only_one_suggestion(bool checked)
{
	settings::show_only_one_suggestion_ = checked;
	settings_->setValue("view/show_only_one_suggestion", checked);
	glwidget_->update();
}

void Viewer::pick_shrink_color()
{
	QColor color = QColorDialog::getColor(settings::shrink_color_);
	settings_->setValue("view/shrink_color", color);
	settings::shrink_color_ = color;
	glwidget_->update();
}

void Viewer::pick_stretch_color()
{
	QColor color = QColorDialog::getColor(settings::stretch_color_);
	settings_->setValue("view/stretch_color", color);
	settings::stretch_color_ = color;
	glwidget_->update();
}