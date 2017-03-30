#include "stable.h"
#include "GDM_CommandMgr.h"
#include "GDM_RulerCmd.h"
#include "GDM_PointCmd.h"
#include "GDM_DrawPolygonCmd.h"
#include "GDM_CameraCmd.h"
#include "GDM_ObjectAddCmd.h"
#include "GIS_Doc.h"
#include "GIS_Node.h"
#include "cameracontroller.h"
#include "GDM_SimCmd.h"
#include "../gdem_libmain/GDM_DataMgr.h"
#include "GDM_3DObjectCache.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "../gdem_scene/Geo3dObjectSceneNode.h"
#include "GeoMeshUtil.h"
#include "config.h"
#include "objectplaceeditdlg.h" //asd 2014.3.31
#include "GDM_MeasureAreaCmd.h"
#include "GDM_DrawObjectCmd.h"
#include "../gdem_libmain/GDM_TextMgr.h"

using namespace  scene;

CameraController* CGDM_CommandMgr::m_pCameraController = NULL;

CGDM_CommandMgr::CGDM_CommandMgr(void)
{
	//m_pCurCmd = NULL;
	//m_pPrevCmd = NULL;
	m_pCameraCmd = new CGDM_CameraCmd();
	m_pCameraCmd->SetCommandMgr(this);
	m_blFirstMove = FALSE;
}

CGDM_CommandMgr::~CGDM_CommandMgr(void)
{
	delete m_pCameraCmd;
}
CGDM_Command* CGDM_CommandMgr::CreateCmd(E_GDM_CMD_TYPE eCmdType, CGDM_CommandDlg *pCommandDlg)
{
	CGDM_Command *pNewCommand = NULL;

	switch(eCmdType)
	{
		case eGDM_CMD_RULER:
			pNewCommand = new CGDM_RulerCmd(pCommandDlg);
			break;
		case eGDM_CMD_PLACEMARK:
			pNewCommand = new CGDM_PointCmd(pCommandDlg);
			break;
		case eGDM_CMD_DRAW_POLYGON:
			pNewCommand = new CGDM_DrawPolygonCmd(pCommandDlg);
			break;
		case eGDM_CMD_SIMULATION:
			pNewCommand = new CGDM_SimCmd(pCommandDlg);
			break;
		case eGDM_CMD_CAMERA:
			break;
		case eGDM_CMD_OBJECT_ADD:
			pNewCommand= new CGDM_ObjectAddCmd(pCommandDlg);
			break;
		case eGDM_CMD_MEASUREAREA:
			pNewCommand= new CGDM_MeasureAreaCmd(pCommandDlg);
			break;
		case eGDM_CMD_DRAW_OBJECT: //asd 14.4.11
			pNewCommand = new CGDM_DrawObjectCmd(pCommandDlg);
			break;
		default:
			//ASSERT(FALSE);
			break;
	}

	if(pNewCommand && m_pEditCmd)
	{
		delete m_pEditCmd;
	}

	m_pEditCmd = pNewCommand;

	if(m_pEditCmd)
		m_pEditCmd->SetCommandMgr(this);

	if(m_pEditCmd && m_pEditCmd->GetNode())
		m_pEditCmd->GetNode()->SetState(E_GISNODE_STATE_SELECT);

	return pNewCommand;
}

bool CGDM_CommandMgr::IsEditing()
{
	if(m_pEditCmd)
		return true;
	return false;
}

CGDM_Command* CGDM_CommandMgr::GetCurrentCmd()
{
	if(m_pEditCmd)
		return m_pEditCmd;
	return m_pCameraCmd;
}

