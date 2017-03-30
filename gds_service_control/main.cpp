#include <QtGui/QApplication>
#include "gds_ctrldialog.h"
#include "gds_servicetrayicon.h"
#include <qglobal.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#ifdef Q_WS_WIN
#if USE_DIALOG
	GDSServiceTrayIcon trayIcon(&a, NULL);
#else
	GDSServiceTrayIcon trayIcon(&a);
#endif
#else
    CtrlDialog w;
    w.show();
#endif

    return a.exec();
}
