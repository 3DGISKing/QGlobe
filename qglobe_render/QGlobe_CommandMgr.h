#ifndef __QGlobe_COMMANDMGR_H__
#define __QGlobe_COMMANDMGR_H__
#include "QGlobe_Command.h"

class CQGlobe_RulerCmd;
class CQGlobe_CameraCmd;
class CameraController;
class CQGlobe_CommandDlg;

class CQGlobe_CommandMgr
{
public:
	CQGlobe_CommandMgr(void);
	~CQGlobe_CommandMgr(void);
	CQGlobe_Command* CreateCmd(E_QGlobe_CMD_TYPE eCmdType, CQGlobe_CommandDlg *pCommandDlg = NULL);

	bool OnMousePress(CursorState* mouseEvent);
	bool OnMouseMove(CursorState* mouseEvent);
	bool OnMouseRelease(CursorState* mouseEvent);
	bool OnMouseDblClick(CursorState* mouseEvent);
	bool OnMouseWheelEvent(CursorState* mouseEvent);
	bool OnMouseClickEvent(CursorState* mouseEvent);
	void OnKeyPressEvent(KeyState keyState);
	
	void OnStartAnimation(QGlobe_ANIMATION_TYPE aniType);
	void OnNotifyEvent(EQGlobe_RENDER_NOTIFY_TYPE nType, int param1 = 0, int param2 = 0, int param3 = 0);

	void			SetCursorPt(CursorState* mouseEvent);
	MOUSE_POSITION	GetCursorPt();

	bool	IsEditing();

	void	CancelEditCmd();
	CURSOR_SHAPE GetCursorShape(){return GetCurrentCmd()->GetCursorType();}
	
	void SetCameraController(CameraController* pCamera){CQGlobe_CommandMgr::m_pCameraController = pCamera;}
	CameraController* GetCameraController(){return m_pCameraController;}

	CQGlobe_Command*	GetCurrentCmd();

private:
	// for select
	void	OnCheckSelectObject(bool blFocus);
	bool    OnCheckSelectGeo3dObject();
	void    OnCheckPlaceMarkContent();//2014 2 6 by ugi
	void    OnCheckSelect3dObject();


private:
	static CameraController*	m_pCameraController;

//	CQGlobe_Command	*m_pCurCmd;
//	CQGlobe_Command	*m_pPrevCmd;
	CQGlobe_Command	*m_pCameraCmd;
	CQGlobe_Command	*m_pEditCmd;
	void*			m_pUserData;

	CursorState		m_cOldPressCursor;
	bool			m_blFirstMove;

	//CQGlobe_RulerCmd	*m_pRulerCmd;
	//CQGlobe_CameraCmd	*m_pCameraCmd;
};
#endif