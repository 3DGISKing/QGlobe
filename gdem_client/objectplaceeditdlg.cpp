#include "objectplaceeditdlg.h"
#include "ui_objectplaceeditdlg.h"
#include "plisttablemodel.h"
#include "gdemclient.h"
#include "renderfrm.h"
#include "msgdata.h"
#include "guiuty.h"
#include "mainwindow.h"
#include "config.h"
#include "asdtextdelegate.h"
// for gis node
#include "GIS_Path.h"
#include "GDM_CommandMgr.h"
#include "GDM_DrawObjectCmd.h"

#include "../gds_common/GDSConfigMgr.h"
#include "filedialog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include "GDM_DataMgr.h"
#include "GDM_CacheMgr.h"
#include "../gdem_scene/GIS_TextureIDCache.h"
#include "GDM_GISDataCache.h"

typedef enum 
{
	STATE_START,
	STATE_SELECT,
	STATE_NEW,
	STATE_MODIFY
}EDIT_STATE;

#define VIDEO_PATH "/video"
#define HTML_PATH  "/html"

ObjectPlaceEditDlg::ObjectPlaceEditDlg(QWidget *parent) :
	CGDM_CommandDlg(parent),
	m_ui(new Ui::ObjectPlaceEditDlg)
{
    m_ui->setupUi(this);

	m_gisInfo = g_app->GetConfig()->m_gisInfoChecked;
	bool tmp = false;
	g_app->GetRenderProxy()->SendMessage(PM_SHOW_LENGTH_AREA, (PrxParam)&tmp);

	m_pathCommand = NULL;
	m_EditMode = false;
	m_PropertyMode = false;
	m_editingPoints = NULL;
	m_EditStart = false;

	InitDialog();

	connect(m_ui->btnModify, SIGNAL(clicked()), this, SLOT(btnModify_click()));
	connect(m_ui->btnNewAdd, SIGNAL(clicked()), this, SLOT(btnNewAdd_click()));
	connect(m_ui->btnDelete, SIGNAL(clicked()), this, SLOT(btnDelete_click()));

	connect(m_ui->cmbCategory, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbCategory_change(int)));
	connect(m_ui->cmbObjectPlace, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbObjectPlace_change(int)));
	connect(m_ui->btnHtmlPath, SIGNAL(clicked()), this, SLOT(edtHtmlPath_change()));
	connect(m_ui->btnVideoPath, SIGNAL(clicked()), this, SLOT(edtVideoPath_change()));

	connect(m_ui->addButton, SIGNAL(clicked()), this, SLOT(AddPointData()));
	connect(m_ui->editButton, SIGNAL(clicked()), this, SLOT(EditPointData()));
	connect(m_ui->deleteButton, SIGNAL(clicked()), this, SLOT(DelPointData()));
	connect(m_pointListModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(ChangedPointData()));

	connect(m_ui->btnSave, SIGNAL(clicked()), this, SLOT(btnSave_click()));
	connect(m_ui->btnCancel, SIGNAL(clicked()), this, SLOT(btnCancel_click()));
	connect(m_ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExit_click()));

	connect(m_ui->btnCenterPoint, SIGNAL(clicked(bool)), this, SLOT(btnCenterPoint_click(bool)));

	connect(m_ui->tabs_ShapeProperty, SIGNAL(currentChanged(int)), this, SLOT(tabIndex_change(int)));
	connect(m_ui->edtLongitude, SIGNAL(editingFinished()), this, SLOT(SetCenterMark()));
	connect(m_ui->edtLatitude, SIGNAL(editingFinished()), this, SLOT(SetCenterMark()));
}

ObjectPlaceEditDlg::~ObjectPlaceEditDlg()
{
	m_pathCommand->OnCancel();
	m_pathCommand = NULL;
	g_app->GetMainWindow()->UpdatePlaceDlg(true);
	delete m_ui;

	g_app->GetConfig()->m_gisInfoChecked = m_gisInfo;
}

void ObjectPlaceEditDlg::InitDialog()
{
	QPoint mainPos = g_app->GetMainWindow()->pos();
	move(mainPos.x() + 50, mainPos.y() + 100);

	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
				   | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setFixedSize(size());

	//m_ui->okButton->setFocus(Qt::OtherFocusReason);

	QList <PointData> data;
	m_pointListModel = new PListTableModel(data);
	m_ui->pListTableView->setModel(m_pointListModel);
	m_ui->pListTableView->setColumnHidden(2, true);
	m_ui->pListTableView->horizontalHeader()->setDefaultSectionSize(150);

	asdTextDelegate *asdTextEdit = new asdTextDelegate();
	m_ui->pListTableView->setItemDelegateForColumn(0, asdTextEdit);
	m_ui->pListTableView->setItemDelegateForColumn(1, asdTextEdit);

	m_ui->tabs_ShapeProperty->setTabEnabled(1, false);
	m_ui->tabs_ShapeProperty->setTabEnabled(2, false);

	m_ui->cmbCategory->blockSignals(true);
	m_ui->cmbCategory->addItem(tr("Name"));
	m_ui->cmbCategory->addItem(tr("Object"));
	m_ui->cmbCategory->setCurrentIndex(1);
	m_ui->cmbCategory->blockSignals(false);
	cmbCategory_change(1);
}

void ObjectPlaceEditDlg::showEvent(QShowEvent *event)
{
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	m_pathCommand = (CGDM_DrawObjectCmd*)pCmdMgr->CreateCmd(eGDM_CMD_DRAW_OBJECT, this);

	setState(STATE_START);
}

