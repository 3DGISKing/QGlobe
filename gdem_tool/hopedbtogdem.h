#ifndef HOPEDBTOGDEM_H
#define HOPEDBTOGDEM_H

#include <QDialog>
#include "ui_hopedbtogdem.h"

class HopeDBToGDem : public QDialog
{
	Q_OBJECT

public:
	HopeDBToGDem(QWidget *parent = 0);
	~HopeDBToGDem();
	
private:
	Ui::HopeDBToGDemClass ui;

	QString     _strInputIEEIDBPath;
	QString     _strSaveGTPath;

	void        Init();
	QString     getGtFileNameFromString(QString str);
	int         getNumberFromString(QString str);
	int         getLevelFromString(QString str);
	void        renameAndCopyToTmp(QString lst);
	void        makeGtFileFromJpgFiles(QStringList* lst);

private slots:
	void        BrowserInput();
	void        BrowserSave();
	void        Convert();
};

#endif // HOPEDBTOGDEM_H
