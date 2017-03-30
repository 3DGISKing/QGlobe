#include <QSqlQuery>
#include <QSqlQueryModel>
#include "guiuty.h"
#include "buildingpropdlg.h"
#include "../gds_database/GDSDatabaseInterface.h"
#include "../gds_database/GDSGeoNameDBManager.h"
#include "gdemclient.h"
#include "mainwindow.h"

BuildingPropDlg::BuildingPropDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnAddPeople, SIGNAL(clicked()), this, SLOT(btnAddPeople_click()));
	connect(ui.btnDelPeople, SIGNAL(clicked()), this, SLOT(btnDelPeople_click()));
	connect(ui.btnCondition, SIGNAL(clicked()), this, SLOT(btnCondition_click()));
	connect(ui.btnClear, SIGNAL(clicked()), this, SLOT(btnClear_click()));
	connect(ui.btnSearch, SIGNAL(clicked()), this, SLOT(btnSearch_click()));
	connect(ui.btnToHouse, SIGNAL(clicked()), this, SLOT(btnToHouse_click()));
	connect(ui.btnToBuilding, SIGNAL(clicked()), this, SLOT(btnToBuilding_click()));
	connect(ui.cmbB_Posts, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh()));
	connect(ui.cmbB_Ban, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh()));
	connect(ui.tblEmplyee, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showDetail(QModelIndex)));
	connect(ui.tblPopulation, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showDetail(QModelIndex)));

	h_model=NULL;
	b_model=NULL;
	p_model=NULL;

	ui.grpPeople->setVisible(false);
	this->setMaximumWidth(400);
	this->resize(400, 600);
	this->setMaximumWidth(16777215);
 }

BuildingPropDlg::~BuildingPropDlg()
{
	if(h_model)
		delete h_model;
	if(b_model)
		delete b_model;
	if(p_model)
		delete p_model;
}

#include "GDM_SceneManager.h"
#include "Geo3dObjectSceneNode.h"

void BuildingPropDlg::setBuildingProp(GeoBuildingProperty* p)
{
	g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pGeo3dObjectNode->SetSelectedID(p->_ID);

	buildingid = p->_ID;

	ui.edtB_Name->setText(p->_Name);
	ui.edtB_Floor->setText(QString("%1").arg(p->_Floor));

	ui.cmbB_Posts->clear(); 
	ui.cmbB_Posts->addItems(p->_PostName.split(","));

	ui.cmbB_Ban->clear(); 
	ui.cmbB_Ban->addItems(p->_Ban.split(","));

	ui.edtB_Address->setText(p->_Address);
}
void BuildingPropDlg::reject()
{
	g_app->GetMainWindow()->UpdatePlaceDlg(true);
	g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pGeo3dObjectNode->SetSelectedID(-1);
	QDialog::reject();
}

void BuildingPropDlg::showEvent(QShowEvent* event)
{
	QPoint mainPos = g_app->GetMainWindow()->pos();
	move(mainPos.x()+20, mainPos.y()+50);
	g_app->GetMainWindow()->UpdatePlaceDlg(false);
	ui.tabB_Peoples->setCurrentIndex(0);
	ui.btnAddPeople->setChecked(false);
	btnAddPeople_click();
	refeshFlag = false;
	QDialog::showEvent(event);
}

void BuildingPropDlg::btnAddPeople_click()
{
	if(ui.btnAddPeople->isChecked()) {
		ui.grpPeople->setVisible(true);
		this->setMaximumWidth(800);
		this->resize(800, 600);
		this->setMaximumWidth(16777215);
	} else {
		ui.grpPeople->setVisible(false);
		this->setMaximumWidth(400);
		this->resize(400, 600);
		this->setMaximumWidth(16777215);
		btnClear_click();
		ui.tblResult->setModel(NULL);
	}

}

void BuildingPropDlg::btnCondition_click()
{
	ui.edtP_Post->setText(ui.edtB_Name->text());
	//ui.edtP_Address->setText(ui.edtB_Address->text());
}

void BuildingPropDlg::btnClear_click()
{
	ui.edtP_Name->setText("");
	ui.spnP_AgeFrom->setValue(0);
	ui.spnP_AgeTo->setValue(0);
	ui.edtP_Post->setText("");
	ui.edtP_Address->setText("");
}

