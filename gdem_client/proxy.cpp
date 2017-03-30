#include "gdemclient.h"
#include "mainwindow.h"
#include "renderfrm.h"
#include "proxy.h"
#include "config.h"

#include "datasrv.h"
#include "datachannel.h"
DataSrv *g_dataSrv = NULL;
DataChannel *g_searchChannel = NULL;

/*
 * data proxy
 */

DataProxy::DataProxy()
{
	g_dataSrv = new DataSrv;
	//g_searchChannel = new DataChannel;

}

DataProxy::~DataProxy()
{
	if (g_dataSrv)
		delete g_dataSrv;
	if (g_searchChannel)
		delete g_searchChannel;
}

PrxRes DataProxy::SendMessage(int msg, PrxParam param1, PrxParam param2)
{
	PrxParam res = 0;
	switch (msg)
	{
	case PM_SET_DATASERVER:
		g_dataSrv->SetServer((char*)param1, (unsigned int)param2);
		g_searchChannel = new DataChannel;
		break;

	case PM_INIT:
		res = g_dataSrv->Init();
		break;

	case PM_RELEASE:
		if (g_dataSrv)
		{
			if (g_searchChannel)
				delete g_searchChannel;
			g_dataSrv->Release();
			delete g_dataSrv;
			g_searchChannel = NULL;
			g_dataSrv = NULL;
		}
		break;

	case PM_GET_DATASERVER:
		res = (PrxRes)g_dataSrv;
		break;

	case PM_GET_SEARCHRESULT:
		res = g_searchChannel->RequestData_block((void*)param1, (void*)param2, PACKET_MAX_BUFFER_SIZE);
		break;
	}

	return res;
}

/*
 * render proxy
 */
RenderProxy::RenderProxy()
{
	m_renderSrv = new RenderSrv;
}

RenderProxy::~RenderProxy()
{
	if (m_renderSrv)
		delete m_renderSrv;
}

PrxRes RenderProxy::SendMessage(int msg, PrxParam param1, PrxParam param2)
{
	PrxRes res = 0;
	switch (msg)
	{
	case PM_SET_DATASERVER:
		m_renderSrv->SetDataSrv((DataSrv*)param1);
		break;

	case PM_INIT:
		res = m_renderSrv->Init();
		break;
	case PM_PAINT:
		res = m_renderSrv->Paint((GDM_RENDER_INFO*)param1);
		break;
	case PM_RELEASE:
		if (m_renderSrv)
		{
			delete m_renderSrv;
			m_renderSrv = NULL;
		}
		break;
	case PM_CHANGED_VIEWSIZE:
		if(m_renderSrv)
		{
			m_renderSrv->ChangedViewSize((int) param1 , (int) param2);
		}
		break;
	case PM_CHANGED_COMPASSANGLE:
		m_renderSrv->ChangedCompassAngle((double*) param1);
		break;
	case PM_GET_COMPASSANGLE:
		m_renderSrv->GetCompassAngle((double*) param1);
		break;
	case PM_SET_RENDERVIEW:
		m_renderSrv->SetRenderView((QGLWidget*)param1);
		break;
	case PM_SEARCH_PLACE:
		m_renderSrv->MoveToPlace((PLACEMARK_DATA*)param1, (int)param2);
		break;
	case PM_MOVE_PLACEMARKVIEW:
		m_renderSrv->FastMoveToPlace((PLACEMARK_DATA*)param1);
		break;
	case PM_GET_STATUS_BAR:
		if(m_renderSrv->m_pCameraController)
		if(g_app->GetConfig()->m_showStatusbar)
			m_renderSrv->m_pCameraController->GetStatusInfo((StatusBarInfo*) param1);
		break;

	/* Cache related options */
	case PM_CHANGE_CACHESIZE:
		m_renderSrv->ChangeCacheSize(*((int*)param1) , *((int*) param2));
		break;
	case PM_CLEAR_MEMORYCACHE:
		m_renderSrv->MemCacheClear();
		break;
	case PM_CLEAR_DISKCACHE:
		m_renderSrv->FileCacheClear();
		break;
	case PM_CLEAR_SIMPLEBUILDINGCACHE:
		m_renderSrv->SimpleBuildingCacheClear();
		break;

	/* Label size options */
	case PM_CHANGE_LABLESIZE:
		m_renderSrv->ChangeLabelSize(*((int*)param1));
		break;
	/* Font Info */
	case PM_INIT_FONT:
		m_renderSrv->InitializeFontInfo((QString *) param1, (QString *) param2);
		break;
	case PM_INIT_FONT_SIZE:
		m_renderSrv->SetFontSize((int)param1);
		break;
	case PM_UPDATE_CONTOUR_STATE:
		m_renderSrv->SetContourState(*(bool*) param1);
		break;
	case PM_TERRAIN_LAYER_STATE:
		m_renderSrv->SetTerrainLayerState(*(bool*) param1);
		break;
	case PM_LAYER_TEXTURE_STATE:
		m_renderSrv->SetLayerTextureState(*(int*) param1);
		break;
	case PM_CONTOUR_STATE:
		m_renderSrv->SetContourOptin((ContourStateInfo*)param1);
		break;
	case PM_GRID_STATE:
		m_renderSrv->SetGridOption(*(bool*)param1, 0);
		break;

	case PM_SHOW_LENGTH_AREA:
		m_renderSrv->ShowLengthArea(*(bool*)param1);
		break;
	// view quality
	case PM_TOPOGRAPHY_QUALITY:
		m_renderSrv->SetTopographyQuality(*(unsigned int*)param1);
		break;
	case PM_HEIGHT_MAGNIFY:
		m_renderSrv->SetHeightMagnify(*(double*)param1);
		break;
	case PM_ON_CHANGE_SERVER:
		m_renderSrv->OnChangeServer();
		break;
	case PM_CHANGE_DISTMODE:
		m_renderSrv->SetDistShowMode(*(unsigned int*)param1);
		break;
	case PM_NEW_PLACEMARK:
		m_renderSrv->GetPlacemarkData((PLACEMARK_DATA*)param1, true, true);
		break;
	case PM_SNAPSHOT_PLACEDATA:
		m_renderSrv->GetPlacemarkData((PLACEMARK_DATA*)param1, false, true);
		break;
	case PM_MOVE_PLACEMARK:
		m_renderSrv->GetPlacemarkData((PLACEMARK_DATA*)param1, true, false);
		break;
	case PM_GET_IMAGE:
		m_renderSrv->GetRenderImage((QImage*)param1);
		break;
	case PM_CAMERA_MOVESPEED:
		m_renderSrv->OnChangeMoveToSpeed((double*)param1);
		break;
	case PM_DATA_THREAD_INIT:
		m_renderSrv->DataThreadInit();
		break;
	case PM_CLEAR_RASTER_CACHE:
		m_renderSrv->ClearRasterCache();
		break;
	/*Graphis*/
	case PM_SUNLIGHT_ENABLED:
		m_renderSrv->SetSunLightEnabled((bool*)param1);
		break;
	case PM_SUN_POSITION:
		m_renderSrv->SetSunPosition((double*)param1);
		break;
	case PM_SUNLIGHT_COLOR:
		m_renderSrv->SetSunLightColor((QColor*)param1);
		break;
	case PM_AMBIENT_COLOR:
		m_renderSrv->SetAmbientColor((QColor*)param1);
		break;
	case PM_LIGHT_ATTENUATION:
		m_renderSrv->SetLightAttenuation(*(double*)param1,0.0,0.0);
		break;
	case PM_CAMERA_FOV:
		m_renderSrv->SetCameraFOV(*(double*)param1);

	}
	return res;
}

