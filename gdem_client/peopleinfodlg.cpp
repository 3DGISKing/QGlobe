#include "stable.h"
#include "peopleinfodlg.h"
#include "peoplesearchdlg.h"

#include "guiuty.h"
#include "gdemclient.h"

PeopleInfoDlg::PeopleInfoDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	Init();  
	Format(); 

	connect(ui.btnHisFam, SIGNAL(clicked()), this, SLOT(btnHisFam_click()));
	connect(ui.btnIssue, SIGNAL(clicked()), this, SLOT(btnIssue_click()));
	connect(ui.btnGrasp, SIGNAL(clicked()), this, SLOT(btnGrasp_click()));
	connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btnToHouse, SIGNAL(clicked()), this, SLOT(btnToHouse_click()));
	connect(ui.btnToBuilding, SIGNAL(clicked()), this, SLOT(btnToBuilding_click()));

	m_parent = (PeopleSearchDlg *)parent;

	ui.btnToHouse->setVisible(false);
}

PeopleInfoDlg::~PeopleInfoDlg()
{
}

void PeopleInfoDlg::Init() 
{
	history_model = NULL;
	family_model = NULL;
	grasp_model = NULL;

	ui.lblImage->setPixmap(QPixmap());
}

void PeopleInfoDlg::Format() 
{
	history_model = new QStandardItemModel();
	history_model->setColumnCount(5);
	history_model->setHeaderData(0, Qt::Horizontal, tr(""));
	history_model->setHeaderData(1, Qt::Horizontal, tr("No"));
	history_model->setHeaderData(2, Qt::Horizontal, tr("startdate"));
	history_model->setHeaderData(3, Qt::Horizontal, tr("enddate"));
	history_model->setHeaderData(4, Qt::Horizontal, tr("position"));
	ui.tblHistory->setModel(history_model);
	ui.tblHistory->setColumnHidden(0, true);
	ui.tblHistory->setColumnWidth(1, 25);
	ui.tblHistory->setColumnWidth(2, 80);
	ui.tblHistory->setColumnWidth(3, 80);

	family_model = new QStandardItemModel();
	family_model->setColumnCount(8);
	family_model->setHeaderData(0, Qt::Horizontal, tr(""));
	family_model->setHeaderData(1, Qt::Horizontal, tr("No"));
	family_model->setHeaderData(2, Qt::Horizontal, tr("relation"));
	family_model->setHeaderData(3, Qt::Horizontal, tr("name"));
	family_model->setHeaderData(4, Qt::Horizontal, tr("Age"));
	family_model->setHeaderData(5, Qt::Horizontal, tr("party"));
	family_model->setHeaderData(6, Qt::Horizontal, tr("position"));
	family_model->setHeaderData(7, Qt::Horizontal, tr("address"));
	ui.tblFamily->setModel(family_model);
	ui.tblFamily->setColumnHidden(0, true);
	ui.tblFamily->setColumnWidth(1, 25);
	ui.tblFamily->setColumnWidth(2, 60);
	ui.tblFamily->setColumnWidth(3, 60);
	ui.tblFamily->setColumnWidth(4, 40);
	ui.tblFamily->setColumnWidth(5, 60);
	ui.tblFamily->setColumnWidth(6, 180);

	grasp_model = new QStandardItemModel();
	grasp_model->setColumnCount(6);
	grasp_model->setHeaderData(0, Qt::Horizontal, tr(""));
	grasp_model->setHeaderData(1, Qt::Horizontal, tr("No"));
	grasp_model->setHeaderData(2, Qt::Horizontal, tr("recievedate"));
	grasp_model->setHeaderData(3, Qt::Horizontal, tr("offer"));
	grasp_model->setHeaderData(4, Qt::Horizontal, tr("content"));
	grasp_model->setHeaderData(5, Qt::Horizontal, tr("reciever"));
	ui.tblGrasp->setModel(grasp_model);
	ui.tblGrasp->setColumnHidden(0, true);
	ui.tblGrasp->setColumnWidth(1, 25);
	ui.tblGrasp->setColumnWidth(2, 80);
	ui.tblGrasp->setColumnWidth(3, 60);
	ui.tblGrasp->setColumnWidth(4, 375);

	ui.frmHisFam->setVisible(true);
	ui.frmIssue->setVisible(false);
	ui.frmGrasp->setVisible(false);
}

