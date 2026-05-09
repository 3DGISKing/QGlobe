#include "QGlobe_LoadDataMgr.h"
#include "QGlobe_Define.h"
#include "QGlobe_ThreadMgr.h"
#include "QGlobe_RequestMgr.h"
#include "QGlobe_DataMgr.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_QuadNode.h"
#include "QGlobe_Mem.h"
#include "common.h"
#include "qglobe_protocol.h"
#include "datachannel.h"
#include "qglobe_config.h"
#include "qglobe_version.h"
#include "../qglobeds_common/QGlobeDSCommon.h"

extern QGlobe_TILE * qglobe_ParsingTileFromResponse(ST_RESPONSE_GEODATA *pResp, E_QGlobe_SUBDATA_TYPE eDataType);
extern void qglobe_PreProcessTile(QGlobe_TILE *pTile);

CQGlobe_LoadDataMgr::CQGlobe_LoadDataMgr(void)
{
}

CQGlobe_LoadDataMgr::~CQGlobe_LoadDataMgr(void)
{
}

/*
 * int	gdmGetTileBuffer(const GdmTileInfo *tileInfo, char * buffer)
 *
 * Description:
 *   Get buffer from tile information
 *
 * Returns: size of buffer if success, -1 if fail
 *
  */

int CQGlobe_LoadDataMgr::GetTileBuffer(const QGlobe_TILE_INFO* tileInfo, void* buffer, void *channel)
{
	RENDER_REQUEST req;
	int			   ret;

	Q_ASSERT(buffer!=NULL);
	Q_ASSERT(tileInfo!=NULL);

	memset(buffer, 0, 40);

	req.bytes = sizeof(req);
	req.type = E_REQUEST_TYPE_GEODATA;
	req.req.column = tileInfo->nX;
	req.req.row	   = tileInfo->nY;
	req.req.level  = tileInfo->nLevel;
	req.req.main_type	= tileInfo->eMainType;
	req.req.sub_type	= tileInfo->eSubType;

	ret = qglobe_GetTileData(channel, &req, buffer, tileInfo->nSize);
	return ret;
}

QGlobe_TILE * CQGlobe_LoadDataMgr::LoadOneTile(QGlobe_TILE_INFO *pTileInfo, void * buffer, void *channel)
{
	int nRet = -1;
	QGlobe_TILE *pTile;
	ST_PACKET_HEADER *pPacket_Hdr;
	ST_RESPONSE_HEADER *pResp_Hdr;
	ST_RESPONSE_GEODATA *pResp_GeoData;

	/* from local */
#ifndef STANDARD_ALONE
	nRet = g_pGDMDataMgr->m_pFileCacheMgr->ReadTileBuffer(pTileInfo, buffer);
#endif

	/* from server */
	if (nRet != RESPONSE_GET_TILE_ERROR_NO)
		nRet = g_pGDMDataMgr->m_pLoadDataMgr->GetTileBuffer(pTileInfo, buffer, channel);


	//success
	if(nRet == RESPONSE_GET_TILE_ERROR_NO)
	{
		pPacket_Hdr = (ST_PACKET_HEADER*)buffer;
		pResp_Hdr	= (ST_RESPONSE_HEADER*)pPacket_Hdr->buffer;

		if(pResp_Hdr->result != E_RESPONSE_ERROR_NONE)
			return NULL;

		// check data
		if(pResp_Hdr->type != E_REQUEST_TYPE_GEODATA)
			return NULL;

		// check response geodata
		pResp_GeoData = (ST_RESPONSE_GEODATA*)pResp_Hdr->body;
		if(pResp_GeoData->column != pTileInfo->nX ||
			pResp_GeoData->row != pTileInfo->nY ||
			pResp_GeoData->level != pTileInfo->nLevel)
		{
			return NULL;
		}

		if(!IsEqualSubVersionByGeoDataType(pResp_GeoData->main_type, pResp_GeoData->version))
			return NULL;

		// get geometric data 
		pTile = qglobe_ParsingTileFromResponse(pResp_GeoData, pTileInfo->eSubType);		// create tile
		if(pTile != NULL)
		{
			memcpy(&pTile->m_sInfo, pTileInfo, sizeof(QGlobe_TILE_INFO));
			return pTile;
		}
		else
			return NULL;
	}
	return NULL;
}

