#include <QtGui>
#include "gdem_protocol.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "sidebar.h"
#include "resulttreeview.h"
#include "searchdlg.h"
#include "searchthread.h"
#include "ui_searchdlg.h"
#include "proxy.h"
#include "treeitem.h"
#include "guiuty.h"
#include "msgdata.h"
#include "config.h"
#include "../gdem_common/gdem_config.h"

#include "GDM_MeasureAreaCmd.h"
#include "GDM_CommandMgr.h"
#include "GIS_Doc.h"
#include "../gds_database/GDSDatabaseInterface.h"
#include <QSqlQuery>
#include <QSqlRecord>


enum SEARCH_RESULT
{
	SR_SUCCESS		= 0,
	SR_NODATA		= -1, 
	SR_NETWORKERROR	= -2,
	SR_UNKNOWN		= -3
};

#define NORMAL_HEIGHT	400
#define MEMORY_ERROR	100

#define PROXY
#define USE_SEARCHTHREAD

static void MakeViewRange(PLACEMARK_DATA *pmd);

SearchDlg::SearchDlg(QWidget *parent)
	: CGDM_CommandDlg(parent),
    m_ui(new Ui::SearchDlg)
{
    m_ui->setupUi(this);
    m_parent = (SideBar*)parent;
	m_deleteHistory = false;
	m_pCommand=NULL;
	m_searchThread=NULL;
	m_searchResultView=NULL;
	m_frame=NULL;

	connect(m_ui->searchCombo->lineEdit(), SIGNAL(returnPressed()), this, SLOT(SearchCommand()));
	connect(m_ui->searchCombo->lineEdit(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowSearchContextMenu(QPoint)));
	connect(m_ui->searchButton, SIGNAL(clicked()), this, SLOT(SearchClick()));

	connect(m_ui->cmbSearchStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbSearchStyle_change(int)));
 }

SearchDlg::~SearchDlg()
{
	if(m_pCommand)
	{
		m_pCommand->OnCancel();
		m_pCommand = NULL;
	}
	delete m_ui;
}

void SearchDlg::Search(QString key ,int start, int itemkey)
{
	if (key.isEmpty())
		return;

#ifdef USE_SEARCHTHREAD
	if (m_searchThread)
	{
		if (m_searchThread->GetState() != STS_IDLE)
			return;

		m_searchThread->SetState(STS_NEW);
		m_searchThread->SetKey(key, start, itemkey);
	}
#else
	int res =  RequestData(key, start);
	if (res == SR_NODATA)
		gu_MessageBox(this, MSG_ERR_SEARCH, MSG_SIDEBAR_SEARCH, MSGBOX_ICONWARNING);
	else if (res == SR_NETWORKERROR)
		gu_MessageBox(this, 	tr("Can't Search"), tr("Search Link Error"), MSGBOX_ICONINFO);
#endif
}

bool SearchDlg::IsActiveView()
{
	if (m_searchResultView->GetSelectItem() && m_searchResultView->hasFocus())
		return true;

	return false;
}

void SearchDlg::SetStatus(int status)
{
    m_status = status;

    if (m_status == BS_Close)
    {
        m_visible = false;
        m_frame->hide();
		setContentsMargins(0,0,0,0);
    }
    else if (m_status != BS_Animation)
    {
        m_visible = true;
        m_frame->show();
		setContentsMargins(0,0,0,BOTTOM_MARGIN);
    }
	UpdateToolTip();
}

int SearchDlg::GetHeight(int sender)
{
	int height = m_oldsize;
    if (!m_visible)
    {
		//height = m_leftTitle->height();
		m_oldsize = this->size().height();
    }
	else if (sender != ST_Search)
		 height = this->size().height();

    return height;
}
void SearchDlg::SendDeleteHistoryMsg()
{
	if (m_searchThread == NULL)
		return;

	m_deleteHistory = true;
	if (m_searchThread->GetState() == STS_IDLE)
	{
		m_searchThread->RemoveAllSearchNode();
		m_deleteHistory = false;
	}
}

void SearchDlg::SendTimeMessage()
{
	if (m_searchThread == NULL)
		return;
	if (m_searchThread->GetState() != STS_RECEIVED)
		return;
	m_searchThread->SetState(STS_ANALYZE);

	int res;
	ST_PACKET_HEADER *response;
	m_searchThread->GetResponseData(&res, (void**)&response);

	QString key = m_searchThread->GetKeyStr();
	int itemkey = m_searchThread->GetItemKey();

	res = AnalyzeResponseData(key, itemkey, res, response);

	if (m_deleteHistory)
	{
		m_searchThread->RemoveAllSearchNode();
		m_deleteHistory = false;
	}

	if (res > 0)
		m_searchThread->AddSearchNode(res);
	else if (res == SR_NODATA)
		gu_MessageBox(this, tr("Search Error"), tr("Search Empty"), MSGBOX_ICONWARNING);
	else if (res == SR_NETWORKERROR)
#if defined(STANDARD_ALONE)
		gu_MessageBox(this, tr("Search Error"), tr("Search DB Error"), MSGBOX_ICONWARNING);
#else
		gu_MessageBox(this, 	tr("Can't Search"), tr("Search Link Error"), MSGBOX_ICONINFO);
#endif

	m_searchThread->SetState(STS_IDLE);
	return;
}

