#pragma once


#include "QGlobe_Tile.h"
#include "qglobe_libmain_global.h"

#define QGlobe_MAX_REQUEST_LIST_COUNT	E_QGlobe_MAINDATA_TYPE_MAX

typedef struct _gdmTileSpec
{
	E_QGlobe_MAINDATA_TYPE     eType;				// the data type
	int						minLevel;			// the min level
	int						maxLevel;			// the max level
	int						offsetLevel;			// request offset level
	int						maxRequestCount;		// max request count
	
}QGlobe_TILE_SPEC;

typedef struct _gdmTileRequest
{
	QGlobe_TILE_SPEC	   spec;
	QGlobe_TILE_INFO_ARR  reqList;
	bool			   isEmpty;
	bool			   isLock;	
}QGlobe_TILE_REQUEST;

class QGLOBE_LIBMAIN_EXPORT CQGlobe_RequestMgr
{
public:
	CQGlobe_RequestMgr(void);
	~CQGlobe_RequestMgr(void);

	void		              UpdateRequest();
    bool		              GetHighRequestTile(QGlobe_TILE_INFO *tile, E_QGlobe_MAINDATA_TYPE eType);
	void		              SetStatusFlag(QGlobe_TILE_INFO *pInfo, E_TILE_STATUS status);
	int			              GetOffsetLevel() { return m_nOffsetBySpeed;}	// for speed
	const QGlobe_TILE_SPEC*      GetSpec(E_QGlobe_MAINDATA_TYPE eType);
	void                      ClearRequest(E_QGlobe_MAINDATA_TYPE eType);
	void		              UpdateRequest(E_QGlobe_MAINDATA_TYPE eType);

private:
	void		              InitSpec();
	QGlobe_TILE_REQUEST*	      GetTileRequest(E_QGlobe_MAINDATA_TYPE eType);
	void		              UpdateRequestTiles(QGlobe_TILE_REQUEST *pRequest, QGlobe_TILE_INFO_ARR *pPyramidList); 
	void		              InitDefaultRequest(void);
	int			              IsExistItem(QGlobe_TILE_INFO_ARR *pReqList);
	void		              CheckRate(void);

	QGlobe_TILE_REQUEST          m_sTileReq_Dem;
	QGlobe_TILE_REQUEST          m_sTileReq_Img;
	QGlobe_TILE_REQUEST          m_sTileReq_Name;
	QGlobe_TILE_REQUEST          m_sTileReq_Shape;
	QGlobe_TILE_REQUEST          m_sTileReq_3D;

	int			              m_blDefaultComplete; // default flag
	double		              m_dCompleteRate;	// 0-1 :	feedback value for velocity
	QGlobe_TILE_REQUEST*         m_pTileReqList[E_QGlobe_MAINDATA_TYPE_MAX];	// for temp manage
	int			              m_nOffsetLevel;	// for speed control
	bool		              m_blHighSpeed;	// for speed control
	int			              m_nOffsetBySpeed; // for speed control
};
