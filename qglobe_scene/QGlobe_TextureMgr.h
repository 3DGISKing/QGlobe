#ifndef _QGlobe_TEXTUREMGR_H_
#define _QGlobe_TEXTUREMGR_H_

#include <QtCore/QMutex>

#include "geoEngine.h"
#include "qglobe_terrainengine_global.h"

class QGLOBE_TERRAINENGINE_EXPORT CQGlobe_TextureMgr
{
public:
	CQGlobe_TextureMgr(void);
	~CQGlobe_TextureMgr(void);

	long	AddTexture(int nX, int nY, int nLevel, void* jpgBuff, int bufSize);
	void	DelTexture(unsigned long TextureID);
	void*	GetTexture(unsigned long TextureID);
	void    DelTextureList();
	void    SetVideoDriver(geo::video::IVideoDriver *driver) { _Driver = driver; }

private:
	geo::video::IVideoDriver                *_Driver;
	geo::core::array<geo::video::ITexture*>	 _DelList;
};

extern QMutex g_TextureListMutex;
CQGlobe_TextureMgr * qglobe_GetTextureMgr();

#endif
