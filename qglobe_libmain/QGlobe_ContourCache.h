#pragma once

#include "QGlobe_CacheDataMgr.h"
#include "gis_contour.h"

class QGLOBE_LIBMAIN_EXPORT CQGlobe_ContourCache :public CQGlobe_CacheDataMgr
{
public:
	CQGlobe_ContourCache(void);
	~CQGlobe_ContourCache(void);

public:
	GDALContour *GetContourData(int nX, int nY, int nLevel, short* pHeightData, int nInterval);
protected:
	virtual void	FreeData(void *pData);
};