void ObjectPlaceEditDlg::btnNewAdd_click()
{
	m_objectID = -1;
	CGIS_Path *pathNode = new CGIS_Path();
	m_pathCommand->Init(pathNode, true);
	m_pathCommand->SetShapeType(m_shapeType);
	copyDataFromNode();
	m_ui->edtName_ko->setText("");
	m_ui->edtName->setText("");
	m_ui->edtLatitude->setText("");
	m_ui->edtLongitude->setText("");
	m_ui->edtHtmlPath->setText("");
	m_ui->edtVideoPath->setText("");
	setState(STATE_NEW);
	m_ui->btnCenterPoint->setEnabled(m_shapeType > OBJECT_POINT);
}

void ObjectPlaceEditDlg::btnModify_click()
{
	CGIS_Path *pathNode = new CGIS_Path();
	m_pathCommand->Init(pathNode, true);
	copyDataToNode();
	m_pathCommand->SetShapeType(m_shapeType);
	m_pathCommand->SetTrackIndex(pathNode->Get_TrackCount() - 1);
	if(m_shapeType == OBJECT_POLYGON) {
		CGDM_Location3D first_pt;
		pathNode->Get_TrackPt(0, &first_pt);
		pathNode->Set_TrackPt(0, &first_pt);
		m_pathCommand->m_drawEnd = true;
	}
	pathNode->Refresh();
	setState(STATE_MODIFY);
	m_ui->btnCenterPoint->setEnabled(m_shapeType > OBJECT_POINT);
}

void ObjectPlaceEditDlg::btnDelete_click()
{
	if(m_objectID >= 0) {
		QSqlQuery qry;
		QString tableName = m_tableNameList.at(m_ui->cmbObjectPlace->currentIndex());

		if(qry.exec("DELETE FROM " + tableName + " WHERE id=" + QString("%1").arg(m_objectID))) { //add, if mod id > -1
			gu_MessageBox(this, tr("ObjectPlacceEdit"), tr("Successfully Delete Data !"), MSGBOX_ICONINFO);
			editClear();
			setState(STATE_START);
			renderFormRefresh();
		} else {
			gu_MessageBox(this, tr("ObjectPlacceEdit"), tr("Failed Delete Data !"), MSGBOX_ICONCRITICAL);
		}
	}
}

void ObjectPlaceEditDlg::editClear()
{
	if(m_pathCommand)
		m_pathCommand->OnCancel();
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	m_pathCommand = (CGDM_DrawObjectCmd*)pCmdMgr->CreateCmd(eGDM_CMD_DRAW_OBJECT, this);
	copyDataFromNode();
	m_ui->edtName_ko->setText("");
	m_ui->edtName->setText("");
	m_ui->edtLatitude->setText("");
	m_ui->edtLongitude->setText("");
	m_ui->edtHtmlPath->setText("");
	m_ui->edtVideoPath->setText("");

	int rowCount = m_pointListModel->rowCount(QModelIndex());
	if(rowCount > 0)
		m_pointListModel->removeRows(0,  rowCount, QModelIndex());

	m_ui->btnCenterPoint->setChecked(false);
}

void ObjectPlaceEditDlg::setState(int state)
{
	switch(state) {
	case STATE_START:
		m_EditMode = false;
		m_ui->cmbCategory->setEnabled(false);
		m_ui->cmbObjectPlace->setEnabled(false);
		m_ui->btnSave->setEnabled(false);
		m_ui->btnCancel->setEnabled(false);
		m_ui->btnNewAdd->setEnabled(true);
		m_ui->btnModify->setEnabled(false);
		m_ui->btnDelete->setEnabled(false);
		m_ui->btnCenterPoint->setEnabled(false);
		m_ui->btnHtmlPath->setEnabled(false);
		m_ui->btnVideoPath->setEnabled(false);
		m_ui->addButton->setEnabled(false);
		m_ui->editButton->setEnabled(false);
		m_ui->deleteButton->setEnabled(false);
		m_ui->lblName->setText("");
		m_ui->lblLat->setText("");
		m_ui->lblLong->setText("");
		break;
	case STATE_SELECT:
		m_EditMode = false;
		m_ui->cmbCategory->setEnabled(false);
		m_ui->cmbObjectPlace->setEnabled(false);
		m_ui->btnSave->setEnabled(false);
		m_ui->btnCancel->setEnabled(false);
		m_ui->btnNewAdd->setEnabled(true);
		m_ui->btnModify->setEnabled(true);
		m_ui->btnDelete->setEnabled(true);
		m_ui->btnCenterPoint->setEnabled(false);
		m_ui->btnHtmlPath->setEnabled(false);
		m_ui->btnVideoPath->setEnabled(false);
		m_ui->addButton->setEnabled(false);
		m_ui->editButton->setEnabled(false);
		m_ui->deleteButton->setEnabled(false);
		break;
	case STATE_NEW:
		m_EditMode = true;
		m_ui->cmbCategory->setEnabled(true);
		m_ui->cmbObjectPlace->setEnabled(true);
		m_ui->btnSave->setEnabled(true);
		m_ui->btnCancel->setEnabled(true);
		m_ui->btnNewAdd->setEnabled(false);
		m_ui->btnModify->setEnabled(false);
		m_ui->btnDelete->setEnabled(false);
		m_ui->btnHtmlPath->setEnabled(true);
		m_ui->btnVideoPath->setEnabled(true);
		m_ui->addButton->setEnabled(true);
		m_ui->editButton->setEnabled(true);
		m_ui->deleteButton->setEnabled(true);
		m_ui->lblName->setText("");
		m_ui->lblLat->setText("");
		m_ui->lblLong->setText("");
		break;
	case STATE_MODIFY:
		m_EditMode = true;
		m_ui->cmbCategory->setEnabled(false);
		m_ui->cmbObjectPlace->setEnabled(false);
		m_ui->btnSave->setEnabled(true);
		m_ui->btnCancel->setEnabled(true);
		m_ui->btnNewAdd->setEnabled(false);
		m_ui->btnModify->setEnabled(false);
		m_ui->btnDelete->setEnabled(false);
		m_ui->btnHtmlPath->setEnabled(true);
		m_ui->btnVideoPath->setEnabled(true);
		m_ui->addButton->setEnabled(true);
		m_ui->editButton->setEnabled(true);
		m_ui->deleteButton->setEnabled(true);
		break;
	}
	if(m_EditMode) {
		m_ui->edtName_ko->setReadOnly(false);
		m_ui->edtName->setReadOnly(false);
		m_ui->edtLatitude->setReadOnly(false);
		m_ui->edtLongitude->setReadOnly(false);
		m_ui->pListTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
	} else {
		m_ui->edtName_ko->setReadOnly(true);
		m_ui->edtName->setReadOnly(true);
		m_ui->edtLatitude->setReadOnly(true);
		m_ui->edtLongitude->setReadOnly(true);
		m_ui->pListTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	}
}

