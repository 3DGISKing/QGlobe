#include "GDM_PyramidMgr.h"
#include "GDM_RequestMgr.h"
#include "GDM_DataMgr.h"
#include "GDM_Camera.h"
#include "GDM_TileFunc.h"
#include "../gds_common/GDSDefine.h"

// some constants
const double		g_earthRadius = EARTH_RADIUS;
const double		g_maxCoord = 1.7976931348623158e+100 ;
const double		g_minCoord = -g_maxCoord;
const double		g_latLimit = 78 * gdm_DEGTORAD;

// level of root tiles
#define  GDM_ROOT_LEVEL		2

// Constants to determine level
#define	GDM_TILE_SPREAD			1.5
#define GDM_TILE_VIEW_RANGE		0.5
#define GDM_PYRAMID_MAX_RATE	2
#define GDM_PYRAMID_STANDARD_TILE_WIDTH	256

//	Update pyramid from camera
//	Generate pyramid quad tree and check them
//	It is called for pyramid calculation

void CGDM_PyramidMgr::Update()
{
	if (m_onWorking)
		return;

	m_onWorking = true;
	m_sTileInfoArrOrg.count = 0;

	UpdateViewInfo();
	GenerateVisibleQuadTree();

	SortTiles();

	m_nMaxLevel = m_sTileInfoArrOrg.maxLevel;

	if(!g_pGDMDataMgr->IsStopDataLoad())
	{
		g_pGDMDataMgr->m_pRequestMgr->UpdateRequest();
		g_pGDMDataMgr->SetModifyFlag();
	}
	m_onWorking = false;
}

//	Get Calculated pyramid tiles
void CGDM_PyramidMgr::GetPyramidTiles(GDM_TILE_INFO_ARR *psTileInfoArr)
{
	Q_ASSERT(psTileInfoArr != NULL);
		
	memcpy(psTileInfoArr, &m_sTileInfoArrOrg, sizeof(GDM_TILE_INFO_ARR));
}

//	Constructor
CGDM_PyramidMgr::CGDM_PyramidMgr(void)
{
	m_levelInfos = NULL;
	m_sTileInfoArrOrg.count = 0;

	m_onWorking = false;

	m_TilePool=new TilePool();

	InitRootQuadTiles();

	InitializeTileSetInfo();

	memset(&m_viewInfo, 0, sizeof(GDM_VIEW_INFO));
}

TilePool::TilePool()
{
	for(int i=0;i<100;i++)
	{
		GDM_QUAD_TILE* quadTile = (GDM_QUAD_TILE*) malloc(sizeof(GDM_QUAD_TILE));

		memset(quadTile,0,sizeof(GDM_QUAD_TILE));
		tilePool.push_back(quadTile);
	}

	usedCount=0;
}

GDM_QUAD_TILE* TilePool::GetOneTile()
{
	int size=tilePool.size();

	if(size==usedCount)
	{
		for(int i=0;i<100;i++)
		{
			GDM_QUAD_TILE* quadTile = (GDM_QUAD_TILE*) malloc(sizeof(GDM_QUAD_TILE));

			tilePool.push_back(quadTile);
		}
	}

	usedCount++;

	return tilePool[usedCount-1];
}

void TilePool::Clear()
{
	usedCount=8;
}

TilePool::~TilePool()
{
	for (int i=0;i<tilePool.size();i++)
		free(tilePool[i]);
}

//	Destructor
CGDM_PyramidMgr::~CGDM_PyramidMgr(void)
{
	if (m_levelInfos)
		free(m_levelInfos);

	if (m_arrRoots)
		free(m_arrRoots);

	if(m_TilePool)
		delete m_TilePool;
}

//	Initialize tile set informations
void CGDM_PyramidMgr::InitializeTileSetInfo()
{
	m_levelNum = MAX_LEVEL+1;
	m_levelInfos = (GDM_LEVEL_INFO*)malloc(m_levelNum * sizeof(GDM_LEVEL_INFO));

	if (!m_levelInfos)
		return;

	int dividNum;
	for (int i = 0; i < m_levelNum; i++)
	{
		m_levelInfos[i].level = i;
		dividNum = 1 << i;
		m_levelInfos[i].tileSize = gdm_TwoPI/ dividNum;
	}
}

