#include "cameracontroller.h"
#include "rendersrv.h"
#include "qglobe_renderview.h"
#include "QGlobe_SceneManager.h"
#include "QGlobe_DataMgr.h"
#include "QGlobe_CommandMgr.h"
#include "QGlobe_RequestMgr.h"
#include "QGlobe_TextMgr.h"
#include "QGlobe_FPSTester.h"
#include "GIS_Doc.h"
#include "QGlobe_SimMgr.h"
#include "QGlobe_SimCmd.h"
#include "animationmgr.h"
#include "QGlobe_ThreadMgr.h"
#include "QGlobe_CacheMgr.h"
#include "../qglobe_scene/RasterTextureIDCache.h"
#include "QGlobe_PyramidMgr.h"
#include "config.h"

QGlobe_RENDER_HANDLE	g_pRender = NULL;

RenderSrv::RenderSrv()
{
    m_pView = NULL;
    m_pCameraController = NULL;
	m_blFirstPaint = TRUE;

	m_pQGlobeDataMgr	= new CQGlobe_DataMgr();
	m_pSceneMgr		= new QGlobe_SceneManager();
	m_pCmdMgr		= new CQGlobe_CommandMgr();
	m_pSimMgr		= new CQGlobe_SimMgr;
	m_pGisDoc		= new CGIS_Doc();
	m_pAnimationMgr = new AnimationMgr();
}

RenderSrv::~RenderSrv()
{
	if(m_pSimMgr)			// must be deleted before camera
		delete m_pSimMgr;
	if(m_pCameraController)
		delete m_pCameraController;
	if(m_pQGlobeDataMgr)
		delete m_pQGlobeDataMgr;
	if(m_pSceneMgr)
		delete m_pSceneMgr;
	if(m_pCmdMgr)
		delete m_pCmdMgr;
	if(m_pGisDoc)
		delete m_pGisDoc;
	if(m_pAnimationMgr)
		delete m_pAnimationMgr;
}

bool RenderSrv::Init()
{
	g_pRender = m_pQGlobeDataMgr;

	if(g_pRender == NULL)
		return false;

	m_pSceneMgr->Initialize();

	m_pCameraController = new CameraController(this);

	if (!m_pCameraController)
		return false;

	m_pCmdMgr->SetCameraController(m_pCameraController);
	m_pCmdMgr->CreateCmd(eQGlobe_CMD_CAMERA);

	if (!m_pCameraController->Init())
		return false;

	m_pSimMgr->SetCamera(m_pQGlobeDataMgr->m_pCamera);

    return true;
}

void RenderSrv::SetRenderView(QGLWidget* view)
{
	InitializeGL(view);
}
void RenderSrv::InitializeGL(QGLWidget *view)
{   
	m_pView = (RenderView*)view;

	m_pView->SetRenderSceneMgr(m_pSceneMgr);
	m_pView->OnInitializeGL();
	m_pSceneMgr->prepareSceneGraph();
	m_pSceneMgr->setSimMgr(m_pSimMgr);
}

bool RenderSrv::Paint(QGlobe_RENDER_INFO *renderInfo)
{
	bool bRet = false;

	if ( m_pSimMgr )
		bRet = m_pSimMgr->AdvanceSimulation();

	if ( bRet == false && m_pCameraController)
		m_pCameraController->OnAnimation();

	m_pCameraController->CheckRequest();

	QGlobe_ERROR_NO retVal = OnRender(renderInfo);

	if(m_blFirstPaint)
	{
		m_blFirstPaint = false;
		if(m_pCameraController)
			m_pCameraController->MoveInitPos();
	}

	return retVal == QGlobe_SUCCESS ? true : false;
}

QGlobe_ERROR_NO RenderSrv::OnRender(QGlobe_RENDER_INFO *renderInfo)
{
	unsigned long curTime = qglobe_GetTickTimer();

	qglobe_GetFPSTester()->BeginDraw(curTime);
	qglobe_GetFPSTester()->m_totalMeshCount = 0;
	qglobe_GetFPSTester()->m_totalLineCount = 0;

	// set camera
	if(m_pQGlobeDataMgr->IsModified())
	{
		QGlobe_Coord3D coord;
		m_pQGlobeDataMgr->m_pCamera->GetCameraCoord(&coord);
		m_pView->UpdateCameraInfo(coord, m_pQGlobeDataMgr->m_pCamera->m_frustum);
    }

	// prepare rendering data
	m_pQGlobeDataMgr->OnBeforeRender();
	m_pSceneMgr->onBeforeRender();
	
	m_pView->OnPaint(renderInfo);

	qglobe_GetFPSTester()->EndDraw(qglobe_GetTickTimer());

	m_pSceneMgr->onAfterRender();
	m_pGisDoc->SetModifyFlag(false);
	m_pQGlobeDataMgr->SetModifyFlag(false);
	return QGlobe_SUCCESS;
}

