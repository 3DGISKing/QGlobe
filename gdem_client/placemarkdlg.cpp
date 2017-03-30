#include "placemarkdlg.h"
#include "ui_placemarkdlg.h"
#include "gdemclient.h"
#include "renderfrm.h"
#include "proxy.h"
#include "msgdata.h"
#include "guiuty.h"
#include "mainwindow.h"
#include "config.h"
#include "placemarkmgr.h"

// for gis node
#include "GIS_Doc.h"
#include "GIS_PlaceIcon.h"
#include "GDM_CommandMgr.h"
#include "GDM_PointCmd.h"
// end

#include "pngbrowser.h"
#include "filedialog.h"

PlaceMarkDlg::PlaceMarkDlg(QWidget *parent, bool isAdd) :
	CGDM_CommandDlg(parent),
    m_ui(new Ui::PlaceMarkDlg)
{
    m_ui->setupUi(this);

	QPoint mainPos = g_app->GetMainWindow()->pos();
	move(mainPos.x()+20, mainPos.y()+100);

	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
								   | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	m_ui->propertyTab->setCurrentIndex(0);
	m_ui->okButton->setFocus(Qt::OtherFocusReason);
	m_ui->lineName->setMaxLength(PMD_STR_MAX_SIZE-1);

	m_ui->lineIconName->setMaxLength(PMD_STR_MAX_SIZE-1);//by RSH 2013.7.30
	m_ui->lineVideoPath->setMaxLength(PMD_STR_MAX_SIZE-1);//by ugi 2014 2 6
	m_ui->lineHtmlPath->setMaxLength(PMD_STR_MAX_SIZE-1);//by ugi 2014 2 7

	m_placeMarkMgr = g_app->GetMainWindow()->GetRenderFrame()->GetPlaceMarkMgr();
	memset(&m_placeMarkData, 0, sizeof(m_placeMarkData));
	m_isAdd = isAdd;
	m_state = PLACEDLG_NORMAL;
	m_viewcenter = false;

	connect(m_ui->propertyTab, SIGNAL(currentChanged(int)), this, SLOT(SetFocusOkButton()));
	connect(m_ui->lineName, SIGNAL(textChanged(QString)), this, SLOT(SetMarkName()));
	connect(m_ui->lineLongitude, SIGNAL(editingFinished()), this, SLOT(SetMarkLongitude()));
	connect(m_ui->lineLatitude, SIGNAL(editingFinished()), this, SLOT(SetMarkLatitude()));
	connect(m_ui->btnGo, SIGNAL(clicked()), this, SLOT(btnGo_click()));							//asd 2014.6.4
	connect(m_ui->textDescription, SIGNAL(textChanged()), this, SLOT(SetMarkDescription()));
	connect(m_ui->lineViewLongitude, SIGNAL(editingFinished()), this, SLOT(SetViewLongitude()));
	connect(m_ui->lineViewLatitude, SIGNAL(editingFinished()), this, SLOT(SetViewLatitude()));
	connect(m_ui->lineViewRange, SIGNAL(editingFinished()), this, SLOT(SetViewRange()));
	connect(m_ui->lineViewHeading, SIGNAL(editingFinished()), this, SLOT(SetViewHeading()));
	connect(m_ui->lineViewTilt, SIGNAL(editingFinished()), this, SLOT(SetViewTilt()));
	connect(m_ui->viewerCenterCheck, SIGNAL(clicked()), this, SLOT(SetCenterView()));
	connect(m_ui->snapshotViewButton, SIGNAL(clicked()), this, SLOT(SnapshotView()));
	connect(m_ui->resetButton, SIGNAL(clicked()), this, SLOT(ResetView()));
	connect(m_ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	
	connect(m_ui->iconnameChangeButton, SIGNAL(clicked()), this, SLOT(IconChange()));//by RSH 2013.7.26
	connect(m_ui->horizontalSliderIconScale,SIGNAL(sliderMoved(int)),SLOT(IconScaleChange(int))); //by ugi 2013.8.1
	connect(m_ui->videoPathChangeButton, SIGNAL(clicked()), this, SLOT(VideoPathChange()));//by ugi 2014 2 6
	connect(m_ui->htmlPathChangeButton, SIGNAL(clicked()), this, SLOT(HtmlPathChange()));//by ugi 2014 2 7
	connect(m_ui->lineIconName,SIGNAL(textChanged(QString)),this,SLOT(SetIconPath()));
	connect(m_ui->lineVideoPath,SIGNAL(textChanged(QString)),this,SLOT(SetVideoPath()));
	connect(m_ui->lineHtmlPath,SIGNAL(textChanged(QString)),this,SLOT(SetHtmlPath()));
}

PlaceMarkDlg::~PlaceMarkDlg()
{
    delete m_ui;

	if (m_state == PLACEDLG_ACCEPT)
		m_placeMarkMgr->GetApplyPlaceData(&m_placeMarkData, m_isAdd);
	else if (m_state == PLACEDLG_REJECT)
		m_placeMarkMgr->GetCancelPlaceData(&m_placeMarkData, m_isAdd);

	m_placeMarkMgr->SetResultTreeView(NULL);
}

void PlaceMarkDlg::accept()
{
	m_state = PLACEDLG_ACCEPT;
	g_app->GetMainWindow()->UpdatePlaceDlg(true);

	m_pCommand->OnEnd();

	QDialog::accept();
}

void PlaceMarkDlg::reject()
{
	m_state = PLACEDLG_REJECT;
	g_app->GetMainWindow()->UpdatePlaceDlg(true);

	m_pCommand->OnCancel();

	QDialog::reject();
}

void PlaceMarkDlg::UpdateData(PLACEMARK_DATA *pmd)
{
	m_placeMarkData = *pmd;
	UpdateOutputInfo();
}

bool PlaceMarkDlg::IsCheckedViewCenter()
{
	return m_viewcenter;
}

void PlaceMarkDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
	default:
        break;
    }
}

