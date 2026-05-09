#ifndef __QGlobe_DRAWCMD_H__
#define __QGlobe_DRAWCMD_H__
#include "QGlobe_Command.h"

class CQGlobe_DrawCmd: public CQGlobe_Command
{
public:
	CQGlobe_DrawCmd(CQGlobe_CommandDlg *pCmdDlg = NULL);
	virtual ~CQGlobe_DrawCmd(void);

public:
	virtual bool OnMousePress(CursorState* mouseEvent){return false;}
	virtual bool OnMouseMove(CursorState* mouseEvent){return false;}
	virtual bool OnMouseRelease(CursorState* mouseEvent){return false;}
	virtual bool OnMouseDblClick(CursorState* mouseEvent){return false;}
};
#endif
