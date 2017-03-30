#ifndef __GDSCOMMON_H__
#define __GDSCOMMON_H__

#include "GDSType.h"
#include "GDSDefine.h"

#include "../gdem_common/gdem_protocol.h"

E_COMPRESS_TYPE GetCompressType(IN E_GDM_MAINDATA_TYPE a_eDataType);

bool IsInvalidLevel(IN E_GDM_SUBDATA_TYPE a_eDataType, IN unsigned int a_nLevel);

#endif // __GDSCOMMON_H__