void PlaceMarkDlg::showEvent(QShowEvent *event)
{
	CGIS_PlaceIcon *pNode;

	// create command
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	m_pCommand = (CGDM_PointCmd*)pCmdMgr->CreateCmd(eGDM_CMD_PLACEMARK, this);

	if (m_isAdd)
	{
		m_placeMarkMgr->GetNewPlaceMarkData(&m_placeMarkData, IT_PLACEMARK);

		// add place mark node 
		pNode = new CGIS_PlaceIcon();
		pNode->SetKey(m_placeMarkData.key);
		m_pCommand->Init(pNode, true);

		// end
	}
	else
	{
		setWindowTitle(tr("Edit PlaceMark"));
		m_placeMarkMgr->GetPropertyPlaceData(&m_placeMarkData, m_isAdd);

		pNode = (CGIS_PlaceIcon*)gdm_GetGISDoc()->GetNodeFromKey(m_placeMarkData.key);
		m_pCommand->Init(pNode, false);

		CopyDataFromNode();
	}

	UpdateOutputInfo();
}

// inherited function
void PlaceMarkDlg::UpdateDialog(void)
{
	MoveCenterPlaceMark();
	CopyDataFromNode();
	if(!m_viewcenter)
	{
		m_placeMarkMgr->GetSnapshotPlaceData(&m_placeMarkData);
	}
	UpdateOutputInfo();

}

void PlaceMarkDlg::CopyDataToNode()
{
	if(m_pCommand->m_pNode == NULL)
		return;

	m_pCommand->m_pNode->SetPt(m_placeMarkData.longitude * PAI / 180, m_placeMarkData.latitude * PAI / 180,
		m_placeMarkData.altitude);

	memcpy(m_pCommand->m_pNode->m_sName, m_placeMarkData.name, PMD_STR_MAX_SIZE);

	//by RSH 2013.7.30
	memcpy(((CGIS_PlaceIcon*)m_pCommand->m_pNode)->m_sIconName, m_placeMarkData.iconname, PMD_STR_MAX_SIZE);
	
	((CGIS_PlaceIcon*)m_pCommand->m_pNode)->m_IconScale=m_placeMarkData.iconscale;// by ugi 2013.8.1

	//2014 2 6 by ugi
	memcpy(((CGIS_PlaceIcon*)m_pCommand->m_pNode)->m_sVideoPath, m_placeMarkData.videopath, PMD_STR_MAX_SIZE);
	//2014 2 7 by ugi
	memcpy(((CGIS_PlaceIcon*)m_pCommand->m_pNode)->m_sHtmlPath, m_placeMarkData.htmlpath, PMD_STR_MAX_SIZE);
}

