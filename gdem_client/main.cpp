#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>

#include "gdemclient.h"
#include "mainwindow.h"
#include "guiuty.h"

#ifdef DEBUG
#include <crtdbg.h>
#endif

GDemClient *g_app;

int main(int argc, char *argv[])
{
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	GDemClient a(argc, argv);
    g_app = &a;

	if (a.isRunning())
		return 0;

    QPixmap pixMap(a.GetResource("/splash.png"));

	QSplashScreen splash(pixMap);

	splash.show();
	a.processEvents();

	splash.showMessage(QApplication::translate("GDemClient","Initializing Application...",0,QApplication::UnicodeUTF8),Qt::AlignCenter|Qt::AlignBottom,Qt::darkGray);
	a.processEvents();

	if (!a.Init())
	{
		gu_MessageBox(NULL, ("Error"), ("Occured error in initialization"), MSGBOX_ICONCRITICAL);
		return 0;
	}
	
    MainWindow mainWnd;

	a.SetMainWindow(&mainWnd);
	
	splash.showMessage(QApplication::translate("GDemClient","Creating Mainwindow...",0,QApplication::UnicodeUTF8),Qt::AlignCenter|Qt::AlignBottom,Qt::darkGray);
	a.processEvents();

	if (mainWnd.Init())
    {
		splash.showMessage(QApplication::translate("GDemClient","Initializing Mainwindow...",0,QApplication::UnicodeUTF8),Qt::AlignCenter|Qt::AlignBottom,Qt::darkGray);
		a.processEvents();

        mainWnd.InitUpdate();

		mainWnd.show();
		splash.finish(&mainWnd);
		mainWnd.AfterUpdate();
    }
    else
    {
		gu_MessageBox(NULL, QApplication::translate("GDemClient","Error",0,QApplication::UnicodeUTF8),QApplication::translate("GDemClient","Occured error in initialization",0,QApplication::UnicodeUTF8),MSGBOX_ICONCRITICAL);
		mainWnd.Release();
		return 0;
    }

	return a.exec();
}
