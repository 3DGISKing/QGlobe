#pragma once

#include "gdem_gis_global.h"
#include "../gdem_base/GDM_Vector3D.h"
#include "../gdem_base/GDM_Location.h"
#include "../gdem_base/GDM_Rect.h"
#include "../gdem_common/gdemclientbase.h"

// geometric node type
typedef enum 
{
	E_GEONODE_TYPE_NONE,
	E_GEONODE_TYPE_POINT,					/* point node   */
	E_GEONODE_TYPE_POLYGON					/* polygon node */
} E_GEONODE_TYPE;
/************************************/

// gis node type
typedef enum
{
	E_GISNODE_TYPE_NONE,
	E_GISNODE_TYPE_PLACEMARK,				/* place mark node  */
	E_GISNODE_TYPE_PLACESEARCH,				/* place mark search node*/
	E_GISNODE_TYPE_RULER,					/* ruler node */
	E_GISNODE_TYPE_PATH,					/* path  node */
	E_GISNODE_TYPE_AREA,					/* polygon node */
} E_GISNODE_TYPE;
/************************************/

// gis node type
typedef enum
{
	E_GISNODE_STATE_NONE,
	E_GISNODE_STATE_SELECT,				/* selected state */
	E_GISNODE_STATE_FOCUS,				/* focus state*/
} E_GISNODE_STATE;
/************************************/

// pen of object
typedef struct _gdm_pen
{
	unsigned int	color;				// (alpha << 24 | r << 16 | g << 8 | b)
	float			width;				// the width of line
}GDM_PEN;

// pen of object
typedef struct _gdm_brush
{
	unsigned int	color;				// (alpha << 24 | r << 16 | g << 8 | b)
}GDM_BRUSH;

/************************************/
class IGIS_Progress;

class GDEM_GIS_EXPORT CGIS_Node
{
public:
	CGIS_Node(void);
	virtual ~CGIS_Node(void);

public:

	// type functions
	E_GEONODE_TYPE	Get_GeoType() {return m_nGeoType;}
	E_GISNODE_TYPE	Get_GisType() {return m_nGisType;}

	// pen and brush function
	GDM_PEN*		Get_Pen() {return &m_sPen;}
	GDM_BRUSH*		Get_Brush() {return &m_sBrush;}

	// track functions for select or modify
	virtual unsigned int	Get_TrackCount()  {return 0;}
	virtual unsigned int	Get_FocusTrackID()  {return m_nActiveTrackId;}
	virtual void			Get_TrackPt(unsigned int trackIndex, CGDM_Location3D *pPt) {}
	virtual void			Set_TrackPt(unsigned int trackIndex, CGDM_Location3D *pPt) {}
	virtual void			Set_TrackID(unsigned int trackIndex){ m_nActiveTrackId = trackIndex;}


	// key
	unsigned int			GetKey()						{return m_nKey;}
	void					SetKey(unsigned int key)		{m_nKey = key;}

	// state
	E_GISNODE_STATE GetState()						{return m_nState;};
	void			SetState(E_GISNODE_STATE state) {m_nState = state;}

	void			SetFocus(bool blFocus);

	// for select
	virtual bool	IsContain(MOUSE_POSITION &pos) {return false;}

	// visible functions
	bool			IsVisible();
	void			SetVisible(bool enable) {m_blVisible = enable;}
	void			SetEnable(bool enable) {m_blEnable = enable;}
	void			SetProgressBar(IGIS_Progress* progress){m_pProgress = progress;}

	// operator 
	CGIS_Node &		operator=(const CGIS_Node & cNode);
	virtual void	CopyFrom(const CGIS_Node & cNode);
	virtual void	Write(QDataStream &out);
	virtual void	Read(QDataStream &out);

	virtual CGIS_Node *		Clone();

public:
	E_GISNODE_TYPE		m_nGisType;
	E_GEONODE_TYPE		m_nGeoType;

	GDM_PEN			    m_sPen;		
	GDM_BRUSH		    m_sBrush;

	unsigned short	    m_sName[PMD_STR_MAX_SIZE];

	bool			    m_blEnable;
	bool			    m_blVisible;
	bool			    m_blEditing;

	unsigned int	    m_nKey;	       // universal index
	E_GISNODE_STATE     m_nState;
	CGDM_Rect2D		    m_cBound;
	bool			    m_blCalculated;	// the calculated flag
protected:
	unsigned int	    m_nActiveTrackId;
	IGIS_Progress*	    m_pProgress;

};

