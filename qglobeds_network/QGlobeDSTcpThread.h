// QGlobeDSTcpThread.h

#ifndef QGlobeDSTCPTHREAD_H
#define QGlobeDSTCPTHREAD_H

#include <QThread>
#include <QTcpSocket>


class QGlobeDSTcpServer;

class QGlobeDSTcpThread : public QThread
{
    Q_OBJECT

public:
    QGlobeDSTcpThread(int socketDescriptor, QObject *parent);
    ~QGlobeDSTcpThread();

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
	QGlobeDSTcpServer *m_tcpServer;
	int m_socketDescriptor;
	char *m_data;
};

#endif
