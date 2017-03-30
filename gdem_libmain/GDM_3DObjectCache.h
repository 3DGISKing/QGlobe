#ifndef GDM_3DOBJECTCACHE_H
#define GDM_3DOBJECTCACHE_H

#include "GDM_CacheDataMgr.h"
#include "gdem_protocol.h"

#include <QtCore/QMutex>

class CGDM_3DObjectCache : public CGDM_CacheDataMgr
{
public:
	CGDM_3DObjectCache();
	~CGDM_3DObjectCache();

	void*     GetMeshs(int nX, int nY, int nLevel, char* pRaw);

	void      AddMeshEntry(int x,int y,int level,void *);
	void*     GetMeshEntry(int x,int y,int level);
	void      DeleteMeshEntry(int x,int y,int level);

protected:
	virtual void FreeData(void *pData);
};

extern QMutex g_3dCacheMutex;

#endif