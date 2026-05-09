#include <QString>
#include <string.h>
#include "QGlobe_Define.h"
#include "QGlobe_Camera.h"
#include "QGlobe_PyramidMgr.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_RequestMgr.h"
#include "GeoContourSceneNode.h"
#include "QGlobe_ContourCache.h"
#include "QGlobe_TextMgr.h"
#include "QGlobe_DataMgr.h"

#define QGlobe_CONTOUR_MIN_LEVEL_100	9
#define QGlobe_CONTOUR_MIN_LEVEL_20	11
#define QGlobe_CONTOUR_MAX_LEVEL	13
#define QGlobe_MAX_CONTOUR_TILES	40
#define QGlobe_CONTOUR_MARGIN		0.5
#define QGlobe_CONTOUR_LABEL_INTERVAL 500
#define QGlobe_CONTOUR_INTERPOLATION_UNIT_COUNT	3
#define QGlobe_CONTOUR_MIN_DISTANCE_BETWEEN_POINTS  0.5
#define QGlobe_MAX_VALUE							 100
#define USE_INTERPOATION 1
#define USE_LABEL		   1
//#define USE_ACUTE_ANGLE_LABEL	1
#define IS_OBTUSE_ANGLE         -1
#define  IS_ACTUE_ANGLE			1
#define  IS_OTHER_ANGLE			0
#define  OPTION_DISTANCE_FEET_MILE	1
#define  OPTION_DISTANCE_METER_KILO 0


CONTOUR_INTERPOLATION_FUNC*		InterpolationFunc(double* x, double* y/*, int interpointsCount*/);
double							GetInterpolationValue(CONTOUR_INTERPOLATION_FUNC func, double x, double xx);
void							CalcInterpolationValue(double* v, double x1, double y1, double x2, double y2, int n);
int								IsAcuteAngle(double x1, double y1, double x2, double y2);
QString							GetContourLabel(double nHeight);

using namespace geo;
using namespace core;

//precondition 
//polygonData�� ˱ã�׳�.

static void qglobe_SetContourPolygonInfo(CONTOUR_POLYGON_DATA *polygonData)
{
	double valleyInterval = 5 * qglobe_GetDataMgr()->m_sOption.contourOption.contourInterval;

	if( (polygonData->nAltitude - ((int)((polygonData->nAltitude) / valleyInterval) * valleyInterval) == 0))
	{
		polygonData->color = qglobe_GetDataMgr()->m_sOption.contourOption.ravineColor;
		polygonData->nWidth = qglobe_GetDataMgr()->m_sOption.contourOption.ravineThick;
	}
	else
	{
		polygonData->color = qglobe_GetDataMgr()->m_sOption.contourOption.mainCurveColor;
		polygonData->nWidth = qglobe_GetDataMgr()->m_sOption.contourOption.mainCurveThick;
	}
}
CONTOUR_POLYGON_DATA * qglobe_FormatContourPolygonData(int pIndex, CONTOUR_POLYGON_LIST* pGeoContourArray)
{
	CONTOUR_POLYGON_DATA *polygonData = NULL;
	polygonData = &pGeoContourArray->polyList[pIndex];
	if (polygonData != NULL && polygonData->pts.size() > 0)
	{
		polygonData->pts.set_used(0);
	}
	return polygonData;
	
}

// to increase speed
void AddContourText(double longitude, double latitude, const unsigned short* str, double height)
{
#if 0
	QGlobe_POINT3D tmpPos;
	double dRadius = 0;
	double dXRadius = 0;

	//qglobe_get3DWorldPoint(longitude, latitude, tmpPos, height);
	dRadius	   = EARTH_RADIUS + height;
	dXRadius = dRadius * cos(latitude);
	tmpPos.m_Z = dRadius * sin(latitude);
	tmpPos.m_X = dXRadius * cos(longitude);
	tmpPos.m_Y = dXRadius * sin(longitude);

	CQGlobe_Vector3DF pos;
	pos.m_tX = tmpPos.m_X;
	pos.m_tY = tmpPos.m_Y;
	pos.m_tZ = tmpPos.m_Z;

	qglobe_GetDataMgr()->m_pCamera->WorldToScreen(&pos);

	qglobe_GetDataMgr()->m_pTextMgr->AddTextWithScreen(pos.m_tX, pos.m_tY, RTT_CONTOUR, str);
#else
	CQGlobe_Location3D loc;
	loc.m_tX = longitude;
	loc.m_tY = latitude;
	loc.m_tZ = height;
	qglobe_GetDataMgr()->m_pTextMgr->AddText(E_QGlobe_SUBDATA_TYPE_NONE,loc, RTT_CONTOUR, str, 0);
#endif
}

