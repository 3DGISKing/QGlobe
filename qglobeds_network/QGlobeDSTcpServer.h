// QGlobeDSTcpServer.h

#ifndef QGlobeDSTCPSERVER_H
#define QGlobeDSTCPSERVER_H

#include <QTcpServer>


class QGlobeDSTcpThread;

enum QGlobeDSTcpServerStatus
{
	SERVER_STOPPED,
	SERVER_STARTED,
	SERVER_PAUSED
};

class QGlobeDSTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    QGlobeDSTcpServer(QObject *parent = 0);
    ~QGlobeDSTcpServer();

	void SetPortNo(unsigned short a_PortNo) { m_PortNo = a_PortNo; }
	void SetMaxConnectNum(int a_MaxConnNum) { m_MaxConnNum = a_MaxConnNum; }

	bool start();
	bool pause();
	bool resume();
	void removeStopedThread();
	void notifyStopThread(QGlobeDSTcpThread *thread);
	void notifyDeleteThread(QGlobeDSTcpThread *thread);
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
	QGlobeDSTcpServerStatus	m_status;
};

#endif
