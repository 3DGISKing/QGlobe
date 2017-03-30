#pragma once


#include "GDM_Tile.h"
#include "gdem_libmain_global.h"

#define GDM_MAX_REQUEST_LIST_COUNT	E_GDM_MAINDATA_TYPE_MAX

typedef struct _gdmTileSpec
{
	E_GDM_MAINDATA_TYPE     eType;				// the data type
	int						minLevel;			// the min level
	int						maxLevel;			// the max level
	int						offsetLevel;			// request offset level
	int						maxRequestCount;		// max request count
	
}GDM_TILE_SPEC;

typedef struct _gdmTileRequest
{
	GDM_TILE_SPEC	   spec;
	GDM_TILE_INFO_ARR  reqList;
	bool			   isEmpty;
	bool			   isLock;	
}GDM_TILE_REQUEST;

class GDEM_LIBMAIN_EXPORT CGDM_RequestMgr
{
public:
	CGDM_RequestMgr(void);
	~CGDM_RequestMgr(void);

	void		              UpdateRequest();
    bool		              GetHighRequestTile(GDM_TILE_INFO *tile, E_GDM_MAINDATA_TYPE eType);
	void		              SetStatusFlag(GDM_TILE_INFO *pInfo, E_TILE_STATUS status);
	int			              GetOffsetLevel() { return m_nOffsetBySpeed;}	// for speed
	const GDM_TILE_SPEC*      GetSpec(E_GDM_MAINDATA_TYPE eType);
	void                      ClearRequest(E_GDM_MAINDATA_TYPE eType);
	void		              UpdateRequest(E_GDM_MAINDATA_TYPE eType);

private:
	void		              InitSpec();
	GDM_TILE_REQUEST*	      GetTileRequest(E_GDM_MAINDATA_TYPE eType);
	void		              UpdateRequestTiles(GDM_TILE_REQUEST *pRequest, GDM_TILE_INFO_ARR *pPyramidList); 
	void		              InitDefaultRequest(void);
	int			              IsExistItem(GDM_TILE_INFO_ARR *pReqList);
	void		              CheckRate(void);

	GDM_TILE_REQUEST          m_sTileReq_Dem;
	GDM_TILE_REQUEST          m_sTileReq_Img;
	GDM_TILE_REQUEST          m_sTileReq_Name;
	GDM_TILE_REQUEST          m_sTileReq_Shape;
	GDM_TILE_REQUEST          m_sTileReq_3D;

	int			              m_blDefaultComplete; // default flag
	double		              m_dCompleteRate;	// 0-1 :	feedback value for velocity
	GDM_TILE_REQUEST*         m_pTileReqList[E_GDM_MAINDATA_TYPE_MAX];	// for temp manage
	int			              m_nOffsetLevel;	// for speed control
	bool		              m_blHighSpeed;	// for speed control
	int			              m_nOffsetBySpeed; // for speed control
};
