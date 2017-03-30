#include "datachannel.h"
#include <QLabel>
#include <QBuffer>
#include <QErrorMessage>
#include "datasrv.h"

#ifdef STANDARD_ALONE
#include "../gds_core/GDSCoreInterface.h"
#include "../gds_core/GDSRequestProcess.h"
#else
#include <QTcpSocket>
#include <QtNetwork>
#endif

#ifndef STANDARD_ALONE

class QTcpSocket;
extern DataSrv *g_pDataSrv;

#define MAX_WAIT_FAIL_COUNT 100

DataChannel::DataChannel ()
{
	Init (g_pDataSrv->m_addr, g_pDataSrv->m_port);

	m_nConnectionFailCount = 0;

	if (g_pDataSrv->GetConfig_State())
	{
		m_status = E_CHANNEL_STATUS_NORMAL;
	}
	else
	{
		m_status = E_CHANNEL_STATUS_NONE;
	}

	g_pDataSrv->mutex.lock ();
	g_pDataSrv->chan_list.append (this);
	g_pDataSrv->mutex.unlock ();

	m_nMaxWaitFailCount = MAX_WAIT_FAIL_COUNT;
}

void
DataChannel::Init (const char *host, quint16 port)
{
    ST_DISTRIBUTE_PATH sPATH;

	memset (&sPATH, 0x00, sizeof (sPATH));

	// create default path
	//CreatePath (0 /* default path */, host, port);
	
	m_nConnectionFailCount = 0;

	if (g_pDataSrv && g_pDataSrv->GetConfig_State())
	{
		m_status = E_CHANNEL_STATUS_NORMAL;
	}
	else
	{
		m_status = E_CHANNEL_STATUS_NONE;
	}

	return;        
}

DataChannel::~DataChannel ()
{
	qDebug ("DataChannel::~DataChannel()");

	DeletePathAll ();
	if(g_pDataSrv)
	{
		g_pDataSrv->mutex.lock ();
		g_pDataSrv->chan_list.removeAll (this);
		g_pDataSrv->mutex.unlock ();
	}
}

E_CHANNEL_STATUS_NO
DataChannel::GetStatus (void)
{
	return m_status;
}

void
DataChannel::SetStatus (E_CHANNEL_STATUS_NO status)
{
	m_status = status;
}

bool
DataChannel::CreatePath (path_t path_id, const char *host, quint16 port)
{
	QTcpSocket *path = NULL;
	ST_DISTRIBUTE_PATH sPATH;

	if (NULL == host || INVALID_PORT_NUM == port)
	{
		return false;
	}

	if (m_PathMap.contains (path_id))	// already exist
	{
		qDebug ("Already exist path id (%d)!!!", path_id);
		return false;
	}

	path = new QTcpSocket (this);

	if (NULL == path)
	{
		qDebug ("Can't create path socket!!!");
		return false;
	}

	memset (&sPATH, 0x00, sizeof (sPATH));
	memcpy (sPATH.conf.ip_address, host, strlen (host));
	sPATH.conf.port = port;
	sPATH.path = path;

	
	m_PathMap.insert (path_id, sPATH);

	return true;
}

bool
DataChannel::DeletePath (path_t path_id)
{
	if (!m_PathMap.contains (path_id))
	{
		qDebug ("m_PathMap id(%d) has no content!!!\n", path_id);
		return false;
	}

	if (m_PathMap[path_id].path)
	{
		disconnectFromServer (path_id);
		//m_PathMap[path_id].path->blockSignals (true);
		//m_PathMap[path_id].path->abort ();
		delete m_PathMap[path_id].path;
		m_PathMap[path_id].path = NULL;
	}

	m_PathMap.remove (path_id);

	return true;
}


void
DataChannel::DeletePathAll (void)
{
	path_t id = 0;

	while (!m_PathMap.empty ())
	{
		DeletePath (id);
	}

	m_PathMap.clear ();
}

void
DataChannel::onConnected ()
{
	qDebug ("Connected!!!\n");
}