//	Construct root quad tiles
//	Now, we start 8 tiles of 2 level because that they are valid in our case
void CGDM_PyramidMgr::InitRootQuadTiles()
{
	m_rootNum = 8;
	m_arrRoots = (GDM_QUAD_TILE**)malloc(sizeof(GDM_QUAD_TILE*) * m_rootNum);
	if (m_arrRoots == NULL)
		return;
	m_arrRoots[0] = MakeNewQuadTile(-gdm_PI, -gdm_HALF_PI, -gdm_HALF_PI, 0, GDM_ROOT_LEVEL);
	if (m_arrRoots[0])
	{
		m_arrRoots[0]->row = 1;
		m_arrRoots[0]->col = 0;
	}
	m_arrRoots[1] = MakeNewQuadTile(-gdm_HALF_PI, 0, -gdm_HALF_PI, 0, GDM_ROOT_LEVEL);
	if (m_arrRoots[1])
	{
		m_arrRoots[1]->row = 1;
		m_arrRoots[1]->col = 1;
	}
	m_arrRoots[2] = MakeNewQuadTile(0, gdm_HALF_PI, -gdm_HALF_PI, 0, GDM_ROOT_LEVEL);
	if (m_arrRoots[2])
	{
		m_arrRoots[2]->row = 1;
		m_arrRoots[2]->col = 2;
	}
	m_arrRoots[3] = MakeNewQuadTile(gdm_HALF_PI, gdm_PI, -gdm_HALF_PI, 00, GDM_ROOT_LEVEL);
	if (m_arrRoots[3])
	{
		m_arrRoots[3]->row = 1;
		m_arrRoots[3]->col = 3;
	}
	m_arrRoots[4] = MakeNewQuadTile(-gdm_PI, -gdm_HALF_PI, 0, gdm_HALF_PI, GDM_ROOT_LEVEL);
	if (m_arrRoots[4])
	{
		m_arrRoots[4]->row = 2;
		m_arrRoots[4]->col = 0;
	}
	m_arrRoots[5] = MakeNewQuadTile(-gdm_HALF_PI, 0, 0, gdm_HALF_PI, GDM_ROOT_LEVEL);
	if (m_arrRoots[5])
	{
		m_arrRoots[5]->row = 2;
		m_arrRoots[5]->col = 1;
	}
	m_arrRoots[6] = MakeNewQuadTile(0, gdm_HALF_PI, 0, gdm_HALF_PI, GDM_ROOT_LEVEL);
	if (m_arrRoots[6])
	{
		m_arrRoots[6]->row = 2;
		m_arrRoots[6]->col = 2;
	}
	m_arrRoots[7] = MakeNewQuadTile(gdm_HALF_PI, gdm_PI, 0, gdm_HALF_PI, GDM_ROOT_LEVEL);
	if (m_arrRoots[7])
	{
		m_arrRoots[7]->row = 2;
		m_arrRoots[7]->col = 3;
	}

	m_MaxLat=-gdm_HALF_PI;
	m_MinLat=gdm_HALF_PI;
	m_Maxlon=-gdm_PI;
	m_MinLon=gdm_PI;
}

// Start the root quad tile and recursively divide the quad tree
void CGDM_PyramidMgr::GenerateVisibleQuadTree()
{
	GDM_QUAD_TILE* root;
	// Destroy already created quad trees.
	// ..! To optimize, will remove invisible node.
	for (int i = 0; i < m_rootNum; i++)
	{
		root = m_arrRoots[i];
		root->child[0][0] = NULL;
		root->child[0][1] = NULL;
		root->child[1][0] = NULL;
		root->child[1][1] = NULL;
		//DestroyQuadTree(root);
	}

	m_TilePool->Clear();

	m_MaxLat=-gdm_HALF_PI;
	m_MinLat=gdm_HALF_PI;
	m_Maxlon=-gdm_PI;
	m_MinLon=gdm_PI;
	// generate visible quad trees newly
	for (int i = 0; i < m_rootNum; i++)
	{
		root = m_arrRoots[i];
		CheckQuadTile(root);
	}
}

// Create new quad tile instance and initialize it.
// To optimize, it's better to use pool instead of malloc
GDM_QUAD_TILE* CGDM_PyramidMgr::MakeNewQuadTile(IN double a_west, IN double a_east, IN double a_south, IN double a_north, IN char a_level)
{
	GDM_QUAD_TILE* quadTile = m_TilePool->GetOneTile();

	// Initialize the created quad tile
	quadTile->level = a_level;
	quadTile->west = a_west;
	quadTile->east = a_east;
	quadTile->south = a_south;
	quadTile->north = a_north;
	quadTile->centerLongitude = (quadTile->west + quadTile->east) * 0.5f;
	quadTile->centerLatitude = (quadTile->south + quadTile->north) * 0.5f;

	double offsetHeight = GDM_MAX_HEIGHT * g_pGDMDataMgr->m_sOption.dem_detail_rate;

	CalcBoundBox(a_west, a_east, a_south, a_north, g_earthRadius, g_earthRadius + offsetHeight, &quadTile->bonndBox);

	quadTile->row = quadTile->col = 0;
	quadTile->parent = NULL;
	quadTile->child[0][0] = NULL;
	quadTile->child[0][1] = NULL;
	quadTile->child[1][0] = NULL;
	quadTile->child[1][1] = NULL;

	return quadTile;
}

