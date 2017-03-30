#ifndef __GDM_COMMANDMGR_H__
#define __GDM_COMMANDMGR_H__
#include "GDM_Command.h"

class CGDM_RulerCmd;
class CGDM_CameraCmd;
class CameraController;
class CGDM_CommandDlg;

class CGDM_CommandMgr
{
public:
	CGDM_CommandMgr(void);
	~CGDM_CommandMgr(void);
	CGDM_Command* CreateCmd(E_GDM_CMD_TYPE eCmdType, CGDM_CommandDlg *pCommandDlg = NULL);

	bool OnMousePress(CursorState* mouseEvent);
	bool OnMouseMove(CursorState* mouseEvent);
	bool OnMouseRelease(CursorState* mouseEvent);
	bool OnMouseDblClick(CursorState* mouseEvent);
	bool OnMouseWheelEvent(CursorState* mouseEvent);
	bool OnMouseClickEvent(CursorState* mouseEvent);
	void OnKeyPressEvent(KeyState keyState);
	
	void OnStartAnimation(GDM_ANIMATION_TYPE aniType);
	void OnNotifyEvent(EGDM_RENDER_NOTIFY_TYPE nType, int param1 = 0, int param2 = 0, int param3 = 0);

	void			SetCursorPt(CursorState* mouseEvent);
	MOUSE_POSITION	GetCursorPt();

	bool	IsEditing();

	void	CancelEditCmd();
	CURSOR_SHAPE GetCursorShape(){return GetCurrentCmd()->GetCursorType();}
	
	void SetCameraController(CameraController* pCamera){CGDM_CommandMgr::m_pCameraController = pCamera;}
	CameraController* GetCameraController(){return m_pCameraController;}

	CGDM_Command*	GetCurrentCmd();

private:
	// for select
	void	OnCheckSelectObject(bool blFocus);
	bool    OnCheckSelectGeo3dObject();
	void    OnCheckPlaceMarkContent();//2014 2 6 by ugi
	void    OnCheckSelect3dObject();


private:
	static CameraController*	m_pCameraController;

//	CGDM_Command	*m_pCurCmd;
//	CGDM_Command	*m_pPrevCmd;
	CGDM_Command	*m_pCameraCmd;
	CGDM_Command	*m_pEditCmd;
	void*			m_pUserData;

	CursorState		m_cOldPressCursor;
	bool			m_blFirstMove;

	//CGDM_RulerCmd	*m_pRulerCmd;
	//CGDM_CameraCmd	*m_pCameraCmd;
};
#endif