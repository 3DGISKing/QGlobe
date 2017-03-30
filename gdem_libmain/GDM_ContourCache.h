#pragma once

#include "GDM_CacheDataMgr.h"
#include "gis_contour.h"

class GDEM_LIBMAIN_EXPORT CGDM_ContourCache :public CGDM_CacheDataMgr
{
public:
	CGDM_ContourCache(void);
	~CGDM_ContourCache(void);

public:
	GDALContour *GetContourData(int nX, int nY, int nLevel, short* pHeightData, int nInterval);
protected:
	virtual void	FreeData(void *pData);
};
