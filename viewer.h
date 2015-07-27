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

	void init();
	static void Message(const string& msg);

protected:
	void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;

private slots:
	void on_pageSlider_valueChanged(int value);

	void newFile();
	void open();
	bool save();
	bool saveAs();
	void documentWasModified();
	void about();
	void render();

	void setMarkdownR(bool checked);
	void setMarkdownDemerits(bool checked);

private:
	Ui::viewerClass ui;
	GLWidget* glwidget_;
	Typesetter typesetter_;
	
	void readSettings();
	void writeSettings();
	bool maybeSave();
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);

	QsciScintilla *textEdit;
	QString curFile;
};

#endif // VIEWER_H
