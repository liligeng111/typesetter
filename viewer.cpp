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


void Viewer::on_renderButton_clicked()
{
	string content = ui.plainTextEdit->toPlainText().toStdString();
	typesetter.set_content(content);
	typesetter.Typeset();
	typesetter.Render(Typesetter::SVG);
}