void PeopleInfoDlg::readData(int cardno)
{
	QStandardItem *item;
	QByteArray byteVal;
	QString strVal;
	int row;

	qry.exec("SELECT * FROM per_text WHERE cardno=" + QString("%1").arg(cardno));
	if(!qry.first()) return;

	strVal = qry.value(1).toString();
	ui.edtName->setText(strVal);
	strVal = qry.value(2).toString();
	ui.edtSex->setText(strVal);
	strVal = qry.value(3).toDate().toString("yyyy-M-d");
	ui.edtBirthday->setText(strVal);
	strVal = qry.value(4).toString();
	ui.edtParty->setText(strVal);
	strVal = qry.value(5).toString();
	ui.edtBirthPlace->setText(strVal);
	strVal = qry.value(6).toString();
	ui.edtAddress->setText(strVal);
	strVal = qry.value(7).toString();
	ui.edtPost->setText(strVal);
	strVal = qry.value(8).toString();
	ui.edtAlias->setText(strVal);
	strVal = qry.value(9).toString();
	ui.edtNumber->setText(strVal);
	byteVal = qry.value(10).toByteArray();
	QByteArray imgBytes = byteVal;
	QPixmap img;
	img.loadFromData(imgBytes);
	ui.lblImage->setPixmap(img);
	strVal = qry.value(11).toString();
	ui.edtKnowledge->setText(strVal);
	strVal = qry.value(12).toString();
	ui.edtClassOrg->setText(strVal);
	strVal = qry.value(13).toString();
	ui.edtSocialOrg->setText(strVal);
	strVal = qry.value(14).toDate().toString("yyyy-M-d");
	ui.edtStartDate->setText(strVal);

	qry.exec("SELECT * FROM per_history WHERE cardno=" + QString("%1").arg(cardno));
	for(int ii = 0; ii < history_model->rowCount(); ii++)
		history_model->removeRow(0);
	row = 0;
	while(qry.next()) {
		item = new QStandardItem(QString("%1").arg(row + 1));
		item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
		item->setEditable(false);
		history_model->setItem(row, 1, item);
		strVal = qry.value(2).toDate().toString("yyyy-M-d");
		item = new QStandardItem();
		item->setData(strVal, Qt::EditRole);
		item->setEditable(false);
		history_model->setItem(row, 2, item);
		strVal = qry.value(3).toDate().toString("yyyy-M-d");
		item = new QStandardItem();
		item->setData(strVal, Qt::EditRole);
		item->setEditable(false);
		history_model->setItem(row, 3, item);
		strVal = qry.value(4).toString();
		item = new QStandardItem(strVal);
		item->setEditable(false);
		history_model->setItem(row, 4, item);

		row++;
	}

	qry.exec("SELECT * FROM per_family WHERE cardno=" + QString("%1").arg(cardno));
	for(int ii = 0; ii < family_model->rowCount(); ii++)
		family_model->removeRow(0);
	row = 0;
	while(qry.next()) {
		item = new QStandardItem(QString("%1").arg(row + 1));
		item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
		item->setEditable(false);
		family_model->setItem(row, 1, item);
		strVal = qry.value(2).toString();
		item = new QStandardItem(strVal);
		item->setEditable(false);
		family_model->setItem(row, 2, item);
		strVal = qry.value(3).toString();
		item = new QStandardItem(strVal);
		item->setEditable(false);
		family_model->setItem(row, 3, item);
		strVal = QString("%1").arg(QDate::currentDate().year() - qry.value(4).toInt());
		item = new QStandardItem(strVal);
		item->setEditable(false);
		family_model->setItem(row, 4, item);
		strVal = qry.value(5).toString();
		item = new QStandardItem(strVal);
		item->setEditable(false);
		family_model->setItem(row, 5, item);
		strVal = qry.value(6).toString();
		item = new QStandardItem(strVal);
		item->setEditable(false);
		family_model->setItem(row, 6, item);
		strVal = qry.value(7).toString();
		item = new QStandardItem(strVal);
		item->setEditable(false);
		family_model->setItem(row, 7, item);

		row++;
	}

	qry.exec("SELECT * FROM per_issue WHERE cardno=" + QString("%1").arg(cardno));
	if(qry.first()) {
		strVal = qry.value(1).toString();
		ui.edtIssueClass1->setText(strVal);
		strVal = qry.value(2).toString();
		ui.edtIssueClass2->setText(strVal);
		strVal = qry.value(3).toString();
		ui.txtContent->setPlainText(strVal);
		strVal = qry.value(4).toString();
		ui.edtMisc->setText(strVal);
	}

	qry.exec("SELECT * FROM per_grasp WHERE cardno=" + QString("%1").arg(cardno));
	for(int ii = 0; ii < grasp_model->rowCount(); ii++)
		grasp_model->removeRow(0);
	row = 0;
	while(qry.next()) {
		item = new QStandardItem(QString("%1").arg(row + 1));
		item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
		item->setEditable(false);
		grasp_model->setItem(row, 1, item);
		strVal = qry.value(2).toDate().toString("yyyy-M-d");
		item = new QStandardItem(strVal);
		item->setEditable(false);
		grasp_model->setItem(row, 2, item);
		strVal = qry.value(3).toString();
		item = new QStandardItem(strVal);
		item->setEditable(false);
		grasp_model->setItem(row, 3, item);
		strVal = qry.value(4).toString();
		item = new QStandardItem(strVal);
		item->setEditable(false);
		grasp_model->setItem(row, 4, item);
		strVal = qry.value(5).toString();
		item = new QStandardItem(strVal);
		item->setEditable(false);
		grasp_model->setItem(row, 5, item);

		row++;
	}


	qry.exec("SELECT houseid, buildingid FROM tb_people WHERE cardno=" + QString("%1").arg(cardno));
	if(qry.first()) {
		houseid = qry.value(0).toLongLong();
		buildingid = qry.value(1).toLongLong();
		if(houseid) ui.btnToHouse->setEnabled(true);
		else ui.btnToHouse->setEnabled(false);
		if(buildingid) ui.btnToBuilding->setEnabled(true);
		else ui.btnToBuilding->setEnabled(false);
	}
}
void PeopleInfoDlg::btnHisFam_click()
{
	ui.btnHisFam->setChecked(true);
	ui.btnIssue->setChecked(false);
	ui.btnGrasp->setChecked(false);

	ui.frmHisFam->setVisible(true);
	ui.frmIssue->setVisible(false);
	ui.frmGrasp->setVisible(false);
}

