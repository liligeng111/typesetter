#include "viewer.h"
#include <qmessagebox.h>
#include <QSvgRenderer>
#include <qpainter>
#include "settings.h"


Viewer::Viewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	showMaximized();
	on_renderButton_clicked();
	// for some reason, you cannot get screen size in constructor, so go with 1920.
	ui.label->setGeometry((1920 - settings::display_width()) / 2, 20, settings::display_width(), settings::display_height());
}

Viewer::~Viewer()
{

}

void Viewer::Message(const string& msg)
{
	QMessageBox::critical(NULL, "Error", QString::fromStdString(msg));
}

void Viewer::Resize()
{
	ui.label->setGeometry((geometry().width() - settings::display_width()) / 2, 20, settings::display_width(), settings::display_height());
}

void Viewer::on_renderButton_clicked()
{
	string content = ui.plainTextEdit->toPlainText().toStdString();
	typesetter.set_content(content);
	typesetter.Typeset();
	typesetter.Render(Typesetter::SVG);

	QSvgRenderer renderer(QString("./output.svg"));
	QImage image(settings::display_width(), settings::display_height(), QImage::Format_ARGB32);
	//background
	image.fill(0xffffffff);
	//paint svg
	QPainter painter(&image);
	renderer.render(&painter);
	ui.label->setPixmap(QPixmap::fromImage(image));
	ui.label->update();
}