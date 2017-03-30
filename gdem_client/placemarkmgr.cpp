#include "placemarkmgr.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "renderfrm.h"
#include "sidebar.h"
#include "resulttreeview.h"
#include "placemarkdlg.h"
#include "placefolderdlg.h"
#include "placemarkdlg.h"
#include "placefolderdlg.h"
#include "guiuty.h"
#include "msgdata.h"
#include "rendercursor.h"
#include "proxy.h"
#include "GIS_Doc.h"
#include "GIS_Node.h"
#include "GDM_CommandMgr.h"
#include "GDM_DataMgr.h"
#include "../gdem_libmain/GDM_TextMgr.h"
#include "../gds_database/GDSDatabaseInterface.h"
#include "../gds_database/GDSGeoNameDBManager.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

//2013/8/1 ugi
#define DEFAULT_ICON_NAME "/res/icon/pushpin/ylw-pushpin.png"
//end

PlaceMarkMgr::PlaceMarkMgr()
{
	m_placeMarkData = SideBar::GetPlaceMarkMap();
	m_resultTreeView = NULL;

	memset(&m_propertyOrgData, 0, sizeof(m_propertyOrgData));
}

PlaceMarkMgr::~PlaceMarkMgr()
{
}

void PlaceMarkMgr::SetResultTreeView(ResultTreeView *currentView)
{
	m_resultTreeView = currentView;
}

void PlaceMarkMgr::GetNewPlaceMarkData(PLACEMARK_DATA *data, ItemType type)
{
	PLACEMARK_DATA pmd;
	memset(&pmd, 0, sizeof(pmd));

	QString name =QObject::tr("Unamed Place Mark");
	gu_qstring2ushort(name, pmd.name, PMD_STR_MAX_SIZE);
	pmd.itemtype = type;
	pmd.cx = g_app->GetMainWindow()->GetRenderFrame()->rect().width() / 2;
	pmd.cy = g_app->GetMainWindow()->GetRenderFrame()->rect().height() / 2;
	pmd.showmark = SMT_SHOW;
	pmd.state = PLACEMARK_SELECT;

	if (pmd.itemtype == IT_PLACEMARK)
	{
		pmd.snapshot = true;

		//2013/8/1 by ugi
		QString iconname=DEFAULT_ICON_NAME;
		gu_qstring2ushort(iconname, pmd.iconname, PMD_STR_MAX_SIZE);
		pmd.iconscale=1.0;
	}

	m_resultTreeView->GetPlaceMarkInfo(pmd);

	RenderProxy *render = g_app->GetRenderProxy();
	render->SendMessage(PM_NEW_PLACEMARK, (PrxParam)&pmd);

	m_placeMarkData->insert(pmd.key, pmd);
	*data = pmd;
}

void PlaceMarkMgr::GetPropertyPlaceData(PLACEMARK_DATA *data, bool isAdd)
{
	Q_ASSERT(m_resultTreeView);

	PLACEMARK_DATA pmd;
	memset(&pmd, 0, sizeof(pmd));

	m_resultTreeView->GetPlaceMarkInfo(pmd, isAdd);

	m_propertyOrgData = pmd;
	pmd.state = PLACEMARK_SELECT;
	pmd.orgDist = PLACEMARK_DEMHEIGHT_INVALID;

	if (pmd.snapshot)
	{
		gu_MakeRenderData(&pmd);
		RenderProxy *render = g_app->GetRenderProxy();
		render->SendMessage(PM_MOVE_PLACEMARKVIEW, (PrxParam)&pmd);
		gu_MakeLocalData(&pmd);
	}

	m_placeMarkData->insert(pmd.key, pmd);
	*data = pmd;
}

void PlaceMarkMgr::GetEditPlaceData(PLACEMARK_DATA *data, unsigned int state)
{
	if (state == PLACEDLG_ACCEPT || state == PLACEDLG_REJECT)
		return;

	PLACEMARK_DATA pmd = *data;

	if (pmd.snapshot && (state == PLACEDLG_VIEW_EDIT))
	{
		gu_MakeRenderData(&pmd);
		RenderProxy *render = g_app->GetRenderProxy();
		render->SendMessage(PM_MOVE_PLACEMARKVIEW, (PrxParam)&pmd);
		gu_MakeLocalData(&pmd);
	}

	m_placeMarkData->insert(pmd.key, pmd);
}

void PlaceMarkMgr::GetApplyPlaceData(PLACEMARK_DATA *data, bool isAdd)
{
	PLACEMARK_DATA pmd = *data;
	pmd.state = PLACEMARK_NORMAL;

	if (pmd.snapshot)
	{
		gu_MakeRenderData(&pmd);
		RenderProxy *render = g_app->GetRenderProxy();
		render->SendMessage(PM_MOVE_PLACEMARKVIEW, (PrxParam)&pmd);
		gu_MakeLocalData(&pmd);
	}

	m_resultTreeView->SetPlaceMarkInfo(pmd, isAdd);
}

