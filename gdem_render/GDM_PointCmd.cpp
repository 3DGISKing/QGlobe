#include "stable.h"
#include "GDM_PointCmd.h"
#include "GIS_Doc.h"

CGDM_PointCmd::CGDM_PointCmd(CGDM_CommandDlg *pCmdDlg) : CGDM_Command(pCmdDlg)
{
	m_eCursorType = CURSOR_SHAPE_POINTING;
	m_pNode = NULL;
	m_pOrgNode = NULL;
	m_blPress = false;
	m_blTrack = false;	
	m_blCenter = false;
}

CGDM_PointCmd::~CGDM_PointCmd(void)
{
	if(m_pOrgNode)
	{
		delete m_pOrgNode;
		m_pOrgNode = NULL;
	}
}

void CGDM_PointCmd::Init(CGIS_Node *pNode, bool blNew)
{
	if(pNode == NULL)
		return;

	if(m_pOrgNode)
		delete m_pOrgNode;
	m_pOrgNode = NULL;

	if(!blNew)
		m_pOrgNode = (CGIS_Point*)pNode->Clone();
	else
		gdm_GetGISDoc()->AddNode(pNode);

	m_pNode = (CGIS_Point*)pNode;

	gdm_GetGISDoc()->SetSelectNode(m_pNode);
}

void CGDM_PointCmd::OnEnd()
{
	gdm_GetGISDoc()->SetSelectNode(NULL);
	CGDM_Command::OnEnd();
}

void CGDM_PointCmd::OnCancel()
{
	if(m_pNode == NULL)
	{
		CGDM_Command::OnCancel();
		return;
	}

	if(m_pOrgNode)  // case new
		*m_pNode = *m_pOrgNode;
	else			// case modify
		gdm_GetGISDoc()->RemoveNode(m_pNode);

	gdm_GetGISDoc()->SetSelectNode(NULL);

	CGDM_Command::OnCancel();
}

bool CGDM_PointCmd::OnMousePress(CursorState* mouseEvent)
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

bool CGDM_PointCmd::OnMouseMove(CursorState* mouseEvent)
{
	if(m_blCenter)
	{
//		UpdateDailog();
		return false;
	}
	CGDM_Location3D pos;

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

bool CGDM_PointCmd::OnMouseRelease(CursorState* mouseEvent)
{
	if(m_blCenter)
	{
//		UpdateDailog();
		return false;
	}

	CGDM_Location3D pos;

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

void CGDM_PointCmd::OnCameraChanged()
{
	if(m_blCenter)
	{
		UpdateDailog();
	}
}