#pragma once

#include <QtCore/QThread>

#include "gdem_protocol.h"
#include "gdem_libmain_global.h"

typedef void (GDM_ThreadAPI)(void*);
GDM_ThreadAPI gdm_DataLoadThread;

// create thread
void gdm_LibMainThreadInit(void);
void gdm_KillAllThread(void);

// loading data thread for DEM, IMG, SHAPE, NAME
class GDEM_LIBMAIN_EXPORT CGDM_LoadThread : public QThread
{
public:
	typedef enum 
	{
		GDM_THREAD_ACTIVE,
		GDM_THREAD_START_KILL,
		GDM_THREAD_STOPED,
	}E_GDM_THREAD_STATE;

	void				        Init( E_GDM_MAINDATA_TYPE type, GDM_ThreadAPI *handleFunc);
	inline void				    Sleep(int vMillisecond ){ this->msleep(vMillisecond); }
	inline E_GDM_MAINDATA_TYPE	GetDataType() {return m_eType;}
	void				        SetState(E_GDM_THREAD_STATE state);
	int					        IsActive();
	int					        IsStoped();
protected:
    virtual void	            run();

private:
	E_GDM_MAINDATA_TYPE	        m_eType;		// the thread data type
	E_GDM_THREAD_STATE	        m_eState;		// the state of thread

	GDM_ThreadAPI		        *handleThread;	// thread call function
};

