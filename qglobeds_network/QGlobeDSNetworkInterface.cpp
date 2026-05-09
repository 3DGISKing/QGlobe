// QGlobeDSNetworkInterface.cpp

#include "QGlobeDSNetworkInterface.h"
#include "QGlobeDSTcpServer.h"
#include "../qglobeds_common/QGlobeDSLog.h"


QGlobeDSTcpServer			*g_tcpServer = NULL;
RECEIVEDREQUEST_FUNCPTR	g_pReceivedCallbackFunc = NULL;

bool InitializeGDSNetwork(unsigned short a_PortNo, int a_MaxConnNum, RECEIVEDREQUEST_FUNCPTR a_pFuncPtr)
{
	if (!g_tcpServer)
	{
		g_tcpServer = new QGlobeDSTcpServer();
		if (g_tcpServer == NULL)
		{
			QGlobeDSLogOutput(E_LOG_ERROR, "new QGlobeDSTcpServer failed at InitializeGDSNetwork.");
			return false;
		}

		g_tcpServer->SetPortNo(a_PortNo);
		g_tcpServer->SetMaxConnectNum(a_MaxConnNum);

		g_pReceivedCallbackFunc = a_pFuncPtr;

		if (!g_tcpServer->start())
		{
			QGlobeDSLogOutput(E_LOG_ERROR, g_tcpServer->errorString().toUtf8().data());
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
