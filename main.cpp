#include "viewer.h"
#include "typesetter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Viewer w;
	Typesetter typesetter;
	w.show();
	return a.exec();
}