void RenderSrv::SetDataSrv(DataSrv *dataSrv)
{   
	if(m_pView != NULL)
		m_pView->repaint();
}

void RenderSrv::ChangedViewSize(int w, int h)
{
	if ( m_pSimMgr )
		m_pSimMgr->SetScreenSize( w, h );

	if(m_pCameraController)
		m_pCameraController->OnChangedViewSize(w , h);

	m_pQGlobeDataMgr->m_pPyramidMgr->Update();
}

void RenderSrv::MoveView(int x, int y)
{
	if(m_pCameraController == NULL)
		return;
	m_pCameraController->MoveView(x , y);
}

void RenderSrv::RotateView(int direct)
{
	if(m_pCameraController == NULL)
		return;
	m_pCameraController->RotateView(direct);
}

void RenderSrv::TiltView(int direct)
{
	if(m_pCameraController == NULL)
		return;
	m_pCameraController->TiltView(direct);
}

void RenderSrv::LookView(int x , int y)
{
	if(m_pCameraController == NULL)
		return;
	m_pCameraController->LookView(x , y);
}

void RenderSrv::MoveForward(int speed , int nFrom)
{
	if(m_pCameraController == NULL)
		return;
	m_pCameraController->MoveForward(speed , nFrom);
}

void RenderSrv::GetCompassAngle(double* angle)
{
	if(m_pCameraController == NULL)
		return;
	m_pCameraController->GetCompassAngle(angle);
}

void RenderSrv::ChangedCompassAngle(double* angle)
{
	m_pCameraController->ChangedCompassAngle(angle);
}

void RenderSrv::MemCacheClear()
{
	qglobe_CacheMemClear(g_pRender);
}

void RenderSrv::FileCacheClear()
{
	qglobe_CacheFileClear(g_pRender);
}

void RenderSrv::SimpleBuildingCacheClear()
{
	qglobe_SimpleBuildingCacheClear(g_pRender);
}

void RenderSrv::ChangeCacheSize(int nMemCacheSize, int nFileCacheSize)
{
	qglobe_CacheChangeSize(g_pRender, nMemCacheSize, nFileCacheSize);
}

void RenderSrv::ChangeLabelSize(int nSize)
{
	qglobe_GetRenderTextLableSize(g_pRender, nSize);
}

void RenderSrv::ResetTilt()
{
	m_pCameraController->ResetTilt();
}

void RenderSrv::ResetAll()
{
	m_pCameraController->ResetAll();
}

void RenderSrv::StopView()
{
	m_pCameraController->StopView();
}

void RenderSrv::ResetView()
{
	m_pCameraController->ResetView();
}

void RenderSrv::MoveToPlace(PLACEMARK_DATA* placeData, int count)
{
	if(count == 1)
	{
		m_pCameraController->MoveToPlace(placeData);
		return;
	}

	int i, findCount = 0, findIndex = -1;
	CQGlobe_Rect2D bound;


	CGIS_Node *pNode;

	for(i = 0; i < count; i++)
	{
		pNode = m_pGisDoc->GetNodeFromKey(placeData[i].key);
		if(pNode == NULL)
			continue;

		if(findCount == 0)
		{
			bound = pNode->m_cBound;
			findIndex = i;
		}
		else
		{
			bound.addInternalPoint(pNode->m_cBound.m_cMaxPt);
			bound.addInternalPoint(pNode->m_cBound.m_cMinPt);
		}

		findCount++;
	}

	// no found
	if(findCount == 0)
		return;

	// find one object
	if(findCount == 1)
	{
	m_pCameraController->MoveToPlace(placeData + findIndex);
		return;
	}
	m_pCameraController->MoveToBound(bound);
}

void RenderSrv::FastMoveToPlace(PLACEMARK_DATA* placeData)
{
	if(placeData == NULL)
		return;
	m_pCameraController->FastMoveToPlace(placeData);
}

void RenderSrv::SetTerrainLayerState(bool state)
{
	qglobe_UpdateTerrainLayerState(state);
}

void RenderSrv::SetContourState(bool state)
{
	qglobe_UpdateContourState(state);
}

void RenderSrv::SetLayerTextureState(int state)
{
	qglobe_UpdateLayerTextureState((unsigned int)state);
}


