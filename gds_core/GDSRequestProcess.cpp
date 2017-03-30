#include "GDSRequestProcess.h"

#include "../gds_database/GDSDatabaseInterface.h"
#include "../gds_common/GDSLog.h"

#ifndef STANDARD_ALONE
#include "../gdem_common/gdem_distributeconfig.h"
#endif

#ifdef STANDARD_ALONE

//====================================================================================
// Function name	: GDSRequestDataForSA (for standard alone)
// Creator			: kangcj
// Description		: process request of client
// Return type		: bool - result(true : success, false : fail) 
// Argument1		: IN void *in_buff - request buffer
// Argument2		: OUT void *out_buff - response buffer
// Argument3		: IN unsigned int out_buff_size - response buffer size
//====================================================================================
bool GDSRequestDataForSA(IN void *in_buff, OUT void *out_buff, IN unsigned int out_buff_size)
{
	if (in_buff == NULL)
	{
		GDSLogOutput(E_LOG_ERROR, "Input buffer param is null at GDSRequestDataForSA.");
		return false;
	}

	if (out_buff == NULL || out_buff_size == 0)
	{
		GDSLogOutput(E_LOG_ERROR, "Output buffer param is null at GDSRequestDataForSA.");
		return false;
	}

	ST_PACKET_HEADER* tInBufferHeader = (ST_PACKET_HEADER*)in_buff;
	ST_PACKET_HEADER* tOutBufferHeader = (ST_PACKET_HEADER*)out_buff;
	E_RESPONSE_ERROR_TYPE tErrorCode = E_RESPONSE_ERROR_UNKNOWN;

	ST_REQUEST_HEADER* pRequest = (ST_REQUEST_HEADER*)tInBufferHeader->buffer;
	ST_RESPONSE_HEADER* pResponse = (ST_RESPONSE_HEADER*)tOutBufferHeader->buffer;
	pResponse->type = pRequest->type;

	unsigned int tBytes = 0;

	switch(pRequest->type)
	{
	case E_REQUEST_TYPE_GEONAMELIST:
		{
			if (GetGeoNameList((ST_REQUEST_GEONAMESEARCH*)pRequest->body, 
				(ST_RESPONSE_GEONAMESEARCH*)pResponse->body,
				&tBytes,
				&tErrorCode))
			{
				if (tBytes > (GDS_MAX_TRANSFER_SIZE - sizeof(ST_RESPONSE_HEADER)))
				{
					tErrorCode = E_RESPONSE_ERROR_BYTES_OVERFLOW;
					break;
				}

				pResponse->result = E_RESPONSE_ERROR_NONE;
				tOutBufferHeader->bytes = sizeof(ST_RESPONSE_HEADER) + tBytes;
				return true;
			}

			break;
		}
	case E_REQUEST_TYPE_GEODATA:
		{
			if (GetGeoData((ST_REQUEST_GEODATA*)pRequest->body, 
				(ST_RESPONSE_GEODATA*)pResponse->body,
				&tBytes, 
				&tErrorCode))
			{
				if (tBytes > (GDS_MAX_TRANSFER_SIZE - sizeof(ST_RESPONSE_HEADER)))
				{
					tErrorCode = E_RESPONSE_ERROR_BYTES_OVERFLOW;
					break;
				}

				pResponse->result = E_RESPONSE_ERROR_NONE;
				tOutBufferHeader->bytes = sizeof(ST_RESPONSE_HEADER) + tBytes;
				return true;
			}

			break;
		}
	case E_REQUEST_TYPE_DBCOMMAND:
		{
			if (RunDBCommand((ST_REQUEST_DBCOMMAND*)pRequest->body, 
				(ST_RESPONSE_DBCOMMAND*)pResponse->body,
				&tBytes, 
				&tErrorCode))
			{
				if (tBytes > (GDS_MAX_TRANSFER_SIZE - sizeof(ST_RESPONSE_HEADER)))
				{
					tErrorCode = E_RESPONSE_ERROR_BYTES_OVERFLOW;
					break;
				}

				pResponse->result = E_RESPONSE_ERROR_NONE;
				tOutBufferHeader->bytes = sizeof(ST_RESPONSE_HEADER) + tBytes;
				return true;
			}

			break;
		}
	default:
		tErrorCode = E_RESPONSE_ERROR_UNKNOWNREQUEST;
		break;
	}

	pResponse->result = tErrorCode;
	tOutBufferHeader->bytes = sizeof(ST_RESPONSE_HEADER);
	GDSLogOutput(E_LOG_ERROR, "Response failed at GDSRequestDataForSA.[error : %s]", g_strErrorList[tErrorCode]);
	return false;
}

