#include "GDM_Tile.h"
#include "GDM_Jpeg.h"
#include "gdem_protocol.h"
#include "GDM_Define.h"
#include "GDM_Mem.h"
#include "GDM_DataMgr.h"
#include "LzmaLib.h"

GDM_TILE * gdm_ParsingDemTileFromResp(ST_RESPONSE_GEODATA *pResp);	// parsing dem tile
GDM_TILE * gdm_ParsingImgTileFromResp(ST_RESPONSE_GEODATA *pResp);	// parsing texture tile
GDM_TILE * gdm_ParsingShapeTileFromResp(ST_RESPONSE_GEODATA *pResp);// parsing shape tile
GDM_TILE * gdm_ParsingNameTileFromResp(ST_RESPONSE_GEODATA *pResp);	// parsing name tile
GDM_TILE * gdm_Parsing3DMeshTileFromResp(ST_RESPONSE_GEODATA *pResp);// parsing 3dmesh tile
GDM_TILE * gdm_ParsingRasterTileFromResp(ST_RESPONSE_GEODATA *pResp);// parsing 3dmesh tile

GDM_TILE * gdm_ParsingTileFromResponse(ST_RESPONSE_GEODATA *pResp, E_GDM_SUBDATA_TYPE eDataType)
{
	E_GDM_MAINDATA_TYPE mainType = gdm_GetMainType(eDataType);
	switch(mainType)
	{
	case E_GDM_MAINDATA_TYPE_DEM:
		return gdm_ParsingDemTileFromResp(pResp);
	case E_GDM_MAINDATA_TYPE_IMG:
		return gdm_ParsingImgTileFromResp(pResp);
	case E_GDM_MAINDATA_TYPE_SHAPE:
		return gdm_ParsingShapeTileFromResp(pResp);
	case E_GDM_MAINDATA_TYPE_NAME:
		return gdm_ParsingNameTileFromResp(pResp);
	case E_GDM_MAINDATA_TYPE_3D:
		return gdm_Parsing3DMeshTileFromResp(pResp);
	case E_GDM_MAINDATA_TYPE_RASTER:
		return gdm_ParsingRasterTileFromResp(pResp);
	}
	return NULL;
}

// decompress buffer
void gdm_DeCompressBuffer(int inBufLen, void* iBuf, int outBufLen, void *oBuf, E_COMPRESS_TYPE CompressType)
{
	size_t	lBufferSizeCpr = 0;
	switch(CompressType)
	{
	case E_COMPRESS_JPG:
		gdm_JPEGDecompressTile(iBuf, oBuf, inBufLen);
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
GDM_TILE * gdm_ParsingDemTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	GDM_DEM_TILE *pTile = NULL;
	ST_GEODATA_DEM *pData = (ST_GEODATA_DEM*)pResp->data;
	int allSize;

	if(pData->bytes == 0)
		return NULL;

	if(pData->bytes > g_pGDMDataMgr->GetMaxTileSize(E_GDM_MAINDATA_TYPE_DEM)) // invalide buffer
		return NULL;

	allSize = sizeof(GDM_DEM_TILE);

	pTile = (GDM_DEM_TILE*)gdmMemMalloc(allSize);

	if(pTile == NULL) // fail malloc memory
		return NULL;

	memset(pTile, 0, allSize);

	if(pData->bytes > 0)
	{
		gdm_DeCompressBuffer(pData->bytes, (void*)pData->buffer, allSize, (void*)pTile->m_pData, pData->compress_type);
		pTile->m_isValidBuffer = TRUE;
	}
	else
		pTile->m_isValidBuffer = FALSE;

	return (GDM_TILE*)pTile;
}

// create dem tile from response
GDM_TILE * gdm_Parsing3DMeshTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	GDM_TILE      *pTile = NULL;
	ST_GEODATA_3DMESH *pData = (ST_GEODATA_3DMESH*)pResp->data;
	int allSize;

	if(pData->bytes == 0)
		return NULL;

	if(pData->bytes > g_pGDMDataMgr->GetMaxTileSize(E_GDM_MAINDATA_TYPE_3D)) // invalide buffer
		return NULL;

	allSize = sizeof(GDM_TILE)+ pData->bytes;

	pTile = (GDM_TILE*)gdmMemMalloc(allSize);

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

	return (GDM_TILE*)pTile;
}

