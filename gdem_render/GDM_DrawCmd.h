#ifndef __GDM_DRAWCMD_H__
#define __GDM_DRAWCMD_H__
#include "GDM_Command.h"

class CGDM_DrawCmd: public CGDM_Command
{
public:
	CGDM_DrawCmd(CGDM_CommandDlg *pCmdDlg = NULL);
	virtual ~CGDM_DrawCmd(void);

public:
	virtual bool OnMousePress(CursorState* mouseEvent){return false;}
	virtual bool OnMouseMove(CursorState* mouseEvent){return false;}
	virtual bool OnMouseRelease(CursorState* mouseEvent){return false;}
	virtual bool OnMouseDblClick(CursorState* mouseEvent){return false;}
};
#endif
