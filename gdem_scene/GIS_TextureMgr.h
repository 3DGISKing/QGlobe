#ifndef _GIS_TEXTUREMGR_H_
#define _GIS_TEXTUREMGR_H_

#include "geoEngine.h"
#include "gdem_terrainengine_global.h"

class GDEM_TERRAINENGINE_EXPORT CGIS_TextureMgr 
{
public:
	CGIS_TextureMgr(void);
	~CGIS_TextureMgr(void);

	void    SetVideoDriver(geo::video::IVideoDriver *driver) { _Driver = driver; }
	long	AddTexture(int nX, int nY, int nLevel, void* pData);
	void	DelTexture(unsigned long TextureID);
	void*	GetTexture(unsigned long TextureID);
	void    DelTextureList();

private:
	geo::video::IVideoDriver *_Driver;
	geo::core::array<geo::video::ITexture*>	 _DelList;
};

CGIS_TextureMgr * gdm_GetGISTextureMgr();

#endif //_GIS_TEXTUREMGR_H_
