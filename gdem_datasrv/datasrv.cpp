#include "datasrv.h"
#include "datachannel.h"
#include <QTime>
#include <QTimerEvent>

#include "../gdem_common/gdem_config.h"

//#define DATASERVER_DEBUG 0

#if defined (WIN32) && defined (DATASERVER_DEBUG)
#include "Windows.h"
#endif

DataSrv *g_pDataSrv = NULL;

DataSrv::DataSrv ()
{
	m_addr[0] = 0;
	m_port = 0;

	m_bConfigured = false;

	g_pDataSrv = this;

	g_pDataSrv->mutex.lock ();
	chan_list.clear ();
	g_pDataSrv->mutex.unlock ();

	m_pconfThread = NULL;
}

void DataSrv::FinishThread()
{
 /*
        if (m_pconfThread)
        {
                if (m_pconfThread->isRunning())
                {
                        m_pconfThread->terminate ();
                        m_pconfThread->Sleep(DEFAULT_TIMEOUT);
                }
                delete m_pconfThread;
                m_pconfThread = NULL;
        }
        */
    if(!m_pconfThread)
        return;

    m_pconfThread->Finsish();
    while(!m_pconfThread->isFinished())
    {
        m_pconfThread->Sleep(10);
    }

    delete m_pconfThread;
    m_pconfThread = NULL;
}
DataSrv::~DataSrv ()
{
    FinishThread();

	g_pDataSrv = NULL;
	chan_list.clear ();
}

bool 
DataSrv::Init ()
{
	//NewChannel(DS_SOCK_MAX);
	return true;
}

void
DataSrv::Release ()
{

}


void
DataSrv::SetServer (char *addr, port_t port)
{
#if 0
	bool ret = false;
	memcpy (m_addr, addr, DS_ADDR_LEN);
	m_port = port;
	m_bConfigured = false;

#ifndef STANDARD_ALONE
	if (m_confTimer.isActive ())
	{
		m_confTimer.stop ();
	}

	mutex.lock ();
	ret = FillServerConfig (m_addr, m_port);
	mutex.unlock ();

	if (true == ret)
	{
		m_bConfigured = true;
		m_confTimer.stop ();
	}
	else
	{
		m_confTimer.start (CONN_TIMEOUT + SEND_TIMEOUT + READ_TIMEOUT, this);
	}

#endif
#else
	bool ret = false;
	memcpy (m_addr, addr, DS_ADDR_LEN);
	m_port = port;
	m_bConfigured = false;

#ifndef STANDARD_ALONE 
    FinishThread();

	m_pconfThread = new DataConfig ();
	if (NULL == m_pconfThread)
	{
		qDebug ("DataConfig Thread create fail!!!\n");
		return;
	}

	m_pconfThread->start ();
#endif /* endof STANDARD_ALONE */
#endif
}


extern int
gdm_GetTileData (void *channel, void *in_buff, void *o_buff,
				 unsigned int o_buff_size)
{
	if (channel == NULL)
		return RESPONSE_GET_TILE_ERROR_DATASRV;

	DataChannel *pChannel = (DataChannel *) channel;

#ifndef STANDARD_ALONE
	if (E_CHANNEL_STATUS_RECONFIG == pChannel->GetStatus ())
	{
		qDebug ("channel does not configured yet!!!");

		for (path_t path_id = 0; path_id < pChannel->m_PathMap.size(); path_id++)
		{
			if (pChannel->isConnected (path_id))
			{
				pChannel->disconnectFromServer (path_id);
			}
		}
		return RESPONSE_GET_TILE_ERROR_NONCONFIG;
	}
	else if (E_CHANNEL_STATUS_NONE == pChannel->GetStatus ())
	{
		return RESPONSE_GET_TILE_ERROR_NONCONFIG;
	}

	return pChannel->RequestData_block (in_buff, o_buff, o_buff_size);

#else

	return pChannel->RequestData_block (in_buff, o_buff, o_buff_size);

#endif
}

