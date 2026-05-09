
#include "../qglobe_common/qglobe_protocol.h"
#include "../qglobeds_common/QGlobeDSDefine.h"
#include "../qglobeds_common/QGlobeDSType.h"


#include <QString>

//����������� ʭ���� ���뼬�� �����溷 ���궦 �в���
bool GetGeoDataFromFile(IN E_QGlobe_MAINDATA_TYPE a_eMainDataType, 
						IN E_QGlobe_SUBDATA_TYPE a_eSubDataType, 
						IN unsigned int a_nLevel,
						IN unsigned int a_nColumn, 
						IN unsigned int a_nRow,
						OUT ST_GEODATA_COMMON* a_pResponse, 
						OUT unsigned int* a_pBytes, 
						OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

bool GetFileName(IN E_QGlobe_MAINDATA_TYPE a_eMainDataType,
					  IN E_QGlobe_SUBDATA_TYPE a_eSubDataType,
					  IN unsigned int a_nLevel,
					  IN unsigned int a_nColumn,
					  IN unsigned int a_nRow,
					  OUT QString& a_strFileName);
