#ifndef VIEWER_H
#define VIEWER_H

#include <QtWidgets/QMainWindow>
#include "ui_viewer.h"
#include "typesetter.h"
#include "settings.h"
#include "glwidget.h"

#include <Qsci/qsciscintilla.h>
using namespace std;

class Viewer : public QMainWindow
{
	Q_OBJECT

public:
	Viewer(QWidget *parent = 0);
	~Viewer();


	static void Message(const string& msg);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void on_renderButton_clicked();
	void on_spaceBorderButton_clicked(bool checked);
	void on_charBorderButton_clicked(bool checked);
	void on_wordBorderButton_clicked(bool checked);
	void on_lineBorderButton_clicked(bool checked);
	void on_pageBorderButton_clicked(bool checked);
	void on_markdownBox_currentIndexChanged(int index);
	void on_riverButton_clicked(bool checked);
	void on_fontSizeBox_valueChanged(int arg1);
	void on_riverThresholdSpinBox_valueChanged(double arg1);
	void on_paperBox_currentIndexChanged(QString page);
	void on_pageSlider_valueChanged(int value);
	void on_expansionBox_currentIndexChanged(int index);
	void on_expansionSpinBox_valueChanged(double arg1);

	void on_useMagicBox_clicked(bool checked);
	void on_magicSpinBox_valueChanged(double arg1);
	void on_magicGainSpinBox_valueChanged(double arg1);

    void on_actionOpen_File_triggered();


	void newFile();
	void open();
	bool save();
	bool saveAs();
	void documentWasModified();

private:
	Ui::viewerClass ui;
	GLWidget* glwidget_;
	Typesetter typesetter;
	string content;

	void update_UI();

	void readSettings();
	void writeSettings();
	bool maybeSave();
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);

	QsciScintilla *textEdit;
	QString curFile;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QAction *newAct;
	QAction *openAct;
	QAction *saveAct;
	QAction *saveAsAct;
	QAction *exitAct;
	QAction *cutAct;
	QAction *copyAct;
	QAction *pasteAct;
	QAction *aboutAct;
	QAction *aboutQtAct;
};

#endif // VIEWER_H
