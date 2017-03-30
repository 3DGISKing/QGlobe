#include "stable.h"
#include "GDM_Command.h"
#include "gdemclient.h"
#include "config.h"
#include "GDM_CommandMgr.h"
#include "cameracontroller.h"
#include "GDM_CommandDlg.h"
#include "render_global.h"


CGDM_Command::CGDM_Command(CGDM_CommandDlg *pCmdDlg)
{
	m_eCmdType = eGDM_CMD_NONE;
	m_eCursorType = CURSOR_SHAPE_NORMAL;
	m_nStep = 0;
	m_pCommandDlg = pCmdDlg;
	m_pCommandMgr = NULL;
}

CGDM_Command::~CGDM_Command(void)
{
}

bool CGDM_Command::OnMouseWheelEvent(CursorState* mouseEvent)
{
	if(mouseEvent->keys & KS_SPACE)
		return false;
	Config *conf = g_app->GetConfig();
	double speed = (double)ZOOM_SPEED_MAX*conf->m_wheelSpeed/OPTION_MOUSE_WHELL_SPEED;
	speed *= (mouseEvent->delta > 0 ? 1 : -1);
	if(fabs(speed) < 1)
		speed *= 1.5 * (speed > 0 ? 1 : -1);
	if (conf->m_wheelDirection)
		speed = -speed;
	m_pCommandMgr->GetCameraController()->MoveForward(speed, 0);
	return true;
}

void CGDM_Command::OnKeyPressEvent(KeyState keyState)
{
	CameraController* pCamera = m_pCommandMgr->GetCameraController();
	if(!pCamera)
		return;
	if(keyState.bMoveKey)
	{
		int x, y;
		x = y = 0;
		if(keyState.key1 == Qt::Key_Right || keyState.key1 == Qt::Key_Left)
			x = keyState.key1 == Qt::Key_Right ? 1 : -1;
		if(keyState.key2 == Qt::Key_Up || keyState.key2 == Qt::Key_Down)
			y = keyState.key2 == Qt::Key_Up ? 1 : -1;
		pCamera->MoveView(x, y);
	}
	else
	{
		switch (keyState.key1)
		{
		case Qt::Key_Left:
			if (keyState.keyFlags & KS_SHIFT)
			{
				pCamera->RotateView(1, 0);
			}
			else if(keyState.keyFlags & KS_CTRL)
			{
				pCamera->RotateView(1, 1);
			}
			break;

		case Qt::Key_Right:
			if (keyState.keyFlags & KS_SHIFT)
			{
				pCamera->RotateView(-1, 0);
			}
			else if(keyState.keyFlags & KS_CTRL)
			{
				pCamera->RotateView(-1, 1);
			}
			break;

		case Qt::Key_Up:
			if (keyState.keyFlags & KS_SHIFT)
			{
				pCamera->TiltView(-1, 0);
			}
			else if(keyState.keyFlags & KS_CTRL)
			{
				pCamera->TiltView(1, 1);
			}
			break;

		case Qt::Key_Down:
			if (keyState.keyFlags & KS_SHIFT)
			{
				pCamera->TiltView(1, 0);
			}
			else if(keyState.keyFlags & KS_CTRL)
			{
				pCamera->TiltView(-1, 1);
			}
			break;
		case Qt::Key_N:
			if (keyState.keyFlags == 0)
				pCamera->ResetView();
			break;

		case Qt::Key_U:
			if (keyState.keyFlags == 0)
				pCamera->ResetTilt();
			break;

		case Qt::Key_R:
			if (keyState.keyFlags == 0)
				pCamera->ResetAll();
			break;

		case Qt::Key_Space:
			pCamera->StopView();
			break;
		case Qt::Key_PageUp:
			if(keyState.keyFlags & KS_ALT)
				pCamera->MoveForward(ZOOM_SPEED_MAX/5, 1);
			else
				pCamera->MoveForward(ZOOM_SPEED_MAX, 1);
			break;
		case Qt::Key_PageDown:
			if(keyState.keyFlags & KS_ALT)
				pCamera->MoveForward(-ZOOM_SPEED_MAX/5, 1);
			else
				pCamera->MoveForward(-ZOOM_SPEED_MAX, 1);
			break;
		case Qt::Key_Plus:
			pCamera->MoveForward(ZOOM_SPEED_MAX/5, 1);
			break;

		case Qt::Key_Minus:
			pCamera->MoveForward(-ZOOM_SPEED_MAX/5, 1);
			break;
		}

	}
}

void CGDM_Command::OnEnd()
{
	m_pCommandMgr->CancelEditCmd();
}

void CGDM_Command::OnCancel()
{
	m_pCommandMgr->CancelEditCmd();
}

CGDM_Location3D	CGDM_Command::GetLocation(CursorState* mouseEvent)
{
	MOUSE_POSITION pos;
	CGDM_Location3D pt;

	pos = GetCurrentLocation();

	if(pos.x != mouseEvent->x || pos.y != mouseEvent->y)
	{
		m_pCommandMgr->SetCursorPt(mouseEvent);
		pos = GetCurrentLocation();
	}

	pt.m_tX = pos.longitude;
	pt.m_tY = pos.latitude;
	pt.m_tZ	= pos.altitude;

	return pt;
}

MOUSE_POSITION	CGDM_Command::GetCurrentLocation()
{
	return m_pCommandMgr->GetCursorPt();
}

void CGDM_Command::UpdateDailog(void)
{
	if(m_pCommandDlg)
		m_pCommandDlg->UpdateDialog();
}