bool CGDM_CommandMgr::OnMousePress(CursorState* mouseEvent)
{
	SetCursorPt(mouseEvent);

	bool ret = false;

	m_cOldPressCursor = *mouseEvent;
	m_blFirstMove = true;

	if(m_pEditCmd){
		ret = m_pEditCmd->OnMousePress(mouseEvent);
		if(mouseEvent->buttons & MBS_MID && m_pEditCmd->GetCmdType() != eGDM_CMD_SIMULATION)
			ret = false;
	}


	if(!ret)
	{
		m_blFirstMove = false;
		ret = m_pCameraCmd->OnMousePress(mouseEvent);
		if(m_pEditCmd)
			m_pEditCmd->SetCursorType(m_pCameraCmd->GetCursorType());
	}

	Config *conf=g_app->GetConfig();

	if(conf->m_IsSelectObject)
		OnCheckSelectObject(false);

	return ret;
}

bool CGDM_CommandMgr::OnMouseMove(CursorState* mouseEvent)
{
	SetCursorPt(mouseEvent);

	bool ret = false;

	if(m_pEditCmd){
		ret = m_pEditCmd->OnMouseMove(mouseEvent);
		if(mouseEvent->buttons & MBS_MID && m_pEditCmd->GetCmdType() != eGDM_CMD_SIMULATION)
			ret = false;
	}

	if(!ret)
	{
		if(m_blFirstMove)
		{
			m_blFirstMove = false;
			m_pCameraCmd->OnMousePress(&m_cOldPressCursor);
		}
		ret = m_pCameraCmd->OnMouseMove(mouseEvent);
		if(m_pEditCmd)
			m_pEditCmd->SetCursorType(m_pCameraCmd->GetCursorType());
	}

	Config* conf=g_app->GetConfig(); 
	if(conf->m_IsSelectObject)
	{
		OnCheckSelectObject(true);
	}
	OnCheckSelect3dObject();

	return ret;
}

bool CGDM_CommandMgr::OnMouseRelease(CursorState* mouseEvent)
{
	SetCursorPt(mouseEvent);
	bool ret = false;

	m_blFirstMove = false;

	if(m_pEditCmd)
	{
		ret = m_pEditCmd->OnMouseRelease(mouseEvent);
		if(mouseEvent->buttons & MBS_MID && m_pEditCmd->GetCmdType() != eGDM_CMD_SIMULATION)
			ret = false;
	}

	if(!ret)
	{
		ret = m_pCameraCmd->OnMouseRelease(mouseEvent);
		if(m_pEditCmd)
			m_pEditCmd->SetCursorType(m_pCameraCmd->GetCursorType());
	}

	if(m_pEditCmd && m_pEditCmd->GetCmdType() != eGDM_CMD_SIMULATION)
		GetCameraController()->EndAnimation();

	OnCheckPlaceMarkContent();//2014 2 6 by ugi

	return ret;
}

bool CGDM_CommandMgr::OnMouseDblClick(CursorState* mouseEvent)
{
	Config* conf=g_app->GetConfig();

	if(conf->m_IsSelectObject)
		if( OnCheckSelectGeo3dObject())
			return false;

	int nFocusID;
	E_GDM_SUBDATA_TYPE type;
	if(GetCurrentCmd()->GetCmdType() == eGDM_CMD_DRAW_OBJECT) { //asd 14.4.11
		ObjectPlaceEditDlg *objectplaceEditDlg = g_app->GetMainWindow()->getObjectPlaceEditDlg();
		if(objectplaceEditDlg->IsPropertyMode()) {
			nFocusID = g_pGDMDataMgr->m_pTextMgr->GetFocusID();	
			if(nFocusID >= 0) {
				type = g_pGDMDataMgr->m_pTextMgr->GetFocusType();
				objectplaceEditDlg->setPropertyData(type, nFocusID);
			}
			return false;
		} else if(objectplaceEditDlg->IsEditMode()) {
			return m_pEditCmd->OnMouseDblClick(mouseEvent);
		} else {
			nFocusID = g_pGDMDataMgr->m_pTextMgr->GetFocusID();	
			if(nFocusID >= 0) {
				type = g_pGDMDataMgr->m_pTextMgr->GetFocusType();
				objectplaceEditDlg->setModifyData(type, nFocusID);
			}
			return false;
		}
	}

	return GetCurrentCmd()->OnMouseDblClick(mouseEvent);
}

