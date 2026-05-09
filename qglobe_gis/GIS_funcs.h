#pragma once

#include "../qglobe_libmain/QGlobe_Tile.h"
#include "QGlobe_Vector3D.h"
#include "QGlobe_Array.h"
#include "QGlobe_Location.h"
#include "QGlobe_Define.h"
#include <math.h>
#include "QGlobe_Mem.h"
#include "QGlobe_Coord3D.h"
#include "qglobeclientbase.h"

#define QGlobe_MAX_DEM_LEVEL	13

// calculate functions

// CQGlobe_LocationArr: ��(�浵, ����, ����)���

// ������� 
double			qglobe_GetAreaInMesh(QGLOBE_MESH &mesh);
// ü�����
double          qglobe_GetVolumeInMesh(QGLOBE_MESH &mesh, double height);

// ���̰��
double			qglobe_GetLengthWithPolygon(CQGlobe_LocationArr &arrPt, bool bClose);
// �ִ����� ���
void			qglobe_GetMaxRatePos(IN CQGlobe_LocationArr &arrPt, OUT QGlobe_POSITION& pos);
// �ִ�/�ּҳ��� ���
void			qglobe_GetMinMaxHeightPos(IN CQGlobe_LocationArr &arrPt, OUT QGlobe_POSITION& minPos, OUT QGlobe_POSITION& maxPos);
// 3�����Ҹ���
void			qglobe_DivisionToTriangle(IN CQGlobe_LocationArr &arrPt, OUT CQGlobe_MeshArr &meshArr);


void qglobe_GetLocationFrom3DPoint(CQGlobe_Vector3DF &i_pt, CQGlobe_Location3D &o_pt);

// get height from location in dem tile
double			qglobe_GetHeightFromDemTile(CQGlobe_Location3D &i_pt, QGlobe_DEM_TILE* pTile);

// �浵,����, ���̷κ��� 3���� ��ǥ�����
void			qglobe_Get3DPointFromLocation(CQGlobe_Location3D &i_pt, CQGlobe_Vector3DF &o_pt, double &height);

// ���а� x = a (y�࿡ ������ ����) ���� �����
bool qglobe_GetCrossPtSegmentWithAxisY (double xConst, double minY, double maxY, CQGlobe_Location3D *pPt1, CQGlobe_Location3D *pPt2, CQGlobe_Location3D &crossPt);

// ���а� y = a (x�࿡ ������ ����) ���� �����
bool qglobe_GetCrossPtSegmentWithAxisX (double yConst, double minX, double maxX, CQGlobe_Location3D *pPt1, CQGlobe_Location3D *pPt2, CQGlobe_Location3D &crossPt);

// �ϳ��� ��(�׸���)�� ����ڰ�輱�� ������� �Ͽ� �����Ǵ� ��Ѽ����� ��� ���.
int  qglobe_GetCrossLineArrWithGrid(CQGlobe_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CQGlobe_LocationPolygonArr &io_crossLineArr, bool blClose = true);

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
bool			qglobe_PtIsInPolygon(CQGlobe_Location3D &i_pt, CQGlobe_LocationArr &i_arrPt);

int				qglobe_LineIsInIntersectWithTriangle(CQGlobe_Location3D &i_startPt, CQGlobe_Location3D &i_endPt, CQGlobe_Vector3DF *i_Pt1 , CQGlobe_Vector3DF *i_Pt2 , CQGlobe_Vector3DF *i_Pt3, CQGlobe_Vector3DF &i_destPt);



// �� ���� ���̴� Ÿ���������.
QGlobe_TILE_INFO	qglobe_GetTileFromPt(CQGlobe_Location3D i_Pt, int i_Level);

//������ ��ʹ� Ÿ�ϵ��� �������.
QGlobe_TILE_INFO *	qglobe_GetTilesFromLine(CQGlobe_Location3D startPos, CQGlobe_Location3D endPos , int i_Level, int *o_TileCount);