void BuildingPropDlg::btnSearch_click()
{
	QString filter = "";
	QString strVal;
	int intVal;

	strVal = ui.edtP_Name->text().trimmed();
	if(!strVal.isEmpty())
		filter = "per_text.name LIKE '%" + strVal + "%'";

	int fromAge = ui.spnP_AgeFrom->value();
	int toAge = ui.spnP_AgeTo->value();

	if(!(fromAge == 0 && toAge == 0)) {
		if(fromAge > 0 && toAge == 0) toAge = 150;

		if(fromAge > toAge) {
			gu_MessageBox(NULL,tr("Missing"),tr("Missing Input Age Range !"), MSGBOX_ICONWARNING);
			ui.spnP_AgeFrom->setFocus();
		} else {
			QDate curDate = QDate::currentDate();
			QString fromDate = curDate.addYears(-fromAge).toString("yyyy/M/d");
			QString toDate = curDate.addYears(-toAge-1).toString("yyyy/M/d");
			if(filter.isEmpty())
				filter = "per_text.birthday >= '" + toDate + "' and per_text.birthday < '" + fromDate + "'";
			else
				filter += " and per_text.birthday >= '" + toDate + "' and per_text.birthday < '" + fromDate + "'";
		}
	}

	strVal = ui.edtP_Post->text().trimmed();
	strVal = strVal.replace(" ", "%");
	if(!strVal.isEmpty()) {
		if(filter.isEmpty())
			filter = "per_text.position LIKE '%" + strVal + "%'";
		else
			filter += " and per_text.position LIKE '%" + strVal + "%'";
	}

	strVal = ui.edtP_Address->text().trimmed();
	strVal = strVal.replace(" ", "%");
	if(!strVal.isEmpty()) {
		if(filter.isEmpty())
			filter = "per_text.address LIKE '%" + strVal + "%'";
		else
			filter += " and per_text.address LIKE '%" + strVal + "%'";
	}

	if(filter.isEmpty()) {
		if(!refeshFlag) gu_MessageBox(NULL, tr("Search Result"), tr("Input Search Condition !"), MSGBOX_ICONINFO);
		ui.edtP_Name->setFocus();
		refeshFlag = false; 
		return;
	}

	if(ui.chkP_PreAdd->isChecked()) {
		if(filter.isEmpty())
			filter = "(tb_people.buildingid IS NULL or tb_people.buildingid='')"; //tb_people.houseid IS NULL or 
		else
			filter += " and (tb_people.buildingid IS NULL or tb_people.buildingid='')"; //tb_people.houseid IS NULL or 
	} else {
		if(filter.isEmpty())
			filter = "(tb_people.buildingid IS NOT NULL or tb_people.buildingid<>'')"; //tb_people.houseid IS NOT NULL and 
		else
			filter += " and (tb_people.buildingid IS NOT NULL or tb_people.buildingid<>'')"; //tb_people.houseid IS NOT NULL and 
	}

	//ui.btnToHouse->setEnabled(false);
	//ui.btnToBuilding->setEnabled(false);

	QString sql = "select per_text.cardno, tb_people.houseid, tb_people.buildingid, per_text.name, ";
	sql += "per_text.sex, per_text.birthday, per_text.address, per_text.position ";
	sql += "from per_text left join tb_people on per_text.cardno=tb_people.cardno ";
	sql += "where " + filter;
	sql += " and per_text.enddate IS NULL";

	if(p_model) {
		delete p_model;
		p_model = NULL;
	}

	p_model = new QSqlQueryModel();
	p_model->setQuery(sql);

	bool reQueryFlag = false;
	ui.tblResult->setModel(p_model);

	ui.tblResult->setColumnHidden(0,true);
	ui.tblResult->setColumnHidden(1,true);
	ui.tblResult->setColumnHidden(2,true);
	//p_model->setHeaderData(1, Qt::Horizontal, tr("house"));
	//p_model->setHeaderData(2, Qt::Horizontal, tr("building"));
	p_model->setHeaderData(3, Qt::Horizontal, tr("Name"));
	p_model->setHeaderData(4, Qt::Horizontal, tr("Sex"));
	p_model->setHeaderData(5, Qt::Horizontal, tr("Birthday"));
	p_model->setHeaderData(6, Qt::Horizontal, tr("Address"));
	p_model->setHeaderData(7, Qt::Horizontal, tr("Position"));
	ui.tblResult->resizeColumnsToContents();

	if(p_model->rowCount() == 0)
		if(!refeshFlag) gu_MessageBox(NULL, tr("Search Result"), tr("No Result"), MSGBOX_ICONINFO);
	refeshFlag = false; 
}

