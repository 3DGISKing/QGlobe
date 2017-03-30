#include "GDSDatabaseInterface.h"

#include "../gds_common/GDSLog.h"
#include "../gdem_common/gdem_version.h"

#include "GDSBlockFileManager.h"
#include "GDSFileManager.h"
#include "GDSGeoNameDBManager.h"
#include "GDSFile.h"
#include "../gdem_libmain/GDM_Interface.h"

GDSFileManager* g_FileManager = NULL;
GDSGeoNameDBManager* g_DBManager = NULL;

//====================================================================================
// Function name	: InitializeGDSDatabase
// Creator			: kangcj
// Description		: initialize database component
// Return type		: bool - result(true : success, false : fail) 
//====================================================================================
bool InitializeGDSDatabase()
{
	if (g_FileManager == NULL)
	{
		g_FileManager = new GDSFileManager(30, 30);
		if (g_FileManager == NULL)
		{
			GDSLogOutput(E_LOG_ERROR, "new GDSFileManager failed at InitializeGDSDatabase.");
			return false;
		}

		if (!g_FileManager->Initialize())
		{
			GDSLogOutput(E_LOG_ERROR, "FileManager Initialize failed at InitializeGDSDatabase.");

			delete g_FileManager;
			g_FileManager = NULL;
			return false;
		}
	}

	if (g_DBManager == NULL)
	{
		g_DBManager = new GDSGeoNameDBManager();
		if (g_DBManager == NULL)
		{
			GDSLogOutput(E_LOG_ERROR, "new GDSGeoNameDBManager failed at InitializeGDSDatabase.");
			return false;
		}

		if (!g_DBManager->Initialize())
		{
			GDSLogOutput(E_LOG_ERROR, "Connect mysql failed at InitializeGDSDatabase.");

			delete g_DBManager;
			g_DBManager = NULL;
			return false;
		}
	}

	return true;
}

//====================================================================================
// Function name	: ReleaseGDSDatabase
// Creator			: kangcj
// Description		: release database component
// Return type		: void
//====================================================================================
void ReleaseGDSDatabase()
{
	if (g_DBManager != NULL)
	{
		g_DBManager->Release();
		delete g_DBManager;
		g_DBManager = NULL;
	}
	if (g_FileManager != NULL)
	{
		delete g_FileManager;
		g_FileManager = NULL;
	}
}

//====================================================================================
// Function name	: GetGeoData
// Creator			: kangcj
// Description		: get geodata(DEM, Texture, Shape, 
// Return type		: bool - result(true : success, false : fail) 
// Argument1		: IN ST_REQUEST_GEODATA* a_pRequest
// Argument2		: OUT ST_RESPONSE_GEODATA* a_pResponse
// Argument3		: OUT unsigned int* a_pBytes - bytes of buffer
// Argument4		: OUT unsigned char* a_nErrorCode - error code
//====================================================================================
bool GetGeoData(IN ST_REQUEST_GEODATA* a_pRequest, 
				OUT ST_RESPONSE_GEODATA* a_pResponse,
				OUT unsigned int* a_pBytes, 
				OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	unsigned int nLevel = a_pRequest->level;
	unsigned int nColumn = a_pRequest->column;
	unsigned int nRow = a_pRequest->row;
	E_GDM_MAINDATA_TYPE nMainDataType = a_pRequest->main_type;
	E_GDM_SUBDATA_TYPE nSubDataType = a_pRequest->sub_type;

	if (nMainDataType == E_GDM_MAINDATA_TYPE_DEM
		|| nMainDataType == E_GDM_MAINDATA_TYPE_IMG
		|| nMainDataType == E_GDM_MAINDATA_TYPE_SHAPE)
	{
		if (!GetGeoDataFromBlock(nMainDataType,
			nSubDataType,
			nLevel, 
			nColumn, 
			nRow, 
			(ST_GEODATA_COMMON*)a_pResponse->data, 
			a_pBytes, 
			a_pErrorCode))
		{
			*a_pBytes = sizeof(ST_RESPONSE_GEODATA) - sizeof(unsigned char)*4;
			GDSLogOutput(E_LOG_ERROR, "GetGeoDataFromBlock failed at GetGeoData. [type : %d, level : %d, x : %d, y : %d]", nMainDataType, nLevel, nColumn, nRow);
			return false;
		}
	}
	else  if(nMainDataType==E_GDM_MAINDATA_TYPE_3D || nMainDataType==E_GDM_MAINDATA_TYPE_RASTER)
	{
			if (!GetGeoDataFromFile(nMainDataType,
			nSubDataType,
			nLevel, 
			nColumn, 
			nRow, 
			(ST_GEODATA_COMMON*)a_pResponse->data, 
			a_pBytes, 
			a_pErrorCode))
		{
			*a_pBytes = sizeof(ST_RESPONSE_GEODATA) - sizeof(unsigned char)*4;
			GDSLogOutput(E_LOG_ERROR, "GetGeoDataFromBlock failed at GetGeoData. [type : %d, level : %d, x : %d, y : %d]", nMainDataType, nLevel, nColumn, nRow);
			return false;
		}
	}
	else if (nMainDataType == E_GDM_MAINDATA_TYPE_NAME)
	{
		if (g_DBManager == NULL)
		{
			*a_pBytes = sizeof(ST_RESPONSE_GEODATA) - sizeof(unsigned char)*4;
			*a_pErrorCode = E_RESPONSE_ERROR_NULLDBMANAGER;
			GDSLogOutput(E_LOG_ERROR, "DBManager is null at GetGeoData.");
			return false;
		}

		if (!g_DBManager->GetGeoNameListInTile(nSubDataType,
											nLevel,
											nColumn, 
											nRow, 
											(ST_GEODATA_COMMON*)a_pResponse->data, 
											a_pBytes, 
											a_pErrorCode))
		{
			*a_pBytes = sizeof(ST_RESPONSE_GEODATA) - sizeof(unsigned char)*4;

			GDSLogOutput(E_LOG_ERROR, "DBManager::GetGeoNameList failed at GetGeoData. [level : %d, x : %d, y : %d]", nLevel, nColumn, nRow);
			return false;
		}
	}
	else
	{
		*a_pBytes = sizeof(ST_RESPONSE_GEODATA) - sizeof(unsigned char)*4;
		*a_pErrorCode = E_RESPONSE_ERROR_UNKNOWNGEODATATYPE;
		GDSLogOutput(E_LOG_ERROR, "GeoData type is unknown at GetGeoData. [type : %d]", nMainDataType);
		return false;
	}

	*a_pBytes += sizeof(ST_RESPONSE_GEODATA) - sizeof(unsigned char)*4;
	return true;
}

