#pragma once

#include "GDM_Tile.h"
#include "../gdem_base/GDM_Vector3D.h"
#include "../gdem_base/GDM_Aabbox3D.h"
#include "../gdem_base/GDM_Triangle3d.h"
#include "GDM_Interface.h"

#define GDM_MAX_CHILD_TYPE_COUNT	10
#define GDM_MAX_NAME_CHILD_TYPE_COUNT	34

#define MAX_VERTEX_BUFFER_SIZE  GDEM_MAX_DEM_PT_COUNT*GDEM_MAX_DEM_PT_COUNT*30
#define MAX_INDEX_BUFFER_SIZE  GDEM_MAX_DEM_PT_COUNT*GDEM_MAX_DEM_PT_COUNT*100

typedef unsigned char BOUND_COND[GDEM_MAX_DEM_DIVISION4+1];
typedef GDM_POINT3D   BOUNDARY[GDEM_MAX_DEM_DIVISION4+1];

int intersect_RayTriangle(Vector3DF rayOrg, Vector3DF rayDir, Triangle3DF T, Vector3DF *destPt, double *distance, bool flag = 1);

// the logical tile structure for render
typedef struct _gdmLogicTile
{
	int					nX;				// the x coordinate of the tile 
	int					nY;             // the y coordinate of the tile 
	int					nLevel;         // the level number of the tile 
	double				prio;
	int					offsetLevel;

	double				minLongitude;
	double				maxLongitude;
	double				minLatitude;
	double				maxLatitude;
	double				bndRadius;
	double				area;

	GDM_POINT3D			edge[4];					// the edge point
	CGDM_Vector3DF		bndCenter;

	GDM_DEM_TILE*		pDemTile;					// covered Dem tile
	GDM_DEM_TILE*		pContourDemTile;			// covered Dem tile for contour
	GDM_TEXTURE_TILE*	pImgTile;					// covered Image tile
	GDM_TILE*	prasterTile;               


	GDM_SHAPE_TILE*		sArrShapeTile [GDM_MAX_CHILD_TYPE_COUNT];	// shape objects tile
	GDM_TILE*           sArr3DMeshTile[GDM_MAX_CHILD_TYPE_COUNT];
	GDM_TILE*           sArrPolygonTile[GDM_MAX_CHILD_TYPE_COUNT];
	GDM_TILE*			sArrNameTile  [GDM_MAX_NAME_CHILD_TYPE_COUNT];	// shape objects tile
	int					nShapeTileCnt;
	int					nNameTileCnt;
	int                 n3dMeshTileCnt;
	int                 nPoygonTileCnt;
}GDM_LOGIC_TILE;

// the logical tile list for render
typedef struct _gdmLogicTileArr
{
	int	nLevel;
	int nCount;
	GDM_LOGIC_TILE pData[GDM_MAX_TILE];
}GDM_LOGIC_TILE_ARR;

typedef struct _gdmGridViewInfo
{
	double dLongiResolution;
	double dLatiResolution;
	double dMinLongi;
	double dMidMinLongi;
	double dMaxLongi;
	double dMidMaxLongi;
	double dMidLongi;
	double dMinLati;
	double dMaxLati;
	double dRealLongiStart;
	double dRealLatiStart;
	double dRealLongiEnd;
	double dRealLatiEnd;	
	double dRealMidMinLongi;
	double dRealMidMaxLongi;
	int nLongiCount;
	int nLatiCount;
	int nMaxLongiResolution;
	int nMaxLatiResolution;
	int nMinLongiResolution;
	int nMinLatiResolution;
}GDM_GRID_VIEW_INFO;

