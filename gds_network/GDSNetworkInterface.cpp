// GDSNetworkInterface.cpp

#include "GDSNetworkInterface.h"
#include "GDSTcpServer.h"
#include "../gds_common/GDSLog.h"


GDSTcpServer			*g_tcpServer = NULL;
RECEIVEDREQUEST_FUNCPTR	g_pReceivedCallbackFunc = NULL;

bool InitializeGDSNetwork(unsigned short a_PortNo, int a_MaxConnNum, RECEIVEDREQUEST_FUNCPTR a_pFuncPtr)
{
	if (!g_tcpServer)
	{
		g_tcpServer = new GDSTcpServer();
		if (g_tcpServer == NULL)
		{
			GDSLogOutput(E_LOG_ERROR, "new GDSTcpServer failed at InitializeGDSNetwork.");
			return false;
		}

		g_tcpServer->SetPortNo(a_PortNo);
		g_tcpServer->SetMaxConnectNum(a_MaxConnNum);

		g_pReceivedCallbackFunc = a_pFuncPtr;

		if (!g_tcpServer->start())
		{
			GDSLogOutput(E_LOG_ERROR, g_tcpServer->errorString().toAscii().data());
			return false;
		}
	}

	return true;
}

void ReleaseGDSNetwork()
{
	if (g_tcpServer)
	{
		delete g_tcpServer;
		g_tcpServer = NULL;

		g_pReceivedCallbackFunc = NULL;
	}
}
