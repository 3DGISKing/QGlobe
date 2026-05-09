#ifndef __GDSCOMMON_H__
#define __GDSCOMMON_H__

#include "QGlobeDSType.h"
#include "QGlobeDSDefine.h"

#include "../qglobe_common/qglobe_protocol.h"

E_COMPRESS_TYPE GetCompressType(IN E_QGlobe_MAINDATA_TYPE a_eDataType);

bool IsInvalidLevel(IN E_QGlobe_SUBDATA_TYPE a_eDataType, IN unsigned int a_nLevel);

#endif // __GDSCOMMON_H__