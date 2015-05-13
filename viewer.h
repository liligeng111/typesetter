#ifndef VIEWER_H
#define VIEWER_H

#include <QtWidgets/QMainWindow>
#include "ui_viewer.h"
#include "typesetter.h"
#include "settings.h"

using namespace std;

class Viewer : public QMainWindow
{
	Q_OBJECT

public:
	Viewer(QWidget *parent = 0);
	~Viewer();


	static void Message(const string& msg);

private slots:
	void on_renderButton_clicked();
	void on_spaceBorderButton_clicked(bool checked);
	void on_charBorderButton_clicked(bool checked);
	void on_wordBorderButton_clicked(bool checked);
	void on_lineBorderButton_clicked(bool checked);
	void on_pageBorderButton_clicked(bool checked);
	void on_riverButton_clicked(bool checked);
	void on_fontSizeBox_valueChanged(int arg1);
	void on_riverThresholdSpinBox_valueChanged(double arg1);
	void on_paperBox_currentIndexChanged(QString page);

    void on_actionOpen_File_triggered();

private:
	Ui::viewerClass ui;
	Typesetter typesetter;
	string content;

	void Resize();
};

#endif // VIEWER_H
