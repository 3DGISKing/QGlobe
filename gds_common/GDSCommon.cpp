#include "GDSCommon.h"
#include "../gdem_libmain/GDM_DataMgr.h"
#include "../gdem_libmain/Layer.h"

//====================================================================================
// Function name	: GetCompressType
// Creator			: kangcj
// Description		: get compress type in geoData
// Return type		: unsigned int - compress type
// Argument1		: IN E_GDM_SUBDATA_TYPE a_eDataType - subject data type
//====================================================================================

E_COMPRESS_TYPE GetCompressType(IN E_GDM_MAINDATA_TYPE a_eDataType)
{
	switch (a_eDataType)
	{
	case E_GDM_MAINDATA_TYPE_DEM:
		return E_COMPRESS_7ZIP;
	case E_GDM_MAINDATA_TYPE_IMG:
		return E_COMPRESS_JPG;
	default:
		break;
	}
	return E_COMPRESS_NONE;
}

//====================================================================================
// Function name	: IsInvalidLevel
// Creator			: kangcj
// Description		: check whether level is invalid
// Return type		: bool - result(true : invalid, false : valid) 
// Argument1		: IN E_GDM_SUBDATA_TYPE a_eDataType - subject data type
// Argument2		: IN unsigned int a_nLevel - level
//====================================================================================

bool IsInvalidLevel(IN E_GDM_SUBDATA_TYPE a_eDataType, IN unsigned int a_nLevel)
{
	if (a_nLevel < MIN_LEVEL || a_nLevel > MAX_LEVEL)
		return false;

	Layer* layer=g_pGDMDataMgr->m_LayerHash[a_eDataType];

	Q_ASSERT(layer!=NULL);
	
	return (a_nLevel < layer->GetMinLevel()) || (a_nLevel > layer->GetMaxLevel());
}

