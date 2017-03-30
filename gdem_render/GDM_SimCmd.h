#ifndef __GDM_SIMCMD_H__
#define __GDM_SIMCMD_H__
#include "GDM_Command.h"


class CGDM_Simulator;

class CGDM_SimCmd: public CGDM_Command
{
public:
	CGDM_SimCmd(CGDM_CommandDlg *pCmdDlg = NULL);
	virtual ~CGDM_SimCmd(void);

public:
	virtual bool	OnMousePress(CursorState* mouseEvent);
	virtual bool	OnMouseMove(CursorState* mouseEvent);
	virtual bool	OnMouseRelease(CursorState* mouseEvent);
	virtual bool	OnMouseDblClick(CursorState* mouseEvent);
	virtual bool	OnMouseWheelEvent(CursorState* mouseEvent);
	virtual bool	OnMouseClickEvent(CursorState* mouseEvent);
	virtual void	OnKeyPressEvent(KeyState keyState);
	virtual void	OnEnd();

	void			SetSimulator(CGDM_Simulator* a_pSimulator);

private:
	void			ChangeCursor();
private:
	CGDM_Simulator*	m_pSimulator;
	GDM_Coord3D		m_orgCoord;
};
#endif