void PlaceMarkDlg::CopyDataFromNode()
{
	if(m_pCommand->m_pNode == NULL)
		return;
	m_placeMarkData.longitude  = m_pCommand->m_pNode->GetPt().m_tX * 180 / PAI;
	m_placeMarkData.latitude   = m_pCommand->m_pNode->GetPt().m_tY * 180 / PAI;		
}

void PlaceMarkDlg::EnableViewControl(bool enable)
{
	if (!enable)
	{
		m_ui->lineViewLatitude->setText(MSG_PLACEFOLDERDLG_NO);
		m_ui->lineViewLongitude->setText(MSG_PLACEFOLDERDLG_NO);
		m_ui->lineViewRange->setText(MSG_PLACEFOLDERDLG_NO);
		m_ui->lineViewHeading->setText(MSG_PLACEFOLDERDLG_NO);
		m_ui->lineViewTilt->setText(MSG_PLACEFOLDERDLG_NO);
	}

	m_ui->lineViewLatitude->setEnabled(enable);
	m_ui->lineViewLongitude->setEnabled(enable);
	m_ui->lineViewRange->setEnabled(enable);
	m_ui->lineViewHeading->setEnabled(enable);
	m_ui->lineViewTilt->setEnabled(enable);
}

void PlaceMarkDlg::UpdateOutputInfo()
{
	QString str = QString::fromUtf16(m_placeMarkData.name);
	m_ui->lineName->setText(str);

	//by RSH 2013.7.30
	str = QString::fromUtf16(m_placeMarkData.iconname);
	m_ui->lineIconName->setText(str); //end edition

	//2014 2 6 by ugi
	str = QString::fromUtf16(m_placeMarkData.videopath);
	m_ui->lineVideoPath->setText(str); //end edition
	//end

	//2014 2 7 by ugi
	str = QString::fromUtf16(m_placeMarkData.htmlpath);
	m_ui->lineHtmlPath->setText(str); //end edition
	//end

	m_ui->horizontalSliderIconScale->setValue(m_placeMarkData.iconscale*10);//by ugi 2013.8.1

	str = QString::fromUtf16(m_placeMarkData.description);
	m_ui->textDescription->setPlainText(str);

	switch(g_app->GetConfig()->m_llShowMode)
	{
		case OPTION_LL_10NOTATION:
			str = QString("%1%2").arg(m_placeMarkData.latitude, 10, 'f', 6).arg(QChar(0x00B0));
			m_ui->lineLatitude->setText(str);
			str = QString("%1%2").arg(m_placeMarkData.longitude, 10, 'f', 6).arg(QChar(0x00B0));
			m_ui->lineLongitude->setText(str);
			str = QString("%1%2").arg(m_placeMarkData.view_latitude, 10, 'f', 6).arg(QChar(0x00B0));
			m_ui->lineViewLatitude->setText(str);
			str = QString("%1%2").arg(m_placeMarkData.view_longitude, 10, 'f', 6).arg(QChar(0x00B0));
			m_ui->lineViewLongitude->setText(str);
			break;
		case OPTION_LL_DMS:
			str = gu_Degree2DMS(m_placeMarkData.latitude, GEO_TYPE_LATITUDE);
			m_ui->lineLatitude->setText(str);
			str = gu_Degree2DMS(m_placeMarkData.longitude, GEO_TYPE_LONGITUDE);
			m_ui->lineLongitude->setText(str);
			str = gu_Degree2DMS(m_placeMarkData.view_latitude, GEO_TYPE_LATITUDE);
			m_ui->lineViewLatitude->setText(str);
			str = gu_Degree2DMS(m_placeMarkData.view_longitude, GEO_TYPE_LONGITUDE);
			m_ui->lineViewLongitude->setText(str);
			break;
		case OPTION_LL_10NOTATION_DM:
			str = gu_Degree2DM(m_placeMarkData.latitude, GEO_TYPE_LATITUDE);
			m_ui->lineLatitude->setText(str);
			str = gu_Degree2DM(m_placeMarkData.longitude, GEO_TYPE_LONGITUDE);
			m_ui->lineLongitude->setText(str);
			str = gu_Degree2DM(m_placeMarkData.view_latitude, GEO_TYPE_LATITUDE);
			m_ui->lineViewLatitude->setText(str);
			str = gu_Degree2DM(m_placeMarkData.view_longitude, GEO_TYPE_LONGITUDE);
			m_ui->lineViewLongitude->setText(str);
			break;
		default:
			break;
	}

	unsigned int range = (unsigned int) (m_placeMarkData.view_range + 0.5);
	str.sprintf("%dm", range);
	m_ui->lineViewRange->setText(str);

	double val;
	if (m_placeMarkData.view_heading > -DELTA && m_placeMarkData.view_heading < DELTA)
		val = 0.0;
	else
		val = -m_placeMarkData.view_heading;
	str = QString("%1%2").arg(val, 0, 'f', 6).arg(QChar(0x00B0));
	m_ui->lineViewHeading->setText(str);

	if (m_placeMarkData.view_tilt > -DELTA && m_placeMarkData.view_tilt < DELTA)
		val = 0.0;
	else
		val = m_placeMarkData.view_tilt;
	str = QString("%1%2").arg(val, 0, 'f', 6).arg(QChar(0x00B0));
	m_ui->lineViewTilt->setText(str);

	EnableViewControl(m_placeMarkData.snapshot);
}

