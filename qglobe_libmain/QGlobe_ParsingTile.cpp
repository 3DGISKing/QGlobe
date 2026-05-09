#include "QGlobe_Tile.h"
#include "QGlobe_Jpeg.h"
#include "qglobe_protocol.h"
#include "QGlobe_Define.h"
#include "QGlobe_Mem.h"
#include "QGlobe_DataMgr.h"
#include "LzmaLib.h"

QGlobe_TILE * qglobe_ParsingDemTileFromResp(ST_RESPONSE_GEODATA *pResp);	// parsing dem tile
QGlobe_TILE * qglobe_ParsingImgTileFromResp(ST_RESPONSE_GEODATA *pResp);	// parsing texture tile
QGlobe_TILE * qglobe_ParsingShapeTileFromResp(ST_RESPONSE_GEODATA *pResp);// parsing shape tile
QGlobe_TILE * qglobe_ParsingNameTileFromResp(ST_RESPONSE_GEODATA *pResp);	// parsing name tile
QGlobe_TILE * qglobe_Parsing3DMeshTileFromResp(ST_RESPONSE_GEODATA *pResp);// parsing 3dmesh tile
QGlobe_TILE * qglobe_ParsingRasterTileFromResp(ST_RESPONSE_GEODATA *pResp);// parsing 3dmesh tile

QGlobe_TILE * qglobe_ParsingTileFromResponse(ST_RESPONSE_GEODATA *pResp, E_QGlobe_SUBDATA_TYPE eDataType)
{
	E_QGlobe_MAINDATA_TYPE mainType = qglobe_GetMainType(eDataType);
	switch(mainType)
	{
	case E_QGlobe_MAINDATA_TYPE_DEM:
		return qglobe_ParsingDemTileFromResp(pResp);
	case E_QGlobe_MAINDATA_TYPE_IMG:
		return qglobe_ParsingImgTileFromResp(pResp);
	case E_QGlobe_MAINDATA_TYPE_SHAPE:
		return qglobe_ParsingShapeTileFromResp(pResp);
	case E_QGlobe_MAINDATA_TYPE_NAME:
		return qglobe_ParsingNameTileFromResp(pResp);
	case E_QGlobe_MAINDATA_TYPE_3D:
		return qglobe_Parsing3DMeshTileFromResp(pResp);
	case E_QGlobe_MAINDATA_TYPE_RASTER:
		return qglobe_ParsingRasterTileFromResp(pResp);
	}
	return NULL;
}

// decompress buffer
void qglobe_DeCompressBuffer(int inBufLen, void* iBuf, int outBufLen, void *oBuf, E_COMPRESS_TYPE CompressType)
{
	size_t	lBufferSizeCpr = 0;
	switch(CompressType)
	{
	case E_COMPRESS_JPG:
		qglobe_JPEGDecompressTile(iBuf, oBuf, inBufLen);
		break;
	case E_COMPRESS_7ZIP:
		lBufferSizeCpr = inBufLen - LZMA_PROPS_SIZE - sizeof(size_t);
		LzmaUncompress((unsigned char *)oBuf, 
			(size_t*)&outBufLen, 
			(unsigned char *)iBuf + LZMA_PROPS_SIZE + sizeof(size_t), 
			&lBufferSizeCpr, 
			(unsigned char *)iBuf + sizeof(size_t),
			*((size_t*)iBuf));
		break;
	default:
		memcpy(oBuf, iBuf, inBufLen);
	}
}

// create dem tile from response
QGlobe_TILE * qglobe_ParsingDemTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	QGlobe_DEM_TILE *pTile = NULL;
	ST_GEODATA_DEM *pData = (ST_GEODATA_DEM*)pResp->data;
	int allSize;

	if(pData->bytes == 0)
		return NULL;

	if(pData->bytes > g_pGDMDataMgr->GetMaxTileSize(E_QGlobe_MAINDATA_TYPE_DEM)) // invalide buffer
		return NULL;

	allSize = sizeof(QGlobe_DEM_TILE);

	pTile = (QGlobe_DEM_TILE*)gdmMemMalloc(allSize);

	if(pTile == NULL) // fail malloc memory
		return NULL;

	memset(pTile, 0, allSize);

	if(pData->bytes > 0)
	{
		qglobe_DeCompressBuffer(pData->bytes, (void*)pData->buffer, allSize, (void*)pTile->m_pData, pData->compress_type);
		pTile->m_isValidBuffer = TRUE;
	}
	else
		pTile->m_isValidBuffer = FALSE;

	return (QGlobe_TILE*)pTile;
}

// create dem tile from response
QGlobe_TILE * qglobe_Parsing3DMeshTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	QGlobe_TILE      *pTile = NULL;
	ST_GEODATA_3DMESH *pData = (ST_GEODATA_3DMESH*)pResp->data;
	int allSize;

	if(pData->bytes == 0)
		return NULL;

	if(pData->bytes > g_pGDMDataMgr->GetMaxTileSize(E_QGlobe_MAINDATA_TYPE_3D)) // invalide buffer
		return NULL;

	allSize = sizeof(QGlobe_TILE)+ pData->bytes;

	pTile = (QGlobe_TILE*)gdmMemMalloc(allSize);

	if(pTile == NULL) // fail malloc memory
		return NULL;

	memset(pTile, 0, allSize);

	if(pData->bytes > 0)
	{
		memcpy(pTile->m_pBuffer,pData->buffer,pData->bytes);
		pTile->m_isValidBuffer = TRUE;
	}
	else
		pTile->m_isValidBuffer = FALSE;

	return (QGlobe_TILE*)pTile;
}

