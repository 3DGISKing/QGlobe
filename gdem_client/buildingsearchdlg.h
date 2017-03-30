#ifndef BUILDINGSEARCHDLG_H
#define BUILDINGSEARCHDLG_H

#include <QDialog>
#include "ui_buildingsearchdlg.h"

class BuildingQueryModel;

class BuildingSearchDlg : public QDialog
{
	Q_OBJECT

public:
	BuildingSearchDlg(QWidget *parent = 0);
	~BuildingSearchDlg();

	void Refresh(QString filter);

private:
	Ui::BuildingSearchDlg ui;

	BuildingQueryModel* model;

protected:
	virtual void reject();
	QString FindByLocation();
	QString FindByFloor();
private slots:
	void Find();  
	void Goto(const QModelIndex&);
	void OnProvinceChange(int);
	void OnCountyChange(int);
};

#endif // BUILDINGSEARCHDLG_H
