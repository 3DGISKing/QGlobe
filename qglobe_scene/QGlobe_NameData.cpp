#include <string.h>
#include "QGlobe_Define.h"
#include "QGlobe_PyramidMgr.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_RequestMgr.h"
#include "QGlobe_DataMgr.h"
#include "GeoNameSceneNode.h"
#include "QGlobe_TextMgr.h"
#include "QGlobe_DataMgr.h"
#include "../qglobe_client/qglobeclient.h"
#include "../qglobe_client/config.h"
#include "../qglobe_terrainengine/SAnimatedMesh.h"
#include "../qglobe_terrainengine/SGeoMesh.h"
#include "QGlobe_3DObjectCache.h"
#include "QGlobe_SimpleBuildingCache.h"

using namespace geo;
using namespace scene;

#define MAX_RENDER_TEXT_COUNT 100

static ushort s_pyongyang[] = {0xd3c9, 0xc591, 0};

inline bool IsPyongyang(ushort *text)
{
	bool res = false;
	int i = 0;

	for (i = 0; s_pyongyang[i] != 0; i ++)
	{
		if (text[i] == 0)
			break;
		if (text[i] != s_pyongyang[i])
			break;
	}

	if (text[i] == 0 && s_pyongyang[i] == 0)
		res = true;

	return res;
}

double IsBackFaceName(QGlobe_Coord3D aCameraPos, QGlobe_POINT3D aViewPos)
{
	QGlobe_POINT3D vt;
	
	vt.m_X = aViewPos.m_X - aCameraPos.m_org.m_tX;
	vt.m_Y = aViewPos.m_Y - aCameraPos.m_org.m_tY;
	vt.m_Z = aViewPos.m_Z - aCameraPos.m_org.m_tZ;

	double scalra = aViewPos.m_X * vt.m_X + aViewPos.m_Y * vt.m_Y + aViewPos.m_Z * vt.m_Z;
	return scalra;
}

void qglobe_MakeRenderGeoNameData();

/**************************************************
	2013 2 27 ugi
	��� �ó󶵴�� ����̩ �˶���˾ �ﻤ�ٳ�.

****************************************************/
void qglobe_MakeRenderTileName()
{
	const QGlobe_LOGIC_TILE_ARR *p_render_tile_list;
	p_render_tile_list = qglobe_GetRenderTiles();

	const QGlobe_LOGIC_TILE *pTile;

	CQGlobe_Location3D		locPt;

	for(int i=0;i<p_render_tile_list->nCount;i++)
	{
		pTile=p_render_tile_list->pData+i;

		//draw 
		double tileWidth = qglobe_TwoPI / (1 << pTile->nLevel);

		double minLongitude = tileWidth * pTile->nX - qglobe_PI;
		double minLatitude	= tileWidth * pTile->nY - qglobe_PI;

		double centerlongi=minLongitude+tileWidth/2;
		double centerlat=minLatitude+tileWidth/2;

		locPt.m_tX=centerlongi;
		locPt.m_tY=centerlat;
		locPt.m_tZ=0;

		int m=pTile->nX/128; int n=pTile->nY/128;

		QString tilname = QString("%1 th :%2_%3_%4_%5_%6")
			.arg(i)
			.arg(pTile->nX)
			.arg(pTile->nY)
			.arg(pTile->nLevel)
			.arg(m)
			.arg(n);

		qglobe_GetDataMgr()->m_pTextMgr->AddText(E_QGlobe_SUBDATA_TYPE_NONE,locPt, RTT_NAME_DEFAULT, tilname, 0, false);

	}

}

/**************************************************
2013 3 7 ugi
��� �ó󶵴�� ���ϴ�̩ �˶���˾ �ﻤ�ٳ�.
****************************************************/
#define FLOOR_FACTOR 3.0

