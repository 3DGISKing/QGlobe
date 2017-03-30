#include "GDM_RequestMgr.h"
#include "GDM_Camera.h"
#include "GDM_PyramidMgr.h"
#include "GDM_DataMgr.h"
#include "GDM_TileFunc.h"
#include "GDM_FPSTester.h"
#include "../gds_common/GDSDefine.h"


// for fps feedback
#define	GDM_FPS_MIN_CHECK			8
#define	GDM_FPS_MAX_CHECK			30	
#define REQUEST_START_OFFSET_LEVEL	0
#define REQUEST_VALID_TILE_COUNT	10

#define GDM_REQUEST_DEM_OFFSET_LEVEL	2
#define GDM_REQUEST_IMG_OFFSET_LEVEL	0
#define GDM_REQUEST_NAME_OFFSET_LEVEL	1
#define GDM_REQUEST_SHAPE_OFFSET_LEVEL	0
// end

CGDM_RequestMgr::CGDM_RequestMgr(void)
{
	m_pTileReqList[0] = &m_sTileReq_Img;
	m_pTileReqList[1] = &m_sTileReq_Dem;
	m_pTileReqList[2] = &m_sTileReq_Name;
	m_pTileReqList[3] = &m_sTileReq_Shape;
	m_pTileReqList[4] = &m_sTileReq_3D;
	m_pTileReqList[5] = &m_sTileReq_Img;
	
	m_dCompleteRate = 1.0;
	m_nOffsetLevel	= 0;
	m_blHighSpeed = false;
	m_nOffsetBySpeed = 1;
	m_blDefaultComplete = 0;

	for(int i = 0; i < GDM_MAX_REQUEST_LIST_COUNT; i++)
		memset(m_pTileReqList[i], 0, sizeof(GDM_TILE_REQUEST));

	InitSpec();
	InitDefaultRequest();
}

CGDM_RequestMgr::~CGDM_RequestMgr(void)
{
}
void CGDM_RequestMgr::InitSpec()
{
	// dem tile
	m_sTileReq_Dem.spec.eType		= E_GDM_MAINDATA_TYPE_DEM;
	m_sTileReq_Dem.spec.maxLevel	= 13;
	m_sTileReq_Dem.spec.minLevel	= 0;
	m_sTileReq_Dem.spec.offsetLevel	= GDM_REQUEST_DEM_OFFSET_LEVEL;
	m_sTileReq_Dem.spec.maxRequestCount	= 10;
	
	// img tile
	m_sTileReq_Img.spec.eType		= E_GDM_MAINDATA_TYPE_IMG;
	m_sTileReq_Img.spec.maxLevel	= 19;
	m_sTileReq_Img.spec.minLevel	= 0;
	m_sTileReq_Img.spec.offsetLevel	= GDM_REQUEST_IMG_OFFSET_LEVEL;
	m_sTileReq_Img.spec.maxRequestCount	= 16;
	
	// name
	m_sTileReq_Name.spec.eType			= E_GDM_MAINDATA_TYPE_NAME;
	m_sTileReq_Name.spec.maxLevel		= 16;
	m_sTileReq_Name.spec.minLevel		= 3;
	m_sTileReq_Name.spec.offsetLevel	= GDM_REQUEST_NAME_OFFSET_LEVEL;
	m_sTileReq_Name.spec.maxRequestCount	= 6;

	
	// shape
	m_sTileReq_Shape.spec.eType			= E_GDM_MAINDATA_TYPE_SHAPE;
	m_sTileReq_Shape.spec.maxLevel		= 17;
	m_sTileReq_Shape.spec.minLevel		= 4;
	m_sTileReq_Shape.spec.offsetLevel	= GDM_REQUEST_SHAPE_OFFSET_LEVEL;
	m_sTileReq_Shape.spec.maxRequestCount	= 6;

	// 3d
	m_sTileReq_3D.spec.eType			= E_GDM_MAINDATA_TYPE_3D;
	m_sTileReq_3D.spec.maxLevel		= 18;
	m_sTileReq_3D.spec.minLevel		= 14;
	m_sTileReq_3D.spec.offsetLevel	= 0;
	m_sTileReq_3D.spec.maxRequestCount	= 6;


}