void SearchDlg::WaiteStop()
{
	if (m_searchThread)
		m_searchThread->WaiteStop();
}

void SearchDlg::UpdateToolTip()
{
	if (g_app->GetConfig()->m_toolTipChecked)
	{
		m_ui->searchCombo->setToolTip(tr("Search Item"));
		m_ui->searchButton->setToolTip(tr("Search Start"));
	}
	else
	{
		m_ui->searchCombo->setToolTip(MSG_NO_TOOLTIP);
		m_ui->searchButton->setToolTip(MSG_NO_TOOLTIP);
	}
}

/*
 *   protected
 */
void SearchDlg::keyPressEvent ( QKeyEvent * event )
{
	if (event->key() == Qt::Key_Escape)
		event->ignore();
	else
		event->accept();
}

void SearchDlg::timerEvent(QTimerEvent *event)
{
	SendTimeMessage();
}

bool SearchDlg::Init()
{
	m_visible = true;

	m_searchThread = new SearchThread;
	if (m_searchThread != NULL)
		m_searchThread->start();
	else
		return false;

	if (!CreateFrame())
		return false;

	m_ui->cmbSearchStyle->blockSignals(true);
	m_ui->cmbSearchStyle->addItem(tr("Common"), QVariant());
	//m_ui->cmbSearchStyle->addItem(tr("Around"), QVariant());
	m_ui->cmbSearchStyle->addItem(tr("Region"), QVariant());
	m_ui->cmbSearchStyle->blockSignals(false);

	m_ui->fraAround->setVisible(false);
	
    SetStatus(BS_Open);
	m_oldsize = this->size().height();
	m_timer.start(1, this);

	if(g_DBManager)
	{
		QSqlQuery qry;
		QString strSql;
		strSql = "SELECT table_name, shape_type FROM tb_type WHERE type >= " + QString("%1").arg(OBJECT_TYPE_NO) + " ORDER BY type";
		if(!qry.exec(strSql))
		{
			gu_MessageBox(this, tr("Failed"), tr("Can not execute SQL query!"), MSGBOX_ICONWARNING);
			return false;
		}
		while (qry.next())
			m_tableNames[qry.value(0).toString()] = qry.value(1).toInt();
	}

    return true;
}

void SearchDlg::cmbSearchStyle_change(int style)
{
	switch(style) 
	{
	case 0:
		m_ui->fraCommon->setVisible(true);
		m_ui->fraAround->setVisible(false);
		if(m_pCommand)
		{
			m_pCommand->OnCancel();
			m_pCommand = NULL;
		}
		break;
	//case 1:
	//	m_ui->fraCommon->setVisible(false);
	//	m_ui->fraAround->setVisible(true);
	//	break;
	case 1:
		if (g_app->GetMainWindow()->BeEditingDlg())
		{
			gu_MessageBox(this, tr("Info"), tr("You must finish with your current item before closing window!"), MSGBOX_ICONINFO);
			m_ui->cmbSearchStyle->setCurrentIndex(0);
			return;
		}

		m_ui->fraCommon->setVisible(false);
		m_ui->fraAround->setVisible(false);

		if(m_pCommand)
		{
			m_pCommand->OnCancel();
			m_pCommand = NULL;
		}

		CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
		m_pCommand = (CGDM_MeasureAreaCmd*)pCmdMgr->CreateCmd(eGDM_CMD_MEASUREAREA, this);
		break;
	}
}

void SearchDlg::Release()
{
	if (m_searchThread)
		delete m_searchThread;

	m_searchThread = NULL;

	DestroyFrame();
}