void PeopleInfoDlg::btnIssue_click()
{
	ui.btnHisFam->setChecked(false);
	ui.btnIssue->setChecked(true);
	ui.btnGrasp->setChecked(false);

	ui.frmHisFam->setVisible(false);
	ui.frmIssue->setVisible(true);
	ui.frmGrasp->setVisible(false);
}

void PeopleInfoDlg::btnGrasp_click()
{
	ui.btnHisFam->setChecked(false);
	ui.btnIssue->setChecked(false);
	ui.btnGrasp->setChecked(true);

	ui.frmHisFam->setVisible(false);
	ui.frmIssue->setVisible(false);
	ui.frmGrasp->setVisible(true);
}

#include <QSqlRecord>
#include <QSqlField>

#include "GDM_SceneManager.h"
#include "Geo3dObjectSceneNode.h"

#define GOTO_HEIGHT 300

void PeopleInfoDlg::Goto(qint64 id)
{
	QSqlQuery qry;
	qry.exec("SELECT * FROM tb_building WHERE id="+QString::number(id));

	if(qry.size()==0)
	{
		gu_MessageBox(NULL,tr("Error"),tr("Enter sql"),MSGBOX_ICONINFO);
		return;
	}

	if(!qry.next())
	{
		gu_MessageBox(NULL,tr("Error"),tr("Enter sql"),MSGBOX_ICONINFO);
		return;
	}

	QSqlRecord record=qry.record();

	double minlati=record.field("minlatitude").value().toDouble();
	double maxlati=record.field("maxlatitude").value().toDouble();
	double minlong=record.field("minlongitude").value().toDouble();
	double maxlong=record.field("maxlongitude").value().toDouble();

	double lat=(minlati+maxlati)/2.0;
	double longi=(minlong+maxlong)/2.0;

	longi=longi;
	lat=lat;

	PLACEMARK_DATA data;

	memset(&data,0,sizeof(PLACEMARK_DATA));

	data.view_longitude=longi;
	data.view_latitude=lat;
	data.orgDist=GOTO_HEIGHT;

	g_app->GetRenderProxy()->GetRenderSrv()->FastMoveToPlace(&data);

	g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pGeo3dObjectNode->SetSelectedID(id);		
}


void PeopleInfoDlg::btnToHouse_click() //asd add 2013.10.7
{
	Goto(houseid);
}

void PeopleInfoDlg::btnToBuilding_click() //asd add 2013.10.7
{
	Goto(buildingid);
}
