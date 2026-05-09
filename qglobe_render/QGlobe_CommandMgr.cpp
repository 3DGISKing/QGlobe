#include "stable.h"
#include "QGlobe_CommandMgr.h"
#include "QGlobe_RulerCmd.h"
#include "QGlobe_PointCmd.h"
#include "QGlobe_DrawPolygonCmd.h"
#include "QGlobe_CameraCmd.h"
#include "QGlobe_ObjectAddCmd.h"
#include "GIS_Doc.h"
#include "GIS_Node.h"
#include "cameracontroller.h"
#include "QGlobe_SimCmd.h"
#include "../qglobe_libmain/QGlobe_DataMgr.h"
#include "QGlobe_3DObjectCache.h"
#include "qglobeclient.h"
#include "mainwindow.h"
#include "../qglobe_scene/Geo3dObjectSceneNode.h"
#include "GeoMeshUtil.h"
#include "config.h"
#include "objectplaceeditdlg.h" //asd 2014.3.31
#include "QGlobe_MeasureAreaCmd.h"
#include "QGlobe_DrawObjectCmd.h"
#include "../qglobe_libmain/QGlobe_TextMgr.h"

using namespace  scene;

CameraController* CQGlobe_CommandMgr::m_pCameraController = NULL;

CQGlobe_CommandMgr::CQGlobe_CommandMgr(void)
{
	//m_pCurCmd = NULL;
	//m_pPrevCmd = NULL;
	m_pCameraCmd = new CQGlobe_CameraCmd();
	m_pCameraCmd->SetCommandMgr(this);
	m_blFirstMove = FALSE;
}

