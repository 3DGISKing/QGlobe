#ifndef BUILDINGPROPDLG_H
#define BUILDINGPROPDLG_H

#include <QDialog>
#include "ui_buildingpropdlg.h"
#include "building.h"


#define BIRTHDAY_COLNUMBER_IN_TB_PEOPLE 3


class QSqlQueryModel;

class BuildingPropDlg : public QDialog
{
	Q_OBJECT

public:
	BuildingPropDlg(QWidget *parent = 0);
	~BuildingPropDlg();

	void setBuildingProp(GeoBuildingProperty* p);

private:
	Ui::BuildingPropDlg ui;

	qint64 buildingid;

	QSqlQueryModel* h_model;
	QSqlQueryModel* b_model;
	QSqlQueryModel* p_model;

	bool refeshFlag;

private:
	bool peopleToHouse(QString);
	bool peopleToBuilding(QString);
	bool RemoveToHouse(QString);
	bool RemoveToBuilding(QString);

protected:
	virtual void showEvent(QShowEvent *event);
	virtual void reject();

private slots:
	void refresh();
	void showDetail(QModelIndex);
	void btnAddPeople_click();
	void btnDelPeople_click();
	void btnCondition_click();
	void btnClear_click();
	void btnSearch_click();
	void btnToHouse_click();
	void btnToBuilding_click();
};

#endif // BUILDINGPROPDLG_H
