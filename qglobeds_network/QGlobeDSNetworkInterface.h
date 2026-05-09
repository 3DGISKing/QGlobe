#ifndef __GDSNETWORKINTERFACE_H__
#define __GDSNETWORKINTERFACE_H__

#include "../qglobeds_common/QGlobeDSDefine.h"
#include "../qglobeds_common/QGlobeDSType.h"

typedef bool (*RECEIVEDREQUEST_FUNCPTR)(INOUT unsigned char* a_pBuff);

extern RECEIVEDREQUEST_FUNCPTR	g_pReceivedCallbackFunc;

bool InitializeGDSNetwork(unsigned short a_PortNo, int a_MaxConnNum, RECEIVEDREQUEST_FUNCPTR a_pFuncPtr);
void ReleaseGDSNetwork();

#endif // __GDSNETWORKINTERFACE_H__