#ifndef VIEWER_H
#define VIEWER_H

#include <QtWidgets/QMainWindow>
#include "ui_viewer.h"
#include "typesetter.h"

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

private:
	Ui::viewerClass ui;
	Typesetter typesetter;
};

#endif // VIEWER_H
