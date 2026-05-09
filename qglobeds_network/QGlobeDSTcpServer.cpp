// QGlobeDSTcpServer.cpp

#include "QGlobeDSTcpServer.h"
#include "QGlobeDSTcpThread.h"
#include "QGlobeDSNetworkInterface.h"
#include "../qglobeds_common/QGlobeDSLog.h"

#include <QThread>

// class QGlobeDSStopThread
/////////////////////////////////////////////
class QGlobeDSStopThread : public QThread
 {
 public:
     void run();

	 void SetServer(QGlobeDSTcpServer* pServer)
	 {
		m_bStoped = false;
		m_pServer = pServer;
	 }

	 bool m_bStoped;

 private:
	 QGlobeDSTcpServer* m_pServer;

 };

 void QGlobeDSStopThread::run()
 {
 	while (!m_pServer->EmptyStartedThread())
	{
		msleep(1000);
	}

	m_bStoped = true;
}

QGlobeDSTcpServer::QGlobeDSTcpServer(QObject *parent)
: QTcpServer(parent)
, m_PortNo(33333)
, m_MaxConnNum(1000)
, m_started(false)
{
	m_status = SERVER_STOPPED;

	QGlobeDSLogOutput(E_LOG_TRACK, "TCP server initialized.");
}

QGlobeDSTcpServer::~QGlobeDSTcpServer()
{
	stop();

	removeStopedThread();

	QGlobeDSLogOutput(E_LOG_TRACK, "TCP server finished.");
}

void QGlobeDSTcpServer::removeStopedThread()
{
	QGlobeDSTcpThread	*thread;

	while (!m_StopedThreadList.empty())
	{
		thread = (QGlobeDSTcpThread *)m_StopedThreadList.front();
		m_StopedThreadList.pop_front();

		delete thread;
	}
}

bool QGlobeDSTcpServer::start()
{
	if (m_status != SERVER_STOPPED)
	{
		return false;
	}

	if (!listen(QHostAddress::Any, m_PortNo))
	{
		QGlobeDSLogOutput(E_LOG_ERROR, "%s", errorString().toUtf8().data());

		return false;
	}

	m_status = SERVER_STARTED;

	QGlobeDSLogOutput(E_LOG_TRACK, "TCP server started.");

	m_started = true;

	return true;
}

bool QGlobeDSTcpServer::pause()
{
	if (m_status != SERVER_STARTED)
	{
		return false;
	}

	m_status = SERVER_PAUSED;

	close();

	QGlobeDSLogOutput(E_LOG_TRACK, "TCP server paused.");

	return true;
}

bool QGlobeDSTcpServer::resume()
{
	if (m_status != SERVER_PAUSED)
	{
		return false;
	}

	if (!listen(QHostAddress::Any, m_PortNo))
	{
		QGlobeDSLogOutput(E_LOG_ERROR, "%s", errorString().toUtf8().data());

		return false;
	}

	m_status = SERVER_STARTED;

	QGlobeDSLogOutput(E_LOG_TRACK, "TCP server resumed.");

	return true;
}

bool QGlobeDSTcpServer::stop()
{
	if (m_status != SERVER_STARTED && m_status != SERVER_PAUSED)
	{
		return false;
	}

	m_status = SERVER_STOPPED;

	close();

	m_started = false;

	QGlobeDSStopThread thread;
	thread.SetServer(this);
	thread.start();
	while (!thread.m_bStoped)
	{
	}

	QGlobeDSLogOutput(E_LOG_TRACK, "TCP server stopped.");

	return true;
}

void QGlobeDSTcpServer::incomingConnection(int socketDescriptor)
{
	//removeStopedThread();

	QGlobeDSTcpThread	*thread = new QGlobeDSTcpThread(socketDescriptor, this);

	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	m_StartedThreadList.push_back((QObject *)thread);
	thread->start();

	QGlobeDSLogOutput(E_LOG_TRACK, "Thread %p created. %d thread(s) activated.", thread, m_StartedThreadList.size());

	if (m_StartedThreadList.size() == m_MaxConnNum)
	{
		pause();
	}
}

void QGlobeDSTcpServer::notifyStopThread(QGlobeDSTcpThread *thread)
{
	if (!m_StartedThreadList.contains(thread))
		return;
	m_StartedThreadList.removeOne((QObject *)thread);
	m_StopedThreadList.push_back((QObject *)thread);

	QGlobeDSLogOutput(E_LOG_TRACK, "Thread %p stoped. %d thread(s) remained.", thread, m_StartedThreadList.size());

	if (m_StartedThreadList.size() < m_MaxConnNum)
	{
		resume();
	}
}

void QGlobeDSTcpServer::notifyDeleteThread(QGlobeDSTcpThread *thread)
{
	if (m_StartedThreadList.contains(thread))
		m_StartedThreadList.removeOne((QObject *)thread);
	if (m_StopedThreadList.contains(thread))
		m_StopedThreadList.removeOne((QObject *)thread);

	QGlobeDSLogOutput(E_LOG_TRACK, "Thread %p deleted. %d thread(s) remained.", thread, m_StartedThreadList.size());
}

bool QGlobeDSTcpServer::EmptyStartedThread()
{
	return m_StartedThreadList.empty();
}

