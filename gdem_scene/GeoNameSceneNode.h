
#ifndef __C_GDM_GEO_NAME_NODE_H_INCLUDE__
#define __C_GDM_GEO_NAME_NODE_H_INCLUDE__

#include "ISceneNode.h"
#include "ISceneCollisionManager.h"
#include "GeoFont.h"
#include <QString>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
extern FT_Library ftlibrary_;
extern bool FTinitialized;

namespace geo
{
	namespace scene
	{

		class CGeoNameSceneNode : public ISceneNode
		{
		public:

			// constructor
			CGeoNameSceneNode(ISceneNode* aParent, ISceneManager* aSceneMgr, s32 aId,
				const core::vector3df& aPos = core::vector3df(0, 0, 0), 
				video::SColor aColor = video::SColor(255, 255, 255, 255));
			
			
			virtual ~CGeoNameSceneNode();

			virtual void OnRegisterSceneNode();

			virtual void render();

			virtual void renderImage(video::ITexture* aTex, core::position2d<f32>& aPos, core::rect<f32>& aRc);
			virtual const geo::core::aabbox3d<f32>& getBoundingBox() const;

			virtual void setText(QString text);
			virtual void setTextColor(video::SColor color);
			virtual void changeFont(QString szKorFileName, QString szEngFileName, unsigned int flag);

			CGeoFont*	 GetGeoFont() {return m_pKorFont;}


		private:
			void			initFont(QString szKorFileName, QString szEngFileName, unsigned int flag);
			void 			DestroyFonts();
			void			InitTextures();
			void			DestroyTextures();
			video::SColor	GetAniColor(unsigned long time);
			geo::video::ITexture*	AddTexture(QString name);


		private:
			QString								m_szText;
			video::SColor						m_Color;
			geo::core::aabbox3d<f32>			m_Box;

			geo::video::ITexture*				m_pPyongyangTex;
			geo::video::ITexture*				m_pPlaceTex;
			geo::video::ITexture*				m_pCrossTex;
			geo::video::ITexture*				m_pSearchTex;
			geo::video::ITexture*				m_pSearchHighlightTex;

			CGeoFont*							m_pKorFont;
			CGeoFont*							m_pEngFont;

			float								m_fFontScale;

			void*								_UserData;		

			unsigned int						m_fontflag;
		};

	} //end namespace scene
} // end namespace geo

#endif