bool SearchDlg::CreateFrame()
{
	QString path = g_app->GetResource("/sidebar/");

	QVBoxLayout *VBoxLayout = new QVBoxLayout(this);
	if (!VBoxLayout)
		return false;
	VBoxLayout->setContentsMargins(0,0,0,0);

	// Search Style
	QHBoxLayout *HBoxStyle = new QHBoxLayout(this);
	if (!HBoxStyle) return false;
	HBoxStyle->setContentsMargins(7,0,5,0);
	HBoxStyle->setSpacing(12);
	HBoxStyle->addWidget(m_ui->lblSearchStyle);
	HBoxStyle->addWidget(m_ui->cmbSearchStyle);
	QSpacerItem *spaSearchStyle = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	if (!spaSearchStyle) return false;
	HBoxStyle->addItem(spaSearchStyle);
	QString filename = path + "search.png";
	m_ui->searchButton->setIcon(QIcon(filename));
	m_ui->searchButton->setText(tr("Search"));
	HBoxStyle->addWidget(m_ui->searchButton);
	m_ui->fraStyle->setLayout(HBoxStyle);
	VBoxLayout->addWidget(m_ui->fraStyle);

	// Search Common
	QHBoxLayout *HBoxCommon = new QHBoxLayout(this);
	if (!HBoxCommon) return false;
	HBoxCommon->setContentsMargins(7,0,5,0);
	HBoxCommon->setSpacing(6);
	HBoxCommon->addWidget(m_ui->lblKeyWord);
	m_ui->searchCombo->setInsertPolicy(QComboBox::InsertAtTop);
	m_ui->searchCombo->setDuplicatesEnabled(false);
	m_ui->searchCombo->setAutoCompletion(false);
	m_ui->searchCombo->setModel(g_app->GetSearchModel());
	m_ui->searchCombo->lineEdit()->setContextMenuPolicy(Qt::CustomContextMenu);
	HBoxCommon->addWidget(m_ui->searchCombo);
	m_ui->fraCommon->setLayout(HBoxCommon);
	VBoxLayout->addWidget(m_ui->fraCommon);

	// Search Around
	QVBoxLayout *VBoxAround = new QVBoxLayout(this);
	if (!VBoxAround) return false;
	VBoxAround->setContentsMargins(0,0,0,0);
	VBoxAround->setSpacing(6);

	QHBoxLayout *HBoxAround_1 = new QHBoxLayout(this);
	if (!HBoxAround_1) return false;
	HBoxAround_1->setContentsMargins(7,0,5,0);
	HBoxAround_1->setSpacing(6);
	HBoxAround_1->addWidget(m_ui->lblDistance);
	HBoxAround_1->addWidget(m_ui->edtDistance);
	HBoxAround_1->addWidget(m_ui->cmbMetrics);
	VBoxAround->addItem(HBoxAround_1);

	QHBoxLayout *HBoxAround_2 = new QHBoxLayout(this);
	if (!HBoxAround_2) return false;
	HBoxAround_2->setContentsMargins(7,0,5,0);
	HBoxAround_2->setSpacing(6);
	HBoxAround_2->addWidget(m_ui->lblCenteral);
	HBoxAround_2->addWidget(m_ui->lblLat);
	HBoxAround_2->addWidget(m_ui->edtLat);
	HBoxAround_2->addWidget(m_ui->lblLong);
	HBoxAround_2->addWidget(m_ui->edtLong);
	VBoxAround->addItem(HBoxAround_2);

	m_ui->fraAround->setLayout(VBoxAround);
	VBoxLayout->addWidget(m_ui->fraAround);

	// RestultTreeView, Clear Button Layout
	QHBoxLayout *HBoxResult = new QHBoxLayout(this);
	if (!HBoxResult) return false;
	HBoxResult->setContentsMargins(5,0,5,0);
	m_searchResultView = new ResultTreeView;
	if (!m_searchResultView) return false;
	HBoxResult->addWidget(m_searchResultView);
	VBoxLayout->addItem(HBoxResult);

	m_searchResultView->SetSideBar(m_parent);

	m_frame = new QFrame(this);
	if (!m_frame)
		return false;
	m_frame->setContentsMargins(0,0,0,0);
	m_frame->setLineWidth(1);
	m_frame->setLayout(VBoxLayout);

    // Main Layout
    QVBoxLayout *searchLayout = new QVBoxLayout(this);
	if (!searchLayout)
		return false;
	searchLayout->addWidget(m_frame);
	searchLayout->setContentsMargins(0,5,0,BOTTOM_MARGIN);
    setLayout(searchLayout);
	
	return true;
}

void SearchDlg::DestroyFrame()
{
	if (m_searchResultView)
	{
		delete m_searchResultView;
		m_searchResultView = NULL;
	}

	if (m_frame)
	{
		delete m_frame;
		m_frame = NULL;
	}
}

