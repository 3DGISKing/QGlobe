#ifndef __GDM_COMMAND_H__
#define __GDM_COMMAND_H__
#include "GDM_Interface.h"
#include "gdemclientbase.h"
#include "GIS_Node.h"
#include "render_global.h"
class CGDM_CommandMgr;
class CGDM_CommandDlg;

class CGDM_Command
{
public:
	CGDM_Command(CGDM_CommandDlg *pCmdDlg = NULL);
	virtual ~CGDM_Command(void);

public:
	virtual bool OnMousePress(CursorState* mouseEvent) {return false;}
	virtual bool OnMouseMove(CursorState* mouseEvent) {return false;}
	virtual bool OnMouseRelease(CursorState* mouseEvent) {return false;}
	virtual bool OnMouseDblClick(CursorState* mouseEvent) {return false;}
	virtual bool OnMouseWheelEvent(CursorState* mouseEvent);
	virtual bool OnMouseClickEvent(CursorState* mouseEvent) {return false;}
	virtual void OnKeyPressEvent(KeyState keyState);

	virtual void OnStartAnimation(GDM_ANIMATION_TYPE aniType){};
	virtual void OnEndAnimation(GDM_ANIMATION_TYPE aniType){};
	virtual void OnCameraChanged(){};

	// return with ok
	virtual void OnEnd();		 

	// return with cancel
	virtual void OnCancel();
	virtual CGIS_Node*	GetNode() {return NULL;}

	
	CURSOR_SHAPE GetCursorType(){return m_eCursorType;}
	void SetCursorType(CURSOR_SHAPE  cursorType){m_eCursorType = cursorType;}
	E_GDM_CMD_TYPE GetCmdType(){return m_eCmdType;}
	void SetCommandMgr(CGDM_CommandMgr* pCommandMgr){m_pCommandMgr = pCommandMgr;}
	CGDM_CommandDlg* GetDlg() {return m_pCommandDlg;}
public:
	MOUSE_POSITION	GetCurrentLocation();

protected:
	CGDM_Location3D	GetLocation(CursorState* mouseEvent);
	void				UpdateDailog(void);

protected:
	CGDM_CommandMgr*	m_pCommandMgr;
	E_GDM_CMD_TYPE		m_eCmdType;
	CURSOR_SHAPE		m_eCursorType;
	int					m_nStep;

	CGDM_CommandDlg *	m_pCommandDlg;
};

#endif