E_LOAD_RESP	qglobe_LoadOneTile(QGlobe_TILE_INFO *pInfo, void * buffer, DataChannel *channel)
{
	int nRet = -1;
	QGlobe_TILE *pTile;
	ST_PACKET_HEADER *pPacket_Hdr;
	ST_RESPONSE_HEADER *pResp_Hdr;
	ST_RESPONSE_GEODATA *pResp_GeoData;
	E_NODE_VALID_FLAG flag;
	QGlobe_TILE_INFO sInfo = *pInfo;
	bool blFileCached = false;

	// set request flag
	g_pGDMDataMgr->m_pRequestMgr->SetStatusFlag(&sInfo, E_TILE_STATUS_REQ);

	// check quad node tree
	flag = g_pGDMDataMgr->m_pNodeMgr->GetValidFlag(sInfo.nX, sInfo.nY, sInfo.nLevel, sInfo.eSubType);
	if(flag == E_NODE_VALID_FLAG_INVALID)
		return LOAD_RESP_NO_EXIST;

	// check cache data 
	if(g_pGDMDataMgr && g_pGDMDataMgr->m_pCacheMgr)
	{
		if(g_pGDMDataMgr->m_pCacheMgr->IsEntryExist(sInfo.eSubType, sInfo.nLevel, sInfo.nX, sInfo.nY))
			return LOAD_RESP_OK;
	}

	if(g_pGDMDataMgr->IsInvalidLevel(pInfo->eSubType,pInfo->nLevel))
	{
		g_pGDMDataMgr->m_pNodeMgr->SetValidFlag(sInfo.nX, sInfo.nY, sInfo.nLevel, sInfo.eSubType, E_NODE_VALID_FLAG_INVALID);
		return LOAD_RESP_NO_EXIST;
	}

	/* from local */
#ifndef STANDARD_ALONE
	nRet = g_pGDMDataMgr->m_pFileCacheMgr->ReadTileBuffer(&sInfo, buffer);
#endif
	if(nRet == RESPONSE_GET_TILE_ERROR_NO)
		blFileCached = true;

	
	/* from server */
	if (nRet != RESPONSE_GET_TILE_ERROR_NO)
		nRet = g_pGDMDataMgr->m_pLoadDataMgr->GetTileBuffer(&sInfo, buffer, channel);

	/*****************************************************
	2013 3 6 ugi explained by ugi

	GetTileBuffer�� �ߺ� 0˾ ���ؼѷ�� ����̩ ������˼ ̩�簡 �ֻ�����.
	******************************************************/
	//if(nRet == RESPONSE_GET_TILE_ERROR_NO)
	{
		pPacket_Hdr = (ST_PACKET_HEADER*)buffer;
		pResp_Hdr	= (ST_RESPONSE_HEADER*)pPacket_Hdr->buffer;

		if(pResp_Hdr->result != E_RESPONSE_ERROR_NONE)
		{
			switch(pResp_Hdr->result)
			{
			case E_RESPONSE_ERROR_FILE_NOEXIST:
				return	 LOAD_RESP_NO_EXIST;
			}
			return LOAD_RESP_ERROR;
		}
		
		// check response geodata
		pResp_GeoData = (ST_RESPONSE_GEODATA*)pResp_Hdr->body;

		// get geometric data 
		pTile = qglobe_ParsingTileFromResponse(pResp_GeoData, sInfo.eSubType);		// create tile

		if(pTile != NULL)
		{
			memcpy(&pTile->m_sInfo, &sInfo, sizeof(QGlobe_TILE_INFO));
			qglobe_PreProcessTile(pTile);
			// write to cache file 
#ifndef STANDARD_ALONE
			if(!blFileCached)
				g_pGDMDataMgr->m_pFileCacheMgr->WriteTileBuffer(&sInfo, buffer);
#endif
			// add tile to the cache
			g_CacheMgrMutex.lock();
			g_pGDMDataMgr->m_pCacheMgr->AddEntry(sInfo.eSubType, sInfo.nLevel, sInfo.nX, sInfo.nY, pTile);
			g_CacheMgrMutex.unlock();

			// add quad node tree
			g_pGDMDataMgr->m_pNodeMgr->SetValidFlag(sInfo.nX, sInfo.nY, sInfo.nLevel, sInfo.eSubType, E_NODE_VALID_FLAG_VALID);

			g_pGDMDataMgr->SetModifyFlag();
			return LOAD_RESP_OK;
		}
		else
		{
			// add quad node tree
			g_pGDMDataMgr->m_pNodeMgr->SetValidFlag(sInfo.nX, sInfo.nY, sInfo.nLevel, sInfo.eSubType, E_NODE_VALID_FLAG_INVALID);
			return LOAD_RESP_NO_EXIST;
		}
	}
	return LOAD_RESP_ERROR;
}

