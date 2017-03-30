// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_SCENE_MANAGER_H_INCLUDED__
#define __C_SCENE_MANAGER_H_INCLUDED__

#include "ISceneManager.h"
#include "ISceneNode.h"
#include "ICursorControl.h"
#include "geoString.h"
#include "geoArray.h"
#include "IMeshLoader.h"
#include "CAttributes.h"

//#include "IGeoFont.h"
//#include "CGeoNameSceneNode.h"
namespace geo
{
namespace io
{
	class IXMLWriter;
	class IFileSystem;
}
namespace scene
{
	class IMeshCache;

	/*!
		The Scene Manager manages scene nodes, mesh recources, cameras and all the other stuff.
	*/
	class CSceneManager : public ISceneManager, public ISceneNode
	{
	public:

		//! constructor
		CSceneManager(video::IVideoDriver* driver, io::IFileSystem* fs, 
			gui::ICursorControl* cursorControl, IMeshCache* cache = 0);

		//! destructor
		virtual ~CSceneManager();

		//! Returns an interface to the mesh cache which is shared beween all existing scene managers.
		virtual IMeshCache* getMeshCache();

		//! returns the video driver
		virtual video::IVideoDriver* getVideoDriver();

		//! adds Volume Lighting Scene Node.
		//! the returned pointer must not be dropped.
		virtual IVolumeLightSceneNode* addVolumeLightSceneNode(ISceneNode* parent=0, s32 id=-1,
			const u32 subdivU = 32, const u32 subdivV = 32,
			const video::SColor foot = video::SColor(51, 0, 230, 180),
			const video::SColor tail = video::SColor(0, 0, 0, 0),
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! adds a cube scene node to the scene. It is a simple cube of (1,1,1) size.
		//! the returned pointer must not be dropped.
		virtual IMeshSceneNode* addCubeSceneNode(f32 size=10.0f, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),	const core::vector3df& rotation = core::vector3df(0,0,0),	const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! Adds a sphere scene node to the scene.
		virtual IMeshSceneNode* addSphereSceneNode(f32 radius=5.0f, s32 polyCount=16, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! adds a scene node for rendering a static mesh
		//! the returned pointer must not be dropped.
		virtual IMeshSceneNode* addMeshSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f),
			bool alsoAddIfMeshPointerZero=false);

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! registers a node for rendering it at a specific time.
		virtual u32 registerNodeForRendering(ISceneNode* node, E_SCENE_NODE_RENDER_PASS = ESNRP_AUTOMATIC);

		//! draws all scene nodes
		virtual void drawAll();

		//! Adds a camera scene node to the tree and sets it as active camera.
		//! \param position: Position of the space relative to its parent where the camera will be placed.
		//! \param lookat: Position where the camera will look at. Also known as target.
		//! \param parent: Parent scene node of the camera. Can be null. If the parent moves,
		//! the camera will move too.
		//! \return Pointer to interface to camera
		virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& lookat = core::vector3df(0,0,100), s32 id=-1);

		//! Adds a camera scene node which is able to be controlle with the mouse similar
		//! like in the 3D Software Maya by Alias Wavefront.
		//! The returned pointer must not be dropped.
		virtual ICameraSceneNode* addCameraSceneNodeMaya(ISceneNode* parent = 0,
			f32 rotateSpeed = -1500.0f, f32 zoomSpeed = 200.0f, f32 translationSpeed = 1500.0f, s32 id=-1);

		//! Adds a camera scene node which is able to be controled with the mouse and keys
		//! like in most first person shooters (FPS):
		virtual ICameraSceneNode* addCameraSceneNodeFPS(ISceneNode* parent = 0,
			f32 rotateSpeed = 100.0f, f32 moveSpeed = .5f, s32 id=-1,
			SKeyMap* keyMapArray=0, s32 keyMapSize=0, bool noVerticalMovement=false,
			f32 jumpSpeed = 0.f);

