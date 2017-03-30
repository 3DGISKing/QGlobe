// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __I_SCENE_MANAGER_H_INCLUDED__
#define __I_SCENE_MANAGER_H_INCLUDED__

#include "IReferenceCounted.h"
#include "geoArray.h"
#include "vector3d.h"
#include "dimension2d.h"
#include "SColor.h"
#include "ETerrainElements.h"
#include "ESceneNodeTypes.h"
#include "EMeshWriterEnums.h"
#include "SceneParameters.h"

class CGeoFont;

namespace geo
{
	struct SKeyMap;
	struct SEvent;

namespace io
{
	class IReadFile;
	class IAttributes;
	class IWriteFile;
} // end namespace io

namespace video
{
	class IVideoDriver;
	class SMaterial;
	class IImage;
	class ITexture;
} // end namespace video

namespace scene
{
	class IMeshWriter;

	//! Enumeration for render passes.
	/** A parameter passed to the registerNodeForRendering() method of the ISceneManager,
	specifying when the node wants to be drawn in relation to the other nodes. */
	enum E_SCENE_NODE_RENDER_PASS
	{
		//! Camera pass. The active view is set up here. The very first pass.
		ESNRP_CAMERA,

		//! In this pass, lights are transformed into camera space and added to the driver
		ESNRP_LIGHT,

		//! This is used for sky boxes.
		ESNRP_SKY_BOX,

		//! All normal objects can use this for registering themselves.
		/** This value will never be returned by
		ISceneManager::getSceneNodeRenderPass(). The scene manager
		will determine by itself if an object is transparent or solid
		and register the object as SNRT_TRANSPARENT or SNRT_SOLD
		automatically if you call registerNodeForRendering with this
		value (which is default). Note that it will register the node
		only as ONE type. If your scene node has both solid and
		transparent material types register it twice (one time as
		SNRT_SOLID, the other time as SNRT_TRANSPARENT) and in the
		render() method call getSceneNodeRenderPass() to find out the
		current render pass and render only the corresponding parts of
		the node. */
		ESNRP_AUTOMATIC,

		//! Solid scene nodes or special scene nodes without materials.
		ESNRP_SOLID,

		//! Drawn after the transparent nodes, the time for drawing shadow volumes
		ESNRP_SHADOW,

		//! Transparent scene nodes, drawn after shadow nodes. They are sorted from back to front and drawn in that order.
		ESNRP_TRANSPARENT,

		//! Never used, value specifing how much parameters there are.
		ESNRP_COUNT
	};

	class IMesh;
	class IMeshBuffer;
	class IAnimatedMesh;
	class IMeshCache;
	class ISceneNode;
	class ICameraSceneNode;
	class IAnimatedMeshSceneNode;
	class ISceneNodeAnimator;
	class ISceneNodeAnimatorCollisionResponse;
	class ILightSceneNode;
	class IBillboardSceneNode;
	class ITerrainSceneNode;
	class IMeshSceneNode;
	class IMeshLoader;
	class ISceneCollisionManager;
	class IParticleSystemSceneNode;
	class IDummyTransformationSceneNode;
	class ITriangleSelector;
	class IMetaTriangleSelector;
	class IMeshManipulator;
	class ITextSceneNode;
	class IBillboardTextSceneNode;
	class IVolumeLightSceneNode;
	class ISceneNodeFactory;
	class ISceneNodeAnimatorFactory;
	class ISceneUserDataSerializer;
	class IGeoTerrainNode;

	namespace quake3
	{
		class SShader;
	} // end namespace quake3

	//! The Scene Manager manages scene nodes, mesh recources, cameras and all the other stuff.
	/** All Scene nodes can be created only here. There is a always growing
	list of scene nodes for lots of purposes: Indoor rendering scene nodes
	like the Octree (addOctTreeSceneNode()) or the terrain renderer
	(addTerrainSceneNode()), different Camera scene nodes
	(addCameraSceneNode(), addCameraSceneNodeMaya()), scene nodes for Light
	(addLightSceneNode()), Billboards (addBillboardSceneNode()) and so on.
	A scene node is a node in the hierachical scene graph. Every scene node
	may have children, which are other scene nodes. Children move relative
	the their parents position. If the parent of a node is not visible, its
	children won't be visible, too. In this way, it is for example easily
	possible to attach a light to a moving car or to place a walking
	character on a moving platform on a moving ship.
	The SceneManager is also able to load 3d mesh files of different
	formats. Take a look at getMesh() to find out what formats are
	supported. And if these formats are not enough use
	addExternalMeshLoader() to add new formats to the engine.
	*/
	class ISceneManager : public virtual IReferenceCounted
	{
	public:

