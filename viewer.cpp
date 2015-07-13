#include "viewer.h"
#include <qmessagebox.h>
#include <QSvgRenderer>
#include <qpainter>
#include <qfiledialog.h>
#include "settings.h"
#include <fstream>
#include <streambuf>
#include "container.h"



Viewer::Viewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	content = "input/sample.txt";
	//showMaximized();
	// for some reason, you cannot get screen size in constructor, so go with 1920.
	ui.label->setGeometry((1920 - settings::display_width()) / 2, 20, settings::display_width(), settings::display_height());
	update_UI();
}

Viewer::~Viewer()
{

}

void Viewer::update_UI()
{
	ui.fontSizeBox->setValue(settings::font_size_);
	ui.expansionBox->setCurrentIndex(settings::expansion_mode_);
	ui.expansionSpinBox->setValue(settings::max_expansion_);

	ui.magicSpinBox->setValue(settings::max_magic_amount_);
	ui.magicGainSpinBox->setValue(settings::min_magic_gain_);
}

void Viewer::Message(const string& msg)
{
	QMessageBox::critical(nullptr, "Error", QString::fromLocal8Bit(msg.c_str()));
}

void Viewer::resize()
{
	ui.label->setGeometry((geometry().width() - settings::display_width()) / 2, 20, settings::display_width(), settings::display_height());
}

void Viewer::on_renderButton_clicked()
{
	resize();
	typesetter.set_content(content);
	typesetter.Typeset();
	//typesetter.render(Typesetter::SVG);

	cout << "Total Page Count:" << typesetter.page_count() << endl;
	ui.pageSlider->setMaximum(typesetter.page_count() - 1);
	ui.pageSlider->setValue(0);
	//on_pageSlider_valueChanged(0);
	nvpr_renderer.render_page(typesetter.page(0), 0);
	nvpr_renderer.start_main_loop();
}


void Viewer::on_pageSlider_valueChanged(int value)
{
	nvpr_renderer.render_page(typesetter.page(value), value);
}

void Viewer::on_spaceBorderButton_clicked(bool checked)
{
	settings::border_[Item::GLUE] = checked;
}

void Viewer::on_charBorderButton_clicked(bool checked)
{
	settings::border_[Item::BOX] = checked;
}

void Viewer::on_wordBorderButton_clicked(bool checked)
{
	settings::border_[Container::WORD] = checked;
}

void Viewer::on_lineBorderButton_clicked(bool checked)
{
	settings::border_[Container::LINE] = checked;
}

void Viewer::on_pageBorderButton_clicked(bool checked)
{
	settings::border_[Container::PAGE] = checked;
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

void Viewer::on_riverThresholdSpinBox_valueChanged(double arg1)
{
	settings::river_threshold_ = arg1;
}

void Viewer::on_actionOpen_File_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",tr("*"));
	
	//ifstream t(fileName);
	//content = string((std::istreambuf_iterator<char>(t)),
	//				 std::istreambuf_iterator<char>());
	//ui.plainTextEdit->setPlainText(QString::fromStdString(content));
	//for now
	string file = fileName.toUtf8().constData();
	cout << "Reading file: " << file << endl;
	content = file;
}

void Viewer::on_markdownBox_currentIndexChanged(int index)
{
	settings::markdown_type_ = index;
	nvpr_renderer.render_page(typesetter.page(ui.pageSlider->tickPosition()), ui.pageSlider->tickPosition());
}

void Viewer::on_paperBox_currentIndexChanged(QString page)
{
	if (page == "A7")
	{
		settings::page_width_ = 74;
		settings::page_height_ = 105;
	}
	else if (page == "A6")
	{
		settings::page_width_ = 105;
		settings::page_height_ = 148;
	}
	else if (page == "A5")
	{
		settings::page_width_ = 148;
		settings::page_height_ = 210;
	}
	else if (page == "A4")
	{
		settings::page_width_ = 210;
		settings::page_height_ = 297;
	}
	else if (page == "A3")
	{
		settings::page_width_ = 297;
		settings::page_height_ = 420;
	}
	else if (page == "A2")
	{
		settings::page_width_ = 420;
		settings::page_height_ = 594;
	}
	else if (page == "B7")
	{
		settings::page_width_ = 88;
		settings::page_height_ = 125;
	}
	else if (page == "B6")
	{
		settings::page_width_ = 125;
		settings::page_height_ = 176;
	}
	else if (page == "B5")
	{
		settings::page_width_ = 176;
		settings::page_height_ = 250;
	}
	else if (page == "B4")
	{
		settings::page_width_ = 250;
		settings::page_height_ = 353;
	}
	else if (page == "B3")
	{
		settings::page_width_ = 353;
		settings::page_height_ = 500;
	}
	else if (page == "B2")
	{
		settings::page_width_ = 500;
		settings::page_height_ = 707;
	}
}

void Viewer::on_expansionBox_currentIndexChanged(int index)
{
	settings::expansion_mode_ = index;
}

void Viewer::on_expansionSpinBox_valueChanged(double arg1)
{
	settings::max_expansion_ = arg1;
}


void Viewer::on_useMagicBox_clicked(bool checked)
{
	settings::use_magic_ = checked;
}

void Viewer::on_magicSpinBox_valueChanged(double arg1)
{
	settings::max_magic_amount_ = arg1;
}

void Viewer::on_magicGainSpinBox_valueChanged(double arg1)
{
	settings::min_magic_gain_ = arg1;
}