void
DataChannel::onDisconnected ()
{
	qDebug ("Disconnected!!!\n");
}

void
DataChannel::onDisconnectedByServer ()
{
	qDebug ("Disconnected by server!!!\n");
}

bool
DataChannel::isConnected (path_t path_id)
{
	QTcpSocket *path = NULL;
	const ST_DISTRIBUTE_PATH *path_info = NULL;

	path_info = GetPathInfo (path_id);

	if (NULL == path_info || NULL == path_info->path)
	{
		return false;
	}

	path = path_info->path;

	return path->state () == QAbstractSocket::ConnectedState;
}

const ST_DISTRIBUTE_PATH *
DataChannel::GetPathInfo (path_t path_id)
{
	if (path_id > MAX_DIST_SRV_CNT)
	{
		return NULL;
	}

	if (m_PathMap.contains (path_id))
	{
		return &m_PathMap[path_id];
	}

	return NULL;
}

bool
DataChannel::connectToServer (path_t path_id)
{
	const int Timeout = CONN_TIMEOUT;
	const ST_DISTRIBUTE_PATH *path_info = NULL;
	QTcpSocket *path = NULL;

	path_info = GetPathInfo (path_id);

	if (NULL == path_info || NULL == path_info->path)
	{
		return false;
	}

	path = path_info->path;

	disconnectFromServer (path_id);	//Reset some state, even if we are already disconnected

	path->connectToHost (path_info->conf.ip_address, path_info->conf.port);

	path->waitForConnected (Timeout);

	if (path->state () != QAbstractSocket::ConnectedState)
	{
		if (path->error () == QAbstractSocket::HostNotFoundError)
		{
			qDebug ("ERR_UNKNOWN_HOST %s %d\n", path_info->conf.ip_address,
				path_info->conf.port);
		}
		else
		{
			qDebug ("ERR_COULD_NOT_CONNECT %s %d\n", path_info->conf.ip_address,
				path_info->conf.port);
		}

		return false;
	}

	return true;
}

void
DataChannel::disconnectFromServer (path_t path_id)
{
	const int Timeout = CONN_TIMEOUT;

	const ST_DISTRIBUTE_PATH *path_info = NULL;
	QTcpSocket *path = NULL;

	path_info = GetPathInfo (path_id);

	if (NULL == path_info || NULL == path_info->path)
	{
		return;
	}

	path = path_info->path;

	if (path->state () == QAbstractSocket::UnconnectedState)
	{
		return;
	}

	path->disconnectFromHost ();

	if (path->state () != QAbstractSocket::UnconnectedState)
	{
		path->waitForDisconnected (Timeout);	// wait for unsent data to be sent
	}

	path->close ();		//whatever that means on a socket
}

int
DataChannel::sendData (path_t path_id, void *send_buff, qint64 length)
{
	int written = -1;
	const ST_DISTRIBUTE_PATH *path_info = NULL;
	QTcpSocket *path = NULL;

	// request write
	if (!send_buff)
	{
		qDebug ("write Block stage 1: socket error\n");
		return -1;
	}

	path_info = GetPathInfo (path_id);

	if (NULL == path_info || NULL == path_info->path)
	{
		return -1;
	}

	path = path_info->path;

	if (!isConnected (path_id))
	{
		connectToServer (path_id);
	}

	written = path->write ((const char *) send_buff, length);
	if (-1 == written)
	{
		qDebug ("write Block : socket write error!!!\n");
	}

	path->flush ();

	if (path->state () != QAbstractSocket::ConnectedState)
	{
		qDebug ("write Block stage 2 : socket write error!!!\n");
		return -1;
	}

	return written;
}

