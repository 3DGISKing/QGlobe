
#ifndef SAMPLEMESHSCENENODE_H
#define SAMPLEMESHSCENENODE_H

#include "ISceneNode.h"
#include "SMesh.h"
#include "../qglobe_base/QGlobe_Coord3D.h"
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
			̻�Űᰣ�溷 �� �ﹶ����̩ �깢�������
			********************************/
			QGlobe_Coord3D      Coord;

			/*******************************
			SourceMesh�� ̻�Űᰣ��� ������ 4*4�Ե�
			********************************/
			CQGlobe_Matrix4     Transform;
		private :
	
			/*******************************
			�ó��ײ��� ­���� ����
			SourceMesḫ ��������,�ʹ��̩ ����̮ ���� ̰���� �ξ��ٳ�.
			�ó��ױ����� �� ����̩ �����������˼ ̻�Űᰣ��� ��������.
			********************************/
			IMesh* WorldMesh;

			/*******************************
			�깢��������溷 ���̼� ̺�� ������ặ̩ ����
			�˰�˼ Mesh�� ̻�Űᰣ��� ������ ̡�� ­���׳�.
			********************************/
			IMesh* LocalMesh;

			void constructMesh();
			void renderCoord();

			bool LightingEnabled;
		};
		
	}
}

#endif
