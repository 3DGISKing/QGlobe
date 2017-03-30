//GeoMeshSceneNode.cpp

#include "Geo3dObjectSceneNode.h"

#include "GDM_SceneManager.h"

#include "../gdem_client/gdemclient.h"
#include "../gdem_client/config.h"

#include "../gdem_libmain/GDM_3DObjectCache.h"
#include "../gdem_libmain/GDM_Camera.h"
#include "../gdem_libmain/GDM_DataMgr.h"
#include "../gdem_libmain/GDM_SimpleBuildingCache.h"

#include "../gdem_terrainengine/S3DVertex.h"
#include "../gdem_terrainengine/COpenGLDriver.h"
#include "../gdem_terrainengine/IMeshManipulator.h"
#include "../gdem_terrainengine/IVideoDriver.h"
#include "../gdem_terrainengine/ISceneManager.h"
#include "../gdem_terrainengine/SAnimatedMesh.h"
#include "../gdem_terrainengine/SGeoMesh.h"
#include "../gdem_terrainengine/SMeshBuffer.h"
#include "../gdem_render/GeoMeshUtil.h"
#include "../gdem_libmain/GDM_PyramidMgr.h"


qint64 g_Focused3dObjectID=-1;
namespace geo
{
	using namespace video;
	using namespace core ;

namespace scene			   
{	
	#define MAX_INDEX_COUNT   9000
	#define MAX_VERTEX_COUNT  3000

	struct RenderData
	{
		array<S3DVertex>    vertexarr;
		array<u16>			indexarr;
	};
  
	RenderData    g_RenderData;
  	
	CGeo3dObjectSceneNode::CGeo3dObjectSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, const core::vector3df& position)
		: ISceneNode(parent, mgr, id, position)
	{
		this->RenderBuffer=new SMeshBuffer;
		this->RenderBuffer->Vertices.set_used(MAX_VERTEX_COUNT);
		this->RenderBuffer->Indices.set_used(MAX_INDEX_COUNT);

		mat.Lighting=false;
		mat.Thickness=0.3f;
		
		TopColor.set(255,150,150,150);
		SideColor.set(255,120,120,120);
		TopOutLineColor.set(255,190,190,190);   //white
		FloorOutLineColor.set(255,140,140,140);
		SideOutLineColor.set(255,190,190,190);
		FloorFactor=3;
		WaterColor.set(100,0,0,255);           //blue
		SelectColor.set(255,255,0,0);	       //yellow
		SearchedColor.set(255,255,255,0);      //red
		SelectedID=-1;
		NewBuilding=NULL;
		LightingEnabled=true;
	}

	CGeo3dObjectSceneNode::~CGeo3dObjectSceneNode()
	{
		if (RenderBuffer)
			RenderBuffer->drop();
	}
	