GDM_TILE * gdm_ParsingRasterTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	GDM_TILE      *pTile = NULL;
	ST_GEODATA_3DMESH *pData = (ST_GEODATA_3DMESH*)pResp->data;
	int allSize;

	if(pData->bytes == 0)
		return NULL;

	if(pData->bytes > g_pGDMDataMgr->GetMaxTileSize(E_GDM_MAINDATA_TYPE_RASTER)) // invalide buffer
		return NULL;

	allSize = sizeof(GDM_TILE)+ pData->bytes;

	pTile = (GDM_TILE*)gdmMemMalloc(allSize);

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

	return (GDM_TILE*)pTile;
}


// create dem tile from response
GDM_TILE * gdm_ParsingImgTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	GDM_TEXTURE_TILE *pTile = NULL;
	ST_GEODATA_TEXTURE *pData = (ST_GEODATA_TEXTURE*)pResp->data;
	int allSize;

	if(pData->bytes == 0)
		return NULL;

	if(pData->bytes > g_pGDMDataMgr->GetMaxTileSize(E_GDM_MAINDATA_TYPE_IMG)) // invalide buffer
		return NULL;

	allSize = sizeof(GDM_TEXTURE_TILE) + pData->bytes;

	pTile = (GDM_TEXTURE_TILE*)gdmMemMalloc(allSize);

	if(pTile == NULL)
		return NULL; // fail malloc memory

	memset(pTile, 0, allSize);

	if(pData->bytes > 0)
	{
		// gdm_DeCompressBuffer(pData->bytes, (void*)pData->buffer, allSize, (void*)pTile->m_pBuffer, pData->compres_type);
		memcpy(pTile->m_pBuffer, pData->buffer, pData->bytes);
		pTile->m_nBufSize = pData->bytes;
		pTile->m_isValidBuffer = TRUE;
	}
	else
		pTile->m_isValidBuffer = FALSE;

	return (GDM_TILE*)pTile;
}

// create shape tile from response
GDM_TILE * gdm_ParsingShapeTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	GDM_SHAPE_TILE *pTile = NULL;
	ST_GEODATA_SHAPE *pData = (ST_GEODATA_SHAPE*)pResp->data;

	GDM_SHAPE_DATA *pShapeData = (GDM_SHAPE_DATA*)pData->buffer;	
	int nBufSize = pData->bytes - sizeof(GDM_SHAPE_DATA) + sizeof(GDM_POLYGON);
	int allSize;
	
	if(nBufSize <= 0)
		return NULL;
	
	if((unsigned int)nBufSize > g_pGDMDataMgr->GetMaxTileSize(E_GDM_MAINDATA_TYPE_SHAPE)) // invalide buffer
		return NULL;
		
	allSize = sizeof(GDM_SHAPE_TILE) + nBufSize;

	pTile = (GDM_SHAPE_TILE*)gdmMemMalloc(allSize);

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

	return (GDM_TILE*)pTile;
}

// create name tile from response
GDM_TILE * gdm_ParsingNameTileFromResp(ST_RESPONSE_GEODATA *pResp)
{
	GDM_TILE *pTile = NULL;
	ST_GEODATA_COMMON *pData = (ST_GEODATA_COMMON*)pResp->data;

	ST_GEODATA_NAMELIST *pNameData = (ST_GEODATA_NAMELIST*)pData->buffer;	
	int nBufSize = pData->bytes;
	int allSize;

	if(nBufSize <= 0)
		return NULL;

	if((unsigned int)nBufSize > g_pGDMDataMgr->GetMaxTileSize(E_GDM_MAINDATA_TYPE_NAME)) // invalide buffer
		return NULL;

	if(pNameData->count <= 0)
		return NULL;

	allSize = sizeof(GDM_TILE) + nBufSize;

	pTile = (GDM_TILE*)gdmMemMalloc(allSize);

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