void PlaceMarkDlg::SetFocusOkButton()
{
	m_ui->okButton->setFocus(Qt::OtherFocusReason);
}

void PlaceMarkDlg::SetMarkName()
{
	QString str = m_ui->lineName->text();
	memset(&m_placeMarkData.name, 0, sizeof(m_placeMarkData.name));
	gu_qstring2ushort(str, m_placeMarkData.name, PMD_STR_MAX_SIZE);

	m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, m_state);
	CopyDataToNode();
}

void PlaceMarkDlg::SetMarkLongitude()
{
	QString str = m_ui->lineLongitude->text();
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

	if ((res + 180 >= -DELTA && (res - 180) <= DELTA))
	{
		m_placeMarkData.longitude = res;
		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, m_state);
	}

	CopyDataToNode();
	UpdateOutputInfo();
}

void PlaceMarkDlg::SetMarkLatitude()
{
	QString str = m_ui->lineLatitude->text();
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
		m_placeMarkData.latitude = res;
		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, m_state);
	}

	CopyDataToNode();
	UpdateOutputInfo();
}

void PlaceMarkDlg::btnGo_click()
{
	GDM_CAMERA_INFO cameraInfo;	
	memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));

	cameraInfo.m_loc.m_dLongitude =  m_placeMarkData.longitude * gdm_DEGTORAD;
	cameraInfo.m_loc.m_dLatitude = m_placeMarkData.latitude * gdm_DEGTORAD;
	cameraInfo.m_loc.m_dDist = 15000;

	g_app->GetRenderProxy()->GetRenderSrv()->m_pCameraController->AutoMoveTo(0, cameraInfo, 4000);
}

void PlaceMarkDlg::SetMarkDescription()
{
	QString str = m_ui->textDescription->toPlainText();
	if (str.length() < PMD_STR_MAX_SIZE)
	{
		memset(&m_placeMarkData.description, 0, sizeof(m_placeMarkData.description));
		gu_qstring2ushort(str, m_placeMarkData.description, PMD_STR_MAX_SIZE);
	}
	else
	{
		m_ui->textDescription->setPlainText(m_ui->textDescription->toPlainText().left(
				m_ui->textDescription->toPlainText().length()-1));
		m_ui->textDescription->moveCursor(QTextCursor::End);
	}
}

void PlaceMarkDlg::SetViewLongitude()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewLongitude->text();
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

	UpdateOutputInfo();
}

void PlaceMarkDlg::SetViewLatitude()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewLatitude->text();
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

	UpdateOutputInfo();
}