// Create 4 children for each quad tile
void CGDM_PyramidMgr::CreateChildQuadTiles(IN OUT GDM_QUAD_TILE* a_parent)
{
	if (a_parent == NULL) return;

	char childLevel = a_parent->level+1;
	double centerLat = a_parent->centerLatitude;
	double centerLon = a_parent->centerLongitude;
	GDM_QUAD_TILE* tile = NULL;

	// West South child
	tile = MakeNewQuadTile(a_parent->west, centerLon, a_parent->south, centerLat, childLevel);
	tile->parent = a_parent;
	tile->row = a_parent->row << 1;
	tile->col = a_parent->col << 1;
	a_parent->child[0][0] = tile;
	// East South child
	tile = MakeNewQuadTile(centerLon, a_parent->east,a_parent->south, centerLat, childLevel);
	tile->parent = a_parent;
	tile->row = a_parent->row << 1;
	tile->col = (a_parent->col << 1) + 1;
	a_parent->child[0][1] = tile;
	// West North child
	tile = MakeNewQuadTile(a_parent->west, centerLon, centerLat, a_parent->north, childLevel);
	tile->parent = a_parent;
	tile->row = (a_parent->row << 1) + 1;
	tile->col = a_parent->col << 1;
	a_parent->child[1][0] = tile;
	// East North child
	tile = MakeNewQuadTile(centerLon, a_parent->east, centerLat, a_parent->north, childLevel);
	tile->parent = a_parent;
	tile->row = (a_parent->row << 1) + 1;
	tile->col = (a_parent->col << 1) + 1;
	a_parent->child[1][1] = tile;
}

// Utility routine that converts Spherical coordinate to Cartesian coordinate
void CGDM_PyramidMgr::SphericalToCartesian(IN double a_latitude, IN double a_longitude, 
										   IN double a_radius, OUT CGDM_Vector3DF& a_xyz)
{
	double radCosLat = (double)(a_radius * cos(a_latitude));

	a_xyz.set((double) (radCosLat * cos(a_longitude)),
		(double) (radCosLat * sin(a_longitude)),
		(double) (a_radius * sin(a_latitude)));
}

// Calculate the bounding box of some part of the earth surface
void CGDM_PyramidMgr::CalcBoundBox(IN double a_west, IN double a_east, 
								   IN double a_south, IN double a_north, 
								   IN double a_radius1, IN double a_radius2, 
								   OUT GDM_BOUND_BOX* a_boundBox)
{
	CGDM_Vector3DF* ver;

	ver = a_boundBox->corners;
	
	a_boundBox->cornerCount = 0;

	SphericalToCartesian(a_south, a_west, a_radius1, ver[a_boundBox->cornerCount++]);
	SphericalToCartesian(a_south, a_east, a_radius1, ver[a_boundBox->cornerCount++]);
	SphericalToCartesian(a_north, a_east, a_radius1, ver[a_boundBox->cornerCount++]);
	SphericalToCartesian(a_north, a_west, a_radius1, ver[a_boundBox->cornerCount++]);
	SphericalToCartesian(a_south, a_west, a_radius2, ver[a_boundBox->cornerCount++]);
	SphericalToCartesian(a_south, a_east, a_radius2, ver[a_boundBox->cornerCount++]);
	SphericalToCartesian(a_north, a_east, a_radius2, ver[a_boundBox->cornerCount++]);
	SphericalToCartesian(a_north, a_west, a_radius2, ver[a_boundBox->cornerCount++]);

	if (a_west < m_viewInfo.cameraLongitude  && a_east > m_viewInfo.cameraLongitude)
	{
		SphericalToCartesian(a_south, m_viewInfo.cameraLongitude, a_radius1, ver[a_boundBox->cornerCount++]);
		SphericalToCartesian(a_north, m_viewInfo.cameraLongitude, a_radius1, ver[a_boundBox->cornerCount++]);
	}

	if (a_south < m_viewInfo.cameraLatitude && a_north > m_viewInfo.cameraLatitude)
	{
		SphericalToCartesian(m_viewInfo.cameraLatitude, a_east, a_radius1, ver[a_boundBox->cornerCount++]);
		SphericalToCartesian(m_viewInfo.cameraLatitude, a_west, a_radius1, ver[a_boundBox->cornerCount++]);
	}

	CGDM_Vector3DF minV, maxV;

	minV.set(g_maxCoord, g_maxCoord, g_maxCoord);
	maxV.set(g_minCoord, g_minCoord, g_minCoord);

	for (int i = 0; i < a_boundBox->cornerCount; i++)
	{
		if (minV.m_tX > ver[i].m_tX) minV.m_tX = ver[i].m_tX;
		if (minV.m_tY > ver[i].m_tY) minV.m_tY = ver[i].m_tY;
		if (minV.m_tZ > ver[i].m_tZ) minV.m_tZ = ver[i].m_tZ;

		if (maxV.m_tX < ver[i].m_tX) maxV.m_tX = ver[i].m_tX;
		if (maxV.m_tY < ver[i].m_tY) maxV.m_tY = ver[i].m_tY;
		if (maxV.m_tZ < ver[i].m_tZ) maxV.m_tZ = ver[i].m_tZ;
	}

	a_boundBox->min = minV;
	a_boundBox->max = maxV;

	a_boundBox->center = minV; 
	a_boundBox->center += maxV;
	a_boundBox->center *= 0.5;

	CGDM_Vector3DF tmp;

	tmp = maxV;
	tmp -= minV;

	a_boundBox->radius = tmp.getLength() * 0.5;
}

