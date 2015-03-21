#include "viewer.h"
#include <qmessagebox.h>


Viewer::Viewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

Viewer::~Viewer()
{

}

void Viewer::Message(const string& msg)
{
	QMessageBox::critical(NULL, "Error", QString::fromStdString(msg));
}