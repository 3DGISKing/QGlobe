// GDSTcpServer.h

#ifndef GDSTCPSERVER_H
#define GDSTCPSERVER_H

#include <QTcpServer>


class GDSTcpThread;

enum GDSTcpServerStatus
{
	SERVER_STOPPED,
	SERVER_STARTED,
	SERVER_PAUSED
};

class GDSTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    GDSTcpServer(QObject *parent = 0);
    ~GDSTcpServer();

	void SetPortNo(unsigned short a_PortNo) { m_PortNo = a_PortNo; }
	void SetMaxConnectNum(int a_MaxConnNum) { m_MaxConnNum = a_MaxConnNum; }

	bool start();
	bool pause();
	bool resume();
	void removeStopedThread();
	void notifyStopThread(GDSTcpThread *thread);
	void notifyDeleteThread(GDSTcpThread *thread);
	bool EmptyStartedThread();

	bool m_started;

protected:
    void incomingConnection(int socketDescriptor);

private:
	bool stop();

	unsigned short		m_PortNo;
	int					m_MaxConnNum;

	QList <QObject *>	m_StartedThreadList;
	QList <QObject *>	m_StopedThreadList;
	GDSTcpServerStatus	m_status;
};

#endif
