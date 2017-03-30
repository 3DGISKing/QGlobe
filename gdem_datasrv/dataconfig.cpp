#include "common.h"
#include "dataconfig.h"
#include "datachannel.h"
#include "datasrv.h"

#define DIST_CONFIG_MAX_SIZE	(150*1024)

extern DataSrv *g_pDataSrv;

DataConfig::DataConfig ()
{
	m_pRequest = NULL;
	m_pResponse = NULL;
	m_pRequest = (ST_PACKET_HEADER *)malloc(DIST_CONFIG_MAX_SIZE);
	m_pResponse = (ST_PACKET_HEADER *)malloc(DIST_CONFIG_MAX_SIZE);
	m_nBufferSize = DIST_CONFIG_MAX_SIZE;
	m_blFinish = false;
}

DataConfig::~DataConfig ()
{
	if (m_pRequest != NULL)
	{
		free(m_pRequest);
	}
	if (m_pResponse != NULL)
	{
		free(m_pResponse);
	}
}

void DataConfig::run ()
{
	char addr[DS_ADDR_LEN];
	port_t port = 0;
	m_blFinish = false;

	try
	{
		while (1)
		{
			if (g_pDataSrv && !g_pDataSrv->GetConfig_State())
			{
				memset (addr, 0x00, sizeof(addr));
				memcpy (addr, g_pDataSrv->m_addr, strlen(g_pDataSrv->m_addr));
				port = g_pDataSrv->m_port;

				setTerminationEnabled (false);
				if (ServerConfig (addr, port))
				{
					g_pDataSrv->SetConfig_State (true);
				}
				setTerminationEnabled ();
				if(m_blFinish)
					break;
			}

			for (int i =0; i < 10; i++)
			{
				this->msleep (1000);
				if(m_blFinish)
				{
					return;
				}
			}
		}
		//terminate();
	}
	catch (...)
	{
		// handle thread error
		qDebug ("handle dataconfig error !!!");
	}

}

bool DataConfig::ServerConfig (const char *addr, port_t port)
{
	bool ret = false;
	DataChannel *newChannel = NULL;
	DataChannel *confChannel = NULL;
	ST_PACKET_HEADER *pPacket_Hdr = NULL;
	ST_RESPONSE_HEADER *pResp_Hdr = NULL;
	ST_REQUEST_HEADER *header = NULL;

	if (NULL == g_pDataSrv)
	{
		return ret;
	}
	/* first release old server config */
	g_DistributeConfig.Initialize();

	newChannel = new DataChannel ();
	newChannel->SetMaxWaitCount(0);
	if (!newChannel)
	{
		qDebug ("Fail in create the DataChannel!!!\n");
		return ret;
	}

	newChannel->SetStatus (E_CHANNEL_STATUS_NORMAL);

	// request buffer
	m_pRequest->bytes = sizeof(ST_PACKET_HEADER) - sizeof(char)*4 + sizeof(ST_REQUEST_HEADER) - sizeof(char)*4;

	header = (ST_REQUEST_HEADER *) m_pRequest->buffer;
	header->type = E_REQUEST_TYPE_DISTRIBUTE;

	g_pDataSrv->mutex.lock ();
	/* first set reconfig flag */
	for (int chan_id = 0; chan_id < g_pDataSrv->chan_list.size (); ++chan_id)
	{
		confChannel = g_pDataSrv->chan_list.at (chan_id);
		if (confChannel && confChannel != newChannel
			&& E_CHANNEL_STATUS_RECONFIG != confChannel->m_status)
		{
			confChannel->SetStatus (E_CHANNEL_STATUS_RECONFIG);
		}
	}
	g_pDataSrv->mutex.unlock ();

	do
	{
		if (RESPONSE_GET_TILE_ERROR_NO ==
			gdm_GetTileData (newChannel, m_pRequest, m_pResponse, DIST_CONFIG_MAX_SIZE))
		{
			if(m_blFinish)
				break;
			pPacket_Hdr = (ST_PACKET_HEADER *) m_pResponse;
			pResp_Hdr = (ST_RESPONSE_HEADER *) m_pResponse->buffer;

			// validate pointer
			if (NULL == m_pResponse || NULL == pResp_Hdr
				|| NULL == pResp_Hdr->body)
			{
				break;
			}

			// validate error 
			if (E_RESPONSE_ERROR_NONE != pResp_Hdr->result)
			{
				break;
			}

			// validate response type
			if (E_REQUEST_TYPE_DISTRIBUTE != pResp_Hdr->type)
			{
				qDebug ("type fail(%d)l!!!\n", pResp_Hdr->type);
				break;
			}

			// validate response distribute data
			if (m_pResponse->bytes > DIST_CONFIG_MAX_SIZE)
			{
				qDebug ("size fail(%d)l!!!\n", m_pResponse->bytes);
				break;
			}

			if (g_DistributeConfig.ReadFromBuffer((char *) pResp_Hdr->body, m_pResponse->bytes - sizeof(ST_RESPONSE_HEADER)))
			{
				ret = true;
				g_pDataSrv->mutex.lock ();
				/* unset reconfig flag */
				for (int chan_id = 0; chan_id < g_pDataSrv->chan_list.size (); ++chan_id)
				{
					confChannel = g_pDataSrv->chan_list.at (chan_id);
					if (confChannel && confChannel != newChannel
						&& E_CHANNEL_STATUS_RECONFIG == confChannel->GetStatus ())
					{
						//// create default path
						//ST_DISTRIBUTE_PC a_PCInfo;
						//memset (&a_PCInfo, 0x00, sizeof (a_PCInfo));
						//ST_DISTRIBUTE_PC & ref_PCInfo = a_PCInfo;
						//if (GetPCInfo (0, ref_PCInfo))
						//{
						//	confChannel->Init (ref_PCInfo.ip_address, ref_PCInfo.port);							
						//}
						//else	//non distributed
						//{
						//	confChannel->Init (m_addr, m_port);							
						//}

						confChannel->SetStatus (E_CHANNEL_STATUS_NORMAL);
					}
				}
				g_pDataSrv->mutex.unlock ();

				qDebug ("read success for global server config!!!\n");
			}
		}
		else
		{
			if(m_blFinish)
				break;
			qDebug ("read socket fail for global server config!!!\n");
		}
	}
	while (0);


	/* free datachannel */
	if (newChannel)
	{
		delete newChannel;
		newChannel = NULL;
	}

	return ret;
}

