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
	Resize();
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

void Viewer::on_spaceBorderButton_clicked(bool checked)
{
	settings::border_[Box::BoxType::SPACE] = checked;
}

void Viewer::on_charBorderButton_clicked(bool checked)
{
	settings::border_[Box::BoxType::CHAR] = checked;
}

void Viewer::on_wordBorderButton_clicked(bool checked)
{
	settings::border_[Box::BoxType::WORD] = checked;
}

void Viewer::on_lineBorderButton_clicked(bool checked)
{
	settings::border_[Box::BoxType::LINE] = checked;
}

void Viewer::on_pageBorderButton_clicked(bool checked)
{
	settings::border_[Box::BoxType::PAGE] = checked;
}

void Viewer::on_riverButton_clicked(bool checked)
{
	settings::show_river_ = checked;
}

void Viewer::on_fontSizeBox_valueChanged(int arg1)
{
	settings::font_size_ = arg1;
	typesetter.LoadFace();
}

void Viewer::on_alignBox_currentIndexChanged(int index)
{
	settings::align_mode_ = static_cast<settings::AlignMode>(index);
}