//Polygon�� ��ʹ� Ÿ�ϵ��� �������.
QGlobe_TILE_INFO *	qglobe_GetCrossTilesFromPolygon(CQGlobe_LocationArr &i_arrPt,  int i_Level, int &o_TileCount);

//PolyLine�� ��ʹ� Ÿ�ϵ��� �������.
QGlobe_TILE_INFO *	qglobe_GetTilesFromPolyLine(CQGlobe_LocationArr &i_arrPt, int i_Level, int &o_TileCount);

//������������ Polygon�� �������.
double			qglobe_GetAreaWithoutTerrain(CQGlobe_LocationArr &i_arrPt, int i_Level);

//delta_latitude, delta_longitude�� ���� ������������ Polygon�� �������.
double			qglobe_GetAreaWithoutTerrain(double latitude, double delta_latitude, double delta_longitude);

// calculate functions
double			qglobe_GetLengthWithPolygon(CQGlobe_LocationArr &i_arrPt, QGlobe_DEM_TILE* pTile);
double			qglobe_GetAreaWithPolygon(CQGlobe_LocationArr &i_arrPt, QGlobe_DEM_TILE* pTile);
double			qglobe_Get2DAreaWithPolygon(CQGlobe_VectorArr &i_arrPt);


void			qglobe_Sign2DAreaWithPolygon(CQGlobe_VectorArr &i_arrPt, int *sign);

// sub functions
bool 			qglobe_GetCrossLocationBetweenTwoLine(double x1, double y1, double x2, double y2, CQGlobe_Location3D i_Pt3, CQGlobe_Location3D i_Pt4, CQGlobe_Location3D& vp);
void 			qglobe_GetAreaWithPolygonInGrid(CQGlobe_LocationArr &i_arrPt, double &dTotalArea, QGlobe_DEM_TILE* pTile);
void 			qglobe_GetAreaWithBaseTriangle(CQGlobe_LocationArr &i_arrPt, double &dTotalArea, QGlobe_DEM_TILE* pTile);
void 			qglobe_GetAreaWithTriangle(CQGlobe_LocationArr &i_arrPt, double &dTotalArea, QGlobe_DEM_TILE* pTile);
int  			qglobe_GetPolygonArrWithGrid (CQGlobe_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CQGlobe_LocationPolygonArr &o_polygonArr);

void 			qglobe_CalcNormalVector(IN CQGlobe_Vector3DF a_Vec1, IN CQGlobe_Vector3DF a_Vec2, OUT CQGlobe_Vector3DF& normalVec);
int 			qglobe_CalcAngle(IN CQGlobe_VectorArr a_NormalVecs, IN CQGlobe_VectorArr a_PosVecs, OUT double& a_angle);

//�ٰ����� ���Ӵٰ����ΰ� �����ϴ� �Լ�.
bool			qglobe_PolygonIsTwisted(IN CQGlobe_LocationArr &arrPt, OUT CQGlobe_LocationPolygonArr & PolygonArray);
bool			qglobe_PolygonDivided(IN CQGlobe_LocationArr &arrPt, IN int &a_nStart1, IN int &a_nEnd1,  IN int &a_nStart2, IN int &a_nEnd2, IN CQGlobe_Location3D &a_crosspt, OUT CQGlobe_LocationArr & arrPt1, OUT CQGlobe_LocationArr & arrPt2);
void			qglobe_PolygonTwistedProc(IN CQGlobe_LocationArr &i_arrPt, OUT CQGlobe_LocationPolygonArr & i_PolygonArray);
void			qglobe_Polygon2Polygons(IN CQGlobe_LocationArr &arrPt, OUT CQGlobe_LocationPolygonArr & PolygonArray);

//2011 3 14 added by ugi
void            qglobe_GetLineArrInRectangle(CQGlobe_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CQGlobe_LocationPolygonArr &o_LineArr);
//end