// Check if this tile covers camera position
bool CGDM_PyramidMgr::IsCoverCameraPos(IN GDM_QUAD_TILE* a_quadTile)
{
	double lat = m_viewInfo.cameraLatitude;
	double lon = m_viewInfo.cameraLongitude;

	return	(a_quadTile->south <= lat) && (a_quadTile->north >= lat) &&
			(a_quadTile->west <= lon)  && (a_quadTile->east >= lon);
}

// Check visibility of quad tile within View Frustum
// if too big, divide it into children
// if it has enough visibility, stop dividing and add it to the tile info list
// if it is not intersected with view frustum, ignore it.

void CGDM_PyramidMgr::CheckQuadTile(IN GDM_QUAD_TILE* a_quadTile)
{
	Q_ASSERT(a_quadTile != NULL);
	Q_ASSERT(m_levelInfos != NULL);

	bool isdivieded = false;

	GDM_LEVEL_INFO* levelInfo = &m_levelInfos[a_quadTile->level];

	if ( levelInfo->level > GDM_ROOT_LEVEL + 1)
	{
		if (IsCoverCameraPos(a_quadTile) == false)
		{
			// Test box's AABB with view frustum AABB
			isdivieded = IntersectBoundingBox(&a_quadTile->bonndBox, &m_viewInfo.frustumBonndBox);
			
			if (!isdivieded) return;

			// Check if quad tile intersects with view frustum
			isdivieded =IntersectWithFrustum(a_quadTile);
			
			if (!isdivieded) return;
		}

		isdivieded = a_quadTile->south < g_latLimit && a_quadTile->north > -g_latLimit;

		// Check if tile size of quad tile is too small
		if (isdivieded)
			isdivieded = m_viewInfo.cameraViewRange < levelInfo->tileSize * GDM_TILE_VIEW_RANGE * (1 + m_viewInfo.quality);

		// Check if position of quad tile is too far
		if (isdivieded)
		{
			double dist = SphericalDistance(a_quadTile->centerLatitude, a_quadTile->centerLongitude,
				m_viewInfo.cameraLatitude, m_viewInfo.cameraLongitude);
			isdivieded = dist < levelInfo->tileSize * GDM_TILE_SPREAD;
		}

		if (!isdivieded || a_quadTile->level == (m_levelNum - 1))
		{
			// Add this quad tile to tile info list
			Q_ASSERT(a_quadTile->level>=0);
			Q_ASSERT(a_quadTile->level<=18);
			Q_ASSERT(a_quadTile->col>=0);
			Q_ASSERT(a_quadTile->row>=0);

			AddTileInfo(a_quadTile);
			return;
		}
	}

	// Divide the quad tile into children and continue checking
	
	CreateChildQuadTiles(a_quadTile);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			CheckQuadTile(a_quadTile->child[i][j]);
}

