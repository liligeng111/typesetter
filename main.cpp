#include "viewer.h"
#include "typesetter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Viewer w;
	w.show();
	return a.exec();
}