bool CGDM_CommandMgr::OnMouseWheelEvent(CursorState* mouseEvent)
{
	SetCursorPt(mouseEvent);
	return GetCurrentCmd()->OnMouseWheelEvent(mouseEvent);
}

bool CGDM_CommandMgr::OnMouseClickEvent(CursorState* mouseEvent)
{
	return GetCurrentCmd()->OnMouseClickEvent(mouseEvent);
}

void CGDM_CommandMgr::OnKeyPressEvent(KeyState keyState)
{
	return GetCurrentCmd()->OnKeyPressEvent(keyState);
}

void CGDM_CommandMgr::CancelEditCmd()
{
	if(m_pEditCmd)
	{
		delete m_pEditCmd;
		m_pEditCmd = NULL;
	}
}

MOUSE_POSITION	CGDM_CommandMgr::GetCursorPt()
{
	MOUSE_POSITION pos;
	m_pCameraController->GetCursorPos(pos);
	return pos;
}

void CGDM_CommandMgr::SetCursorPt(CursorState* mouseEvent)
{
	m_pCameraController->SetCursorPt(mouseEvent);
}

void CGDM_CommandMgr::OnCheckSelectObject(bool blFocus)
{
	MOUSE_POSITION pos;
	CGIS_Doc *pGISDoc;
	CGIS_Node *pNode;
	unsigned int index, nCnt;

	// check edit state
	if(m_pEditCmd)
		return;

	// check camera moving state
	// TODO

	// get mouse position
	pos = m_pCameraCmd->GetCurrentLocation();

	pGISDoc = gdm_GetGISDoc();

	if(blFocus)
	{
		nCnt = pGISDoc->GetCount();

		for(index = 0; index < nCnt; index++)
		{
			pNode = pGISDoc->GetNodeFromIndex(index);
			if(pNode->IsVisible())
				continue;
			
			if(pNode->IsContain(pos)  )
				pNode->SetFocus(true);
			else
				pNode->SetFocus(false);
		}

	}
	else
	{
		if(pGISDoc->GetSelectNode() != NULL)
		{
			if(pGISDoc->GetSelectNode()->IsContain(pos))
				return;
		}

		nCnt = pGISDoc->GetCount();

		for(index = 0; index < nCnt; index++)
		{
			pNode = pGISDoc->GetNodeFromIndex(index);
			if(!pNode->IsVisible())
				continue;
			
			if(pNode->IsContain(pos))
			{
				pGISDoc->SetSelectNode(pNode);
				return;
			}
		}

		pGISDoc->SetSelectNode(NULL);
	}
}

//ººÀõ´ô 3dÊï¹¶½¤Àâ°¡ ËØËº·² °ûËæ ´ÝÂÙ ºÐºÂË¾ ¸óÊÞ¼Ñ°Ö true¶¦ ´®¶°³Þ.
//ººÀõ´ô Êï¹¶½¤Àâ°¡ ÊÖËº·² false¶¦ ´®¶°³Þ.

#include "GDM_SimpleBuildingCache.h"

