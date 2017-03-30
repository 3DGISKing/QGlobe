#ifndef GDM_GISDATACACHE_H
#define GDM_GISDATACACHE_H

#include "GDM_CacheDataMgr.h"
#include "../gdem_common/gdem_protocol.h"

class GISLayer;


class CGDM_GISDataCache: public CGDM_CacheDataMgr
{
public:

	~CGDM_GISDataCache();
	GISLayer* GetLayer(int nX, int nY, int nLevel, E_GDM_SUBDATA_TYPE type);
	void      AddLayer(int x,int y,int level,E_GDM_SUBDATA_TYPE type,GISLayer* layer);

protected:
	virtual void FreeData(void *layer);
};


#endif