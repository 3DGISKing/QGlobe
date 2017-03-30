#include "GDM_TextMgr.h"
#include "GDM_Camera.h"
#include "GIS_PlaceIcon.h"
#include "GIS_Path.h"
#include "GIS_Area.h"
#include "GIS_Doc.h"
#include "GDM_DataMgr.h"
#include "GDM_PyramidMgr.h"
#include "gdemclientbase.h"
#include "../gds_common/GDSCommon.h"
#include "GIS_Dataset.h"
#include "GDM_GISDataCache.h"

#define PLACEMARK_LIMIT_DIST	20000000.0f

double IsBackFace(GDM_Coord3D aCameraPos, GDM_POINT3D aWorldPos)
{
	GDM_POINT3D vt;

	vt.m_X = aCameraPos.m_org.m_tX - aWorldPos.m_X;
	vt.m_Y = aCameraPos.m_org.m_tY - aWorldPos.m_Y;
	vt.m_Z = aCameraPos.m_org.m_tZ - aWorldPos.m_Z;
	double scalra = aWorldPos.m_X * vt.m_X + aWorldPos.m_Y * vt.m_Y + aWorldPos.m_Z * vt.m_Z;

	return scalra;
}
//added by RSH 2013.3.13
#define CAMERA_TILT_ANGLE 0.5
//end

