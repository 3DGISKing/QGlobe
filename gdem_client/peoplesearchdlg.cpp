#include "peoplesearchdlg.h"
#include "peopleinfodlg.h"

#include "guiuty.h"
#include "mainwindow.h"
#include "gdemclient.h"

#include <QSqlQuery>

PeopleSearchDlg::PeopleSearchDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.tblResult, SIGNAL(clicked(QModelIndex)), this, SLOT(tblResult_click(QModelIndex)));
	connect(ui.tblResult, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tblResult_doubleClick(QModelIndex)));
	connect(ui.cmbIssueClass1, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbIssueClass1_changed(int)));
	connect(ui.btnSearch, SIGNAL(clicked()), this, SLOT(btnSearch_click()));
	connect(ui.btnClear, SIGNAL(clicked()), this, SLOT(btnClear_click()));
	connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(close()));

	Init();  //asd add 2013.10.7
	this->setGeometry(QRect(100, 100, 650, 550));
}

PeopleSearchDlg::~PeopleSearchDlg()
{
	if(m_infoDlg) {
		m_infoDlg->close();
		delete m_infoDlg;
	}
	if(result_model)
		delete result_model;
}

void PeopleSearchDlg::reject()
{
	m_infoDlg->close();
	g_app->GetMainWindow()->UpdatePlaceDlg(true);
	QDialog::reject();
}

void PeopleSearchDlg::Init() 
{
	QSqlQuery qry;
	qry.exec("SELECT * FROM lst_sex ORDER BY id");
	while(qry.next())
		ui.cmbSex->addItem(qry.value(1).toString(), qry.value(2).toString());

	qry.exec("SELECT * FROM lst_party ORDER BY id");
	while(qry.next())
		ui.cmbParty->addItem(qry.value(1).toString(), qry.value(2).toString());

	qry.exec("SELECT * FROM lst_knowledge ORDER BY id");
	while(qry.next())
		ui.cmbKnowledge->addItem(qry.value(1).toString(), qry.value(2).toString());

	qry.exec("SELECT * FROM lst_origin ORDER BY id");
	while(qry.next())
		ui.cmbSocialOrg->addItem(qry.value(1).toString(), qry.value(2).toString());

	qry.exec("SELECT DISTINCT class1 FROM lst_issue ORDER BY id");
	while(qry.next())
		ui.cmbIssueClass1->addItem(qry.value(0).toString(), qry.value(0).toString());
	ui.cmbIssueClass1->addItem(tr("All"), "%");

	result_model = NULL;

	m_infoDlg = new PeopleInfoDlg(this);
}


void PeopleSearchDlg::cmbIssueClass1_changed(int id) 
{
	if(id == ui.cmbIssueClass1->count() - 1) return;
	ui.cmbIssueClass2->clear();
	QSqlQuery classQry;
	classQry.exec("SELECT class2 FROM lst_issue WHERE id=0 or class1='" + ui.cmbIssueClass1->itemText(id) + "' ORDER BY id");
	while (classQry.next())
		ui.cmbIssueClass2->addItem(classQry.value(0).toString(), classQry.value(0).toString());
}

