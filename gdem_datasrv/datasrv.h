#ifndef DATASRV_H
#define DATASRV_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#include "dataconfig.h"
#include "gdem_datasrv_global.h"

#include "../gdem_common/gdem_protocol.h"

#include <QList>
#include <QMutex>
#include <QObject>
#include <QBasicTimer>

class DataChannel;


class GDEM_DATASRV_EXPORT DataSrv:public QObject
{
public:
	DataSrv ();
	~DataSrv ();

	bool Init ();
	void Release ();
	void SetServer (char *addr, port_t port);

	bool GetConfig_State ()
	{
		return m_bConfigured;
	}

	void SetConfig_State (bool stat)
	{
		m_bConfigured = stat;
	}

	void ReleaseConfig ()
	{
		m_bConfigured = false;
	}


private:
	bool m_bConfigured;

	void        FinishThread();

protected:
	DataConfig *m_pconfThread;

public:
	char m_addr[DS_ADDR_LEN];
	port_t m_port;
	QList < DataChannel * >chan_list;
	QMutex mutex;
};

#endif // DATASRV_H
