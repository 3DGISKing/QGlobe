#include <QtGui/QApplication>
#if 0 /* Old Dialog */
#include "configdialog.h"
#else /* New Dialog */
#include "configdialog2.h"
#endif
#include <QTranslator>
 #include <QCoreApplication>

#define APP_TRANS_FNAME		"/gdemserverconfig.qm"

QString stylesheet = " * {font: 10pt \"PRK P Gothic\"}";

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QTranslator translator;
	QString transFile = QCoreApplication::applicationDirPath()+ APP_TRANS_FNAME;
	translator.load(transFile);
	app.installTranslator(&translator);
	app.setStyleSheet(stylesheet);

#if 0 /* Old Dialog */
	ConfigDialog dialog;
#else /* New Dialog */
	ConfigDialog2 dialog;
#endif
	if (!dialog.Init())
		return 0;

	dialog.show();
	return app.exec();
}