#else

//====================================================================================
// Function name	: GDSReceivedRequestCallback
// Creator			: kangcj
// Description		: process request of client
// Return type		: bool - result(true : success, false : fail) 
// Argument1		: INOUT unsigned char* a_pBuff - request packet buffer
//====================================================================================
bool GDSReceivedRequestCallback(INOUT unsigned char* a_pBuff)
{
	if (a_pBuff == NULL)
	{
#ifdef DEBUG
		GDSLogOutput(E_LOG_ERROR, "Buffer param is null at GDSReceivedRequestCallback.");
#endif
		return false;
	}

	ST_PACKET_HEADER* tPacketHeader = (ST_PACKET_HEADER*)a_pBuff;
	E_RESPONSE_ERROR_TYPE tErrorCode = E_RESPONSE_ERROR_UNKNOWN;

	ST_REQUEST_HEADER* pRequest = (ST_REQUEST_HEADER*)tPacketHeader->buffer;
	ST_RESPONSE_HEADER* pResponse = (ST_RESPONSE_HEADER*)tPacketHeader->buffer;

	unsigned int tBytes = 0;

	switch(pRequest->type)
	{
	case E_REQUEST_TYPE_DISTRIBUTE:
		{
			unsigned int nBufferSize = 0;
			if (g_DistributeConfig.WriteToBuffer((char*)pResponse->body, nBufferSize))
			{
				pResponse->result = E_RESPONSE_ERROR_NONE;
				tPacketHeader->bytes = sizeof(ST_RESPONSE_HEADER) + nBufferSize;
				return true;
			}

			break;
		}
	case E_REQUEST_TYPE_GEONAMELIST:
		{
			if (GetGeoNameList((ST_REQUEST_GEONAMESEARCH*)pRequest->body, 
					(ST_RESPONSE_GEONAMESEARCH*)pResponse->body,
					&tBytes,
					&tErrorCode))
			{
				if (tBytes > (GDS_MAX_TRANSFER_SIZE - sizeof(ST_RESPONSE_HEADER)))
				{
					tErrorCode = E_RESPONSE_ERROR_BYTES_OVERFLOW;
					break;
				}

				pResponse->result = E_RESPONSE_ERROR_NONE;
				tPacketHeader->bytes = sizeof(ST_RESPONSE_HEADER) + tBytes;
				return true;
			}

			break;
		}
	case E_REQUEST_TYPE_GEODATA:
		{
			if (GetGeoData((ST_REQUEST_GEODATA*)pRequest->body, 
					(ST_RESPONSE_GEODATA*)pResponse->body,
					&tBytes, 
					&tErrorCode))
			{
				if (tBytes > (GDS_MAX_TRANSFER_SIZE - sizeof(ST_RESPONSE_HEADER)))
				{
					tErrorCode = E_RESPONSE_ERROR_BYTES_OVERFLOW;
					break;
				}

				pResponse->result = E_RESPONSE_ERROR_NONE;
				tPacketHeader->bytes = sizeof(ST_RESPONSE_HEADER) + tBytes;
				return true;
			}

			break;
		}
	case E_REQUEST_TYPE_DBCOMMAND:
		{
			if (RunDBCommand((ST_REQUEST_DBCOMMAND*)pRequest->body, 
				(ST_RESPONSE_DBCOMMAND*)pResponse->body,
				&tBytes, 
				&tErrorCode))
			{
				if (tBytes > (GDS_MAX_TRANSFER_SIZE - sizeof(ST_RESPONSE_HEADER)))
				{
					tErrorCode = E_RESPONSE_ERROR_BYTES_OVERFLOW;
					break;
				}

				pResponse->result = E_RESPONSE_ERROR_NONE;
				tPacketHeader->bytes = sizeof(ST_RESPONSE_HEADER) + tBytes;
				return true;
			}

			break;
		}
	default:
		tErrorCode = E_RESPONSE_ERROR_UNKNOWNREQUEST;
		break;
	}

	pResponse->result = tErrorCode;
	tPacketHeader->bytes = sizeof(ST_RESPONSE_HEADER);

	if (tErrorCode != E_RESPONSE_ERROR_INVALID_LEVEL)
		GDSLogOutput(E_LOG_ERROR, "GeoData response failed at GDSReceivedRequestCallback.[error : %s]", g_strErrorList[tErrorCode]);
	return false;
}

#endif