#include "GeoMeshUtil.h"
void qglobe_MakeRenderGeo3dObjectName()
{
	double offsetheight=g_pQGlobeDataMgr->m_pCamera->m_location.m_dDist/100.0;
	
	const QGlobe_LOGIC_TILE *ptile;
	QGlobe_TILE* meshtile;
	SAnimatedMesh* meshs;
	SGeoMesh* mesh;

	CQGlobe_Location3D		locPt;

	const QGlobe_LOGIC_TILE_ARR *render_tile_list= qglobe_GetRenderTiles();

	GeoBuildingProperty p;

	for(int i=0;i<render_tile_list->nCount;i++)
	{
		ptile=render_tile_list->pData+i;//i���� �㶮����˾ �в���.

		if(ptile->nLevel<16)
			continue;

		for(int j=0;j<ptile->n3dMeshTileCnt;j++)
		{
			meshtile=ptile->sArr3DMeshTile[j];
			meshs=(SAnimatedMesh*)g_pQGlobeDataMgr->m_p3DObjectCache->GetMeshs(meshtile->m_sInfo.nX,
																			meshtile->m_sInfo.nY,
																			meshtile->m_sInfo.nLevel,
																			meshtile->m_pBuffer);

			for (unsigned int k=0;k<meshs->Meshes.size();k++)
			{
				mesh=(SGeoMesh*)meshs->Meshes[k];

				if(mesh->GeoInfo.Longitude < ptile->minLongitude || mesh->GeoInfo.Longitude > ptile->maxLongitude
					|| mesh->GeoInfo.Latitude < ptile->minLatitude || mesh->GeoInfo.Latitude > ptile->maxLatitude)
					continue;

				locPt.m_tX=mesh->GeoInfo.Longitude;
				locPt.m_tY=mesh->GeoInfo.Latitude;
				
				core::aabbox3df box=mesh->getBoundingBox();
				double objectheight=box.MaxEdge.Z-box.MinEdge.Z;

				locPt.m_tZ=mesh->GeoInfo.Height+objectheight+offsetheight;

				if(GeoMeshUtil::GetGeoBuildingData(mesh->GeoInfo.ID,&p))

				if(!p._Name.isEmpty())
					qglobe_GetDataMgr()->m_pTextMgr->AddText(E_QGlobe_SUBDATA_TYPE_NONE,locPt, RTT_NAME_DEFAULT,p._Name, 0);
			}
		}//int j=0;j<ptile->n3dMeshTileCnt

		BuildingArray* pSimpleBuildingArr;

		for(int j=0;j<ptile->nPoygonTileCnt;j++)
		{
			QGlobe_TILE *polyTile=ptile->sArrPolygonTile[j];

			pSimpleBuildingArr=g_pQGlobeDataMgr->m_pSimpleBuildingCache->GetBuildingArray(polyTile);

			if(!pSimpleBuildingArr)		continue;

			for(unsigned int k=0;k<pSimpleBuildingArr->Count();k++)
			{
				SimpleBuilding* psimplebuilding=(SimpleBuilding*)pSimpleBuildingArr->GetBuilding(k);

				/*if(psimplebuilding->GetBottomPolygon()->m_cBound.m_cMinPt.m_tX < ptile->minLongitude) continue;
				if(psimplebuilding->GetBottomPolygon()->m_cBound.m_cMinPt.m_tY < ptile->minLatitude) continue;
				if(psimplebuilding->GetBottomPolygon()->m_cBound.m_cMaxPt.m_tX > ptile->maxLongitude) continue;
				if(psimplebuilding->GetBottomPolygon()->m_cBound.m_cMaxPt.m_tY > ptile->maxLatitude) continue;*/

				locPt.m_tX=psimplebuilding->GetBottomPolygon()->m_cBound.m_cMinPt.m_tX;
				locPt.m_tX+=(psimplebuilding->GetBottomPolygon()->m_cBound.m_cMaxPt.m_tX-psimplebuilding->GetBottomPolygon()->m_cBound.m_cMinPt.m_tX)/2.0;

				locPt.m_tY=psimplebuilding->GetBottomPolygon()->m_cBound.m_cMinPt.m_tY;
				locPt.m_tY+=(psimplebuilding->GetBottomPolygon()->m_cBound.m_cMaxPt.m_tY-psimplebuilding->GetBottomPolygon()->m_cBound.m_cMinPt.m_tY)/2.0;
				locPt.m_tZ=psimplebuilding->GetHeight()+psimplebuilding->GetFloor()*FLOOR_FACTOR;
				
				if(!psimplebuilding->GetName().isEmpty())
					qglobe_GetDataMgr()->m_pTextMgr->AddText(E_QGlobe_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING,locPt, RTT_NAME_DEFAULT,psimplebuilding->GetName(), 0);

			}
		}
	}
}