GDM_TILE_REQUEST* CGDM_RequestMgr::GetTileRequest(E_GDM_MAINDATA_TYPE eType)
{
	GDM_TILE_REQUEST *pReqList = NULL;

	switch(eType)
	{
	case E_GDM_MAINDATA_TYPE_DEM:
		pReqList = &m_sTileReq_Dem;
		break;
	case E_GDM_MAINDATA_TYPE_IMG:
		pReqList = &m_sTileReq_Img;
		break;
	case E_GDM_MAINDATA_TYPE_NAME:
		pReqList = &m_sTileReq_Name;
		break;
	case E_GDM_MAINDATA_TYPE_SHAPE:
		pReqList = &m_sTileReq_Shape;
		break;
	case E_GDM_MAINDATA_TYPE_3D:
		pReqList = &m_sTileReq_3D;
		break;
	case E_GDM_MAINDATA_TYPE_RASTER:
		pReqList = &m_sTileReq_Img;
		break;
	}

	return pReqList;
}

const GDM_TILE_SPEC* CGDM_RequestMgr::GetSpec(E_GDM_MAINDATA_TYPE eType)
{
	GDM_TILE_REQUEST *pReq = GetTileRequest(eType);

	Q_ASSERT(pReq != NULL);

	return &pReq->spec;
}

void CGDM_RequestMgr::SetStatusFlag(GDM_TILE_INFO *pInfo, E_TILE_STATUS status)
{
	GDM_TILE_REQUEST *pRequest = NULL;
	GDM_TILE_INFO_ARR *pReqList = NULL;

	pRequest = GetTileRequest(pInfo->eMainType);

	Q_ASSERT(pRequest != NULL);

	pReqList = &pRequest->reqList;

	if(pRequest->isLock)
		return;

	for(int i = 0; i < pReqList->count; i++)
	{
		if(pReqList->data[i].nLevel == pInfo->nLevel &&
			pReqList->data[i].nX == pInfo->nX &&
			pReqList->data[i].nY == pInfo->nY)
		{
			pReqList->data[i].nStatus = status;
			return;
		}
	}
}

//°¡»þ Ë§ºº±îËË ²ÚË¼ À°ËÎË¾ ´®¶°³Þ.
//ËË·ç Êý½ìÂÙ À°ËÎË¾ ´®¶®¼è Ê±²÷³Þ.

bool CGDM_RequestMgr::GetHighRequestTile(GDM_TILE_INFO *tile, E_GDM_MAINDATA_TYPE eType)
{
	GDM_TILE_REQUEST *pRequest = NULL;
	GDM_TILE_INFO_ARR *pReqList = NULL;
	GDM_TILE_INFO *pTile = NULL;
	float prio = -1;

	memset(tile, 0, sizeof(GDM_TILE_INFO));

	pRequest = GetTileRequest(eType);
	
	Q_ASSERT(pRequest != NULL);

	pReqList = &pRequest->reqList;

	if(pRequest->isLock)
		return NULL;

	for(int i = 0; i < pReqList->count; i++)
	{
		if(pRequest->isLock)
			return NULL;

		if(pReqList->data[i].nStatus != E_TILE_STATUS_NONE)
			continue;

		if(prio < 0 || pReqList->data[i].area2D > prio)
		{
			prio = pReqList->data[i].area2D;
			pTile = pReqList->data + i;
		}
	}

	if(pTile == NULL)
	{
		pRequest->isEmpty = true;
		return false;
	}

	memcpy(tile, pTile, sizeof(GDM_TILE_INFO));


	Q_ASSERT(tile->nX>=0);
	Q_ASSERT(tile->nY>=0);
	Q_ASSERT(tile->nLevel>=0 && tile->nLevel<19);
	return true;
}

// add one tile
void addOnTile(int nX, int nY, int nLevel, int prio, GDM_TILE_INFO_ARR *tileArr)
{
	if(tileArr->count >= GDM_MAX_TILE)
		return;

	tileArr->data[tileArr->count].nLevel= nLevel;
	tileArr->data[tileArr->count].nX	= nX;
	tileArr->data[tileArr->count].nY	= nY;
	tileArr->data[tileArr->count].area2D= prio;
	tileArr->count ++;
}

