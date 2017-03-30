#ifndef COMMON_H
#define COMMON_H


#define DS_SOCK_MAX 6
#define GDS_MAX_TRANSFER_SIZE	(1024*1024)	// 260KB
#define INVALID_ID -1

#define INVALID_PORT_NUM					0

#define RESPONSE_GET_TILE_ERROR_NO			0
#define RESPONSE_GET_TILE_ERROR_MEMORY		1
#define RESPONSE_GET_TILE_ERROR_RESOURCE	2 
#define RESPONSE_GET_TILE_ERROR_WRITE		3			
#define RESPONSE_GET_TILE_ERROR_READ		4
#define RESPONSE_GET_TILE_ERROR_IN_SIZE		5
#define RESPONSE_GET_TILE_ERROR_OUT_SIZE	6
#define RESPONSE_GET_TILE_ERROR_DATASRV		7
#define RESPONSE_GET_TILE_ERROR_DISTTYPE	8
#define RESPONSE_GET_TILE_ERROR_NONCONFIG	9


#define DEFAULT_TIMEOUT 3000
#define SEND_TIMEOUT    DEFAULT_TIMEOUT
#define READ_TIMEOUT    DEFAULT_TIMEOUT
#define CONN_TIMEOUT    DEFAULT_TIMEOUT

#include "gdem_datasrv_global.h"

typedef enum
{
	E_CHANNEL_STATUS_NONE = 0,
	E_CHANNEL_STATUS_RECONFIG,
	E_CHANNEL_STATUS_NORMAL,

} E_CHANNEL_STATUS_NO;

typedef unsigned int ip_t;
typedef unsigned short port_t;


GDEM_DATASRV_EXPORT int gdm_GetTileData(void *channel, void *in_buff, void *o_buff, unsigned int o_buff_size);

#endif // COMMON_H
