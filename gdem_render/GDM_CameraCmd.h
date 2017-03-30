#ifndef __GDM_CAMERACMD_H__
#define __GDM_CAMERACMD_H__

#include "GDM_Command.h"

class CGDM_CameraCmd : 	public CGDM_Command
{
public:
	CGDM_CameraCmd(CGDM_CommandDlg *pCmdDlg = NULL);
	~CGDM_CameraCmd(void);

	virtual bool OnMousePress(CursorState* mouseEvent);
	virtual bool OnMouseMove(CursorState* mouseEvent);
	virtual bool OnMouseRelease(CursorState* mouseEvent);
	virtual bool OnMouseDblClick(CursorState* mouseEvent);
	virtual void OnKeyPressEvent(KeyState keyState);
	virtual void OnStartAnimation(GDM_ANIMATION_TYPE aniType){};
	virtual void OnEndAnimation(GDM_ANIMATION_TYPE aniType) {};
private:
	CURSOR_SHAPE GetCursorType();

};

#endif

