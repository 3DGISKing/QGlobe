#ifndef __GDEM_VERSION_H
#define __GDEM_VERSION_H

#include "gdem_protocol.h"


// application main version
#define GDEM_VERSION_MAJOR	1 // If program architecture was modified, this version changes.
#define GDEM_VERSION_MINOR	3 // If database type was added or protocol was modified, this version changes.
#define GDEM_VERSION_MICRO	2 // If some database was modified, this version changes.
#define GDEM_VERSION_EXTRA	1 // If Bug was fixed, this version changes.

// subject version by geodata type
#define GDEM_SUB_VERSION		"3.0.0.0.4.0"	// 0 - search name, 1 - TEXTURE DB, 2 - DEM, 3 - SHAPE, 4 - NAME, 5 - 3D 

// subject version example
//#define GDEM_SUB_VERSION		"0.0.0.0.0"		// 0 - search name, 1 - TEXTURE DB, 2 - DEM, 3 - SHAPE, 4 - NAME
//#define GDEM_SUB_VERSION		"0.0.0.0.0.0"	// Added shape (5 - 3D)
//#define GDEM_SUB_VERSION		"0.0.0.0.1.0"	// Modified NAME structure.

// main version check type
typedef enum E_VERSION_MISMATCH_TYPE
{
	E_VERSION_EQUAL = 0,
	E_VERSION_MISMATCH_EXTRA,
	E_VERSION_MISMATCH_MICRO,
	E_VERSION_MISMATCH_MINOR,
	E_VERSION_MISMATCH_MAJOR,
};



// use at server
int GetMainVersion();
int GetSubVersionByGeoDataType(E_GDM_MAINDATA_TYPE a_eGeoDataType);
int GetSubVersionBySearchName();


// use at client
E_VERSION_MISMATCH_TYPE CheckMainVersion(int a_nVersion);
bool IsEqualSubVersionByGeoDataType(E_GDM_MAINDATA_TYPE a_eGeoDataType, int a_nSubVersion);
bool IsEqualSubVersionBySearchName(int a_nSubVersion);



#endif // __GDEM_VERSION_H