void gdm_PreparePlacemarkData()
{
	CGIS_Doc*			pGISDoc = gdm_GetGISDoc();
	CGDM_DataMgr*		pGDMDataMgr = gdm_GetDataMgr();
	CGIS_Node*			pNode = NULL;
	unsigned int index, nCount = 0;
	RENTER_TEXT_TYPE	type = RTT_PLACE_MARK_NORMAL;
	CGDM_Location3D		locPt;
	QString				strName;
	int					key = 0;
	bool				blKorean = true;

	if (gdm_GetDataMgr()->m_pCamera->m_location.m_dDist > PLACEMARK_LIMIT_DIST)
		return;

	GDM_Coord3D camera_coord;
	pGDMDataMgr->m_pCamera->GetCameraCoord(&camera_coord);

	if(pGISDoc == NULL)
		return;
	nCount = pGISDoc->GetCount();

	QString iconname;

	for(index = 0; index < nCount; index++)
	{
		pNode = pGISDoc->GetNodeFromIndex(index);

		// check visible state
		if(!pNode->IsVisible())
			continue;

		key = pNode->GetKey();

		// check place mark
		if(pNode->Get_GisType() == E_GISNODE_TYPE_PLACEMARK || pNode->Get_GisType() == E_GISNODE_TYPE_PLACESEARCH)
		{
			CGIS_PlaceIcon * pMarkNode	= (CGIS_PlaceIcon*)pNode;
			locPt.m_tX	= pMarkNode->GetPt().m_tX;
			locPt.m_tY	= pMarkNode->GetPt().m_tY;

			switch(pMarkNode->GetState())
			{
			case E_GISNODE_STATE_NONE:
				type = RTT_PLACE_MARK_NORMAL;
				break;
			case E_GISNODE_STATE_SELECT:
				type = RTT_PLACE_MARK_SELECT;
				break;
			}

			strName = strName.fromUtf16(pMarkNode->m_sName);
			
			// get screen point
			CGDM_Vector3DF pos;
			g_pGDMDataMgr->m_pCamera->WorldToScreen(locPt.m_tX, locPt.m_tY, &pos);
			pMarkNode->SetScreenPos(pos.m_tX, pos.m_tY);

		}
		// check length of polyline
		else if(pNode->Get_GisType() == E_GISNODE_TYPE_PATH || pNode->Get_GisType() == E_GISNODE_TYPE_RULER)
		{
			if(pGDMDataMgr->m_sOption.showLenArea)
			{
				CGIS_Polygon   * pMarkNode	= (CGIS_Polygon*)pNode;
				//int n = (pMarkNode->GetCount() - 1) / 2;

				if(pMarkNode->GetCount() > 1)
				{
					//longitude	= (pMarkNode->m_PtArr[n].m_tX + pMarkNode->m_PtArr[n + 1].m_tX) / 2;
					//latitude	= (pMarkNode->m_PtArr[n].m_tY + pMarkNode->m_PtArr[n + 1].m_tY) / 2;

					locPt.m_tX	= (pMarkNode->m_cBound.m_cMinPt.m_tX + pMarkNode->m_cBound.m_cMaxPt.m_tX) / 2;
					locPt.m_tY	= (pMarkNode->m_cBound.m_cMinPt.m_tY + pMarkNode->m_cBound.m_cMaxPt.m_tY) / 2;

					type = RTT_PLACE_LENGTH;
					
					QString length;
					double dlength = 0;

					if (pNode->Get_GisType() == E_GISNODE_TYPE_RULER)
						length = pMarkNode->ConvertLengthToString();
					else
					{
						dlength = pMarkNode->m_dLength;

						if (dlength < 1000)
							length = QString("%1m").arg(dlength);
						else
							length = QString("%1km").arg(dlength/1000);
					}

					strName = length;		
					blKorean = false;
				}
			}
		}

		// check length of polyline
		else if(pNode->Get_GisType() == E_GISNODE_TYPE_AREA)
		{
			if(pGDMDataMgr->m_sOption.showLenArea)
			{
				CGIS_Area   * pMarkNode	= (CGIS_Area*)pNode;

				if(pMarkNode->m_dArea>0)
				{
					locPt.m_tX	= (pMarkNode->m_cBound.m_cMinPt.m_tX + pMarkNode->m_cBound.m_cMaxPt.m_tX) / 2;
					locPt.m_tY	= (pMarkNode->m_cBound.m_cMinPt.m_tY + pMarkNode->m_cBound.m_cMaxPt.m_tY) / 2;
					type = RTT_PLACE_AREA;
					
					double	 area = pMarkNode->m_dArea;

					if (area < 1000000)
						strName = QString("%1m%2").arg(area).arg(QChar(0x00B2));
					else
						strName = QString("%1km%2").arg(area/1000000).arg(QChar(0x00B2));
					blKorean = false;
				}
			}
		}

		GDM_POINT3D tmpPos;
		gdm_get3DWorldPoint(locPt.m_tX, locPt.m_tY, tmpPos, 0, NULL, true);

		//by RSH 2013.3.12 for placemarks not to be rendered if they are behind terrain
		///*
		if (gdm_GetDataMgr()->m_pCamera->m_fScalarOfDir < CAMERA_TILT_ANGLE)
		{
			CGDM_Vector3DF rayDir;
			rayDir.m_tX = tmpPos.m_X - camera_coord.m_org.m_tX;
			rayDir.m_tY = tmpPos.m_Y - camera_coord.m_org.m_tY;
			rayDir.m_tZ = tmpPos.m_Z - camera_coord.m_org.m_tZ;
			CGDM_Point3DF pCamToTerrainPt(0,0,0);
			GDM_ERROR_NO ret = gdm_intersectRayDEM(camera_coord.m_org , rayDir , &pCamToTerrainPt);

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

		if (IsBackFace(camera_coord, tmpPos) < 0)
		{
			continue;
		}

		locPt.m_tZ = 0;

		// add text

		//by RSH 2013.7.25

		if(pNode->Get_GisType() == E_GISNODE_TYPE_PLACEMARK )
		{
			iconname=QString::fromUtf16(((CGIS_PlaceIcon*)pNode)->m_sIconName);

			pGDMDataMgr->m_pTextMgr->AddText(locPt, type, strName, key, blKorean, iconname,((CGIS_PlaceIcon*)pNode)->m_IconScale);
		}
		else
			pGDMDataMgr->m_pTextMgr->AddText(E_GDM_SUBDATA_TYPE_NONE,locPt, type, strName, key, blKorean);
	}

}

void gdm_PrepareLinkLenAreaName()
{
	if(!g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_NAME_POLUTANT))
		return;
	if(g_pGDMDataMgr->IsInvalidLevel(E_GDM_SUBDATA_TYPE_NAME_POLUTANT,g_pGDMDataMgr->m_pPyramidMgr->GetMaxLevel()))
		return;
	
	GISLayer* layer=g_pGDMDataMgr->m_pGISDataCache->GetLayer(0,0,0,E_GDM_SUBDATA_TYPE_NAME_ACCIDENT);

	if(!layer)
		return;
	
	CGDM_Location3D		locPt;
	RENTER_TEXT_TYPE	type;

	for(int k=0;k<layer->FeatureCount();k++)
	{
		CGIS_Path* pNode=(CGIS_Path*)layer->GetFeature(k);
		
		locPt.m_tX	= (pNode->m_cBound.m_cMinPt.m_tX + pNode->m_cBound.m_cMaxPt.m_tX) / 2;
		locPt.m_tY	= (pNode->m_cBound.m_cMinPt.m_tY + pNode->m_cBound.m_cMaxPt.m_tY) / 2;

		type = RTT_PLACE_LENGTH;

		QString length;
		double dlength = 0;
		
		dlength = pNode->m_dLength;

		if (dlength < 1000)
			length = QString("%1m").arg(dlength);
		else
			length = QString("%1km").arg(dlength/1000);

		
		g_pGDMDataMgr->m_pTextMgr->AddText(E_GDM_SUBDATA_TYPE_NONE,locPt, type, length, -1, true);
	}
}

void gdm_GetPlaceCameraInfo(PLACEMARK_DATA* pData)
{
	GDM_CAMERA_INFO cameraInfo;

	if (pData == NULL)
		return;

	g_pGDMDataMgr->m_pCamera->GetInfo(&cameraInfo);
	pData->view_longitude	= cameraInfo.m_loc.m_dLongitude * gdm_RADTODEG;
	pData->view_latitude	= cameraInfo.m_loc.m_dLatitude* gdm_RADTODEG;
	pData->view_range		= cameraInfo.m_loc.m_dDist;		
	pData->view_tilt		= cameraInfo.m_pitch * gdm_RADTODEG;
	pData->view_heading		= cameraInfo.m_roll * gdm_RADTODEG;
	pData->orgDist			= cameraInfo.m_orgDist;
}