void ObjectPlaceEditDlg::setModifyData(int tb_type, int id)
{
	QSqlQuery qry;
	QString strSql;
	QString strVal;
	double dblVal;
	QByteArray byteVal;

	if(tb_type == 0) {
		gu_MessageBox(this, tr("Wrong"), tr("Select Object Correctly !"), MSGBOX_ICONWARNING);
		return;
	}

	if(tb_type >= OBJECT_TYPE_NO)
		m_ui->cmbCategory->setCurrentIndex(1);
	else
		m_ui->cmbCategory->setCurrentIndex(0);

	int lstId = m_typeList.indexOf(tb_type);
	m_ui->cmbObjectPlace->setCurrentIndex(lstId);

	QString tableName = m_tableNameList.at(lstId);
	m_objectID = id;

	strSql = "SELECT * FROM " + tableName + " WHERE id=" + QString("%1").arg(m_objectID);
	qry.exec(strSql);
	if(qry.first()) {
		strVal = qry.record().value("name").toString();
		m_ui->edtName->setText(strVal);

		strVal = qry.record().value("name_ko").toString();
		m_ui->edtName_ko->setText(strVal);

		dblVal = qry.record().value("latitude").toDouble();
		m_ui->edtLatitude->setText(ValueToString(dblVal));

		dblVal = qry.record().value("longitude").toDouble();
		m_ui->edtLongitude->setText(ValueToString(dblVal));

		if(m_shapeType > PLACE_POINT) {
			strVal = qry.record().value("html_path").toString();
			m_ui->edtHtmlPath->setText(strVal);

			strVal = qry.record().value("video_path").toString();
			m_ui->edtVideoPath->setText(strVal);
		}

		if(m_shapeType > OBJECT_POINT) {
			byteVal = qry.record().value("points").toByteArray();
			pointsRead(byteVal);
		} else {
			byteVal = pointsWrite();
			pointsRead(byteVal);
		}

		if(tb_type == E_GDM_SUBDATA_TYPE_NAME_POLUTANT) {
			strVal = qry.record().value("accidentname").toString();
			m_ui->lblName->setText(strVal);

			dblVal = qry.record().value("accidentlat").toDouble();
			m_ui->lblLat->setText(ValueToString(dblVal));

			dblVal = qry.record().value("accidentlong").toDouble();
			m_ui->lblLong->setText(ValueToString(dblVal));
		}
	}

	setState(STATE_SELECT);
}

void ObjectPlaceEditDlg::setPropertyData(int tb_type, int id)
{
	if(tb_type == E_GDM_SUBDATA_TYPE_NAME_ACCIDENT) {
		QSqlQuery qry;
		QString strSql;
		QString strVal;
		double dblVal;

		int lstId = m_typeList.indexOf(tb_type);
		QString tableName = m_tableNameList.at(lstId);
		strSql = "SELECT name_ko, latitude, longitude FROM " + tableName + " WHERE id=" + QString("%1").arg(id);
		qry.exec(strSql);
		if(qry.first()) {
			strVal = qry.record().value("name_ko").toString();
			m_ui->lblName->setText(strVal);

			dblVal = qry.record().value("latitude").toDouble();
			m_ui->lblLat->setText(ValueToString(dblVal));

			dblVal = qry.record().value("longitude").toDouble();
			m_ui->lblLong->setText(ValueToString(dblVal));
		}
	}
}

void ObjectPlaceEditDlg::cmbCategory_change(int idx)
{
	int objectTypeNo = OBJECT_TYPE_NO;

	QSqlQuery qry;
	QString strSql;
	int intVal;
	if(idx == 0)
		strSql = "SELECT name_ko, shape_type, table_name, type FROM tb_type WHERE type<=" + QString("%1").arg(objectTypeNo) + " ORDER BY id";
	else
		strSql = "SELECT name_ko, shape_type, table_name, type FROM tb_type WHERE type>" + QString("%1").arg(objectTypeNo) + " ORDER BY id";

	qry.exec(strSql);
	m_ui->cmbObjectPlace->blockSignals(true);
	m_ui->cmbObjectPlace->clear();
	m_tableNameList.clear();
	m_typeList.clear();
	while (qry.next()) {
		m_ui->cmbObjectPlace->addItem(qry.value(0).toString(), qry.value(1));
		m_tableNameList << qry.value(2).toString();
		m_typeList << qry.value(3).toInt();
	}
	m_ui->cmbObjectPlace->blockSignals(false);
	cmbObjectPlace_change(0);
}