		//! Destructor
		virtual ~ISceneManager() {}

		//! Returns an interface to the mesh cache which is shared beween all existing scene managers.
		/** With this interface, it is possible to manually add new loaded
		meshes (if ISceneManager::getMesh() is not sufficient), to remove them and to iterate
		through already loaded meshes. */
		virtual IMeshCache* getMeshCache() = 0;

		//! Returns the video driver.
		/** \return Returns pointer to the video Driver.
		 This pointer should not be dropped. See IReferenceCounted::drop() for more information. */
		virtual video::IVideoDriver* getVideoDriver() = 0;

		//! adds Volume Lighting Scene Node.
		//! the returned pointer must not be dropped.
		/** Example Usage:
			scene::IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode(NULL, -1,
						32, 32, //Subdivide U/V
						video::SColor(0, 180, 180, 180), //foot color
						video::SColor(0, 0, 0, 0) //tail color
						);
			if (n)
			{
				n->setScale(core::vector3df(46.0f, 45.0f, 46.0f));
				n->getMaterial(0).setTexture(0, smgr->getVideoDriver()->getTexture("lightFalloff.png"));
			}
		**/
		virtual IVolumeLightSceneNode* addVolumeLightSceneNode(ISceneNode* parent=0, s32 id=-1,
			const u32 subdivU = 32, const u32 subdivV = 32,
			const video::SColor foot = video::SColor(51, 0, 230, 180),
			const video::SColor tail = video::SColor(0, 0, 0, 0),
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) = 0;


		//! Adds a test scene node for test purposes to the scene.
		/** It is a simple cube of (1,1,1) size.
		\param size: Size of the cube.
		\param parent: Parent of the scene node. Can be NULL if no parent.
		\param id: Id of the node. This id can be used to identify the scene node.
		\param position: Position of the space relative to its parent where the
		 scene node will be placed.
		\param rotation: Initital rotation of the scene node.
		\param scale: Initial scale of the scene node.
		\return Returns pointer to the created test scene node. This
		pointer should not be dropped. See IReferenceCounted::drop()
		for more information. */
		virtual IMeshSceneNode* addCubeSceneNode(f32 size=10.0f, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) = 0;

		//! Adds a sphere scene node for test purposes to the scene.
		/** It is a simple sphere.
		\param radius: Radius of the sphere.
		\param polyCount: Polycount of the sphere.
		\param parent: Parent of the scene node. Can be NULL if no parent.
		\param id: Id of the node. This id can be used to identify the scene node.
		\param position: Position of the space relative to its parent where the
		 scene node will be placed.
		\param rotation: Initital rotation of the scene node.
		\param scale: Initial scale of the scene node.
		\return Returns pointer to the created test scene node. This
		pointer should not be dropped. See IReferenceCounted::drop()
		for more information. */
		virtual IMeshSceneNode* addSphereSceneNode(f32 radius=5.0f, s32 polyCount=16,
				ISceneNode* parent=0, s32 id=-1,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) = 0;


		//! Adds a scene node for rendering a static mesh.
		/** \param mesh: Pointer to the loaded static mesh to be displayed.
		\param parent: Parent of the scene node. Can be NULL if no parent.
		\param id: Id of the node. This id can be used to identify the scene node.
		\param position: Position of the space relative to its parent where the
		scene node will be placed.
		\param rotation: Initital rotation of the scene node.
		\param scale: Initial scale of the scene node.
		\param alsoAddIfMeshPointerZero: Add the scene node even if a 0 pointer is passed.
		\return Returns pointer to the created scene node.
		This pointer should not be dropped. See IReferenceCounted::drop() for more information. */
		virtual IMeshSceneNode* addMeshSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f),
			bool alsoAddIfMeshPointerZero=false) = 0;