bool CGDM_CommandMgr::OnCheckSelectGeo3dObject()
{
	MOUSE_POSITION pos;

	// get mouse position
	pos = m_pCameraCmd->GetCurrentLocation();

	const GDM_LOGIC_TILE *ptile;
	GDM_TILE* meshtile;
	SAnimatedMesh* meshs;
	SGeoMesh* mesh;
	
	CGDM_Matrix4 transform;
	const GDM_LOGIC_TILE_ARR *render_tile_list= gdm_GetRenderTiles();

	for(int i=0;i<render_tile_list->nCount;i++)
	{
		ptile=render_tile_list->pData+i;//i¸ÜÉã µã¶®À°ËÎË¾ ÊÐ²÷³Þ.

		for(int j=0;j<ptile->n3dMeshTileCnt;j++)
		{
			meshtile=ptile->sArr3DMeshTile[j];
			meshs=(SAnimatedMesh*)g_pGDMDataMgr->m_p3DObjectCache->GetMeshs(meshtile->m_sInfo.nX,
																			meshtile->m_sInfo.nY,
																			meshtile->m_sInfo.nLevel,
																			meshtile->m_pBuffer);

			for (unsigned int k=0;k<meshs->Meshes.size();k++)
			{
				mesh=(SGeoMesh*)meshs->Meshes[k];

				if(mesh->GeoInfo.Longitude < ptile->minLongitude || mesh->GeoInfo.Longitude > ptile->maxLongitude
					|| mesh->GeoInfo.Latitude < ptile->minLatitude || mesh->GeoInfo.Latitude > ptile->maxLatitude)
					continue;

				core::rect<s32> screenrect;

				GeoMeshUtil::CalculateScreenRect(mesh,screenrect);

				if(screenrect.isPointInside(core::position2di(pos.x,pos.y)))				 
				{
					g_app->GetMainWindow()->ShowBuildingPropDlg(mesh->GeoInfo.ID);
					return true;
				}  
			}  //unsigned int k=0;k<meshs->Meshes.size()
		}  //int j=0;j<ptile->n3dMeshTileCnt;j++

		BuildingArray* pSimpleBuildingArr;
		for(int j=0;j<ptile->nPoygonTileCnt;j++)
		{
			GDM_TILE *polyTile=ptile->sArrPolygonTile[j];

			pSimpleBuildingArr=g_pGDMDataMgr->m_pSimpleBuildingCache->GetBuildingArray(polyTile);

			if(!pSimpleBuildingArr)		continue;

			for(unsigned int k=0;k<pSimpleBuildingArr->Count();k++)
			{
				SimpleBuilding* psimplebuilding=(SimpleBuilding*)pSimpleBuildingArr->GetBuilding(k);

				/*if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tX < ptile->minLongitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tY < ptile->minLatitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tX > ptile->maxLongitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tY > ptile->maxLatitude) continue;*/

				CGDM_Rect2D screenrect;

				psimplebuilding->CalculateScreenRect(screenrect);

				if(screenrect.isPointInside(pos.x,pos.y))				 
				{
					g_app->GetMainWindow()->ShowBuildingPropDlg(psimplebuilding->GetID());
					return true;
				}  //screenrect.isPointInside
			}
		}
	}
	return false;
}


void CGDM_CommandMgr::OnStartAnimation(GDM_ANIMATION_TYPE aniType)
{
	GetCurrentCmd()->OnStartAnimation(aniType);
}

void CGDM_CommandMgr::OnNotifyEvent(EGDM_RENDER_NOTIFY_TYPE nType, int param1, int param2 , int param3)
{
	switch(nType)
	{
	case GDM_RENDER_NITIFY_START_ANIMATION:
		GetCurrentCmd()->OnStartAnimation((GDM_ANIMATION_TYPE)param1);
		break;
	case GDM_RENDER_NITIFY_END_ANIMATION:
		GetCurrentCmd()->OnEndAnimation((GDM_ANIMATION_TYPE)param1);
		break;
	case GDM_RENDER_NITIFY_CAMERA_CHANGED:
		{
			GetCurrentCmd()->OnCameraChanged();
			break;
		}
	}
}

//2014 2 7 by ugi
void CGDM_CommandMgr::OnCheckPlaceMarkContent()
{
	MOUSE_POSITION pos;
	CGIS_Doc *pGISDoc;
	CGIS_Node *pNode;

	// check edit state
	if(m_pEditCmd)
		return;

	// get mouse position
	pos = m_pCameraCmd->GetCurrentLocation();

	pGISDoc = gdm_GetGISDoc();

	for(unsigned index = 0; index < pGISDoc->GetCount(); index++)
	{
		pNode = pGISDoc->GetNodeFromIndex(index);
		if(!pNode->IsVisible())
			continue;
		
		if(pNode->Get_GisType()!=E_GISNODE_TYPE_PLACEMARK) 
			continue;

		if(pNode->IsContain(pos))
		{
			CGIS_PlaceIcon * placeicon=(CGIS_PlaceIcon*)pNode;
			
			if(placeicon->m_sVideoPath[0]==0 && placeicon->m_sHtmlPath[0]==0)
				continue;

			QString videopath;

			if(placeicon->m_sVideoPath[0]!=0 )
				videopath=QString::fromUtf16(placeicon->m_sVideoPath);
						
			QString htmlpath;
			if(placeicon->m_sHtmlPath[0]!=0)
				htmlpath=QString::fromUtf16(placeicon->m_sHtmlPath);

			g_app->GetMainWindow()->ShowVideoWebView(videopath,htmlpath,true);
				return;
		}
	}
}

