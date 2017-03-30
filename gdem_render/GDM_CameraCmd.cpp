#include "stable.h"
#include "GDM_CameraCmd.h"
#include "GDM_CommandMgr.h"
#include "cameracontroller.h"

CGDM_CameraCmd::CGDM_CameraCmd(CGDM_CommandDlg *pCmdDlg) : CGDM_Command(pCmdDlg)
{
	m_eCmdType = eGDM_CMD_CAMERA;
}

CGDM_CameraCmd::~CGDM_CameraCmd(void)
{
}

bool CGDM_CameraCmd::OnMousePress(CursorState* mouseEvent)
{
	if(mouseEvent->keys & KS_SPACE)
		return false;
	m_pCommandMgr->GetCameraController()->MousePress(mouseEvent);
	m_eCursorType = GetCursorType();
	return true;
}

bool CGDM_CameraCmd::OnMouseMove(CursorState* mouseEvent)
{
	if(m_pCommandMgr->GetCameraController()->GetMode() == 0)
	{
		return false;
	}
	m_pCommandMgr->GetCameraController()->MouseMove(mouseEvent);
	m_eCursorType = GetCursorType();
	return true;
}

bool CGDM_CameraCmd::OnMouseRelease(CursorState* mouseEvent)
{
	if(m_pCommandMgr->GetCameraController()->GetMode() == 0)
	{
		return false;
	}
	m_pCommandMgr->GetCameraController()->MouseRelease(mouseEvent);
	m_eCursorType = GetCursorType();
	return true;
}

bool CGDM_CameraCmd::OnMouseDblClick(CursorState* mouseEvent)
{
	if(mouseEvent->keys & KS_SPACE)
		return false;
	m_pCommandMgr->GetCameraController()->MouseDblClick(mouseEvent);
	return true;
}

void CGDM_CameraCmd::OnKeyPressEvent(KeyState keyState)
{
	CGDM_Command::OnKeyPressEvent(keyState);
}

CURSOR_SHAPE CGDM_CameraCmd::GetCursorType()
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
