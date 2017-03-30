#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <qfiledialog.h>

class FileDialog : public QFileDialog
{
public:
	FileDialog(QWidget *parent=0, const QString &caption=QString(),
			   const QString &dir=QString(), const QString &filter=QString());

	static QString GetOpenFileName(QWidget *parent=0, const QString &caption=QString(),
										const QString &dire=QString(),
										const QString &filter=QString(),
										QString * selectedFilter=0, Options options=0);

	static QStringList GetOpenFileNames(QWidget *parent=0, const QString &caption=QString(),
										const QString &dire=QString(),
										const QString &filter=QString(),
										QString * selectedFilter=0, Options options=0);

	static QString GetSaveFileName (QWidget *parent=0, const QString &caption=QString(),
									const QString &dir=QString(),
									const QString &filter=QString(),
									QString *selectedFilter=0, Options options=0);

};

#endif // FILEDIALOG_H