QGlobe_TILE * qglobe_ParsingRasterTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	QGlobe_TILE      *pTile = NULL;
	ST_GEODATA_3DMESH *pData = (ST_GEODATA_3DMESH*)pResp->data;
	int allSize;

	if(pData->bytes == 0)
		return NULL;

	if(pData->bytes > g_pGDMDataMgr->GetMaxTileSize(E_QGlobe_MAINDATA_TYPE_RASTER)) // invalide buffer
		return NULL;

	allSize = sizeof(QGlobe_TILE)+ pData->bytes;

	pTile = (QGlobe_TILE*)gdmMemMalloc(allSize);

	if(pTile == NULL) // fail malloc memory
		return NULL;

	memset(pTile, 0, allSize);

	if(pData->bytes > 0)
	{
		memcpy(pTile->m_pBuffer,pData->buffer,pData->bytes);
		pTile->m_isValidBuffer = TRUE;
	}
	else
		pTile->m_isValidBuffer = FALSE;

	return (QGlobe_TILE*)pTile;
}


// create dem tile from response
QGlobe_TILE * qglobe_ParsingImgTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	QGlobe_TEXTURE_TILE *pTile = NULL;
	ST_GEODATA_TEXTURE *pData = (ST_GEODATA_TEXTURE*)pResp->data;
	int allSize;

	if(pData->bytes == 0)
		return NULL;

	if(pData->bytes > g_pGDMDataMgr->GetMaxTileSize(E_QGlobe_MAINDATA_TYPE_IMG)) // invalide buffer
		return NULL;

	allSize = sizeof(QGlobe_TEXTURE_TILE) + pData->bytes;

	pTile = (QGlobe_TEXTURE_TILE*)gdmMemMalloc(allSize);

	if(pTile == NULL)
		return NULL; // fail malloc memory

	memset(pTile, 0, allSize);

	if(pData->bytes > 0)
	{
		// qglobe_DeCompressBuffer(pData->bytes, (void*)pData->buffer, allSize, (void*)pTile->m_pBuffer, pData->compres_type);
		memcpy(pTile->m_pBuffer, pData->buffer, pData->bytes);
		pTile->m_nBufSize = pData->bytes;
		pTile->m_isValidBuffer = TRUE;
	}
	else
		pTile->m_isValidBuffer = FALSE;

	return (QGlobe_TILE*)pTile;
}

// create shape tile from response
QGlobe_TILE * qglobe_ParsingShapeTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	QGlobe_SHAPE_TILE *pTile = NULL;
	ST_GEODATA_SHAPE *pData = (ST_GEODATA_SHAPE*)pResp->data;

	QGlobe_SHAPE_DATA *pShapeData = (QGlobe_SHAPE_DATA*)pData->buffer;	
	int nBufSize = pData->bytes - sizeof(QGlobe_SHAPE_DATA) + sizeof(QGlobe_POLYGON);
	int allSize;
	
	if(nBufSize <= 0)
		return NULL;
	
	if((unsigned int)nBufSize > g_pGDMDataMgr->GetMaxTileSize(E_QGlobe_MAINDATA_TYPE_SHAPE)) // invalide buffer
		return NULL;
		
	allSize = sizeof(QGlobe_SHAPE_TILE) + nBufSize;

	pTile = (QGlobe_SHAPE_TILE*)gdmMemMalloc(allSize);

	if(pTile == NULL) // fail malloc memory
		return NULL;

	memset(pTile, 0, allSize);

	if(nBufSize > 0)
	{
		memcpy((void*)pTile->m_pData, (void*)pShapeData->m_pPolygon, pData->bytes-sizeof(int) * 2);
		pTile->m_nBufSize = nBufSize;
		pTile->m_nPolygonCount = pShapeData->m_nCount;
		pTile->m_isValidBuffer = TRUE;
	}
	else
		pTile->m_isValidBuffer = FALSE;

	return (QGlobe_TILE*)pTile;
}

// create name tile from response
QGlobe_TILE * qglobe_ParsingNameTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	QGlobe_TILE *pTile = NULL;
	ST_GEODATA_COMMON *pData = (ST_GEODATA_COMMON*)pResp->data;

	ST_GEODATA_NAMELIST *pNameData = (ST_GEODATA_NAMELIST*)pData->buffer;	
	int nBufSize = pData->bytes;
	int allSize;

	if(nBufSize <= 0)
		return NULL;

	if((unsigned int)nBufSize > g_pGDMDataMgr->GetMaxTileSize(E_QGlobe_MAINDATA_TYPE_NAME)) // invalide buffer
		return NULL;

	if(pNameData->count <= 0)
		return NULL;

	allSize = sizeof(QGlobe_TILE) + nBufSize;

	pTile = (QGlobe_TILE*)gdmMemMalloc(allSize);

	if(pTile == NULL) // fail malloc memory
		return NULL;

	memset(pTile, 0, allSize);

	if (pData->bytes > 0 && pNameData->count > 0)
	{
		memcpy(pTile->m_pBuffer, pNameData, nBufSize);
		pTile->m_isValidBuffer = TRUE;
	}
	else
	{
		pTile->m_isValidBuffer = FALSE;
	}

	return pTile;
}