void ObjectPlaceEditDlg::cmbObjectPlace_change(int idx)
{
	m_shapeType = m_ui->cmbObjectPlace->itemData(idx).toInt();

	if(m_EditMode) {
		m_pathCommand->OnCancel();
		CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
		m_pathCommand = (CGDM_DrawObjectCmd*)pCmdMgr->CreateCmd(eGDM_CMD_DRAW_OBJECT, this);
		btnNewAdd_click();
		m_pathCommand->SetShapeType(m_shapeType);
	}

	if(m_shapeType > OBJECT_POINT) {
		m_ui->tabs_ShapeProperty->setTabEnabled(1, true);
		m_ui->tabs_ShapeProperty->setTabEnabled(2, false);

		//----------------------------------------------------
		int tbType = m_typeList.at(idx);
		if(tbType == E_GDM_SUBDATA_TYPE_NAME_POLUTANT)
			m_ui->tabs_ShapeProperty->setTabEnabled(2, true);
		//----------------------------------------------------

		m_ui->btnCenterPoint->setEnabled(true);
	} else {
		m_ui->tabs_ShapeProperty->setTabEnabled(1, false);
		m_ui->tabs_ShapeProperty->setTabEnabled(2, false);
		m_ui->btnCenterPoint->setEnabled(false);
	}
}

void ObjectPlaceEditDlg::UpdateDialog()
{
	copyDataFromNode();
}

void ObjectPlaceEditDlg::copyDataToNode(bool bRefresh)
{
	CGIS_Path *pathNode = (CGIS_Path *)m_pathCommand->m_pNode;
	if(pathNode == NULL)
		return;

	// point list data of polygon
	m_pointListModel->getData(pathNode->m_PtArr);
}

void ObjectPlaceEditDlg::copyDataFromNode()
{
	CGIS_Path *pathNode = (CGIS_Path *)m_pathCommand->m_pNode;
	if(pathNode == NULL)
		return;

	m_pointListModel->setData(pathNode->m_PtArr);

	if(m_shapeType < OBJECT_LINE || m_ui->btnCenterPoint->isChecked()) {
		CGDM_Location3D pt;
		QString strVal;
		pathNode->Get_TrackPt(0, &pt);
		strVal = ValueToString(radian2degree(pt.m_tX));
		m_ui->edtLongitude->setText(strVal);
		strVal = ValueToString(radian2degree(pt.m_tY));
		m_ui->edtLatitude->setText(strVal);
	}
}

void ObjectPlaceEditDlg::btnCenterPoint_click(bool isCenterPointEdit)
{
	if(isCenterPointEdit) {
		double plat = StringToValue(m_ui->edtLatitude->text());
		double plong = StringToValue(m_ui->edtLongitude->text());
		if(plat == 0 || plong == 0) {
			SetPlaceLatLong();
			plat = StringToValue(m_ui->edtLatitude->text());
			plong = StringToValue(m_ui->edtLongitude->text());
		}

		if(m_pathCommand->GetNode()->Get_TrackCount() > 0)
			m_editingPoints = pointsWrite();

		m_pathCommand->OnEnd();
		m_pathCommand = NULL;
		CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
		m_pathCommand = (CGDM_DrawObjectCmd*)pCmdMgr->CreateCmd(eGDM_CMD_DRAW_OBJECT, this);
		CGIS_Path *pathNode = new CGIS_Path();
		m_pathCommand->Init(pathNode, true);
		m_pathCommand->SetShapeType(0);

		int rowCount = m_pointListModel->rowCount(QModelIndex());
		if(rowCount > 0)
			m_pointListModel->removeRows(0,  rowCount, QModelIndex());

		m_pointListModel->insertRows(0, 1, QModelIndex());
		QModelIndex index = m_pointListModel->index(0, 0, QModelIndex());
		m_pointListModel->setData(index, plat, Qt::EditRole);
		index = m_pointListModel->index(0, 1, QModelIndex());
		m_pointListModel->setData(index, plong, Qt::EditRole);
		copyDataToNode();

		pathNode->Refresh();
	} else {
		if(m_pathCommand)
			m_pathCommand->Clear();
		m_pathCommand = NULL;
		CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
		m_pathCommand = (CGDM_DrawObjectCmd*)pCmdMgr->CreateCmd(eGDM_CMD_DRAW_OBJECT, this);
		CGIS_Path *pathNode = new CGIS_Path();
		m_pathCommand->Init(pathNode, true);
		m_pathCommand->SetShapeType(m_shapeType);

		pointsRead(m_editingPoints);
		copyDataToNode();

		pathNode->Refresh();
		m_pathCommand->SetTrackIndex(pathNode->Get_TrackCount() - 1);
		m_EditStart = false;
	}
}

void ObjectPlaceEditDlg::SetPlaceLatLong()
{
	CGIS_Path *pathNode = (CGIS_Path *)m_pathCommand->m_pNode;
	CGDM_Location3D pt;
	QList<double> mbrList;
	QString strVal;
	int idx;
	switch(m_shapeType) {
	case OBJECT_LINE:
		idx = (int)(pathNode->Get_TrackCount() / 2);
		pathNode->Get_TrackPt(idx, &pt);
		strVal = ValueToString(radian2degree(pt.m_tX));
		m_ui->edtLongitude->setText(strVal);
		strVal = ValueToString(radian2degree(pt.m_tY));
		m_ui->edtLatitude->setText(strVal);
		break;
	case OBJECT_POLYGON:
		mbrList = getMBR();
		strVal = ValueToString(radian2degree((mbrList[2] - mbrList[0]) / 2 + mbrList[0]));
		m_ui->edtLongitude->setText(strVal);
		strVal = ValueToString(radian2degree((mbrList[3] - mbrList[1]) / 2 + mbrList[1]));
		m_ui->edtLatitude->setText(strVal);
		break;
	default:
		pathNode->Get_TrackPt(0, &pt);
		strVal = ValueToString(radian2degree(pt.m_tX));
		m_ui->edtLongitude->setText(strVal);
		strVal = ValueToString(radian2degree(pt.m_tY));
		m_ui->edtLatitude->setText(strVal);
		break;
	}
}

