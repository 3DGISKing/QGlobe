#include "cameracontroller.h"
#include "rendersrv.h"
#include "gdm_renderview.h"
#include "GDM_SceneManager.h"
#include "GDM_DataMgr.h"
#include "GDM_CommandMgr.h"
#include "GDM_RequestMgr.h"
#include "GDM_TextMgr.h"
#include "GDM_FPSTester.h"
#include "GIS_Doc.h"
#include "GDM_SimMgr.h"
#include "GDM_SimCmd.h"
#include "animationmgr.h"
#include "GDM_ThreadMgr.h"
#include "GDM_CacheMgr.h"
#include "../gdem_scene/RasterTextureIDCache.h"
#include "GDM_PyramidMgr.h"
#include "config.h"

GDM_RENDER_HANDLE	g_pRender = NULL;

RenderSrv::RenderSrv()
{
    m_pView = NULL;
    m_pCameraController = NULL;
	m_blFirstPaint = TRUE;

	m_pGDMDataMgr	= new CGDM_DataMgr();
	m_pSceneMgr		= new GDM_SceneManager();
	m_pCmdMgr		= new CGDM_CommandMgr();
	m_pSimMgr		= new CGDM_SimMgr;
	m_pGisDoc		= new CGIS_Doc();
	m_pAnimationMgr = new AnimationMgr();
}

