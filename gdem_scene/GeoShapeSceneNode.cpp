
#include "GeoShapeSceneNode.h"
#include "ISceneManager.h"
#include "SColor.h"
#include "vector3d.h"
#include "GDM_Mem.h"
#include "gdem_protocol.h"
#include "COpenGLTexture.h"
#include "GDM_Tile.h"
#include "GIS_GenerateTexture.h"
#include "../gdem_base/GDM_Math.h"
#include "GIS_TextureMgr.h"
#include "GIS_TextureIDCache.h"

#include "../gdem_client/gdemclient.h"
#include "../gdem_client/config.h"

extern int	gdm_GetGeoShapeInfo(int* pIndex, PolyLinesegment* pGeoShapeArray,GDM_TILE_INFO& tileinfo);
extern void get3dVertexLinesegmentArray(const GDM_LOGIC_TILE* pTile, PolyLinesegment* pGeoShapeArray);
extern void get2dVertexLinesegmentArray(const GDM_LOGIC_TILE* pTile, PolyLinesegment* pGeoShapeArray);
namespace geo
{
	namespace scene
	{
		// constructor
		CGeoShapeSceneNode::CGeoShapeSceneNode(ISceneNode* parent, ISceneManager* mgr, 
			s32 id, const core::vector3df& position,
			const core::vector3df& rotation, const core::vector3df& scale)
			: ISceneNode(parent, mgr, id, position)
		{
#ifdef _DEBUG
			setDebugName("CGeoShapeSceneNode");
#endif
			setAutomaticCulling(scene::EAC_OFF);

		}

		CGeoShapeSceneNode::~CGeoShapeSceneNode()
		{
			for (int i = 0; i < MAX_POLYGON_COUNT; i++)
				_PolyList.polyList[i].pts.clear();
		}

		//! pre render event
		void CGeoShapeSceneNode::OnRegisterSceneNode()
		{
			SceneManager->registerNodeForRendering(this);
		}

		//added by RSH 2013.3.13 from  hope project	UGI
		///*
		#define EARTH_CIRCUMFERENCE 40000000
		inline void CGeoShapeSceneNode::ToggleColorType(int * type)
		{
			if (*type == 0) *type = 1;
			else *type = 0;
			return;
		}
		void CGeoShapeSceneNode::DrawRailway(LineSegment* line, video::IVideoDriver* driver, int level, int * lastType)
		{
			geo::f32 space = EARTH_CIRCUMFERENCE/(1<<(level+5));
			geo::video::SColor color[2];
			geo::f32 fWidth = getWidthFrom(line->type, color[0]);
			glLineWidth(fWidth);
			color[1].set(255,255,255,255);

			geo::f32 dist, remainedDist = 0;
			geo::core::array<geo::core::vector3df> ptArrSegment;
			geo::core::vector3df prevPt, curPt;

			ptArrSegment.push_back(line->pts[0]);
			prevPt = line->pts[0];

			for (unsigned int k = 1; k<line->pts.size(); k++)
			{
				curPt  = line->pts[k];
				dist = prevPt.getDistanceFrom(curPt); 
				remainedDist += dist;
				if (space > remainedDist)
				{
					ptArrSegment.push_back(curPt);
					prevPt = curPt;
				}	
				else
				{
					geo::core::vector3df pt;
					geo::core::vector3df normalVect = curPt - prevPt;
					normalVect.normalize();
					pt = (space - remainedDist + dist)*normalVect;
					pt += prevPt;
					ptArrSegment.push_back(pt);
					prevPt = pt;
					driver->draw3DPolyline(ptArrSegment, false, color[*lastType]);
					ptArrSegment.set_used(0);

					ptArrSegment.push_back(pt);
					ToggleColorType(lastType);

					int num = (int)((remainedDist - space)/space);
					remainedDist = remainedDist - (num + 1)*space;

					for (int m = 1; m < num+1; m++)
					{
						pt = prevPt + space*normalVect;
						ptArrSegment.push_back(pt);
						driver->draw3DPolyline(ptArrSegment, false, color[*lastType]);
						ptArrSegment.set_used(0);
						prevPt = pt;

						ptArrSegment.push_back(pt);
						ToggleColorType(lastType);
					}
					if (remainedDist > 0)
					{
						ptArrSegment.push_back(curPt);
						prevPt = curPt;
					}
				}
			}
			driver->draw3DPolyline(ptArrSegment, false, color[*lastType]);
			ptArrSegment.set_used(0);
			ToggleColorType(lastType);
		}
		//*/
		//end addition