// the logical tile structure for render
typedef struct _gdmMeshTile
{
	int					nX;				// the x coordinate of the tile 
	int					nY;             // the y coordinate of the tile 
	int					nLevel;         // the level number of the tile 

	double				minLong;		// min longitude
	double				minLati;		// min latitude

	GDM_POINT3D*		buf_dem;		// dem buffer
	short*				buf_height;		// dem buffer
	GDM_DIRECTION_INFO* buf_dirInfo;	// dem normal vector info
	unsigned long		nTextureID;		// textureID
	unsigned long		nOverlayTextureID;		// over;ay textureID
	float				fTextureLeft;	// texture coord of row
	float				fTextureBottom;	// texture coord of Colum
	float				fTextureWidth;	// texture coord of Colum

	int					nDivision;		// the division of tile
	int					nDemDivision;
	int					nSubDivision;

	BOUND_COND			mBC;
	BOUND_COND			mBU;
	BOUNDARY			mBP;

	int					nSubX;
	int					nSubY;
	int					nSubs;
	GDM_DEM_TILE*		pDemTile;	// for height in small tile		
}GDM_MESH_TILE;

// the logical tile list for render
typedef struct _gdmMeshTileArr
{
	int	nLevel;
	int nCount;
	GDM_MESH_TILE pData[GDM_MAX_TILE];
}GDM_MESH_TILE_ARR;

typedef struct _NodeIndex
{
	unsigned short start;
	unsigned short end;
} NodeIndex;




typedef struct _QuadTree
{
	int nX, nY, nLevel;
	int ngridSize;
	NodeIndex		m_nIndexArray[400];
	Aabbox3DF		m_nBboxArray[400];		
	
	Vector3DF		*m_pVertexBuf;
	unsigned short	*m_pIndexBuf;
	Triangle3DF		m_pColTri;

	Triangle3DF		m_pLeftQuad;
	Triangle3DF		m_pRightQuad;
		
	BOUND_COND	m_BC;
	int			m_nVertex;
	int			m_nIndices;
	float		priority;	
	int			LRU;
	double		LRUpriority;	

	double				minLongitude;
	double				minLatitude;
	double				tileWidth;	

	bool isUpdated(BOUND_COND &bc,int gridsize)
	{
		bool flag = memcmp(m_BC, bc, sizeof(BOUND_COND));
		return (flag || gridsize != ngridSize);
	}

	int getHeight(double dLongitude, double dLatitude, double &a_Height);
	int getIntersection(Vector3DF p, Vector3DF v, double* lamda, int index);
	int getIntersectionAll(Vector3DF rayOrg, Vector3DF rayDir, double* lamda);
	bool intersectsBoxWithLine(int index, Vector3DF rayOrg, Vector3DF rayDir, int *idx);

	_QuadTree *pNext;
	_QuadTree *pPrev;

	_QuadTree *pLRUNext;
	_QuadTree *pLRUPrev;

	_QuadTree():nX(0),nY(0),nLevel(0),ngridSize(0),m_nVertex(0),m_nIndices(0),m_pIndexBuf(0),priority(0),LRU(0){}
} QuadTree; 

