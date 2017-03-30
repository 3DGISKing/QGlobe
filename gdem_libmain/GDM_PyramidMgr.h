#pragma once

#include "GDM_Interface.h"
#include "GDM_Plane3D.h"
#include <QtCore/QList>

#define TILE_PRIO_DEFAULT	100

class  CGDM_Camera;

// Describe the tile set information each level
struct GDM_LEVEL_INFO
{
	char	level;				// level number
	double	tileSize;			// average tile size in degree
} ;

// Describe the bounding box that is not cube. AABB
struct GDM_BOUND_BOX
{
	int				cornerCount;// counts of corners
	CGDM_Vector3DF	corners[10];// corners
	CGDM_Vector3DF	min;		// min of AABB
	CGDM_Vector3DF	max;		// max of AABB
	CGDM_Vector3DF	center;		// center of bounding box
	double			radius;		// radius of surrounding sphere which has the above center.
}  ;

// Describe the quad tile in Quad Tree
struct GDM_QUAD_TILE
{
	char		level;			// level number
	double		west;			// left longitude 
	double		east;			// right longitude
	double		south;			// bottom latitude
	double		north;			// top latitude

	double		centerLatitude; // center latitude in radian
	double		centerLongitude;// center longitude in radian

	int			row;			// vertically tile number for 'level' tileset
	int			col;			// horizontally tile number for 'level' tileset

	GDM_QUAD_TILE*	parent;		// pointer to parent
	GDM_QUAD_TILE*	child[2][2];// children pointer 
								// [0][0]:WestSouth, [0][1]:EastSouth, [1][0]:WestNorth, [1][1]:EastNorth

	GDM_BOUND_BOX	bonndBox;	// bounding box of quad tile
} ;

// Describe the view information
struct GDM_VIEW_INFO
{
	double			cameraLongitude;	// longitude of camera position in radian
	double			cameraLatitude;		// latitude of camera position in radian
	double			cameraViewRange;	// camera's view range in degree
	CGDM_Plane3DF	frustumPlanes[6];	// six planes that consists the view frustum
	GDM_BOUND_BOX	frustumBonndBox;	// bounding box of view frustum
	double			quality;			// 0-1
} ;

//Pyramid class

class TilePool
{
public:
	TilePool();
	~TilePool();
	GDM_QUAD_TILE* GetOneTile();
	void           Clear(); 
private:
	int                   usedCount;
	QList<GDM_QUAD_TILE*> tilePool;
};

class GDEM_LIBMAIN_EXPORT CGDM_PyramidMgr
{
public:
	CGDM_PyramidMgr();
	~CGDM_PyramidMgr();

public:
	void		    GetPyramidTiles(GDM_TILE_INFO_ARR *psTileInfoArr);
	void		    GetNear4Tiles(int nLevel, double dLongitude, double	dLatitude, GDM_TILE_INFO_ARR *psTileInfoArr);
	int			    GetMaxLevel () { return m_nMaxLevel;}
	void		    Update();
	bool            Intersect(CGDM_Aabbox3DF box);

private:
	void		    InitializeTileSetInfo();
	void		    UpdateViewInfo();
	void		    GenerateVisibleQuadTree();
	void		    CheckQuadTile(IN GDM_QUAD_TILE* a_quadTile);

	GDM_QUAD_TILE*	MakeNewQuadTile(IN double a_west, IN double a_east, 
									IN double a_south, IN double a_north, IN char a_level);
	void			CreateChildQuadTiles(IN OUT GDM_QUAD_TILE* a_parent);
	void		    AddTileInfo(IN GDM_QUAD_TILE* a_quadTile);
	void		    SphericalToCartesian(IN double a_latitude, IN double a_longitude, 
									 IN double a_radius, OUT CGDM_Vector3DF& a_xyz);
	bool		    IntersectWithFrustum(IN GDM_QUAD_TILE* a_quadTile);
	bool		    IntersectBoundingBox(IN GDM_BOUND_BOX* a_boxA, IN GDM_BOUND_BOX* a_boxB);
	bool		    IsCoverCameraPos(IN GDM_QUAD_TILE* a_quadTile);
	double		    SphericalDistance(IN double a_latitude1, IN double a_longitude1,
										IN double a_latitude2, IN double a_longitude2);
	void		    CalcBoundBox(IN double a_west, IN double a_east, 
							 IN double a_south, IN double a_north, 
							 IN double a_radius1, IN double a_radius2, OUT GDM_BOUND_BOX* a_boundBox);

	void		    CalcVFBoundBox(IN GDM_Coord3D* a_coord, IN GDM_ViewFrustum* a_frustum, OUT GDM_BOUND_BOX* a_boundBox);

	void		    InitRootQuadTiles();
	void		    SortTiles(); 
	void		    CheckAndCompleteTiles();

	double          m_MinLat,m_MinLon;
	double          m_MaxLat, m_Maxlon;
private :
	char				m_levelNum;			// total level count
	GDM_LEVEL_INFO*		m_levelInfos;		// array to tileset info

	GDM_VIEW_INFO		m_viewInfo;			// view information for checking visibility

	int					m_rootNum;			// Number of root quad tiles
	GDM_QUAD_TILE**		m_arrRoots;			// array of root quad tiles

	bool				m_onWorking;

	GDM_TILE_INFO_ARR   m_sTileInfoArrOrg; // for calcu

	int					m_nMaxLevel;
	TilePool*           m_TilePool;
};
