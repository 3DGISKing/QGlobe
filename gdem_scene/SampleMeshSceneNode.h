
#ifndef SAMPLEMESHSCENENODE_H
#define SAMPLEMESHSCENENODE_H

#include "ISceneNode.h"
#include "SMesh.h"
#include "../gdem_base/GDM_Coord3D.h"
namespace geo
{
	namespace scene
	{
		class CSampleMeshSceneNode :public ISceneNode
		{
		public:
			//! constructor
			CSampleMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0,0,0));

			//! destructor
			virtual ~CSampleMeshSceneNode();

			virtual void OnRegisterSceneNode();

			//! renders the node.

			virtual void render();

			//! returns the axis aligned bounding box of this node
			virtual const core::aabbox3d<f32>& getBoundingBox() const;

			void setLocalMesh(IMesh* mesh); 
			IMesh* getWorldMesh(){return WorldMesh;}

			void clear();
			
			//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
			/* In this way it is possible to change the materials a mesh causing all mesh scene nodes 
			referencing this mesh to change too. */
			virtual void setReadOnlyMaterials(bool readonly) {}
			
			//! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
			virtual bool isReadOnlyMaterials() const { return false; }

			void EnableLighting(bool enable =true);

			/*******************************
			Ì»´Å°á°£Ëæº· ËË Êï¹¶½¤ÀâÌ© °ê¹¢»ô¶®Áì±Ë
			********************************/
			GDM_Coord3D      Coord;

			/*******************************
			SourceMesh¶¦ Ì»´Å°á°£Ëæµá ²¿±¨²÷ 4*4ÃÔµÛ
			********************************/
			CGDM_Matrix4     Transform;
		private :
	
			/*******************************
			¶Ã³ó¶µÂ×²÷´ç Â­ÊýÂÙ ·ô»Õ
			SourceMeshÌ® ¼³¼°±¶ºã,ËÍ´èº÷Ì© ±¶ºãÌ® °ªËË Ì°¼­ÃÅ ËÎ¾·ÂÙ³Þ.
			¶Ã³ó¶µÂ×±¨¼­Ëæ ËË ·ô»ÕÌ© ¼³¼«»ô¶®Áì´ÉË¼ Ì»´Å°á°£Ëæµá ¸êÃû´ô³Þ.
			********************************/
			IMesh* WorldMesh;

			/*******************************
			°ê¹¢»ô¶®Áì±ËËæº· ¼ÑÊÌ¼ê Ìº¶· »¶»ôµêµáº·Ì© ·ô»Õ
			ËË°ÃË¼ Mesh¶¦ Ì»´Å°á°£Ëæµá ²¿±¨±¨ Ì¡ÃÍ Â­ÊýÂ×³Þ.
			********************************/
			IMesh* LocalMesh;

			void constructMesh();
			void renderCoord();

			bool LightingEnabled;
		};
		
	}
}

#endif