void PeopleSearchDlg::btnSearch_click() 
{
	QString filter;
	QString strVal;
	int intVal;

	strVal = ui.edtName->text().trimmed();
	if(!strVal.isEmpty())
		filter = "per_text.name LIKE '%" + strVal + "%'";

	intVal = ui.cmbSex->currentIndex();
	if(intVal > 0) {
		if(filter.isEmpty())
			filter = "per_text.sex LIKE '%" + ui.cmbSex->itemData(intVal).toString() + "%'";
		else
			filter += " and per_text.sex LIKE '%" + ui.cmbSex->itemData(intVal).toString() + "%'";
	}

	int fromAge = ui.spnAgeFrom->value();
	int toAge = ui.spnAgeTo->value();

	if(!(fromAge == 0 && toAge == 0)) {
		if(fromAge > 0 && toAge == 0) toAge = 150;

		if(fromAge > toAge) {
			gu_MessageBox(NULL,tr("Missing"),tr("Missing Input Age Range !"), MSGBOX_ICONWARNING);
			ui.spnAgeFrom->setFocus();
		} else {
			QDate curDate = QDate::currentDate();
			QString fromDate = curDate.addYears(-fromAge).toString("yyyy-M-d");
			QString toDate = curDate.addYears(-toAge-1).toString("yyyy-M-d");
			if(filter.isEmpty())
				filter = "per_text.birthday >= '" + toDate + "' and per_text.birthday < '" + fromDate + "'";
			else
				filter += " and per_text.birthday >= '" + toDate + "' and per_text.birthday < '" + fromDate + "'";
		}
	}

	intVal = ui.cmbParty->currentIndex();
	if(intVal > 0) {
		strVal = ui.cmbParty->itemData(intVal).toString();
		if(filter.isEmpty())
			filter = "per_text.party LIKE '%" + strVal + "%'";
		else
			filter += " and per_text.party LIKE '%" + strVal + "%'";
	}

	intVal = ui.cmbKnowledge->currentIndex();
	if(intVal > 0) {
		strVal = ui.cmbKnowledge->itemData(intVal).toString();
		if(filter.isEmpty())
			filter = "per_text.knowledge LIKE '%" + strVal + "%'";
		else
			filter += " and per_text.knowledge LIKE '%" + strVal + "%'";
	}

	strVal = ui.edtBirthPlace->text().trimmed();
	strVal = strVal.replace(" ", "%");
	if(!strVal.isEmpty()) {
		if(filter.isEmpty())
			filter = "per_text.birthplace LIKE '%" + strVal + "%'";
		else
			filter += " and per_text.birthplace LIKE '%" + strVal + "%'";
	}

	strVal = ui.edtAddress->text().trimmed();
	strVal = strVal.replace(" ", "%");
	if(!strVal.isEmpty()) {
		if(filter.isEmpty())
			filter = "per_text.address LIKE '%" + strVal + "%'";
		else
			filter += " and per_text.address LIKE '%" + strVal + "%'";
	}

	strVal = ui.edtPost->text().trimmed();
	strVal = strVal.replace(" ", "%");
	if(!strVal.isEmpty()) {
		if(filter.isEmpty())
			filter = "per_text.position LIKE '%" + strVal + "%'";
		else
			filter += " and per_text.position LIKE '%" + strVal + "%'";
	}

	intVal = ui.cmbSocialOrg->currentIndex();
	if(intVal > 0) {
		strVal = ui.cmbSocialOrg->itemData(intVal).toString();
		if(filter.isEmpty())
			filter = "per_text.socialorigin LIKE '%" + strVal + "%'";
		else
			filter += " and per_text.socialorigin LIKE '%" + strVal + "%'";
	}

	//if(ui.chkRecycled->isChecked()) {
	//	if(filter.isEmpty())
	//		filter = "per_text.enddate IS NOT NULL";
	//	else
	//		filter += " and per_text.enddate IS NOT NULL";
	//}

	intVal = ui.cmbIssueClass1->currentIndex();
	if(intVal > 0) {
		strVal = ui.cmbIssueClass1->itemData(intVal).toString();
		if(filter.isEmpty())
			filter = "per_issue.class1 LIKE '%" + strVal + "%'";
		else
			filter += " and per_issue.class1 LIKE '%" + strVal + "%'";
	}

	intVal = ui.cmbIssueClass2->currentIndex();
	if(intVal > 0) {
		strVal = ui.cmbIssueClass2->itemData(intVal).toString();
		if(filter.isEmpty())
			filter = "per_issue.class2 LIKE '%" + strVal + "%'";
		else
			filter += " and per_issue.class2 LIKE '%" + strVal + "%'";
	}

	if(filter.isEmpty()) {
		gu_MessageBox(NULL, tr("Search Result"), tr("Input Search Condition !"), MSGBOX_ICONINFO);
		ui.edtName->setFocus();
		return;
	}

	//ui.btnToHouse->setEnabled(false);
	//ui.btnToBuilding->setEnabled(false);

	QString sql = "select per_text.cardno, per_text.name, per_text.sex, per_text.birthday, per_text.party, ";
	sql += "per_text.knowledge, per_text.birthplace, per_text.address, per_text.position, per_text.socialorigin, ";
	//if(ui.chkRecycled->isChecked()) sql += "per_text.enddate, "; else sql += "per_text.startdate, ";
	sql += "per_issue.class1, per_issue.class2 ";
	sql += "from per_text left join per_issue on per_text.cardno=per_issue.cardno ";
	sql += "where " + filter;
	//if(ui.chkRecycled->isChecked()) sql += " and per_text.enddate IS NOT NULL"; else sql += " and per_text.enddate IS NULL";
	sql += " and per_text.enddate IS NULL";

	if(result_model) {
		delete result_model;
		result_model = NULL;
	}

	result_model = new QSqlQueryModel();
	result_model->setQuery(sql);

	ui.tblResult->setModel(result_model);

	result_model->setHeaderData(0, Qt::Horizontal, tr("cardno"));
	result_model->setHeaderData(1, Qt::Horizontal, tr("Name"));
	result_model->setHeaderData(2, Qt::Horizontal, tr("Sex"));
	result_model->setHeaderData(3, Qt::Horizontal, tr("Birthday"));
	result_model->setHeaderData(4, Qt::Horizontal, tr("Party"));
	result_model->setHeaderData(5, Qt::Horizontal, tr("Knowledge"));
	result_model->setHeaderData(6, Qt::Horizontal, tr("BirthPlace"));
	result_model->setHeaderData(7, Qt::Horizontal, tr("Address"));
	result_model->setHeaderData(8, Qt::Horizontal, tr("Position"));
	result_model->setHeaderData(9, Qt::Horizontal, tr("SocialOrg"));
	//if(ui.chkRecycled->isChecked()) 
	//	result_model->setHeaderData(10, Qt::Horizontal, tr("EndDate"));
	//else 
		result_model->setHeaderData(10, Qt::Horizontal, tr("StartDate"));
	result_model->setHeaderData(11, Qt::Horizontal, tr("IssueCls1"));
	result_model->setHeaderData(12, Qt::Horizontal, tr("IssueCls2"));
	ui.tblResult->setModel(result_model);
	ui.tblResult->setColumnHidden(0,true);

	if(result_model->rowCount() == 0)	{
		gu_MessageBox(NULL, tr("Search Result"), tr("No Result"), MSGBOX_ICONINFO);
		return;
	}

	ui.tblResult->resizeColumnsToContents();
}

void PeopleSearchDlg::tblResult_click(QModelIndex id) 
{
	cardno = id.sibling(id.row(), 0).data().toInt();
}

void PeopleSearchDlg::tblResult_doubleClick(QModelIndex id) 
{
	if(m_infoDlg)
		delete m_infoDlg;
	m_infoDlg = new PeopleInfoDlg(this);
	m_infoDlg->readData(cardno);
	m_infoDlg->show();
}

void PeopleSearchDlg::btnClear_click() 
{
	ui.edtName->setText("");
	ui.cmbSex->setCurrentIndex(0);
	ui.spnAgeFrom->setValue(0);
	ui.spnAgeTo->setValue(0);
	ui.cmbParty->setCurrentIndex(0);
	ui.cmbKnowledge->setCurrentIndex(0);
	ui.edtBirthPlace->setText("");
	ui.edtAddress->setText("");
	ui.edtPost->setText("");
	ui.cmbSocialOrg->setCurrentIndex(0);
	//ui.chkRecycled->setChecked(false);
	ui.cmbIssueClass1->setCurrentIndex(0);
	ui.cmbIssueClass2->setCurrentIndex(0);
}
