// GDSTcpThread.cpp

#include <QtNetwork>

#include "GDSTcpThread.h"
#include "GDSTcpServer.h"
#include "GDSNetworkInterface.h"
#include "../gds_common/GDSLog.h"



GDSTcpThread::GDSTcpThread(int socketDescriptor, QObject *parent)
: QThread(parent)
, m_tcpServer((GDSTcpServer *)parent)
, m_socketDescriptor(socketDescriptor)
{
	m_data = new char[GDS_MAX_TRANSFER_SIZE];
}

GDSTcpThread::~GDSTcpThread()
{
	QTcpSocket	tcpSocket;
	char		ipStr[16];

#if 0
	terminate();

	GDSLogOutput(E_LOG_TRACK, "Thread %p terminated.", this);

	if (tcpSocket.setSocketDescriptor(m_socketDescriptor))
	{
		if (tcpSocket.state() != QAbstractSocket::UnconnectedState)
		{
			strcpy(ipStr, tcpSocket.peerAddress().toString().toAscii().data());

			tcpSocket.disconnectFromHost();
			if (tcpSocket.state() != QAbstractSocket::UnconnectedState)
			{
				tcpSocket.waitForDisconnected();
			}

			GDSLogOutput(E_LOG_TRACK, "%s disconnected from thread %p.", ipStr, this);
		}
	}
#else
	GDSLogOutput(E_LOG_TRACK, "Thread %p terminated.", this);
#endif

	delete []m_data;

	m_tcpServer->notifyDeleteThread(this);
}

void GDSTcpThread::run()
{
	QTcpSocket	tcpSocket;
	char		ipStr[16];

	if (tcpSocket.setSocketDescriptor(m_socketDescriptor))
	{
		strcpy(ipStr, tcpSocket.peerAddress().toString().toAscii().data());

		GDSLogOutput(E_LOG_TRACK, "%s connected to thread %p.", ipStr, this);

		while (tcpSocket.state() == QAbstractSocket::ConnectedState)
		{
			qint64	size = 0, offset = 0;

			do
			{
				if (!tcpSocket.waitForReadyRead(10000))
				{
					break;
				}

				offset += tcpSocket.read(m_data+offset, GDS_MAX_TRANSFER_SIZE);

				if (size == 0)
				{
					size = (qint64)*(qint32 *)m_data;

					GDSLogOutput(E_LOG_TRACK, "%s sent %d bytes to thread %p.", ipStr, (qint32)size, this);
				}
			} while (offset < size);

			if (offset != 0)
			{
				GDSLogOutput(E_LOG_TRACK, "Thread %p received %d bytes from %s.", this, (qint32)offset, ipStr);

				if (size == offset)
				{
					if (g_pReceivedCallbackFunc((unsigned char *)m_data))
					{
						GDSLogOutput(E_LOG_TRACK, "Thread %p read %d bytes from database.", this, *(qint32 *)m_data);
					}
					else
					{
						GDSLogOutput(E_LOG_TRACK, "Thread %p failed to get requested data from database.", this);
					}

					size = tcpSocket.write(m_data, (qint64)*(qint32 *)m_data);
					if (size > GDS_MAX_TRANSFER_SIZE)
					{
						GDSLogOutput(E_LOG_ERROR, "Buffer overflow in thread %p.", this);
					}

					GDSLogOutput(E_LOG_TRACK, "Thread %p sent %d bytes to %s.", this, (qint32)size, ipStr);
				}
			}

			if (!m_tcpServer->m_started)
			{
				break;
			}

			//msleep(100);
		}

		tcpSocket.disconnectFromHost();

		if (tcpSocket.state() != QAbstractSocket::UnconnectedState)
		{
			tcpSocket.waitForDisconnected();
		}
	}
	else
	{
		emit error(tcpSocket.error());
	}


	m_tcpServer->notifyStopThread(this);

	GDSLogOutput(E_LOG_TRACK, "%s disconnected from thread %p.", ipStr, this);
}