		//! Adds a dynamic light scene node. The light will cast dynamic light on all
		//! other scene nodes in the scene, which have the material flag video::MTF_LIGHTING
		//! turned on. (This is the default setting in most scene nodes).
		virtual ILightSceneNode* addLightSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0),
			video::SColorf color = video::SColorf(1.0f, 1.0f, 1.0f),
			f32 range=100.0f, s32 id=-1);

		//! Adds a skybox scene node. A skybox is a big cube with 6 textures on it and
		//! is drawn around the camera position.
		virtual ISceneNode* addSkyBoxSceneNode(video::ITexture* top, video::ITexture* bottom,
			video::ITexture* left, video::ITexture* right, video::ITexture* front,
			video::ITexture* back, ISceneNode* parent = 0, s32 id=-1);

		//! Adds a skydome scene node. A skydome is a large (half-) sphere with a
		//! panoramic texture on it and is drawn around the camera position.
		virtual ISceneNode* addSkyDomeSceneNode(video::ITexture* texture,
			u32 horiRes=16, u32 vertRes=8,
			f64 texturePercentage=0.9, f64 spherePercentage=2.0,
			ISceneNode* parent=0, s32 id=-1);

		//! Adds a Hill Plane mesh to the mesh pool. The mesh is
		//! generated on the fly and looks like a plane with some hills
		//! on it. You can specify how many hills should be on the plane
		//! and how high they should be. Also you must specify a name
		//! for the mesh because the mesh is added to the mesh pool and
		//! can be retrieved back using ISceneManager::getMesh with the
		//! name as parameter.
		virtual IAnimatedMesh* addHillPlaneMesh(const c8* name,
			const core::dimension2d<f32>& tileSize, const core::dimension2d<u32>& tileCount,
			video::SMaterial* material = 0,	f32 hillHeight = 0.0f,
			const core::dimension2d<f32>& countHills = core::dimension2d<f32>(1.0f, 1.0f),
			const core::dimension2d<f32>& textureRepeatCount = core::dimension2d<f32>(1.0f, 1.0f));

		//! Adds a terrain mesh to the mesh pool.
		virtual IAnimatedMesh* addTerrainMesh(const c8* meshname,	video::IImage* texture, video::IImage* heightmap,
			const core::dimension2d<f32>& stretchSize,
			f32 maxHeight, const core::dimension2d<s32>& defaultVertexBlockSize);

		//! Add a arrow mesh to the mesh pool
		virtual IAnimatedMesh* addArrowMesh(const c8* name,
				video::SColor vtxColor0, video::SColor vtxColor1,
				u32 tesselationCylinder, u32 tesselationCone,
				f32 height, f32 cylinderHeight, f32 width0,
				f32 width1);

		//! Adds a static sphere mesh to the mesh pool.
		IAnimatedMesh* addSphereMesh(const c8* name,
				f32 radius, u32 polyCountX, u32 polyCountY);

		//! Adds an empty scene node.
		virtual ISceneNode* addEmptySceneNode(ISceneNode* parent, s32 id=-1);

		//! Returns the root scene node. This is the scene node wich is parent
		//! of all scene nodes. The root scene node is a special scene node which
		//! only exists to manage all scene nodes. It is not rendered and cannot
		//! be removed from the scene.
		//! \return Pointer to the root scene node.
		virtual ISceneNode* getRootSceneNode();

		//! Returns the current active camera.
		//! \return The active camera is returned. Note that this can be NULL, if there
		//! was no camera created yet.
		virtual ICameraSceneNode* getActiveCamera();

		//! Sets the active camera. The previous active camera will be deactivated.
		//! \param camera: The new camera which should be active.
		virtual void setActiveCamera(ICameraSceneNode* camera);

		//! creates a rotation animator, which rotates the attached scene node around itself.
		//! \param rotationPerSecond: Specifies the speed of the animation
		//! \return The animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		virtual ISceneNodeAnimator* createRotationAnimator(const core::vector3df& rotationPerSecond);

		//! creates a fly circle animator
		/** Lets the attached scene node fly around a center.
		\param center Center relative to node origin
		\param speed rotation speed
		\return Animator. Attach it to a scene node with ISceneNode::addAnimator()
		and the animator will animate it. */
		virtual ISceneNodeAnimator* createFlyCircleAnimator(
				const core::vector3df& center=core::vector3df(0.f, 0.f, 0.f),
				f32 radius=100.f, f32 speed=0.001f,
				const core::vector3df& direction=core::vector3df(0.f, 1.f, 0.f));

		//! Creates a fly straight animator, which lets the attached scene node
		//! fly or move along a line between two points.
		virtual ISceneNodeAnimator* createFlyStraightAnimator(const core::vector3df& startPoint,
			const core::vector3df& endPoint, u32 timeForWay, bool loop=false);

		//! Creates a texture animator, which switches the textures of the target scene
		//! node based on a list of textures.
		virtual ISceneNodeAnimator* createTextureAnimator(const core::array<video::ITexture*>& textures,
			s32 timePerFrame, bool loop);

		//! Creates a scene node animator, which deletes the scene node after
		//! some time automaticly.
		virtual ISceneNodeAnimator* createDeleteAnimator(u32 timeMS);


		//! Creates a special scene node animator for doing automatic collision detection
		//! and response.
		virtual ISceneNodeAnimatorCollisionResponse* createCollisionResponseAnimator(
			ITriangleSelector* world, ISceneNode* sceneNode,
			const core::vector3df& ellipsoidRadius = core::vector3df(30,60,30),
			const core::vector3df& gravityPerSecond = core::vector3df(0,-1.0f,0),
			const core::vector3df& ellipsoidTranslation = core::vector3df(0,0,0),
			f32 slidingValue = 0.0005f);

		//! Creates a follow spline animator.
		virtual ISceneNodeAnimator* createFollowSplineAnimator(s32 startTime,
			const core::array< core::vector3df >& points,
			f32 speed = 1.0f, f32 tightness = 0.5f);


		//! Creates a simple ITriangleSelector, based on a mesh.
		virtual ITriangleSelector* createTriangleSelector(IMesh* mesh, ISceneNode* node);

		//! Creates a simple dynamic ITriangleSelector, based on a axis aligned bounding box.
		virtual ITriangleSelector* createTriangleSelectorFromBoundingBox(
			ISceneNode* node);

		//! Creates a meta triangle selector.
		virtual IMetaTriangleSelector* createMetaTriangleSelector();

		//! Returns a pointer to the scene collision manager.
		virtual ISceneCollisionManager* getSceneCollisionManager();

		//! Returns a pointer to the mesh manipulator.
		virtual IMeshManipulator* getMeshManipulator();

		//! Sets the color of stencil buffers shadows drawn by the scene manager.
		virtual void setShadowColor(video::SColor color);

		//! Returns the current color of shadows.
		virtual video::SColor getShadowColor() const;

		//! Adds a scene node to the deletion queue.
		virtual void addToDeletionQueue(ISceneNode* node);

		//! Returns the first scene node with the specified id.
		virtual ISceneNode* getSceneNodeFromId(s32 id, ISceneNode* start=0);

		//! Returns the first scene node with the specified name.
		virtual ISceneNode* getSceneNodeFromName(const c8* name, ISceneNode* start=0);

		//! Returns the first scene node with the specified type.
		virtual ISceneNode* getSceneNodeFromType(scene::ESCENE_NODE_TYPE type, ISceneNode* start=0);

		//! returns scene nodes by type.
		virtual void getSceneNodesFromType(ESCENE_NODE_TYPE type, core::array<scene::ISceneNode*>& outNodes, ISceneNode* start=0);

		//! Posts an input event to the environment. Usually you do not have to
		//! use this method, it is used by the internal engine.
		virtual bool postEventFromUser(const SEvent& event);

		//! Clears the whole scene. All scene nodes are removed.
		virtual void clear();

		//! Removes all children of this scene node
		virtual void removeAll();

		//! Returns interface to the parameters set in this scene.
		virtual io::IAttributes* getParameters();

		//! Returns current render pass.
		virtual E_SCENE_NODE_RENDER_PASS getSceneNodeRenderPass() const;

		//! Creates a new scene manager.
		virtual ISceneManager* createNewSceneManager(bool cloneContent);

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_UNKNOWN; }

		//! Sets ambient color of the scene
		virtual void setAmbientLight(const video::SColorf &ambientColor);

		//! Returns ambient color of the scene
		virtual const video::SColorf& getAmbientLight() const;

	private:

		//! returns if node is culled
		bool isCulled(const ISceneNode* node);

		//! clears the deletion list
		void clearDeletionList();

		struct DefaultNodeEntry
		{
			DefaultNodeEntry(ISceneNode* n) :
				Node(n), TextureValue(0)
			{
				if (n->getMaterialCount())
					TextureValue = (n->getMaterial(0).getTexture(0));
			}

			bool operator < (const DefaultNodeEntry& other) const
			{
				return (TextureValue < other.TextureValue);
			}

			ISceneNode* Node;
			private:
			void* TextureValue;
		};


		struct TransparentNodeEntry
		{
			TransparentNodeEntry(ISceneNode* n, const core::vector3df& camera)
				: Node(n)
			{
				Distance = (f32)(Node->getAbsoluteTransformation().getTranslation().getDistanceFromSQ(camera));
			}

			bool operator < (const TransparentNodeEntry& other) const
			{
				return (Distance > other.Distance);
			}

			ISceneNode* Node;
			private:
			f32 Distance;
		};

		//! sort on distance (sphere) to camera
		struct DistanceNodeEntry
		{
			DistanceNodeEntry(ISceneNode* n, f64 d)
				: Node(n), Distance(d) {}

			DistanceNodeEntry(ISceneNode* n, const core::vector3df& cameraPos)
				: Node(n)
			{
				Distance = Node->getAbsoluteTransformation().getTranslation().getDistanceFromSQ(cameraPos);
				Distance -= Node->getBoundingBox().getExtent().getLengthSQ() * 0.5;
			}

			bool operator < (const DistanceNodeEntry& other) const
			{
				return Distance < other.Distance;
			}

			ISceneNode* Node;
			private:
			f64 Distance;
		};

		//! video driver
		video::IVideoDriver* Driver;

		//! file system
		io::IFileSystem* FileSystem;

		//! cursor control
		gui::ICursorControl* CursorControl;

		//! collision manager
		ISceneCollisionManager* CollisionManager;

		//! render pass lists
		core::array<ISceneNode*> CameraList;
		core::array<DistanceNodeEntry> LightList;
		core::array<ISceneNode*> ShadowNodeList;
		core::array<ISceneNode*> SkyBoxList;
		core::array<DefaultNodeEntry> SolidNodeList;
		core::array<TransparentNodeEntry> TransparentNodeList;
		core::array<ISceneNode*> DeletionList;

		//! current active camera
		ICameraSceneNode* ActiveCamera;
		core::vector3df camWorldPos; // Position of camera for transparent nodes.

		video::SColor ShadowColor;
		video::SColorf AmbientLight;

		//! String parameters
		io::CAttributes Parameters;

		//! Mesh cache
		IMeshCache* MeshCache;

		E_SCENE_NODE_RENDER_PASS CurrentRendertime;
	};

} // end namespace video
} // end namespace scene

#endif

