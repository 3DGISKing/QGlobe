#pragma once

#include "SMesh.h"
#include "CDynamicMeshBuffer.h"
#include "GDM_TextureMgr.h"
#include "ISceneNode.h"
#include <QtCore/QHash>

typedef struct _gdemRenderGeoParam
{
	int								nIndex;			// tile index
	geo::video::S3DVertex2TCoords*	pVertexBuf;		// vertex buff
	int								nVertexCount;	// vertexCount							
	unsigned short*					pIndexBuf;		// pIndexBuff
	int								nIdBufCount;	// indexBuffCount;
	unsigned long					nTextureID;		// texture ID;
	unsigned long					nOverlayTextureID;		// Overlay texture ID;
}GDEM_RENDER_GEO_PARAM;

#define GDM_GIS_MAX_ANGLE	80

namespace geo
{
	namespace scene
	{
		class CGeoTerrainSceneNode : public ISceneNode
		{
		public:
			//! constructor
			CGeoTerrainSceneNode(ISceneNode* parent, ISceneManager* mgr,	s32 id,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

			virtual ~CGeoTerrainSceneNode(void);

		public:
			void setHeightTexture(video::ITexture* texture) { heightTexture = texture;}
			void setEarthTexture(video::ITexture* texture) {earthTexture = texture;}
			void setRateAngleTexture(video::ITexture* texture) {rateAngleTexture = texture;}
			void setRatePlanTexture(video::ITexture* texture) {ratePlanTexture = texture;}
			void setRasterNoDataTexture(video::ITexture* texture) {rasterNodataTexture = texture;}

			void enableLighting(bool enable=true);

			//set fog params
			virtual void setFogParams(bool fogEnable, f32 fogStart, f32 fogEnd) {
				FogEnable = fogEnable;
				FogStart = fogStart;
				FogEnd = fogEnd;
			}
			void	SetFont(CGeoFont* pFont) {m_pFont = pFont;}

		protected:
			//! frame
			virtual void OnRegisterSceneNode();

			//! Renders the node.
			virtual void render();

			virtual const core::aabbox3d<f32>& getBoundingBox() const;
		private:
			bool    Initialize();
			void    drawMark();
			void    DrawRasterSymbol();
			void    CalcuFog();
			void	DrawOutlineString( float x, float y, QString& str, float scale );
			void	Draw2DLine( float x1, float y1, float x2, float y2 );
			void    PreRender();
			void    AfterRender();

            SMesh               Mesh;
			CDynamicMeshBuffer* RenderBuffer;

			CGeoFont*		m_pFont;
			video::SColor	m_lineColor;
			
			video::ITexture* heightTexture;
			video::ITexture* earthTexture;
			video::ITexture* rasterNodataTexture;
			video::ITexture* rateAngleTexture;
			video::ITexture* ratePlanTexture;

			QHash<QString,video::ITexture*> _RasterSymbolHash;

			s32     MaxTileSize;
			bool	FogEnable;
			f32		FogStart;
			f32		FogEnd;
		};
	}
}
#include "GDM_RenderTile.h"

extern GDM_MESH_TILE_ARR g_gdm_MeshTileArr;
extern void gdm_GetPyramidDivisionTiles(GDM_MESH_TILE_ARR *pArr);
extern TerrainQuadTreeList *pWriteQuadList;
extern TerrainQuadTreeList *pReadQuadList;

