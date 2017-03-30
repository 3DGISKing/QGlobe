#ifndef _RASTER_TEXTUREMGR_H_
#define _RASTER_TEXTUREMGR_H_

#include <QMutex>
#include "geoEngine.h"

#include "gdem_terrainengine_global.h"

//#define _DEFAULT_TRANSPARENT_TEXTURE_SUPPORT_

class QGLWidget;

class GDEM_TERRAINENGINE_EXPORT RasterTextureMgr 
{
public:
	RasterTextureMgr();
	~RasterTextureMgr(void);

	void SetVideoDriver(geo::video::IVideoDriver *driver) { _Driver = driver; }

#ifdef _DEFAULT_TRANSPARENT_TEXTURE_SUPPORT_
	static bool createTransparentTexture(geo::video::IVideoDriver* driver);
	static long getTransparentTextureID() { return (long)_TransparentTexture; }
#endif

private:
	QMutex mutex;
	geo::video::IVideoDriver *_Driver;
	geo::core::array<geo::video::ITexture*>	 _DelList;

#ifdef _DEFAULT_TRANSPARENT_TEXTURE_SUPPORT_
	static geo::video::ITexture*	 _TransparentTexture;
#endif

public:

	long	AddTexture(int nX, int nY, int nLevel, void* pData);
	void	DelTexture(unsigned long TextureID);
	void*	GetTexture(unsigned long TextureID);
	void    DelTextureList();
};

RasterTextureMgr * gdm_GetRasterTextureMgr();

#endif //_GIS_TEXTUREMGR_H_