#if 0
int SearchDlg::RequestData(QString key, int start)
{
#ifdef PROXY
	// request buffer
	int totalsize = sizeof(ST_PACKET_HEADER) - sizeof(unsigned char) * 4
					+ sizeof(ST_REQUEST_HEADER) - sizeof(unsigned char) * 4
					+ sizeof(ST_REQUEST_GEONAMESEARCH) - sizeof(unsigned short) * 2
					+ sizeof(unsigned short) * (key.count() + 1);
	ST_PACKET_HEADER *request =(ST_PACKET_HEADER * )malloc(totalsize);
	if (!request)
		return MEMORY_ERROR;

	memset(request,0x00, totalsize);
	request->bytes = totalsize;
	ST_REQUEST_HEADER *header = (ST_REQUEST_HEADER*)request->buffer;
	header->type = E_REQUEST_TYPE_GEONAMELIST;
	ST_REQUEST_GEONAMESEARCH *data = (ST_REQUEST_GEONAMESEARCH*)header->body;

	data->level = 0;
	data->left = -180;
	data->top = 180;
	data->right = 180;
	data->bottom = -180;
	data->start_index = start;
	data->end_index = start + TI_MAX_SEARCH_COUNT -1;
	data->length = (key.count() + 1) * sizeof(unsigned short) ;
	wchar_t tempkey[254];
	key.toWCharArray(tempkey);
	tempkey[key.count()] = 0;

	//memcpy((uchar*)data->key_string, (uchar*)tempkey, data->length);
	gu_wchar2ushort(tempkey, data->key_string, key.count());

	// response buffer
	ST_PACKET_HEADER *response = (ST_PACKET_HEADER *)malloc(PACKET_MAX_BUFFER_SIZE);
	if (!response)
		return MEMORY_ERROR;
	response->bytes = PACKET_MAX_BUFFER_SIZE;

	DataProxy *datasrv = g_app->GetDataProxy();
	int res = MEMORY_ERROR;
	if (datasrv)
		res = datasrv->SendMessage(PM_GET_SEARCHRESULT, (PrxParam)request, (PrxParam)response);

	free(request);

	if (res)
	{
		free(response);
		return SR_NODATA;
	}

	ST_RESPONSE_HEADER *responseHeader = (ST_RESPONSE_HEADER *)response->buffer;
	if (responseHeader->result != E_RESPONSE_ERROR_NONE)
	{
		free(response);
		return SR_NODATA;

	}
	ST_RESPONSE_GEONAMELIST *responseData = (ST_RESPONSE_GEONAMELIST *)responseHeader->body;

	int data_count = responseData->total_count;
	if (data_count == 0)
	{
		free(response);
		return SR_NODATA;
	}

	PLACEMARK_DATA_LIST *list = new PLACEMARK_DATA_LIST;
	memset(list, 0, sizeof(PLACEMARK_DATA_LIST));

	list->total_count = responseData->total_count;
	list->start_index = responseData->start_index;
	list->end_index = responseData->end_index;

	char * pBuff = (char *)responseData->data_list;

	for (int i = 0; i < (int)(list->end_index-list->start_index+1); i++)
	{
		ST_GEODATA_NAME* pRecord = (ST_GEODATA_NAME*)pBuff;
		list->data_list[i].latitude = pRecord->latitude;
		list->data_list[i].longitude = pRecord->longitude;
		list->data_list[i].type = pRecord->type;
		unsigned int len = pRecord->length_ko;
		memcpy(list->data_list[i].name, (char*)pRecord->name + pRecord->length, len);
//		gu_ushort2wchar(pRecord->name, list->data_list[i].name, 0);

		QString dbstr = QString("SearchDlg::RequestData:") + QString::fromUtf16(list->data_list[i].name);
		qDebug() << dbstr;

		list->data_list[i].altitude = 0;
		list->data_list[i].description[0] = 0;
		list->data_list[i].itemtype = IT_SEARCH;
		list->data_list[i].view_longitude = pRecord->longitude;
		list->data_list[i].view_latitude = pRecord->latitude;
		list->data_list[i].view_range = 0;
		list->data_list[i].view_heading = 0;
		list->data_list[i].view_tilt = 0;
		list->data_list[i].showmark = SMT_SHOW;

		pBuff += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pRecord->length + pRecord->length_ko;
	}

	free(response);

	m_searchResultView->InsertTable(list, key);
	//if (this->height() < NORMAL_HEIGHT)
	//	m_parent->ResizeForSearch(NORMAL_HEIGHT);

	delete list;

	return SR_SUCCESS;
#endif

#ifdef TEST
	PLACEMARK_DATA_LIST *data = new PLACEMARK_DATA_LIST;

	// requst to datasrv
	// ok
	// test code by kimsh
	data->total_count = 100;
	data->start_index = 41;
	data->end_index = 50;

	for (int i = 41; i<51; i++)
	{
		data->data_list[i-41].latitude = i;
		data->data_list[i-41].longitude = i;
		data->data_list[i-41].type = 2;
		wcscpy(data->data_list[i-41].name, L"this is name");
		wcscpy(data->data_list[i-41].description, L"this is description my name is google. i go to home. i am a korean. that is a good. i go to school");
		data->data_list[i-41].itemtype = IT_PLACEMARK;
	}

	//false
	//return false;
	data->start_index = 41;
	data->end_index = 50;

	m_searchResultView->InsertTable(data, key);

	return 0;
#endif
}
#endif

