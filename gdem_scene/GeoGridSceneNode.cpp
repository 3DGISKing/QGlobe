
#include "GeoGridSceneNode.h"
#include "ISceneManager.h"
#include "ISceneCollisionManager.h"
#include "IVideoDriver.h"
#include "SColor.h"
#include "vector3d.h"
#include "GDM_Mem.h"
#include "GLee.h"
//#include <QString>

extern int	gdm_CallBackGetGridVtx(void* pUserData, void *param1, void *param2, void *param3);

namespace geo
{
	namespace scene
	{
		// constructor
		CGeoGridSceneNode::CGeoGridSceneNode(ISceneNode* parent, ISceneManager* mgr, 
			s32 id, const core::vector3df& position,
			const core::vector3df& rotation, const core::vector3df& scale)
			: ISceneNode(parent, mgr, id, position)
		{
#ifdef _DEBUG
			setDebugName("CGeoGridSceneNode");
#endif
			setAutomaticCulling(scene::EAC_OFF);

			//InitFonts();
			UserData = NULL;
		}

		CGeoGridSceneNode::~CGeoGridSceneNode()
		{
			//DestroyFonts();
		}

		//! pre render event
		void CGeoGridSceneNode::OnRegisterSceneNode()
		{
			if (IsVisible)
				SceneManager->registerNodeForRendering(this);

			ISceneNode::OnRegisterSceneNode();
		}


		//! render
		void CGeoGridSceneNode::render()
		{			
			video::IVideoDriver* driver = SceneManager->getVideoDriver();

			ICameraSceneNode* camera = SceneManager->getActiveCamera();
			if(!camera)
				return;

			ISceneCollisionManager* collMgr = SceneManager->getSceneCollisionManager();
			if(!collMgr)
				return;
	
			geo::video::SColor gridColor(192, 192, 192, 192);
			geo::video::SColor parallelColor(192, 255, 255, 0);
			
			
			mat.Lighting = false;
			mat.ZBuffer = true;
			mat.Thickness = 1;
			mat.Wireframe = true;
			driver->setMaterial(mat);
	
			driver->setTransform (video::ETS_WORLD, core::IdentityMatrix);

			s32 index, i;
			// call CALL-BACK functions
			gdm_CallBackGetGridVtx(NULL, &m_gridArray, &m_gridTropicArray, NULL);
			
			for(index = 0 ; index < (s32)m_gridArray.size() ; index++)
			{
				geo::core::array<geo::core::vector3df>* pArray = &m_gridArray[index].gridLineArray;
				if( pArray && pArray->size() != 0 )
				{
					for(i = 0; i < (s32)(pArray->size() - 1); i++)
					{
						driver->draw3DLine((*pArray)[i], (*pArray)[i+1], gridColor);
					}
				}
				pArray->clear();
			}
			m_gridArray.clear();				

			mat.Thickness = 2;
			driver->setMaterial(mat);
		
			for(index = 0 ; index < (s32)m_gridTropicArray.size() ; index++)
			{
				geo::core::array<geo::core::vector3df>* pArray = &m_gridTropicArray[index].gridLineArray;
				if( pArray && pArray->size() != 0 )
				{
					for(i = 0; i < (s32)(pArray->size() - 1); i++)
					{
						driver->draw3DLine((*pArray)[i], (*pArray)[i+1], parallelColor);
					}
				}
				pArray->clear();
			}
			m_gridTropicArray.clear();			
		}
		
		//! returns the axis aligned bounding box of this node
		const core::aabbox3d<f32>& CGeoGridSceneNode::getBoundingBox() const
		{
			return Box;
		}

	} // end namespace scene
} // end namespace geo
