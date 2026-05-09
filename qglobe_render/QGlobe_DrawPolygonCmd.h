#ifndef __QGlobe_DRAWPOLYGONCMD_H__
#define __QGlobe_DRAWPOLYGONCMD_H__

#include "QGlobe_DrawCmd.h"
#include "GIS_Polygon.h"

class CQGlobe_DrawPolygonCmd: public CQGlobe_DrawCmd
{
public:
	CQGlobe_DrawPolygonCmd(CQGlobe_CommandDlg *pCmdDlg = NULL);
	virtual ~CQGlobe_DrawPolygonCmd(void);

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
