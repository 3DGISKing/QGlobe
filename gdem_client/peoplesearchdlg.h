#ifndef PEOPLESEARCHDLG_H
#define PEOPLESEARCHDLG_H

#include <QDialog>
#include "ui_peoplesearchdlg.h"

class PeopleInfoDlg;
class QSqlQueryModel;

class PeopleSearchDlg : public QDialog
{
	Q_OBJECT

public:
	PeopleSearchDlg(QWidget *parent = 0);
	~PeopleSearchDlg();

private:
	Ui::PeopleSearchDlg ui;

	void Init();
	void Format();

	PeopleInfoDlg *m_infoDlg;

	QSqlQueryModel *result_model; 

	//int curretRow; 
	int cardno;

protected:
	virtual void reject();

private slots:
	void btnClear_click(); //asd add 2013.10.7
	void btnSearch_click(); //asd add 2013.10.7
	void tblResult_click(QModelIndex); //asd add 2013.10.7
	void tblResult_doubleClick(QModelIndex); //asd add 2013.10.7
	void cmbIssueClass1_changed(int);
};

#endif // PEOPLESEARCHDLG_H
