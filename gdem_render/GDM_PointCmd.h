#pragma once

#include "GDM_Command.h"
#include "GDM_Coord3D.h"
#include "GIS_PlaceIcon.h"

class CGDM_PointCmd : 	public CGDM_Command
{
public:
	CGDM_PointCmd(CGDM_CommandDlg *pCmdDlg = NULL);
	~CGDM_PointCmd(void);

public:
	// inherited function
	virtual bool OnMousePress(CursorState* mouseEvent);
	virtual bool OnMouseMove(CursorState* mouseEvent);
	virtual bool OnMouseRelease(CursorState* mouseEvent);

	virtual CGIS_Node*	GetNode()	{return m_pNode;}
	virtual void		OnCancel();
	virtual void		OnEnd();
	virtual void		OnCameraChanged();

public:
	void		Init(CGIS_Node *pNode, bool blNew);

public:
	CGIS_Point*		m_pNode;
	bool			m_blCenter;
private:
	CGIS_Point*		m_pOrgNode;
	bool			m_blPress;
	bool			m_blTrack;
};

