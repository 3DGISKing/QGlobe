#ifndef __GDM_DRAWPOLYGONCMD_H__
#define __GDM_DRAWPOLYGONCMD_H__

#include "GDM_DrawCmd.h"
#include "GIS_Polygon.h"

class CGDM_DrawPolygonCmd: public CGDM_DrawCmd
{
public:
	CGDM_DrawPolygonCmd(CGDM_CommandDlg *pCmdDlg = NULL);
	virtual ~CGDM_DrawPolygonCmd(void);

public:
	virtual bool OnMousePress(CursorState* mouseEvent);
	virtual bool OnMouseMove(CursorState* mouseEvent);
	virtual bool OnMouseRelease(CursorState* mouseEvent);
	
	virtual CGIS_Node*	GetNode()	{return m_pNode;}
	virtual void		OnCancel();
	virtual void		OnEnd();

public:
	void				Init(CGIS_Polygon *pNode, bool blNew);
private:
	int			GetSelectTrackIndex(CursorState* mouseEvent);

public:
	CGIS_Polygon*		m_pNode;
protected:
	CGIS_Polygon*		m_pOrgNode;
	bool				m_blPress;
	bool				m_blTrack;
	bool				m_blActive;
	CursorState			m_cPressCursor;
	int					m_nCurTrackIndex;

	double				m_dTopHeight;
};
#endif