void PlaceMarkDlg::SetViewRange()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewRange->text();
	QRegExp rx("^\\s*(\\d+(\\.\\d*)?)\\s*[\\m]$");

	if (rx.exactMatch(str))
	{
		if (rx.cap(1).toDouble() > 63710098)
			m_placeMarkData.view_range = 63710098;
		else
			m_placeMarkData.view_range = rx.cap(1).toDouble();

		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	UpdateOutputInfo();
}

void PlaceMarkDlg::SetViewHeading()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewHeading->text();
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

	UpdateOutputInfo();
}

void PlaceMarkDlg::SetViewTilt()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewTilt->text();
	QRegExp rx("^\\s*(\\d+(\\.\\d*)?)\\s*[\\x00b0]$");

	if (rx.exactMatch(str))
	{
		if (rx.cap(1).toDouble() > 90.00)
			m_placeMarkData.view_tilt = 90.00;
		else
			m_placeMarkData.view_tilt = rx.cap(1).toDouble();

		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	UpdateOutputInfo();
}

void PlaceMarkDlg::SetCenterView()
{
	m_viewcenter = !m_viewcenter;
	if (m_viewcenter)
		MoveCenterPlaceMark();
	if(m_pCommand && m_pCommand)
		m_pCommand->m_blCenter = m_viewcenter;
}

void PlaceMarkDlg::SnapshotView()
{
	m_placeMarkMgr->GetSnapshotPlaceData(&m_placeMarkData);
	UpdateOutputInfo();
}

void PlaceMarkDlg::ResetView()
{
	m_placeMarkMgr->GetResetPlaceData(&m_placeMarkData);
	UpdateOutputInfo();
}


void PlaceMarkDlg::MoveCenterPlaceMark()
{
	if(!m_viewcenter)
		return;

	m_placeMarkData.cx = g_app->GetMainWindow()->GetRenderFrame()->width() / 2;
	m_placeMarkData.cy = g_app->GetMainWindow()->GetRenderFrame()->height() / 2;

	RenderProxy *render = g_app->GetRenderProxy();
	render->SendMessage(PM_MOVE_PLACEMARK, (PrxParam)&m_placeMarkData);
	render->SendMessage(PM_SNAPSHOT_PLACEDATA, (PrxParam)&m_placeMarkData);
	CopyDataToNode();
}

//2014 2 7 by ugi

#define ICONNAME_SUBPATH "/res/icon"

void PlaceMarkDlg::IconChange()
{	
	QString fileName;
	PngBrowser b;
	fileName=b.getPngFileName(g_app->GetWorkDir()+ICONNAME_SUBPATH);

	if(fileName=="") return;

	memset(&m_placeMarkData.iconname, 0, sizeof(m_placeMarkData.iconname));
	gu_qstring2ushort(fileName, m_placeMarkData.iconname, PMD_STR_MAX_SIZE);

	m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, m_state);
	m_ui->lineIconName->setText(fileName);
	CopyDataToNode();
}

//by ugi 2013.8.1

void PlaceMarkDlg::IconScaleChange(int val)
{
	m_placeMarkData.iconscale=val/10.0;
	CopyDataToNode();
}


#define VIDEO_PATH "/video/"
#define HTML_PATH  "/html/"
#include "../gds_common/GDSConfigMgr.h"