bool CGDM_PyramidMgr::IntersectBoundingBox(IN GDM_BOUND_BOX* a_boxA, IN GDM_BOUND_BOX* a_boxB)
{
	if (a_boxA->max.m_tX < a_boxB->min.m_tX) return false;
	if (a_boxA->max.m_tY < a_boxB->min.m_tY) return false;
	if (a_boxA->max.m_tZ < a_boxB->min.m_tZ) return false;

	if (a_boxB->max.m_tX < a_boxA->min.m_tX) return false;
	if (a_boxB->max.m_tY < a_boxA->min.m_tY) return false;
	if (a_boxB->max.m_tZ < a_boxA->min.m_tZ) return false;

	return true;
}

// Check quad tile intersects with frustum
// if intersect, return true
bool CGDM_PyramidMgr::IntersectWithFrustum(IN GDM_QUAD_TILE* a_quadTile)
{
	GDM_BOUND_BOX* box = &a_quadTile->bonndBox;

	// Test box's sphere with frustum
	double d = 0;
	for (int j = 0 ; j < 6; j++)
	{
		d = m_viewInfo.frustumPlanes[j].getDistanceTo(box->center) - box->radius;
		if (d > 0)
			return false;
	}

	//	Test box's corners with planes of view frustum
	bool inside = false;
	for (int i = 0; i < 5; i++)	// far plane checking is not necessary because of backward checking before.
	{
		inside = false;
		for (int j = 0 ; j < box->cornerCount; j++)
		{

			d = m_viewInfo.frustumPlanes[i].getDistanceTo(box->corners[j]);
			if (d < 0)
			{
				inside = true;
				break;
			}
		}
		if(!inside)
			return false;
	}

	return true;
}

// Calculate the degree distance between two position
// Note that input values are radian and return value is radian
double CGDM_PyramidMgr::SphericalDistance(IN double a_latitude1, IN double a_longitude1, 
												IN double a_latitude2, IN double a_longitude2)
{
	double val = cos(a_latitude1) * cos(a_latitude2) * cos(a_longitude1 - a_longitude2) +
		sin(a_latitude1) * sin(a_latitude2);
	val = acos(val);

	return val;
}

// Add the quad tile info into tile info list
void CGDM_PyramidMgr::AddTileInfo(IN GDM_QUAD_TILE* a_quadTile)
{
	if(m_sTileInfoArrOrg.count >= GDM_MAX_TILE)
		return;

	int n = m_sTileInfoArrOrg.count;
	m_sTileInfoArrOrg.data[n].nLevel	= a_quadTile->level;
	m_sTileInfoArrOrg.data[n].nX		= a_quadTile->col;
	m_sTileInfoArrOrg.data[n].nY		= a_quadTile->row;
	m_sTileInfoArrOrg.data[n].cx		= a_quadTile->bonndBox.center.m_tX;
	m_sTileInfoArrOrg.data[n].cy		= a_quadTile->bonndBox.center.m_tY;
	m_sTileInfoArrOrg.data[n].cz		= a_quadTile->bonndBox.center.m_tZ;
	m_sTileInfoArrOrg.data[n].radius	= a_quadTile->bonndBox.radius;

	m_sTileInfoArrOrg.count++;

	if(m_MaxLat<a_quadTile->north)
		m_MaxLat=a_quadTile->north;
	
	if(m_Maxlon<a_quadTile->east)
		m_Maxlon=a_quadTile->east;

	if(m_MinLat>a_quadTile->south)
		m_MinLat=a_quadTile->south;

	if(m_MinLon>a_quadTile->west)
		m_MinLon=a_quadTile->west;
}

// Construct frustum matrix
void FrustumMatrix (double left, double right,
				    double bottom, double top,
				    double nearPlane, double farPlane,
				    CGDM_Matrix4& mat)
{
	double width = right-left;
	double height = top-bottom;
	double range = farPlane-nearPlane;
	double A = (right+left)/width;
	double B = (top+bottom)/height;
	double C = -(nearPlane+farPlane)/range;
	double D = -2*farPlane*nearPlane/range;
	double* m = mat.pointer();

	m[0] = 2*nearPlane/width; m[4] = 0; m[8] = A; m[12] = 0;
	m[1] = 0; m[5] = 2*nearPlane/height; m[9] = B; m[13] = 0;
	m[2] = 0; m[6] = 0; m[10] = C; m[14] = D;
	m[3] = 0; m[7] = 0; m[11] = -1; m[15] = 0;
}

// Construct perspective matrix
void PerspectiveMatrix (double fovy, double aspect, double nearPlane, double farPlane, CGDM_Matrix4& m)
{
	double ymax = nearPlane * tan ((fovy) / 2);
	FrustumMatrix (-ymax*aspect, ymax*aspect, -ymax, ymax, nearPlane, farPlane,	m);
}