void ObjectPlaceEditDlg::GetNewPointData(PointData &pd, int curRow)
{
	QList<PointData> list = m_pointListModel->getList();

	CGDM_Location3D prePos, nextPos, newPos;

	if (curRow != 0 && list.size() >= curRow+1) { // insert
		prePos.m_tX = degree2radian((list.at(curRow-1).m_longitude).toDouble());
		prePos.m_tY = degree2radian((list.at(curRow-1).m_latitude).toDouble());
		nextPos.m_tX = degree2radian((list.at(curRow).m_longitude).toDouble());
		nextPos.m_tY = degree2radian((list.at(curRow).m_latitude).toDouble());
		GetValidPos(&prePos, &nextPos, &newPos, false);
	} else if (list.size() >= 2) {
		prePos.m_tX = degree2radian((list.at(0).m_longitude).toDouble());
		prePos.m_tY = degree2radian((list.at(0).m_latitude).toDouble());
		if (curRow != 0) {
			nextPos.m_tX = degree2radian((list.at(curRow-1).m_longitude).toDouble());
			nextPos.m_tY = degree2radian((list.at(curRow-1).m_latitude).toDouble());
		} else {
			nextPos.m_tX = degree2radian((list.at(list.size()-1).m_longitude).toDouble());
			nextPos.m_tY = degree2radian((list.at(list.size()-1).m_latitude).toDouble());
		}
		GetValidPos(&prePos, &nextPos, &newPos, false);
	} else if (list.size() == 1) {
		prePos.m_tX = degree2radian((list.at(0).m_longitude).toDouble());
		prePos.m_tY = degree2radian((list.at(0).m_latitude).toDouble());
		nextPos.m_tX = degree2radian((list.at(0).m_longitude).toDouble());
		nextPos.m_tY = degree2radian((list.at(0).m_latitude).toDouble());
		GetValidPos(&prePos, &nextPos, &newPos, false);
	} else {
		GetValidPos(NULL, NULL, &newPos, false);
	}

	pd.m_latitude = QString("%1").arg(radian2degree(newPos.m_tY));
	pd.m_longitude = QString("%1").arg(radian2degree(newPos.m_tX));
}

void ObjectPlaceEditDlg::AddPointData()
{
	QList<PointData> list = m_pointListModel->getList();
	QModelIndex curIndex = m_ui->pListTableView->currentIndex();
	int curRow = curIndex.row() + 1;

	if (curIndex.row() < 0) // when do not select treeview
		curRow = list.size();
	
	PointData pd;
	GetNewPointData(pd, curRow);

	if (!list.contains(pd))
	{
		m_pointListModel->insertRows(curRow, 1, QModelIndex());
		QModelIndex index = m_pointListModel->index(curRow, 0, QModelIndex());
		m_pointListModel->setData(index, pd.m_latitude, Qt::EditRole);
        index = m_pointListModel->index(curRow, 1, QModelIndex());
		m_pointListModel->setData(index, pd.m_longitude, Qt::EditRole);
		index = m_pointListModel->index(curRow, 2, QModelIndex());
		m_pointListModel->setData(index, pd.m_height, Qt::EditRole);
    } 
	copyDataToNode();
}

void ObjectPlaceEditDlg::EditPointData()
{
	QModelIndex curIndex = m_ui->pListTableView->currentIndex();
	m_ui->pListTableView->edit(curIndex);
}

void ObjectPlaceEditDlg::ChangedPointData()
{
	copyDataToNode();
}

void ObjectPlaceEditDlg::DelPointData()
{
	QModelIndex curIndex = m_ui->pListTableView->currentIndex();
	int curRow = curIndex.row();
	if (curIndex.row() < 0)
		curRow = m_pointListModel->rowCount(QModelIndex());
	m_pointListModel->removeRows(curRow, 1, QModelIndex());
	copyDataToNode();
}

void ObjectPlaceEditDlg::btnSave_click()
{
	if(m_ui->tabs_ShapeProperty->currentIndex() != 0)
		m_ui->tabs_ShapeProperty->setCurrentIndex(0);
	if(m_ui->btnCenterPoint->isChecked())
		btnCenterPoint_click(false);
	if(!checkInputData()) return;
	if(saveObjectPlace()) { 
		gu_MessageBox(this, tr("ObjectPlacceEdit"), tr("Successfully Save Data !"), MSGBOX_ICONINFO);
		editClear();
		setState(STATE_START);
		renderFormRefresh();
	} else {
		gu_MessageBox(this, tr("ObjectPlacceEdit"), tr("Failed Save Data !"), MSGBOX_ICONCRITICAL);
		return;
	}
}

void ObjectPlaceEditDlg::renderFormRefresh()
{
	g_pGDMDataMgr->m_pCacheMgr->NameCacheClear();
	gdm_GetGISTextureIDCache()->Clear();
	g_pGDMDataMgr->m_pGISDataCache->Clear();
	g_pGDMDataMgr->OnChangeServer();
}

void ObjectPlaceEditDlg::btnCancel_click()
{
	editClear();
	setState(STATE_START);
}

void ObjectPlaceEditDlg::btnExit_click()
{
	QDialog::reject();
}

QList<double> ObjectPlaceEditDlg::getMBR()
{
	QList<double> retVal;
	CGIS_Path *pathNode = (CGIS_Path *)m_pathCommand->m_pNode;
	CGDM_Location3D pt;
	double tmp;
	retVal << 4 << 4 << -4 << -4;
	int count = pathNode->Get_TrackCount();
	if(count < 1) return retVal;

	for(int ii = 0; ii < count; ii++) {
		pathNode->Get_TrackPt(ii, &pt);

		tmp = pt.m_tX;
		if(retVal[0] > tmp) retVal[0] = tmp;
		if(retVal[2] < tmp) retVal[2] = tmp;

		tmp = pt.m_tY;
		if(retVal[1] > tmp) retVal[1] = tmp;
		if(retVal[3] < tmp) retVal[3] = tmp;
	}

	return retVal;
}

