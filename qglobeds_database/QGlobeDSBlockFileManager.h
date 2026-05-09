#ifndef __GDSBLOCKFILEMANAGER_H__
#define __GDSBLOCKFILEMANAGER_H__

#include "../qglobe_common/qglobe_protocol.h"
#include "../qglobeds_common/QGlobeDSDefine.h"
#include "../qglobeds_common/QGlobeDSType.h"

#include <QString>

typedef struct tagBLOCKFILE_HEADER_TEMP
{
	unsigned int level;
	unsigned int count;
	unsigned int offset_list[1];
} ST_BLOCKFILE_HEADER_TEMP;

typedef struct tagTILE_RECORD
{
	unsigned int offset;
	unsigned int size;
} ST_TILE_RECORD;

typedef struct tagBLOCKFILE_HEADER
{
	unsigned int level;
	unsigned int count;
	ST_TILE_RECORD offset_list[1];
} ST_BLOCKFILE_HEADER;

bool GetGeoDataFromBlock(IN E_QGlobe_MAINDATA_TYPE a_eMainDataType, 
						 IN E_QGlobe_SUBDATA_TYPE a_eSubDataType, 
						 IN unsigned int a_nLevel,
						 IN unsigned int a_nColumn, 
						 IN unsigned int a_nRow,
						 OUT ST_GEODATA_COMMON* a_pResponse, 
						 OUT unsigned int* a_pBytes, 
						 OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

bool GetBlockFileName(IN E_QGlobe_MAINDATA_TYPE a_eMainDataType,
					  IN E_QGlobe_SUBDATA_TYPE a_eSubDataType,
					  IN unsigned int a_nLevel,
					  IN unsigned int a_nColumn,
					  IN unsigned int a_nRow,
					  OUT QString& a_strFileName);

bool CalcTileOffset(IN QGlobeDSFILEHANDLE a_mapHandle,
					IN unsigned int a_nLevel,
					IN unsigned int a_nColumn,
					IN unsigned int a_nRow,
					OUT unsigned int* a_pOffset,
					 OUT unsigned int* a_pSize,
					OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

unsigned int GetTileRowCountInBlockFile(unsigned int a_nLevel);
unsigned int GetBlockFileHeaderSize(unsigned int a_nLevel);


#endif // __GDSBLOCKFILEMANAGER_H__