// Extract view informations from camera 
void CGDM_PyramidMgr::UpdateViewInfo()
{
	CGDM_Camera *a_pCamera=g_pGDMDataMgr->m_pCamera;

	GDM_LOCATION location;

	a_pCamera->GetLocation(&location);

	m_viewInfo.cameraLatitude = location.m_dLatitude;
	m_viewInfo.cameraLongitude =location.m_dLongitude;
	
	double h = gdm_getHeight(m_viewInfo.cameraLongitude, m_viewInfo.cameraLatitude);

	// the level must be same for contour. RYM
	if(g_pGDMDataMgr->m_sOption.blContour)
		h = 0;

	double cameraAltitude = location.m_dDist - h;

	double rate = cameraAltitude / g_earthRadius;//???

	if (rate > 1)
		m_viewInfo.cameraViewRange = gdm_PI;
	else
		m_viewInfo.cameraViewRange = asin(rate) * 2;

	if (m_viewInfo.cameraViewRange < 0)
		m_viewInfo.cameraViewRange = -m_viewInfo.cameraViewRange ;

	GDM_Coord3D cameraCoord;
	a_pCamera->GetCameraCoord(&cameraCoord);

	m_viewInfo.quality		   = GDM_PYRAMID_MAX_RATE * a_pCamera->m_fScalarOfDir* a_pCamera->m_fScalarOfDir;	

	// Calculate six plane equations of view frustum
	// Camera view matrix

	CGDM_Matrix4 viewMat, projMat, combMat;

	gdm_MatrixSCStoTCS(&cameraCoord, &viewMat);

	GDM_ViewFrustum frustum;
	a_pCamera->GetViewFrustum(&frustum);

	PerspectiveMatrix(frustum.m_dFovy, frustum.m_dAspect, frustum.m_dNear, frustum.m_dFar, projMat);

	// Combine view and projection matrix
	combMat.setbyproduct_nocheck(projMat, viewMat);

	// calculate view frustum bounding box
	CalcVFBoundBox(&cameraCoord, &frustum, &m_viewInfo.frustumBonndBox);

	// Construct plane equations of view frustum

	CGDM_Vector3DF normal;
	double d;

	// left plane
	normal.set( -combMat(0,0) - combMat(0,3), 
				-combMat(1,0) - combMat(1,3), 
				-combMat(2,0) - combMat(2,3));

	d =	-combMat(3,0) - combMat(3,3);

	m_viewInfo.frustumPlanes[0].setPlane(normal,d);
	m_viewInfo.frustumPlanes[0].normalize();

	// right plane
	normal.set( combMat(0,0) - combMat(0,3), 
				combMat(1,0) - combMat(1,3), 
				combMat(2,0) - combMat(2,3));

	d =	combMat(3,0) - combMat(3,3) ;

	m_viewInfo.frustumPlanes[1].setPlane(normal,d);
	m_viewInfo.frustumPlanes[1].normalize();

	// bottom plane
	normal.set( -combMat(0,1) - combMat(0,3), 
				-combMat(1,1) - combMat(1,3), 
				-combMat(2,1) - combMat(2,3));

	d = -combMat(3,1) - combMat(3,3) ;

	m_viewInfo.frustumPlanes[2].setPlane(normal,d);
	m_viewInfo.frustumPlanes[2].normalize();

	// top plane
	normal.set( combMat(0,1) - combMat(0,3), 
				combMat(1,1) - combMat(1,3), 
				combMat(2,1) - combMat(2,3));

	d = combMat(3,1) - combMat(3,3) ;

	m_viewInfo.frustumPlanes[3].setPlane(normal,d);
	m_viewInfo.frustumPlanes[3].normalize();

	// near plane
	normal.set( -combMat(0,2) - combMat(0,3), 
				-combMat(1,2) - combMat(1,3), 
				-combMat(2,2) - combMat(2,3));

	d = -combMat(3,2) - combMat(3,3) ;

	m_viewInfo.frustumPlanes[4].setPlane(normal,d);
	m_viewInfo.frustumPlanes[4].normalize();

	// far plane	// it's not necessary
	normal.set( combMat(0,2) - combMat(0,3), 
				combMat(1,2) - combMat(1,3), 
				combMat(2,2) - combMat(2,3));

	d = combMat(3,2) - combMat(3,3) ;

	m_viewInfo.frustumPlanes[5].setPlane(normal,d);
	m_viewInfo.frustumPlanes[5].normalize();
}

