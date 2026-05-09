#ifndef __QGlobe_SIMCMD_H__
#define __QGlobe_SIMCMD_H__
#include "QGlobe_Command.h"


class CQGlobe_Simulator;

class CQGlobe_SimCmd: public CQGlobe_Command
{
public:
	CQGlobe_SimCmd(CQGlobe_CommandDlg *pCmdDlg = NULL);
	virtual ~CQGlobe_SimCmd(void);

public:
	virtual bool	OnMousePress(CursorState* mouseEvent);
	virtual bool	OnMouseMove(CursorState* mouseEvent);
	virtual bool	OnMouseRelease(CursorState* mouseEvent);
	virtual bool	OnMouseDblClick(CursorState* mouseEvent);
	virtual bool	OnMouseWheelEvent(CursorState* mouseEvent);
	virtual bool	OnMouseClickEvent(CursorState* mouseEvent);
	virtual void	OnKeyPressEvent(KeyState keyState);
	virtual void	OnEnd();

	void			SetSimulator(CQGlobe_Simulator* a_pSimulator);

private:
	void			ChangeCursor();
private:
	CQGlobe_Simulator*	m_pSimulator;
	QGlobe_Coord3D		m_orgCoord;
};
#endif