typedef struct _TerrainQuadTreeList
{
	QuadTree quadTrees[GDM_MAX_TILE];
	
	QuadTree		*LRUHead;
	Vector3DF		pVertexBuf[MAX_VERTEX_BUFFER_SIZE];
	unsigned short	pIndexBuf[MAX_INDEX_BUFFER_SIZE];	
	Vector3DF		*pVBuf;
	unsigned short	*pIBuf;

	bool LRUUsage;

	GDM_LOCATION	m_CameraLocation;	
	int  m_nUsed;

	QuadTree& operator [](int index)
	{
		return quadTrees[index];
	}
	QuadTree * getbyPosition(double dLongitude, double dLatitude)
	{
		QuadTree *tmp;
		double lati,longi;
		for (int i = 0; i < m_nUsed;i++)
		{
			tmp = &quadTrees[i];	
			lati = dLatitude-tmp->minLatitude;
			longi = dLongitude-tmp->minLongitude;
			if (lati <= tmp->tileWidth && lati >= 0 &&
				 longi <= tmp->tileWidth && longi >= 0)
				return tmp;			
		}
	
		return 0;
	}

	QuadTree * quickSortTiles(Vector3DF rayOrg, Vector3DF rayDir, int &qtree)
	{		
		QuadTree *newSort = 0, *next, *tmp;
		Vector3DF mid, VP;
		Vector3DF pos;
			
		double mindis;
		double diag;

		qtree = -1;
		for (int i = 0; i < m_nUsed;i++)
		{
			tmp = &quadTrees[i];			
			if (intersect_RayTriangle(rayOrg,rayDir,tmp->m_pLeftQuad,&pos,&mindis) ||
				intersect_RayTriangle(rayOrg,rayDir,tmp->m_pRightQuad,&pos,&mindis))
			{					
				qtree = i;
				continue;
			}
			mid = tmp->m_nBboxArray[0].getCenter();
			diag = (tmp->m_nBboxArray[0].MaxEdge-mid).getLengthSQ();
			VP = (mid-rayOrg);
			mindis = VP.dotProduct(rayDir);			
			pos = rayOrg+rayDir*mindis;	
			if ((pos-mid).getLengthSQ() < diag)
			{
				tmp->priority = fabs(mindis) + fabs(rayDir.dotProduct(VP.normalize()));
				next = newSort;
				if (!newSort){
					newSort = tmp;
					tmp->pPrev = tmp->pNext = 0;					
					continue;
				}
				if (next->priority > tmp->priority)
				{
					newSort = tmp;
					tmp->pNext = next;
					next->pPrev = tmp;
					tmp->pPrev = 0;
				}
				else
				{					
					while(next->pNext && next->pNext->priority < tmp->priority){
						next = next->pNext;
					}							
					if (next->pNext) 
						next->pNext->pPrev = tmp;
					
					tmp->pNext = next->pNext;
					next->pNext = tmp;
					tmp->pPrev = next;					
				}
			}							
		}
		return newSort;				
	}

	Vector3DF *getVBuffer(int size)
	{
		if (MAX_VERTEX_BUFFER_SIZE-(pVBuf-pVertexBuf) < size){			
			return 0;
		}
		pVBuf+= size;
		return (pVBuf-size);
	}

	unsigned short *getIBuffer(int size)
	{
		if (MAX_INDEX_BUFFER_SIZE-(pIBuf-pIndexBuf) < size){			
			return 0;
		}		
		return pIBuf;
	}

	void copyIndexBuffer(QuadTree *dst, QuadTree *src)
	{
		if (MAX_INDEX_BUFFER_SIZE-(pIBuf-pIndexBuf) < src->m_nIndices){
			dst->ngridSize= -1;
			dst->m_pIndexBuf = src->m_pIndexBuf;
			dst->m_nIndices = src->m_nIndices;
			return;
		}
		memcpy(pIBuf, src->m_pIndexBuf, src->m_nIndices*sizeof(unsigned short));
		dst->m_pIndexBuf = pIBuf;
		dst->m_nIndices = src->m_nIndices;
		pIBuf = &pIBuf[src->m_nIndices+1];		
	}
		
	QuadTree *find_exist(int nx, int ny, int nlevel)
	{		
		int index = 0;
		for (index = 0; index < m_nUsed; index++){
			if (quadTrees[index].nX == nx &&
				quadTrees[index].nY == ny &&
				quadTrees[index].nLevel == nlevel)
				return &quadTrees[index];						
		}
		return 0;
	}

	void start();
	
	void end()
	{		

	}

	QuadTree *add(int nX, int nY,int nLevel, BOUND_COND &aBC)
	{	
		QuadTree *qtree = &quadTrees[m_nUsed++];			
		qtree->nX = nX;
		qtree->nY = nY;
		qtree->nLevel = nLevel;
		qtree->ngridSize = -1;
		memcpy(qtree->m_BC, aBC, sizeof(BOUND_COND));				
		
		return qtree;
	}

	QuadTree *add(QuadTree *other)
	{		
		QuadTree *qtree = &quadTrees[m_nUsed++];			
		qtree->nX = other->nX;
		qtree->nY = other->nY;
		qtree->nLevel = other->nLevel;
		qtree->ngridSize = other->ngridSize;
		memcpy(qtree->m_BC, other->m_BC, sizeof(BOUND_COND));				
		memcpy(qtree->m_nBboxArray, other->m_nBboxArray, sizeof(other->m_nBboxArray));
		memcpy(qtree->m_nIndexArray, other->m_nIndexArray, sizeof(other->m_nIndexArray));
		qtree->m_pIndexBuf = getIBuffer(other->m_nIndices);
		qtree->m_pVertexBuf = getVBuffer(other->m_nVertex);
		if (!qtree->m_pIndexBuf || !qtree->m_pVertexBuf)
		{
			m_nUsed--;
			return 0;
		}
		memcpy(qtree->m_pIndexBuf,  other->m_pIndexBuf, sizeof(unsigned short)*other->m_nIndices);
		memcpy(qtree->m_pVertexBuf, other->m_pVertexBuf, sizeof(Vector3DF)*other->m_nVertex);
		pIBuf = &qtree->m_pIndexBuf[other->m_nIndices+2];
		qtree->m_nIndices = other->m_nIndices;
		qtree->m_nVertex  = other->m_nVertex;

		qtree->m_pLeftQuad = other->m_pLeftQuad;
		qtree->m_pRightQuad = other->m_pRightQuad; 
		
		qtree->minLongitude = other->minLongitude;
		qtree->minLatitude  = other->minLatitude;
		qtree->tileWidth = other->tileWidth;			
		
		calcLRU(qtree,qtree->minLongitude+qtree->tileWidth/2,qtree->minLatitude+qtree->tileWidth/2);
		updateLRU(qtree);
		return qtree;
	}

	void calcLRU(QuadTree *qtree, double dLongitude, double dLatitude)
	{
		qtree->LRUpriority = (dLatitude - m_CameraLocation.m_dLatitude)*(dLatitude - m_CameraLocation.m_dLatitude)+
			(dLongitude - m_CameraLocation.m_dLongitude)*(dLongitude - m_CameraLocation.m_dLongitude);
	}
	void updateLRU(QuadTree *qtree)
	{		
		if (!LRUHead){
			LRUHead = qtree;
			qtree->pLRUNext = 0;
			qtree->pLRUPrev = 0;
			return;
		}

		QuadTree *head = LRUHead;
		if (LRUHead->LRUpriority > qtree->LRUpriority)
		{
			LRUHead = qtree;
			qtree->pLRUNext = head;
			head->pLRUPrev = qtree;
			qtree->pLRUPrev = 0;
			return;
		}
		
		while(head->pLRUNext && head->pLRUNext->LRUpriority < qtree->LRUpriority)
		{
			head = head->pLRUNext;
		}
		qtree->pLRUNext = head->pLRUNext;
		if (head->pLRUNext)
			head->pLRUNext->pLRUPrev = qtree;
		head->pLRUNext = qtree;
		qtree->pLRUPrev = head;		
	}

	_TerrainQuadTreeList():m_nUsed(0),LRUUsage(0),LRUHead(0){};
}TerrainQuadTreeList;

typedef struct _TreeListSelector
{
	TerrainQuadTreeList m_pBuf[2];	
	int m_nWriteBuf;
	int m_nReadBuf;	
	

	TerrainQuadTreeList *GetBufForRead()
	{		
		if (m_nReadBuf < 0){
			return 0;
		}
		return &m_pBuf[m_nReadBuf];		
	}
	
	TerrainQuadTreeList *GetBufForWrite()
	{		
		return &m_pBuf[m_nWriteBuf];		
	}

	void SetReadBuf()
	{
		m_nReadBuf = m_nWriteBuf;
		m_nWriteBuf = 1-m_nReadBuf;
	}
	void SetLRUUsage(bool usage)
	{
		m_pBuf[0].LRUUsage = usage;		
		m_pBuf[1].LRUUsage = usage;
	}
	_TreeListSelector():m_nReadBuf(-1), m_nWriteBuf(0){}
}TreeListSelector;

extern TreeListSelector	   *gdm_TileListSelector;	