//====================================================================================
// Function name	: GetGeoNameList
// Creator			: kangcj
// Description		: process to search geoname
// Return type		: bool - result(true : success, false : fail) 
// Argument1		: IN ST_REQUEST_GEONAMESEARCH* a_pRequest
// Argument2		: OUT ST_RESPONSE_GEONAMESEARCH* a_pResponse
// Argument3		: OUT unsigned int* a_pBytes
// Argument3		: OUT unsigned char* a_nErrorCode
//====================================================================================
bool GetGeoNameList(IN ST_REQUEST_GEONAMESEARCH* a_pRequest, 
					OUT ST_RESPONSE_GEONAMESEARCH* a_pResponse, 
					OUT unsigned int* a_pBytes, 
					OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	if (g_DBManager == NULL)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_NULLDBMANAGER;

		GDSLogOutput(E_LOG_ERROR, "DBManager is null at GetGeoNameList.");
		return false;
	}

	return g_DBManager->GetGeoNameRecordData(a_pRequest, a_pResponse, a_pBytes, a_pErrorCode);
}

//====================================================================================
// Function name	: RunDBCommand
// Creator			: kangcj
// Description		: process command to geoname database
// Return type		: bool - result(true : success, false : fail) 
// Argument1		: IN ST_REQUEST_DBCOMMAND* a_pRequest
// Argument2		: OUT ST_RESPONSE_GEONAMELIST* a_pResponse
// Argument3		: OUT unsigned int* a_pBytes
// Argument3		: OUT unsigned char* a_nErrorCode
//====================================================================================
bool RunDBCommand(IN ST_REQUEST_DBCOMMAND* a_pRequest, 
					OUT ST_RESPONSE_DBCOMMAND* a_pResponse, 
					OUT unsigned int* a_pBytes, 
					OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	if (g_DBManager == NULL)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_NULLDBMANAGER;

		GDSLogOutput(E_LOG_ERROR, "DBManager is null at GetGeoNameList.");
		return false;
	}

	bool bSuccess;

	E_DBCOMMAND_TYPE nCommandType = a_pRequest->command_type;
	ST_RESPONSE_DBCOMMAND* pResponse = (ST_RESPONSE_DBCOMMAND*)a_pRequest;

	if (nCommandType == E_DBCOMMAND_ADDNAME)
	{
		ST_ADDNAME_DATA* pCommand = (ST_ADDNAME_DATA*)a_pRequest->data;

		bSuccess = g_DBManager->AddGeoNameData(pCommand, a_pResponse, a_pErrorCode);
	}
	else if (nCommandType == E_DBCOMMAND_UPDATENAME)
	{
		ST_UPDATENAME_DATA* pCommand = (ST_UPDATENAME_DATA*)a_pRequest->data;

		bSuccess = g_DBManager->UpdateGeoNameData(pCommand, a_pResponse, a_pErrorCode);
		a_pResponse->success = bSuccess;
	}
	else if (nCommandType == E_DBCOMMAND_DELETENAME)
	{
		ST_DELETENAME_DATA* pCommand = (ST_DELETENAME_DATA*)a_pRequest->data;

		bSuccess = g_DBManager->DeleteGeoNameData(pCommand, a_pResponse, a_pErrorCode);
		a_pResponse->success = bSuccess;
	}

	a_pResponse->version = GetSubVersionBySearchName();
	a_pResponse->command_type = nCommandType;

	*a_pBytes = sizeof(ST_RESPONSE_DBCOMMAND);

	return bSuccess;
}
