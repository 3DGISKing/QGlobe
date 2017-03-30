
#include "GDM_SimCmd.h"
#include "GDM_Simulator.h"
#include "rendersrv.h"
#include "GDM_CommandMgr.h"
#include "cameracontroller.h"
#include "gdemclient.h"


CGDM_SimCmd::CGDM_SimCmd(CGDM_CommandDlg *pCmdDlg) : CGDM_Command(pCmdDlg)
{
	m_eCmdType = eGDM_CMD_SIMULATION;
	gdm_CameraGetCoord(g_pRender, &m_orgCoord);
	g_app->ResetRenderKeys();// Key상태를 초기화 한다.
}

CGDM_SimCmd::~CGDM_SimCmd(void)
{
	m_pSimulator = NULL;
	g_app->ResetRenderKeys();// Key상태를 초기화 한다.
}

void
CGDM_SimCmd::ChangeCursor()
{
	GDM_SIM_STATUS simStatus;
	m_pSimulator->GetSimulationStatus( &simStatus );
	if ( simStatus.m_type == SIM_TYPE_FLIGHT )
	{
		if (simStatus.m_useMouse)
			m_eCursorType = CURSOR_SHAPE_CROSS;
		else	
			m_eCursorType = CURSOR_SHAPE_NORMAL;//CURSOR_SHAPE_NO;
	}
}

bool	
CGDM_SimCmd::OnMousePress(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	bool ret = m_pSimulator->OnMousePress(mouseEvent);
	ChangeCursor();
	return ret;
}

bool	
CGDM_SimCmd::OnMouseMove(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseMove(mouseEvent);
}

bool	
CGDM_SimCmd::OnMouseRelease(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseRelease(mouseEvent);
}

bool	
CGDM_SimCmd::OnMouseDblClick(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseDblClick(mouseEvent);

}

bool	
CGDM_SimCmd::OnMouseWheelEvent(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseWheelEvent(mouseEvent);

}

bool	
CGDM_SimCmd::OnMouseClickEvent(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseClickEvent(mouseEvent);

}

void	
CGDM_SimCmd::OnKeyPressEvent(KeyState keyState)
{
	if (!m_pSimulator)
		return ;
	m_pSimulator->OnKeyPressEvent(keyState);

}

void			
CGDM_SimCmd::SetSimulator(CGDM_Simulator* a_pSimulator)
{
	m_pSimulator = a_pSimulator;
	ChangeCursor();
}

void CGDM_SimCmd::OnEnd()
{
	GDM_SIM_STATUS status;
	m_pSimulator->GetSimulationStatus(&status);
	if(status.m_autoMove)
	{
		m_pCommandMgr->GetCameraController()->StopView();
		gdm_CameraSetCoord(g_pRender, m_orgCoord);
	}
	else
	{
		GDM_CAMERA_INFO cameraInfo;
		gdm_CameraGetInfo(g_pRender, &cameraInfo, 1);
		cameraInfo.m_yaw = 0;
		cameraInfo.m_pitch = 70 * gdm_DEGTORAD;
		gdm_CameraSetInfo(g_pRender, &cameraInfo, 1);
	}
	CGDM_Command::OnEnd();//반드시 마지막에 호출해야 함
}
