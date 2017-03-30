#ifndef PEOPLEINFODLG_H
#define PEOPLEINFODLG_H

#include <QDialog>
#include "ui_peopleinfodlg.h"
#include <QSqlRelationalTableModel>
#include "../gds_database/GDSDatabaseInterface.h"
#include "../gds_database/GDSGeoNameDBManager.h"

#include <QSqlQuery>

#define ISSUE_ALL	"All"

class PeopleSearchDlg;
class QStandardItemModel;

class PeopleInfoDlg : public QDialog
{
	Q_OBJECT

public:
	PeopleInfoDlg(QWidget *parent = 0);
	~PeopleInfoDlg();

	void readData(int cardno);

private:
	Ui::PeopleInfoDlg ui;

	void Init(); 
	void Format(); 

	QStandardItemModel *history_model; 
	QStandardItemModel *family_model; 
	QStandardItemModel *grasp_model; 

	PeopleSearchDlg *m_parent;
	QSqlQuery qry;

	void Goto(qint64 id);

	qint64 houseid; //asd add 2013.10.7
	qint64 buildingid; //asd add 2013.10.7

private slots:
	void btnHisFam_click();
	void btnIssue_click();
	void btnGrasp_click();

	void btnToHouse_click(); //asd add 2013.10.7
	void btnToBuilding_click(); //asd add 2013.10.7
};

#endif // PEOPLEINFODLG_H
