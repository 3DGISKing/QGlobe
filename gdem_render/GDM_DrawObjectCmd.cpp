#include "stable.h"
#include "GDM_DrawObjectCmd.h"
#include "GIS_Doc.h"
#include "GDM_CommandMgr.h"
#include "cameracontroller.h"
#include "GDM_Camera.h"
#include "GDM_DataMgr.h"

CGDM_DrawObjectCmd::CGDM_DrawObjectCmd(CGDM_CommandDlg *pCmdDlg) : CGDM_DrawCmd(pCmdDlg)
{
	m_eCmdType = eGDM_CMD_DRAW_OBJECT;
	m_eCursorType = CURSOR_SHAPE_RULER;
	m_pOrgNode	= NULL;
	m_pNode		= NULL;
	m_blPress	= false;
	m_blActive  = false;
	m_drawEnd	= false;
	m_nCurTrackIndex = -1;
}

CGDM_DrawObjectCmd::~CGDM_DrawObjectCmd(void)
{
}

void CGDM_DrawObjectCmd::Init(CGIS_Polygon *pNode, bool blNew)
{
	if(pNode == NULL)
		return;

	if(m_pOrgNode)
		delete m_pOrgNode;
	m_pOrgNode = NULL;

	if(!blNew)
		m_pOrgNode = (CGIS_Polygon*)pNode->Clone();
	else
		gdm_GetGISDoc()->AddNode(pNode);

	m_pNode = (CGIS_Polygon*)pNode;

	gdm_GetGISDoc()->SetSelectNode(m_pNode);
}

void CGDM_DrawObjectCmd::OnCancel()
{
	if(m_pNode == NULL)
	{
		CGDM_Command::OnCancel();
		return;
	}

	if(m_pOrgNode)  // case modify
	{
		m_pNode->CopyFrom(*m_pOrgNode);
		m_pNode->Refresh(false);
	}
	else			// case new
		gdm_GetGISDoc()->RemoveNode(m_pNode);

	gdm_GetGISDoc()->SetSelectNode(NULL);

	CGDM_Command::OnCancel();
}

void CGDM_DrawObjectCmd::OnEnd()
{
	gdm_GetGISDoc()->SetSelectNode(NULL);
	CGDM_Command::OnEnd();
}

void CGDM_DrawObjectCmd::Clear()
{
	gdm_GetGISDoc()->DestroyNodeFromType(E_GISNODE_TYPE_PATH);
}

int	CGDM_DrawObjectCmd::GetSelectTrackIndex(CursorState* mouseEvent)
{
	CGDM_Location3D pt;
	CGDM_Point3DF pos;
	MOUSE_POSITION	mouse_pos = GetCurrentLocation();

	if(m_pNode == NULL)
		return -1;

	unsigned int id, count = m_pNode->Get_TrackCount();
	for(id = 0; id < count; id++)
	{
		m_pNode->Get_TrackPt(id, &pt);
		gdm_LocationToScreen(pt.m_tX, pt.m_tY, pt.m_tZ, &pos);
		if(abs(mouseEvent->x - pos.m_tX) < mouse_pos.rangeInScreen && 
			abs(mouseEvent->y - pos.m_tY) < mouse_pos.rangeInScreen)
			return id;
	}

	return -1;
}


bool CGDM_DrawObjectCmd::OnMousePress(CursorState* mouseEvent)
{
	int nTrackId;
	CGDM_Location3D trackPt;

	if(m_pNode == NULL)
	{
		return false;
	}

	m_blPress	= true;
	m_blTrack = false;
	m_blActive = true;

	nTrackId = GetSelectTrackIndex(mouseEvent);

	if(nTrackId >= 0)
	{
		m_nCurTrackIndex = nTrackId;
		m_blTrack = true;

		if(nTrackId >= m_pNode->GetCount())
		{
			m_pNode->Get_TrackPt(nTrackId, &trackPt);
			m_dTopHeight = trackPt.m_tZ;
		}
		else
			m_dTopHeight = 0;
	} else if(m_shapeType < OBJECT_LINE) {
		m_pNode->Clear();
		m_nCurTrackIndex = -1;
		return true;
	}

	m_cPressCursor = *mouseEvent;

	if(m_shapeType == OBJECT_POLYGON && m_drawEnd) {
		CGDM_Location3D first_pt;
		CGDM_Location3D end_pt;
		m_pNode->Get_TrackPt(0, &first_pt);
		int lastPtId = m_pNode->GetCount() - 1;
		m_pNode->Get_TrackPt(lastPtId, &end_pt);
		if(first_pt == end_pt)
			m_pNode->RemovePoint(lastPtId);
		m_drawEnd = false;
	}

	return true;
}