//2014 2 6 by ugi
void PlaceMarkDlg::VideoPathChange()
{
	QString openDir = g_ServerConfig.GetFileDBPath() + VIDEO_PATH;

	QString fileName;

#ifndef APP_USE_CUSTOM_DIALOG
	fileName = QFileDialog::getOpenFileName(this, tr("Select Video File"),
		openDir, tr("Video Files (*.avi *.mpeg *.mpg)"));
#else
	fileName = FileDialog::GetOpenFileName(this, tr("Select Video File"),
		openDir, tr("Video Files (*.avi *.mpeg *.mpg)"));
#endif

	if (fileName.isEmpty())
		return;

	int subpathInx = fileName.lastIndexOf(VIDEO_PATH);

	QString path(VIDEO_PATH);

	if (subpathInx != -1)
	{
		subpathInx += path.length();
	}
	else
	{
		subpathInx = fileName.lastIndexOf("/") + 1;
		QString filestr = fileName.mid(subpathInx);
		openDir = openDir + filestr;
		if (!QFile::copy(fileName, openDir))
		{
			gu_MessageBox(this, tr("File Copy Error!"), 
				tr("Cannot copy icon file to resource file! \n Same file name already exists or source file not found!"));
			return;
		}
	}

	fileName = fileName.mid(subpathInx);
	fileName = VIDEO_PATH + fileName;

	memset(&m_placeMarkData.videopath, 0, sizeof(m_placeMarkData.videopath));
	gu_qstring2ushort(fileName, m_placeMarkData.videopath, PMD_STR_MAX_SIZE);

	m_ui->lineVideoPath->setText(fileName);
	CopyDataToNode();
}

//2014 2 7 by ugi
void PlaceMarkDlg::HtmlPathChange()
{
	QString openDir = g_ServerConfig.GetFileDBPath() + HTML_PATH;

	QString fileName;

#ifndef APP_USE_CUSTOM_DIALOG
	fileName = QFileDialog::getOpenFileName(this, tr("Select Html File"),
		openDir, tr("Html Files (*.html *.htm)"));
#else
	fileName = FileDialog::GetOpenFileName(this, tr("Select Html File"),
		openDir, tr("Html Files (*.html *.htm)"));
#endif

	if (fileName.isEmpty())
		return;

	QFileInfo fileinfo(fileName);


	int subpathInx = fileName.lastIndexOf(HTML_PATH);

	QString path(HTML_PATH);

	if (subpathInx != -1)
	{
		subpathInx += path.length();
	}
	else
	{
		

		QString filter=fileinfo.baseName()+".*";

		QString d=fileinfo.path();

		QDir dir(fileinfo.path(),filter);

		QStringList imagelist=dir.entryList();

		for(int i=0;i<imagelist.size();i++)
		{
			QFile::copy(fileinfo.path()+"/"+imagelist[i],
				        openDir+"/"+imagelist[i]);
		}
		
		QString opendDir=fileinfo.path();

		//copy addtional file
		QStringList fList;
		if(!dir.exists(openDir + "/img")) 
		{
			dir.mkdir(openDir + "/img");
			dir.setPath(opendDir + "/img");
			fList = dir.entryList();
			fList.removeOne("."); 
			fList.removeOne(".."); 
			fList.removeOne("Thumbs.db");
			if(fList.count() > 1) 
			{
				for(int i=0; i<fList.count(); i++)
					QFile::copy(opendDir + "/img/" + fList.at(i), openDir + "/img/" + fList.at(i));
			}
		}
	}

	fileName = HTML_PATH + fileinfo.fileName();

	memset(&m_placeMarkData.htmlpath, 0, sizeof(m_placeMarkData.htmlpath));
	gu_qstring2ushort(fileName, m_placeMarkData.htmlpath, PMD_STR_MAX_SIZE);

	m_ui->lineHtmlPath->setText(fileName);
	CopyDataToNode();
}

void PlaceMarkDlg::SetIconPath()
{
	QString str = m_ui->lineIconName->text();
	memset(&m_placeMarkData.iconname, 0, sizeof(m_placeMarkData.iconname));
	gu_qstring2ushort(str, m_placeMarkData.iconname, PMD_STR_MAX_SIZE);

	CopyDataToNode();
}

void PlaceMarkDlg::SetHtmlPath()
{
	QString str = m_ui->lineHtmlPath->text();
	memset(&m_placeMarkData.htmlpath, 0, sizeof(m_placeMarkData.htmlpath));
	gu_qstring2ushort(str, m_placeMarkData.htmlpath, PMD_STR_MAX_SIZE);

	CopyDataToNode();
}

void PlaceMarkDlg::SetVideoPath()
{
	QString str = m_ui->lineVideoPath->text();
	memset(&m_placeMarkData.videopath, 0, sizeof(m_placeMarkData.videopath));
	gu_qstring2ushort(str, m_placeMarkData.videopath, PMD_STR_MAX_SIZE);

	CopyDataToNode();
}