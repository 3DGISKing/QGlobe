//GeoMeshSceneNode.cpp

#include "SampleMeshSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "../gdem_libmain/GDM_DataMgr.h"
#include "../gdem_libmain/GDM_Camera.h"
#include "../gdem_terrainengine/COpenGLDriver.h"
#include "../gdem_terrainengine/IMeshManipulator.h"
#include "../gdem_terrainengine/S3DVertex.h"
#include "../gdem_terrainengine/SMeshBuffer.h"

namespace geo
{
namespace scene			   
{
	CSampleMeshSceneNode::CSampleMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, const core::vector3df& position)
		: ISceneNode(parent, mgr, id, position)
	{
		WorldMesh=NULL;
		LocalMesh=NULL;
		LightingEnabled=true;
	}

	CSampleMeshSceneNode::~CSampleMeshSceneNode()
	{
		if (WorldMesh)
			WorldMesh->drop();
	}
	
	void CSampleMeshSceneNode::render()
	{
		if(this->LocalMesh==NULL) return;
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		if (!WorldMesh || !driver)
			return;

		constructMesh();

		driver->setTransform(video::ETS_WORLD,core::IdentityMatrix);

		for (u32 i=0; i<WorldMesh->getMeshBufferCount(); ++i)
		{
			scene::IMeshBuffer* mb = WorldMesh->getMeshBuffer(i);
			scene::IMeshBuffer* mb1 =LocalMesh->getMeshBuffer(i);

			if(mb)
			{
				video::SMaterial& material = mb1->getMaterial() ;

				for (u32 i=0; i<video::MATERIAL_MAX_TEXTURES; ++i)
					material.TextureLayer[i].TextureWrap =video:: ETC_REPEAT;

				if(material.MaterialType==video::EMT_TRANSPARENT_ALPHA_CHANNEL)
				{
					driver->setAllowZWriteOnTransparent(true);
				}
				
				video::SMaterial tmpmat;

				tmpmat=material;

				if(LightingEnabled && material.Lighting)
					tmpmat.Lighting=true;
				else
					tmpmat.Lighting=false;

				driver->setMaterial(tmpmat);
				
				driver->drawMeshBuffer(mb);
			}
		}
		
		renderCoord();
	}

	void CSampleMeshSceneNode::renderCoord()
	{
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		video::SMaterial mat;
		mat.Lighting = false;
		mat.Wireframe = true;
		mat.ZBuffer = true;
		mat.Thickness=2;
		driver->setMaterial(mat);

		core::aabbox3d<f32> box=getBoundingBox();
		float lenghxaxis=box.MaxEdge.X-box.MinEdge.X;
		float lenghyaxis=box.MaxEdge.Y-box.MinEdge.Y;
		float lenghzaxis=box.MaxEdge.Z-box.MinEdge.Z;

		//render Coord

		float axisheight=lenghzaxis/2.0f;

		float orglength=this->Coord.m_org.getLength();

		float deltax=this->Coord.m_org.m_tX/orglength*axisheight;
		float deltay=this->Coord.m_org.m_tY/orglength*axisheight;
		float deltaz=this->Coord.m_org.m_tZ/orglength*axisheight;


		core::vector3df x=core::vector3df(this->Coord.m_org.m_tX+this->Coord.m_x.m_tX*lenghxaxis+deltax,
			                              this->Coord.m_org.m_tY+this->Coord.m_x.m_tY*lenghxaxis+deltay,
			                              this->Coord.m_org.m_tZ+this->Coord.m_x.m_tZ*lenghxaxis+deltaz);

		core::vector3df y=core::vector3df(this->Coord.m_org.m_tX+this->Coord.m_y.m_tX*lenghyaxis+deltax,
			                              this->Coord.m_org.m_tY+this->Coord.m_y.m_tY*lenghyaxis+deltay,
			                              this->Coord.m_org.m_tZ+this->Coord.m_y.m_tZ*lenghyaxis+deltaz);
		
		core::vector3df z=core::vector3df(this->Coord.m_org.m_tX+this->Coord.m_z.m_tX*lenghzaxis+deltax,
			                              this->Coord.m_org.m_tY+this->Coord.m_z.m_tY*lenghzaxis+deltay,
			                              this->Coord.m_org.m_tZ+this->Coord.m_z.m_tZ*lenghzaxis+deltaz)	;

		core::vector3df start(this->Coord.m_org.m_tX+deltax-g_OffsetBodyPt.m_tX,g_OffsetBodyPt.m_tY-this->Coord.m_org.m_tY-deltay,this->Coord.m_org.m_tZ+deltaz-g_OffsetBodyPt.m_tZ);
		core::vector3df end(x.X-g_OffsetBodyPt.m_tX,g_OffsetBodyPt.m_tY-x.Y,x.Z-g_OffsetBodyPt.m_tZ);
		driver->draw3DLine(start,end,video::SColor(255,255,0,0));

		start.set(this->Coord.m_org.m_tX+deltax-g_OffsetBodyPt.m_tX,g_OffsetBodyPt.m_tY-this->Coord.m_org.m_tY-deltay,this->Coord.m_org.m_tZ+deltaz-g_OffsetBodyPt.m_tZ)	;
		end.set(y.X-g_OffsetBodyPt.m_tX,g_OffsetBodyPt.m_tY-y.Y,y.Z-g_OffsetBodyPt.m_tZ);
		driver->draw3DLine(start,end,video::SColor(255,0,255,0));


		start.set(this->Coord.m_org.m_tX+deltax-g_OffsetBodyPt.m_tX,g_OffsetBodyPt.m_tY-this->Coord.m_org.m_tY-deltay,this->Coord.m_org.m_tZ+deltaz-g_OffsetBodyPt.m_tZ)	;
		end.set(z.X-g_OffsetBodyPt.m_tX,g_OffsetBodyPt.m_tY-z.Y,z.Z-g_OffsetBodyPt.m_tZ);
		driver->draw3DLine(start,end,video::SColor(255,0,0,255));
	}

