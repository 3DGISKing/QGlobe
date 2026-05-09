#include "QGlobe_ThreadMgr.h"

CQGlobe_LoadThread*	g_qglobe_ThreadList[E_QGlobe_MAINDATA_TYPE_MAX];
int					g_qglobe_ThreadCount;
void qglobe_WaitAllKillThread();


void qglobe_AddDataLoadThread(E_QGlobe_MAINDATA_TYPE type)
{
	g_qglobe_ThreadList[g_qglobe_ThreadCount] = new CQGlobe_LoadThread();
	g_qglobe_ThreadList[g_qglobe_ThreadCount]->Init(type, qglobe_DataLoadThread);
	g_qglobe_ThreadList[g_qglobe_ThreadCount]->start();
	g_qglobe_ThreadCount++;
}

// create thread
void qglobe_LibMainThreadInit(void)
{
	memset(g_qglobe_ThreadList, 0, sizeof(CQGlobe_LoadThread*) * E_QGlobe_MAINDATA_TYPE_MAX);

	// create Dataload thread for DEM
	qglobe_AddDataLoadThread(E_QGlobe_MAINDATA_TYPE_DEM);

	// create Dataload thread for Image
	qglobe_AddDataLoadThread(E_QGlobe_MAINDATA_TYPE_IMG);

	// create Dataload thread for Name
	qglobe_AddDataLoadThread(E_QGlobe_MAINDATA_TYPE_NAME);

	// create Dataload thread for shape
	qglobe_AddDataLoadThread(E_QGlobe_MAINDATA_TYPE_SHAPE);

	qglobe_AddDataLoadThread(E_QGlobe_MAINDATA_TYPE_3D);

	qglobe_AddDataLoadThread(E_QGlobe_MAINDATA_TYPE_RASTER);
}

void qglobe_KillAllThread(void)
{
	int i;

	for(i = 0; i < E_QGlobe_MAINDATA_TYPE_MAX; i++)
	{
		if(g_qglobe_ThreadList[i] != NULL)
		{
			g_qglobe_ThreadList[i]->SetState(CQGlobe_LoadThread::QGlobe_THREAD_START_KILL);
		}
	}

	// create check thread
	qglobe_WaitAllKillThread();
}

void CQGlobe_LoadThread::Init( E_QGlobe_MAINDATA_TYPE type, QGlobe_ThreadAPI *handleFunc)
{
	handleThread = handleFunc;

	// set type;
	m_eType = type;
	m_eState = QGlobe_THREAD_ACTIVE;
}

void CQGlobe_LoadThread::SetState(E_QGlobe_THREAD_STATE state)
{
	m_eState = state;
}

int CQGlobe_LoadThread::IsActive()
{
	int ret = 1;
	if(m_eState != QGlobe_THREAD_ACTIVE)
		ret = 0;
	return ret;
}

int CQGlobe_LoadThread::IsStoped()
{
	int ret = 1;
	
	if(m_eState != QGlobe_THREAD_STOPED)
		ret = 0;
	return ret;
}

void CQGlobe_LoadThread::run()
{
	try
	{
		handleThread(this);
	}
	catch(...)
	{
		// handle thread error
	}
}

void qglobe_WaitAllKillThread()
{
	while(1)
	{
		for(int i = 0; i < E_QGlobe_MAINDATA_TYPE_MAX; i++)
		{
			if(g_qglobe_ThreadList[i] != NULL)
			{
				if(g_qglobe_ThreadList[i]->IsStoped() && g_qglobe_ThreadList[i]->isFinished())
				{
					delete g_qglobe_ThreadList[i];
					g_qglobe_ThreadList[i] = NULL;
				}
				else
					g_qglobe_ThreadList[i]->Sleep(1);
			}
		}
		
		bool allkilled = true;
		
		for(int i = 0; i < E_QGlobe_MAINDATA_TYPE_MAX; i++)
		{
			if(g_qglobe_ThreadList[i] != NULL)
			{
				allkilled=false;
				break;
			}
		}


		if(allkilled)
			break;

		// sleep
	}
}
