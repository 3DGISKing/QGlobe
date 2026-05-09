
#include "QGlobe_SimCmd.h"
#include "QGlobe_Simulator.h"
#include "rendersrv.h"
#include "QGlobe_CommandMgr.h"
#include "cameracontroller.h"
#include "qglobeclient.h"


CQGlobe_SimCmd::CQGlobe_SimCmd(CQGlobe_CommandDlg *pCmdDlg) : CQGlobe_Command(pCmdDlg)
{
	m_eCmdType = eQGlobe_CMD_SIMULATION;
	qglobe_CameraGetCoord(g_pRender, &m_orgCoord);
	g_app->ResetRenderKeys();// Key���¸� �ʱ�ȭ �Ѵ�.
}

CQGlobe_SimCmd::~CQGlobe_SimCmd(void)
{
	m_pSimulator = NULL;
	g_app->ResetRenderKeys();// Key���¸� �ʱ�ȭ �Ѵ�.
}

void
CQGlobe_SimCmd::ChangeCursor()
{
	QGlobe_SIM_STATUS simStatus;
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
CQGlobe_SimCmd::OnMousePress(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	bool ret = m_pSimulator->OnMousePress(mouseEvent);
	ChangeCursor();
	return ret;
}

bool	
CQGlobe_SimCmd::OnMouseMove(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseMove(mouseEvent);
}

bool	
CQGlobe_SimCmd::OnMouseRelease(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseRelease(mouseEvent);
}

bool	
CQGlobe_SimCmd::OnMouseDblClick(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseDblClick(mouseEvent);

}

bool	
CQGlobe_SimCmd::OnMouseWheelEvent(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseWheelEvent(mouseEvent);

}

bool	
CQGlobe_SimCmd::OnMouseClickEvent(CursorState* mouseEvent)
{
	if (!m_pSimulator)
		return false;
	return m_pSimulator->OnMouseClickEvent(mouseEvent);

}

void	
CQGlobe_SimCmd::OnKeyPressEvent(KeyState keyState)
{
	if (!m_pSimulator)
		return ;
	m_pSimulator->OnKeyPressEvent(keyState);

}

void			
CQGlobe_SimCmd::SetSimulator(CQGlobe_Simulator* a_pSimulator)
{
	m_pSimulator = a_pSimulator;
	ChangeCursor();
}

void CQGlobe_SimCmd::OnEnd()
{
	QGlobe_SIM_STATUS status;
	m_pSimulator->GetSimulationStatus(&status);
	if(status.m_autoMove)
	{
		m_pCommandMgr->GetCameraController()->StopView();
		qglobe_CameraSetCoord(g_pRender, m_orgCoord);
	}
	else
	{
		QGlobe_CAMERA_INFO cameraInfo;
		qglobe_CameraGetInfo(g_pRender, &cameraInfo, 1);
		cameraInfo.m_yaw = 0;
		cameraInfo.m_pitch = 70 * qglobe_DEGTORAD;
		qglobe_CameraSetInfo(g_pRender, &cameraInfo, 1);
	}
	CQGlobe_Command::OnEnd();//�ݵ�� �������� ȣ���ؾ� ��
}
