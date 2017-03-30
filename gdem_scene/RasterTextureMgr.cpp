#include "RasterTextureMgr.h"
#include <QtGui/QImage>

#define GDM_GIS_TEXTURE_SAME_THREAD

RasterTextureMgr* g_pRasterTextureMgr = NULL;

RasterTextureMgr::RasterTextureMgr()
{
	_Driver = 0;
	g_pRasterTextureMgr = this;
}

RasterTextureMgr::~RasterTextureMgr(void)
{

}

RasterTextureMgr * gdm_GetRasterTextureMgr(){return g_pRasterTextureMgr;}

long RasterTextureMgr::AddTexture(int nX, int nY, int nLevel, void* pData)
{
	int index;

	char textureName[256] = {0,};

	sprintf(textureName, "Raster_%d_%d_%u", nX, nY, nLevel);

	geo::video::ITexture* texture = 0;

	mutex.lock();
	QImage* qimage = (QImage*)pData;

	geo::video::IImage *image = _Driver->createImageFromData(geo::video::ECF_A8R8G8B8, geo::core::dimension2di(qimage->width(), qimage->height()), qimage->bits(), true, true);
	if(!image)		return 0;

	texture = _Driver->addTexture(textureName, image);

	index = _DelList.linear_reverse_search(texture);

	if(index != -1)		_DelList.erase(index, 1);

	// modify data
	mutex.unlock();

	return (unsigned long)texture;
}

void RasterTextureMgr::DelTexture(unsigned long TextureID)
{
	geo::video::ITexture* texture = (geo::video::ITexture*)TextureID;

	// TODO
	mutex.lock();
	// add texture to deleted texture list 
	_DelList.push_back(texture);
	mutex.unlock();
}

// delete all textures in deleted texture list
void RasterTextureMgr::DelTextureList()
{		
	geo::video::ITexture* tex;
	// TODO
	mutex.lock();	
	for(geo::u32 i = 0;i < _DelList.size();i++)
	{
		tex = (geo::video::ITexture*)_DelList[i];
	    qDebug("%d\n",tex);
		_Driver->removeTexture(tex);
		//tex->drop();
	}		
	_DelList.set_used(0);
	mutex.unlock();
}


void* RasterTextureMgr::GetTexture(unsigned long TextureID)
{
	if(TextureID == 0)
		return NULL;

	geo::video::ITexture* texture = (geo::video::ITexture*)TextureID;
	if(texture->validateTexture())
		return texture;
	return NULL;
}

