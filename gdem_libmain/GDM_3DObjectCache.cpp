#include "GDM_3DObjectCache.h"

#include "GDM_Tile.h"                     //for using MAX_TILE_LEVEL
#include "../gdem_render/GeoMeshUtil.h"  //for using GeoMeshUtil
#include "ITexture.h"
#include "../gdem_scene/GDM_SceneManager.h"

#define MAKE_CACHE_KEY1(level, x, y)	((level << MAX_TILE_LEVEL) | x)
#define MAKE_CACHE_KEY2(level, x, y)	((level << MAX_TILE_LEVEL) | y)
 
QMutex g_3dCacheMutex;

CGDM_3DObjectCache::CGDM_3DObjectCache()
{
}

CGDM_3DObjectCache::~CGDM_3DObjectCache()
{
	Clear();
}

void* CGDM_3DObjectCache::GetMeshs(int x, int y, int level,char* pRaw)
{
	void* meshs=GetMeshEntry(x,y,level);

	if(meshs!=NULL)
		return meshs;

	meshs=GeoMeshUtil::CreateGeoMeshsFromData(pRaw);

	AddMeshEntry(x,y,level,meshs);		

	return meshs;
}

void CGDM_3DObjectCache::AddMeshEntry(int x, int y, int level, void *meshs)
{
	Q_ASSERT(meshs!=NULL);

	if(meshs==NULL) return;

	int type=(int)E_GDM_SUBDATA_TYPE_3D_BUILDING;

	int key1 = MAKE_CACHE_KEY1(level, x, y);
	int key2 = MAKE_CACHE_KEY2(type, x, y);

	AddEntry(key1,key2,meshs);
}

void *CGDM_3DObjectCache::GetMeshEntry(int x,int y,int level)
{
	int type=(int)E_GDM_SUBDATA_TYPE_3D_BUILDING;

	int key1 = MAKE_CACHE_KEY1(level, x, y);
	int key2 = MAKE_CACHE_KEY2(type, x, y);

	return GetEntry(key1,key2);
}

void CGDM_3DObjectCache::DeleteMeshEntry(int x, int y, int level)
{
	int type=(int)E_GDM_SUBDATA_TYPE_3D_BUILDING;

	int key1 = MAKE_CACHE_KEY1(level, x, y);
	int key2 = MAKE_CACHE_KEY2(type, x, y);

	void * data=GetCacheEntry(key1,key2);
	DeleteEntry(data);
}

void CGDM_3DObjectCache::FreeData(void *pData)
{
   SAnimatedMesh* meshs=(SAnimatedMesh*)pData;

   ITexture* texture=NULL;
	
   for (u32 i=0; i<meshs->getFrameCount(); i++)
   {
	    IMesh* mesh=meshs->Meshes[i];

		for(u32 j=0;j<mesh->getMeshBufferCount();j++)
		{
			IMeshBuffer* meshbuffer=mesh->getMeshBuffer(j);

			texture=meshbuffer->getMaterial().TextureLayer[0].Texture;
			
			if(texture)
			{
				texture->drop();

				if(texture->getReferenceCount()==1)
				{
					/*this texture is no longer neccessary
					  but exists in driver 's texture array

					 following code snip is run in this thread!
					 it is very dangerous*/

					//gdm_GetSceneManager()->VideoDriver()->removeTexture(texture);
				}
			}
		}
   }

   meshs->drop();
}