void CGDM_PyramidMgr::CalcVFBoundBox(IN GDM_Coord3D* a_coord, IN GDM_ViewFrustum* a_frustum, OUT GDM_BOUND_BOX* a_boundBox)
{
	double xmax, ymax, zmax;
	zmax = a_frustum->m_dFar;
	ymax = zmax * tan( a_frustum->m_dFovy/2 );
	xmax = ymax * a_frustum->m_dAspect;
	double coff[5][3] = {{ xmax,  ymax, -zmax},
						 { xmax, -ymax, -zmax},
						 {-xmax, -ymax, -zmax},
						 {-xmax,  ymax, -zmax},
						 {0, 0, 0}};
	CGDM_Vector3DF minV, maxV, tmp;

	minV.set(g_maxCoord, g_maxCoord, g_maxCoord);
	maxV.set(g_minCoord, g_minCoord, g_minCoord);

	for (int i = 0; i < 5; i++)
	{
		tmp = (coff[i][0] * a_coord->m_x) + (coff[i][1] * a_coord->m_y) + (coff[i][2] * a_coord->m_z) + a_coord->m_org;
		if (minV.m_tX > tmp.m_tX) minV.m_tX = tmp.m_tX;
		if (minV.m_tY > tmp.m_tY) minV.m_tY = tmp.m_tY;
		if (minV.m_tZ > tmp.m_tZ) minV.m_tZ = tmp.m_tZ;

		if (maxV.m_tX < tmp.m_tX) maxV.m_tX = tmp.m_tX;
		if (maxV.m_tY < tmp.m_tY) maxV.m_tY = tmp.m_tY;
		if (maxV.m_tZ < tmp.m_tZ) maxV.m_tZ = tmp.m_tZ;
	}
	a_boundBox->min = minV;
	a_boundBox->max = maxV;
}


/* 점과 가장 가까운 nLevel에 해당하는 4개 tile을 얻는다.*/
void CGDM_PyramidMgr::GetNear4Tiles(int nLevel, double dLongitude, double	dLatitude, GDM_TILE_INFO_ARR *psTileInfoArr)
{
	double tileWidth, dX, dY;
	int nX, nY, nOffsetX, nOffsetY, index = 0, nMaxX, nMaxY, nNewX, nNewY;

	tileWidth = PAI * 2 / (1<<nLevel);

	dX = dLongitude + PAI;
	dY = dLatitude  + PAI;

	memset(psTileInfoArr, 0, sizeof(GDM_TILE_INFO_ARR));

	// get Include tile
	nX = (int)(dX / tileWidth);
	nY = (int)(dY / tileWidth);
	psTileInfoArr->data[index].nX = nX;
	psTileInfoArr->data[index].nY = nY;
	psTileInfoArr->data[index].nLevel = nLevel;
	psTileInfoArr->count++;
	index++;

	nMaxX = 1 << nLevel;
	nMaxY = 1 << nLevel;

	// get offset
	if((dX - (nX * tileWidth)) > (tileWidth / 2))
		nOffsetX = 1;
	else
		nOffsetX = -1;

	if((dY - (nY * tileWidth)) > (tileWidth / 2))
		nOffsetY = 1;
	else
		nOffsetY = -1;

	// get offset tile
	nNewX = (nX + nOffsetX + nMaxX) % nMaxX;
	nNewY = nY;
	if(!gdm_IsContained(nNewX, nNewY, psTileInfoArr))
	{
		psTileInfoArr->data[index].nX = nNewX;
		psTileInfoArr->data[index].nY = nNewY;
		psTileInfoArr->data[index].nLevel = nLevel;
		psTileInfoArr->count++;
		index++;
	}

	// get offset tile
	nNewX = nX;
	nNewY = (nY + nOffsetY + nMaxY) % nMaxY;
	if(!gdm_IsContained(nNewX, nNewY, psTileInfoArr))
	{
		psTileInfoArr->data[index].nX = nNewX;
		psTileInfoArr->data[index].nY = nNewY;
		psTileInfoArr->data[index].nLevel = nLevel;
		psTileInfoArr->count++;
		index++;
	}

	// get offset tile
	nNewX = (nX + nOffsetX + nMaxX) % nMaxX;
	nNewY = (nY + nOffsetY + nMaxY) % nMaxY;
	if(!gdm_IsContained(nNewX, nNewY, psTileInfoArr))
	{
		psTileInfoArr->data[index].nX = nNewX;
		psTileInfoArr->data[index].nY = nNewY;
		psTileInfoArr->data[index].nLevel = nLevel;
		psTileInfoArr->count++;
		index++;
	}
}

