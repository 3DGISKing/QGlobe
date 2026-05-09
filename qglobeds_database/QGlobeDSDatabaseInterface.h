#ifndef __GDSDATABASEINTERFACE_H__
#define __GDSDATABASEINTERFACE_H__

#include "../qglobeds_common/QGlobeDSDefine.h"
#include "../qglobe_common/qglobe_protocol.h"
#include "../qglobeds_common/QGlobeDSCommon.h"

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



class QGlobeDSGeoNameDBManager;

extern QGlobeDSGeoNameDBManager* g_DBManager;

#endif // __GDSDATABASEINTERFACE_H__