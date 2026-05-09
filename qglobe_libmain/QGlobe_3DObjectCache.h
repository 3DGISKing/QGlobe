#ifndef QGlobe_3DOBJECTCACHE_H
#define QGlobe_3DOBJECTCACHE_H

#include "QGlobe_CacheDataMgr.h"
#include "qglobe_protocol.h"

#include <QtCore/QMutex>

class CQGlobe_3DObjectCache : public CQGlobe_CacheDataMgr
{
public:
	CQGlobe_3DObjectCache();
	~CQGlobe_3DObjectCache();

	void*     GetMeshs(int nX, int nY, int nLevel, char* pRaw);

	void      AddMeshEntry(int x,int y,int level,void *);
	void*     GetMeshEntry(int x,int y,int level);
	void      DeleteMeshEntry(int x,int y,int level);

protected:
	virtual void FreeData(void *pData);
};

extern QMutex g_3dCacheMutex;

#endif