int
DataChannel::receiveData (path_t path_id, void *rec_buff, qint64 length)
{

#if 1
	qint64 size = 0, totalSize = 0, lBufLength = 0, numRead = 0;

	const ST_DISTRIBUTE_PATH *path_info = NULL;
	QTcpSocket *path = NULL;

	if (NULL == rec_buff)
	{
		return -1;
	}

	path_info = GetPathInfo (path_id);

	if (NULL == path_info || NULL == path_info->path)
	{
		return -1;
	}

	path = path_info->path;

	while (0 == lBufLength)
	{
		if (!path->waitForReadyRead (READ_TIMEOUT))
		{
			return -1;
		}

		lBufLength = path->bytesAvailable ();
	}

	if (length < lBufLength)
	{
		qDebug ("insufficient memory!!!\n");
		path->read (lBufLength);
		return -1;
	}

	do
	{
		numRead = path->read ((char *) rec_buff + size, GDS_MAX_TRANSFER_SIZE);

		if (numRead == 0 && !path->waitForReadyRead (READ_TIMEOUT))
			break;

		size += numRead;

		if (totalSize == 0)
		{
			totalSize = *(qint32 *) rec_buff;
			if (totalSize < 0)
			{
				qDebug ("Invalid data header. size(%d)!!!\n", (qint32)totalSize);
				return -1;
			}
			else if (totalSize >= length)
			{
				QByteArray un_bytearr;
				do 
				{
					un_bytearr = path->readAll();
				}while (!(un_bytearr.size () == 0 && !path->waitForReadyRead (READ_TIMEOUT)));
				return -1;
			}
		}
	}
	while (size < totalSize);

	return totalSize;

#else
	qint64 size = 0, offset = 0, readed_bytes = 0;

	if (NULL == rec_buff)
	{
		return -1;
	}

	do
	{
		if (!m_socket->waitForReadyRead (READ_TIMEOUT))
		{
			break;
		}
		readed_bytes =
			m_socket->read ((char *) rec_buff + offset, GDS_MAX_TRANSFER_SIZE);

		if (readed_bytes)
		{
			offset += readed_bytes;
		}

		if (size == 0)
		{
			size = (qint64) * (qint32 *) rec_buff;
		}
	}
	while (offset < size);

	return size;
#endif

}

void
DataChannel::onError (int socketError)
{
#if 1
	switch (socketError)
	{
	case QAbstractSocket::ConnectionRefusedError:
		qDebug ("ConnectionRefusedError!!!\n");
		break;
	case QAbstractSocket::RemoteHostClosedError:
		qDebug ("RemoteHostClosedError!!!\n");
		break;
	case QAbstractSocket::HostNotFoundError:
		qDebug ("HostNotFoundError!!!\n");
		break;
	case QAbstractSocket::SocketAccessError:
		qDebug ("SocketAccessError!!!\n");
		break;
	case QAbstractSocket::SocketResourceError:
		qDebug ("SocketResourceError!!!\n");
		break;
	case QAbstractSocket::SocketTimeoutError:
		qDebug ("SocketTimeoutError!!!\n");
		break;
	case QAbstractSocket::DatagramTooLargeError:
		qDebug ("DatagramTooLargeError!!!\n");
		break;
	case QAbstractSocket::NetworkError:
		qDebug ("NetworkError!!!\n");
		break;
	case QAbstractSocket::AddressInUseError:
		qDebug ("AddressInUseError!!!\n");
		break;
	case QAbstractSocket::SocketAddressNotAvailableError:
		qDebug ("SocketAddressNotAvailableError!!!\n");
		break;
	case QAbstractSocket::UnsupportedSocketOperationError:
		qDebug ("UnsupportedSocketOperationError!!!\n");
		break;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		qDebug ("ProxyAuthenticationRequiredError!!!\n");
		break;
	case QAbstractSocket::SslHandshakeFailedError:
		qDebug ("SslHandshakeFailedError!!!\n");
		break;
	case QAbstractSocket::UnfinishedSocketOperationError:
		qDebug ("UnfinishedSocketOperationError!!!\n");
		break;
	default:
		qDebug ("UnknownSocketError!!!\n");
		break;
	}
#else
	qDebug ("SocketError %d occurred!!!\n", socketError);
#endif
}

