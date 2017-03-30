#include <string.h>
#include "GDM_Define.h"
#include "GDM_PyramidMgr.h"
#include "GDM_CacheMgr.h"
#include "GDM_RequestMgr.h"
#include "GDM_DataMgr.h"
#include "GeoNameSceneNode.h"
#include "GDM_TextMgr.h"
#include "GDM_DataMgr.h"
#include "../gdem_client/gdemclient.h"
#include "../gdem_client/config.h"
#include "../gdem_terrainengine/SAnimatedMesh.h"
#include "../gdem_terrainengine/SGeoMesh.h"
#include "GDM_3DObjectCache.h"
#include "GDM_SimpleBuildingCache.h"

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

double IsBackFaceName(GDM_Coord3D aCameraPos, GDM_POINT3D aViewPos)
{
	GDM_POINT3D vt;
	
	vt.m_X = aViewPos.m_X - aCameraPos.m_org.m_tX;
	vt.m_Y = aViewPos.m_Y - aCameraPos.m_org.m_tY;
	vt.m_Z = aViewPos.m_Z - aCameraPos.m_org.m_tZ;

	double scalra = aViewPos.m_X * vt.m_X + aViewPos.m_Y * vt.m_Y + aViewPos.m_Z * vt.m_Z;
	return scalra;
}

void gdm_MakeRenderGeoNameData();

/**************************************************
	2013 2 27 ugi
	Âï¼õ ¶Ã³ó¶µ´ó²÷ À°ËÎÌ© ËË¶§´ÉË¾ Âï»¤ÂÙ³Þ.

****************************************************/
void gdm_MakeRenderTileName()
{
	const GDM_LOGIC_TILE_ARR *p_render_tile_list;
	p_render_tile_list = gdm_GetRenderTiles();

	const GDM_LOGIC_TILE *pTile;

	CGDM_Location3D		locPt;

	for(int i=0;i<p_render_tile_list->nCount;i++)
	{
		pTile=p_render_tile_list->pData+i;

		//draw 
		double tileWidth = gdm_TwoPI / (1 << pTile->nLevel);

		double minLongitude = tileWidth * pTile->nX - gdm_PI;
		double minLatitude	= tileWidth * pTile->nY - gdm_PI;

		double centerlongi=minLongitude+tileWidth/2;
		double centerlat=minLatitude+tileWidth/2;

		locPt.m_tX=centerlongi;
		locPt.m_tY=centerlat;
		locPt.m_tZ=0;

		int m=pTile->nX/128; int n=pTile->nY/128;

		QString tilname;
		tilname.sprintf("%d th :%d_%d_%d_%d_%d",i,pTile->nX,pTile->nY,pTile->nLevel,m,n);

		gdm_GetDataMgr()->m_pTextMgr->AddText(E_GDM_SUBDATA_TYPE_NONE,locPt, RTT_NAME_DEFAULT, tilname, 0, false);

	}

}

/**************************************************
2013 3 7 ugi
Âï¼õ ¶Ã³ó¶µ´ó²÷ °¼·Ï´ÉÌ© ËË¶§´ÉË¾ Âï»¤ÂÙ³Þ.
****************************************************/
#define FLOOR_FACTOR 3.0

#include "GeoMeshUtil.h"
void gdm_MakeRenderGeo3dObjectName()
{
	double offsetheight=g_pGDMDataMgr->m_pCamera->m_location.m_dDist/100.0;
	
	const GDM_LOGIC_TILE *ptile;
	GDM_TILE* meshtile;
	SAnimatedMesh* meshs;
	SGeoMesh* mesh;

	CGDM_Location3D		locPt;

	const GDM_LOGIC_TILE_ARR *render_tile_list= gdm_GetRenderTiles();

	GeoBuildingProperty p;

	for(int i=0;i<render_tile_list->nCount;i++)
	{
		ptile=render_tile_list->pData+i;//i¸ÜÉã µã¶®À°ËÎË¾ ÊÐ²÷³Þ.

		if(ptile->nLevel<16)
			continue;

		for(int j=0;j<ptile->n3dMeshTileCnt;j++)
		{
			meshtile=ptile->sArr3DMeshTile[j];
			meshs=(SAnimatedMesh*)g_pGDMDataMgr->m_p3DObjectCache->GetMeshs(meshtile->m_sInfo.nX,
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
					gdm_GetDataMgr()->m_pTextMgr->AddText(E_GDM_SUBDATA_TYPE_NONE,locPt, RTT_NAME_DEFAULT,p._Name, 0);
			}
		}//int j=0;j<ptile->n3dMeshTileCnt

		BuildingArray* pSimpleBuildingArr;

		for(int j=0;j<ptile->nPoygonTileCnt;j++)
		{
			GDM_TILE *polyTile=ptile->sArrPolygonTile[j];

			pSimpleBuildingArr=g_pGDMDataMgr->m_pSimpleBuildingCache->GetBuildingArray(polyTile);

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
					gdm_GetDataMgr()->m_pTextMgr->AddText(E_GDM_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING,locPt, RTT_NAME_DEFAULT,psimplebuilding->GetName(), 0);

			}
		}
	}
}

