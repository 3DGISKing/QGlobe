#ifndef __GDM_RULERCMD_H__
#define __GDM_RULERCMD_H__

#include "GDM_Command.h"
#include "GDM_Coord3D.h"
#include "GIS_Ruler.h"

class CGDM_RulerCmd :	public CGDM_Command
{
public:
	CGDM_RulerCmd(CGDM_CommandDlg *pCmdDlg = NULL);
	~CGDM_RulerCmd(void);

public:

	// inherited function
	virtual bool OnMousePress(CursorState* mouseEvent);
	virtual bool OnMouseMove(CursorState* mouseEvent);
	virtual bool OnMouseRelease(CursorState* mouseEvent);
	virtual void OnKeyPressEvent(KeyState keyState);

	virtual CGIS_Node*	GetNode()	{return m_pRulerNode;}
	virtual void		OnCancel();

public:
	void		Clear ();
	double		CountAllLength (bool blLine = true);
	double		CountAngle (bool blLine = true);
	void		SetMeasureMode(MeasureInfo measureInfo);
private:
	void		Init(void);
	int			GetSelectTrackIndex(CursorState* mouseEvent);


public:
	CGIS_Ruler			*m_pRulerNode;
	bool				m_blLine;
	bool				m_blMouseCheck;
private:
	bool				m_blPress;
	bool				m_blTrack;
	bool				m_blActive;
	CursorState			m_cPressCursor;

	int					m_nCurTrackIndex;
};

#endif