		void CGeoShapeSceneNode::render()
		{
			if(g_app->GetConfig()->m_IsLicence==false) return;

			video::IVideoDriver* driver = SceneManager->getVideoDriver();

			geo::s32 nTileIndex = 0;
			geo::video::SColor color(255, 255, 255, 0);

			video::SMaterial mat;
			mat.Lighting = false;
			mat.Wireframe = true;
			mat.ZBuffer = true;
			driver->setMaterial(mat);

			const GDM_LOGIC_TILE *ptile;	
			
			//added by RSH 2013.3.13
			int LastRailType = 0;
			//end addition

			const GDM_LOGIC_TILE_ARR *render_tile_list= gdm_GetRenderTiles();

			for(int i=0;i<render_tile_list->nCount;i++)
			{
				ptile=render_tile_list->pData+i;//i¸ÜÉã µã¶®À°ËÎË¾ ÊÐ²÷³Þ.
				
				_PolyList.nCount=0;

				//¼ÑÊÌ¼ê µã¶®À°ËÎË¾ ¶®Ë¦Â×ÊÞ 3½ÓÌº»ô¶®Áì´Éµá °éºÂ´ô _PolyList¶¦ ÊÐ²÷³Þ.
				get3dVertexLinesegmentArray(ptile,&_PolyList);

				for(s32 index = 0 ; index < _PolyList.nCount; index++)
				{
					
					//added by RSH 2013.3.13
					if (_PolyList.polyList[index].type == E_GDM_SUBDATA_TYPE_SHAPE_NARROWRAILWAY ||
						_PolyList.polyList[index].type == E_GDM_SUBDATA_TYPE_SHAPE_WIDERAILWAY)
					{
						DrawRailway(&_PolyList.polyList[index], driver, ptile->nLevel, &LastRailType);
						_PolyList.polyList[index].pts.set_used(0);
						continue;
					}
					//end
				
					geo::f32 fWidth = getWidthFrom(_PolyList.polyList[index].type, color);
					glLineWidth(fWidth);

					driver->draw3DPolyline(_PolyList.polyList[index].pts, false, color);
					
#ifdef DEBUG			
					/***************************************************************************************
					µ¹ËÍÌ© ½ë¼°±á ¶ë¼è¶ì¼°Ë¾ ³Þ¶¥»°Ëºµá °û¶®²÷ ¿¸´ÅÀË¶ì
					µ¹ËÍÌ© ·ô¼°Ëæ 4°¢ÂôË¾ °û¶®²÷ ¿¸´ÅÀË¶ì*/

					if(g_app->GetConfig()->m_isShowShapeNodes)
					{
						glPointSize(fWidth+5);
						for(unsigned int i=1;i<_PolyList.polyList[index].pts.size()-1;i++)
							driver->draw3DPoint(_PolyList.polyList[index].pts[i],geo::video::SColor(255,255,255,255));

						driver->draw3DPoint(_PolyList.polyList[index].pts[0],geo::video::SColor(255,255,0,0));
						driver->draw3DPoint(_PolyList.polyList[index].pts[_PolyList.polyList[index].pts.size()-1],geo::video::SColor(255,0,0,255));
					}
#endif			
					_PolyList.polyList[index].pts.set_used(0);
				}
			}

#ifdef DEBUG
			if(g_app->GetConfig()->m_isShowRenderTileBoundry)
				DrawRenderTilesBoundry();
#endif
		}

		geo::f32 CGeoShapeSceneNode::getWidthFrom(geo::s32 nType, geo::video::SColor& color)
		{
			geo::f32 fWidth = 1.0f;

			//added by RSH 2013.3.16

			if (nType < E_GDM_SUBDATA_TYPE_SHAPE_COUNTRY || nType > E_GDM_SUBDATA_TYPE_SHAPE_FORESTLAND)
			{
#ifdef DEBUG	
				qDebug("Bug is in getWidthFrom");
#endif
				return fWidth;
			}

			int n = nType - (int)E_GDM_SUBDATA_TYPE_SHAPE_COUNTRY;
			fWidth = g_pGDMDataMgr->m_sOption.m_geoShapeAttr[n].width;
			color.set(g_pGDMDataMgr->m_sOption.m_geoShapeAttr[n].color);
			return fWidth;
		}

		//! returns the axis aligned bounding box of this node
		const core::aabbox3d<f32>& CGeoShapeSceneNode::getBoundingBox() const
		{
			return Box;
		}

		void CGeoShapeSceneNode::DrawRenderTilesBoundry()
		{
			double baseOffset = gdm_GetDataMgr()->m_pCamera->m_location.m_dDist / 200.0;
			const GDM_LOGIC_TILE_ARR *p_render_tile_list;
			p_render_tile_list = gdm_GetRenderTiles();

			const GDM_LOGIC_TILE *pTile;

			video::IVideoDriver* driver = SceneManager->getVideoDriver();
			geo::video::SColor color(255,255,0,255);
			glLineWidth(1);

			for(int i=0;i<p_render_tile_list->nCount;i++)
			{
				pTile=p_render_tile_list->pData+i;

				//draw 
				double tileWidth = gdm_TwoPI / (1 << pTile->nLevel);

				double minLongitude = tileWidth * pTile->nX - gdm_PI;
				double minLatitude	= tileWidth * pTile->nY - gdm_PI;

				geo::core::array<geo::core::vector3df> pts;

				GDM_POINT3D pt;
				geo::core::vector3df vertex;

				double drawgap=tileWidth/30.0f;
				int division=tileWidth/drawgap;
				double height;
				for(double longi=minLongitude;longi<=minLongitude+tileWidth; longi=longi+drawgap)
				{
					height=gdm_getExactHeight(longi,minLatitude);
					gdm_get3DBodyPoint(longi,minLatitude,pt,height+baseOffset);
					vertex.X = pt.m_X;
					vertex.Y = - pt.m_Y;
					vertex.Z = pt.m_Z;	
					pts.push_back(vertex);					  

				}

				for(double lat=minLatitude;lat<=minLatitude+tileWidth; lat=lat+drawgap)
				{
					height=gdm_getExactHeight(minLongitude+tileWidth,lat);
					gdm_get3DBodyPoint(minLongitude+tileWidth ,lat,pt,height+baseOffset);
					vertex.X = pt.m_X;
					vertex.Y = - pt.m_Y;
					vertex.Z = pt.m_Z;	
					pts.push_back(vertex);					  
				}

				driver->draw3DPolyline(pts,false,color);
			}
		}

	} // end namespace scene
} // end namespace geo