	void CGeo3dObjectSceneNode::render()
	{
		if(g_app->GetConfig()->m_IsLicence==false) return;

		simpleBuildingRender();
		newBuildingRender();

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setTransform(video::ETS_WORLD,core::IdentityMatrix);

		const GDM_LOGIC_TILE *ptile;
		GDM_TILE* meshtile;
		SAnimatedMesh* meshs;
		SGeoMesh* mesh;
		SMeshBuffer* meshbuffer;

		const GDM_LOGIC_TILE_ARR *render_tile_list= gdm_GetRenderTiles();

		for(int i=0;i<render_tile_list->nCount;i++)
		{
			ptile=render_tile_list->pData+i;//i¸ÜÉã µã¶®À°ËÎË¾ ÊÐ²÷³Þ.

			for(int j=0;j<ptile->n3dMeshTileCnt;j++)
			{
				meshtile=ptile->sArr3DMeshTile[j];
				meshs=(SAnimatedMesh*)g_pGDMDataMgr->m_p3DObjectCache->GetMeshs(meshtile->m_sInfo.nX,
					                                                            meshtile->m_sInfo.nY,
																				meshtile->m_sInfo.nLevel,
																				meshtile->m_pBuffer);

				for (unsigned int k=0;k<meshs->Meshes.size();k++)
				{
					mesh=(SGeoMesh*)meshs->Meshes[k];
					
					if(!mesh->Selected)
					if(mesh->GeoInfo.Longitude < ptile->minLongitude || mesh->GeoInfo.Longitude > ptile->maxLongitude
						|| mesh->GeoInfo.Latitude < ptile->minLatitude || mesh->GeoInfo.Latitude > ptile->maxLatitude)
						continue;

					GDM_LOCATION loc;

					loc.m_dLongitude=mesh->GeoInfo.Longitude;
					loc.m_dLatitude=mesh->GeoInfo.Latitude;

					/*******************************
					DEMÌ© ²ÚËË¶¦ »ô´´¼¬Ëºµá °ÖµØÂ×±Ê²÷¼è²÷ ´¸°Ö¸ó»ô
					********************************/
					if(g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
						loc.m_dDist= mesh->GeoInfo.Height*g_pGDMDataMgr->m_sOption.dem_detail_rate;
					else
						loc.m_dDist =0;

					//³ÞËÁ ËË Ì¡¾·Ëæº· Áì¼Ó»ô¶®Áì±Ë¶¦ ÊÐ²÷³Þ.
					GDM_Coord3D standCoord;
					gdm_GetStandardCoord(&loc, &standCoord);

					//ËË »ô¶®Áì±Ë¶¦ ¸ÒÌ¡°¢¶í¿Ñ Ãä¼­»¤¿Ö³Þ.
					gdm_RotateAroundZAxis(&standCoord,mesh->GeoInfo.Azimuth*gdm_DEGTORAD);

					//Êï¹¶½¤Àâ¶¦ Ì»´Å»ô¶®Áì±ËËæµá ²¿±¨²÷ ¸êÃûÃÔµÛË¾ ÊÐ²÷³Þ.

					gdm_MatrixTCStoSCS(&standCoord, &this->Transform );

					CGDM_Aabbox3DF boundingbox;

					boundingbox.MinEdge.set(mesh->BoundingBox.MinEdge.X,
						                    mesh->BoundingBox.MinEdge.Y,
											mesh->BoundingBox.MinEdge.Z);

					boundingbox.MaxEdge.set(mesh->BoundingBox.MaxEdge.X,
											mesh->BoundingBox.MaxEdge.Y,
											mesh->BoundingBox.MaxEdge.Z);

					this->Transform.transformBoxEx(boundingbox);

					if(!g_pGDMDataMgr->m_pPyramidMgr->Intersect(boundingbox))
						continue;


					for(unsigned int l=0;l<mesh->getMeshBufferCount();l++)
					{
						meshbuffer=(SMeshBuffer*)mesh->getMeshBuffer(l);
						TransformForRender(meshbuffer);

						for (u32 i=0; i<video::MATERIAL_MAX_TEXTURES; ++i)
							meshbuffer->Material.TextureLayer[i].TextureWrap =video:: ETC_REPEAT;

						if(meshbuffer->Material.MaterialType==video::EMT_TRANSPARENT_ALPHA_CHANNEL)
						{
							driver->setAllowZWriteOnTransparent(true);
						}
						
						SMaterial tmpmat;

						tmpmat=meshbuffer->Material;

						if(LightingEnabled && meshbuffer->Material.Lighting)
							tmpmat.Lighting=true;
						else
							tmpmat.Lighting=false;

						tmpmat.FogEnable=false;

						driver->setMaterial(tmpmat);
			
						driver->drawVertexPrimitiveList(this->RenderBuffer->Vertices.pointer(),
														this->RenderBuffer->getVertexCount(),
														this->RenderBuffer->Indices.pointer(),
														this->RenderBuffer->getIndexCount()/3,
														RenderBuffer->getVertexType(),
														EPT_TRIANGLES, 
														RenderBuffer->getIndexType() );
					}

					if(mesh->Selected)
					{
						DrawBoundingBox(mesh);

						 //code snippt for drawing screen rect
						/*video::SMaterial mat;
						mat.Wireframe=true;
						mat.Lighting=false;
						driver->setMaterial(mat);
						core::rect<s32> screenrect;
						GeoMeshUtil::CalculateScreenRect(mesh,screenrect);
						driver->draw2DRectangle(video::SColor(255,255,0,0),screenrect);*/
					}
				}
			}
		}
	}


	//·Æº¡¶¦ Ì¡ÃÍ RenderBuffer¶¦ °éºÂÂÙ³Þ

	void CGeo3dObjectSceneNode::TransformForRender(SMeshBuffer* meshbuffer)
	{
		CGDM_Vector3DF worldpt;

		//¼³¼°±¶ºã¶íÂÙ µßÊßÃú¸ó
		this->RenderBuffer->Vertices.set_used(meshbuffer->getVertexCount());

		//¼³¼°Ì© »ô¶®ÁìÌ® Á¢º÷½î·ôÂ±»ô¶®Áì¶¦ ¸ôº¡
		for(unsigned int j=0;j<meshbuffer->getVertexCount();j++)
		{   
			worldpt.m_tX=meshbuffer->getPosition(j).X;
			worldpt.m_tY=meshbuffer->getPosition(j).Y;
			worldpt.m_tZ=meshbuffer->getPosition(j).Z;

			this->Transform.transformVect(worldpt);

			RenderBuffer->getPosition(j).X=worldpt.m_tX-g_OffsetBodyPt.m_tX;
			RenderBuffer->getPosition(j).Y=g_OffsetBodyPt.m_tY-worldpt.m_tY;
			RenderBuffer->getPosition(j).Z=worldpt.m_tZ-g_OffsetBodyPt.m_tZ;

			RenderBuffer->getTCoords(j).X=meshbuffer->getTCoords(j).X;
			RenderBuffer->getTCoords(j).Y=meshbuffer->getTCoords(j).Y;

			RenderBuffer->Vertices[j].Color=meshbuffer->getMaterial().DiffuseColor;
		}

		//ËÍ´èº÷¶¦ Ì¡ÂÙ µßÊßÃú¸ó
		this->RenderBuffer->Indices.set_used(meshbuffer->getIndexCount());
		memcpy(this->RenderBuffer->Indices.pointer(),
			   meshbuffer->Indices.pointer(),sizeof(u16)*meshbuffer->getIndexCount());

		//¸âºº¹ÖÀË¶£±Ëº¤
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->getMeshManipulator()->recalculateNormals(RenderBuffer,true);

		for(unsigned int j=0;j<RenderBuffer->getVertexCount();j++)
			RenderBuffer->Vertices[j].Normal*=-1;
	}

	//precondition
	//Transform memeber of this class must setted for this meshbuffer
	//Offset member must be updated for rendering

	void CGeo3dObjectSceneNode::DrawBoundingBox(SMesh *mesh)
	{
		 core::aabbox3df localbox=mesh->getBoundingBox();

		 CGDM_Aabbox3DF worldbox(localbox.MinEdge.X,localbox.MinEdge.Y,localbox.MinEdge.Z,
								 localbox.MaxEdge.X,localbox.MaxEdge.Y,localbox.MaxEdge.Z);

		 CGDM_Vector3DF edges[8];

		 worldbox.getEdges(edges);
		
		 //slightly larger

		 for(int i=0;i<8;i++)
			edges[i]*=1.05f;

		 for(int i=0;i<8;i++)
			 this->Transform.transformVect(edges[i]);

		 SMeshBuffer boundingbox;

		 video::SColor clr(0,50,0,0);

		 float x,y,z;

		 for(int i=0;i<8;i++)
		 {
			 x=edges[i].m_tX-g_OffsetBodyPt.m_tX;
			 y=g_OffsetBodyPt.m_tY-edges[i].m_tY;
			 z=edges[i].m_tZ-g_OffsetBodyPt.m_tZ;

			 boundingbox.Vertices.push_back(video::S3DVertex(x,y,z, -1,-1,-1, clr, 0, 1));
		 }

		 /*
		 Edges are stored in this way:
		 Hey, am I an ascii artist, or what? :) niko.
		    /3--------/7
		   /  |      / |
		  /   |     /  |
		 1---------5   |
		 |  2- - - | - 6
		 | /       |  /
		 |/        | /
		 0---------4/
		 */

		 video::IVideoDriver* driver = SceneManager->getVideoDriver();

		 video::SMaterial mat;
		 mat.Lighting=false;
		 mat.MaterialType=EMT_TRANSPARENT_VERTEX_ALPHA;

		 driver->setMaterial(mat);

		 const u16 u[36] = {   5,1,3,   5,3,7,   5,7,6,   5,6,4,   0,2,4,   4,2,6, 
							   1,3,2,   1,2,0,   2,3,7,   2,7,6,   0,1,5,   0,5,4};

		 boundingbox.Indices.set_used(36);

		 for (u32 i=0; i<36; ++i)
			 boundingbox.Indices[i] = u[i];

		 driver->drawMeshBuffer(&boundingbox);

		 clr.set(255,255,255,0);

		 driver->draw3DLine(boundingbox.Vertices[0].Pos,boundingbox.Vertices[4].Pos,clr);
		 driver->draw3DLine(boundingbox.Vertices[4].Pos,boundingbox.Vertices[5].Pos,clr);
		 driver->draw3DLine(boundingbox.Vertices[5].Pos,boundingbox.Vertices[1].Pos,clr);
		 driver->draw3DLine(boundingbox.Vertices[1].Pos,boundingbox.Vertices[0].Pos,clr);

		 driver->draw3DLine(boundingbox.Vertices[5].Pos,boundingbox.Vertices[7].Pos,clr);
		 driver->draw3DLine(boundingbox.Vertices[7].Pos,boundingbox.Vertices[6].Pos,clr);
		 driver->draw3DLine(boundingbox.Vertices[6].Pos,boundingbox.Vertices[4].Pos,clr);

		 driver->draw3DLine(boundingbox.Vertices[7].Pos,boundingbox.Vertices[3].Pos,clr);
		 driver->draw3DLine(boundingbox.Vertices[3].Pos,boundingbox.Vertices[2].Pos,clr);
		 driver->draw3DLine(boundingbox.Vertices[2].Pos,boundingbox.Vertices[6].Pos,clr);

		 driver->draw3DLine(boundingbox.Vertices[3].Pos,boundingbox.Vertices[1].Pos,clr);
		 driver->draw3DLine(boundingbox.Vertices[2].Pos,boundingbox.Vertices[0].Pos,clr);
	}

	void CGeo3dObjectSceneNode::OnRegisterSceneNode()
	{
		SceneManager->registerNodeForRendering(this);
	}

	const core::aabbox3d<f32>& CGeo3dObjectSceneNode::getBoundingBox() const
	{
		return  Mesh->getBoundingBox() ;
	}

	void CGeo3dObjectSceneNode::simpleBuildingRender()
	{
		const GDM_LOGIC_TILE_ARR *pBaseArr = gdm_GetRenderTiles();
		const GDM_LOGIC_TILE *pTile;
	 
		BuildingArray* pBuildingArr;

		GDM_POINT3D	pos;
		S3DVertex  v,offset;

		offset.Pos.X=  -g_OffsetBodyPt.m_tX; offset.Pos.Y	= g_OffsetBodyPt.m_tY; offset.Pos.Z=  -g_OffsetBodyPt.m_tZ;

		video::IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setMaterial(mat);

		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE);
		
		RenderedIDs.clear();

		for(int i= 0; i < pBaseArr->nCount; i++)
		{
			pTile = &pBaseArr->pData[i];

			for(int j=0;j<pTile->nPoygonTileCnt;j++)
			{
				mat.Wireframe=true;	 driver->setMaterial(mat);
				
				GDM_TILE *polyTile=pTile->sArrPolygonTile[j];

				pBuildingArr=g_pGDMDataMgr->m_pSimpleBuildingCache->GetBuildingArray(polyTile);

				if(!pBuildingArr)		continue;

				g_RenderData.vertexarr.set_used(0);
				g_RenderData.indexarr.set_used(0);
				g_RenderData.vertexarr.push_back(offset);

				for(unsigned int k=0;k<pBuildingArr->Count();k++)
				{
					SimpleBuilding* pbuilding=(SimpleBuilding*)pBuildingArr->GetBuilding(k);

					if(RenderedIDs.contains(pbuilding->GetID()))
						continue;
					else
						RenderedIDs.push_back(pbuilding->GetID());

					/*if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tX < pTile->minLongitude) continue;
					if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMinPt.m_tY < pTile->minLatitude) continue;
					if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tX > pTile->maxLongitude) continue;
					if(psimplebuilding->_pBottomPolygon->m_cBound.m_cMaxPt.m_tY > pTile->maxLatitude) continue;
*/
					double height=pbuilding->GetFloor()*FloorFactor+pbuilding->GetHeight();

					bool iswater=false,searched=false,selected=false;
					SColor color;
					color.set(255,SideColor.getRed()+3*pbuilding->GetFloor(),
						          SideColor.getGreen(),
								  SideColor.getBlue());

					if(height<0)
					{
						height=0.5;
						iswater=true;
						color=WaterColor;
					}
										
					for(int c=0;c<SearchedIDs.size();c++)
					{
						if(pbuilding->GetID()==SearchedIDs[c])
						{
							searched=true;
							color=SearchedColor;
							break;
						}
					}

					if(pbuilding->GetID()==SelectedID)
					{
						selected=true;
						color=SelectColor;
					}

					for(unsigned int l=0;l<pbuilding->GetBottomPolygon()->RingCount();l++)  
					{
						URing *pring=pbuilding->GetBottomPolygon()->GetRing(l);
						
						for(unsigned int m=0;m<pring->_LocationArr.size();m++)    
						{
							gdm_get3DBodyPoint(pring->_LocationArr[m].m_tX, pring->_LocationArr[m].m_tY	, pos, height);

							v.Pos.X=pos.m_X; 	v.Pos.Y=-pos.m_Y;	v.Pos.Z=pos.m_Z; 	v.Color=color;
																					
							g_RenderData.vertexarr.push_back(v);

							g_RenderData.indexarr.push_back(0);
							g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-1);

							if(m==pring->_LocationArr.size()-1)   
								g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-m-1);
							else
								g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size());

							driver->draw3DLine(g_RenderData.vertexarr[g_RenderData.vertexarr.size()-1].Pos,g_RenderData.vertexarr[0].Pos,SideOutLineColor);
						}

						for(int n=1;n<=pbuilding->GetFloor();n++)
						{
							double h=pbuilding->GetHeight()+n*FloorFactor;

							TopOutline.set_used(0);

							for(unsigned int m=0;m<pring->_LocationArr.size();m++)    
							{
								gdm_get3DBodyPoint(pring->_LocationArr[m].m_tX, pring->_LocationArr[m].m_tY	, pos, h);

								v.Pos.X=pos.m_X; 	v.Pos.Y=-pos.m_Y;	v.Pos.Z=pos.m_Z; 	
							
								TopOutline.push_back(v.Pos);
							}
						   if(n==pbuilding->GetFloor())
								driver->draw3DPolyline(TopOutline,true,TopOutLineColor);
						   else
							    driver->draw3DPolyline(TopOutline,true,FloorOutLineColor);
						}

						if(!iswater && !searched && !selected)
						{
							color.set(255,TopColor.getRed()+3*pbuilding->GetFloor(),
										TopColor.getGreen(),
										TopColor.getBlue());	
						}
						for(unsigned int m=0;m<pring->_CoverMeshArr.size();m++)
						{
							gdm_get3DBodyPoint(pring->_CoverMeshArr[m].pt1.m_tX, pring->_CoverMeshArr[m].pt1.m_tY, pos, height);

							v.Pos.X=pos.m_X; 	v.Pos.Y=-pos.m_Y;	v.Pos.Z=pos.m_Z; v.Color=color;
							g_RenderData.vertexarr.push_back(v);

							gdm_get3DBodyPoint(pring->_CoverMeshArr[m].pt2.m_tX, pring->_CoverMeshArr[m].pt2.m_tY, pos, height);

							v.Pos.X=pos.m_X; 	v.Pos.Y=-pos.m_Y;	v.Pos.Z=pos.m_Z; 	v.Color=color;
							g_RenderData.vertexarr.push_back(v);
							
							gdm_get3DBodyPoint(pring->_CoverMeshArr[m].pt3.m_tX, pring->_CoverMeshArr[m].pt3.m_tY, pos, height);

							v.Pos.X=pos.m_X; 	v.Pos.Y=-pos.m_Y;	v.Pos.Z=pos.m_Z; v.Color=color;
							
							g_RenderData.vertexarr.push_back(v);

							g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-3);
							g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-2);
							g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-1);
						}	 
					}
				}

				mat.Wireframe=false;
				driver->setMaterial(mat);
				
				driver->drawVertexPrimitiveList(g_RenderData.vertexarr.pointer(),
												g_RenderData.vertexarr.size(),
												g_RenderData.indexarr.pointer(),
												g_RenderData.indexarr.size()/3,
												video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
			}
		}
	}

	void CGeo3dObjectSceneNode::newBuildingRender()
	{
		if(!NewBuilding) return;

		GDM_POINT3D	pos;
		S3DVertex  v,offset;

		offset.Pos.X=  -g_OffsetBodyPt.m_tX; offset.Pos.Y	= g_OffsetBodyPt.m_tY; offset.Pos.Z=  -g_OffsetBodyPt.m_tZ;

		video::IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setMaterial(mat);

		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE);

		g_RenderData.vertexarr.set_used(0);
		g_RenderData.indexarr.set_used(0);
		g_RenderData.vertexarr.push_back(offset);

		double height=NewBuilding->GetFloor()*FloorFactor+NewBuilding->GetHeight();
		SColor color=SelectColor;

		for(unsigned int l=0;l<NewBuilding->GetBottomPolygon()->RingCount();l++)  
		{
			URing *pring=NewBuilding->GetBottomPolygon()->GetRing(l);

			TopOutline.set_used(0);
			for(unsigned int m=0;m<pring->_LocationArr.size();m++)    
			{
				gdm_get3DBodyPoint(pring->_LocationArr[m].m_tX, pring->_LocationArr[m].m_tY	, pos, height);

				v.Pos.X=pos.m_X; 	v.Pos.Y=-pos.m_Y;	v.Pos.Z=pos.m_Z; 	v.Color=color;

				g_RenderData.vertexarr.push_back(v);

				g_RenderData.indexarr.push_back(0);
				g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-1);

				if(m==pring->_LocationArr.size()-1)   
					g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-m-1);
				else
					g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size());

				driver->draw3DLine(g_RenderData.vertexarr[g_RenderData.vertexarr.size()-1].Pos,g_RenderData.vertexarr[0].Pos,SideOutLineColor);

				TopOutline.push_back(g_RenderData.vertexarr[g_RenderData.vertexarr.size()-1].Pos);
			}

			driver->draw3DPolyline(TopOutline,true,TopOutLineColor);

			
			for(unsigned int m=0;m<pring->_CoverMeshArr.size();m++)
			{
				gdm_get3DBodyPoint(pring->_CoverMeshArr[m].pt1.m_tX, pring->_CoverMeshArr[m].pt1.m_tY, pos, height);

				v.Pos.X=pos.m_X; 	v.Pos.Y=-pos.m_Y;	v.Pos.Z=pos.m_Z; v.Color=color;
				g_RenderData.vertexarr.push_back(v);

				gdm_get3DBodyPoint(pring->_CoverMeshArr[m].pt2.m_tX, pring->_CoverMeshArr[m].pt2.m_tY, pos, height);

				v.Pos.X=pos.m_X; 	v.Pos.Y=-pos.m_Y;	v.Pos.Z=pos.m_Z; 	v.Color=color;
				g_RenderData.vertexarr.push_back(v);

				gdm_get3DBodyPoint(pring->_CoverMeshArr[m].pt3.m_tX, pring->_CoverMeshArr[m].pt3.m_tY, pos, height);

				v.Pos.X=pos.m_X; 	v.Pos.Y=-pos.m_Y;	v.Pos.Z=pos.m_Z; v.Color=color;

				g_RenderData.vertexarr.push_back(v);

				g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-3);
				g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-2);
				g_RenderData.indexarr.push_back(g_RenderData.vertexarr.size()-1);
			}	 
		}

		mat.Wireframe=false;
		driver->setMaterial(mat);

		driver->drawVertexPrimitiveList(g_RenderData.vertexarr.pointer(),
			g_RenderData.vertexarr.size(),
			g_RenderData.indexarr.pointer(),
			g_RenderData.indexarr.size()/3,
			video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
	}

	void CGeo3dObjectSceneNode::EnableLighting(bool enable/* =true */)
	{
		LightingEnabled=enable;
	}
}
}