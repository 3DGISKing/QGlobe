#ifndef DATACHANNEL_H
#define DATACLIENT_H

#include "common.h"
#include "../gdem_common/gdem_config.h"

#include "../gdem_common/gdem_distributeconfig.h"

#include <QObject>

class QTcpSocket;

typedef unsigned short path_t;

typedef struct tagDISTRIBUTE_PATH
{
	ST_DISTRIBUTE_PC conf;
	QTcpSocket* path;
} ST_DISTRIBUTE_PATH;


typedef QMap<unsigned int, ST_DISTRIBUTE_PATH> DistPathMap;

class GDEM_DATASRV_EXPORT DataChannel: public QObject
{
public:
	DataChannel();
	~DataChannel();
	
	void Init(const char *host, quint16 port);
	
	/* connnection related */
	bool isConnected(path_t path_id);
	bool connectToServer(path_t path_id);
	void disconnectFromServer(path_t path_id);
	int sendData(path_t path_id, void *send_buff, qint64 length);
	int receiveData(path_t path_id, void *rec_buff, qint64 length);
	
	/* path related */
	const ST_DISTRIBUTE_PATH* GetPathInfo(path_t path_id);
	bool CreatePath(path_t path_id, const char *host, quint16 port);
	bool DeletePath(path_t path_id);
	void DeletePathAll(void);

	/* media related */
	bool CreateMedia (path_t path_id);
	bool DeleteMedia (path_t path_id);

	/* status related */
	E_CHANNEL_STATUS_NO GetStatus (void);
	void SetStatus (E_CHANNEL_STATUS_NO status);
	void SetMaxWaitCount(int count){m_nMaxWaitFailCount = count;}

	/* data related */
	int  RequestData_block(void *in_buff, void *o_buff, unsigned int o_buff_size);

signals:
	void error(int socketError, const QString &message);

private slots:
	void onConnected();
	void onDisconnected();
	void onDisconnectedByServer();
	void onError(int socketError);

private:
	bool m_isUsed;
	int	 m_nConnectionFailCount;
	int  m_nMaxWaitFailCount;
	
public:
	E_CHANNEL_STATUS_NO m_status;
	DistPathMap m_PathMap;
	
protected:
};
#endif // DATACHANNEL_H