//latlong˺�� ���̼� ����̩ �麺������ ���ٳ�.
inline double getDistance(double x1,double y1,double x2,double y2)
{
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

//̡�Ҵ��ɵ�˾ ��̡�� �൹ ���� ����� 3��̺������ɵ� �����ٳ�.

void	divideAndConvert3d(int level, core::array<vector3df>& pts,int heightlevel)
{
	QGlobe_POINT3D pt;
	double offsetheight = qglobe_GetDataMgr()->m_pCamera->m_location.m_dDist / 100.0;

	double height;
	double firstnonheight=0.0;   //���� ������ 0�� ʭ�� ���˰�
	double latestnonheight;      //���� �װ��� ������ 0�� ʭ�� ���˰�

	if(level<12)
	{		
		for(unsigned int i=0;i<pts.size();i++)
		{
			height=qglobe_getHeight(pts[i].X, pts[i].Y);

			if(firstnonheight==0.0 && height>0.0)
				firstnonheight=height;

			if(height>0.0)	   latestnonheight=height;

			if(height==0.0)		height=latestnonheight;

			pts[i].Z=height;
		}

		if(pts[0].Z==0.0) 	pts[0].Z=firstnonheight;

		for(unsigned int i=0;i<pts.size();i++)
		{
			qglobe_get3DBodyPoint(pts[i].X, pts[i].Y, pt, pts[i].Z+offsetheight);

			pts[i].X=pt.m_X;
			pts[i].Y=-pt.m_Y;
			pts[i].Z=pt.m_Z;
		}
		return;
	}

	array<vector3df> arrdivpt;//�����ױ� ���漰

	double deltalong,deltalat;
	
	if(level>13) level=13;

	double tilewidth=qglobe_TwoPI/(1<<level);
	double divisionangle=tilewidth/128.0;	

	arrdivpt.push_back(pts[0]);  //˧�� �󱨼�˾ �Ĳ���.
	double dist;
	vector3df tmp;

	int ndiv;
	firstnonheight=0.0;
	latestnonheight=0.0;

	for(unsigned int i=1;i<pts.size();i++)
	{
		dist=getDistance(pts[i-1].X,pts[i-1].Y,	pts[i].X,pts[i].Y);//������Ĵ��̩ ������ �˺��ٳ�.

		if(dist<=divisionangle)//�����ױ� ���� ­���� �ֲ� ��˧
		{
			double x=pts[i].X;
			double y=pts[i].Y;
			height=qglobe_getExactHeight(x,y);
			pts[i].Z=height;
			arrdivpt.push_back(pts[i]);
			continue;
		}

		ndiv=(int)(dist / divisionangle + 1);
		deltalong=pts[i].X-pts[i-1].X;
		deltalong=deltalong/ndiv;
		deltalat=pts[i].Y-pts[i-1].Y;
		deltalat=deltalat/ndiv;

		for(int j=1;j<=ndiv;j++)                        //�޻� �����ױ� ����ʳ�.
		{
			tmp.X=pts[i-1].X+j*deltalong;
			tmp.Y=pts[i-1].Y+j*deltalat;
			height=qglobe_getExactHeight(tmp.X,tmp.Y);

			if(firstnonheight==0.0 && height>0.0)
				firstnonheight=height;                  //���� ��˽˺�� 0�� ʭ�� ���˰� �ݾ�

			if(height>0.0)	   latestnonheight=height;   //���� �װ��� ������ 0�� ʭ�� ���˰�

			if(height<=0.0)	
				height=latestnonheight;
			tmp.Z=height;

			arrdivpt.push_back(tmp);
		}

		for(unsigned int k=arrdivpt.size()-ndiv-2;k<arrdivpt.size();k++)
		{
			if(arrdivpt[k].Z<=0)
				arrdivpt[k].Z=latestnonheight;
		}
	}

	//���� ������̩ ���˰�˾ �ֵ��ٳ�.

	height=qglobe_getExactHeight(arrdivpt[0].X,arrdivpt[0].Y);

	if(height!=0.0)
		arrdivpt[0].Z=height;
	else
		arrdivpt[0].Z=firstnonheight;

	pts.set_used(0);

	//3��̺������� �����ٳ�.

	for(unsigned int i=0;i<arrdivpt.size();i++)
	{														 //��̩ �ĵ������� ��˼ ��˺�� ���˰�˾ ˱���ױ� �ݾ��ײ� ����
		qglobe_get3DBodyPoint(arrdivpt[i].X, arrdivpt[i].Y, pt, /*arrdivpt[i].Z*/heightlevel+offsetheight);

		tmp.X=pt.m_X;
		tmp.Y=-pt.m_Y;
		tmp.Z=pt.m_Z;
		pts.push_back(tmp);
		}
	}

int	qglobe_GetGeoContourInfo(int* pIndex, CONTOUR_POLYGON_LIST* pGeoContourArray)
{
	const QGlobe_LOGIC_TILE_ARR *p_contour_tile_list = qglobe_GetRenderTiles();

	if(*pIndex >= p_contour_tile_list->nCount || *pIndex >= QGlobe_MAX_CONTOUR_TILES)
		return 0;

	QGlobe_POINT3D pt = {0,};
	
	int polyIndex=0;
	
	double	minLongitude,maxLongitude,	minLatitude,maxLatitude,	tileWidth;
	
	GDALContourItem*	pItem = NULL;

	// get tile
	const QGlobe_LOGIC_TILE *pTile	= p_contour_tile_list->pData + *pIndex;

	if(pTile->pContourDemTile == NULL)	return 0;

	int nInterval = qglobe_GetDataMgr()->m_sOption.contourOption.contourInterval;

	short* pHeightData = pTile->pContourDemTile->m_pData;

	GDALContour *pContourData  = qglobe_GetDataMgr()->m_pContour->GetContourData(pTile->pContourDemTile->m_sInfo.nX,
																pTile->pContourDemTile->m_sInfo.nY,
																pTile->pContourDemTile->m_sInfo.nLevel, pHeightData, nInterval);

	if(pContourData == NULL)   	return 0;
	double valleyWidth = 5 * nInterval;

	// get min/max
	tileWidth = qglobe_TwoPI / (1 << pTile->nLevel);
	minLongitude = tileWidth * pTile->nX - qglobe_PI;
	minLatitude	= tileWidth * pTile->nY - qglobe_PI;
	maxLongitude = pTile->minLongitude + tileWidth;
	maxLatitude	= pTile->minLatitude + tileWidth;
	
	unsigned short ContourLabel[RTI_MAX_TEXTLEN];

#ifdef USE_ACUTE_ANGLE_LABEL
	unsigned short AngleLabel[RTI_MAX_TEXTLEN];
#endif

	CONTOUR_POLYGON_DATA *polygonData= polygonData = &pGeoContourArray->polyList[polyIndex];

	if(polygonData->pts.size() > 0)	  	polygonData->pts.set_used(0);

	double longitude,latitude,pointWidth,pointsDistance;
	int nPoints,angleflag;
	geo::core::vector3df beforeVertex;

	for (int i = 0; i < pContourData->nCount; i++)
	{
		pItem = &pContourData->items[i];

		nPoints = pItem->nPoints;

		if(polyIndex >= MAX_CONTOUR_POLYGON_COUNT)
			break;

		polygonData = &pGeoContourArray->polyList[polyIndex];

		for (int j = 0; j < nPoints; j++)
		{
			if(polyIndex >= MAX_CONTOUR_POLYGON_COUNT)	
				break;
			
			pointWidth =  pContourData->gridWidth/QGLOBE_MAX_DEM_DIVISION;

			pointsDistance = 0;

			angleflag = 0;

			longitude	= (pItem->padfX[j] - QGlobe_CONTOUR_MARGIN) * pContourData->gridWidth + pContourData->minLongitude;
			latitude	= (pItem->padfY[j] - QGlobe_CONTOUR_MARGIN) * pContourData->gridWidth + pContourData->minLatitude;

			if(longitude < minLongitude - pointWidth || longitude > maxLongitude + pointWidth 
				|| latitude < minLatitude - pointWidth || latitude > maxLatitude + pointWidth)
			{
				if(polygonData->pts.size() > 0)
				{
					polygonData->nAltitude = pItem->dfLevel;
					qglobe_SetContourPolygonInfo(polygonData);
					polyIndex++;
					polygonData=&pGeoContourArray->polyList[polyIndex];
					polygonData->pts.set_used(0);
				}

				continue;
			}

			//calculate interpolation function and interpolation points.
			if (j > 0 &&  j < nPoints - 1 ) 
			{
				double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
				x1 = pItem->padfX[j-1] - pItem->padfX[j];
				x2 = pItem->padfX[j+1] - pItem->padfX[j];
				y1 = pItem->padfY[j-1] - pItem->padfY[j];
				y2 = pItem->padfY[j+1] - pItem->padfY[j];
				angleflag = IsAcuteAngle(x1, y1, x2, y2);
			}
			else
				angleflag = IS_OTHER_ANGLE;

			if (angleflag == IS_ACTUE_ANGLE)
			{
				  pItem->padPt[j].x = beforeVertex.X ;
				  pItem->padPt[j].y = beforeVertex.Y ;
				  pItem->padPt[j].z = beforeVertex.Z ;
									
				  polygonData->pts.push_back(beforeVertex);
			}
			else 
			{
				beforeVertex.X=longitude;
				beforeVertex.Y=latitude;
				beforeVertex.Z=0;
				polygonData->pts.push_back(beforeVertex);
			}

#ifdef USE_INTERPOATION	
				if (angleflag == IS_OBTUSE_ANGLE || angleflag == IS_ACTUE_ANGLE)
				{
					for (int k = j-1; k <= j; k++)
					{
						int intercounts = 0;
						pointsDistance = (pItem->padfX[k] - pItem->padfX[k-1]) * (pItem->padfX[k] - pItem->padfX[k-1]) + (pItem->padfY[k] - pItem->padfY[k-1]) * (pItem->padfY[k] - pItem->padfY[k-1]);
						intercounts = (int)(pointsDistance/QGlobe_CONTOUR_MIN_DISTANCE_BETWEEN_POINTS);
						if (intercounts > 0)
						{
							double* interY = NULL;
							interY = (double*)malloc(sizeof(double) * intercounts * 2);

							CalcInterpolationValue(interY, pItem->padfX[k-1], pItem->padfX[k], pItem->padfY[k-1], pItem->padfY[k], intercounts * 2);

							for (int p = 0; p > intercounts * 2; p++)
							{
								double interPointX = pItem->padfX[k-1] + p * (pItem->padfX[k] - pItem->padfX[k-1]) / (2 * intercounts);
								double interlongitude	= (interPointX - QGlobe_CONTOUR_MARGIN) * pContourData->gridWidth + pContourData->minLongitude;
								double interlatitude	= (interY[p] - QGlobe_CONTOUR_MARGIN) * pContourData->gridWidth + pContourData->minLatitude;

								geo::core::vector3df intervertex;
								intervertex.X =  interlongitude;
								intervertex.Y =  interlatitude;
								intervertex.Z =  0;

								polygonData->pts.push_back(intervertex);
							}
							free(interY);
						}
					}
				}
#endif
#ifdef USE_LABEL
			if (qglobe_PI != pItem->padfAngle[j] && (((pItem->dfLevel - ((int)((pItem->dfLevel)/ valleyWidth)) * valleyWidth) == 0)))
			{
				memset ((void *)ContourLabel, 0, sizeof(ushort) * RTI_MAX_TEXTLEN);
				QString str = GetContourLabel(pItem->dfLevel);
				const ushort *name = str.utf16();
				int len = qMin(str.length(), RTI_MAX_TEXTLEN-1);
				memcpy(ContourLabel, name, len * sizeof(ushort));
				ContourLabel[len] = 0;

				// qglobe_GetDataMgr()->m_pTextMgr->AddText(longitude, latitude, RTT_CONTOUR, ContourLabel, (qglobe_GetDataMgr()->m_sOption.is_LayerTerrain) ? true : false);
                // to increase speed
				AddContourText(longitude, latitude, ContourLabel, pItem->dfLevel * qglobe_GetDataMgr()->m_sOption.dem_detail_rate + 100/*(qglobe_GetDataMgr()->m_sOption.is_LayerTerrain) ? pItem->dfLevel : 0*/);
			}

#endif
#ifdef USE_ACUTE_ANGLE_LABEL
			if (bInterpolation)
			{
				memset ((void *)AngleLabel, 0, sizeof(ushort)*RTI_MAX_TEXTLEN);
				QString str = QString("Acute");
				const ushort *name = str.utf16();
				int len = qMin(str.length(), RTI_MAX_TEXTLEN-1);
				memcpy(AngleLabel, name, len * sizeof(ushort));
				AngleLabel[len] = 0;

				qglobe_GetDataMgr()->m_pTextMgr->AddText(longitude, latitude, RTT_CONTOUR, AngleLabel, (pInfo->nLevel > 10) ? true : false);
			}
#endif
    	}

		if(polygonData->pts.size() > 0)
		{
			polygonData->nAltitude = pItem->dfLevel;
			qglobe_SetContourPolygonInfo(polygonData);
			polyIndex++;
			polygonData=&pGeoContourArray->polyList [polyIndex];
			polygonData->pts.set_used(0);
		}

	}
	pGeoContourArray->nCount = polyIndex;

	for(int i=0;i<polyIndex;i++)
		divideAndConvert3d(pTile->nLevel,pGeoContourArray->polyList[i].pts,pGeoContourArray->polyList[i].nAltitude);
	return 1;
}

void CalcInterpolationValue(double* v, double x1, double y1, double x2, double y2, int n)
{
	double b1x, b2x, b1y, b2y;

	b1x = x1 * 2.0 / 3.0 + x2 / 3.0;
	b2x = x1 / 3.0 + x2 * 2.0 / 3.0;
	b1y = y1 * 2.0 / 3.0 + y2 / 3.0;
	b2y = y1 / 3.0 + y2 * 2.0 / 3.0;

	for (int j = 0; j < n; j++ )
	{
		 double t = (double)j / (double)n;
		 v[j] = (1 - t) * (1 - t) * (1 - t) * x1 + 3 * (1 - t) * (1 - t) * t * b1x +
			 3 * (1 - t) * t * t * b2x + t * t * t * x2;
	}
}

int IsAcuteAngle(double x1, double y1, double x2, double y2)
{
	double angle = 0;
	angle = ((x1 * x2) + (y1 * y2))/(sqrt((x1 * x1) + (y1 * y1)) * sqrt((x2 * x2) + (y2 * y2)));

	if (angle > 0.6)
		return IS_ACTUE_ANGLE;
	else if ( -0.6 < angle && angle <= 0.6)
	{
		return IS_OBTUSE_ANGLE;
	}
	else
		return IS_OTHER_ANGLE;
}

QString GetContourLabel(double nHeight)
{
	QString text;

	switch(qglobe_GetDataMgr()->m_sOption.dist_ViewMode)
	{
	case OPTION_DISTANCE_FEET_MILE:
			text = QString("%1ft ").arg((int)nHeight*100/30.48);
		break;
	case OPTION_DISTANCE_METER_KILO:
			text = QString("%1m ").arg((int)nHeight);
		break;
	default:
		break;
	}
	return text;
}
