#include "viewer.h"
#include "typesetter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Viewer w;
	w.init();
	w.show();
	return a.exec();
}
