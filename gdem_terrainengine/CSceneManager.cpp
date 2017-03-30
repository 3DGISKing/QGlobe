// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "geoCompileConfig.h"
#include "CSceneManager.h"
#include "IVideoDriver.h"
#include "IFileSystem.h"
#include "SAnimatedMesh.h"
#include "CMeshCache.h"
#include "IWriteFile.h"
#include "IXMLWriter.h"
#include "IMaterialRenderer.h"
#include "IReadFile.h"


#include "os.h"

#include "CGeometryCreator.h"

#include "CCubeSceneNode.h"
#include "CSphereSceneNode.h"
#include "CCameraSceneNode.h"

#include "CLightSceneNode.h"
#include "CMeshSceneNode.h"
#include "CSkyBoxSceneNode.h"
#include "CSkyDomeSceneNode.h"
#include "CEmptySceneNode.h"

#include "CSceneCollisionManager.h"
#include "CTriangleSelector.h"
#include "CTriangleBBSelector.h"
#include "CMetaTriangleSelector.h"

#include "CSceneNodeAnimatorRotation.h"
#include "CSceneNodeAnimatorFlyCircle.h"
#include "CSceneNodeAnimatorFlyStraight.h"
#include "CSceneNodeAnimatorTexture.h"
#include "CSceneNodeAnimatorCollisionResponse.h"
#include "CSceneNodeAnimatorDelete.h"
#include "CSceneNodeAnimatorFollowSpline.h"
#include "CSceneNodeAnimatorCameraFPS.h"
#include "CSceneNodeAnimatorCameraMaya.h"

#include "CVolumeLightSceneNode.h"

//! Enable debug features
#define SCENEMANAGER_DEBUG

