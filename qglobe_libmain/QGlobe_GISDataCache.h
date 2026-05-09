#ifndef QGlobe_GISDATACACHE_H
#define QGlobe_GISDATACACHE_H

#include "QGlobe_CacheDataMgr.h"
#include "../qglobe_common/qglobe_protocol.h"

class GISLayer;


class CQGlobe_GISDataCache: public CQGlobe_CacheDataMgr
{
public:

	~CQGlobe_GISDataCache();
	GISLayer* GetLayer(int nX, int nY, int nLevel, E_QGlobe_SUBDATA_TYPE type);
	void      AddLayer(int x,int y,int level,E_QGlobe_SUBDATA_TYPE type,GISLayer* layer);

protected:
	virtual void FreeData(void *layer);
};


#endif