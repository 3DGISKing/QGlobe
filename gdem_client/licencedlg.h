#ifndef LICENCEDLG_H
#define LICENCEDLG_H

#include <QDialog>
#include "ui_licencedlg.h"

class LicenceDlg : public QDialog
{
	Q_OBJECT

public:
	LicenceDlg(QWidget *parent = 0);
	~LicenceDlg();

private:
	Ui::LicenceDlg ui;

	int trynumber;
private slots:
	void setLicence();
};

#endif // LICENCEDLG_H
