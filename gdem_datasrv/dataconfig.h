#ifndef DATACONFIG_H
#define DATACONFIG_H

#include "common.h"

#include "../gdem_common/gdem_protocol.h"

#include <QObject>
#include <QThread>
#include <QMutex>

class GDEM_DATASRV_EXPORT DataConfig: public QThread
{
public:
	DataConfig();
	~DataConfig();
	
	bool ServerConfig (const char *addr, port_t port);
        void Sleep(unsigned long time) {msleep(time);}
        void    Finsish() {m_blFinish = true;}

protected:
     virtual void	run();
	 
private:
	int					m_nBufferSize;
	ST_PACKET_HEADER *	m_pRequest;
	ST_PACKET_HEADER *	m_pResponse;
	QMutex	mutex;

        bool        m_blFinish;
};
#endif // DATACONFIG_H