void BuildingPropDlg::btnDelPeople_click()
{
	QItemSelectionModel * itemModel;
	QModelIndexList rows;
	if(ui.tabB_Peoples->currentIndex() == 0) {
		itemModel = ui.tblEmplyee->selectionModel();
		rows = itemModel->selectedRows();
		int count = rows.count();
		if(count == 0) return;
		if(gu_MessageBox(NULL, tr("Confirm"), tr("Selected Peoples Removed to Employee !\n\n Do you Remove ?"), MSGBOX_IDYESNO) == MSGBOX_IDYES) {
			for(int ii = 0; ii < count; ii++) {
				if(!RemoveToBuilding(rows[ii].data().toString())) return;
			}
		}
	} else {
		itemModel = ui.tblPopulation->selectionModel();
		rows = itemModel->selectedRows();
		int count = rows.count();
		if(count == 0) return;
		if(gu_MessageBox(NULL, tr("Confirm"), tr("Selected Peoples Removed to House !\n\n Do you Remove ?"), MSGBOX_IDYESNO) == MSGBOX_IDYES) {
			for(int ii = 0; ii < count; ii++) {
				if(!RemoveToHouse(rows[ii].data().toString())) return;
			}
		}
	}
	refresh();
}

void BuildingPropDlg::btnToBuilding_click()
{
	QItemSelectionModel * itemModel = ui.tblResult->selectionModel();
	if(!itemModel) return;
	QModelIndexList rows = itemModel->selectedRows();
	int count = rows.count();
	if(count == 0) {
		gu_MessageBox(NULL, tr("Warning"), tr("please Select peoples !"), MSGBOX_ICONWARNING);
		return;
	}
	for(int ii = 0; ii < count; ii++) {
		if(!peopleToBuilding(rows[ii].data().toString())) return;
	}
	refresh();
	ui.tabB_Peoples->setCurrentIndex(0);
}

void BuildingPropDlg::btnToHouse_click()
{
	QItemSelectionModel * itemModel = ui.tblResult->selectionModel();
	if(!itemModel) return;
	QModelIndexList rows = itemModel->selectedRows();
	int count = rows.count();
	if(count == 0) {
		gu_MessageBox(NULL, tr("Warning"), tr("please Select peoples !"), MSGBOX_ICONWARNING);
		return;
	}
	for(int ii = 0; ii < count; ii++) {
		if(!peopleToHouse(rows[ii].data().toString())) return;
	}
	refresh();
	ui.tabB_Peoples->setCurrentIndex(1);
}

bool BuildingPropDlg::peopleToHouse(QString cardno) //asd add 2013.10.7
{
	QString sql = "SELECT * FROM tb_people WHERE cardno=" + cardno;
	QSqlQuery qry;
	qry.exec(sql);

	if(qry.first()) {
		sql = "UPDATE tb_people SET houseid=" + QString("%1").arg(buildingid) + ", subban=" + QString("%1").arg(ui.cmbB_Ban->currentText().toInt()) + " WHERE cardno=" + cardno;
		if(!qry.exec(sql)) {
			gu_MessageBox(NULL, tr("Failed"), tr("Don't Add People !"), MSGBOX_ICONCRITICAL);
			return false;
		}
	} else {
		sql = "INSERT INTO tb_people (cardno, houseid, subban) VALUES (" + cardno + ", " + QString("%1").arg(buildingid) + ", " + QString("%1").arg(ui.cmbB_Ban->currentText().toInt()) + ")";
		if(!qry.exec(sql)) {
			gu_MessageBox(NULL, tr("Failed"), tr("Don't Add People !"), MSGBOX_ICONCRITICAL);
			return false;
		}
	}
	return true;
}

bool BuildingPropDlg::peopleToBuilding(QString cardno) //asd add 2013.10.7
{
	QString sql = "SELECT * FROM tb_people WHERE cardno=" + cardno;
	QSqlQuery qry;

	qry.exec(sql);
	if(qry.first()) {
		sql = "UPDATE tb_people SET buildingid=" + QString("%1").arg(buildingid) + ", subpost='" + ui.cmbB_Posts->currentText() + "' WHERE cardno=" + cardno;
		if(!qry.exec(sql)) {
			gu_MessageBox(NULL, tr("Failed"), tr("Don't Add People !"), MSGBOX_ICONCRITICAL);
			return false;
		}
	} else {
		sql = "INSERT INTO tb_people (cardno, buildingid, subpost) VALUES (" + cardno + ", " + QString("%1").arg(buildingid) + ", '" + ui.cmbB_Posts->currentText() + "')";
		if(!qry.exec(sql)) {
			gu_MessageBox(NULL, tr("Failed"), tr("Don't Add People !"), MSGBOX_ICONCRITICAL);
			return false;
		}
	}
	return true;
}

bool BuildingPropDlg::RemoveToBuilding(QString cardno) //asd add 2013.10.7
{
	QString sql = "SELECT * FROM tb_people WHERE cardno=" + cardno;
	QSqlQuery qry;

	qry.exec(sql);
	if(qry.first()) {
		sql = "UPDATE tb_people SET buildingid=NULL, subpost=NULL WHERE cardno=" + cardno;
		if(!qry.exec(sql)) {
			gu_MessageBox(NULL, tr("Failed"), tr("Don't Remove People !"), MSGBOX_ICONCRITICAL);
			return false;
		}
	} 
	return true;
}

