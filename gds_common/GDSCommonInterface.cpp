#include "GDSCommonInterface.h"
#include "GDSLog.h"
#include "GDSConfigMgr.h"

#ifndef STANDARD_ALONE
#include "../gdem_common/gdem_distributeconfig.h"
#endif

//====================================================================================
// Function name	: InitializeGDSCommon
// Creator			: kangcj
// Description		: initialize common component
// Return type		: bool - result(true : success, false : fail) 
//====================================================================================
bool InitializeGDSCommon(QString& a_strCurPath)
{
	if (!g_ServerConfig.InitializeConfig(a_strCurPath))
		return false;

#ifndef STANDARD_ALONE
	g_DistributeConfig.ReadFromRegistry();
	g_DistributeConfig.WriteToRegistry();
#endif

	InitializeLog(g_ServerConfig.GetLogOutputType(), g_ServerConfig.GetLogOutputLevel());

	return true;
}

//====================================================================================
// Function name	: ReleaseGDSCommon
// Creator			: kangcj
// Description		: release common component
// Return type		: void 
//====================================================================================
void ReleaseGDSCommon()
{
	FinalizeLog();
}

