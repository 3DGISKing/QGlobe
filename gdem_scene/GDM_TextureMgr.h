#ifndef _GDM_TEXTUREMGR_H_
#define _GDM_TEXTUREMGR_H_

#include <QtCore/QMutex>

#include "geoEngine.h"
#include "gdem_terrainengine_global.h"

class GDEM_TERRAINENGINE_EXPORT CGDM_TextureMgr
{
public:
	CGDM_TextureMgr(void);
	~CGDM_TextureMgr(void);

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
CGDM_TextureMgr * gdm_GetTextureMgr();

#endif
