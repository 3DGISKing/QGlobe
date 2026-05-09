#include "../qglobe_common/qglobe_config.h"

#include "QGlobeDSCoreInterface.h"
#include "QGlobeDSRequestProcess.h"
#include "../qglobeds_common/QGlobeDSLog.h"
#include "../qglobeds_common/QGlobeDSConfigMgr.h"
#include "../qglobeds_common/QGlobeDSCommonInterface.h"
#include "../qglobeds_database/QGlobeDSDatabaseInterface.h"

#ifndef STANDARD_ALONE
#include "../qglobeds_network/QGlobeDSNetworkInterface.h"
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

	QGlobeDSLogOutput(E_LOG_TRACK, "Initialize server core.");

	if (!InitializeGDSDatabase())
	{
		QGlobeDSLogOutput(E_LOG_ERROR, "InitializeGDSDatabase failed.");
		ReleaseGDSCommon();
		return false;
	}

#ifndef STANDARD_ALONE
	if (!InitializeGDSNetwork(g_ServerConfig.GetPortNo(), 
							g_ServerConfig.GetMaxSocketNum(), 
							QGlobeDSReceivedRequestCallback))
	{
#ifdef DEBUG
		QGlobeDSLogOutput(E_LOG_ERROR, "InitializeGDSNetwork failed.");
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
	QGlobeDSLogOutput(E_LOG_TRACK, "Release server core.");

#ifndef STANDARD_ALONE
	ReleaseGDSNetwork();
#endif
	ReleaseGDSDatabase();
	ReleaseGDSCommon();
}

