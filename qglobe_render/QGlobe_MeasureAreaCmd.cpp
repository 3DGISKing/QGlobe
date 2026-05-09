#include "QGlobe_MeasureAreaCmd.h"
#include "GIS_Area.h"
#include "GIS_Doc.h"
#include "QGlobe_DataMgr.h"
#include "QGlobe_Camera.h"

CQGlobe_MeasureAreaCmd::CQGlobe_MeasureAreaCmd(CQGlobe_CommandDlg *pCmdDlg): CQGlobe_Command(pCmdDlg)
{
	m_pAreaNode=NULL;
	m_pPrevAreaNode=NULL;
	m_eCmdType=eQGlobe_CMD_MEASUREAREA;
	m_eCursorType = CURSOR_SHAPE_RULER;
	m_MoveDirection=270;
	m_MoveVelocity=0.0;
	m_GrowVelocity=0.0;
	m_blPress	= false;
	m_blActive  = false;
	m_blTrack   = false;
	m_nCurTrackIndex = -1;
	m_dTopHeight=0.0;


	Init();
}

CQGlobe_MeasureAreaCmd::~CQGlobe_MeasureAreaCmd()
{

}

void CQGlobe_MeasureAreaCmd::OnCancel()
{
	qglobe_GetGISDoc()->SetSelectNode(NULL);
	qglobe_GetGISDoc()->RemoveNode(m_pAreaNode);
	delete m_pAreaNode;

	if(m_pPrevAreaNode)
	{
		qglobe_GetGISDoc()->RemoveNode(m_pPrevAreaNode);
		delete m_pPrevAreaNode;
	}

	CQGlobe_Command::OnCancel();
}

void CQGlobe_MeasureAreaCmd::Init()
{
	if(m_pAreaNode)
		m_pPrevAreaNode=m_pAreaNode;

	m_pAreaNode=new CGIS_Area();

	m_pAreaNode->m_sBrush.color	= 0x77ffff00; // yellow
	qglobe_GetGISDoc()->AddNode(m_pAreaNode);
	qglobe_GetGISDoc()->SetSelectNode(m_pAreaNode);
}


void CQGlobe_MeasureAreaCmd::OnEnd()
{

	CQGlobe_Command::OnEnd();
}

void CQGlobe_MeasureAreaCmd::Clear()
{
	qglobe_GetGISDoc()->SetSelectNode(NULL);
	qglobe_GetGISDoc()->RemoveNode(m_pAreaNode);
	delete m_pAreaNode;
	m_pAreaNode=NULL;
	Init();
}

bool CQGlobe_MeasureAreaCmd::OnMousePress(CursorState* mouseEvent)
{
	if(m_pPrevAreaNode)
	{
		qglobe_GetGISDoc()->RemoveNode(m_pPrevAreaNode);
		delete m_pPrevAreaNode;
		m_pPrevAreaNode=NULL;
	}

	int nTrackId;
	CQGlobe_Location3D trackPt;

	if(m_pAreaNode == NULL)
		return false;

	m_blPress	= true;
	m_blTrack = false;
	m_blActive = true;

	nTrackId = GetSelectTrackIndex(mouseEvent);

	if(nTrackId >= 0)
	{
		m_nCurTrackIndex = nTrackId;
		m_blTrack = true;
		m_pAreaNode->Get_TrackPt(nTrackId, &trackPt);
		m_dTopHeight = trackPt.m_tZ;
	}
	
	m_cPressCursor = *mouseEvent;

	return true;
}

int	CQGlobe_MeasureAreaCmd::GetSelectTrackIndex(CursorState* mouseEvent)
{
	CQGlobe_Location3D pt;
	CQGlobe_Point3DF pos;
	MOUSE_POSITION	mouse_pos = GetCurrentLocation();

	if(m_pAreaNode == NULL)
		return -1;

	unsigned int id, count = m_pAreaNode->Get_TrackCount();
	for(id = 0; id < count; id++)
	{
		m_pAreaNode->Get_TrackPt(id, &pt);
		qglobe_LocationToScreen(pt.m_tX, pt.m_tY, pt.m_tZ, &pos);
		if(abs(mouseEvent->x - pos.m_tX) < mouse_pos.rangeInScreen && 
			abs(mouseEvent->y - pos.m_tY) < mouse_pos.rangeInScreen)
			return id;
	}

	return -1;
}