bool BuildingPropDlg::RemoveToHouse(QString cardno) //asd add 2013.10.7
{
	QString sql = "SELECT * FROM tb_people WHERE cardno=" + cardno;
	QSqlQuery qry;

	qry.exec(sql);
	if(qry.first()) {
		sql = "UPDATE tb_people SET houseid=NULL, subban=NULL WHERE cardno=" + cardno;
		if(!qry.exec(sql)) {
			gu_MessageBox(NULL, tr("Failed"), tr("Don't Remove People !"), MSGBOX_ICONCRITICAL);
			return false;
		}
	} 
	return true;
}

void BuildingPropDlg::refresh()
{
	QString sql = "select per_text.cardno, per_text.name, per_text.sex, per_text.birthday, per_text.party, ";
	sql += "per_text.birthplace, per_text.address, per_text.position, per_issue.class1, per_issue.class2 ";
	sql += "from per_text left join tb_people on per_text.cardno=tb_people.cardno ";
	sql += "left join per_issue on per_text.cardno=per_issue.cardno ";
	sql += "where tb_people.buildingid=" + QString("%1").arg(buildingid) + " ";
	sql += "and tb_people.subpost='" + ui.cmbB_Posts->currentText() + "'";
	sql += " and per_text.enddate IS NULL";

	if(b_model) {
		delete b_model;
		b_model = NULL;
	}

	b_model = new QSqlQueryModel();
	b_model->setQuery(sql);
	ui.tblEmplyee->setModel(b_model);

	ui.tblEmplyee->setColumnHidden(0,true);
	b_model->setHeaderData(1, Qt::Horizontal, tr("Name"));
	b_model->setHeaderData(2, Qt::Horizontal, tr("Sex"));
	b_model->setHeaderData(3, Qt::Horizontal, tr("Birthday"));
	b_model->setHeaderData(4, Qt::Horizontal, tr("Party"));
	b_model->setHeaderData(5, Qt::Horizontal, tr("BirthPlace"));
	b_model->setHeaderData(6, Qt::Horizontal, tr("Address"));
	b_model->setHeaderData(7, Qt::Horizontal, tr("Position"));
	b_model->setHeaderData(8, Qt::Horizontal, tr("class1"));
	b_model->setHeaderData(9, Qt::Horizontal, tr("class2"));

	ui.tblEmplyee->resizeColumnsToContents();

	QString p_Count = " (" + QString("%1").arg(b_model->rowCount()) + ")";
	ui.tabB_Peoples->setTabText(0, tr("Emp") + p_Count);

	sql = "select per_text.cardno, per_text.name, lst_sex.view, per_text.birthday, per_text.party, ";
	sql += "per_text.birthplace, per_text.address, per_text.position ";
	sql += "from per_text left join tb_people on per_text.cardno=tb_people.cardno ";
	sql += "where tb_people.houseid=" + QString("%1").arg(buildingid) + " ";
	sql += "and tb_people.subban=" + ui.cmbB_Ban->currentText();
	sql += " and per_text.enddate IS NULL";

	if(h_model) {
		delete h_model;
		h_model = NULL;
	}

	h_model = new QSqlQueryModel();
	h_model->setQuery(sql);
	ui.tblPopulation->setModel(h_model);

	ui.tblPopulation->setColumnHidden(0,true);
	h_model->setHeaderData(1, Qt::Horizontal, tr("Name"));
	h_model->setHeaderData(2, Qt::Horizontal, tr("Sex"));
	h_model->setHeaderData(3, Qt::Horizontal, tr("Birthday"));
	h_model->setHeaderData(4, Qt::Horizontal, tr("Party"));
	h_model->setHeaderData(5, Qt::Horizontal, tr("BirthPlace"));
	h_model->setHeaderData(6, Qt::Horizontal, tr("Address"));
	h_model->setHeaderData(7, Qt::Horizontal, tr("Position"));

	ui.tblPopulation->resizeColumnsToContents();

	QString h_Count = " (" + QString("%1").arg(h_model->rowCount()) + ")";
	ui.tabB_Peoples->setTabText(1, tr("Pop") + h_Count);

	ui.tblResult->clearSelection();
	refeshFlag = true;
	btnSearch_click();
}

void BuildingPropDlg::showDetail(QModelIndex modelId)
{
	QString cardno = modelId.model()->data(modelId.sibling(modelId.row(), 0)).toString();
	g_app->GetMainWindow()->ShowPeopleDetail(cardno);
}