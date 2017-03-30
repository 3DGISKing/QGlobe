#ifndef __GDSREQUESTPROCESS_H__
#define __GDSREQUESTPROCESS_H__

#include "../gds_common/GDSType.h"
#include "../gdem_common/gdem_config.h"

#ifdef STANDARD_ALONE
bool GDSRequestDataForSA(IN void *in_buff, OUT void *out_buff, IN unsigned int o_buff_size);
#else
bool GDSReceivedRequestCallback(INOUT unsigned char* a_pBuff);
#endif

#endif // __GDSREQUESTPROCESS_H__