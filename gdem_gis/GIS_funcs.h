#pragma once

#include "../gdem_libmain/GDM_Tile.h"
#include "GDM_Vector3D.h"
#include "GDM_Array.h"
#include "GDM_Location.h"
#include "GDM_Define.h"
#include <math.h>
#include "GDM_Mem.h"
#include "GDM_Coord3D.h"
#include "gdemclientbase.h"

#define GDM_MAX_DEM_LEVEL	13

// calculate functions

// CGDM_LocationArr: ��(�浵, ����, ����)���

// ������� 
double			gdem_GetAreaInMesh(GDEM_MESH &mesh);
// ü�����
double          gdem_GetVolumeInMesh(GDEM_MESH &mesh, double height);

// ���̰��
double			gdem_GetLengthWithPolygon(CGDM_LocationArr &arrPt, bool bClose);
// �ִ����� ���
void			gdem_GetMaxRatePos(IN CGDM_LocationArr &arrPt, OUT GDM_POSITION& pos);
// �ִ�/�ּҳ��� ���
void			gdem_GetMinMaxHeightPos(IN CGDM_LocationArr &arrPt, OUT GDM_POSITION& minPos, OUT GDM_POSITION& maxPos);
// 3�����Ҹ���
void			gdem_DivisionToTriangle(IN CGDM_LocationArr &arrPt, OUT CGDM_MeshArr &meshArr);


void gdem_GetLocationFrom3DPoint(CGDM_Vector3DF &i_pt, CGDM_Location3D &o_pt);

// get height from location in dem tile
double			gdem_GetHeightFromDemTile(CGDM_Location3D &i_pt, GDM_DEM_TILE* pTile);

// �浵,����, ���̷κ��� 3���� ��ǥ�����
void			gdem_Get3DPointFromLocation(CGDM_Location3D &i_pt, CGDM_Vector3DF &o_pt, double &height);

// ���а� x = a (y�࿡ ������ ����) ���� �����
bool gdem_GetCrossPtSegmentWithAxisY (double xConst, double minY, double maxY, CGDM_Location3D *pPt1, CGDM_Location3D *pPt2, CGDM_Location3D &crossPt);

// ���а� y = a (x�࿡ ������ ����) ���� �����
bool gdem_GetCrossPtSegmentWithAxisX (double yConst, double minX, double maxX, CGDM_Location3D *pPt1, CGDM_Location3D *pPt2, CGDM_Location3D &crossPt);

// �ϳ��� ��(�׸���)�� ����ڰ�輱�� ������� �Ͽ� �����Ǵ� ��Ѽ����� ��� ���.
int  gdem_GetCrossLineArrWithGrid(CGDM_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &io_crossLineArr, bool blClose = true);

/*
struct LINE_SEGEMENT
{
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2;
	LINE_SEGEMENT(double x1, double y1, double x2, double y2) 
	{
		m_x1 = x1;
		m_y1 = y1;
		m_x2 = x2;
		m_y2 = y2;
	}
};
*/

int				sign(float value) ;

// check point in polygon
bool			gdem_PtIsInPolygon(CGDM_Location3D &i_pt, CGDM_LocationArr &i_arrPt);

int				gdem_LineIsInIntersectWithTriangle(CGDM_Location3D &i_startPt, CGDM_Location3D &i_endPt, CGDM_Vector3DF *i_Pt1 , CGDM_Vector3DF *i_Pt2 , CGDM_Vector3DF *i_Pt3, CGDM_Vector3DF &i_destPt);



// �� ���� ���̴� Ÿ���������.
GDM_TILE_INFO	gdem_GetTileFromPt(CGDM_Location3D i_Pt, int i_Level);

//������ ��ʹ� Ÿ�ϵ��� �������.
GDM_TILE_INFO *	gdem_GetTilesFromLine(CGDM_Location3D startPos, CGDM_Location3D endPos , int i_Level, int *o_TileCount);

//Polygon�� ��ʹ� Ÿ�ϵ��� �������.
GDM_TILE_INFO *	gdem_GetCrossTilesFromPolygon(CGDM_LocationArr &i_arrPt,  int i_Level, int &o_TileCount);

//PolyLine�� ��ʹ� Ÿ�ϵ��� �������.
GDM_TILE_INFO *	gdem_GetTilesFromPolyLine(CGDM_LocationArr &i_arrPt, int i_Level, int &o_TileCount);

//����������� Polygon�� �������.
double			gdem_GetAreaWithoutTerrain(CGDM_LocationArr &i_arrPt, int i_Level);

//delta_latitude, delta_longitude�� ���� ����������� Polygon�� �������.
double			gdem_GetAreaWithoutTerrain(double latitude, double delta_latitude, double delta_longitude);

// calculate functions
double			gdem_GetLengthWithPolygon(CGDM_LocationArr &i_arrPt, GDM_DEM_TILE* pTile);
double			gdem_GetAreaWithPolygon(CGDM_LocationArr &i_arrPt, GDM_DEM_TILE* pTile);
double			gdem_Get2DAreaWithPolygon(CGDM_VectorArr &i_arrPt);


void			gdem_Sign2DAreaWithPolygon(CGDM_VectorArr &i_arrPt, int *sign);

// sub functions
bool 			gdem_GetCrossLocationBetweenTwoLine(double x1, double y1, double x2, double y2, CGDM_Location3D i_Pt3, CGDM_Location3D i_Pt4, CGDM_Location3D& vp);
void 			gdem_GetAreaWithPolygonInGrid(CGDM_LocationArr &i_arrPt, double &dTotalArea, GDM_DEM_TILE* pTile);
void 			gdem_GetAreaWithBaseTriangle(CGDM_LocationArr &i_arrPt, double &dTotalArea, GDM_DEM_TILE* pTile);
void 			gdem_GetAreaWithTriangle(CGDM_LocationArr &i_arrPt, double &dTotalArea, GDM_DEM_TILE* pTile);
int  			gdem_GetPolygonArrWithGrid (CGDM_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &o_polygonArr);

void 			gdem_CalcNormalVector(IN CGDM_Vector3DF a_Vec1, IN CGDM_Vector3DF a_Vec2, OUT CGDM_Vector3DF& normalVec);
int 			gdem_CalcAngle(IN CGDM_VectorArr a_NormalVecs, IN CGDM_VectorArr a_PosVecs, OUT double& a_angle);

//�ٰ����� ���Ӵٰ����ΰ� �����ϴ� �Լ�.
bool			gdem_PolygonIsTwisted(IN CGDM_LocationArr &arrPt, OUT CGDM_LocationPolygonArr & PolygonArray);
bool			gdem_PolygonDivided(IN CGDM_LocationArr &arrPt, IN int &a_nStart1, IN int &a_nEnd1,  IN int &a_nStart2, IN int &a_nEnd2, IN CGDM_Location3D &a_crosspt, OUT CGDM_LocationArr & arrPt1, OUT CGDM_LocationArr & arrPt2);
void			gdem_PolygonTwistedProc(IN CGDM_LocationArr &i_arrPt, OUT CGDM_LocationPolygonArr & i_PolygonArray);
void			gdem_Polygon2Polygons(IN CGDM_LocationArr &arrPt, OUT CGDM_LocationPolygonArr & PolygonArray);

//2011 3 14 added by ugi
void            gdem_GetLineArrInRectangle(CGDM_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &o_LineArr);
//end