int SearchDlg::AnalyzeResponseData(QString &key, int parentkey, int res, void *resData)
{
	unsigned int len;
	QString str;
	int itemkey;

	switch (res)
	{
		case SEARCH_THREAD_ERROR_MEMORY_FAIL:
		case SEARCH_THREAD_ERROR_SERVER:
			return SR_NETWORKERROR;
		case SEARCH_THREAD_ERROR_NODATA:
			return SR_NODATA;
	}

	SEARCH_INDEX_INFO_HEADER *info_header = (SEARCH_INDEX_INFO_HEADER*)resData;

	ST_PACKET_HEADER *response = (ST_PACKET_HEADER *)((char*)resData + sizeof(SEARCH_INDEX_INFO_HEADER));
	ST_RESPONSE_HEADER *responseHeader = (ST_RESPONSE_HEADER *)response->buffer;
	if (responseHeader->result != E_RESPONSE_ERROR_NONE)
		return SR_NETWORKERROR;

	ST_RESPONSE_GEONAMESEARCH *search_header = (ST_RESPONSE_GEONAMESEARCH*)responseHeader->body;
	if (search_header->type != E_SEARCH_TYPE_DATA)
		return SR_NETWORKERROR;

	ST_GEODATA_NAMELIST *responseData = (ST_GEODATA_NAMELIST *)search_header->response;

	int data_count = info_header->total_count;
	if (data_count == 0)
		return SR_NODATA;

	PLACEMARK_DATA_LIST *list = new PLACEMARK_DATA_LIST;
	memset(list, 0, sizeof(PLACEMARK_DATA_LIST));

	list->total_count = info_header->total_count;
	list->start_index = info_header->start_index;
	list->end_index	  = info_header->start_index + responseData->count - 1;

	char * pBuff = (char *)responseData->data_list;

	for (int i = 0; i < responseData->count; i++)
	{
		ST_GEODATA_NAME* pData = (ST_GEODATA_NAME*)pBuff;
		list->data_list[i].db_index	 = pData->id;
		list->data_list[i].latitude	 = pData->latitude;
		list->data_list[i].longitude = pData->longitude;
		list->data_list[i].type		 = pData->type;

		len = pData->length;
		if (pData->length)	/* National Name */
		{
			len = pData->length / sizeof(ushort);
			str = QString::fromUtf16((ushort*)pData->name, len);
			if (str.contains(key, Qt::CaseInsensitive))
			{
				memcpy(list->data_list[i].name, (char*)pData->name, pData->length);
				memcpy(list->data_list[i].other_name, (char*)pData->name + pData->length, pData->length_ko);
			}
		}
		if (list->data_list[i].name[0] == 0)
		{
			len = pData->length_ko;
			memcpy(list->data_list[i].name, (char*)pData->name + pData->length, len);
			memcpy(list->data_list[i].other_name, (char*)pData->name, pData->length);
		}

#ifdef DEBUG
		QString dbstr = QString("SearchDlg::RequestData:") + QString::fromUtf16(list->data_list[i].name);
		qDebug() << dbstr;
#endif

		list->data_list[i].altitude = pData->height;
		list->data_list[i].description[0] = 0;
		list->data_list[i].itemtype = IT_SEARCH;
		list->data_list[i].view_longitude = pData->longitude;
		list->data_list[i].view_latitude = pData->latitude;
		list->data_list[i].view_range = pData->view_height;
		list->data_list[i].view_heading = 0;
		list->data_list[i].view_tilt = 0;
		list->data_list[i].showmark = SMT_SHOW;
		list->data_list[i].snapshot = true;

		MakeViewRange(&list->data_list[i]);

		pBuff += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pData->length + pData->length_ko;
	}

	if (parentkey)
		m_searchResultView->ReplaceTable(list, key, parentkey);
	else
		itemkey = m_searchResultView->InsertTable(list, key);

	delete list;

	if (parentkey)
		return SR_SUCCESS;
	return itemkey;
}

/*
 *   protected slot
 */
void SearchDlg::SearchCommand()
{
	QString str = m_ui->searchCombo->lineEdit()->text();
	if (str.length() < 30)
		Search(str);
	else
		gu_MessageBox(this, tr("Search Error"), tr("Search Empty"), MSGBOX_ICONWARNING);
}

