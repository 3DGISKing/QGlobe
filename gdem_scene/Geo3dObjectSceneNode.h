#ifndef GEO3DOBJECTSCENENODE_H
#define GEO3DOBJECTSCENENODE_H

#include <QList>

#include "ISceneNode.h"
#include "SGeoMesh.h"
#include "../gdem_base/GDM_Coord3D.h"
#include "../gdem_terrainengine/SMeshBuffer.h"

class SimpleBuilding;
extern qint64 g_Focused3dObjectID;
namespace geo
{
	namespace scene
	{
		//extern void CalculateScreenRect(SGeoMesh* mesh,core::rect<s32>& rect );

		class CGeo3dObjectSceneNode :public ISceneNode
		{
		public:
			//! constructor
			CGeo3dObjectSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0,0,0));

			//! destructor
			virtual ~CGeo3dObjectSceneNode();

			virtual void OnRegisterSceneNode();

			//! renders the node.

			virtual void render();

			//! returns the axis aligned bounding box of this node
			virtual const core::aabbox3d<f32>& getBoundingBox() const;

			//! Returns the current mesh
			virtual IMesh* getMesh() { return Mesh; }
			
			//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
			/* In this way it is possible to change the materials a mesh causing all mesh scene nodes 
			referencing this mesh to change too. */
			virtual void setReadOnlyMaterials(bool readonly) {}
			
			//! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
			virtual bool isReadOnlyMaterials() const { return false; }

			void TransformForRender(SMeshBuffer* mesh);
			void DrawBoundingBox(SMesh *meshbuffer);
			void SetSelectedID(qint64 id){SelectedID=id;}
			int  GetSelectedID()      {return SelectedID;}
			void ClearSerachedIDs()	  {SearchedIDs.clear();}
			QList<qint64> & GetSearchedIds(){return SearchedIDs;} 

			inline void SetNewBuilding(SimpleBuilding* p){NewBuilding=p;}
			inline SimpleBuilding* GetNewBuilding(){return NewBuilding;}

			void   EnableLighting(bool enable=true);
		private :
			SMeshBuffer* RenderBuffer;
			void simpleBuildingRender();
			void newBuildingRender();

			/*******************************
			RenderBuffer¶¦ Ì»´Å°á°£Ëæµá ²¿±¨²÷ 4*4ÃÔµÛ
			********************************/
			CGDM_Matrix4     Transform;

			//ºåºã°¡º¬ÂÜºã¶¦ ¼õ¼³Ì©Â×±¨ Ì¡Â×ÊÞ ÂÚºã ÊÖËË ººÊÎÂÙ ¸êºã 
			IMesh *          Mesh;

			video::SMaterial mat;
			video::SColor	 SideColor;
			video::SColor    TopColor;
			video::SColor    TopOutLineColor;
			video::SColor    SideOutLineColor;
			video::SColor    FloorOutLineColor;
			video::SColor    WaterColor;
			video::SColor    SelectColor;
			video::SColor    SearchedColor;
			qint64           SelectedID;

			core::array<core::vector3df> TopOutline;
			float            FloorFactor;
			QList<qint64>       SearchedIDs;
			QList<qint64>       RenderedIDs;

			SimpleBuilding  *NewBuilding;

			bool             LightingEnabled;

		};
	}
}

#endif