		//! Adds a camera scene node to the scene graph and sets it as active camera.
		/** This camera does not react on user input like for example the one created with
		addCameraSceneNodeFPS(). If you want to move or animate it, use animators or the
		ISceneNode::setPosition(), ICameraSceneNode::setTarget() etc methods.
		 \param position: Position of the space relative to its parent where the camera will be placed.
		 \param lookat: Position where the camera will look at. Also known as target.
		 \param parent: Parent scene node of the camera. Can be null. If the parent moves,
		 the camera will move too.
		 \param id: id of the camera. This id can be used to identify the camera.
		 \return Returns pointer to interface to camera if successful, otherwise 0.
		 This pointer should not be dropped. See IReferenceCounted::drop() for more information. */
		virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& lookat = core::vector3df(0,0,100), s32 id=-1) = 0;

		//! Adds a maya style user controlled camera scene node to the scene graph.
		/** This is a standard camera with an animator that provides
		mouse control similar to camera in the 3D Software Maya.
		\param parent: Parent scene node of the camera. Can be null.
		\param rotateSpeed: Rotation speed of the camera.
		\param zoomSpeed: Zoom speed of the camera.
		\param translationSpeed: Translation speed of the camera.
		\param id: id of the camera. This id can be used to identify the camera.
		\return Pointer to the interface of the camera if successful, otherwise 0.
		This pointer should not be dropped. See
		IReferenceCounted::drop() for more information. */
		virtual ICameraSceneNode* addCameraSceneNodeMaya(ISceneNode* parent = 0,
			f32 rotateSpeed = -1500.0f, f32 zoomSpeed = 200.0f,
			f32 translationSpeed = 1500.0f, s32 id=-1) = 0;

		//! Adds a camera scene node with an animator which provides mouse and keyboard control appropriate for first person shooters (FPS).
		/** This FPS camera is intended to provide a demonstration of a
		camera that behaves like a typical First Person Shooter. It is
		useful for simple demos and prototyping but is not intended to
		provide a full solution for a production quality game. It binds
		the camera scene node rotation to the look-at target; @see
		ICameraSceneNode::bindTargetAndRotation().  With this camera,
		you look with the mouse, and move with cursor keys. If you want
		to change the key layout, you can specify your own keymap. For
		example to make the camera be controlled by the cursor keys AND
		the keys W,A,S, and D, do something like this:
		\code
		SKeyMap keyMap[8];
		keyMap[0].Action = EKA_MOVE_FORWARD;
		keyMap[0].KeyCode = KEY_UP;
		keyMap[1].Action = EKA_MOVE_FORWARD;
		keyMap[1].KeyCode = KEY_KEY_W;

		keyMap[2].Action = EKA_MOVE_BACKWARD;
		keyMap[2].KeyCode = KEY_DOWN;
		keyMap[3].Action = EKA_MOVE_BACKWARD;
		keyMap[3].KeyCode = KEY_KEY_S;

		keyMap[4].Action = EKA_STRAFE_LEFT;
		keyMap[4].KeyCode = KEY_LEFT;
		keyMap[5].Action = EKA_STRAFE_LEFT;
		keyMap[5].KeyCode = KEY_KEY_A;

		keyMap[6].Action = EKA_STRAFE_RIGHT;
		keyMap[6].KeyCode = KEY_RIGHT;
		keyMap[7].Action = EKA_STRAFE_RIGHT;
		keyMap[7].KeyCode = KEY_KEY_D;

		camera = sceneManager->addCameraSceneNodeFPS(0, 100, 500, -1, keyMap, 8);
		\endcode
		\param parent: Parent scene node of the camera. Can be null.
		\param rotateSpeed: Speed in degress with which the camera is
		rotated. This can be done only with the mouse.
		\param moveSpeed: Speed in units per millisecond with which
		the camera is moved. Movement is done with the cursor keys.
		\param id: id of the camera. This id can be used to identify the camera.
		\param keyMapArray: Optional pointer to an array of a keymap, specifying what
		keys should be used to move the camera. If this is null, the default keymap
		is used. You can define actions more then one time in the array, to bind
		multiple keys to the same action.
		\param keyMapSize: Amount of items in the keymap array.
		\param noVerticalMovement: Setting this to true makes the
		camera only move within a horizontal plane, and disables
		vertical movement as known from most ego shooters. Default is
		'false', with which it is possible to fly around in space, if
		no gravity is there.
		\param jumpSpeed: Speed with which the camera is moved when jumping.
		\return Pointer to the interface of the camera if successful, otherwise 0.
		This pointer should not be dropped. See
		IReferenceCounted::drop() for more information. */
		virtual ICameraSceneNode* addCameraSceneNodeFPS(ISceneNode* parent = 0,
			f32 rotateSpeed = 100.0f, f32 moveSpeed = .5f, s32 id=-1,
			SKeyMap* keyMapArray=0, s32 keyMapSize=0, bool noVerticalMovement=false,
			f32 jumpSpeed = 0.f) = 0;

		//! Adds a dynamic light scene node to the scene graph.
		/** The light will cast dynamic light on all
		 other scene nodes in the scene, which have the material flag video::MTF_LIGHTING
		 turned on. (This is the default setting in most scene nodes).
		 \param parent: Parent scene node of the light. Can be null. If the parent moves,
		 the light will move too.
		 \param position: Position of the space relative to its parent where the light will be placed.
		 \param color: Diffuse color of the light. Ambient or Specular colors can be set manually with
		 the ILightSceneNode::getLightData() method.
		 \param radius: Radius of the light.
		 \param id: id of the node. This id can be used to identify the node.
		 \return Returns pointer to the interface of the light if successful, otherwise NULL.
		 This pointer should not be dropped. See IReferenceCounted::drop() for more information. */
		virtual ILightSceneNode* addLightSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0),
			video::SColorf color = video::SColorf(1.0f, 1.0f, 1.0f),
			f32 radius=100.0f, s32 id=-1) = 0;

		//! Adds a skybox scene node to the scene graph.
		/** A skybox is a big cube with 6 textures on it and
		 is drawn around the camera position.
		 \param top: Texture for the top plane of the box.
		 \param bottom: Texture for the bottom plane of the box.
		 \param left: Texture for the left plane of the box.
		 \param right: Texture for the right plane of the box.
		 \param front: Texture for the front plane of the box.
		 \param back: Texture for the back plane of the box.
		 \param parent: Parent scene node of the skybox. A skybox usually has no parent,
		 so this should be null. Note: If a parent is set to the skybox, the box will not
		 change how it is drawn.
		 \param id: An id of the node. This id can be used to identify the node.
		 \return Returns a pointer to the sky box if successful, otherwise NULL.
		 This pointer should not be dropped. See IReferenceCounted::drop() for more information. */
		virtual ISceneNode* addSkyBoxSceneNode(video::ITexture* top, video::ITexture* bottom,
			video::ITexture* left, video::ITexture* right, video::ITexture* front,
			video::ITexture* back, ISceneNode* parent = 0, s32 id=-1) = 0;

		//! Adds a skydome scene node to the scene graph.
		/** A skydome is a large (half-) sphere with a panoramic texture
		 on the inside and is drawn around the camera position.
		 \param texture: Texture for the dome.
		 \param horiRes: Number of vertices of a horizontal layer of the sphere.
		 \param vertRes: Number of vertices of a vertical layer of the sphere.
		 \param texturePercentage: How much of the height of the
		 texture is used. Should be between 0 and 1.
		 \param spherePercentage: How much of the sphere is drawn.
		 Value should be between 0 and 2, where 1 is an exact
		 half-sphere and 2 is a full sphere.
		 \param parent: Parent scene node of the dome. A dome usually has no parent,
		 so this should be null. Note: If a parent is set, the dome will not
		 change how it is drawn.
		 \param id: An id of the node. This id can be used to identify the node.
		 \return Returns a pointer to the sky dome if successful, otherwise NULL.
		 This pointer should not be dropped. See IReferenceCounted::drop() for more information. */
		virtual ISceneNode* addSkyDomeSceneNode(video::ITexture* texture,
			u32 horiRes=16, u32 vertRes=8,
			f64 texturePercentage=0.9, f64 spherePercentage=2.0,
			ISceneNode* parent=0, s32 id=-1) = 0;

		//! Adds an empty scene node to the scene graph.
		/** Can be used for doing advanced transformations
		 or structuring the scene graph.
		 \return Returns pointer to the created scene node.
		 This pointer should not be dropped. See IReferenceCounted::drop() for more information. */
		virtual ISceneNode* addEmptySceneNode(ISceneNode* parent=0, s32 id=-1) = 0;

		//! Adds a Hill Plane mesh to the mesh pool.
		/** The mesh is generated on the fly
		 and looks like a plane with some hills on it. It is uses mostly for quick
		 tests of the engine only. You can specify how many hills there should be
		 on the plane and how high they should be. Also you must specify a name for
		 the mesh, because the mesh is added to the mesh pool, and can be retrieved
		 again using ISceneManager::getMesh() with the name as parameter.
		 \param name: The name of this mesh which must be specified in order
		 to be able to retrieve the mesh later with ISceneManager::getMesh().
		 \param tileSize: Size of a tile of the mesh. (10.0f, 10.0f) would be a
		 good value to start, for example.
		 \param tileCount: Specifies how much tiles there will be. If you specifiy
		 for example that a tile has the size (10.0f, 10.0f) and the tileCount is
		 (10,10), than you get a field of 100 tiles which has the dimension 100.0fx100.0f.
		 \param material: Material of the hill mesh.
		 \param hillHeight: Height of the hills. If you specify a negative value
		 you will get holes instead of hills. If the height is 0, no hills will be
		 created.
		 \param countHills: Amount of hills on the plane. There will be countHills.X
		 hills along the X axis and countHills.Y along the Y axis. So in total there
		 will be countHills.X * countHills.Y hills.
		 \param textureRepeatCount: Defines how often the texture will be repeated in
		 x and y direction.
		\return Returns null if the creation failed. The reason could be that you
		 specified some invalid parameters or that a mesh with that name already
		 exists. If successful, a pointer to the mesh is returned.
		 This pointer should not be dropped. See IReferenceCounted::drop() for more information. */
		virtual IAnimatedMesh* addHillPlaneMesh(const c8* name,
			const core::dimension2d<f32>& tileSize, const core::dimension2d<u32>& tileCount,
			video::SMaterial* material = 0, f32 hillHeight = 0.0f,
			const core::dimension2d<f32>& countHills = core::dimension2d<f32>(0.0f, 0.0f),
			const core::dimension2d<f32>& textureRepeatCount = core::dimension2d<f32>(1.0f, 1.0f)) = 0;

		 //! Adds a static terrain mesh to the mesh pool.
		 /** The mesh is generated on the fly
		 from a texture file and a height map file. Both files may be huge
		 (8000x8000 pixels would be no problem) because the generator splits the
		 files into smaller textures if necessary.
		 You must specify a name for the mesh, because the mesh is added to the mesh pool,
		 and can be retrieved again using ISceneManager::getMesh() with the name as parameter.
		 \param meshname: The name of this mesh which must be specified in order
		 to be able to retrieve the mesh later with ISceneManager::getMesh().
		 \param texture: Texture for the terrain. Please note that this is not a
		 hardware texture as usual (ITexture), but an IImage software texture.
		 You can load this texture with IVideoDriver::createImageFromFile().
		 \param heightmap: A grayscaled heightmap image. Like the texture,
		 it can be created with IVideoDriver::createImageFromFile(). The amount
		 of triangles created depends on the size of this texture, so use a small
		 heightmap to increase rendering speed.
		 \param stretchSize: Parameter defining how big a is pixel on the heightmap.
		 \param maxHeight: Defines how high a white pixel on the heighmap is.
		 \param defaultVertexBlockSize: Defines the initial dimension between vertices.
		 \return Returns null if the creation failed. The reason could be that you
		 specified some invalid parameters, that a mesh with that name already
		 exists, or that a texture could not be found. If successful, a pointer to the mesh is returned.
		 This pointer should not be dropped. See IReferenceCounted::drop() for more information. */
		virtual IAnimatedMesh* addTerrainMesh(const c8* meshname,
			video::IImage* texture, video::IImage* heightmap,
			const core::dimension2d<f32>& stretchSize = core::dimension2d<f32>(10.0f,10.0f),
			f32 maxHeight=200.0f,
			const core::dimension2d<s32>& defaultVertexBlockSize = core::dimension2d<s32>(64,64)) = 0;

		//! add a static arrow mesh to the meshpool
		/** \param name Name of the mesh
		\param vtxColor0 color of the cylinder
		\param vtxColor1 color of the cone
		\param tesselationCylinder Number of quads the cylinder side consists of
		\param tesselationCone Number of triangles the cone's roof consits of
		\param height Total height of the arrow
		\param cylinderHeight Total height of the cylinder, should be lesser than total height
		\param width0 Diameter of the cylinder
		\param width1 Diameter of the cone's base, should be not smaller than the cylinder's diameter */
		virtual IAnimatedMesh* addArrowMesh(const c8* name,
				video::SColor vtxColor0=0xFFFFFFFF,
				video::SColor vtxColor1=0xFFFFFFFF,
				u32 tesselationCylinder=4, u32 tesselationCone=8,
				f32 height=1.f, f32 cylinderHeight=0.6f,
				f32 width0=0.05f, f32 width1=0.3f) = 0;

		//! add a static sphere mesh to the meshpool
		/** \param name Name of the mesh
		\param radius Radius of the sphere
		\param polyCountX Number of quads used for the horizontal tiling
		\param polyCountY Number of quads used for the vertical tiling */
		virtual IAnimatedMesh* addSphereMesh(const c8* name,
				f32 radius=5.f, u32 polyCountX = 16,
				u32 polyCountY = 16) = 0;

		//! Returns the root scene node.
		/** This is the scene node which is parent
		 of all scene nodes. The root scene node is a special scene node which
		 only exists to manage all scene nodes. It will not be rendered and cannot
		 be removed from the scene.
		 \return Returns a pointer to the root scene node. */
		virtual ISceneNode* getRootSceneNode() = 0;

		//! Returns the first scene node with the specified id.
		/** \param id: The id to search for
		 \param start: Scene node to start from. All children of this scene
		 node are searched. If null is specified, the root scene node is
		 taken.
		 \return Returns pointer to the first scene node with this id,
		 and null if no scene node could be found. */
		virtual ISceneNode* getSceneNodeFromId(s32 id, ISceneNode* start=0) = 0;

		//! Returns the first scene node with the specified name.
		/** \param name: The name to search for
		 \param start: Scene node to start from. All children of this scene
		 node are searched. If null is specified, the root scene node is
		 taken.
		 \return Returns pointer to the first scene node with this id,
		 and null if no scene node could be found. */
		virtual ISceneNode* getSceneNodeFromName(const c8* name, ISceneNode* start=0) = 0;

		//! Returns the first scene node with the specified type.
		/** \param type: The type to search for
		 \param start: Scene node to start from. All children of this scene
		 node are searched. If null is specified, the root scene node is
		 taken.
		 \return Returns pointer to the first scene node with this type,
		 and null if no scene node could be found. */
		virtual ISceneNode* getSceneNodeFromType(scene::ESCENE_NODE_TYPE type, ISceneNode* start=0) = 0;

		//! Get scene nodes by type.
		/** \param type: Type of scene node to find (ESNT_ANY will return all child nodes).
		\param outNodes: array to be filled with results.
		\param start: Scene node to start from. All children of this scene
		node are searched. If null is specified, the root scene node is
		taken. */
		virtual void getSceneNodesFromType(ESCENE_NODE_TYPE type,
				core::array<scene::ISceneNode*>& outNodes,
				ISceneNode* start=0) = 0;

		//! Get the current active camera.
		/** \return The active camera is returned. Note that this can be NULL, if there
		 was no camera created yet. */
		virtual ICameraSceneNode* getActiveCamera() = 0;

		//! Sets the currently active camera.
		/** The previous active camera will be deactivated.
		 \param camera: The new camera which should be active. */
		virtual void setActiveCamera(ICameraSceneNode* camera) = 0;

		//! Sets the color of stencil buffers shadows drawn by the scene manager.
		virtual void setShadowColor(video::SColor color = video::SColor(150,0,0,0)) = 0;

		//! Returns the current color of shadows.
		virtual video::SColor getShadowColor() const = 0;

		//! Registers a node for rendering it at a specific time.
		/** This method should only be used by SceneNodes when they get a
		 ISceneNode::OnRegisterSceneNode() call.
		 \param node: Node to register for drawing. Usually scene nodes would set 'this'
		 as parameter here because they want to be drawn.
		 \param pass: Specifies when the node wants to be drawn in relation to the other nodes.
		 For example, if the node is a shadow, it usually wants to be drawn after all other nodes
		 and will use ESNRP_SHADOW for this. See scene::E_SCENE_NODE_RENDER_PASS for details.
		 \return scene will be rendered ( passed culling ) */
		virtual u32 registerNodeForRendering(ISceneNode* node,
			E_SCENE_NODE_RENDER_PASS pass = ESNRP_AUTOMATIC) = 0;

		//! Draws all the scene nodes.
		/** This can only be invoked between
		 IVideoDriver::beginScene() and IVideoDriver::endScene(). Please note that
		 the scene is not only drawn when calling this, but also animated
		 by existing scene node animators, culling of scene nodes is done, etc. */
		virtual void drawAll() = 0;

		//! Creates a rotation animator, which rotates the attached scene node around itself.
		/** \param rotationPerSecond: Specifies the speed of the animation
		 \return The animator. Attach it to a scene node with ISceneNode::addAnimator()
		 and the animator will animate it.
		 If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		 See IReferenceCounted::drop() for more information. */
		virtual ISceneNodeAnimator* createRotationAnimator(const core::vector3df& rotationPerSecond) = 0;

		//! Creates a fly circle animator, which lets the attached scene node fly around a center.
		/** \param center: Center of the circle.
		 \param radius: Radius of the circle.
		 \param speed: Specifies the speed of the flight.
		 \param direction: Specifies the upvector used for alignment of the mesh.
		 \return The animator. Attach it to a scene node with ISceneNode::addAnimator()
		 and the animator will animate it.
		 If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		 See IReferenceCounted::drop() for more information. */
		virtual ISceneNodeAnimator* createFlyCircleAnimator(
				const core::vector3df& center=core::vector3df(0.f,0.f,0.f),
				f32 radius=100.f, f32 speed=0.001f,
				const core::vector3df& direction=core::vector3df(0.f, 1.f, 0.f)) = 0;

		//! Creates a fly straight animator, which lets the attached scene node fly or move along a line between two points.
		/** \param startPoint: Start point of the line.
		 \param endPoint: End point of the line.
		 \param timeForWay: Time in milli seconds how long the node should need to
		 move from the start point to the end point.
		 \param loop: If set to false, the node stops when the end point is reached.
		 If loop is true, the node begins again at the start.
		 \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		 and the animator will animate it.
		 If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		 See IReferenceCounted::drop() for more information. */
		virtual ISceneNodeAnimator* createFlyStraightAnimator(const core::vector3df& startPoint,
			const core::vector3df& endPoint, u32 timeForWay, bool loop=false) = 0;

		//! Creates a texture animator, which switches the textures of the target scene node based on a list of textures.
		/** \param textures: List of textures to use.
		 \param timePerFrame: Time in milliseconds, how long any texture in the list
		 should be visible.
		 \param loop: If set to to false, the last texture remains set, and the animation
		 stops. If set to true, the animation restarts with the first texture.
		 \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		 and the animator will animate it.
		 If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		 See IReferenceCounted::drop() for more information. */
		virtual ISceneNodeAnimator* createTextureAnimator(const core::array<video::ITexture*>& textures,
			s32 timePerFrame, bool loop=true) = 0;

		//! Creates a scene node animator, which deletes the scene node after some time automatically.
		/** \param timeMs: Time in milliseconds, after when the node will be deleted.
		 \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		 and the animator will animate it.
		 If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		 See IReferenceCounted::drop() for more information. */
		virtual ISceneNodeAnimator* createDeleteAnimator(u32 timeMs) = 0;

		//! Creates a special scene node animator for doing automatic collision detection and response.
		/** See ISceneNodeAnimatorCollisionResponse for details.
		 \param world: Triangle selector holding all triangles of the world with which
		 the scene node may collide. You can create a triangle selector with
		 ISceneManager::createTriangleSelector();
		 \param sceneNode: SceneNode which should be manipulated. After you added this animator
		 to the scene node, the scene node will not be able to move through walls and is
		 affected by gravity.
		 \param ellipsoidRadius: Radius of the ellipsoid with which collision detection and
		 response is done. If you have got a scene node, and you are unsure about
		 how big the radius should be, you could use the following code to determine
		 it:
		 \code
		 const core::aabbox3d<f32>& box = yourSceneNode->getBoundingBox();
		 core::vector3df radius = box.MaxEdge - box.getCenter();
		 \endcode
		 \param gravityPerSecond: Sets the gravity of the environment, as an acceleration in 
		 units per second per second. If your units are equivalent to metres, then 
		 core::vector3df(0,-10.0f,0) would give an approximately realistic gravity.
		 You can disable gravity by setting it to core::vector3df(0,0,0).
		 \param ellipsoidTranslation: By default, the ellipsoid for collision detection is created around
		 the center of the scene node, which means that the ellipsoid surrounds
		 it completely. If this is not what you want, you may specify a translation
		 for the ellipsoid.
		 \param slidingValue: DOCUMENTATION NEEDED.
		 \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		 and the animator will cause it to do collision detection and response.
		 If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		 See IReferenceCounted::drop() for more information. */
		virtual ISceneNodeAnimatorCollisionResponse* createCollisionResponseAnimator(
			ITriangleSelector* world, ISceneNode* sceneNode,
			const core::vector3df& ellipsoidRadius = core::vector3df(30,60,30),
			const core::vector3df& gravityPerSecond = core::vector3df(0,-10.0f,0),
			const core::vector3df& ellipsoidTranslation = core::vector3df(0,0,0),
			f32 slidingValue = 0.0005f) = 0;

		//! Creates a follow spline animator.
		/** The animator modifies the position of
		 the attached scene node to make it follow a hermite spline.
		 It uses a subset of hermite splines: either cardinal splines
		 (tightness != 0.5) or catmull-rom-splines (tightness == 0.5).
		 The animator moves from one control point to the next in
		 1/speed seconds. This code was sent in by Matthias Gall. */
		virtual ISceneNodeAnimator* createFollowSplineAnimator(s32 startTime,
			const core::array< core::vector3df >& points,
			f32 speed = 1.0f, f32 tightness = 0.5f) = 0;

		//! Creates a simple ITriangleSelector, based on a mesh.
		/** Triangle selectors
		 can be used for doing collision detection. Don't use this selector
		 for a huge amount of triangles like in Quake3 maps.
		 Instead, use for example ISceneManager::createOctTreeTriangleSelector().
		 Please note that the created triangle selector is not automaticly attached
		 to the scene node. You will have to call ISceneNode::setTriangleSelector()
		 for this. To create and attach a triangle selector is done like this:
		 \code
		 ITriangleSelector* s = sceneManager->createTriangleSelector(yourMesh,
				yourSceneNode);
		 yourSceneNode->setTriangleSelector(s);
		 s->drop();
		 \endcode
		 \param mesh: Mesh of which the triangles are taken.
		 \param node: Scene node of which visibility and transformation is used.
		 \return Returns the selector, or null if not successful.
		 If you no longer need the selector, you should call ITriangleSelector::drop().
		 See IReferenceCounted::drop() for more information. */
		virtual ITriangleSelector* createTriangleSelector(IMesh* mesh, ISceneNode* node) = 0;

		//! Creates a simple dynamic ITriangleSelector, based on a axis aligned bounding box.
		/** Triangle selectors
		 can be used for doing collision detection. Every time when triangles are
		 queried, the triangle selector gets the bounding box of the scene node,
		 an creates new triangles. In this way, it works good with animated scene nodes.
		 \param node: Scene node of which the bounding box, visibility and transformation is used.
		 \return Returns the selector, or null if not successful.
		 If you no longer need the selector, you should call ITriangleSelector::drop().
		 See IReferenceCounted::drop() for more information. */
		virtual ITriangleSelector* createTriangleSelectorFromBoundingBox(ISceneNode* node) = 0;

		//! Creates a meta triangle selector.
		/** A meta triangle selector is nothing more than a
		 collection of one or more triangle selectors providing together
		 the interface of one triangle selector. In this way,
		 collision tests can be done with different triangle soups in one pass.
		 \return Returns the selector, or null if not successful.
		 If you no longer need the selector, you should call ITriangleSelector::drop().
		 See IReferenceCounted::drop() for more information. */
		virtual IMetaTriangleSelector* createMetaTriangleSelector() = 0;

		//! Returns a pointer to the scene collision manager.
		virtual ISceneCollisionManager* getSceneCollisionManager() = 0;

		//! Returns a pointer to the mesh manipulator.
		virtual IMeshManipulator* getMeshManipulator() = 0;

		//! Adds a scene node to the deletion queue.
		/** The scene node is immediatly
		 deleted when it's secure. Which means when the scene node does not
		 execute animators and things like that. This method is for example
		 used for deleting scene nodes by their scene node animators. In
		 most other cases, a ISceneNode::remove() call is enough, using this
		 deletion queue is not necessary.
		 See ISceneManager::createDeleteAnimator() for details.
		 \param node: Node to detete. */
		virtual void addToDeletionQueue(ISceneNode* node) = 0;

		//! Posts an input event to the environment.
		/** Usually you do not have to
		 use this method, it is used by the internal engine. */
		virtual bool postEventFromUser(const SEvent& event) = 0;

		//! Clears the whole scene.
		/** All scene nodes are removed. */
		virtual void clear() = 0;

		//! Returns interface to the parameters set in this scene.
		/** String parameters can be used by plugins and mesh loaders.
		 For example the CMS and LMTS loader want a parameter named 'CSM_TexturePath'
		 and 'LMTS_TexturePath' set to the path were attached textures can be found. See
		 CSM_TEXTURE_PATH, LMTS_TEXTURE_PATH, MY3D_TEXTURE_PATH,
		 COLLADA_CREATE_SCENE_INSTANCES, DMF_TEXTURE_PATH and DMF_USE_MATERIALS_DIRS*/
		virtual io::IAttributes* getParameters() = 0;

		//! Returns current render pass.
		/** All scene nodes are being rendered in a specific order.
		 First lights, cameras, sky boxes, solid geometry, and then transparent
		 stuff. During the rendering process, scene nodes may want to know what the scene
		 manager is rendering currently, because for example they registered for rendering
		 twice, once for transparent geometry and once for solid. When knowing what rendering
		 pass currently is active they can render the correct part of their geometry. */
		virtual E_SCENE_NODE_RENDER_PASS getSceneNodeRenderPass() const = 0;

		//! Creates a new scene manager.
		/** This can be used to easily draw and/or store two
		independent scenes at the same time. The mesh cache will be
		shared between all existing scene managers, which means if you
		load a mesh in the original scene manager using for example
		getMesh(), the mesh will be available in all other scene
		managers too, without loading.
		The original/main scene manager will still be there and
		accessible via GeoDevice::getSceneManager(). If you need
		input event in this new scene manager, for example for FPS
		cameras, you'll need to forward input to this manually: Just
		implement an IEventReceiver and call
		yourNewSceneManager->postEventFromUser(), and return true so
		that the original scene manager doesn't get the event.
		Otherwise, all input will go to the main scene manager
		automatically.
		If you no longer need the new scene manager, you should call
		ISceneManager::drop().
		See IReferenceCounted::drop() for more information. */
		virtual ISceneManager* createNewSceneManager(bool cloneContent=false) = 0;

		//! Sets ambient color of the scene
		virtual void setAmbientLight(const video::SColorf &ambientColor) = 0;

		//! Returns ambient color of the scene
		virtual const video::SColorf& getAmbientLight() const = 0;
	};


} // end namespace scene
} // end namespace geo

#endif