void RenderSrv::SetContourOptin(ContourStateInfo* contourOptin)
{
	qglobe_SetContourOptin(contourOptin);
}

void RenderSrv::SetGridOption(int gridView, int gridMode)
{
	qglobe_SetGridOtion(gridView, gridMode);
}

bool RenderSrv::GetRenderImage(QImage* image)
{
	if(m_pView == NULL)
		return false;
	if(!m_pView->GetFrameBuffer(image))
		return false;

	return true;
}

void RenderSrv::GetPlacemarkData(PLACEMARK_DATA* data, bool blPos, bool blCamera)
{
	bool result;
	CQGlobe_Point3DF pt;

	if (data == NULL)
		return;

	if (blPos) /* Get position's info */
	{
		result = m_pCameraController->HitPoint(data->cx, data->cy , &pt);
		if (result)
		{
			QGlobe_LOCATION loc;
			// Get longitude/latitude
			qglobe_GetLocation(&pt, &loc);
			data->longitude = loc.m_dLongitude * qglobe_RADTODEG;
			data->latitude  = loc.m_dLatitude * qglobe_RADTODEG;
		}
	}

	if (blCamera)	/* Get Camera info */
	{
		qglobe_GetPlacemarkViewerData(g_pRender, data);
	}	
}

// quality function: value 1-100
void RenderSrv::SetDistShowMode(unsigned int distviewmode)
{
	qglobe_SetDistShowMode(distviewmode);
}

// quality function: value 1-100
void RenderSrv::SetTopographyQuality(unsigned int quality)
{
	qglobe_SetTopographyQuality(quality);
}

// rate: 0.5-3
void RenderSrv::SetHeightMagnify(double rate)
{
	qglobe_SetHeightMagnify(rate);
}

void RenderSrv::OnChangeMoveToSpeed(double* speed)
{
	qglobe_SetMoveToSpeed(g_pRender , *speed);
}

void RenderSrv::InitializeFontInfo(QString *korName, QString *engName)
{
	int default_font_size = 12;

	m_pSceneMgr->setFont(*korName, *engName, 0);
	SetFontSize(default_font_size);
}

void RenderSrv::SetFontSize(int size)
{
	m_pQGlobeDataMgr->m_pTextMgr->SetFontSize(size);
}

void RenderSrv::OnChangeServer()
{
	m_pQGlobeDataMgr->OnChangeServer();
}

void RenderSrv::ShowLengthArea(bool blShow)
{
	m_pQGlobeDataMgr->m_sOption.showLenArea = blShow;
}

void RenderSrv::StartSimulation(QGlobe_SIM_TYPE simType, bool restart)
{
	m_pSimMgr->StartSimulation(simType, restart);
	CQGlobe_Simulator* pSim = m_pSimMgr->GetCurSimulator();
	CQGlobe_Command* pCommand = m_pCmdMgr->CreateCmd(eQGlobe_CMD_SIMULATION);
	CQGlobe_SimCmd* pSimCmd = (CQGlobe_SimCmd*) pCommand;
	pSimCmd->SetSimulator(pSim);
}	

void RenderSrv::EndSimulation()
{
	//ȣ�� ������ �ٲٸ� �ȵ�
	m_pCmdMgr->GetCurrentCmd()->OnEnd();
	m_pSimMgr->EndSimulation();
}

void RenderSrv::DataThreadInit()
{
	qglobe_LibMainThreadInit();
}

void RenderSrv::ClearRasterCache()
{
	m_pQGlobeDataMgr->	m_pCacheMgr->RasterCacheClear();
	qglobe_GetRasterTextureIDCache()->Clear();
	m_pQGlobeDataMgr->	m_pRequestMgr->UpdateRequest();

	m_pQGlobeDataMgr->SetModifyFlag();
}

void RenderSrv::SetSunLightEnabled(bool* enabled)
{
	m_pSceneMgr->enableLighting(*enabled);
}

void RenderSrv::SetAmbientColor(QColor* color)
{
	m_pSceneMgr->setAmbientColor(*color);
}

void RenderSrv::SetSunLightColor(QColor* color)
{
	m_pSceneMgr->setGlobalLightColor(*color);
}

void RenderSrv::SetSunPosition(double* pos)
{
	m_pSceneMgr->setGlobalLightPosition(pos[0],pos[1],pos[2]);
}

void RenderSrv::SetLightAttenuation(double c,double l,double q)
{
	m_pSceneMgr->setLightAttenuation(c,l,q);
}

void RenderSrv::SetCameraFOV(double fov)
{
	m_pQGlobeDataMgr->m_pCamera->SetFOV(fov);
}