void SearchDlg::ShowSearchContextMenu(QPoint pos)
{
	MainWindow *mainWnd = g_app->GetMainWindow();
	mainWnd->ReleaseShortcutKey(true);

	QPoint globalPos = m_ui->searchCombo->lineEdit()->mapToGlobal(pos);
	QMenu *popup = m_ui->searchCombo->lineEdit()->createStandardContextMenu();
	popup->exec(globalPos);

	mainWnd->ReleaseShortcutKey(false);
	delete popup;
}

void SearchDlg::SearchClick()
{
	QString str;
	int style = m_ui->cmbSearchStyle->currentIndex();
	switch(style) {
	case 0:
		str = m_ui->searchCombo->lineEdit()->text();
		if (m_ui->searchCombo->findText(str) < 0)
			m_ui->searchCombo->insertItem(0, str);

		if (str.length() < 30)
			Search(str);
		else
			gu_MessageBox(this, tr("Search Error"), tr("Search Empty"), MSGBOX_ICONWARNING);
		break;
		//case 1:
		//	break;
	case 1:
		if(!m_pCommand) return;
		if(m_pCommand->GetNode()->Get_TrackCount() < 3) {
			gu_MessageBox(this, tr("Wrong"), tr("Draw Area Correctly !"), MSGBOX_ICONWARNING);
			return;
		}
		searchRangeData();
		break;
	}
}

int SearchDlg::ChangeState(bool press)
{
   bool pressed = press;
   if (m_status == BS_Disable)
       return 0;

   if (pressed == true)
   {
       SetStatus(BS_Animation);
   }
   else
   {
       if (m_visible)
       {
           SetStatus(BS_Close);
		   m_parent->ChangePanel();
       }
       else
       {
           SetStatus(BS_Open);
		   m_parent->ChangePanel();
       }
   }

   return 1;
}

void SearchDlg::ClearList(bool press)
{
	QString path = g_app->GetResource("/sidebar/");

	if (press)
	{
		
	}
	else
	{
		m_searchResultView->ClearTable();
		SendDeleteHistoryMsg();
	}
}

static void MakeViewRange(PLACEMARK_DATA *pmd)
{
	if (!pmd)
		return;

	double dist = 3000000;
	if(pmd->view_range == 0)
	{
		switch(pmd->type)
		{
		case E_GDM_SUBDATA_TYPE_NAME_COUNTRY:
			dist = 1500000;
			break;
		case E_GDM_SUBDATA_TYPE_NAME_POPULAR:
			dist = 15000;
			break;
		case E_GDM_SUBDATA_TYPE_NAME_FIRSTADMIN:
			dist = 150000;
			break;
		case E_GDM_SUBDATA_TYPE_NAME_SECONDADMIN:
			dist = 2000;
			break;
		case E_GDM_SUBDATA_TYPE_NAME_THIRDADMIN:
			dist = 1000;
			break;
		case E_GDM_SUBDATA_TYPE_NAME_SEA:
			dist = 15000;
			break;
		case E_GDM_SUBDATA_TYPE_NAME_MOUNTAIN:
			dist = 1000;
			break;
		case E_GDM_SUBDATA_TYPE_NAME_RIVER:
			dist = 5000;
			break;
		default:
			dist=1000;
		}
		pmd->view_range = dist;
	}
}