	//°ê¹¢»ô¶®Áì±ËËæº· ¼ÑÊÌ¼ê ·ô»Õ this->SourceMesh¶¦ ¶®Ë¦Â×ÊÞ 
	// ¶Ã³ó¶µÂ×²÷´ç Â­ÊýÂÙ world°á°£Ì© ¼°Ëæµá this->MeshÌ© »ô¶®Áì´ÉË¾ °éºÂÂÙ³Þ.
	//³ÞËÁ this->MeshÌ© ¸âºº¹ÖÀË¶£´ÉË¾ °éºÂÂÙ³Þ.
	//this->MeshÌ© ¸ÆË©´Ù¸Çº÷¶¦ ±½»¦ÂÙ³Þ.

	void CSampleMeshSceneNode::constructMesh()
	{
		CGDM_Vector3DF worldpt;
		
		core::aabbox3df meshbox;

	   bool initmeshbox=false;

	  
	   for(unsigned int i=0;i<this->LocalMesh->getMeshBufferCount();i++)
	   { 
		   SMeshBuffer* sourcemeshbuffer=(SMeshBuffer*)this->LocalMesh->getMeshBuffer(i);
		   SMeshBuffer* meshbuffer=(SMeshBuffer*)this->WorldMesh->getMeshBuffer(i);
			    
		   core::aabbox3df bufferbox;
		   bool initbufferbox=false;
		   for(unsigned int j=0;j<sourcemeshbuffer->getVertexCount();j++)
		   { 
			   //Ë§ºº °ê¹¢»ô¶®Áì±ËÌ© °ªË¾ ÊÐ²÷³Þ.
			   worldpt.m_tX=sourcemeshbuffer->getPosition(j).X;
			   worldpt.m_tY=sourcemeshbuffer->getPosition(j).Y;
			   worldpt.m_tZ=sourcemeshbuffer->getPosition(j).Z;

			   //Ì»´Å»ô¶®Áì±ËËæµá ¼­ÃûÂÙ³Þ.
			   this->Transform.transformVect(worldpt);
			  
			   //¾ò¸¡µ¹Ì© Ì¡¾·¶¦ °ÖµØÂÙ³Þ.
			   meshbuffer->getPosition(j).X=worldpt.m_tX-g_OffsetBodyPt.m_tX;
			   meshbuffer->getPosition(j).Y=g_OffsetBodyPt.m_tY-worldpt.m_tY;
			   meshbuffer->getPosition(j).Z=worldpt.m_tZ-g_OffsetBodyPt.m_tZ;

			   meshbuffer->Vertices[j].Color=meshbuffer->getMaterial().DiffuseColor;

			   if(initbufferbox==false)
			   {
				   bufferbox.reset( meshbuffer->getPosition(j));
				   initbufferbox=true;
			   }
			   else
				 bufferbox.addInternalPoint( meshbuffer->getPosition(j));
		   }
		   if(initmeshbox)
		      meshbuffer->setBoundingBox(bufferbox);

		   if(meshbuffer->getVertexCount()==0)
			   continue;

		   if(initmeshbox==false)
		   {
			    meshbox.reset(bufferbox);
				initmeshbox=true;
		   }
		   else
			    meshbox.addInternalBox(bufferbox);
	   }

	   //¸âºº¹ÖÀË¶£¶¦ ±Ëº¤ÂÙ³Þ.
	   video::IVideoDriver* driver = SceneManager->getVideoDriver();
	   driver->getMeshManipulator()->recalculateNormals(this->WorldMesh);

	   for(unsigned int i=0;i<this->LocalMesh->getMeshBufferCount();i++)
	   { 
		   SMeshBuffer* meshbuffer=(SMeshBuffer*)this->WorldMesh->getMeshBuffer(i);

		   for(unsigned int j=0;j<meshbuffer->getVertexCount();j++)
			   meshbuffer->Vertices[j].Normal*=-1;
	   }

	   this->WorldMesh->setBoundingBox(meshbox);
	}

	void CSampleMeshSceneNode::OnRegisterSceneNode()
	{
		SceneManager->registerNodeForRendering(this);
	}

	const core::aabbox3d<f32>& CSampleMeshSceneNode::getBoundingBox() const
	{
		return  WorldMesh->getBoundingBox() ;
	}

	void CSampleMeshSceneNode::setLocalMesh(IMesh* mesh) 
	{
		this->LocalMesh=mesh;
		mesh->grab();

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		IMesh* copymesh=driver->getMeshManipulator()->createMeshCopy(mesh);

		this->WorldMesh=copymesh;
	}

	void CSampleMeshSceneNode::clear()
	{
		LocalMesh->drop();
		WorldMesh->drop();
		LocalMesh=WorldMesh=NULL;
	}

	void CSampleMeshSceneNode::EnableLighting(bool enable/* =true */)
	{
		LightingEnabled=enable;
	}
}
}