#include "GIS_TextureMgr.h"

#include <QtGui/QImage>
#include "GDM_TextureMgr.h"

#define GDM_GIS_TEXTURE_SAME_THREAD

CGIS_TextureMgr* g_pGisTextureMgr = NULL;
CGIS_TextureMgr * gdm_GetGISTextureMgr(){return g_pGisTextureMgr;}

CGIS_TextureMgr::CGIS_TextureMgr(void)
{
	_Driver = 0;
	g_pGisTextureMgr = this;
}

CGIS_TextureMgr::~CGIS_TextureMgr(void)
{

}

long CGIS_TextureMgr::AddTexture(int nX, int nY, int nLevel, void* pData)
{
	int index;

	if(!_Driver)
		return 0;

	char textureName[256] = {0,};
	sprintf(textureName, "GIS_%d_%d_%u", nX, nY, nLevel);

	geo::video::ITexture* texture = 0;

	g_TextureListMutex.lock();

	QImage* qimage = (QImage*)pData;
	geo::video::IImage *image = _Driver->createImageFromData(geo::video::ECF_A8R8G8B8, geo::core::dimension2di(qimage->width(), qimage->height()), qimage->bits(), false, true);
	if(!image)
		return 0;

	texture = _Driver->addTexture(textureName, image);

	image->drop();

	index = _DelList.linear_reverse_search(texture);

	if(index != -1)
		_DelList.erase(index, 1);

	g_TextureListMutex.unlock();

	return (unsigned long)texture;
}

void CGIS_TextureMgr::DelTexture(unsigned long TextureID)
{
	geo::video::ITexture* texture = (geo::video::ITexture*)TextureID;

	g_TextureListMutex.lock();
#ifdef GDM_GIS_TEXTURE_SAME_THREAD
	_Driver->removeTexture(texture);
#else

	// TODO
	mutex.lock();
	// add texture to deleted texture list 
	_DelList.push_back(texture);
	mutex.unlock();
#endif
	g_TextureListMutex.unlock();
}

// delete all textures in deleted texture list
void CGIS_TextureMgr::DelTextureList()
{		
#ifndef GDM_GIS_TEXTURE_SAME_THREAD
	geo::video::ITexture* tex;
	// TODO
	mutex.lock();	
	for(geo::u32 i = 0;i < _DelList.size();i++){
		tex = (geo::video::ITexture*)_DelList[i];
		_Driver->removeTexture(tex);
		//tex->drop();
	}		
	_DelList.set_used(0);
	mutex.unlock();
#endif
}


void* CGIS_TextureMgr::GetTexture(unsigned long TextureID)
{
	if(TextureID == 0)
#ifdef _DEFAULT_TRANSPARENT_TEXTURE_SUPPORT_
		return _TransparentTexture;
#else
		return NULL;
#endif

	geo::video::ITexture* texture = (geo::video::ITexture*)TextureID;
	if(texture->validateTexture())
		return texture;
	return NULL;
}

#ifdef _DEFAULT_TRANSPARENT_TEXTURE_SUPPORT_
bool CGIS_TextureMgr::createTransparentTexture(geo::video::IVideoDriver* driver)
{
	if(!driver)
		return false;

	char textureName[256] = "GIS_Overlay_Transparent_Texture";
	unsigned long* pBuf[4] = {0, };

	geo::video::ITexture* texture = 0;
	geo::video::IImage *image = driver->createImageFromData(geo::video::ECF_A8R8G8B8, geo::core::dimension2di(2, 2), pBuf, true, true);
	if(!image)
		return 0;

	_TransparentTexture = driver->addTexture(textureName, image);
	if(_TransparentTexture)
		return true;

	return false;
}
#endif