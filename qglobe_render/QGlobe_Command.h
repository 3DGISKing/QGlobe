#ifndef __QGlobe_COMMAND_H__
#define __QGlobe_COMMAND_H__
#include "QGlobe_Interface.h"
#include "qglobeclientbase.h"
#include "GIS_Node.h"
#include "render_global.h"
class CQGlobe_CommandMgr;
class CQGlobe_CommandDlg;

class CQGlobe_Command
{
public:
	CQGlobe_Command(CQGlobe_CommandDlg *pCmdDlg = NULL);
	virtual ~CQGlobe_Command(void);

public:
	virtual bool OnMousePress(CursorState* mouseEvent) {return false;}
	virtual bool OnMouseMove(CursorState* mouseEvent) {return false;}
	virtual bool OnMouseRelease(CursorState* mouseEvent) {return false;}
	virtual bool OnMouseDblClick(CursorState* mouseEvent) {return false;}
	virtual bool OnMouseWheelEvent(CursorState* mouseEvent);
	virtual bool OnMouseClickEvent(CursorState* mouseEvent) {return false;}
	virtual void OnKeyPressEvent(KeyState keyState);

	virtual void OnStartAnimation(QGlobe_ANIMATION_TYPE aniType){};
	virtual void OnEndAnimation(QGlobe_ANIMATION_TYPE aniType){};
	virtual void OnCameraChanged(){};

	// return with ok
	virtual void OnEnd();		 

	// return with cancel
	virtual void OnCancel();
	virtual CGIS_Node*	GetNode() {return NULL;}

	
	CURSOR_SHAPE GetCursorType(){return m_eCursorType;}
	void SetCursorType(CURSOR_SHAPE  cursorType){m_eCursorType = cursorType;}
	E_QGlobe_CMD_TYPE GetCmdType(){return m_eCmdType;}
	void SetCommandMgr(CQGlobe_CommandMgr* pCommandMgr){m_pCommandMgr = pCommandMgr;}
	CQGlobe_CommandDlg* GetDlg() {return m_pCommandDlg;}
public:
	MOUSE_POSITION	GetCurrentLocation();

protected:
	CQGlobe_Location3D	GetLocation(CursorState* mouseEvent);
	void				UpdateDailog(void);

protected:
	CQGlobe_CommandMgr*	m_pCommandMgr;
	E_QGlobe_CMD_TYPE		m_eCmdType;
	CURSOR_SHAPE		m_eCursorType;
	int					m_nStep;

	CQGlobe_CommandDlg *	m_pCommandDlg;
};

#endif