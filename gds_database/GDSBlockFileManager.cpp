#include "GDSBlockFileManager.h"
#include "GDSFileManager.h"
#include "../gds_common/GDSCommon.h"
#include "../gds_common/GDSConfigMgr.h"
#include "../gds_common/GDSLog.h"
#include "../gdem_libmain/GDM_DataMgr.h"
#include "../gdem_libmain/Layer.h"

#ifndef STANDARD_ALONE
#include "../gdem_common/gdem_distributeconfig.h"
#endif

#include <QtCore/QFile>

//====================================================================================
// Function name	: GetGeoDataFromBlock
// Creator			: kangcj
// Description		: get geodata from block file
// Return type		: bool - result(true : success, false : fail) 
// Argument1		: IN E_GDM_MAINDATA_TYPE a_eMainDataType - data type
// Argument2		: IN unsigned int a_nLevel - level
// Argument3		: IN unsigned int a_nColumn - x
// Argument4		: IN unsigned int a_nRow - y
// Argument5		: OUT ST_GEODATA_COMMON* a_pResponse - response data
// Argument6		: OUT unsigned int* a_pBytes - bytes of response
// Argument7		: OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode - error code
//====================================================================================

bool GetGeoDataFromBlock(IN E_GDM_MAINDATA_TYPE a_eMainDataType, 
						 IN E_GDM_SUBDATA_TYPE a_eSubDataType, 
						 IN unsigned int a_nLevel,
						 IN unsigned int a_nColumn, 
						 IN unsigned int a_nRow,
						 OUT ST_GEODATA_COMMON* a_pResponse, 
						 OUT unsigned int* a_pBytes, 
						 OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	QString tFileName;
	if (!GetBlockFileName(a_eMainDataType, a_eSubDataType, a_nLevel, a_nColumn, a_nRow, tFileName))
	{
		*a_pErrorCode = E_RESPONSE_ERROR_FILENAME;
		GDSLogOutput(E_LOG_ERROR, "Block file name is unknown at GetGeoDataFromBlock! [type : %d]", (int)a_eMainDataType);
		return false;
	}

	unsigned int tHeaderSize = GetBlockFileHeaderSize(a_nLevel);
	GDSFILEHANDLE tFileHandle = INVALID_GDSFILEHANDLE;
	bool bExistFile = false;

	if(!g_FileManager ) return false;
	if (!g_FileManager->OpenFileMap(tFileName, tHeaderSize, &tFileHandle, &bExistFile))
	{
		if (!bExistFile)
		{
			a_pResponse->bytes = 0;
			*a_pBytes = sizeof(ST_GEODATA_COMMON) - sizeof(unsigned char)*4;
			return true;
		}

		*a_pErrorCode = E_RESPONSE_ERROR_FILE_OPEN;
		GDSLogOutput(E_LOG_ERROR, "OpenFileMap failed at GetGeoDataFromBlock! [file : %s]", tFileName.toAscii().data());
		return false;
	}

	unsigned int tOffset = 0;
	unsigned int tSize = 0;
	if(!CalcTileOffset(tFileHandle, a_nLevel, a_nColumn, a_nRow, &tOffset, &tSize, a_pErrorCode))
	{
		g_FileManager->CloseFileMap(tFileHandle);
		*a_pErrorCode = E_RESPONSE_ERROR_TILE_NOFOUND;
		GDSLogOutput(E_LOG_ERROR, "Tile is not found at GetGeoDataFromBlock! [type : %d, level : %d, x : %d, y : %d]", (int)a_eMainDataType, a_nLevel, a_nColumn, a_nRow);
		return false;
	}

	if (tSize == 0)
	{
		g_FileManager->CloseFileMap(tFileHandle);
		a_pResponse->bytes = 0;
		*a_pBytes = sizeof(ST_GEODATA_COMMON) - sizeof(unsigned char)*4;
		return true;
	}

	if( !g_FileManager->ReadFileMap(tFileHandle, tOffset, tSize, a_pResponse->buffer) )
	{
		g_FileManager->CloseFileMap(tFileHandle);
		*a_pErrorCode = E_RESPONSE_ERROR_FILE_READ;
		GDSLogOutput(E_LOG_ERROR, "Tile read was failed at GetGeoDataFromBlock! [type : %d, level : %d, x : %d, y : %d]", (int)a_eMainDataType, a_nLevel, a_nColumn, a_nRow);
		return false;
	}

	g_FileManager->CloseFileMap(tFileHandle);

	a_pResponse->bytes = tSize;
	a_pResponse->compress_type = GetCompressType(a_eMainDataType);

	*a_pBytes = sizeof(ST_GEODATA_COMMON) - sizeof(unsigned char)*4 + tSize;

	return true;
}

//====================================================================================
// Function name	: GetBlockFileName
// Creator			: kangcj
// Description		: get block file name
// Return type		: bool - result(true : success, false : fail) 
// Argument1		: IN E_GDM_MAINDATA_TYPE a_eMainDataType - data type
// Argument2		: IN unsigned int a_nLevel - level
// Argument3		: IN unsigned int a_nColumn - x
// Argument4		: IN unsigned int a_nRow - y
// Argument5		: OUT QString& a_strFileName - data type
//====================================================================================