void CGDM_RequestMgr::InitDefaultRequest()
{
	GDM_TILE_INFO_ARR tileArr;

	tileArr.count = 0;
	memset(&tileArr, 0, sizeof(GDM_TILE_INFO_ARR));

	// add 0 level tile
	addOnTile(0,0,0,TILE_PRIO_DEFAULT, &tileArr);

	// add 1 level tile
	addOnTile(0,0,1,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(0,1,1,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(1,0,1,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(1,1,1,TILE_PRIO_DEFAULT, &tileArr);

	// add 2 level tile
	addOnTile(0,1,2,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(0,2,2,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(1,1,2,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(1,2,2,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(2,1,2,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(2,2,2,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(3,1,2,TILE_PRIO_DEFAULT, &tileArr);
	addOnTile(3,2,2,TILE_PRIO_DEFAULT, &tileArr);

	// add request tiles
	UpdateRequestTiles(&m_sTileReq_Dem, &tileArr);
	UpdateRequestTiles(&m_sTileReq_Img, &tileArr);
}

void CGDM_RequestMgr:: UpdateRequest()
{
	GDM_TILE_INFO_ARR tileArr;
	GDM_TILE_INFO_ARR tileArrforEstimate;
	
	if(!g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
		m_blDefaultComplete = true;

	if(!m_blDefaultComplete)
	{
		for(int i = 0; i < GDM_MAX_REQUEST_LIST_COUNT; i++)
		{
			if(IsExistItem(&m_pTileReqList[i]->reqList))
				return;
		}
		m_blDefaultComplete = true;
	}

	// get pyramid tiles
	g_pGDMDataMgr->m_pPyramidMgr->GetPyramidTiles(&tileArr);

	// get near tile info for dem tile
	double logitidue = g_pGDMDataMgr->m_pCamera->m_location.m_dLongitude;
	double latitude  = g_pGDMDataMgr->m_pCamera->m_location.m_dLatitude;
	int	   nLevel = tileArr.maxLevel;
	g_pGDMDataMgr->m_pPyramidMgr->GetNear4Tiles(nLevel, logitidue, latitude, &tileArrforEstimate);

	CheckRate();

	if(tileArr.count > 0)
	{
		if(tileArrforEstimate.count+tileArr.count>GDM_MAX_TILE)
		{
			qFatal("Program encountered error in CGDM_RequestMgr::UpdateRequest! file:%s line:%d",__FILE__,__LINE__);
		}
		memcpy(&tileArrforEstimate.data[tileArrforEstimate.count], tileArr.data, sizeof(GDM_TILE_INFO) * tileArr.count);
		tileArrforEstimate.count += tileArr.count;
		tileArrforEstimate.maxLevel = tileArr.maxLevel;
	}

	// add request tiles
	for(int i = 0; i < GDM_MAX_REQUEST_LIST_COUNT; i++)
	{
		if(m_pTileReqList[i]->spec.eType == E_GDM_MAINDATA_TYPE_DEM)
			UpdateRequestTiles(m_pTileReqList[i], &tileArrforEstimate);
		else
			UpdateRequestTiles(m_pTileReqList[i], &tileArr);
	}
}

void CGDM_RequestMgr::UpdateRequestTiles(GDM_TILE_REQUEST *pRequest, GDM_TILE_INFO_ARR *pPyramidList)
{
	GDM_TILE_INFO cTile;
	GDM_TILE_INFO_ARR newReqList;
	int i,j, isReq, index = 0;
	int sLevel, eLevel, nLevel, nX, nY;
	int offsetLevel;	// for speed
	int maxRequestCount; // for speed

	pRequest->isLock = true;

	maxRequestCount = (int)(pRequest->spec.maxRequestCount + 2);
	if(!m_blDefaultComplete)
		maxRequestCount = 13;

	memset(&newReqList, 0, sizeof(GDM_TILE_INFO_ARR));

	for(i = 0; i < pPyramidList->count; i++)
	{
		memcpy(&cTile, pPyramidList->data + i, sizeof(GDM_TILE_INFO));

		if(i >= maxRequestCount * 3)
			break;

		if(pRequest->spec.eType == E_GDM_MAINDATA_TYPE_IMG)
			offsetLevel = m_nOffsetLevel - m_nOffsetBySpeed;
		else
			offsetLevel = m_nOffsetLevel;

		if(pRequest->spec.eType != E_GDM_MAINDATA_TYPE_IMG && cTile.offsetLevel > 0)
			offsetLevel += cTile.offsetLevel;

		// shift with offset level
		if(pRequest->spec.offsetLevel > 0)
		{
			cTile.nLevel = cTile.nLevel - pRequest->spec.offsetLevel;
			cTile.nX = cTile.nX >> pRequest->spec.offsetLevel;
			cTile.nY = cTile.nY >> pRequest->spec.offsetLevel;
		}

		// shift with offset level for speed in feedback structure
		if(offsetLevel > (cTile.nLevel - 1) && cTile.nLevel > 0)
			offsetLevel = cTile.nLevel - 1;

		if(offsetLevel > 0)
		{
			cTile.nLevel = cTile.nLevel - offsetLevel;
			cTile.nX = cTile.nX >> offsetLevel;
			cTile.nY = cTile.nY >> offsetLevel;
		}

		// check start/end level
		eLevel = cTile.nLevel;
		sLevel = cTile.nLevel - REQUEST_START_OFFSET_LEVEL;

		if(sLevel > pRequest->spec.maxLevel)
			sLevel = pRequest->spec.maxLevel;
		else if(sLevel < pRequest->spec.minLevel)
			sLevel = pRequest->spec.minLevel;
		if(eLevel > pRequest->spec.maxLevel)
			eLevel = pRequest->spec.maxLevel;
		else if(eLevel < pRequest->spec.minLevel)
			eLevel = -1;

		if(sLevel > eLevel)
			continue;

		if(i >= REQUEST_VALID_TILE_COUNT)
			eLevel = sLevel;

		nLevel	= cTile.nLevel;
		nX		= cTile.nX;
		nY		= cTile.nY;

		cTile.nLevel = sLevel - 1;

		while(cTile.nLevel < eLevel)
		{
			cTile.nLevel++;
			cTile.nX = nX >> (nLevel - cTile.nLevel);
			cTile.nY = nY >> (nLevel - cTile.nLevel);

			// check request tile status
			isReq = 1;
			for(j = 0; j < pRequest->reqList.count; j++)
			{
				if(pRequest->reqList.data[j].nStatus == E_TILE_STATUS_REQ && gdm_IsSameTile(&cTile, pRequest->reqList.data + j))
				{
					isReq =0 ;
					break;
				}
			}
			if(!isReq)
				continue;

			// check list
			if(gdm_IsContained(cTile.nX, cTile.nY, cTile.nLevel, &newReqList))
				continue;

			// add tile
			newReqList.data[index].eMainType = pRequest->spec.eType;
			newReqList.data[index].nX	 = cTile.nX;
			newReqList.data[index].nY	 = cTile.nY;
			newReqList.data[index].nLevel = cTile.nLevel;
			newReqList.data[index].area2D  = cTile.area2D > 0?cTile.area2D:0;
			index++;
			newReqList.count = index;


			Q_ASSERT(cTile.nX>=0);
			Q_ASSERT(cTile.nY>=0);
			Q_ASSERT(cTile.nLevel>=0 && cTile.nLevel<19);
			if(index >= maxRequestCount)
			{
				break;
			}
		}
		if(index >= maxRequestCount)
		{
			break;
		}
	}

	// replace request tile list
	memcpy(&(pRequest->reqList), &newReqList, sizeof(GDM_TILE_INFO_ARR));
	pRequest->isEmpty = false;
	pRequest->isLock = false;
}

int CGDM_RequestMgr::IsExistItem(GDM_TILE_INFO_ARR *pReqList)
{
	int i = 0;
	for(i = 0; i < pReqList->count; i++)
	{
		if(pReqList->data[i].nStatus == E_TILE_STATUS_NONE)
			return TRUE;
	}
	return FALSE;
}

void CGDM_RequestMgr::CheckRate(void)
{
	GDM_TILE_REQUEST *pRequest;
	int allReqCount, completeCount, baseOffsetLevel;

	allReqCount = 0;
	completeCount = 0;
	for(int i = 0; i < GDM_MAX_REQUEST_LIST_COUNT; i++)
	{
		pRequest = m_pTileReqList[i];
		allReqCount += pRequest->reqList.count;
		for(int j = 0; j < pRequest->reqList.count; j++)
		{
			if(pRequest->reqList.data[j].nStatus != E_TILE_STATUS_NONE)
				completeCount++;
		}
	}

	if(allReqCount == 0)
		m_dCompleteRate = 1.0;
	else
	{
		m_dCompleteRate = ((double)completeCount) / allReqCount;
		m_dCompleteRate=qBound(0.1,m_dCompleteRate,1.0);

	}

	baseOffsetLevel = log(1 / m_dCompleteRate) / log(2.0);

	long curFrame = gdm_GetFPSTester()->m_curFrame;

	if(m_blHighSpeed && curFrame < GDM_FPS_MIN_CHECK)
	{
		m_nOffsetBySpeed =1;
		m_blHighSpeed = false;
	}

	if(!m_blHighSpeed && curFrame > GDM_FPS_MAX_CHECK)
	{
		m_nOffsetBySpeed = 0;
		m_blHighSpeed = true;
	}

	if(g_pGDMDataMgr->m_sOption.blContour)
		m_nOffsetLevel = baseOffsetLevel;
	else
		m_nOffsetLevel = baseOffsetLevel + m_nOffsetBySpeed;

//	qDebug("OffsetLevel=%d",m_nOffsetLevel);
}

void CGDM_RequestMgr::ClearRequest(E_GDM_MAINDATA_TYPE eType)
{
	for(int i = 0; i < GDM_MAX_REQUEST_LIST_COUNT; i++)
	{
		if(m_pTileReqList[i]->spec.eType == eType)
			m_pTileReqList[i]->reqList.count=0;
	}
}

void CGDM_RequestMgr::UpdateRequest(E_GDM_MAINDATA_TYPE eType)
{
	GDM_TILE_INFO_ARR tileArr;
	GDM_TILE_INFO_ARR tileArrforEstimate;

	if(!g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
		m_blDefaultComplete = true;

	if(!m_blDefaultComplete)
	{
		for(int i = 0; i < GDM_MAX_REQUEST_LIST_COUNT; i++)
		{
			if(IsExistItem(&m_pTileReqList[i]->reqList))
				return;
		}
		m_blDefaultComplete = true;
	}

	// get pyramid tiles
	g_pGDMDataMgr->m_pPyramidMgr->GetPyramidTiles(&tileArr);

	// get near tile info for dem tile
	double logitidue = g_pGDMDataMgr->m_pCamera->m_location.m_dLongitude;
	double latitude  = g_pGDMDataMgr->m_pCamera->m_location.m_dLatitude;
	int	   nLevel = tileArr.maxLevel;
	g_pGDMDataMgr->m_pPyramidMgr->GetNear4Tiles(nLevel, logitidue, latitude, &tileArrforEstimate);

	CheckRate();

	if(tileArr.count > 0)
	{
		if(tileArrforEstimate.count+tileArr.count>GDM_MAX_TILE)
		{
			qFatal("Program encountered error in CGDM_RequestMgr::UpdateRequest! file:%s line:%d",__FILE__,__LINE__);
		}
		memcpy(&tileArrforEstimate.data[tileArrforEstimate.count], tileArr.data, sizeof(GDM_TILE_INFO) * tileArr.count);
		tileArrforEstimate.count += tileArr.count;
		tileArrforEstimate.maxLevel = tileArr.maxLevel;
	}

	// add request tiles
	for(int i = 0; i < GDM_MAX_REQUEST_LIST_COUNT; i++)
	{
		if(m_pTileReqList[i]->spec.eType == eType)
			UpdateRequestTiles(m_pTileReqList[i], &tileArr);
	}
}