bool CQGlobe_MeasureAreaCmd::OnMouseMove(CursorState* mouseEvent)
{
	int nTrackId;

	if(m_blPress && !m_blTrack) // moving camera
	{
		if(abs(m_cPressCursor.x - mouseEvent->x) + abs(m_cPressCursor.y - mouseEvent->y) < 10)
			return true;
		m_blActive = false;
		return false;
	}

	if(m_pAreaNode == NULL)
		return false;

	if(m_blPress) // move track pt
	{
		CQGlobe_Location3D location;
		m_eCursorType = CURSOR_SHAPE_POINTING;

		if(m_dTopHeight == 0) // bottom point
		{
			location = GetLocation(mouseEvent);
		}
		else // top point
		{
			qglobe_GetDataMgr()->m_pCamera->GetHitLocation(mouseEvent->x, mouseEvent->y, 
				m_dTopHeight + EARTH_RADIUS, &location);
		}
		m_pAreaNode->Set_TrackPt(m_nCurTrackIndex, &location);
		m_pAreaNode->Refresh();
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


bool CQGlobe_MeasureAreaCmd::OnMouseRelease(CursorState* mouseEvent)
{
	CQGlobe_Location3D location;

	m_blPress = false;
	m_eCursorType = CURSOR_SHAPE_RULER;

	if(!m_blActive)
		return false;

	if(m_pAreaNode == NULL)
		return false;

	location = GetLocation(mouseEvent);

	if(m_blTrack) // move track pt
	{
		if(m_dTopHeight == 0) // bottom point
			location = GetLocation(mouseEvent);
		else // top point
		{
			qglobe_GetDataMgr()->m_pCamera->GetHitLocation(mouseEvent->x, mouseEvent->y, 
				m_dTopHeight + EARTH_RADIUS, &location);
		}
		m_pAreaNode->Set_TrackPt(m_nCurTrackIndex, &location);
		m_pAreaNode->Refresh();
	}
	else // add pt
	{
		location = GetLocation(mouseEvent);

		if(mouseEvent->buttons & MBS_LEFT)
		{
			m_nCurTrackIndex++;
			m_pAreaNode->InsertPoint(m_nCurTrackIndex, location);	// add  point
		}
		else if(mouseEvent->buttons & MBS_RIGHT)
		{
			m_pAreaNode->RemovePoint(m_nCurTrackIndex);
			m_nCurTrackIndex--;
			if(m_nCurTrackIndex < 0)
				m_nCurTrackIndex = m_pAreaNode->GetCount() - 1;
		}
		m_pAreaNode->Set_TrackID(m_nCurTrackIndex);
	}

	m_pAreaNode->Refresh();
	UpdateDailog();

	m_blActive = false;
	return true;
}

double CQGlobe_MeasureAreaCmd::GetArea()
{
	Q_ASSERT(m_pAreaNode!=NULL);
	return m_pAreaNode->m_dArea;
}

void CQGlobe_MeasureAreaCmd::OnKeyPressEvent(KeyState keyState)
{
	CQGlobe_Command::OnKeyPressEvent(keyState);
}

void CQGlobe_MeasureAreaCmd::OnSimulation()
{
	double centerx	= (m_pAreaNode->m_cBound.m_cMinPt.m_tX + m_pAreaNode->m_cBound.m_cMaxPt.m_tX) / 2;
	double centery	= (m_pAreaNode->m_cBound.m_cMinPt.m_tY + m_pAreaNode->m_cBound.m_cMaxPt.m_tY) / 2;

	for(int i=0;i<m_pAreaNode->m_PtArr.size();i++)
	{
		double x=m_pAreaNode->m_PtArr[i].m_tX;
		double y=m_pAreaNode->m_PtArr[i].m_tY;
 
		//first apply scale
		
		x=x-centerx;
		y=y-centery;

		double d=sqrt(x*x+y*y);
		x=x*(1.0+m_GrowVelocity/d);
		y=y*(1.0+m_GrowVelocity/d);

		x=x+centerx;
		y=y+centery;

		//apply translation 
		
		x=x+m_MoveVelocity*cos(qglobe_DEGTORAD*m_MoveDirection);
		y=y+m_MoveVelocity*sin(qglobe_DEGTORAD*m_MoveDirection);	
		m_pAreaNode->m_PtArr[i].m_tX=x;
		m_pAreaNode->m_PtArr[i].m_tY=y;
	}
	
	m_pAreaNode->Refresh();
	UpdateDailog();
}
