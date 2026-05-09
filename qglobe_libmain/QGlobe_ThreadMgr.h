#pragma once

#include <QtCore/QThread>

#include "qglobe_protocol.h"
#include "qglobe_libmain_global.h"

typedef void (QGlobe_ThreadAPI)(void*);
QGlobe_ThreadAPI qglobe_DataLoadThread;

// create thread
void qglobe_LibMainThreadInit(void);
void qglobe_KillAllThread(void);

// loading data thread for DEM, IMG, SHAPE, NAME
class QGLOBE_LIBMAIN_EXPORT CQGlobe_LoadThread : public QThread
{
public:
	typedef enum 
	{
		QGlobe_THREAD_ACTIVE,
		QGlobe_THREAD_START_KILL,
		QGlobe_THREAD_STOPED,
	}E_QGlobe_THREAD_STATE;

	void				        Init( E_QGlobe_MAINDATA_TYPE type, QGlobe_ThreadAPI *handleFunc);
	inline void				    Sleep(int vMillisecond ){ this->msleep(vMillisecond); }
	inline E_QGlobe_MAINDATA_TYPE	GetDataType() {return m_eType;}
	void				        SetState(E_QGlobe_THREAD_STATE state);
	int					        IsActive();
	int					        IsStoped();
protected:
    virtual void	            run();

private:
	E_QGlobe_MAINDATA_TYPE	        m_eType;		// the thread data type
	E_QGlobe_THREAD_STATE	        m_eState;		// the state of thread

	QGlobe_ThreadAPI		        *handleThread;	// thread call function
};