void qglobe_DataLoadThread(void *param)
{
	E_LOAD_RESP			eRet;
	QGlobe_TILE_INFO		sTileInfo, sTileInfoOrg;
	QList<E_QGlobe_SUBDATA_TYPE>	arrTypes;	
	CQGlobe_LoadThread *	pThread			= (CQGlobe_LoadThread*)param;
	E_QGlobe_MAINDATA_TYPE eMainDataType	= pThread->GetDataType();
	DataChannel *		channel			= new DataChannel();
	int					minLevel;		
	int		            index;

	Q_ASSERT(g_pGDMDataMgr!=NULL);
	
	arrTypes=g_pGDMDataMgr->GetTypes(eMainDataType);
	
	pThread->Sleep(1000);

	void* buffer;
	int  bufferSize;

	// malloc buffer
	bufferSize	= g_pGDMDataMgr->GetMaxTileSize(eMainDataType) + 50/*header size */;
	buffer		= gdmMemMalloc(bufferSize);

	while(1)
	{
		while(1)
		{
			if(!pThread->IsActive())
				break;

			if(g_pGDMDataMgr->IsStopDataLoad())
				break;

			// get high priority request tile
			if(!g_pGDMDataMgr->m_pRequestMgr->GetHighRequestTile(&sTileInfoOrg, eMainDataType))
				break;

			Q_ASSERT(sTileInfoOrg.nX>=0);
			Q_ASSERT(sTileInfoOrg.nY>=0);
			Q_ASSERT(sTileInfoOrg.nLevel>=0 && sTileInfoOrg.nLevel<19);

			for(index = 0; index < arrTypes.size(); index++)
			{
				sTileInfo = sTileInfoOrg;
				sTileInfo.nSize  = bufferSize;
				sTileInfo.eMainType = eMainDataType;
				sTileInfo.eSubType  = arrTypes[index];

				if(!g_pGDMDataMgr->IsNecessary(sTileInfo.eSubType))
					continue;

				eRet = qglobe_LoadOneTile(&sTileInfo, buffer, channel);

				minLevel=g_pGDMDataMgr->GetMinLevel(sTileInfo.eSubType);
				while(eRet == LOAD_RESP_NO_EXIST)
				{
					sTileInfo.nLevel--;
					sTileInfo.nX = sTileInfo.nX >> 1;
					sTileInfo.nY = sTileInfo.nY >> 1;

					if(sTileInfo.nLevel < minLevel)
						break;
					eRet = qglobe_LoadOneTile(&sTileInfo, buffer, channel);
				}
				
				if(eRet == LOAD_RESP_OK)
					g_pGDMDataMgr->SetModifyFlag();

				if(eMainDataType != E_QGlobe_MAINDATA_TYPE_IMG)
					pThread->Sleep(5);
			}

			pThread->Sleep(5);
		}

		if(!pThread->IsActive())
			break;

		pThread->Sleep(20);
	}

	if(channel)
	{
		delete  channel;
		channel = NULL;
	}

	gdmMemFree(buffer);

	pThread->SetState(CQGlobe_LoadThread::QGlobe_THREAD_STOPED);
}