RenderSrv::~RenderSrv()
{
	if(m_pSimMgr)			// must be deleted before camera
		delete m_pSimMgr;
	if(m_pCameraController)
		delete m_pCameraController;
	if(m_pGDMDataMgr)
		delete m_pGDMDataMgr;
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
	g_pRender = m_pGDMDataMgr;

	if(g_pRender == NULL)
		return false;

	m_pSceneMgr->Initialize();

	m_pCameraController = new CameraController(this);

	if (!m_pCameraController)
		return false;

	m_pCmdMgr->SetCameraController(m_pCameraController);
	m_pCmdMgr->CreateCmd(eGDM_CMD_CAMERA);

	if (!m_pCameraController->Init())
		return false;

	m_pSimMgr->SetCamera(m_pGDMDataMgr->m_pCamera);

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

bool RenderSrv::Paint(GDM_RENDER_INFO *renderInfo)
{
	bool bRet = false;

	if ( m_pSimMgr )
		bRet = m_pSimMgr->AdvanceSimulation();

	if ( bRet == false && m_pCameraController)
		m_pCameraController->OnAnimation();

	m_pCameraController->CheckRequest();

	GDM_ERROR_NO retVal = OnRender(renderInfo);

	if(m_blFirstPaint)
	{
		m_blFirstPaint = false;
		if(m_pCameraController)
			m_pCameraController->MoveInitPos();
	}

	return retVal == GDM_SUCCESS ? true : false;
}

GDM_ERROR_NO RenderSrv::OnRender(GDM_RENDER_INFO *renderInfo)
{
	unsigned long curTime = gdm_GetTickTimer();

	gdm_GetFPSTester()->BeginDraw(curTime);
	gdm_GetFPSTester()->m_totalMeshCount = 0;
	gdm_GetFPSTester()->m_totalLineCount = 0;

	// set camera
	if(m_pGDMDataMgr->IsModified())
	{
		GDM_Coord3D coord;
		m_pGDMDataMgr->m_pCamera->GetCameraCoord(&coord);
		m_pView->UpdateCameraInfo(coord, m_pGDMDataMgr->m_pCamera->m_frustum);
    }

	// prepare rendering data
	m_pGDMDataMgr->OnBeforeRender();
	m_pSceneMgr->onBeforeRender();
	
	m_pView->OnPaint(renderInfo);

	gdm_GetFPSTester()->EndDraw(gdm_GetTickTimer());

	m_pSceneMgr->onAfterRender();
	m_pGisDoc->SetModifyFlag(false);
	m_pGDMDataMgr->SetModifyFlag(false);
	return GDM_SUCCESS;
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

	m_pGDMDataMgr->m_pPyramidMgr->Update();
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
	gdm_CacheMemClear(g_pRender);
}

void RenderSrv::FileCacheClear()
{
	gdm_CacheFileClear(g_pRender);
}

void RenderSrv::SimpleBuildingCacheClear()
{
	gdm_SimpleBuildingCacheClear(g_pRender);
}

void RenderSrv::ChangeCacheSize(int nMemCacheSize, int nFileCacheSize)
{
	gdm_CacheChangeSize(g_pRender, nMemCacheSize, nFileCacheSize);
}

void RenderSrv::ChangeLabelSize(int nSize)
{
	gdm_GetRenderTextLableSize(g_pRender, nSize);
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
	CGDM_Rect2D bound;


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
	gdm_UpdateTerrainLayerState(state);
}

void RenderSrv::SetContourState(bool state)
{
	gdm_UpdateContourState(state);
}

void RenderSrv::SetLayerTextureState(int state)
{
	gdm_UpdateLayerTextureState((unsigned int)state);
}


void RenderSrv::SetContourOptin(ContourStateInfo* contourOptin)
{
	gdm_SetContourOptin(contourOptin);
}

void RenderSrv::SetGridOption(int gridView, int gridMode)
{
	gdm_SetGridOtion(gridView, gridMode);
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
	CGDM_Point3DF pt;

	if (data == NULL)
		return;

	if (blPos) /* Get position's info */
	{
		result = m_pCameraController->HitPoint(data->cx, data->cy , &pt);
		if (result)
		{
			GDM_LOCATION loc;
			// Get longitude/latitude
			gdm_GetLocation(&pt, &loc);
			data->longitude = loc.m_dLongitude * gdm_RADTODEG;
			data->latitude  = loc.m_dLatitude * gdm_RADTODEG;
		}
	}

	if (blCamera)	/* Get Camera info */
	{
		gdm_GetPlacemarkViewerData(g_pRender, data);
	}	
}

// quality function: value 1-100
void RenderSrv::SetDistShowMode(unsigned int distviewmode)
{
	gdm_SetDistShowMode(distviewmode);
}

// quality function: value 1-100
void RenderSrv::SetTopographyQuality(unsigned int quality)
{
	gdm_SetTopographyQuality(quality);
}

// rate: 0.5-3
void RenderSrv::SetHeightMagnify(double rate)
{
	gdm_SetHeightMagnify(rate);
}

void RenderSrv::OnChangeMoveToSpeed(double* speed)
{
	gdm_SetMoveToSpeed(g_pRender , *speed);
}

void RenderSrv::InitializeFontInfo(QString *korName, QString *engName)
{
	int default_font_size = 12;

	m_pSceneMgr->setFont(*korName, *engName, 0);
	SetFontSize(default_font_size);
}

void RenderSrv::SetFontSize(int size)
{
	m_pGDMDataMgr->m_pTextMgr->SetFontSize(size);
}

void RenderSrv::OnChangeServer()
{
	m_pGDMDataMgr->OnChangeServer();
}

void RenderSrv::ShowLengthArea(bool blShow)
{
	m_pGDMDataMgr->m_sOption.showLenArea = blShow;
}

void RenderSrv::StartSimulation(GDM_SIM_TYPE simType, bool restart)
{
	m_pSimMgr->StartSimulation(simType, restart);
	CGDM_Simulator* pSim = m_pSimMgr->GetCurSimulator();
	CGDM_Command* pCommand = m_pCmdMgr->CreateCmd(eGDM_CMD_SIMULATION);
	CGDM_SimCmd* pSimCmd = (CGDM_SimCmd*) pCommand;
	pSimCmd->SetSimulator(pSim);
}	

void RenderSrv::EndSimulation()
{
	//호출 순서를 바꾸면 안됨
	m_pCmdMgr->GetCurrentCmd()->OnEnd();
	m_pSimMgr->EndSimulation();
}

void RenderSrv::DataThreadInit()
{
	gdm_LibMainThreadInit();
}

void RenderSrv::ClearRasterCache()
{
	m_pGDMDataMgr->	m_pCacheMgr->RasterCacheClear();
	gdm_GetRasterTextureIDCache()->Clear();
	m_pGDMDataMgr->	m_pRequestMgr->UpdateRequest();

	m_pGDMDataMgr->SetModifyFlag();
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
	m_pGDMDataMgr->m_pCamera->SetFOV(fov);
}