namespace geo
{
namespace scene
{

//! constructor
CSceneManager::CSceneManager(video::IVideoDriver* driver, io::IFileSystem* fs,
		gui::ICursorControl* cursorControl, IMeshCache* cache)
: ISceneNode(0, 0), Driver(driver), FileSystem(fs), CursorControl(cursorControl), CollisionManager(0),
	ActiveCamera(0), ShadowColor(150,0,0,0), AmbientLight(0,0,0,0),
	MeshCache(cache), CurrentRendertime(ESNRP_COUNT)
{
	#ifdef _DEBUG
	ISceneManager::setDebugName("CSceneManager ISceneManager");
	ISceneNode::setDebugName("CSceneManager ISceneNode");
	#endif

	// root node's scene manager
	SceneManager = this;

	if (Driver)
		Driver->grab();

	if (FileSystem)
		FileSystem->grab();

	if (CursorControl)
		CursorControl->grab();

	// create mesh cache if not there already
	if (!MeshCache)
		MeshCache = new CMeshCache();
	else
		MeshCache->grab();

	// create collision manager
	CollisionManager = new CSceneCollisionManager(this, Driver);
}



//! destructor
CSceneManager::~CSceneManager()
{
	clearDeletionList();

	if (FileSystem)
		FileSystem->drop();

	if (CursorControl)
		CursorControl->drop();

	if (CollisionManager)
		CollisionManager->drop();

	if (ActiveCamera)
		ActiveCamera->drop();
	ActiveCamera = 0;

	if (MeshCache)
		MeshCache->drop();

	// remove all nodes and animators before dropping the driver
	// as render targets may be destroyed twice
	removeAll();
	removeAnimators();

	if (Driver)
		Driver->drop();
}

//! returns the video driver
video::IVideoDriver* CSceneManager::getVideoDriver()
{
	return Driver;
}

//! adds Volume Lighting Scene Node.
//! the returned pointer must not be dropped.
IVolumeLightSceneNode* CSceneManager::addVolumeLightSceneNode(ISceneNode* parent, s32 id,
	const u32 subdivU, const u32 subdivV,
	const video::SColor foot, const video::SColor tail,
	const core::vector3df& position, const core::vector3df& rotation, const core::vector3df& scale)
{
	if (!parent)
		parent = this;

	IVolumeLightSceneNode* node = new CVolumeLightSceneNode(parent, this, id, subdivU, subdivV, foot, tail, position, rotation, scale);
	node->drop();

	return node;
}

//! adds a test scene node for test purposes to the scene. It is a simple cube of (1,1,1) size.
//! the returned pointer must not be dropped.
IMeshSceneNode* CSceneManager::addCubeSceneNode(f32 size, ISceneNode* parent,
		s32 id, const core::vector3df& position,
		const core::vector3df& rotation, const core::vector3df& scale)
{
	if (!parent)
		parent = this;

	IMeshSceneNode* node = new CCubeSceneNode(size, parent, this, id, position, rotation, scale);
	node->drop();

	return node;
}


//! Adds a sphere scene node for test purposes to the scene.
IMeshSceneNode* CSceneManager::addSphereSceneNode(f32 radius, s32 polyCount,
		ISceneNode* parent, s32 id, const core::vector3df& position,
		const core::vector3df& rotation, const core::vector3df& scale)
{
	if (!parent)
		parent = this;

	IMeshSceneNode* node = new CSphereSceneNode(radius, polyCount, polyCount, parent, this, id, position, rotation, scale);
	node->drop();

	return node;
}


//! adds a scene node for rendering a static mesh
//! the returned pointer must not be dropped.
IMeshSceneNode* CSceneManager::addMeshSceneNode(IMesh* mesh, ISceneNode* parent, s32 id,
	const core::vector3df& position, const core::vector3df& rotation,
	const core::vector3df& scale, bool alsoAddIfMeshPointerZero)
{
	if (!alsoAddIfMeshPointerZero && !mesh)
		return 0;

	if (!parent)
		parent = this;

	IMeshSceneNode* node = new CMeshSceneNode(mesh, parent, this, id, position, rotation, scale);
	node->drop();

	return node;
}

//! Adds a camera scene node to the tree and sets it as active camera.
//! \param position: Position of the space relative to its parent where the camera will be placed.
//! \param lookat: Position where the camera will look at. Also known as target.
//! \param parent: Parent scene node of the camera. Can be null. If the parent moves,
//! the camera will move too.
//! \return Returns pointer to interface to camera
ICameraSceneNode* CSceneManager::addCameraSceneNode(ISceneNode* parent,
	const core::vector3df& position, const core::vector3df& lookat, s32 id)
{
	if (!parent)
		parent = this;

	ICameraSceneNode* node = new CCameraSceneNode(parent, this, id, position, lookat);
	node->drop();

	setActiveCamera(node);

	return node;
}


//! Adds a camera scene node which is able to be controlld with the mouse similar
//! to in the 3D Software Maya by Alias Wavefront.
//! The returned pointer must not be dropped.
ICameraSceneNode* CSceneManager::addCameraSceneNodeMaya(ISceneNode* parent,
	f32 rotateSpeed, f32 zoomSpeed, f32 translationSpeed, s32 id)
{
	if (!parent)
		parent = this;

	ICameraSceneNode* node = new CCameraSceneNode(parent, this, id);
	ISceneNodeAnimator* anm = new CSceneNodeAnimatorCameraMaya(CursorControl, 
		rotateSpeed, zoomSpeed, translationSpeed);

	node->addAnimator(anm);
	setActiveCamera(node);

	anm->drop();
	node->drop();

	return node;
}


//! Adds a camera scene node which is able to be controled with the mouse and keys
//! like in most first person shooters (FPS):
ICameraSceneNode* CSceneManager::addCameraSceneNodeFPS(ISceneNode* parent,
	f32 rotateSpeed, f32 moveSpeed, s32 id,
	SKeyMap* keyMapArray, s32 keyMapSize, bool noVerticalMovement,f32 jumpSpeed)
{
	if (!parent)
		parent = this;

	ICameraSceneNode* node = new CCameraSceneNode(parent, this, id);
	ISceneNodeAnimator* anm = new CSceneNodeAnimatorCameraFPS(CursorControl,
			rotateSpeed, moveSpeed, jumpSpeed,
			keyMapArray, keyMapSize, noVerticalMovement);

	// Bind the node's rotation to its target. This is consistent with 1.4.2 and below.
	node->bindTargetAndRotation(true);

	node->addAnimator(anm);
	setActiveCamera(node);

	anm->drop();
	node->drop();

	return node;
}


//! Adds a dynamic light scene node. The light will cast dynamic light on all
//! other scene nodes in the scene, which have the material flag video::MTF_LIGHTING
//! turned on. (This is the default setting in most scene nodes).
ILightSceneNode* CSceneManager::addLightSceneNode(ISceneNode* parent,
	const core::vector3df& position, video::SColorf color, f32 range, s32 id)
{
	if (!parent)
		parent = this;

	ILightSceneNode* node = new CLightSceneNode(parent, this, id, position, color, range);
	node->drop();

	return node;
}

//! Adds a skybox scene node. A skybox is a big cube with 6 textures on it and
//! is drawn around the camera position.
ISceneNode* CSceneManager::addSkyBoxSceneNode(video::ITexture* top, video::ITexture* bottom,
	video::ITexture* left, video::ITexture* right, video::ITexture* front,
	video::ITexture* back, ISceneNode* parent, s32 id)
{
	if (!parent)
		parent = this;

	ISceneNode* node = new CSkyBoxSceneNode(top, bottom, left, right,
			front, back, parent, this, id);

	node->drop();
	return node;
}


//! Adds a skydome scene node. A skydome is a large (half-) sphere with a
//! panoramic texture on it and is drawn around the camera position.
ISceneNode* CSceneManager::addSkyDomeSceneNode(video::ITexture* texture,
	u32 horiRes, u32 vertRes, f64 texturePercentage,
	f64 spherePercentage, ISceneNode* parent, s32 id)
{
	if (!parent)
		parent = this;

	ISceneNode* node = new CSkyDomeSceneNode(texture, horiRes, vertRes,
		texturePercentage, spherePercentage, parent, this, id);

	node->drop();
	return node;
}

//! Adds an empty scene node.
ISceneNode* CSceneManager::addEmptySceneNode(ISceneNode* parent, s32 id)
{
	if (!parent)
		parent = this;

	ISceneNode* node = new CEmptySceneNode(parent, this, id);
	node->drop();

	return node;
}

//! Adds a Hill Plane mesh to the mesh pool. The mesh is generated on the fly
//! and looks like a plane with some hills on it. You can specify how many hills
//! there should be on the plane and how high they should be. Also you must
//! specify a name for the mesh, because the mesh is added to the mesh pool,
//! and can be retrieved again using ISceneManager::getMesh with the name as
//! parameter.
IAnimatedMesh* CSceneManager::addHillPlaneMesh(const c8* name,
		const core::dimension2d<f32>& tileSize,
		const core::dimension2d<u32>& tileCount,
		video::SMaterial* material, f32 hillHeight,
		const core::dimension2d<f32>& countHills,
		const core::dimension2d<f32>& textureRepeatCount)
{
	if (!name)
		return 0;

	if (MeshCache->isMeshLoaded(name))
		return MeshCache->getMeshByFilename(name);

	IMesh* mesh = CGeometryCreator::createHillPlaneMesh(tileSize,
			tileCount, material, hillHeight, countHills,
			textureRepeatCount);
	if (!mesh)
		return 0;

	SAnimatedMesh* animatedMesh = new SAnimatedMesh();
	if (!animatedMesh)
	{
		mesh->drop();
		return 0;
	}

	animatedMesh->addMesh(mesh);
	mesh->drop();
	animatedMesh->recalculateBoundingBox();

	MeshCache->addMesh(name, animatedMesh);
	animatedMesh->drop();

	return animatedMesh;
}


//! Adds a terrain mesh to the mesh pool.
IAnimatedMesh* CSceneManager::addTerrainMesh(const c8* name,
	video::IImage* texture, video::IImage* heightmap,
	const core::dimension2d<f32>& stretchSize,
	f32 maxHeight,
	const core::dimension2d<s32>& defaultVertexBlockSize)
{
	if (!name)
		return 0;

	if (MeshCache->isMeshLoaded(name))
		return MeshCache->getMeshByFilename(name);

	IMesh* mesh = CGeometryCreator::createTerrainMesh(texture, heightmap,
			stretchSize, maxHeight, getVideoDriver(),
			defaultVertexBlockSize);
	if (!mesh)
		return 0;

	SAnimatedMesh* animatedMesh = new SAnimatedMesh();
	if (!animatedMesh)
	{
		mesh->drop();
		return 0;
	}

	animatedMesh->addMesh(mesh);
	mesh->drop();
	animatedMesh->recalculateBoundingBox();

	MeshCache->addMesh(name, animatedMesh);
	animatedMesh->drop();

	return animatedMesh;
}

//! Adds an arrow mesh to the mesh pool.
IAnimatedMesh* CSceneManager::addArrowMesh(const c8* name,
		video::SColor vtxColor0, video::SColor vtxColor1,
		u32 tesselationCylinder, u32 tesselationCone, f32 height,
		f32 cylinderHeight, f32 width0,f32 width1)
{
	if (!name)
		return 0;

	if (MeshCache->isMeshLoaded(name))
		return MeshCache->getMeshByFilename(name);

	IMesh* mesh = CGeometryCreator::createArrowMesh( tesselationCylinder,
			tesselationCone, height, cylinderHeight, width0,width1,
			vtxColor0, vtxColor1);
	if (!mesh)
		return 0;

	SAnimatedMesh* animatedMesh = new SAnimatedMesh();
	if (!animatedMesh)
	{
		mesh->drop();
		return 0;
	}

	animatedMesh->addMesh(mesh);
	mesh->drop();
	animatedMesh->recalculateBoundingBox();

	MeshCache->addMesh(name, animatedMesh);
	animatedMesh->drop();

	return animatedMesh;
}



//! Adds a static sphere mesh to the mesh pool.
IAnimatedMesh* CSceneManager::addSphereMesh(const c8* name,
		f32 radius, u32 polyCountX, u32 polyCountY)
{
	if (!name)
		return 0;

	if (MeshCache->isMeshLoaded(name))
		return MeshCache->getMeshByFilename(name);

	IMesh* mesh = CGeometryCreator::createSphereMesh(radius, polyCountX, polyCountY);
	if (!mesh)
		return 0;

	SAnimatedMesh* animatedMesh = new SAnimatedMesh();
	if (!animatedMesh)
	{
		mesh->drop();
		return 0;
	}

	animatedMesh->addMesh(mesh);
	mesh->drop();
	animatedMesh->recalculateBoundingBox();

	MeshCache->addMesh(name, animatedMesh);
	animatedMesh->drop();

	return animatedMesh;
}



//! Returns the root scene node. This is the scene node wich is parent
//! of all scene nodes. The root scene node is a special scene node which
//! only exists to manage all scene nodes. It is not rendered and cannot
//! be removed from the scene.
//! \return Returns a pointer to the root scene node.
ISceneNode* CSceneManager::getRootSceneNode()
{
	return this;
}


//! Returns the current active camera.
//! \return The active camera is returned. Note that this can be NULL, if there
//! was no camera created yet.
ICameraSceneNode* CSceneManager::getActiveCamera()
{
	return ActiveCamera;
}


//! Sets the active camera. The previous active camera will be deactivated.
//! \param camera: The new camera which should be active.
void CSceneManager::setActiveCamera(ICameraSceneNode* camera)
{
	if (ActiveCamera)
		ActiveCamera->drop();

	ActiveCamera = camera;

	if (ActiveCamera)
		ActiveCamera->grab();
}


//! renders the node.
void CSceneManager::render()
{
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CSceneManager::getBoundingBox() const
{
	_GEO_DEBUG_BREAK_IF(true) // Bounding Box of Scene Manager wanted.

	// should never be used.
	return *((core::aabbox3d<f32>*)0);
}


//! returns if node is culled
bool CSceneManager::isCulled(const ISceneNode* node)
{
	const ICameraSceneNode* cam = getActiveCamera();
	if (!cam)
	{
		_GEO_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return false;
	}

	return false;

	switch ( node->getAutomaticCulling() )
	{
		// can be seen by a bounding box ?
		case scene::EAC_BOX:
		{
			core::aabbox3d<f32> tbox = node->getBoundingBox();
			node->getAbsoluteTransformation().transformBoxEx(tbox);
			_GEO_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return !(tbox.intersectsWithBox(cam->getViewFrustum()->getBoundingBox() ));
		}

		// can be seen by a bounding sphere
		case scene::EAC_FRUSTUM_SPHERE:
		{ // requires bbox diameter
		}
		break;

		// can be seen by cam pyramid planes ?
		case scene::EAC_FRUSTUM_BOX:
		{
			SViewFrustum frust = *cam->getViewFrustum();

			//transform the frustum to the node's current absolute transformation
			core::matrix4 invTrans(node->getAbsoluteTransformation());
			invTrans.makeInverse();
			frust.transform(invTrans);

			core::vector3df edges[8];
			node->getBoundingBox().getEdges(edges);

			for (s32 i=0; i<scene::SViewFrustum::VF_PLANE_COUNT; ++i)
			{
				bool boxInFrustum=false;
				for (u32 j=0; j<8; ++j)
				{
					if (frust.planes[i].classifyPointRelation(edges[j]) != core::ISREL3D_FRONT)
					{
						boxInFrustum=true;
						break;
					}
				}

				if (!boxInFrustum)
					return true;
			}
		}
		break;

		case scene::EAC_OFF:
		break;
	}

	_GEO_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return false;
}


//! registers a node for rendering it at a specific time.
u32 CSceneManager::registerNodeForRendering(ISceneNode* node, E_SCENE_NODE_RENDER_PASS time)
{
	u32 taken = 0;

	switch(time)
	{
		// take camera if it doesn't exists
		case ESNRP_CAMERA:
		{
			taken = 1;
			for ( u32 i = 0; i != CameraList.size(); ++i )
			{
				if ( CameraList[i] == node )
				{
					taken = 0;
					break;
				}
			}
			if ( taken )
			{
				CameraList.push_back(node);
			}
		}break;

	case ESNRP_LIGHT:
		// TODO: Point Light culling..
		// Lighting modell in irrlicht has to be redone..
		//if (!isCulled(node))
		{
			LightList.push_back(DistanceNodeEntry(node, camWorldPos));
			taken = 1;
		}
		break;

	case ESNRP_SKY_BOX:
		SkyBoxList.push_back(node);
		taken = 1;
		break;
	case ESNRP_SOLID:
		if (!isCulled(node))
		{
			SolidNodeList.push_back(node);
			taken = 1;
		}
		break;
	case ESNRP_TRANSPARENT:
		if (!isCulled(node))
		{
			TransparentNodeList.push_back(TransparentNodeEntry(node, camWorldPos));
			taken = 1;
		}
		break;
	case ESNRP_AUTOMATIC:
		if (!isCulled(node))
		{
			u32 count = node->getMaterialCount();

			taken = 0;
			for (u32 i=0; i<count; ++i)
			{
				video::IMaterialRenderer* rnd =
					Driver->getMaterialRenderer(node->getMaterial(i).MaterialType);
				if (rnd && rnd->isTransparent())
				{
					// register as transparent node
					TransparentNodeEntry e(node, camWorldPos);
					TransparentNodeList.push_back(e);
					taken = 1;
					break;
				}
			}

			// not transparent, register as solid
			if ( 0 == taken )
			{
				SolidNodeList.push_back(node);
				taken = 1;
			}
		}
		break;
	case ESNRP_SHADOW:
		if (!isCulled(node))
		{
			ShadowNodeList.push_back(node);
			taken = 1;
		}
		break;

	case ESNRP_COUNT: // ignore this one
		break;
	}

#ifdef SCENEMANAGER_DEBUG
	s32 index = Parameters.findAttribute ( "calls" );
	Parameters.setAttribute ( index, Parameters.getAttributeAsInt ( index ) + 1 );

	if ( 0 == taken )
	{
		index = Parameters.findAttribute ( "culled" );
		Parameters.setAttribute ( index, Parameters.getAttributeAsInt ( index ) + 1 );
	}
#endif

	return taken;
}

//#define _TEST_GL_LIST

//GLuint pbufferList = 0;
int	   callIndex = 0;


//! This method is called just before the rendering process of the whole scene.
//! draws all scene nodes
void CSceneManager::drawAll()
{
	if (!Driver)
		return;

	// reset attributes
	Parameters.setAttribute ( "culled", 0 );
	Parameters.setAttribute ( "calls", 0 );
	Parameters.setAttribute ( "drawn", 0 );

	// reset all transforms
	video::IVideoDriver* driver = getVideoDriver();
	// [ChaKI] no need code
	//if ( driver )
	//{
	//	driver->setTransform ( video::ETS_PROJECTION, core::IdentityMatrix );
	//	driver->setTransform ( video::ETS_VIEW, core::IdentityMatrix );
	//	driver->setTransform ( video::ETS_WORLD, core::IdentityMatrix );
	//	driver->setTransform ( video::ETS_TEXTURE_0, core::IdentityMatrix );
	//	driver->setTransform ( video::ETS_TEXTURE_1, core::IdentityMatrix );
	//	driver->setTransform ( video::ETS_TEXTURE_2, core::IdentityMatrix );
	//	driver->setTransform ( video::ETS_TEXTURE_3, core::IdentityMatrix );
	//}

	driver->setAllowZWriteOnTransparent(Parameters.getAttributeAsBool( ALLOW_ZWRITE_ON_TRANSPARENT) );

	// do animations and other stuff.
//	OnAnimate(os::Timer::getTime()); // chaki

	// validate textures
	driver->validateTextures();

	/*!
		First Scene Node for prerendering should be the active camera
		consistent Camera is needed for culling
	*/
	camWorldPos.set(0,0,0);
	if ( ActiveCamera )
	{
		ActiveCamera->OnRegisterSceneNode();
		camWorldPos = ActiveCamera->getAbsolutePosition();
	}

	// let all nodes register themselves
	OnRegisterSceneNode();

	u32 i; // new ISO for scoping problem in some compilers

	//render camera scenes
	{
		CurrentRendertime = ESNRP_CAMERA;
		for (i=0; i<CameraList.size(); ++i)
			CameraList[i]->render();

		CameraList.set_used(0);

	}

	//render lights scenes
	{
		CurrentRendertime = ESNRP_LIGHT;

		Driver->deleteAllDynamicLights();

		Driver->setAmbientLight(AmbientLight);

		LightList.sort();		// on distance to camera

		u32 maxLights = core::min_ ( Driver->getMaximalDynamicLightAmount(), LightList.size() );
		for (i=0; i< maxLights; ++i)
			LightList[i].Node->render();

		LightList.set_used(0);
	}

	// render skyboxes
	{
		CurrentRendertime = ESNRP_SKY_BOX;

		for (i=0; i<SkyBoxList.size(); ++i)
			SkyBoxList[i]->render();

		SkyBoxList.set_used(0);
	}


	// render default objects
	{
		CurrentRendertime = ESNRP_SOLID;
		//SolidNodeList.sort(); // sort by textures

#ifdef _TEST_GL_LIST
		if(pbufferList == 0 || callIndex > 2)
		{
			if(pbufferList != 0)
				glDeleteLists(pbufferList, 1);

			pbufferList = glGenLists(1);
			glNewList(pbufferList, GL_COMPILE);

			callIndex = 0;

			for (i=0; i<SolidNodeList.size(); ++i)
					SolidNodeList[i].Node->render();
		    glEndList();
		}
		else
		{
			callIndex++;
		}
		
		glCallList(pbufferList);

#else
		for (i=0; i<SolidNodeList.size(); ++i)
			SolidNodeList[i].Node->render();
#endif

		Parameters.setAttribute ( "drawn", (s32) SolidNodeList.size() );

		SolidNodeList.set_used(0);
	}

	//GetRulerNode ()->render ();

	// render shadows
	{
		CurrentRendertime = ESNRP_SHADOW;
		for (i=0; i<ShadowNodeList.size(); ++i)
			ShadowNodeList[i]->render();

		if (!ShadowNodeList.empty())
			Driver->drawStencilShadow(true,ShadowColor, ShadowColor,
				ShadowColor, ShadowColor);

		ShadowNodeList.set_used(0);
	}

	// render transparent objects.
	{
		CurrentRendertime = ESNRP_TRANSPARENT;
		TransparentNodeList.sort(); // sort by distance from camera

		for (i=0; i<TransparentNodeList.size(); ++i)
			TransparentNodeList[i].Node->render();

		TransparentNodeList.set_used(0);
	}

	clearDeletionList();

	CurrentRendertime = ESNRP_COUNT;
}


//! Sets the color of stencil buffers shadows drawn by the scene manager.
void CSceneManager::setShadowColor(video::SColor color)
{
	ShadowColor = color;
}


//! Returns the current color of shadows.
video::SColor CSceneManager::getShadowColor() const
{
	return ShadowColor;
}



//! creates a rotation animator, which rotates the attached scene node around itself.
ISceneNodeAnimator* CSceneManager::createRotationAnimator(const core::vector3df& rotationPerSecond)
{
	ISceneNodeAnimator* anim = new CSceneNodeAnimatorRotation(os::Timer::getTime(),
		rotationPerSecond);

	return anim;
}



//! creates a fly circle animator, which lets the attached scene node fly around a center.
ISceneNodeAnimator* CSceneManager::createFlyCircleAnimator(
		const core::vector3df& center, f32 radius, f32 speed,
		const core::vector3df& direction)
{
	ISceneNodeAnimator* anim = new CSceneNodeAnimatorFlyCircle(
			os::Timer::getTime(), center,
			radius, speed, direction);
	return anim;
}


//! Creates a fly straight animator, which lets the attached scene node
//! fly or move along a line between two points.
ISceneNodeAnimator* CSceneManager::createFlyStraightAnimator(const core::vector3df& startPoint,
					const core::vector3df& endPoint, u32 timeForWay, bool loop)
{
	ISceneNodeAnimator* anim = new CSceneNodeAnimatorFlyStraight(startPoint,
		endPoint, timeForWay, loop, os::Timer::getTime());

	return anim;
}


//! Creates a texture animator, which switches the textures of the target scene
//! node based on a list of textures.
ISceneNodeAnimator* CSceneManager::createTextureAnimator(const core::array<video::ITexture*>& textures,
	s32 timePerFrame, bool loop)
{
	ISceneNodeAnimator* anim = new CSceneNodeAnimatorTexture(textures,
		timePerFrame, loop, os::Timer::getTime());

	return anim;
}


//! Creates a scene node animator, which deletes the scene node after
//! some time automaticly.
ISceneNodeAnimator* CSceneManager::createDeleteAnimator(u32 when)
{
	return new CSceneNodeAnimatorDelete(this, os::Timer::getTime() + when);
}




//! Creates a special scene node animator for doing automatic collision detection
//! and response.
ISceneNodeAnimatorCollisionResponse* CSceneManager::createCollisionResponseAnimator(
	ITriangleSelector* world, ISceneNode* sceneNode, const core::vector3df& ellipsoidRadius,
	const core::vector3df& gravityPerSecond,
	const core::vector3df& ellipsoidTranslation, f32 slidingValue)
{
	ISceneNodeAnimatorCollisionResponse* anim = new
		CSceneNodeAnimatorCollisionResponse(this, world, sceneNode,
			ellipsoidRadius, gravityPerSecond,
			ellipsoidTranslation, slidingValue);

	return anim;
}


//! Creates a follow spline animator.
ISceneNodeAnimator* CSceneManager::createFollowSplineAnimator(s32 startTime,
	const core::array< core::vector3df >& points,
	f32 speed, f32 tightness)
{
	ISceneNodeAnimator* a = new CSceneNodeAnimatorFollowSpline(startTime, points,
		speed, tightness);
	return a;
}

//! Returns a pointer to the scene collision manager.
ISceneCollisionManager* CSceneManager::getSceneCollisionManager()
{
	return CollisionManager;
}


//! Returns a pointer to the mesh manipulator.
IMeshManipulator* CSceneManager::getMeshManipulator()
{
	return Driver->getMeshManipulator();
}


//! Creates a simple ITriangleSelector, based on a mesh.
ITriangleSelector* CSceneManager::createTriangleSelector(IMesh* mesh, ISceneNode* node)
{
	if (!mesh || !node)
		return 0;

	return new CTriangleSelector(mesh, node);
}


//! Creates a simple dynamic ITriangleSelector, based on a axis aligned bounding box.
ITriangleSelector* CSceneManager::createTriangleSelectorFromBoundingBox(ISceneNode* node)
{
	if (!node)
		return 0;

	return new CTriangleBBSelector(node);
}

//! Creates a meta triangle selector.
IMetaTriangleSelector* CSceneManager::createMetaTriangleSelector()
{
	return new CMetaTriangleSelector();
}


//! Adds a scene node to the deletion queue.
void CSceneManager::addToDeletionQueue(ISceneNode* node)
{
	if (!node)
		return;

	node->grab();
	DeletionList.push_back(node);
}


//! clears the deletion list
void CSceneManager::clearDeletionList()
{
	if (DeletionList.empty())
		return;

	for (u32 i=0; i<DeletionList.size(); ++i)
	{
		DeletionList[i]->remove();
		DeletionList[i]->drop();
	}

	DeletionList.clear();
}


//! Returns the first scene node with the specified name.
ISceneNode* CSceneManager::getSceneNodeFromName(const char* name, ISceneNode* start)
{
	if (start == 0)
		start = getRootSceneNode();

	if (!strcmp(start->getName(),name))
		return start;

	ISceneNode* node = 0;

	const core::list<ISceneNode*>& list = start->getChildren();
	core::list<ISceneNode*>::ConstIterator it = list.begin();
	for (; it!=list.end(); ++it)
	{
		node = getSceneNodeFromName(name, *it);
		if (node)
			return node;
	}

	return 0;
}


//! Returns the first scene node with the specified id.
ISceneNode* CSceneManager::getSceneNodeFromId(s32 id, ISceneNode* start)
{
	if (start == 0)
		start = getRootSceneNode();

	if (start->getID() == id)
		return start;

	ISceneNode* node = 0;

	const core::list<ISceneNode*>& list = start->getChildren();
	core::list<ISceneNode*>::ConstIterator it = list.begin();
	for (; it!=list.end(); ++it)
	{
		node = getSceneNodeFromId(id, *it);
		if (node)
			return node;
	}

	return 0;
}


//! Returns the first scene node with the specified type.
ISceneNode* CSceneManager::getSceneNodeFromType(scene::ESCENE_NODE_TYPE type, ISceneNode* start)
{
	if (start == 0)
		start = getRootSceneNode();

	if (start->getType() == type || ESNT_ANY == type)
		return start;

	ISceneNode* node = 0;

	const core::list<ISceneNode*>& list = start->getChildren();
	core::list<ISceneNode*>::ConstIterator it = list.begin();
	for (; it!=list.end(); ++it)
	{
		node = getSceneNodeFromType(type, *it);
		if (node)
			return node;
	}

	return 0;
}

//! returns scene nodes by type.
void CSceneManager::getSceneNodesFromType(ESCENE_NODE_TYPE type, core::array<scene::ISceneNode*>& outNodes, ISceneNode* start)
{
	if (start == 0)
		start = getRootSceneNode();

	if (start->getType() == type || ESNT_ANY == type)
		outNodes.push_back(start);

	const core::list<ISceneNode*>& list = start->getChildren();
	core::list<ISceneNode*>::ConstIterator it = list.begin();

	for (; it!=list.end(); ++it)
	{
		getSceneNodesFromType(type, outNodes, *it);
	}
}


//! Posts an input event to the environment. Usually you do not have to
//! use this method, it is used by the internal engine.
bool CSceneManager::postEventFromUser(const SEvent& event)
{
	bool ret = false;
	ICameraSceneNode* cam = getActiveCamera();
	if (cam)
		ret = cam->OnEvent(event);

	_GEO_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return ret;
}


//! Removes all children of this scene node
void CSceneManager::removeAll()
{
	ISceneNode::removeAll();
	setActiveCamera(0);
}


//! Clears the whole scene. All scene nodes are removed.
void CSceneManager::clear()
{
	removeAll();
}


//! Returns interface to the parameters set in this scene.
io::IAttributes* CSceneManager::getParameters()
{
	return &Parameters;
}


//! Returns current render pass.
E_SCENE_NODE_RENDER_PASS CSceneManager::getSceneNodeRenderPass() const
{
	return CurrentRendertime;
}



//! Returns an interface to the mesh cache which is shared beween all existing scene managers.
IMeshCache* CSceneManager::getMeshCache()
{
	return MeshCache;
}


//! Creates a new scene manager.
ISceneManager* CSceneManager::createNewSceneManager(bool cloneContent)
{
	CSceneManager* manager = new CSceneManager(Driver, FileSystem, CursorControl, MeshCache);

	if (cloneContent)
		manager->cloneMembers(this, manager);

	return manager;
}

//! Sets ambient color of the scene
void CSceneManager::setAmbientLight(const video::SColorf &ambientColor)
{
	AmbientLight = ambientColor;
}


//! Returns ambient color of the scene
const video::SColorf& CSceneManager::getAmbientLight() const
{
	return AmbientLight;
}


// creates a scenemanager
ISceneManager* createSceneManager(video::IVideoDriver* driver,
		io::IFileSystem* fs, gui::ICursorControl* cursorcontrol)
{
	return new CSceneManager(driver, fs, cursorcontrol, 0);
}


} // end namespace scene
} // end namespace geo

