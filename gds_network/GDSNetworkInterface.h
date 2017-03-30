#ifndef __GDSNETWORKINTERFACE_H__
#define __GDSNETWORKINTERFACE_H__

#include "../gds_common/GDSDefine.h"
#include "../gds_common/GDSType.h"

typedef bool (*RECEIVEDREQUEST_FUNCPTR)(INOUT unsigned char* a_pBuff);

extern RECEIVEDREQUEST_FUNCPTR	g_pReceivedCallbackFunc;

bool InitializeGDSNetwork(unsigned short a_PortNo, int a_MaxConnNum, RECEIVEDREQUEST_FUNCPTR a_pFuncPtr);
void ReleaseGDSNetwork();

#endif // __GDSNETWORKINTERFACE_H__