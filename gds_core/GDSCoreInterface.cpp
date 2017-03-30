#include "../gdem_common/gdem_config.h"

#include "GDSCoreInterface.h"
#include "GDSRequestProcess.h"
#include "../gds_common/GDSLog.h"
#include "../gds_common/GDSConfigMgr.h"
#include "../gds_common/GDSCommonInterface.h"
#include "../gds_database/GDSDatabaseInterface.h"

#ifndef STANDARD_ALONE
#include "../gds_network/GDSNetworkInterface.h"
#endif

//====================================================================================
// Function name	: InitializeGDSCore
// Creator			: kangcj
// Description		: initialize all server component
// Return type		: bool - result(true : success, false : fail)  
//====================================================================================
bool InitializeGDSCore(QString& a_strCurPath)
{
	if (!InitializeGDSCommon(a_strCurPath))
	{
		return false;
	}

	GDSLogOutput(E_LOG_TRACK, "Initialize server core.");

	if (!InitializeGDSDatabase())
	{
		GDSLogOutput(E_LOG_ERROR, "InitializeGDSDatabase failed.");
		ReleaseGDSCommon();
		return false;
	}

#ifndef STANDARD_ALONE
	if (!InitializeGDSNetwork(g_ServerConfig.GetPortNo(), 
							g_ServerConfig.GetMaxSocketNum(), 
							GDSReceivedRequestCallback))
	{
#ifdef DEBUG
		GDSLogOutput(E_LOG_ERROR, "InitializeGDSNetwork failed.");
#endif
		ReleaseGDSDatabase();
		ReleaseGDSCommon();
		return false;
	}
#endif

	return true;
}

//====================================================================================
// Function name	: ReleaseGDSCore
// Creator			: kangcj
// Description		: release all server component
// Return type		: void 
//====================================================================================
void ReleaseGDSCore()
{
	GDSLogOutput(E_LOG_TRACK, "Release server core.");

#ifndef STANDARD_ALONE
	ReleaseGDSNetwork();
#endif
	ReleaseGDSDatabase();
	ReleaseGDSCommon();
}