QByteArray ObjectPlaceEditDlg::pointsWrite()
{
	QByteArray *retArr = new QByteArray();
	QDataStream out(retArr, QIODevice::WriteOnly);

	CGIS_Path *pathNode = (CGIS_Path *)m_pathCommand->m_pNode;
	CGDM_Location3D first_pt;
	CGDM_Location3D pt;
	switch(m_shapeType) {
	case OBJECT_LINE:
		for(int ii = 0; ii < pathNode->Get_TrackCount(); ii++) {
			pathNode->Get_TrackPt(ii, &pt);
			out << pt.m_tX;
			out << pt.m_tY;
		}
		break;
	case OBJECT_POLYGON:
		pathNode->Get_TrackPt(0, &first_pt);
		out << first_pt.m_tX;
		out << first_pt.m_tY;
		for(int ii = 1; ii < pathNode->Get_TrackCount(); ii++) {
			pathNode->Get_TrackPt(ii, &pt);
			if(pt == first_pt) 
				continue;
			out << pt.m_tX;
			out << pt.m_tY;
		}
		break;
	default:
		double doubleVal;
		doubleVal = degree2radian(StringToValue(m_ui->edtLongitude->text()));
		out << doubleVal;
		doubleVal = degree2radian(StringToValue(m_ui->edtLatitude->text()));
		out << doubleVal;
		break;
	}
	return *retArr;
}

void ObjectPlaceEditDlg::pointsRead(QByteArray &arr)
{
	double pt_X, pt_Y;

	int rowCount = m_pointListModel->rowCount(QModelIndex());
	if(rowCount > 0)
		m_pointListModel->removeRows(0,  rowCount, QModelIndex());

	int row = 0;
	QDataStream in(&arr, QIODevice::ReadOnly);
	while(!in.atEnd()) {
		in >> pt_X;
		in >> pt_Y;

		m_pointListModel->insertRows(row, 1, QModelIndex());

		QModelIndex index = m_pointListModel->index(row, 0, QModelIndex());
		m_pointListModel->setData(index, radian2degree(pt_Y), Qt::EditRole);
		index = m_pointListModel->index(row, 1, QModelIndex());
		m_pointListModel->setData(index, radian2degree(pt_X), Qt::EditRole);

		row++;
	}	
}

bool ObjectPlaceEditDlg::checkInputData()
{
	if(m_ui->edtName_ko->text().trimmed().isEmpty()) {
		gu_MessageBox(this, tr("ObjectPlacceEdit"), tr("Input Name_Ko Please !"), MSGBOX_ICONCRITICAL);
		return false;
	}

	double plat = StringToValue(m_ui->edtLatitude->text());
	double plong = StringToValue(m_ui->edtLongitude->text());

	int points = m_pointListModel->rowCount(QModelIndex());
	switch(m_shapeType) {
	case OBJECT_LINE:
		if(points < 2) {
			gu_MessageBox(this, tr("ObjectPlacceEdit"), tr("Draw Line Correctly !"), MSGBOX_ICONCRITICAL);
			return false;
		}
		if(plat == 0 || plong == 0 || plat < -90 || plat > 90 || plong < -180 || plong > 180)
			SetPlaceLatLong();
		break;
	case OBJECT_POLYGON:
		if(points < 3) {
			gu_MessageBox(this, tr("ObjectPlacceEdit"), tr("Draw Polygon Correctly !"), MSGBOX_ICONCRITICAL);
			return false;
		}
		if(plat == 0 || plong == 0 || plat < -90 || plat > 90 || plong < -180 || plong > 180)
			SetPlaceLatLong();
		break;
	default:
		if(plat == 0 || plong == 0 || plat < -90 || plat > 90 || plong < -180 || plong > 180) {
			gu_MessageBox(this, tr("ObjectPlacceEdit"), tr("Appoint Position Correctly !"), MSGBOX_ICONCRITICAL);
			return false;
		}
		break;
	}
	return true;
}