void CGDM_CommandMgr::OnCheckSelect3dObject()
{
	if(g_app->GetConfig()->m_showObjectAdd)
		return;

	g_Focused3dObjectID=-1;

    MOUSE_POSITION pos;

	// get mouse position
	pos = m_pCameraCmd->GetCurrentLocation();

	const GDM_LOGIC_TILE *ptile;
	GDM_TILE* meshtile;
	SAnimatedMesh* meshs;
	SGeoMesh* mesh;
	
	CGDM_Matrix4 transform;
	const GDM_LOGIC_TILE_ARR *render_tile_list= gdm_GetRenderTiles();

	for(int i=0;i<render_tile_list->nCount;i++)
	{
		ptile=render_tile_list->pData+i;//i¸ÜÉã µã¶®À°ËÎË¾ ÊÐ²÷³Þ.

		for(int j=0;j<ptile->n3dMeshTileCnt;j++)
		{
			meshtile=ptile->sArr3DMeshTile[j];
			meshs=(SAnimatedMesh*)g_pGDMDataMgr->m_p3DObjectCache->GetMeshs(meshtile->m_sInfo.nX,
																			meshtile->m_sInfo.nY,
																			meshtile->m_sInfo.nLevel,
																			meshtile->m_pBuffer);

			for (unsigned int k=0;k<meshs->Meshes.size();k++)
			{
				mesh=(SGeoMesh*)meshs->Meshes[k];

				if(mesh->GeoInfo.Longitude < ptile->minLongitude || mesh->GeoInfo.Longitude > ptile->maxLongitude
					|| mesh->GeoInfo.Latitude < ptile->minLatitude || mesh->GeoInfo.Latitude > ptile->maxLatitude)
					continue;

				core::rect<s32> screenrect;

				GeoMeshUtil::CalculateScreenRect(mesh,screenrect);

				if(screenrect.isPointInside(core::position2di(pos.x,pos.y)))				 
				{
					g_Focused3dObjectID=mesh->GeoInfo.ID;
				}  
			}  //unsigned int k=0;k<meshs->Meshes.size()
		}  //int j=0;j<ptile->n3dMeshTileCnt;j++

		BuildingArray* pSimpleBuildingArr;
		for(int j=0;j<ptile->nPoygonTileCnt;j++)
		{
			GDM_TILE *polyTile=ptile->sArrPolygonTile[j];

			pSimpleBuildingArr=g_pGDMDataMgr->m_pSimpleBuildingCache->GetBuildingArray(polyTile);

			if(!pSimpleBuildingArr)		continue;

			for(unsigned int k=0;k<pSimpleBuildingArr->Count();k++)
			{
				SimpleBuilding* psimplebuilding=(SimpleBuilding*)pSimpleBuildingArr->GetBuilding(k);

				/*if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tX < ptile->minLongitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tY < ptile->minLatitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tX > ptile->maxLongitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tY > ptile->maxLatitude) continue;*/

				CGDM_Rect2D screenrect;

				psimplebuilding->CalculateScreenRect(screenrect);

				if(screenrect.isPointInside(pos.x,pos.y))				 
				{
					g_Focused3dObjectID=psimplebuilding->GetID();
				}  //screenrect.isPointInside
			}
		}
	}
}
