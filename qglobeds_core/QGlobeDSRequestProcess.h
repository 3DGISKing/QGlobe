#ifndef __GDSREQUESTPROCESS_H__
#define __GDSREQUESTPROCESS_H__

#include "../qglobeds_common/QGlobeDSType.h"
#include "../qglobe_common/qglobe_config.h"

#ifdef STANDARD_ALONE
bool QGlobeDSRequestDataForSA(IN void *in_buff, OUT void *out_buff, IN unsigned int o_buff_size);
#else
bool QGlobeDSReceivedRequestCallback(INOUT unsigned char* a_pBuff);
#endif

#endif // __GDSREQUESTPROCESS_H__