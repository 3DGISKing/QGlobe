#include "stable.h"
#include "QGlobe_PointCmd.h"
#include "GIS_Doc.h"

CQGlobe_PointCmd::CQGlobe_PointCmd(CQGlobe_CommandDlg *pCmdDlg) : CQGlobe_Command(pCmdDlg)
{
	m_eCursorType = CURSOR_SHAPE_POINTING;
	m_pNode = NULL;
	m_pOrgNode = NULL;
	m_blPress = false;
	m_blTrack = false;	
	m_blCenter = false;
}

CQGlobe_PointCmd::~CQGlobe_PointCmd(void)
{
	if(m_pOrgNode)
	{
		delete m_pOrgNode;
		m_pOrgNode = NULL;
	}
}

void CQGlobe_PointCmd::Init(CGIS_Node *pNode, bool blNew)
{
	if(pNode == NULL)
		return;

	if(m_pOrgNode)
		delete m_pOrgNode;
	m_pOrgNode = NULL;

	if(!blNew)
		m_pOrgNode = (CGIS_Point*)pNode->Clone();
	else
		qglobe_GetGISDoc()->AddNode(pNode);

	m_pNode = (CGIS_Point*)pNode;

	qglobe_GetGISDoc()->SetSelectNode(m_pNode);
}

void CQGlobe_PointCmd::OnEnd()
{
	qglobe_GetGISDoc()->SetSelectNode(NULL);
	CQGlobe_Command::OnEnd();
}

void CQGlobe_PointCmd::OnCancel()
{
	if(m_pNode == NULL)
	{
		CQGlobe_Command::OnCancel();
		return;
	}

	if(m_pOrgNode)  // case new
		*m_pNode = *m_pOrgNode;
	else			// case modify
		qglobe_GetGISDoc()->RemoveNode(m_pNode);

	qglobe_GetGISDoc()->SetSelectNode(NULL);

	CQGlobe_Command::OnCancel();
}

bool CQGlobe_PointCmd::OnMousePress(CursorState* mouseEvent)
{
	if(m_blCenter)
	{
//		UpdateDailog();
		return false;
	}

	MOUSE_POSITION pos = GetCurrentLocation();

	if(m_pNode == NULL)
		return false;

	m_blPress = true;

	if(m_pNode->IsContain(pos))
	{
		m_blTrack = true;
		m_eCursorType = CURSOR_SHAPE_POINTING;
		return true;
	}

	return false;
}

bool CQGlobe_PointCmd::OnMouseMove(CursorState* mouseEvent)
{
	if(m_blCenter)
	{
//		UpdateDailog();
		return false;
	}
	CQGlobe_Location3D pos;

	if(m_pNode == NULL)
		return false;

	if(m_blTrack)
	{
		pos = GetLocation(mouseEvent);
		m_pNode->SetPt(pos.m_tX, pos.m_tY, pos.m_tZ);
		UpdateDailog();
		return true;
	}
	else if(!m_blPress)
	{
		MOUSE_POSITION mPos = GetCurrentLocation();
		if(m_pNode->IsContain(mPos))
			m_eCursorType = CURSOR_SHAPE_POINTING;
		else
			m_eCursorType = CURSOR_SHAPE_NORMAL;
		return true;
	}

	return false;
}

bool CQGlobe_PointCmd::OnMouseRelease(CursorState* mouseEvent)
{
	if(m_blCenter)
	{
//		UpdateDailog();
		return false;
	}

	CQGlobe_Location3D pos;

	if(m_pNode == NULL)
		return false;

	m_blPress = false;

	if(m_blTrack)
	{
		pos = GetLocation(mouseEvent);
		m_pNode->SetPt(pos.m_tX, pos.m_tY, pos.m_tZ);
		UpdateDailog();
		m_blTrack= false;
		return true;
	}

	return false;
}

void CQGlobe_PointCmd::OnCameraChanged()
{
	if(m_blCenter)
	{
		UpdateDailog();
	}
}