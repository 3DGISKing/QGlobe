// GDSTcpServer.cpp

#include "GDSTcpServer.h"
#include "GDSTcpThread.h"
#include "GDSNetworkInterface.h"
#include "../gds_common/GDSLog.h"

#include <QThread>

// class GDSStopThread
/////////////////////////////////////////////
class GDSStopThread : public QThread
 {
 public:
     void run();

	 void SetServer(GDSTcpServer* pServer)
	 {
		m_bStoped = false;
		m_pServer = pServer;
	 }

	 bool m_bStoped;

 private:
	 GDSTcpServer* m_pServer;

 };

 void GDSStopThread::run()
 {
 	while (!m_pServer->EmptyStartedThread())
	{
		msleep(1000);
	}

	m_bStoped = true;
}

GDSTcpServer::GDSTcpServer(QObject *parent)
: QTcpServer(parent)
, m_PortNo(33333)
, m_MaxConnNum(1000)
, m_started(false)
{
	m_status = SERVER_STOPPED;

	GDSLogOutput(E_LOG_TRACK, "TCP server initialized.");
}

GDSTcpServer::~GDSTcpServer()
{
	stop();

	removeStopedThread();

	GDSLogOutput(E_LOG_TRACK, "TCP server finished.");
}

void GDSTcpServer::removeStopedThread()
{
	GDSTcpThread	*thread;

	while (!m_StopedThreadList.empty())
	{
		thread = (GDSTcpThread *)m_StopedThreadList.front();
		m_StopedThreadList.pop_front();

		delete thread;
	}
}

bool GDSTcpServer::start()
{
	if (m_status != SERVER_STOPPED)
	{
		return false;
	}

	if (!listen(QHostAddress::Any, m_PortNo))
	{
		GDSLogOutput(E_LOG_ERROR, "%s", errorString().toAscii().data());

		return false;
	}

	m_status = SERVER_STARTED;

	GDSLogOutput(E_LOG_TRACK, "TCP server started.");

	m_started = true;

	return true;
}

bool GDSTcpServer::pause()
{
	if (m_status != SERVER_STARTED)
	{
		return false;
	}

	m_status = SERVER_PAUSED;

	close();

	GDSLogOutput(E_LOG_TRACK, "TCP server paused.");

	return true;
}

bool GDSTcpServer::resume()
{
	if (m_status != SERVER_PAUSED)
	{
		return false;
	}

	if (!listen(QHostAddress::Any, m_PortNo))
	{
		GDSLogOutput(E_LOG_ERROR, "%s", errorString().toAscii().data());

		return false;
	}

	m_status = SERVER_STARTED;

	GDSLogOutput(E_LOG_TRACK, "TCP server resumed.");

	return true;
}

bool GDSTcpServer::stop()
{
	if (m_status != SERVER_STARTED && m_status != SERVER_PAUSED)
	{
		return false;
	}

	m_status = SERVER_STOPPED;

	close();

	m_started = false;

	GDSStopThread thread;
	thread.SetServer(this);
	thread.start();
	while (!thread.m_bStoped)
	{
	}

	GDSLogOutput(E_LOG_TRACK, "TCP server stopped.");

	return true;
}

void GDSTcpServer::incomingConnection(int socketDescriptor)
{
	//removeStopedThread();

	GDSTcpThread	*thread = new GDSTcpThread(socketDescriptor, this);

	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	m_StartedThreadList.push_back((QObject *)thread);
	thread->start();

	GDSLogOutput(E_LOG_TRACK, "Thread %p created. %d thread(s) activated.", thread, m_StartedThreadList.size());

	if (m_StartedThreadList.size() == m_MaxConnNum)
	{
		pause();
	}
}

void GDSTcpServer::notifyStopThread(GDSTcpThread *thread)
{
	if (!m_StartedThreadList.contains(thread))
		return;
	m_StartedThreadList.removeOne((QObject *)thread);
	m_StopedThreadList.push_back((QObject *)thread);

	GDSLogOutput(E_LOG_TRACK, "Thread %p stoped. %d thread(s) remained.", thread, m_StartedThreadList.size());

	if (m_StartedThreadList.size() < m_MaxConnNum)
	{
		resume();
	}
}

void GDSTcpServer::notifyDeleteThread(GDSTcpThread *thread)
{
	if (m_StartedThreadList.contains(thread))
		m_StartedThreadList.removeOne((QObject *)thread);
	if (m_StopedThreadList.contains(thread))
		m_StopedThreadList.removeOne((QObject *)thread);

	GDSLogOutput(E_LOG_TRACK, "Thread %p deleted. %d thread(s) remained.", thread, m_StartedThreadList.size());
}

bool GDSTcpServer::EmptyStartedThread()
{
	return m_StartedThreadList.empty();
}

