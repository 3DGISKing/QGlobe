#include "qglobe_tool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qglobe_tool w;
	w.show();
	return a.exec();
}