bool CGDM_DrawObjectCmd::OnMouseMove(CursorState* mouseEvent)
{
	int nTrackId;

	if(m_blPress && !m_blTrack) // moving camera
	{
		if(abs(m_cPressCursor.x - mouseEvent->x) + abs(m_cPressCursor.y - mouseEvent->y) < 10)
			return true;
		m_blActive = false;
		return false;
	}

	if(m_pNode == NULL)
	{
		return false;
	}

	if(m_blPress) // move track pt
	{
		CGDM_Location3D location;
		m_eCursorType = CURSOR_SHAPE_POINTING;
	
		if(m_dTopHeight == 0) // bottom point
		{
			location = GetLocation(mouseEvent);
		}
		else // top point
		{
			gdm_GetDataMgr()->m_pCamera->GetHitLocation(mouseEvent->x, mouseEvent->y, 
				m_dTopHeight + EARTH_RADIUS, &location);
		}
		m_pNode->Set_TrackPt(m_nCurTrackIndex, &location);
		m_pNode->Refresh();
		UpdateDailog();
	}
	else // focus track pt
	{
		nTrackId = GetSelectTrackIndex(mouseEvent);
		if(nTrackId >= 0)
			m_eCursorType = CURSOR_SHAPE_POINTING;
		else
			m_eCursorType = CURSOR_SHAPE_RULER;

	}

	return true;
}

bool CGDM_DrawObjectCmd::OnMouseRelease(CursorState* mouseEvent)
{
	CGDM_Location3D location;

	m_blPress = false;
	m_eCursorType = CURSOR_SHAPE_RULER;

	if(!m_blActive)
		return false;

	if(m_pNode == NULL)
	{
		return false;
	}
	location = GetLocation(mouseEvent);

	if(m_blTrack) // move track pt
	{
		if(m_dTopHeight == 0) // bottom point
		{
			location = GetLocation(mouseEvent);
		}
		else // top point
		{
			gdm_GetDataMgr()->m_pCamera->GetHitLocation(mouseEvent->x, mouseEvent->y, 
				m_dTopHeight + EARTH_RADIUS, &location);
		}
		m_pNode->Set_TrackPt(m_nCurTrackIndex, &location);
		m_pNode->Refresh();
	}
	else // add pt
	{
		location = GetLocation(mouseEvent);

		if(mouseEvent->buttons & MBS_LEFT)
		{
			m_nCurTrackIndex++;
			m_pNode->InsertPoint(m_nCurTrackIndex, location);	// add  point
		}
		else if(mouseEvent->buttons & MBS_RIGHT)
		{
			m_pNode->RemovePoint(m_nCurTrackIndex);
			m_nCurTrackIndex--;
			if(m_nCurTrackIndex < 0)
				m_nCurTrackIndex = m_pNode->GetCount() - 1;
		}
		m_pNode->Set_TrackID(m_nCurTrackIndex);
	}

	m_pNode->Refresh();
	UpdateDailog();

	m_blActive = false;
	return true;
}

bool CGDM_DrawObjectCmd::OnMouseDblClick(CursorState* mouseEvent)
{
	if(m_shapeType < OBJECT_POLYGON) return false;
	CGDM_Location3D firstPt;
	m_pNode->Get_TrackPt(0, &firstPt);
	m_pNode->AddPoint(firstPt);
	m_drawEnd = true;
	return true;
}