void CGDM_PyramidMgr::CheckAndCompleteTiles()
{
	GDM_TILE_INFO *pTile, *pOther;
	double len, compareLen;
	int nX, nY, nLevel;
	bool	checkDivision = true;

	compareLen = GDM_PYRAMID_STANDARD_TILE_WIDTH * (1 - g_pGDMDataMgr->m_sOption.dem_quality_rate) + GDM_PYRAMID_STANDARD_TILE_WIDTH / 2;
	int count = m_sTileInfoArrOrg.count;

	for(int i = 0; i <m_sTileInfoArrOrg.count; i++)
	{
		pTile = &m_sTileInfoArrOrg.data[i];
		pTile->tileWidth = gdm_TwoPI / (1 << pTile->nLevel);
		pTile->minLongitude = pTile->tileWidth * pTile->nX - gdm_PI;
		pTile->minLatitude	= pTile->tileWidth * pTile->nY - gdm_PI;

		// get area
		pTile->area2D = g_pGDMDataMgr->m_pCamera->GetAreaInScreen(pTile->minLongitude, pTile->minLongitude + pTile->tileWidth, 
			pTile->minLatitude, pTile->minLatitude + pTile->tileWidth);

		len = sqrt(pTile->area2D);
		if(len < 0.1)
			len = 0.1;

		// get offset level and prio
		pTile->offsetLevel = log(compareLen / len) / log(2.0); 
	
		if(m_sTileInfoArrOrg.count > (GDM_MAX_TILE - 4) || (i >= count))
			continue;

		if(pTile->minLatitude > g_latLimit || (pTile->minLatitude + pTile->tileWidth) < (-g_latLimit))
			continue;

		// we divide current tile into higher four child tiles because current tile is larger

		if(pTile->nLevel < MAX_LEVEL && checkDivision && pTile->offsetLevel < 0)
		{
			nX = pTile->nX;
			nY = pTile->nY;
			nLevel = pTile->nLevel;

			for(int k = 0; k < 4; k++)
			{
				if(k == 0)
					pOther = &m_sTileInfoArrOrg.data[i];   //level of this tile will be increased 
				else
				{
					pOther = &m_sTileInfoArrOrg.data[m_sTileInfoArrOrg.count]; //three new tile is added
					m_sTileInfoArrOrg.count++;
				}

				pOther->nLevel = nLevel + 1;
				pOther->nX	   = nX * 2 + k % 2;
				pOther->nY	   = nY * 2 + k / 2;
			}
			checkDivision = false;                  // increased this tile is not checked
			i--;
			continue;
		}

		checkDivision = true;
	}
}

void CGDM_PyramidMgr::SortTiles()
{
	Q_ASSERT(m_sTileInfoArrOrg.count <= GDM_MAX_TILE);

	GDM_TILE_INFO *pTile1, *pTile2, sTileTmp;

	m_sTileInfoArrOrg.maxLevel = 0;

	CheckAndCompleteTiles();

	for(int i = 0; i <m_sTileInfoArrOrg.count - 1; i++)
	{
		pTile1 = &m_sTileInfoArrOrg.data[i];
		for(int j = i + 1; j < m_sTileInfoArrOrg.count; j++)
		{
			pTile2 = &m_sTileInfoArrOrg.data[j];

			if(pTile1->area2D < pTile2->area2D)
			{
				memcpy(&sTileTmp, pTile1, sizeof(GDM_TILE_INFO));
				memcpy(pTile1, pTile2, sizeof(GDM_TILE_INFO));
				memcpy(pTile2, &sTileTmp, sizeof(GDM_TILE_INFO));
			}
		}
		if(pTile1->nLevel > m_sTileInfoArrOrg.maxLevel)
			m_sTileInfoArrOrg.maxLevel = pTile1->nLevel;

		if(pTile1->area2D < 1)
		{
			m_sTileInfoArrOrg.count = i;
			break;
		}
	}
}

bool CGDM_PyramidMgr::Intersect(CGDM_Aabbox3DF box)
{
	if (box.MaxEdge.m_tX < m_viewInfo.frustumBonndBox.min.m_tX) return false;
	if (box.MaxEdge.m_tY < m_viewInfo.frustumBonndBox.min.m_tY) return false;
	if (box.MaxEdge.m_tZ < m_viewInfo.frustumBonndBox.min.m_tZ) return false;

	if (box.MaxEdge.m_tX < m_viewInfo.frustumBonndBox.min.m_tX) return false;
	if (box.MaxEdge.m_tY < m_viewInfo.frustumBonndBox.min.m_tY) return false;
	if (box.MaxEdge.m_tZ < m_viewInfo.frustumBonndBox.min.m_tZ) return false;

	return true;
}
