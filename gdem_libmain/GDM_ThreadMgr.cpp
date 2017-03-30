#include "GDM_ThreadMgr.h"

CGDM_LoadThread*	g_gdm_ThreadList[E_GDM_MAINDATA_TYPE_MAX];
int					g_gdm_ThreadCount;
void gdem_WaitAllKillThread();


void gdm_AddDataLoadThread(E_GDM_MAINDATA_TYPE type)
{
	g_gdm_ThreadList[g_gdm_ThreadCount] = new CGDM_LoadThread();
	g_gdm_ThreadList[g_gdm_ThreadCount]->Init(type, gdm_DataLoadThread);
	g_gdm_ThreadList[g_gdm_ThreadCount]->start();
	g_gdm_ThreadCount++;
}

// create thread
void gdm_LibMainThreadInit(void)
{
	memset(g_gdm_ThreadList, 0, sizeof(CGDM_LoadThread*) * E_GDM_MAINDATA_TYPE_MAX);

	// create Dataload thread for DEM
	gdm_AddDataLoadThread(E_GDM_MAINDATA_TYPE_DEM);

	// create Dataload thread for Image
	gdm_AddDataLoadThread(E_GDM_MAINDATA_TYPE_IMG);

	// create Dataload thread for Name
	gdm_AddDataLoadThread(E_GDM_MAINDATA_TYPE_NAME);

	// create Dataload thread for shape
	gdm_AddDataLoadThread(E_GDM_MAINDATA_TYPE_SHAPE);

	gdm_AddDataLoadThread(E_GDM_MAINDATA_TYPE_3D);

	gdm_AddDataLoadThread(E_GDM_MAINDATA_TYPE_RASTER);
}

void gdm_KillAllThread(void)
{
	int i;

	for(i = 0; i < E_GDM_MAINDATA_TYPE_MAX; i++)
	{
		if(g_gdm_ThreadList[i] != NULL)
		{
			g_gdm_ThreadList[i]->SetState(CGDM_LoadThread::GDM_THREAD_START_KILL);
		}
	}

	// create check thread
	gdem_WaitAllKillThread();
}

void CGDM_LoadThread::Init( E_GDM_MAINDATA_TYPE type, GDM_ThreadAPI *handleFunc)
{
	handleThread = handleFunc;

	// set type;
	m_eType = type;
	m_eState = GDM_THREAD_ACTIVE;
}

void CGDM_LoadThread::SetState(E_GDM_THREAD_STATE state)
{
	m_eState = state;
}

int CGDM_LoadThread::IsActive()
{
	int ret = 1;
	if(m_eState != GDM_THREAD_ACTIVE)
		ret = 0;
	return ret;
}

int CGDM_LoadThread::IsStoped()
{
	int ret = 1;
	
	if(m_eState != GDM_THREAD_STOPED)
		ret = 0;
	return ret;
}

void CGDM_LoadThread::run()
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

void gdem_WaitAllKillThread()
{
	while(1)
	{
		for(int i = 0; i < E_GDM_MAINDATA_TYPE_MAX; i++)
		{
			if(g_gdm_ThreadList[i] != NULL)
			{
				if(g_gdm_ThreadList[i]->IsStoped() && g_gdm_ThreadList[i]->isFinished())
				{
					delete g_gdm_ThreadList[i];
					g_gdm_ThreadList[i] = NULL;
				}
				else
					g_gdm_ThreadList[i]->Sleep(1);
			}
		}
		
		bool allkilled = true;
		
		for(int i = 0; i < E_GDM_MAINDATA_TYPE_MAX; i++)
		{
			if(g_gdm_ThreadList[i] != NULL)
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
