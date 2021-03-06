#ifndef VIEWER_H
#define VIEWER_H

#include <QtWidgets/QMainWindow>
#include "ui_viewer.h"
#include "typesetter.h"
#include "settings.h"
#include "glwidget.h"
#include <QSettings>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscicommandset.h>
using namespace std;

class Viewer : public QMainWindow
{
	Q_OBJECT

public:
	Viewer(QWidget *parent = 0);
	~Viewer();

	void init();
	static void Message(const string& msg);

protected:
	void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE; 
	void keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;

private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void documentWasModified();
	void about();
	void auto_typeset(bool checked);
	void use_magic(bool checked);
	void typeset();
	void jump(int line, int index);
	void margin_clicked(int margin, int line, Qt::KeyboardModifiers state);
	void commit();
	void previous();
	void next();

	void setMarkdownR(bool checked);
	void setMarkdownDemerits(bool checked);
	void setMarkdownPenalty(bool checked);
	void setMarkdownSpaceR(bool checked);
	void setMarkdownFontR(bool checked);
	void setMarkdownLetterSpaceR(bool checked);
	void pick_shrink_color();
	void pick_stretch_color();
	void show_only_one_suggestion(bool);

private:
	Ui::viewerClass ui;
	GLWidget* glwidget_;
	Typesetter typesetter_;
	QSettings* settings_;
	
	void readSettings();
	void writeSettings();
	bool maybeSave();
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);

	QsciScintilla *textEdit;
	QString curFile;

	vector<vector<QString>> backups_;
	vector<int> backups_index_;
	void clear_backups();
	int backup_marker_;

	//TODO::init?
	vector<pair<int, float>> suggestions_;
	int suggestions_index_;
};

#endif // VIEWER_H
