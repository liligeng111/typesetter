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
	void on_alignBox_currentIndexChanged(int index);

private:
	Ui::viewerClass ui;
	Typesetter typesetter;

	void Resize();
};

#endif // VIEWER_H