QList<double> SearchDlg::getMBR()
{
	QList<double> retVal;
	CGIS_Node *pathNode = (CGIS_Node *)m_pCommand->GetNode();
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

void SearchDlg::searchRangeData()
{
	QDate tDate = QDate::currentDate();
	QTime tTime = QTime::currentTime();

	QString key;
	key.sprintf("%04d/%02d/%02d %02d:%02d ",
		tDate.year(), 
		tDate.month(), 
		tDate.day(),
		tTime.hour(), 
		tTime.minute());

	QList<double> mbrList = getMBR();

	QVector<QPointF> ptFs;
	QPointF ptF;
	CGDM_Location3D locPt;
	for(int ii = 0; ii < m_pCommand->GetNode()->Get_TrackCount(); ii++) {
		m_pCommand->GetNode()->Get_TrackPt(ii, &locPt);
		ptF.setX((qreal)locPt.m_tY);
		ptF.setY((qreal)locPt.m_tX);
		ptFs << ptF;
	}
	QPolygonF searchRegion(ptFs);
	
	//double toplat, toplong, botlat, botlong;
	//if(!GetRecLongLat(m_dlat, m_dlong, m_dRad, toplat, toplong, botlat, botlong))
	//	return;

	PLACEMARK_DATA_LIST *list = new PLACEMARK_DATA_LIST;
	memset(list, 0, sizeof(PLACEMARK_DATA_LIST));
	list->start_index = 0;
	list->end_index	  = 0;
	list->total_count = 0;
	int id = 0;

	QSqlQuery qry;
	QString strSql;
	QString tmpstr;
	wchar_t tempkey[254];

	QMapIterator<QString, int> iter(m_tableNames);
	while (iter.hasNext()) {
		iter.next();
		//cout << ii.key() << ": " << ii.value() << endl;

		QString mbr_minLat, mbr_maxLat, mbr_minLong, mbr_maxLong;
		if(iter.value() < OBJECT_LINE) {
			mbr_minLat = QString::number(radian2degree(mbrList[1]), 'g', 8); 
			mbr_maxLat = QString::number(radian2degree(mbrList[3]), 'g', 8); 
			mbr_minLong = QString::number(radian2degree(mbrList[0]), 'g', 9); 
			mbr_maxLong = QString::number(radian2degree(mbrList[2]), 'g', 9); 

			strSql = "SELECT name_ko, name, latitude, longitude, type FROM " + iter.key();
			strSql += " WHERE latitude >= " + mbr_minLat + " and latitude <= " + mbr_maxLat;
			strSql += " and longitude >= " + mbr_minLong + " and longitude <= " + mbr_maxLong;
		} else {
			mbr_minLat = QString::number(mbrList[1], 'g', 10);
			mbr_maxLat = QString::number(mbrList[3], 'g', 10);
			mbr_minLong = QString::number(mbrList[0], 'g', 10);
			mbr_maxLong = QString::number(mbrList[2], 'g', 10);

			strSql = "SELECT name_ko, name, latitude, longitude, type, points,";
			strSql += " minlatitude, minlongitude, maxlatitude, maxlongitude FROM " + iter.key();

			strSql += " WHERE (minlatitude >= " + mbr_minLat + " and minlatitude <= " + mbr_maxLat;
			strSql += " and minlongitude >= " + mbr_minLong + " and minlongitude <= " + mbr_maxLong;

			strSql += ") or (maxlatitude >= " + mbr_minLat + " and maxlatitude <= " + mbr_maxLat;
			strSql += " and maxlongitude >= " + mbr_minLong + " and maxlongitude <= " + mbr_maxLong;

			strSql += ") or (maxlatitude >= " + mbr_minLat + " and minlatitude <= " + mbr_minLat;
			strSql += " and maxlongitude >= " + mbr_minLong + " and minlongitude <= " + mbr_minLong;

			strSql += ") or (maxlatitude >= " + mbr_maxLat + " and minlatitude <= " + mbr_maxLat;
			strSql += " and maxlongitude >= " + mbr_maxLong + " and minlongitude <= " + mbr_maxLong;

			strSql += ") or (minlatitude >= " + mbr_minLat + " and minlatitude <= " + mbr_maxLat;
			strSql += " and maxlongitude >= " + mbr_minLong + " and maxlongitude <= " + mbr_maxLong;

			strSql += ") or (maxlatitude >= " + mbr_minLat + " and maxlatitude <= " + mbr_maxLat;
			strSql += " and minlongitude >= " + mbr_minLong + " and minlongitude <= " + mbr_maxLong;

			strSql += ") or (maxlatitude >= " + mbr_minLat + " and minlatitude <= " + mbr_minLat;
			strSql += " and maxlongitude >= " + mbr_maxLong + " and minlongitude <= " + mbr_maxLong;

			strSql += ") or (maxlatitude >= " + mbr_maxLat + " and minlatitude <= " + mbr_maxLat;
			strSql += " and maxlongitude >= " + mbr_minLong + " and minlongitude <= " + mbr_minLong;

			strSql += ") or (minlatitude >= " + mbr_minLat + " and minlatitude <= " + mbr_maxLat;
			strSql += " and maxlatitude >= " + mbr_minLat + " and maxlongitude <= " + mbr_maxLat;
			strSql += " and maxlongitude >= " + mbr_minLong + " and minlongitude <= " + mbr_minLong;
			strSql += " and maxlongitude >= " + mbr_maxLong + " and minlongitude <= " + mbr_maxLong;

			strSql += ") or (minlatitude >= " + mbr_minLat + " and maxlatitude <= " + mbr_minLat;
			strSql += " and minlatitude >= " + mbr_maxLat + " and maxlongitude <= " + mbr_maxLat;
			strSql += " and minlongitude >= " + mbr_minLong + " and minlongitude <= " + mbr_maxLong;
			strSql += " and maxlongitude >= " + mbr_minLong + " and maxlongitude <= " + mbr_maxLong + ")";
		}
		qry.exec(strSql);
		while (qry.next()) {
			double placeLat = qry.record().value("latitude").toDouble();
			double placeLong = qry.record().value("longitude").toDouble();
			double radianLat, radianLong;
			if(iter.value() < OBJECT_LINE) {
				radianLat= degree2radian(placeLat);
				radianLong = degree2radian(placeLong);
				QPointF dbPtF((qreal)radianLat, (qreal)radianLong);
				if(!searchRegion.containsPoint(dbPtF, Qt::WindingFill))
					continue;
			} else {
				ptFs.clear();
				QByteArray ptArr = qry.record().value("points").toByteArray();
				QDataStream in(&ptArr, QIODevice::ReadOnly);
				while(!in.atEnd()) {
					in >> radianLong;
					in >> radianLat;
					ptF.setX((qreal)radianLat);
					ptF.setY((qreal)radianLong);
					ptFs << ptF;
				}	
				QPolygonF dbRegion(ptFs);
				QPolygonF interSect = dbRegion.intersected(searchRegion);
				if(interSect.isEmpty())
					continue;
			}

			//list->data_list[id].db_index = 1;
			list->data_list[id].latitude = placeLat;
			list->data_list[id].longitude = placeLong;
			list->data_list[id].type = qry.record().value("type").toInt();
			//tmpstr = ""; tempkey = 0;
			tmpstr = qry.record().value("name_ko").toString();
			tmpstr.toWCharArray(tempkey);
			tempkey[tmpstr.count()] = 0;
			wcscpy(list->data_list[id].name, tempkey);
			//wcscpy(list->data_list[id].description, L"");

			list->data_list[id].altitude = 500;
			switch(iter.value()) {
			case PLACE_POINT:
				list->data_list[id].itemtype = IT_PLACEMARK;
				break;
			case OBJECT_POINT:
				list->data_list[id].itemtype = IT_SEARCH;
				break;
			case OBJECT_LINE:
				list->data_list[id].itemtype = IT_PATH;
				break;
			case OBJECT_POLYGON:
				list->data_list[id].itemtype = IT_POLYAREA;
				break;
			}
			list->data_list[id].view_latitude = placeLat;
			list->data_list[id].view_longitude = placeLong;
			list->data_list[id].view_range = 15000;
			list->data_list[id].view_heading = 0;
			list->data_list[id].view_tilt = 0;
			list->data_list[id].showmark = SMT_SHOW;
			list->data_list[id].snapshot = true;

			MakeViewRange(&list->data_list[id]);
			id++;
			if(id == TI_MAX_SEARCH_COUNT) {
				gu_MessageBox(this, tr("Wrong"), tr("Very Too Many Search Results !"), MSGBOX_ICONWARNING);
				goto loopBreak;
			}
		}
	}

loopBreak:
	list->end_index	  = id - 1;
	list->total_count = id;
	if(id == 0) {
		gu_MessageBox(this, tr("Wrong"), tr("There are no Search Results !"), MSGBOX_ICONWARNING);
		m_pCommand->Clear();
		return;
	}

	//<span style=" font-size:8pt; color:#ff0000;">1</span>&nbsp;<a href="http://pic.com/maps?q=´ª&start=17^itemkey=3"><span style=" text-decoration: underline; color:#0000ff;">2</span></a>&nbsp;

	//for (int i = 0; i < responseData->count; i++)
	//{
	//ST_GEODATA_NAME* pData = (ST_GEODATA_NAME*)pBuff;
	//		pBuff += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pData->length + pData->length_ko;
	//}

	int itemkey = m_searchResultView->InsertTable(list, key);
	if(list) delete list;

	m_pCommand->Init();
}

void SearchDlg::setSearchStyle(int idx)
{
	m_ui->cmbSearchStyle->setCurrentIndex(idx);
}
#include "GIS_Area.h"
void SearchDlg::setSearchRegion(CGIS_Node *region)
{
	if(!m_pCommand) 
	{
		CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
		m_pCommand=(CGDM_MeasureAreaCmd*)pCmdMgr->CreateCmd(eGDM_CMD_MEASUREAREA, this);
	}

	m_pCommand->GetNode()->CopyFrom(*region);

	CGIS_Area* area=(CGIS_Area*)m_pCommand->GetNode();

	area->Refresh();
	gdm_GetGISDoc()->SetModifyFlag();
}
void SearchDlg::UpdateDialog(void)
{
}

void::SearchDlg::showEvent( QShowEvent * event )
{
	int style = m_ui->cmbSearchStyle->currentIndex();

	if(style==1 && m_pCommand==NULL)
	{
		CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	    m_pCommand=(CGDM_MeasureAreaCmd*)pCmdMgr->CreateCmd(eGDM_CMD_MEASUREAREA, this);
	}
}

void::SearchDlg::hideEvent( QHideEvent * event )
{
	if(m_pCommand)
		m_pCommand->OnCancel();

	m_pCommand=NULL;
}