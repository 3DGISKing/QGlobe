#pragma once

/*
 * Name: QGlobe_TileInfo.h
 *
 * Description: 
 *   Internal API definitions about the internal structure of tile
 *   headers.
 *
 * Function List:
 *
 */

#include "../qglobe_common/qglobe_protocol.h"
#include "../qglobe_base/QGlobe_List.h"

#define MAX_TILE_LEVEL			20
#define QGlobe_MAX_TILE	        150
#define QGLOBE_MAX_DEM_DIVISION	128
#define QGLOBE_MAX_DEM_DIVISION2	256
#define QGLOBE_MAX_DEM_DIVISION3	384
#define QGLOBE_MAX_DEM_DIVISION4	512

#define QGLOBE_MAX_DEM_PT_COUNT	(QGLOBE_MAX_DEM_DIVISION + 1)
#define QGLOBE_MAX_IMG_DIVISION	256

/* An enumerated type to specify the status of tile */
typedef enum {
	E_TILE_STATUS_NONE,			/* no process tile */
	E_TILE_STATUS_REQ,          /* request tile */
	E_TILE_STATUS_RESP_DATA,    /* response tile with data*/
	E_TILE_STATUS_RESP_NONE,	/* response tile with non-exist */
	E_TILE_STATUS_RESP_SEA,     /* response tile with sea */
} E_TILE_STATUS;

typedef struct _gdmTileInfo{
	int					nX;             // the x coordinate of the tile
	int					nY;             // the y coordinate of the tile
	int					nLevel;         // the level number of the tile
	E_QGlobe_MAINDATA_TYPE eMainType;		// main data type
	E_QGlobe_SUBDATA_TYPE	eSubType;       // sub data type
	//float				prio;			// the priority of the tile
	int					nSize;          // the size of the tile in bytes
	float				area2D;			// the area in screean
	int					offsetLevel;    // the offset level for high speed render
	double				minLongitude;	// min logititude in radian
	double				minLatitude;	// min latitude in radian
	double				tileWidth;		// tile width in radian
	E_TILE_STATUS		nStatus;	    // the status of tile
	double				cx,cy,cz,radius;
}QGlobe_TILE_INFO;

typedef struct _gdmTileInfoArr{
	int	           count;				/* the count of tile */
	int			   maxLevel;
	QGlobe_TILE_INFO  data[QGlobe_MAX_TILE];
}QGlobe_TILE_INFO_ARR;

// common tile structure
typedef struct _gdmTile
{
	QGlobe_TILE_INFO	m_sInfo;			// tile information
	char		    m_isValidBuffer;	// 0: no invalid buffer, 1: valid buffer 
	char			m_pBuffer[1];		// the buffer of tile for individual
}QGlobe_TILE;

// texture tile structure
typedef struct _gdemTexTureTile
{
	QGlobe_TILE_INFO	m_sInfo;			// tile information
	char		    m_isValidBuffer;	// 0: no invalid buffer, 1: valid buffer 

	int				m_nBufSize;			// the buffer size
	char			m_pBuffer[1];		// the image buffer of tile
}QGlobe_TEXTURE_TILE;

// dem tile structure
typedef struct _gdmDemTile
{
	QGlobe_TILE_INFO	m_sInfo;			// tile information
	char		    m_isValidBuffer;	// 0: no invalid buffer, 1: valid buffer 

	double			minLongitude;		// in radian
	double			maxLongitude;		// in radian
	double			minLatitude;		// in radian
	double			maxLatitude;		// in radian
	double			tileWidth;			// width of dem tile in radian
	double			gridWidth;			// width of grid in radian
	short			m_pData[QGLOBE_MAX_DEM_PT_COUNT * QGLOBE_MAX_DEM_PT_COUNT];	// the buffer of height
}QGlobe_DEM_TILE;

typedef struct _gdmPoint2D
{
	double			m_X;
	double			m_Y;
}QGlobe_POINT2D;



typedef struct _gdmPolygon
{	
	unsigned int		m_nCount;			// the count of vertex
	int                 ID;  

	//added by RSH 2013 3 3 for bounding box
	double m_bMinX;
	double m_bMinY;
	double m_bMaxX;
	double m_bMaxY;
	//end
	QGlobe_POINT2D			m_pVertex[1];
}QGlobe_POLYGON;

// shape tile structure
typedef struct _gdmShapeTile
{
	QGlobe_TILE_INFO	m_sInfo;			// tile information
	char		    m_isValidBuffer;	// 0: no invalid buffer, 1: valid buffer 

	int				m_nBufSize;			// the buffer size
	int				m_nPolygonCount;	// the polygon count
	QGlobe_POLYGON		m_pData[1];		    // the shape buffer of tile
}QGlobe_SHAPE_TILE;

typedef struct tagGEODATA_SHAPELIST
{
	unsigned int	type;
	unsigned int	m_nCount;			// the count of polygon	
	QGlobe_POLYGON		m_pPolygon[1];
} QGlobe_SHAPE_DATA;

typedef struct _gdmPoint3D
{
	double			m_X;
	double			m_Y;
	double			m_Z;
}QGlobe_POINT3D;

typedef struct _qglobe_direction_info
{
	float	norX;		// x value of normal vector		0 ~ 1
	float	norY;		// x value of normal vector		0 ~ 1
	float	norZ;		// x value of normal vector		0 ~ 1
	short	angle;		// angle of normal vector		0 ~ 90
	short	direction;	// direction of normal vector	0 ~ 360
}QGlobe_DIRECTION_INFO;








