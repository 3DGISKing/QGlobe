#include "stable.h"
#include "GDM_RulerCmd.h"
#include "GDM_DataMgr.h"
#include "GDM_Camera.h"
#include "GDM_CommandMgr.h"
#include "cameracontroller.h"
#include "GIS_Doc.h"
#include <stdio.h>

CGDM_RulerCmd::CGDM_RulerCmd(CGDM_CommandDlg *pCmdDlg) : CGDM_Command(pCmdDlg)
{
	m_pRulerNode = NULL;
	m_nStep = 0;
	Init();
}

CGDM_RulerCmd::~CGDM_RulerCmd(void)
{
}

void CGDM_RulerCmd::Init(void)
{
	m_eCmdType = eGDM_CMD_RULER;
	m_pRulerNode = new CGIS_Ruler();
	m_eCursorType = CURSOR_SHAPE_RULER;
	gdm_GetGISDoc()->AddNode(m_pRulerNode);
	gdm_GetGISDoc()->SetSelectNode(m_pRulerNode);
	m_nStep = 0;
	m_blPress	= false;
}

void CGDM_RulerCmd::OnCancel()
{
	gdm_GetGISDoc()->RemoveNode(m_pRulerNode);

	CGDM_Command::OnCancel();
}

int	CGDM_RulerCmd::GetSelectTrackIndex(CursorState* mouseEvent)
{
	CGDM_Location3D pt;
	CGDM_Point3DF pos;
	MOUSE_POSITION	mouse_pos = GetCurrentLocation();

	if(m_pRulerNode == NULL)
	{
		return -1;
	}

	unsigned int id, count = m_pRulerNode->Get_TrackCount();
	for(id = 0; id < count; id++)
	{
		m_pRulerNode->Get_TrackPt(id, &pt);
		gdm_LocationToScreen(pt.m_tX, pt.m_tY, pt.m_tZ, &pos);
		if(abs(mouseEvent->x - pos.m_tX) < mouse_pos.rangeInScreen && 
			abs(mouseEvent->y - pos.m_tY) < mouse_pos.rangeInScreen)
			return id;
	}

	return -1;
}

bool CGDM_RulerCmd::OnMousePress(CursorState* mouseEvent)
{
	int nTrackId;

	if(m_pRulerNode == NULL)
	{
		return false;
	}

	m_blPress	= true;
	m_blTrack = false;
	m_blActive = true;

	nTrackId = GetSelectTrackIndex(mouseEvent);

	if(m_blLine && nTrackId == 1)
		nTrackId = -1;

	if(nTrackId >= 0)
	{
		m_nCurTrackIndex = nTrackId;
		m_blTrack = true;
	}

	m_cPressCursor = *mouseEvent;

	return true;
}

void CGDM_RulerCmd::SetMeasureMode(MeasureInfo measureInfo) 
{
	if (m_pRulerNode)
		m_pRulerNode->SetMeasureMode(measureInfo);
}

bool CGDM_RulerCmd::OnMouseMove(CursorState* mouseEvent)
{
	int nTrackId;
	CGDM_Location3D location;

	if(m_blPress && (!m_blTrack)) // moving camera
	{
		if(abs(m_cPressCursor.x - mouseEvent->x) + abs(m_cPressCursor.y - mouseEvent->y) < 10)
			return true;
		m_blActive = false;
		return false;
	}

	if(m_pRulerNode == NULL)
	{
		return false;
	}
	
	location = GetLocation(mouseEvent);

	if(m_blPress) // move track pt
	{
		m_eCursorType = CURSOR_SHAPE_POINTING;
		m_pRulerNode->Set_TrackPt(m_nCurTrackIndex, &location);
		m_pRulerNode->Refresh();
		UpdateDailog();
	}
	else if(!(m_blLine && m_blMouseCheck)) // focus track pt
	{
		nTrackId = GetSelectTrackIndex(mouseEvent);
		if(nTrackId >= 0)
			m_eCursorType = CURSOR_SHAPE_POINTING;
		else
			m_eCursorType = CURSOR_SHAPE_RULER;

	}

	if(m_nStep == 1 && m_blLine && m_blMouseCheck)
	{
		m_pRulerNode->SetPoint(location, m_nStep);	// add first point
		m_pRulerNode->Refresh();

		UpdateDailog();
	}

	return true;
}

bool CGDM_RulerCmd::OnMouseRelease(CursorState* mouseEvent)
{
	CGDM_Location3D location;

	m_blPress = false;
	m_eCursorType = CURSOR_SHAPE_RULER;

	if(!m_blActive)
		return false;

	if(m_pRulerNode == NULL)
	{
		return false;
	}

	location = GetLocation(mouseEvent);

	if(m_blTrack) // move track pt
	{
		m_pRulerNode->Set_TrackPt(m_nCurTrackIndex, &location);
		return true;
	}


	if(m_blLine)
	{
		if(!(mouseEvent->buttons & MBS_LEFT))
			return false;

		if(m_blMouseCheck)
		{
			if(m_nStep == 2 || m_nStep == 0)
			{
				m_nStep = 0;
				m_pRulerNode->Clear();
				m_pRulerNode->AddPoint(location);	// add first point
				m_pRulerNode->AddPoint(location);	// add scond point
			}
			m_nStep++;
		}
		else
		{
			if(m_pRulerNode->GetCount() == 2)
			{
				m_nStep = 0;
				m_pRulerNode->Clear();
			}
			m_pRulerNode->AddPoint(location);	// add first point
			m_nStep++;
		}
	}
	else // add pt
	{
		if(mouseEvent->buttons & MBS_LEFT)
		{
			m_nCurTrackIndex++;
			m_pRulerNode->InsertPoint(m_nCurTrackIndex, location);	// add  point
		}
		else if(mouseEvent->buttons & MBS_RIGHT)
		{
			m_pRulerNode->RemovePoint(m_nCurTrackIndex);
			m_nCurTrackIndex--;
			if(m_nCurTrackIndex < 0)
				m_nCurTrackIndex = m_pRulerNode->GetCount() - 1;
		}
		m_pRulerNode->Set_TrackID(m_nCurTrackIndex);
	}

	m_pRulerNode->Refresh();
	UpdateDailog();

	m_blActive = false;
	return true;
}

void CGDM_RulerCmd::Clear ()
{
	gdm_GetGISDoc()->DestroyNodeFromType(E_GISNODE_TYPE_RULER);
	Init();
}

double CGDM_RulerCmd::CountAllLength (bool blLine)
{
	if (m_pRulerNode == NULL)
		return 0;

	return m_pRulerNode->m_dLength;
}

double CGDM_RulerCmd::CountAngle (bool blLine)
{
	double	dblAngle = 0;

	if (m_pRulerNode == NULL)
		return 0;
	if (blLine && m_pRulerNode->GetCount() == 2)
	{
		double	dLatitude = m_pRulerNode->m_PtArr[1].m_tY - m_pRulerNode->m_PtArr[0].m_tY;
		double	dLongitude = m_pRulerNode->m_PtArr[1].m_tX - m_pRulerNode->m_PtArr[0].m_tX;

		if(gdm_iszero(dLongitude))
			dblAngle = 0;
		else
			dblAngle = -atan (dLatitude / dLongitude);
		dblAngle += gdm_HALF_PI;
		if (dLongitude < 0)
			dblAngle += gdm_PI;
		dblAngle *= gdm_RADTODEG;
	}

	return	dblAngle;
}

void CGDM_RulerCmd::OnKeyPressEvent(KeyState keyState)
{
	CGDM_Command::OnKeyPressEvent(keyState);
}
