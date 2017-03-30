#ifndef WRITEREMOVEJGW_H
#define WRITEREMOVEJGW_H

#include <QDialog>
#include "ui_writeremovejgw.h"

class WriteRemoveJGW : public QDialog
{
	Q_OBJECT

public:
	WriteRemoveJGW(QWidget *parent = 0);
	~WriteRemoveJGW();
	
private slots:
	void OpenFolder();
	void Run();
	void SetJGWRemoveFlag(void);
private:
	Ui::WriteRemoveJGWClass ui;

	QString _OpenPath;

	bool	m_removeflag;

private:
	void removeJGWfiles();
	void writeJGWfiles();
};

#endif // WRITEREMOVEJGW_H
