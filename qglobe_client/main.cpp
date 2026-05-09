#include <QApplication>
#include <QtWidgets/QSplashScreen>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
class QApplicationQt4Compat : public QApplication
{
public:
    using QApplication::QApplication;
    enum { UnicodeUTF8 = -1 };
};
#define QApplication QApplicationQt4Compat
#endif

#include "qglobeclient.h"
#include "mainwindow.h"
#include "guiuty.h"

#ifdef DEBUG
#include <crtdbg.h>
#endif

QGlobeClient *g_app;

int main(int argc, char *argv[])
{
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	QGlobeClient a(argc, argv);
    g_app = &a;

	if (a.isRunning())
		return 0;

    QPixmap pixMap(a.GetResource("/splash.png"));

	QSplashScreen splash(pixMap);

	splash.show();
	a.processEvents();

	splash.showMessage(QApplication::translate("QGlobeClient","Initializing Application..."),Qt::AlignCenter|Qt::AlignBottom,Qt::darkGray);
	a.processEvents();

	if (!a.Init())
	{
		gu_MessageBox(NULL, ("Error"), ("Occured error in initialization"), MSGBOX_ICONCRITICAL);
		return 0;
	}
	
    MainWindow mainWnd;

	a.SetMainWindow(&mainWnd);
	
	splash.showMessage(QApplication::translate("QGlobeClient","Creating Mainwindow..."),Qt::AlignCenter|Qt::AlignBottom,Qt::darkGray);
	a.processEvents();

	if (mainWnd.Init())
    {
		splash.showMessage(QApplication::translate("QGlobeClient","Initializing Mainwindow..."),Qt::AlignCenter|Qt::AlignBottom,Qt::darkGray);
		a.processEvents();

        mainWnd.InitUpdate();

		mainWnd.show();
		splash.finish(&mainWnd);
		mainWnd.AfterUpdate();
    }
    else
    {
		gu_MessageBox(NULL, QApplication::translate("QGlobeClient","Error"),QApplication::translate("QGlobeClient","Occured error in initialization"),MSGBOX_ICONCRITICAL);
		mainWnd.Release();
		return 0;
    }

	return a.exec();
}