int
DataChannel::RequestData_block (void *in_buff, void *o_buff,
								unsigned int o_buff_size)
{
	int written_bytes = -1, readed_bytes = -1, in_bytes = -1;
	bool ret = false;
	unsigned int path_id = 0;
	ST_DISTRIBUTE_PC a_PCInfo;
	E_DISTRIBUTE_TYPE dist_type = E_DISTRIBUTE_TYPE_NONE;
	char ip_addr[DS_ADDR_LEN];
	unsigned int port = 0;

	ST_PACKET_HEADER *header = NULL;
	ST_REQUEST_HEADER *request = NULL;
	RENDER_REQUEST *geo_req = NULL;

	if (NULL == in_buff || NULL == o_buff)
	{
		qDebug ("invalid memory pointer!!!\n");
		return RESPONSE_GET_TILE_ERROR_MEMORY;
	}

	in_bytes = *(qint32 *) in_buff;
    if ((0 > in_bytes) || (in_bytes >= INT_MAX))
	{
		qDebug ("invalid input memory size!!!\n");
		return RESPONSE_GET_TILE_ERROR_IN_SIZE;
	}

	header = (ST_PACKET_HEADER *) in_buff;
	request = (ST_REQUEST_HEADER *) header->buffer;

	if ((request->type != E_REQUEST_TYPE_GEONAMELIST) &&
		(request->type != E_REQUEST_TYPE_DBCOMMAND))
	{
		if (m_nConnectionFailCount > 0
			&& m_nConnectionFailCount < m_nMaxWaitFailCount)
		{
			m_nConnectionFailCount++;
			return RESPONSE_GET_TILE_ERROR_WRITE;
		}
		else
			m_nConnectionFailCount = 0;
	}

	memset (ip_addr, 0x00, sizeof(ip_addr));
	memcpy (ip_addr, g_pDataSrv->m_addr, strlen(g_pDataSrv->m_addr));
	port = g_pDataSrv->m_port;

	dist_type = g_DistributeConfig.GetDistributableType ();

	if (E_DISTRIBUTE_TYPE_PC != dist_type)
	{
		// original server. server is non-recognize the client request.
		path_id = 0;
	}
	else
	{
		if (E_REQUEST_TYPE_GEODATA == request->type)
		{
			// get path index
			qDebug ("E_REQUEST_TYPE_GEODATA!!!\n");

			geo_req = (RENDER_REQUEST *) in_buff;
			memset (&a_PCInfo, 0x00, sizeof (a_PCInfo));
			ST_DISTRIBUTE_PC & ref_PCInfo = a_PCInfo;

			ret = g_DistributeConfig.GetPCInfo (geo_req->req.main_type, 
				geo_req->req.level, geo_req->req.column, geo_req->req.row, ref_PCInfo);

			if (!ret)
			{
				qDebug ("not configured server information!!!\n");
				return RESPONSE_GET_TILE_ERROR_OUT_SIZE;
			}

			if (!strlen (ref_PCInfo.ip_address) || 0 == ref_PCInfo.port)
			{
				qDebug ("Invalid address information ip (%s) port (%d)!!!\n",
					ref_PCInfo.ip_address, ref_PCInfo.port);
				return RESPONSE_GET_TILE_ERROR_OUT_SIZE;
			}

			path_id= g_DistributeConfig.GetTargetIndex(geo_req->req.main_type, 
				geo_req->req.level, geo_req->req.column, geo_req->req.row);

			memset (ip_addr, 0x00, sizeof(ip_addr));
			memcpy (ip_addr, ref_PCInfo.ip_address, strlen(ref_PCInfo.ip_address));
			port = ref_PCInfo.port;

			if (!m_PathMap.contains (path_id))
			{
				// create new path
				CreatePath (path_id, ip_addr, port);
			}
			else
			{
				memset (&a_PCInfo, 0x00, sizeof (a_PCInfo));
				ST_DISTRIBUTE_PC & ref_PCInfo = a_PCInfo;

				if (g_DistributeConfig.GetDistributeTargetNum())
				{
					g_DistributeConfig.GetPCInfo (path_id, ref_PCInfo);
				}
				else
				{
					memcpy (ref_PCInfo.ip_address, g_pDataSrv->m_addr, DS_ADDR_LEN -1);
					ref_PCInfo.port = g_pDataSrv->m_port;
				}

				if (memcmp (ref_PCInfo.ip_address, ip_addr, DS_ADDR_LEN) 
					|| ref_PCInfo.port != port)
				{
					DeletePath (path_id);
					CreatePath (path_id, ip_addr, port);
				}
			}
		}
	}

	if (!m_PathMap.contains (path_id))
	{
		// create new path
		CreatePath (path_id, ip_addr, port);
	}
	else
	{
		memset (&a_PCInfo, 0x00, sizeof (a_PCInfo));
		ST_DISTRIBUTE_PC & ref_PCInfo = a_PCInfo;

		if (E_DISTRIBUTE_TYPE_PC == dist_type)
		{
			g_DistributeConfig.GetPCInfo (path_id, ref_PCInfo);
		}
		else
		{
			memcpy (ref_PCInfo.ip_address, m_PathMap[path_id].conf.ip_address, DS_ADDR_LEN -1);
			ref_PCInfo.port = m_PathMap[path_id].conf.port;
		}

		if (memcmp (ref_PCInfo.ip_address, ip_addr, DS_ADDR_LEN) 
			|| ref_PCInfo.port != port)
		{
			DeletePath (path_id);
			CreatePath (path_id, ip_addr, port);
		}
	}

	if (o_buff_size == 0 || o_buff_size >= UINT_MAX)
	{
		qDebug ("invalid output memory size!!!\n");
		return RESPONSE_GET_TILE_ERROR_OUT_SIZE;
	}

	// send reqeust
	written_bytes = sendData (path_id, in_buff, in_bytes);
	if (-1 == written_bytes)
	{
		qDebug ("write data error!!!\n");
		m_nConnectionFailCount++;
		return RESPONSE_GET_TILE_ERROR_WRITE;
	}

	// receive request
	readed_bytes = receiveData (path_id, o_buff, o_buff_size);
	if (-1 == readed_bytes)
	{
		qDebug ("read data error!!!\n");
		return RESPONSE_GET_TILE_ERROR_READ;
	}

	int pad = o_buff_size - readed_bytes;
	if (pad > 4)
	{
		pad = 4;
	}

	memset ((char *) o_buff + readed_bytes, 0x00, pad);

	return RESPONSE_GET_TILE_ERROR_NO;
}


