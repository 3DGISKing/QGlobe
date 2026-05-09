#include "stable.h"
#include "QGlobe_CameraCmd.h"
#include "QGlobe_CommandMgr.h"
#include "cameracontroller.h"

CQGlobe_CameraCmd::CQGlobe_CameraCmd(CQGlobe_CommandDlg *pCmdDlg) : CQGlobe_Command(pCmdDlg)
{
	m_eCmdType = eQGlobe_CMD_CAMERA;
}

CQGlobe_CameraCmd::~CQGlobe_CameraCmd(void)
{
}

bool CQGlobe_CameraCmd::OnMousePress(CursorState* mouseEvent)
{
	if(mouseEvent->keys & KS_SPACE)
		return false;
	m_pCommandMgr->GetCameraController()->MousePress(mouseEvent);
	m_eCursorType = GetCursorType();
	return true;
}

bool CQGlobe_CameraCmd::OnMouseMove(CursorState* mouseEvent)
{
	if(m_pCommandMgr->GetCameraController()->GetMode() == 0)
	{
		return false;
	}
	m_pCommandMgr->GetCameraController()->MouseMove(mouseEvent);
	m_eCursorType = GetCursorType();
	return true;
}

bool CQGlobe_CameraCmd::OnMouseRelease(CursorState* mouseEvent)
{
	if(m_pCommandMgr->GetCameraController()->GetMode() == 0)
	{
		return false;
	}
	m_pCommandMgr->GetCameraController()->MouseRelease(mouseEvent);
	m_eCursorType = GetCursorType();
	return true;
}

bool CQGlobe_CameraCmd::OnMouseDblClick(CursorState* mouseEvent)
{
	if(mouseEvent->keys & KS_SPACE)
		return false;
	m_pCommandMgr->GetCameraController()->MouseDblClick(mouseEvent);
	return true;
}

void CQGlobe_CameraCmd::OnKeyPressEvent(KeyState keyState)
{
	CQGlobe_Command::OnKeyPressEvent(keyState);
}

CURSOR_SHAPE CQGlobe_CameraCmd::GetCursorType()
{
	int nMode = m_pCommandMgr->GetCameraController()->GetMode();
	switch(nMode)
	{
	case 1:
		return CURSOR_SHAPE_SELECTED;
	case 2:
		return CURSOR_SHAPE_POINTMARK;
	default:
		return CURSOR_SHAPE_NORMAL;
	}
}
