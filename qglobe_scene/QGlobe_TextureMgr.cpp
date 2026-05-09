#include "QGlobe_TextureMgr.h"

CQGlobe_TextureMgr* g_pQGlobeTextureMgr;
QMutex           g_TextureListMutex;

CQGlobe_TextureMgr * qglobe_GetTextureMgr(){return g_pQGlobeTextureMgr;}

CQGlobe_TextureMgr::CQGlobe_TextureMgr(void)
{
	_Driver = 0;
	g_pQGlobeTextureMgr = this;
}

CQGlobe_TextureMgr::~CQGlobe_TextureMgr(void)
{
}

long CQGlobe_TextureMgr::AddTexture(int nX, int nY, int nLevel, void* jpgBuff, int bufSize)
{
	int index;
	// TODO
	if(!_Driver)
		return 0;

	char textureName[256] = {0,};
	sprintf_s(textureName, "%d_%d_%u", nX, nY, nLevel);

	geo::video::ITexture* texture = 0;
	
	g_TextureListMutex.lock();

    texture = _Driver->getTexture(jpgBuff, bufSize, (geo::s8*)".jpg", (geo::s8*)textureName);

	g_TextureListMutex.unlock();

	index = _DelList.linear_reverse_search(texture);

	if(index != -1)
		_DelList.erase(index, 1);

	return (unsigned long)texture;
}

void CQGlobe_TextureMgr::DelTexture(unsigned long TextureID)
{
	geo::video::ITexture* texture = (geo::video::ITexture*)TextureID;

	// add texture to deleted texture list 
	_DelList.push_back(texture);
}

// delete all textures in deleted texture list
void CQGlobe_TextureMgr::DelTextureList()
{	
	geo::video::ITexture* tex;

	g_TextureListMutex.lock();	

	for(geo::u32 i = 0;i < _DelList.size();i++)
	{
		tex = (geo::video::ITexture*)_DelList[i];
		_Driver->removeTexture(tex);
		//tex->drop();
	}		
	_DelList.set_used(0);
	g_TextureListMutex.unlock();
}


void* CQGlobe_TextureMgr::GetTexture(unsigned long TextureID)
{
	if(TextureID == 0)
		return NULL;

	geo::video::ITexture* texture = (geo::video::ITexture*)TextureID;
	if(texture->validateTexture())
		return texture;
	return NULL;
}