bool ObjectPlaceEditDlg::saveObjectPlace()
{
	QSqlQuery qry;
	QString strQry;
	QString strVal;
	int intVal;
	double doubleVal;
	QByteArray byteVal;
	QVariant bindData;

	int curId = m_ui->cmbObjectPlace->currentIndex();
	QString tableName = m_tableNameList.at(curId);
	int tbType = m_typeList.at(curId);

	if(m_objectID >= 0) {
		if(m_shapeType > OBJECT_POINT) {
			strQry = "UPDATE " + tableName + " SET type=?, name=?, name_ko=?, latitude=?, longitude=?, html_path=?, video_path=?, ";
			strQry += "points=?, minlatitude=?, minlongitude=?, maxlatitude=?, maxlongitude=? ";
			strQry += "WHERE id=" + QString("%1").arg(m_objectID);
		} else if(m_shapeType > PLACE_POINT) {
			strQry = "UPDATE " + tableName + " SET type=?, name=?, name_ko=?, latitude=?, longitude=?, html_path=?, video_path=? ";
			strQry += "WHERE id=" + QString("%1").arg(m_objectID);
		} else {
			strQry = "UPDATE " + tableName + " SET type=?, name=?, name_ko=?, latitude=?, longitude=? ";
			strQry += "WHERE id=" + QString("%1").arg(m_objectID);
		}
		qry.prepare(strQry);

		bindData.setValue(tbType);
		qry.bindValue(0, bindData);

		strVal = m_ui->edtName->text().trimmed();
		bindData.setValue(strVal);
		qry.bindValue(1, bindData);

		strVal = m_ui->edtName_ko->text().trimmed();
		bindData.setValue(strVal);
		qry.bindValue(2, bindData);

		doubleVal = StringToValue(m_ui->edtLatitude->text());
		bindData.setValue(doubleVal);
		qry.bindValue(3, bindData);

		doubleVal = StringToValue(m_ui->edtLongitude->text());
		bindData.setValue(doubleVal);
		qry.bindValue(4, bindData);

		if(m_shapeType > PLACE_POINT) {
			strVal = m_ui->edtHtmlPath->text().trimmed();
			bindData.setValue(strVal);
			qry.bindValue(5, bindData);

			strVal = m_ui->edtVideoPath->text().trimmed();
			bindData.setValue(strVal);
			qry.bindValue(6, bindData);
		}

		if(m_shapeType > OBJECT_POINT) {
			byteVal = pointsWrite();
			bindData.setValue(byteVal);
			qry.bindValue(7, bindData, QSql::In|QSql::Binary);

			QList<double> mbrList = getMBR();

			bindData.setValue(radian2degree(mbrList[1]));
			qry.bindValue(8, bindData);

			bindData.setValue(radian2degree(mbrList[0]));
			qry.bindValue(9, bindData);

			bindData.setValue(radian2degree(mbrList[3]));
			qry.bindValue(10, bindData);

			bindData.setValue(radian2degree(mbrList[2]));
			qry.bindValue(11, bindData);
		}

		if(!qry.exec()) return false;
	} else {
		if(m_shapeType > OBJECT_POINT) {
			strQry = "INSERT INTO " + tableName + " (type, name, name_ko, latitude, longitude, html_path, video_path, ";
			strQry += "points, minlatitude, minlongitude, maxlatitude, maxlongitude) ";
			strQry += "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
		} else if(m_shapeType > PLACE_POINT) {
			strQry = "INSERT INTO " + tableName + " (type, name, name_ko, latitude, longitude, html_path, video_path) ";
			strQry += "VALUES (?, ?, ?, ?, ?, ?, ?)";
		} else {
			strQry = "INSERT INTO " + tableName + " (type, name, name_ko, latitude, longitude) ";
			strQry += "VALUES (?, ?, ?, ?, ?)";
		}
		qry.prepare(strQry);

		bindData.setValue(tbType);
		qry.bindValue(0, bindData);

		strVal = m_ui->edtName->text().trimmed();
		bindData.setValue(strVal);
		qry.bindValue(1, bindData);

		strVal = m_ui->edtName_ko->text().trimmed();
		bindData.setValue(strVal);
		qry.bindValue(2, bindData);

		doubleVal = StringToValue(m_ui->edtLatitude->text());
		bindData.setValue(doubleVal);
		qry.bindValue(3, bindData);

		doubleVal = StringToValue(m_ui->edtLongitude->text());
		bindData.setValue(doubleVal);
		qry.bindValue(4, bindData);

		if(m_shapeType > PLACE_POINT) {
			strVal = m_ui->edtHtmlPath->text().trimmed();
			bindData.setValue(strVal);
			qry.bindValue(5, bindData);

			strVal = m_ui->edtVideoPath->text().trimmed();
			bindData.setValue(strVal);
			qry.bindValue(6, bindData);
		}

		if(m_shapeType > OBJECT_POINT) {
			byteVal = pointsWrite();
			bindData.setValue(byteVal);
			qry.bindValue(7, bindData, QSql::In|QSql::Binary);

			QList<double> mbrList = getMBR();

			bindData.setValue(radian2degree(mbrList[1]));
			qry.bindValue(8, bindData);

			bindData.setValue(radian2degree(mbrList[0]));
			qry.bindValue(9, bindData);

			bindData.setValue(radian2degree(mbrList[3]));
			qry.bindValue(10, bindData);

			bindData.setValue(radian2degree(mbrList[2]));
			qry.bindValue(11, bindData);
		}

		if(!qry.exec()) return false;

	}

	if(tbType == E_GDM_SUBDATA_TYPE_NAME_POLUTANT) {
		if(!m_ui->lblName->text().isEmpty()) {
			if(m_objectID < 0) {
				strQry = "select max(id) from " + tableName;
				qry.exec(strQry);
				if(!qry.first()) return false;

				m_objectID = qry.value(0).toInt();
			}

			strQry = "UPDATE " + tableName + " SET accidentname=?, accidentlat=?, accidentlong=? ";
			strQry += "WHERE id=" + QString("%1").arg(m_objectID);
			qry.prepare(strQry);

			strVal = m_ui->lblName->text();
			bindData.setValue(strVal);
			qry.bindValue(0, bindData);

			doubleVal = StringToValue(m_ui->lblLat->text());
			bindData.setValue(doubleVal);
			qry.bindValue(1, bindData);

			doubleVal = StringToValue(m_ui->lblLong->text());
			bindData.setValue(doubleVal);
			qry.bindValue(2, bindData);

			qry.exec();
		}
	}

	return true;
}

void ObjectPlaceEditDlg::edtHtmlPath_change()
{
	QString openDir = g_ServerConfig.GetFileDBPath() + HTML_PATH;
	QString fileName;
	QStringList fList;

	fileName = FileDialog::GetOpenFileName(this, tr("Select Html File"), openDir, tr("Html Files (*.html *.htm)"));
	if (fileName.isEmpty()) return;

	QString orgFile = m_ui->edtHtmlPath->text();

	if (!orgFile.isEmpty() && fileName.contains(orgFile)) return;

	if(!orgFile.isEmpty()) {
		orgFile.remove(HTML_PATH);
		QFileInfo orgFileInfo(openDir + orgFile);
		QString filter = orgFileInfo.baseName() + ".*";
		QDir dir(openDir, filter);
		fList = dir.entryList();
		for(int ii = 0; ii < fList.size(); ii++)
			QFile::remove(openDir + "/" + fList.at(ii));
	}

	QFileInfo fileinfo(fileName);
	if (!fileName.contains(openDir)) {
		QString filter = fileinfo.baseName() + ".*";
		QDir dir(fileinfo.path(), filter);
		fList = dir.entryList();
		for(int ii = 0; ii < fList.size(); ii++)
			QFile::copy(fileinfo.path() + "/" + fList.at(ii), openDir + "/" + fList.at(ii));

		QString orgDir = fileinfo.path();
		//copy addtional file
		if(!dir.exists(openDir + "/img")) {
			dir.mkdir(openDir + "/img");
			QDir tmpDir(orgDir + "/img", "*.png");
			fList = tmpDir.entryList();
			if(fList.count() > 1) {
				for(int ii = 0; ii < fList.count(); ii++)
					QFile::copy(orgDir + "/img/" + fList.at(ii), openDir + "/img/" + fList.at(ii));
			}
		}
	}

	fileName = QString(HTML_PATH) + "/" + fileinfo.fileName();

	m_ui->edtHtmlPath->setText(fileName);
}

