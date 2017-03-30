#include "filedialog.h"
#ifdef GDEM_CLIENT
#include "gdemclient.h"
#define TR(t)			g_app->Trans(t)
#define TransText(t)	g_app->Trans(t)
#else
#define TR(t)			(t)
#define TransText(t)	(t)
#endif

#define STR_FILE_LOOKIN			TR("Look In")
#define STR_FILE_FNAME			TR("File Name")
#define STR_FILE_FTYPE			TR("File Type")
#define STR_FILE_OPEN			TR("Open")
#define STR_FILE_SAVE			TR("Save")
#define STR_FILE_FILTERALL		TR("All Files (*)")
#define STR_FILE_OK				TR("OK")
#define STR_FILE_CANCEL			TR("Cancel")

FileDialog::FileDialog(QWidget *parent, const QString &caption, const QString &directory,
					   const QString &filter) :
			QFileDialog(parent, caption, directory, filter)
{
	if (filter.isEmpty())
		setNameFilter(STR_FILE_FILTERALL);
}

QString FileDialog::GetOpenFileName(QWidget *parent, const QString &caption,
										const QString &dir, const QString &filter,
										QString * selectedFilter, Options options)
{
	QString fileName;
	FileDialog dlg(parent, caption, dir, filter);
	dlg.setAcceptMode(QFileDialog::AcceptOpen);

	if (QDialog::Accepted == dlg.exec())
	{
		QStringList files = dlg.selectedFiles();
		fileName = files.at(0);
	}

	return fileName;
}

QStringList FileDialog::GetOpenFileNames(QWidget *parent, const QString &caption,
										const QString &dir, const QString &filter,
										QString * selectedFilter, Options options)
{
	QStringList fileNames;
	FileDialog dlg(parent, caption, dir, filter);

	dlg.setAcceptMode(QFileDialog::AcceptOpen);
	dlg.setFileMode(QFileDialog::ExistingFiles);

	if (QDialog::Accepted == dlg.exec())
		fileNames = dlg.selectedFiles();

	return fileNames;
}

QString FileDialog::GetSaveFileName (QWidget *parent, const QString &caption,
									 const QString &dir, const QString &filter,
									 QString *selectedFilter, Options options)
{
	QString fileName;
	FileDialog dlg(parent, caption, dir, filter);

	dlg.setAcceptMode(QFileDialog::AcceptSave);

	if (QDialog::Accepted == dlg.exec())
	{
		QStringList files = dlg.selectedFiles();
		fileName = files.at(0);
	}

	return fileName;
}