void qglobe_MakeRenderGeoNameData()
{
	int tileID = 0;
	unsigned int i;
	int k;
	ST_GEODATA_NAMELIST			*pNameData;
	const QGlobe_LOGIC_TILE		*pTile;
	int							nameIndex = 0;
	ST_GEODATA_NAME				*pGeoCell;
	unsigned short	*korName,   *natName;

	bool bHeight = false;
	int key = 0;
	bool blKorean = true;

	const QGlobe_LOGIC_TILE_ARR	*p_name_tile_list = qglobe_GetRenderTiles();
	
	int nIndex = p_name_tile_list->nCount;

	QGlobe_Coord3D camera_coord;
	QGlobe_POINT3D tmpPos;
	CQGlobe_Location3D		locPt;
	qglobe_GetDataMgr()->m_pCamera->GetCameraCoord(&camera_coord);

	for (tileID = 0; tileID < nIndex; tileID++)
	{
		pTile = &p_name_tile_list->pData[tileID];

		bHeight = (pTile->nLevel > 10) ? true : false;

		for(k = 0; k < pTile->nNameTileCnt; k++)
		{
			if(pTile->sArrNameTile[k] == NULL)
				continue;

			pNameData = (ST_GEODATA_NAMELIST*)pTile->sArrNameTile[k]->m_pBuffer;
			unsigned char* pBuff = (unsigned char*)pNameData->data_list;

			for(i = 0; i < pNameData->count; i++)
			{
				ST_GEODATA_NAME* pRecord = (ST_GEODATA_NAME*)pBuff;

				pGeoCell = pRecord;

				locPt.m_tX = pGeoCell->longitude * qglobe_DEGTORAD;
				locPt.m_tY = pGeoCell->latitude * qglobe_DEGTORAD;

				pBuff += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pRecord->length + pRecord->length_ko;

				natName = pRecord->name;
				korName = pRecord->name + (pRecord->length / 2);

				if(locPt.m_tX < pTile->minLongitude || locPt.m_tX > pTile->maxLongitude ||
					locPt.m_tY < pTile->minLatitude || locPt.m_tY > pTile->maxLatitude)
					continue;

				qglobe_get3DWorldPoint(locPt.m_tX, locPt.m_tY, tmpPos);

				//by RSH 2013.3.12 for names not to be rendered if they are behind terrain
				///*
				if (qglobe_GetDataMgr()->m_pCamera->m_fScalarOfDir < 0.5)
				{
					CQGlobe_Vector3DF rayDir;
					rayDir.m_tX = tmpPos.m_X - camera_coord.m_org.m_tX;
					rayDir.m_tY = tmpPos.m_Y - camera_coord.m_org.m_tY;
					rayDir.m_tZ = tmpPos.m_Z - camera_coord.m_org.m_tZ;
					CQGlobe_Point3DF pCamToTerrainPt(0,0,0);
					QGlobe_ERROR_NO ret = qglobe_intersectRayDEM(camera_coord.m_org , rayDir, &pCamToTerrainPt);

					if (ret == QGlobe_SUCCESS)	
					{
						double camToNameDist = camera_coord.m_org.getDistanceFrom(CQGlobe_Vector3DF(tmpPos.m_X, tmpPos.m_Y, tmpPos.m_Z));
						double camToTerrainDist = camera_coord.m_org.getDistanceFrom(CQGlobe_Vector3DF(pCamToTerrainPt.m_tX,
							pCamToTerrainPt.m_tY,
							pCamToTerrainPt.m_tZ));
						if (camToNameDist > camToTerrainDist)
							continue;
					}
				}
				//*/
				//end addition by RSH

				if(pTile->nLevel < 6)
				{
					if (IsBackFaceName(camera_coord, tmpPos) > 0)
						continue;			
				}

				// check korean name
				blKorean = true;
				if(korName[0] == 0)
				{
					korName = natName;
					blKorean = false;
				}

				key = pRecord->id;

				// get height
				if(pRecord->height > 1.0)
					locPt.m_tZ = pRecord->height;
				else
					locPt.m_tZ = 0;

				if (IsPyongyang(pGeoCell->name))
					qglobe_GetDataMgr()->m_pTextMgr->AddText((E_QGlobe_SUBDATA_TYPE)pGeoCell->type,locPt, RTT_PYONGYANG, korName, key, blKorean);
				else
					qglobe_GetDataMgr()->m_pTextMgr->AddText((E_QGlobe_SUBDATA_TYPE)pGeoCell->type, locPt, RTT_NAME_DEFAULT, korName, key, blKorean);
			}
		}
	}

#ifdef DEBUG
	if(g_app->GetConfig()->m_isShowRenderTileName)
		qglobe_MakeRenderTileName();
#endif

	if(g_pQGlobeDataMgr->m_pCamera->m_location.m_dDist<500)
	if(g_pQGlobeDataMgr->m_pCamera->m_fScalarOfDir>0.5)
	if(g_app->GetConfig()->m_IsShowBuildingName)
		qglobe_MakeRenderGeo3dObjectName();
}
