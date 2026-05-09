#ifndef __QGlobe_DRAWOBJECTCMD_H__
#define __QGlobe_DRAWOBJECTCMD_H__

#include "QGlobe_DrawCmd.h"
#include "GIS_Polygon.h"

class CQGlobe_DrawObjectCmd: public CQGlobe_DrawCmd
{
public:
	CQGlobe_DrawObjectCmd(CQGlobe_CommandDlg *pCmdDlg = NULL);
	virtual ~CQGlobe_DrawObjectCmd(void);

public:
	virtual bool OnMousePress(CursorState* mouseEvent);
	virtual bool OnMouseMove(CursorState* mouseEvent);
	virtual bool OnMouseRelease(CursorState* mouseEvent);
	virtual bool OnMouseDblClick(CursorState* mouseEvent);
	
	virtual CGIS_Node*	GetNode()	{return m_pNode;}
	virtual void		OnCancel();
	virtual void		OnEnd();
	virtual void		Clear();

	void		SetShapeType(int shapeType)	{ m_shapeType = shapeType; }
	void		SetTrackIndex(int trackId)	{ m_nCurTrackIndex = trackId; }
	bool				m_drawEnd;

public:
	void				Init(CGIS_Polygon *pNode, bool blNew);
private:
	int					GetSelectTrackIndex(CursorState* mouseEvent);

	int					m_shapeType;

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