bool GetBlockFileName(IN E_GDM_MAINDATA_TYPE a_eMainDataType,
					  IN E_GDM_SUBDATA_TYPE a_eSubDataType,
					  IN unsigned int a_nLevel, 
					  IN unsigned int a_nColumn, 
					  IN unsigned int a_nRow, 
					  OUT QString& a_strFileName)
{
	unsigned int m = a_nColumn / TILE_ROW_COUNT_IN_BLOCK;
	unsigned int n = a_nRow / TILE_ROW_COUNT_IN_BLOCK;

	Layer* layer=g_pGDMDataMgr->m_LayerHash[a_eSubDataType];
	Q_ASSERT(layer!=NULL);

	QString datapath=layer->GetDataPath();

	switch (a_eMainDataType)
	{
	case E_GDM_MAINDATA_TYPE_DEM:
#ifndef STANDARD_ALONE
		if (g_DistributeConfig.IsDistributable() 
			&& g_DistributeConfig.GetDistributableType() == E_DISTRIBUTE_TYPE_DIRECTORY)
		{
			ST_DISTRIBUTE_FOLDER tPath;
			if (!g_DistributeConfig.GetFolderInfo(a_eMainDataType, a_nLevel, a_nColumn, a_nRow, tPath))
				break;


			a_strFileName = QString::fromUtf16(tPath.path) + a_strFileName.sprintf(DEM_BLOCKFILE_PATH_FMT, a_nLevel, m, n, a_nLevel);
		}
		else
#endif
		a_strFileName = g_ServerConfig.GetFileDBPath() + "/"+datapath+a_strFileName.sprintf(DEM_BLOCKFILE_PATH_FMT, a_nLevel, m, n, a_nLevel);
		return true;
	case E_GDM_MAINDATA_TYPE_IMG:
#ifndef STANDARD_ALONE
		if (g_DistributeConfig.IsDistributable() 
			&& g_DistributeConfig.GetDistributableType() == E_DISTRIBUTE_TYPE_DIRECTORY)
		{
			ST_DISTRIBUTE_FOLDER tPath;
			if (!g_DistributeConfig.GetFolderInfo(a_eMainDataType, a_nLevel, a_nColumn, a_nRow, tPath))
				break;
			a_strFileName = QString::fromUtf16(tPath.path) + a_strFileName.sprintf(TEXTURE_BLOCKFILE_PATH_FMT, a_nLevel, m, n, a_nLevel);
		}
		else
#endif
			a_strFileName = g_ServerConfig.GetFileDBPath() +  "/"+datapath+a_strFileName.sprintf(TEXTURE_BLOCKFILE_PATH_FMT, a_nLevel, m, n, a_nLevel);
		return true;
	case E_GDM_MAINDATA_TYPE_SHAPE:
		a_strFileName = g_ServerConfig.GetFileDBPath() +  "/"+datapath+a_strFileName.sprintf(SHAPE_COUNTRY_BLOCKFILE_PATH_FMT, a_nLevel, m, n, a_nLevel);
		return true;
	default:
		break;
	}

	return false;
}


//====================================================================================
// Function name	: CalcTileOffset
// Creator			: kangcj
// Description		: get tile offset in block file
// Return type		: bool - result(true : success, false : fail) 
//====================================================================================

bool CalcTileOffset(IN GDSFILEHANDLE a_mapHandle,
					IN unsigned int a_nLevel,
					IN unsigned int a_nColumn,
					IN unsigned int a_nRow,
					OUT unsigned int* a_pOffset,
					OUT unsigned int* a_pSize,
					OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	unsigned char* tHeaderPtr = NULL;
	unsigned int tHeaderSize = g_FileManager->GetHeaderPointer(a_mapHandle, &tHeaderPtr);
	if (tHeaderSize == 0)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_BLOCKFILE_HEADERSIZE_ZERO;
		GDSLogOutput(E_LOG_ERROR, "Size of header of block file is zero at CalcTileOffset! [file : %s]", g_FileManager->GetFileName(a_mapHandle).toAscii().data());
		return false;
	}


	unsigned int tRowCount = GetTileRowCountInBlockFile(a_nLevel);
	int x = a_nColumn % tRowCount;
	int y = a_nRow % tRowCount;

	int tIndex = tRowCount*y + x;

	if (a_nLevel != ((ST_BLOCKFILE_HEADER*)tHeaderPtr)->level)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_BLOCKFILE_LEVEL_DIFFERENT;
		GDSLogOutput(E_LOG_ERROR, "Level of block file is different with request level at CalcTileOffset! [request : %d, file : %d]", a_nLevel, ((ST_BLOCKFILE_HEADER*)tHeaderPtr)->level);
		return false;
	}

	if (tIndex >= ((ST_BLOCKFILE_HEADER*)tHeaderPtr)->count)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_BLOCKFILE_TILE_INDEX;
		GDSLogOutput(E_LOG_ERROR, "Index of Tile file is bigger at CalcTileOffset! [index : %d, count : %d]", tIndex, ((ST_BLOCKFILE_HEADER*)tHeaderPtr)->count);
		return false;
	}

	*a_pOffset = ((ST_BLOCKFILE_HEADER*)tHeaderPtr)->offset_list[tIndex].offset;
	*a_pSize = ((ST_BLOCKFILE_HEADER*)tHeaderPtr)->offset_list[tIndex].size;

	if (!g_FileManager->CheckFileOffsetAndSize(a_mapHandle, *a_pOffset, *a_pSize))
	{
		*a_pErrorCode = E_RESPONSE_ERROR_BLOCKFILE_HEADERSIZE_ZERO;
		GDSLogOutput(E_LOG_ERROR, "File size is smaller than offset invalid at CalcTileOffset! [file : %s]", g_FileManager->GetFileName(a_mapHandle).toAscii().data());
		return false;
	}

	return true;
}


unsigned int GetTileRowCountInBlockFile(unsigned int a_nLevel)
{
	return (a_nLevel >= 7) ? TILE_ROW_COUNT_IN_BLOCK : (1<<a_nLevel);
}

unsigned int GetBlockFileHeaderSize(unsigned int a_nLevel)
{
	return (4 + 4 + 8*(GetTileRowCountInBlockFile(a_nLevel)*GetTileRowCountInBlockFile(a_nLevel)));
}