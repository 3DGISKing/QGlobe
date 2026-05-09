#include <QApplication>
#include "qglobeds_ctrldialog.h"
#include "qglobeds_servicetrayicon.h"
#include <qglobal.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#ifdef Q_OS_WIN
#if USE_DIALOG
	QGlobeDSServiceTrayIcon trayIcon(&a, NULL);
#else
	QGlobeDSServiceTrayIcon trayIcon(&a);
#endif
#else
    CtrlDialog w;
    w.show();
#endif

    return a.exec();
}