void PlaceMarkMgr::GetCancelPlaceData(PLACEMARK_DATA *data, bool isAdd)
{
	PLACEMARK_DATA pmd = *data;

	if (isAdd)
	{
		m_resultTreeView->RemovePlaceMarkInfo(pmd);
	}
	else
	{
		pmd = m_propertyOrgData;
		m_resultTreeView->RemovePlaceMarkInfo(pmd, false);
	}
}

void PlaceMarkMgr::GetSnapshotPlaceData(PLACEMARK_DATA *data)
{
	PLACEMARK_DATA pmd = *data;
	pmd.snapshot = true;

	RenderProxy *render = g_app->GetRenderProxy();
	render->SendMessage(PM_SNAPSHOT_PLACEDATA, (PrxParam)&pmd);

	m_placeMarkData->insert(pmd.key, pmd);
	*data = pmd;
}

void PlaceMarkMgr::GetResetPlaceData(PLACEMARK_DATA *data)
{
	PLACEMARK_DATA pmd = *data;

	if (pmd.itemtype == IT_PLACEMARK || pmd.itemtype == IT_SEARCH)
	{
		pmd.view_latitude = pmd.latitude;
		pmd.view_longitude = pmd.longitude;
	}
	pmd.view_range = 1000;
	pmd.view_heading = 0;
	pmd.view_tilt = 0;

	gu_MakeRenderData(&pmd);
	RenderProxy *render = g_app->GetRenderProxy();
	render->SendMessage(PM_SEARCH_PLACE, (PrxParam)&pmd);
	gu_MakeLocalData(&pmd);

	m_placeMarkData->insert(pmd.key, pmd);
	*data = pmd;
}

void PlaceMarkMgr::GetNewPlaceFolderData(PLACEMARK_DATA *data)
{
	PLACEMARK_DATA pmd;
	memset(&pmd, 0, sizeof(pmd));

	QString name = QApplication::translate("PlaceMarkMgr","Unnamed Place Folder",0,QApplication::UnicodeUTF8);
	gu_qstring2ushort(name, pmd.name, PMD_STR_MAX_SIZE);
	pmd.itemtype = IT_FOLDER;
	pmd.showmark = SMT_SHOW;
	pmd.state = PLACEMARK_SELECT;

	m_resultTreeView->GetPlaceMarkInfo(pmd);

	*data = pmd;
}

// show context menu
bool PlaceMarkMgr::MouseClickEvent(const CursorState *event)
{
	unsigned int key;

	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	if(pCmdMgr->IsEditing())
		return false;
	key = GetSelectKey();

	if (key >0)
	{	
		PLACEMARK_DATA data = m_placeMarkData->value(key);

		QPoint pos;
		GetGlobalPos(event->x, event->y, &pos);

		MainWindow *mainWnd = g_app->GetMainWindow();
		SideBar *sideBar =g_app->GetMainWindow()->GetSideBar();

		sideBar->SetSelectItem(&data, true);
		
		if (event->buttons & MBS_RIGHT)
			mainWnd->ShowContextMenu(pos, false);
	}

    E_GDM_SUBDATA_TYPE type=g_pGDMDataMgr->m_pTextMgr->GetFocusType();

	int nFocusID = g_pGDMDataMgr->m_pTextMgr->GetFocusID();	

	if(nFocusID!=-1 && type!=E_GDM_SUBDATA_TYPE_NONE)
	{
		QSqlRecord rec=g_DBManager->GetTableRecord(type,nFocusID);

		QString htmlpath=rec.field("html_path").value().toString();
		QString videopath=rec.field("video_path").value().toString();

		if(htmlpath.isEmpty() &&videopath.isEmpty())
			return false;
		
		g_app->GetMainWindow()->ShowVideoWebView(videopath,htmlpath,true);
	}
	return true;
}

bool PlaceMarkMgr::IsSelectedMark(PLACEMARK_DATA *data)
{
	CGIS_Doc *pDoc = gdm_GetGISDoc();
	if(pDoc->GetSelectNode() == NULL)
		return false;
	if(pDoc->GetSelectNode()->GetKey() != data->key)
		return false;

	return true;
}
unsigned int PlaceMarkMgr::GetSelectKey()
{
	CGIS_Doc *pDoc = gdm_GetGISDoc();
	if(pDoc->GetSelectNode() == NULL)
		return 0;

	return pDoc->GetSelectNode()->GetKey();
}

void PlaceMarkMgr::GetGlobalPos(const int x, const int y, QPoint *pos)
{
	QPoint mainWndPos = g_app->GetMainWindow()->pos();
	QSize mainWndSize = g_app->GetMainWindow()->size();
	QSize renderFrm = g_app->GetMainWindow()->GetRenderFrame()->size();

	int dx = mainWndSize.width() - renderFrm.width();
	int dy = mainWndSize.height() - renderFrm.height() + 32;

	pos->setX(mainWndPos.x() + x + dx);
	pos->setY(mainWndPos.y() + y + dy);
}
