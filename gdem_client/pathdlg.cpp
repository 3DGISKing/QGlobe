#include "pathdlg.h"
#include "ui_pathdlg.h"
#include "plisttablemodel.h"
#include "gdemclient.h"
#include "renderfrm.h"
#include "proxy.h"
#include "msgdata.h"
#include "guiuty.h"
#include "mainwindow.h"
#include "config.h"
#include "placemarkmgr.h"
#include "calcprogressdlg.h"
// for gis node
#include "GIS_Doc.h"
#include "GIS_Path.h"
#include "GDM_CommandMgr.h"
#include "GDM_DrawPolygonCmd.h"
#include "guiuty.h"

PathDlg::PathDlg(QWidget *parent, bool isAdd) :
	CGDM_CommandDlg(parent),
	m_ui(new Ui::PathDlg)
{
    m_ui->setupUi(this);
	m_pointListModel=NULL;
	InitDialog();
	UpdateOutputInfo();

	m_isAdd = isAdd;
	m_placeMarkMgr = g_app->GetMainWindow()->GetRenderFrame()->GetPlaceMarkMgr();
	memset(&m_placeMarkData, 0, sizeof(m_placeMarkData));
	m_isAdd = isAdd;
	m_state = PLACEDLG_NORMAL;

	connect(m_ui->nameEdit, SIGNAL(textChanged(QString)), this, SLOT(SetPathName()));
	connect(m_ui->descriptionEdit, SIGNAL(textChanged()), this, SLOT(SetPathDescription()));

	connect(m_ui->pathPropertyTab, SIGNAL(currentChanged(int)), this, SLOT(SetFocusOkButton()));
	connect(m_ui->latitudeLineEdit, SIGNAL(editingFinished()), this, SLOT(SetLatitude()));
	connect(m_ui->longitudeLineEdit, SIGNAL(editingFinished()), this, SLOT(SetLongitude()));
	connect(m_ui->rangeLineEdit, SIGNAL(editingFinished()), this, SLOT(SetRange()));
	connect(m_ui->headingLineEdit, SIGNAL(editingFinished()), this, SLOT(SetHeading()));
	connect(m_ui->tiltLineEdit, SIGNAL(editingFinished()), this, SLOT(SetTilt()));
	connect(m_ui->snapshotViewButton, SIGNAL(clicked()), this, SLOT(ViewCurrentSnapshot()));
	connect(m_ui->resetButton, SIGNAL(clicked()), this, SLOT(ResetView()));
	
	connect(m_ui->lineGroupBox, SIGNAL(clicked()), this, SLOT(SetLineCheck()));
	connect(m_ui->lineColorButton, SIGNAL(clicked()), this, SLOT(SetLineColor()));
	connect(m_ui->lineThickSpin, SIGNAL(valueChanged(QString)), this, SLOT(SetLineThick()));
	connect(m_ui->lineOpacitySpin, SIGNAL(valueChanged(QString)), this, SLOT(SetLineOpacity()));
	connect(m_ui->fillGroupBox, SIGNAL(clicked()), this, SLOT(SetFillCheck()));
	connect(m_ui->fillColorButton, SIGNAL(clicked()), this, SLOT(SetFillColor()));
	connect(m_ui->fillOpacitySpin, SIGNAL(valueChanged(QString)), this, SLOT(SetFillOpacity()));
	connect(m_ui->heightModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(SetLineHeightMode(int)));
	connect(m_ui->heightLineEdit, SIGNAL(textChanged(QString)), this, SLOT(SetLineHeight()));
	connect(m_ui->lineHeightlSlider, SIGNAL(actionTriggered(int)), this, SLOT(OnSliderAct(int)));
	//connect(m_ui->lineHeightlSlider, SIGNAL(sliderMoved(int)), this, SLOT(SetLineHeightWith(int)));
	connect(m_ui->extendCheckBox, SIGNAL(clicked()), this, SLOT(SetExtended()));
	connect(m_ui->addButton, SIGNAL(clicked()), this, SLOT(AddPointData()));
	connect(m_ui->editButton, SIGNAL(clicked()), this, SLOT(EditPointData()));
	connect(m_ui->deleteButton, SIGNAL(clicked()), this, SLOT(DelPointData()));
	connect(m_pointListModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(ChangedPointData()));

	connect(m_ui->calcGroupBox, SIGNAL(clicked()), this, SLOT(SetCalcWithDem()));
	connect(m_ui->calcPushButton, SIGNAL(clicked()), this, SLOT(StartCalc()));

	connect(m_ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

PathDlg::~PathDlg()
{
    delete m_ui;

	if (m_state == PLACEDLG_ACCEPT)
		m_placeMarkMgr->GetApplyPlaceData(&m_placeMarkData, m_isAdd);
	else if (m_state == PLACEDLG_REJECT)
		m_placeMarkMgr->GetCancelPlaceData(&m_placeMarkData, m_isAdd);

	m_placeMarkMgr->SetResultTreeView(NULL);

	if(m_pointListModel)
	{
		delete m_pointListModel;
		m_pointListModel=NULL;
	}
}

void PathDlg::UpdateDialog()
{
	copyDataFromNode();
	UpdateOutputInfo();
}

void PathDlg::InitDialog()
{
	QPoint mainPos = g_app->GetMainWindow()->pos();
	move(mainPos.x()+20, mainPos.y()+100);

	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
				   | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setFixedSize(size());

	m_ui->pathPropertyTab->setCurrentIndex(2);
	m_ui->okButton->setFocus(Qt::OtherFocusReason);
	m_ui->nameEdit->setMaxLength(PATHDLG_STR_MAX_SIZE-1);

	QString name = tr("Unnamed Path");
	gu_qstring2ushort(name, m_pathName, PATHDLG_STR_MAX_SIZE);
	memset(&m_pathDesc, 0, sizeof(m_pathDesc));

	QString suffix = QString("%1").arg(QChar(0xFF05));
	m_ui->lineOpacitySpin->setSuffix(suffix);
	m_ui->fillOpacitySpin->setSuffix(suffix);
	m_lineChecked = true;
	m_lineColor = QColor(Qt::green);
	m_lineThick = 1.0;
	m_lineOpacity = 1.0;
	m_fillChecked = true;
	m_fillColor = QColor(Qt::green);
	m_fillOpacity = 1.0;

	QStringList items;
	items << tr("Clamped to ground") << tr("Relative to ground") << tr("Absolute");
	m_heightMode = 0;
	m_ui->heightModeCombo->addItems(items);
	m_ui->heightModeCombo->setCurrentIndex(m_heightMode);

	m_heightFromEarth = 0;
	m_extended = false;

	QList <PointData> data;
	m_pointListModel = new PListTableModel(data);
	m_ui->pListTableView->setModel(m_pointListModel);
	m_ui->pListTableView->horizontalHeader()->setDefaultSectionSize(150);

	m_length = 0;
	m_calcWithDem = false;
	m_lengthWithDem = 0;
	m_maxHeight = 0;
	m_maxHtLati = 0;
	m_maxHtLongi = 0;
	m_minHeight = 0;
	m_minHtLati = 0;
	m_minHtLongi = 0;
	m_maxRate = 0;
	m_maxRateLati = 0;
	m_maxRateLongi = 0;
}

void PathDlg::copyDataToNode(bool bRefresh)
{
	if(m_blNoDataCopy)
		return;

	if(m_pCommand == NULL)
		return;

	CGIS_Path *pNode = (CGIS_Path *)m_pCommand->m_pNode;
	if (pNode == NULL)
		return;

	// name
	memcpy(m_placeMarkData.name, m_pathName, sizeof(m_pathName));

	// description
	memcpy(m_placeMarkData.description, m_pathDesc, sizeof(m_pathDesc));

	// color
	pNode->m_sPen.color = ((((int)(m_lineOpacity * 255 + 0.5f)) & 0xff) << 24) + (m_lineColor.red() << 16) + 
		(m_lineColor.green() << 8) + m_lineColor.blue();

	// line thick
	pNode->m_sPen.width = m_lineThick;

	// fill color
	pNode->m_sBrush.color = ((((int)(m_fillOpacity * 255 + 0.5f)) & 0xff) << 24) + (m_fillColor.red() << 16) + 
		(m_fillColor.green() << 8) + m_fillColor.blue();

	// polygon mode
	if(m_fillChecked && m_lineChecked)
		pNode->m_nDrawType = E_GISDRAWPOLY_ALL;
	else if(!m_fillChecked && m_lineChecked)
		pNode->m_nDrawType = E_GISDRAWPOLY_LINE;
	else if(m_fillChecked && (!m_lineChecked))
		pNode->m_nDrawType = E_GISDRAWPOLY_FILL;
	else
		pNode->m_nDrawType = E_GISDRAWPOLY_NONE;

	// height mode
	pNode->m_nHeightType = (E_GISPOLYHEIGHT_TYPE)m_heightMode;

	// extend height to earth
	pNode->m_dHeight = m_heightFromEarth;
	
	// extend flag
	pNode->m_blHeight = m_extended;

	// point list data of polygon
	m_pointListModel->getData(pNode->m_PtArr);

	// length of polygon
	pNode->m_dLength = m_length;

	if(bRefresh)
	{
		pNode->Refresh();
		copyDataFromNode();
	}
}

void PathDlg::copyDataFromNode()
{
	if(m_blNoDataCopy)
		return;
	if(m_pCommand == NULL)
		return;

	CGIS_Path *pNode = (CGIS_Path *)m_pCommand->m_pNode;
	if (pNode == NULL)
		return;

	// name
	memcpy(m_pathName, m_placeMarkData.name, sizeof(m_pathName));

	// description
	memcpy(m_pathDesc, m_placeMarkData.description, sizeof(m_pathDesc));

	// color
	unsigned int clr = pNode->m_sPen.color;
	m_lineColor = QColor((clr >> 16) & 0xff, (clr >> 8) & 0xff, (clr) & 0xff);

	// line thick
	m_lineThick = pNode->m_sPen.width;
	m_lineOpacity = ((clr >> 24) & 0xff) / 255.0f;

	// fill color
	clr = pNode->m_sBrush.color;
	m_fillColor = QColor((clr >> 16) & 0xff, (clr >> 8) & 0xff, (clr) & 0xff);
	m_fillOpacity = ((clr >> 24) & 0xff) / 255.0f;

	// polygon mode
	switch(pNode->m_nDrawType)
	{
	case E_GISDRAWPOLY_ALL:
		m_fillChecked = true;
		m_lineChecked = true;
		break;
	case E_GISDRAWPOLY_LINE:
		m_fillChecked = false;
		m_lineChecked = true;
		break;
	case E_GISDRAWPOLY_FILL:
		m_fillChecked = true;
		m_lineChecked = false;
		break;
	case E_GISDRAWPOLY_NONE:
		m_fillChecked = false;
		m_lineChecked = false;
		break;
	}

	// height mode
	m_heightMode = pNode->m_nHeightType;

	// extend height to earth
	m_heightFromEarth = pNode->m_dHeight;
	// extend flag
	m_extended = pNode->m_blHeight;

	// point list data of polygon
	m_pointListModel->setData(pNode->m_PtArr);

	// length of polygon
	m_length = pNode->m_dLength;

	// calulation flag
	m_calcWithDem = pNode->m_blCalculated;

	// length with surface
	m_lengthWithDem = pNode->m_dLengthOnSurface;

	// max height
	m_maxHeight		= pNode->m_sMaxHeightPos.height;
	m_maxHtLati		= radian2degree(pNode->m_sMaxHeightPos.latitude);
	m_maxHtLongi	= radian2degree(pNode->m_sMaxHeightPos.longitude);

	// min height
	m_minHeight		= pNode->m_sMinHeightPos.height;
	m_minHtLati		= radian2degree(pNode->m_sMinHeightPos.latitude);
	m_minHtLongi	= radian2degree(pNode->m_sMinHeightPos.longitude);

	// max rate
	m_maxRate		= pNode->m_sMaxRatePos.rate;
	m_maxRateLati	= radian2degree(pNode->m_sMaxRatePos.latitude);
	m_maxRateLongi	= radian2degree(pNode->m_sMaxRatePos.longitude);
}

void PathDlg::GetNewPointData(PointData &pd, int curRow)
{
	QList <PointData> list = m_pointListModel->getList();

	CGDM_Location3D prePos, nextPos, newPos;
	
	if (curRow != 0 && list.size() >= curRow+1)  // insert
	{
		prePos.m_tX = degree2radian((list.at(curRow-1).m_longitude).toDouble());
		prePos.m_tY = degree2radian((list.at(curRow-1).m_latitude).toDouble());
		nextPos.m_tX = degree2radian((list.at(curRow).m_longitude).toDouble());
		nextPos.m_tY = degree2radian((list.at(curRow).m_latitude).toDouble());
		GetValidPos(&prePos, &nextPos, &newPos, false);
	}
	else if (list.size() >= 2)
	{
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
	}
	else if (list.size() == 1)
	{
		prePos.m_tX = degree2radian((list.at(0).m_longitude).toDouble());
		prePos.m_tY = degree2radian((list.at(0).m_latitude).toDouble());
		nextPos.m_tX = degree2radian((list.at(0).m_longitude).toDouble());
		nextPos.m_tY = degree2radian((list.at(0).m_latitude).toDouble());
		GetValidPos(&prePos, &nextPos, &newPos, false);
	}
	else
	{
		GetValidPos(NULL, NULL, &newPos, false);
	}

	pd.m_latitude = QString("%1").arg(radian2degree(newPos.m_tY));
	pd.m_longitude = QString("%1").arg(radian2degree(newPos.m_tX));
}

/*	From 0 ~ 512 
	To 1~80(step 1),81~160(step 5), 161~240(step 25), 241~320(step 125)
	,321~400(step 625),401~480(step 3125),481~512(15625) = 1 ~ 800000
*/
static void slider2exp(double &sum, double i)
{
	if (i > 0)
	{
		i--;
		sum += pow(5.0, i) * 80;
		slider2exp(sum, i);
	}
}

static void exp2slider(double &value)
{
	int i = 0;
	double sum = 0;
	for (i = 1; i < 9; i++)
	{
		sum = 0;
		slider2exp(sum, i);

		if (value <= sum)
			break;
	}

	if (i > 1)
	{
		double preSum = 0;
		slider2exp(preSum, i-1);

		double delta = (value - preSum) / pow(5.0, i-1);
		value = 80*(i-1) + delta;
	}
}

void PathDlg::UpdateOutputInfo()
{
	m_blNoDataCopy = true;
	QString str = QString::fromUtf16(m_pathName);
	m_ui->nameEdit->setText(str);

	str = QString::fromUtf16(m_pathDesc);
	m_ui->descriptionEdit->setPlainText(str);

	// View Tab
	switch(g_app->GetConfig()->m_llShowMode)
	{
		case OPTION_LL_10NOTATION:
			str = QString("%1%2").arg(m_placeMarkData.view_latitude, 10, 'f', 6).arg(QChar(0x00B0));
			m_ui->latitudeLineEdit->setText(str);
			str = QString("%1%2").arg(m_placeMarkData.view_longitude, 10, 'f', 6).arg(QChar(0x00B0));
			m_ui->longitudeLineEdit->setText(str);
			break;
		case OPTION_LL_DMS:
			str = gu_Degree2DMS(m_placeMarkData.view_latitude, GEO_TYPE_LATITUDE);
			m_ui->latitudeLineEdit->setText(str);
			str = gu_Degree2DMS(m_placeMarkData.view_longitude, GEO_TYPE_LONGITUDE);
			m_ui->longitudeLineEdit->setText(str);
			break;
		case OPTION_LL_10NOTATION_DM:
			str = gu_Degree2DM(m_placeMarkData.view_latitude, GEO_TYPE_LATITUDE);
			m_ui->latitudeLineEdit->setText(str);
			str = gu_Degree2DM(m_placeMarkData.view_longitude, GEO_TYPE_LONGITUDE);
			m_ui->longitudeLineEdit->setText(str);
			break;
		default:
			break;
	}

	unsigned int range = (unsigned int) (m_placeMarkData.view_range + 0.5);
	str.sprintf("%dm", range);
	m_ui->rangeLineEdit->setText(str);

	double val;
	if (m_placeMarkData.view_heading > -DELTA && m_placeMarkData.view_heading < DELTA)
		val = 0.0;
	else
		val = -m_placeMarkData.view_heading;
	str = QString("%1%2").arg(val, 0, 'f', 6).arg(QChar(0x00B0));
	m_ui->headingLineEdit->setText(str);

	if (m_placeMarkData.view_tilt > -DELTA && m_placeMarkData.view_tilt < DELTA)
		val = 0.0;
	else
		val = m_placeMarkData.view_tilt;
	str = QString("%1%2").arg(val, 0, 'f', 6).arg(QChar(0x00B0));
	m_ui->tiltLineEdit->setText(str);

	// Shape Property Tab
	m_ui->lineGroupBox->setChecked(m_lineChecked);
	QString sheet = QString("background-color: %1").arg(m_lineColor.name());
	m_ui->lineColorButton->setStyleSheet(sheet);
	m_ui->lineThickSpin->setValue(m_lineThick);
	m_ui->lineOpacitySpin->setValue((int)(m_lineOpacity*100 + 0.5f));
	m_ui->fillGroupBox->setChecked(m_fillChecked);
	sheet = QString("background-color: %1").arg(m_fillColor.name());
	m_ui->fillColorButton->setStyleSheet(sheet);
	m_ui->fillOpacitySpin->setValue((int)(m_fillOpacity*100 + 0.5f));

	if (m_heightMode == 0)
	{
		m_ui->heightLineEdit->setDisabled(true);
		m_ui->lineHeightlSlider->setDisabled(true);
		m_ui->extendCheckBox->setDisabled(true);
	}
	else
	{
		m_ui->heightLineEdit->setEnabled(true);
		m_ui->lineHeightlSlider->setEnabled(true);
		m_ui->extendCheckBox->setEnabled(true);
	}
	m_ui->heightModeCombo->setCurrentIndex(m_heightMode);
	
	str = QString("%1m").arg(m_heightFromEarth);
	m_ui->heightLineEdit->setText(str);
	m_ui->heightLineEdit->setCursorPosition(str.length()-1);

	val = m_heightFromEarth;
	if (val != 800000)
		exp2slider(val);
	m_ui->lineHeightlSlider->setValue((int)val);
	m_ui->extendCheckBox->setChecked(m_extended);

	// GIS Property Tab
	str = QString("%1").arg(gu_m2km(m_length, GIS_UNIT_LENGTH));
	m_ui->lengthValueLabel->setText(str);

	m_ui->calcGroupBox->setChecked(m_calcWithDem);
	str = QString("%1").arg(gu_m2km(m_lengthWithDem, GIS_UNIT_LENGTH));
	m_ui->calcLengthValueLabel->setText(str);
	
	// Max Height
	str = QString("%1").arg(gu_m2km(m_maxHeight, GIS_UNIT_LENGTH));
	m_ui->maxHeightValueLabel->setText(str);
	str = QString("%1%2").arg(m_maxHtLati).arg(QChar(0x00B0));
	m_ui->maxHeightLatiValueLabel->setText(str);
	str = QString("%1%2").arg(m_maxHtLongi).arg(QChar(0x00B0));
	m_ui->maxHeightLongiValueLabel->setText(str);

	// Min Height
	str = QString("%1").arg(gu_m2km(m_minHeight, GIS_UNIT_LENGTH));
	m_ui->minHeightValueLabel->setText(str);
	str = QString("%1%2").arg(m_minHtLati).arg(QChar(0x00B0));
	m_ui->minHeightLatiValueLabel->setText(str);
	str = QString("%1%2").arg(m_minHtLongi).arg(QChar(0x00B0));
	m_ui->minHeightLongiValueLabel->setText(str);

	// Max Rate
	str = QString("%1%2").arg(m_maxRate).arg(QChar(0x00B0));
	m_ui->maxRateValueLabel->setText(str);
	str = QString("%1%2").arg(m_maxRateLati).arg(QChar(0x00B0));
	m_ui->maxRateLatiValueLabel->setText(str);
	str = QString("%1%2").arg(m_maxRateLongi).arg(QChar(0x00B0));
	m_ui->maxRateLongiValueLabel->setText(str);

	m_blNoDataCopy = false;

	EnableViewControl(m_placeMarkData.snapshot);
}

void PathDlg::EnableViewControl(bool enable)
{
	if (!enable)
	{
		m_ui->latitudeLineEdit->setText(MSG_PLACEFOLDERDLG_NO);
		m_ui->longitudeLineEdit->setText(MSG_PLACEFOLDERDLG_NO);
		m_ui->rangeLineEdit->setText(MSG_PLACEFOLDERDLG_NO);
		m_ui->headingLineEdit->setText(MSG_PLACEFOLDERDLG_NO);
		m_ui->tiltLineEdit->setText(MSG_PLACEFOLDERDLG_NO);
	}

	m_ui->latitudeLineEdit->setEnabled(enable);
	m_ui->longitudeLineEdit->setEnabled(enable);
	m_ui->rangeLineEdit->setEnabled(enable);
	m_ui->headingLineEdit->setEnabled(enable);
	m_ui->tiltLineEdit->setEnabled(enable);
}

void PathDlg::showEvent(QShowEvent *event)
{
	CGIS_Path *pNode;

	// create command
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	m_pCommand = (CGDM_DrawPolygonCmd*)pCmdMgr->CreateCmd(eGDM_CMD_DRAW_POLYGON, this);

	if (m_isAdd)
	{
		m_placeMarkMgr->GetNewPlaceMarkData(&m_placeMarkData, IT_PATH);

		// add place mark node 
		pNode = new CGIS_Path();
		pNode->SetKey(m_placeMarkData.key);
		m_pCommand->Init(pNode, true);

		// end
	}
	else
	{
		setWindowTitle(tr("Edit Path"));
		m_placeMarkMgr->GetPropertyPlaceData(&m_placeMarkData, m_isAdd);

		pNode = (CGIS_Path*)gdm_GetGISDoc()->GetNodeFromKey(m_placeMarkData.key);
		m_pCommand->Init(pNode, false);
	}
	copyDataFromNode();
	UpdateOutputInfo();
}

void PathDlg::accept()
{
	m_state = PLACEDLG_ACCEPT;
	g_app->GetMainWindow()->UpdatePlaceDlg(true);

	UpdateLookAt();
	m_pCommand->OnEnd();
	QDialog::accept();
}

void PathDlg::reject()
{
	m_state = PLACEDLG_REJECT;
	g_app->GetMainWindow()->UpdatePlaceDlg(true);

	m_pCommand->OnCancel();
	m_pCommand = NULL;
	QDialog::reject();
}

void PathDlg::SetPathName()
{
	QString str = m_ui->nameEdit->text();
	memset(&m_pathName, 0, sizeof(m_pathName));
	gu_qstring2ushort(str, m_pathName, PATHDLG_STR_MAX_SIZE);

	copyDataToNode(false);
}

void PathDlg::SetPathDescription()
{
	QString str = m_ui->descriptionEdit->toPlainText();
	if (str.length() < PATHDLG_STR_MAX_SIZE)
	{
		memset(&m_pathDesc, 0, sizeof(m_pathDesc));
		gu_qstring2ushort(str, m_pathDesc, PATHDLG_STR_MAX_SIZE);
	}
	else
	{
		m_ui->descriptionEdit->setPlainText(m_ui->descriptionEdit->toPlainText().left(
			m_ui->descriptionEdit->toPlainText().length()-1));
		m_ui->descriptionEdit->moveCursor(QTextCursor::End);
	}
	copyDataToNode(false);
}

void PathDlg::SetFocusOkButton()
{
	m_ui->okButton->setFocus(Qt::OtherFocusReason);
}

void PathDlg::SetLatitude()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->latitudeLineEdit->text();
	double res = 0.0;

	switch(g_app->GetConfig()->m_llShowMode)
	{
		case OPTION_LL_10NOTATION:
			res = gu_Degree2Degree(str);
			break;
		case OPTION_LL_DMS:
			res = gu_DMS2Degree(str, GEO_TYPE_LATITUDE);
			break;
		case OPTION_LL_10NOTATION_DM:
			res = gu_DM2Degree(str, GEO_TYPE_LATITUDE);
			break;
		default:
			break;
	}

	if ((res + 90) >= -DELTA && (res - 90 <= DELTA))
	{
		m_placeMarkData.view_latitude = res;
		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetLongitude()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->longitudeLineEdit->text();
	double res = 0.0;

	switch(g_app->GetConfig()->m_llShowMode)
	{
		case OPTION_LL_10NOTATION:
			res = gu_Degree2Degree(str);
			break;
		case OPTION_LL_DMS:
			res = gu_DMS2Degree(str, GEO_TYPE_LONGITUDE);
			break;
		case OPTION_LL_10NOTATION_DM:
			res = gu_DM2Degree(str, GEO_TYPE_LONGITUDE);
			break;
		default:
			break;
	}

	if ((res + 180 >= -DELTA) && (res - 180 <= DELTA))
	{
		m_placeMarkData.view_longitude = res;
		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetRange()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->rangeLineEdit->text();
	QRegExp rx("^\\s*(\\d+(\\.\\d*)?)\\s*[\\m]$");

	if (rx.exactMatch(str))
	{
		if (rx.cap(1).toDouble() > 63710098)
			m_placeMarkData.view_range = 63710098;
		else
			m_placeMarkData.view_range = rx.cap(1).toDouble();

		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetHeading()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->headingLineEdit->text();
	QRegExp rx("^\\s*([\\-+]?\\d+(\\.\\d*)?)\\s*[\\x00b0]$");

	if (rx.exactMatch(str))
	{
		if (rx.cap(1).toDouble() > 180.00)
			m_placeMarkData.view_heading = 180.00;
		else if (rx.cap(1).toDouble() < -180.00)
			m_placeMarkData.view_heading = -180.00;
		else
			m_placeMarkData.view_heading = -rx.cap(1).toDouble();

		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetTilt()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->tiltLineEdit->text();
	QRegExp rx("^\\s*(\\d+(\\.\\d*)?)\\s*[\\x00b0]$");

	if (rx.exactMatch(str))
	{
		if (rx.cap(1).toDouble() > 90.00)
			m_placeMarkData.view_tilt = 90.00;
		else
			m_placeMarkData.view_tilt = rx.cap(1).toDouble();

		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::ViewCurrentSnapshot()
{
	m_placeMarkMgr->GetSnapshotPlaceData(&m_placeMarkData);

	copyDataToNode(false);
	UpdateOutputInfo();
}

void PathDlg::ResetView()
{
	m_placeMarkMgr->GetResetPlaceData(&m_placeMarkData);

	copyDataToNode(false);
	UpdateOutputInfo();
}
void PathDlg::SetLineCheck()
{
	m_lineChecked = m_ui->lineGroupBox->isChecked();

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetLineColor()
{
#ifdef Q_WS_MAC
	QColor color = QColorDialog::getColor(m_lineColor, this, QString(),
										  QColorDialog::DontUseNativeDialog);
#else
	QColor color = QColorDialog::getColor(m_lineColor);
#endif
	if (color.isValid())
		m_lineColor = color;

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetLineThick()
{
	double thick = m_ui->lineThickSpin->value();
	if (thick >=0 && thick <= 99)
		m_lineThick = thick;

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetLineOpacity()
{
	double opacity = m_ui->lineOpacitySpin->value();
	if (opacity >= 0 && opacity <= 100)
		m_lineOpacity = opacity/100 + DELTA;

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetFillCheck()
{
	m_fillChecked = m_ui->fillGroupBox->isChecked();

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetFillColor()
{
#ifdef Q_WS_MAC
	QColor color = QColorDialog::getColor(m_fillColor, this, QString(),
										  QColorDialog::DontUseNativeDialog);
#else
	QColor color = QColorDialog::getColor(m_fillColor);
#endif
	if (color.isValid())
		m_fillColor = color;

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetFillOpacity()
{
	double opacity = m_ui->fillOpacitySpin->value();
	if (opacity >= 0 && opacity <= 100)
		m_fillOpacity = opacity/100 + DELTA;

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetLineHeightMode(int mode)
{
	m_heightMode = mode;
	if (m_heightMode == 0)
	{
		m_heightFromEarth = 0;
		m_extended = false;
	}
	
	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetLineHeight()
{
	QString str = m_ui->heightLineEdit->text();
	str.replace("m", "");

	double value = str.toDouble();

	if (value >= 0 && value <= 800000)
		m_heightFromEarth = value;

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::SetLineHeightWith(int val)
{
	double div = val / 80;
	double sum = 0;
	
	slider2exp(sum, div);
	
	m_heightFromEarth = sum + (val%80) * pow(5, div);

	if (m_heightFromEarth > 800000)
		m_heightFromEarth = 800000;

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::OnSliderAct(int a_nAct)
{
    int nPos = m_ui->lineHeightlSlider->sliderPosition();

    switch (a_nAct)
    {
    case QAbstractSlider::SliderPageStepAdd:
    case QAbstractSlider::SliderPageStepSub:
    case QAbstractSlider::SliderMove:
        SetLineHeightWith(nPos);
        break;
    default:
        break;
    }
}

void PathDlg::SetExtended()
{
	m_extended = m_ui->extendCheckBox->isChecked();

	copyDataToNode();
	UpdateOutputInfo();
}

void PathDlg::AddPointData()
{
	QList < PointData> list = m_pointListModel->getList();
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

void PathDlg::EditPointData()
{
	QModelIndex curIndex = m_ui->pListTableView->currentIndex();
	m_ui->pListTableView->edit(curIndex);
}

void PathDlg::ChangedPointData()
{
	copyDataToNode();
}

void PathDlg::DelPointData()
{
	QModelIndex curIndex = m_ui->pListTableView->currentIndex();
	int curRow = curIndex.row();
	if (curIndex.row() < 0)
		curRow = m_pointListModel->rowCount(QModelIndex());
	m_pointListModel->removeRows(curRow, 1, QModelIndex());
	copyDataToNode();
}

void PathDlg::SetCalcWithDem()
{
	m_calcWithDem = m_ui->calcGroupBox->isChecked();

	UpdateOutputInfo();
	copyDataToNode(false);
}

void PathDlg::StartCalc()
{
	if(m_pCommand == NULL)
		return;

	CalcProgressDlg *dlg = new CalcProgressDlg(this);
	dlg->show();

	m_pCommand->m_pNode->SetProgressBar((IGIS_Progress*)dlg);
	m_pCommand->m_pNode->OnCalculateGISInfo();

	copyDataFromNode();
	UpdateOutputInfo();
}

void PathDlg::UpdateLookAt()
{
	if(!m_placeMarkData.snapshot)
	{
		CGIS_Polygon* pNode = (CGIS_Polygon*) m_pCommand->GetNode();
		GDM_CAMERA_INFO cameraInfo;
		if(GetLookAtInfo(pNode, &cameraInfo))
		{
			m_placeMarkData.view_longitude = radian2degree(cameraInfo.m_loc.m_dLongitude);
			m_placeMarkData.view_latitude = radian2degree(cameraInfo.m_loc.m_dLatitude);
			m_placeMarkData.view_range = cameraInfo.m_loc.m_dDist;
			m_placeMarkData.view_tilt = radian2degree(cameraInfo.m_pitch);
			m_placeMarkData.view_heading = radian2degree(cameraInfo.m_roll);
		}
	}
}
