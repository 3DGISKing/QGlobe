#ifndef __QGlobe_CAMERACMD_H__
#define __QGlobe_CAMERACMD_H__

#include "QGlobe_Command.h"

class CQGlobe_CameraCmd : 	public CQGlobe_Command
{
public:
	CQGlobe_CameraCmd(CQGlobe_CommandDlg *pCmdDlg = NULL);
	~CQGlobe_CameraCmd(void);

	virtual bool OnMousePress(CursorState* mouseEvent);
	virtual bool OnMouseMove(CursorState* mouseEvent);
	virtual bool OnMouseRelease(CursorState* mouseEvent);
	virtual bool OnMouseDblClick(CursorState* mouseEvent);
	virtual void OnKeyPressEvent(KeyState keyState);
	virtual void OnStartAnimation(QGlobe_ANIMATION_TYPE aniType){};
	virtual void OnEndAnimation(QGlobe_ANIMATION_TYPE aniType) {};
private:
	CURSOR_SHAPE GetCursorType();

};

#endif