CQGlobe_CommandMgr::~CQGlobe_CommandMgr(void)
{
	delete m_pCameraCmd;
}
CQGlobe_Command* CQGlobe_CommandMgr::CreateCmd(E_QGlobe_CMD_TYPE eCmdType, CQGlobe_CommandDlg *pCommandDlg)
{
	CQGlobe_Command *pNewCommand = NULL;

	switch(eCmdType)
	{
		case eQGlobe_CMD_RULER:
			pNewCommand = new CQGlobe_RulerCmd(pCommandDlg);
			break;
		case eQGlobe_CMD_PLACEMARK:
			pNewCommand = new CQGlobe_PointCmd(pCommandDlg);
			break;
		case eQGlobe_CMD_DRAW_POLYGON:
			pNewCommand = new CQGlobe_DrawPolygonCmd(pCommandDlg);
			break;
		case eQGlobe_CMD_SIMULATION:
			pNewCommand = new CQGlobe_SimCmd(pCommandDlg);
			break;
		case eQGlobe_CMD_CAMERA:
			break;
		case eQGlobe_CMD_OBJECT_ADD:
			pNewCommand= new CQGlobe_ObjectAddCmd(pCommandDlg);
			break;
		case eQGlobe_CMD_MEASUREAREA:
			pNewCommand= new CQGlobe_MeasureAreaCmd(pCommandDlg);
			break;
		case eQGlobe_CMD_DRAW_OBJECT: //asd 14.4.11
			pNewCommand = new CQGlobe_DrawObjectCmd(pCommandDlg);
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

bool CQGlobe_CommandMgr::IsEditing()
{
	if(m_pEditCmd)
		return true;
	return false;
}

CQGlobe_Command* CQGlobe_CommandMgr::GetCurrentCmd()
{
	if(m_pEditCmd)
		return m_pEditCmd;
	return m_pCameraCmd;
}

bool CQGlobe_CommandMgr::OnMousePress(CursorState* mouseEvent)
{
	SetCursorPt(mouseEvent);

	bool ret = false;

	m_cOldPressCursor = *mouseEvent;
	m_blFirstMove = true;

	if(m_pEditCmd){
		ret = m_pEditCmd->OnMousePress(mouseEvent);
		if(mouseEvent->buttons & MBS_MID && m_pEditCmd->GetCmdType() != eQGlobe_CMD_SIMULATION)
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

bool CQGlobe_CommandMgr::OnMouseMove(CursorState* mouseEvent)
{
	SetCursorPt(mouseEvent);

	bool ret = false;

	if(m_pEditCmd){
		ret = m_pEditCmd->OnMouseMove(mouseEvent);
		if(mouseEvent->buttons & MBS_MID && m_pEditCmd->GetCmdType() != eQGlobe_CMD_SIMULATION)
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

bool CQGlobe_CommandMgr::OnMouseRelease(CursorState* mouseEvent)
{
	SetCursorPt(mouseEvent);
	bool ret = false;

	m_blFirstMove = false;

	if(m_pEditCmd)
	{
		ret = m_pEditCmd->OnMouseRelease(mouseEvent);
		if(mouseEvent->buttons & MBS_MID && m_pEditCmd->GetCmdType() != eQGlobe_CMD_SIMULATION)
			ret = false;
	}

	if(!ret)
	{
		ret = m_pCameraCmd->OnMouseRelease(mouseEvent);
		if(m_pEditCmd)
			m_pEditCmd->SetCursorType(m_pCameraCmd->GetCursorType());
	}

	if(m_pEditCmd && m_pEditCmd->GetCmdType() != eQGlobe_CMD_SIMULATION)
		GetCameraController()->EndAnimation();

	OnCheckPlaceMarkContent();//2014 2 6 by ugi

	return ret;
}

bool CQGlobe_CommandMgr::OnMouseDblClick(CursorState* mouseEvent)
{
	Config* conf=g_app->GetConfig();

	if(conf->m_IsSelectObject)
		if( OnCheckSelectGeo3dObject())
			return false;

	int nFocusID;
	E_QGlobe_SUBDATA_TYPE type;
	if(GetCurrentCmd()->GetCmdType() == eQGlobe_CMD_DRAW_OBJECT) { //asd 14.4.11
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

bool CQGlobe_CommandMgr::OnMouseWheelEvent(CursorState* mouseEvent)
{
	SetCursorPt(mouseEvent);
	return GetCurrentCmd()->OnMouseWheelEvent(mouseEvent);
}

bool CQGlobe_CommandMgr::OnMouseClickEvent(CursorState* mouseEvent)
{
	return GetCurrentCmd()->OnMouseClickEvent(mouseEvent);
}

void CQGlobe_CommandMgr::OnKeyPressEvent(KeyState keyState)
{
	return GetCurrentCmd()->OnKeyPressEvent(keyState);
}

void CQGlobe_CommandMgr::CancelEditCmd()
{
	if(m_pEditCmd)
	{
		delete m_pEditCmd;
		m_pEditCmd = NULL;
	}
}

MOUSE_POSITION	CQGlobe_CommandMgr::GetCursorPt()
{
	MOUSE_POSITION pos;
	m_pCameraController->GetCursorPos(pos);
	return pos;
}

void CQGlobe_CommandMgr::SetCursorPt(CursorState* mouseEvent)
{
	m_pCameraController->SetCursorPt(mouseEvent);
}

void CQGlobe_CommandMgr::OnCheckSelectObject(bool blFocus)
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

	pGISDoc = qglobe_GetGISDoc();

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

//������ 3d�ﹶ���Ⱑ ��˺�� ���� ���� �к�˾ ���޼Ѱ� true�� ������.
//������ �ﹶ���Ⱑ ��˺�� false�� ������.

#include "QGlobe_SimpleBuildingCache.h"

bool CQGlobe_CommandMgr::OnCheckSelectGeo3dObject()
{
	MOUSE_POSITION pos;

	// get mouse position
	pos = m_pCameraCmd->GetCurrentLocation();

	const QGlobe_LOGIC_TILE *ptile;
	QGlobe_TILE* meshtile;
	SAnimatedMesh* meshs;
	SGeoMesh* mesh;
	
	CQGlobe_Matrix4 transform;
	const QGlobe_LOGIC_TILE_ARR *render_tile_list= qglobe_GetRenderTiles();

	for(int i=0;i<render_tile_list->nCount;i++)
	{
		ptile=render_tile_list->pData+i;//i���� �㶮����˾ �в���.

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
			QGlobe_TILE *polyTile=ptile->sArrPolygonTile[j];

			pSimpleBuildingArr=g_pGDMDataMgr->m_pSimpleBuildingCache->GetBuildingArray(polyTile);

			if(!pSimpleBuildingArr)		continue;

			for(unsigned int k=0;k<pSimpleBuildingArr->Count();k++)
			{
				SimpleBuilding* psimplebuilding=(SimpleBuilding*)pSimpleBuildingArr->GetBuilding(k);

				/*if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tX < ptile->minLongitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tY < ptile->minLatitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tX > ptile->maxLongitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tY > ptile->maxLatitude) continue;*/

				CQGlobe_Rect2D screenrect;

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


void CQGlobe_CommandMgr::OnStartAnimation(QGlobe_ANIMATION_TYPE aniType)
{
	GetCurrentCmd()->OnStartAnimation(aniType);
}

void CQGlobe_CommandMgr::OnNotifyEvent(EQGlobe_RENDER_NOTIFY_TYPE nType, int param1, int param2 , int param3)
{
	switch(nType)
	{
	case QGlobe_RENDER_NITIFY_START_ANIMATION:
		GetCurrentCmd()->OnStartAnimation((QGlobe_ANIMATION_TYPE)param1);
		break;
	case QGlobe_RENDER_NITIFY_END_ANIMATION:
		GetCurrentCmd()->OnEndAnimation((QGlobe_ANIMATION_TYPE)param1);
		break;
	case QGlobe_RENDER_NITIFY_CAMERA_CHANGED:
		{
			GetCurrentCmd()->OnCameraChanged();
			break;
		}
	}
}

//2014 2 7 by ugi
void CQGlobe_CommandMgr::OnCheckPlaceMarkContent()
{
	MOUSE_POSITION pos;
	CGIS_Doc *pGISDoc;
	CGIS_Node *pNode;

	// check edit state
	if(m_pEditCmd)
		return;

	// get mouse position
	pos = m_pCameraCmd->GetCurrentLocation();

	pGISDoc = qglobe_GetGISDoc();

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

void CQGlobe_CommandMgr::OnCheckSelect3dObject()
{
	if(g_app->GetConfig()->m_showObjectAdd)
		return;

	g_Focused3dObjectID=-1;

    MOUSE_POSITION pos;

	// get mouse position
	pos = m_pCameraCmd->GetCurrentLocation();

	const QGlobe_LOGIC_TILE *ptile;
	QGlobe_TILE* meshtile;
	SAnimatedMesh* meshs;
	SGeoMesh* mesh;
	
	CQGlobe_Matrix4 transform;
	const QGlobe_LOGIC_TILE_ARR *render_tile_list= qglobe_GetRenderTiles();

	for(int i=0;i<render_tile_list->nCount;i++)
	{
		ptile=render_tile_list->pData+i;//i���� �㶮����˾ �в���.

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
			QGlobe_TILE *polyTile=ptile->sArrPolygonTile[j];

			pSimpleBuildingArr=g_pGDMDataMgr->m_pSimpleBuildingCache->GetBuildingArray(polyTile);

			if(!pSimpleBuildingArr)		continue;

			for(unsigned int k=0;k<pSimpleBuildingArr->Count();k++)
			{
				SimpleBuilding* psimplebuilding=(SimpleBuilding*)pSimpleBuildingArr->GetBuilding(k);

				/*if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tX < ptile->minLongitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tY < ptile->minLatitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tX > ptile->maxLongitude) continue;
				if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tY > ptile->maxLatitude) continue;*/

				CQGlobe_Rect2D screenrect;

				psimplebuilding->CalculateScreenRect(screenrect);

				if(screenrect.isPointInside(pos.x,pos.y))				 
				{
					g_Focused3dObjectID=psimplebuilding->GetID();
				}  //screenrect.isPointInside
			}
		}
	}
}