void ObjectPlaceEditDlg::edtVideoPath_change()
{
	QString openDir = g_ServerConfig.GetFileDBPath() + VIDEO_PATH;
	QString fileName;

	fileName = FileDialog::GetOpenFileName(this, tr("Select Video File"), openDir, tr("Video Files (*.avi *.mpeg *.mpg)"));
	if (fileName.isEmpty()) return;

	QString orgFile = m_ui->edtVideoPath->text();

	if (!orgFile.isEmpty() && fileName.contains(orgFile)) return;

	if(!orgFile.isEmpty()) {
		orgFile.remove(VIDEO_PATH);
		QFile::remove(openDir + "/" + orgFile);
	}

	QFileInfo fileinfo(fileName);
	if (!fileName.contains(openDir))
		QFile::copy(fileName, openDir + "/" + fileinfo.fileName());

	fileName = QString(VIDEO_PATH) + "/" + fileinfo.fileName();

	m_ui->edtVideoPath->setText(fileName);
}

void ObjectPlaceEditDlg::tabIndex_change(int id)
{
	if(!m_EditMode) return;
	if(m_ui->btnCenterPoint->isChecked()) {
		btnCenterPoint_click(false);
		m_ui->btnCenterPoint->setChecked(false);
	}
	if(id == 2) {
		m_PropertyMode = true;
		//if(m_editingPoints.isEmpty())
		m_editingPoints = pointsWrite();
		m_pathCommand->OnEnd();
		m_pathCommand = NULL;
		CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
		m_pathCommand = (CGDM_DrawObjectCmd*)pCmdMgr->CreateCmd(eGDM_CMD_DRAW_OBJECT, this);
	} else {
		if(m_PropertyMode) {
			if(!m_pathCommand) return;
			m_pathCommand->Clear();
			CGIS_Path *pathNode = new CGIS_Path();
			m_pathCommand->Init(pathNode, true);
			m_pathCommand->SetShapeType(m_shapeType);

			pointsRead(m_editingPoints);
			copyDataToNode();
			//m_editingPoints.clear();

			pathNode->Refresh();
			m_pathCommand->SetTrackIndex(pathNode->Get_TrackCount() - 1);
			m_PropertyMode = false;
		}
	}
}

QString ObjectPlaceEditDlg::ValueToString(double value) //asd 2014.6.4
{
	QString str;
	switch(g_app->GetConfig()->m_llShowMode)
	{
	case OPTION_LL_10NOTATION:
		str = QString("%1%2").arg(value, 10, 'f', 6).arg(QChar(0x00B0));
		break;
	case OPTION_LL_DMS:
		str = gu_Degree2DMS(value, GEO_TYPE_LATITUDE);
		break;
	case OPTION_LL_10NOTATION_DM:
		str = gu_Degree2DM(value, GEO_TYPE_LATITUDE);
		break;
	default:
		break;
	}
	return str;
}

double ObjectPlaceEditDlg::StringToValue(QString &str) //asd 2014.6.4
{
	double value;
	switch(g_app->GetConfig()->m_llShowMode)
	{
	case OPTION_LL_10NOTATION:
		value = gu_Degree2Degree(str);
		break;
	case OPTION_LL_DMS:
		value = gu_DMS2Degree(str, GEO_TYPE_LATITUDE);
		break;
	case OPTION_LL_10NOTATION_DM:
		value = gu_DM2Degree(str, GEO_TYPE_LATITUDE);
		break;
	default:
		break;
	}
	return value;
}

void ObjectPlaceEditDlg::SetCenterMark()
{
	double clong = 0.0;
	double clat = 0.0;
	if(!m_EditMode) return;
	if(!m_ui->edtLongitude->text().isEmpty()) {
		clong = StringToValue(m_ui->edtLongitude->text());
		m_ui->edtLongitude->setText(ValueToString(clong));
	}
	if(!m_ui->edtLatitude->text().isEmpty()) {
		clat = StringToValue(m_ui->edtLatitude->text());
		m_ui->edtLatitude->setText(ValueToString(clat));
	}

	if(clong > 0 && clat > 0) {
		if(!m_EditStart) {
			btnCenterPoint_click(true);
			m_ui->btnCenterPoint->setChecked(true);
			m_EditStart = true;
		} else {
			QModelIndex index = m_pointListModel->index(0, 0, QModelIndex());
			m_pointListModel->setData(index, clat, Qt::EditRole);
			index = m_pointListModel->index(0, 1, QModelIndex());
			m_pointListModel->setData(index, clong, Qt::EditRole);
			copyDataToNode();
			m_pathCommand->m_pNode->Refresh();
		}

		GDM_LOCATION tmp;
		g_app->GetRenderProxy()->GetRenderSrv()->m_pGDMDataMgr->m_pCamera->GetLocation(&tmp);
		double viewHeight = tmp.m_dDist;

		GDM_CAMERA_INFO cameraInfo;	
		memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));

		cameraInfo.m_loc.m_dLongitude =  clong * gdm_DEGTORAD;
		cameraInfo.m_loc.m_dLatitude = clat * gdm_DEGTORAD;
		cameraInfo.m_loc.m_dDist = viewHeight;

		g_app->GetRenderProxy()->GetRenderSrv()->m_pCameraController->AutoMoveTo(0, cameraInfo, 1000);
	}
}
