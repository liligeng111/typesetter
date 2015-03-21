#ifndef VIEWER_H
#define VIEWER_H

#include <QtWidgets/QMainWindow>
#include "ui_viewer.h"

class Viewer : public QMainWindow
{
	Q_OBJECT

public:
	Viewer(QWidget *parent = 0);
	~Viewer();

private:
	Ui::viewerClass ui;
};

#endif // VIEWER_H
