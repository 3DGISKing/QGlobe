#ifndef __GDSDATABASEINTERFACE_H__
#define __GDSDATABASEINTERFACE_H__

#include "../gds_common/GDSDefine.h"
#include "../gdem_common/gdem_protocol.h"
#include "../gds_common/GDSCommon.h"

bool InitializeGDSDatabase();
void ReleaseGDSDatabase();

bool GetGeoData(IN ST_REQUEST_GEODATA* a_pRequest, 
				OUT ST_RESPONSE_GEODATA* a_pResponse,
				OUT unsigned int* a_pBytes, 
				OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

bool GetGeoNameList(IN ST_REQUEST_GEONAMESEARCH* a_pRequest, 
					OUT ST_RESPONSE_GEONAMESEARCH* a_pResponse, 
					OUT unsigned int* a_pBytes, 
					OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

bool RunDBCommand(IN ST_REQUEST_DBCOMMAND* a_pRequest, 
				OUT ST_RESPONSE_DBCOMMAND* a_pResponse, 
				OUT unsigned int* a_pBytes, 
				OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);



class GDSGeoNameDBManager;

extern GDSGeoNameDBManager* g_DBManager;

#endif // __GDSDATABASEINTERFACE_H__