void gdm_MakeRenderGeoNameData()
{
	int tileID = 0;
	unsigned int i;
	int k;
	ST_GEODATA_NAMELIST			*pNameData;
	const GDM_LOGIC_TILE		*pTile;
	int							nameIndex = 0;
	ST_GEODATA_NAME				*pGeoCell;
	unsigned short	*korName,   *natName;

	bool bHeight = false;
	int key = 0;
	bool blKorean = true;

	const GDM_LOGIC_TILE_ARR	*p_name_tile_list = gdm_GetRenderTiles();
	
	int nIndex = p_name_tile_list->nCount;

	GDM_Coord3D camera_coord;
	GDM_POINT3D tmpPos;
	CGDM_Location3D		locPt;
	gdm_GetDataMgr()->m_pCamera->GetCameraCoord(&camera_coord);

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

				locPt.m_tX = pGeoCell->longitude * gdm_DEGTORAD;
				locPt.m_tY = pGeoCell->latitude * gdm_DEGTORAD;

				pBuff += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pRecord->length + pRecord->length_ko;

				natName = pRecord->name;
				korName = pRecord->name + (pRecord->length / 2);

				if(locPt.m_tX < pTile->minLongitude || locPt.m_tX > pTile->maxLongitude ||
					locPt.m_tY < pTile->minLatitude || locPt.m_tY > pTile->maxLatitude)
					continue;

				gdm_get3DWorldPoint(locPt.m_tX, locPt.m_tY, tmpPos);

				//by RSH 2013.3.12 for names not to be rendered if they are behind terrain
				///*
				if (gdm_GetDataMgr()->m_pCamera->m_fScalarOfDir < 0.5)
				{
					CGDM_Vector3DF rayDir;
					rayDir.m_tX = tmpPos.m_X - camera_coord.m_org.m_tX;
					rayDir.m_tY = tmpPos.m_Y - camera_coord.m_org.m_tY;
					rayDir.m_tZ = tmpPos.m_Z - camera_coord.m_org.m_tZ;
					CGDM_Point3DF pCamToTerrainPt(0,0,0);
					GDM_ERROR_NO ret = gdm_intersectRayDEM(camera_coord.m_org , rayDir, &pCamToTerrainPt);

					if (ret == GDM_SUCCESS)	
					{
						double camToNameDist = camera_coord.m_org.getDistanceFrom(CGDM_Vector3DF(tmpPos.m_X, tmpPos.m_Y, tmpPos.m_Z));
						double camToTerrainDist = camera_coord.m_org.getDistanceFrom(CGDM_Vector3DF(pCamToTerrainPt.m_tX,
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
					gdm_GetDataMgr()->m_pTextMgr->AddText((E_GDM_SUBDATA_TYPE)pGeoCell->type,locPt, RTT_PYONGYANG, korName, key, blKorean);
				else
					gdm_GetDataMgr()->m_pTextMgr->AddText((E_GDM_SUBDATA_TYPE)pGeoCell->type, locPt, RTT_NAME_DEFAULT, korName, key, blKorean);
			}
		}
	}

#ifdef DEBUG
	if(g_app->GetConfig()->m_isShowRenderTileName)
		gdm_MakeRenderTileName();
#endif

	if(g_pGDMDataMgr->m_pCamera->m_location.m_dDist<500)
	if(g_pGDMDataMgr->m_pCamera->m_fScalarOfDir>0.5)
	if(g_app->GetConfig()->m_IsShowBuildingName)
		gdm_MakeRenderGeo3dObjectName();
}
