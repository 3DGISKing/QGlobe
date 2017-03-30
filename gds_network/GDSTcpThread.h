// GDSTcpThread.h

#ifndef GDSTCPTHREAD_H
#define GDSTCPTHREAD_H

#include <QThread>
#include <QTcpSocket>


class GDSTcpServer;

class GDSTcpThread : public QThread
{
    Q_OBJECT

public:
    GDSTcpThread(int socketDescriptor, QObject *parent);
    ~GDSTcpThread();

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
	GDSTcpServer *m_tcpServer;
	int m_socketDescriptor;
	char *m_data;
};

#endif
