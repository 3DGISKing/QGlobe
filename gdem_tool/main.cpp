#include "gdem_tool.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	gdem_tool w;
	w.show();
	return a.exec();
}