#else // non client-server communication

DataChannel::DataChannel ()
{
}

void
DataChannel::Init (const char *host, quint16 port)
{
	;
}

DataChannel::~DataChannel ()
{
}

void
DataChannel::onConnected ()
{
}

void
DataChannel::onDisconnected ()
{
}

void
DataChannel::onDisconnectedByServer ()
{
}

bool
DataChannel::isConnected (path_t path_id)
{
	return true;
}

bool
DataChannel::connectToServer (path_t path_id)
{
	return true;
}

void
DataChannel::disconnectFromServer (path_t path_id)
{
}

int
DataChannel::sendData (path_t path_id, void *send_buff, qint64 length)
{
	return 0;
}

int
DataChannel::receiveData (path_t path_id, void *rec_buff, qint64 length)
{
	return 0;
}

void
DataChannel::onError (int socketError)
{
}

bool
DataChannel::CreatePath (path_t path_id, const char *host, quint16 port)
{
	return true;
}

bool
DataChannel::DeletePath (path_t path_id)
{
	return true;
}

void
DataChannel::DeletePathAll (void)
{
}

E_CHANNEL_STATUS_NO
DataChannel::GetStatus (void)
{
	return m_status;
}

void
DataChannel::SetStatus (E_CHANNEL_STATUS_NO status)
{
	m_status = status;
}

int
DataChannel::RequestData_block (void *in_buff, void *o_buff,
								unsigned int o_buff_size)
{
	int ret;

	ret = GDSRequestDataForSA (in_buff, o_buff, o_buff_size);

        return RESPONSE_GET_TILE_ERROR_NO;
}
#endif
