
#include "../gdem_common/gdem_protocol.h"
#include "../gds_common/GDSDefine.h"
#include "../gds_common/GDSType.h"


#include <QString>

//¹¦µá¿ÍÃùËÎËË Ê­³£µ¹ ±¶¸ë¼¬ËÍ ÃùËÎËæº· »ôµê¶¦ ÊÐ²÷³Þ
bool GetGeoDataFromFile(IN E_GDM_MAINDATA_TYPE a_eMainDataType, 
						IN E_GDM_SUBDATA_TYPE a_eSubDataType, 
						IN unsigned int a_nLevel,
						IN unsigned int a_nColumn, 
						IN unsigned int a_nRow,
						OUT ST_GEODATA_COMMON* a_pResponse, 
						OUT unsigned int* a_pBytes, 
						OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

bool GetFileName(IN E_GDM_MAINDATA_TYPE a_eMainDataType,
					  IN E_GDM_SUBDATA_TYPE a_eSubDataType,
					  IN unsigned int a_nLevel,
					  IN unsigned int a_nColumn,
					  IN unsigned int a_nRow,
					  OUT QString& a_strFileName);
