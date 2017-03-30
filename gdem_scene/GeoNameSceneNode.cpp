#include <QApplication>
#include "GeoNameSceneNode.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "GLee.h"
#include "gdem_protocol.h"
#include "gdemclientbase.h"
#include "COpenGLTexture.h"
#include "GDM_DataMgr.h"
#include "GDM_TextMgr.h"
#include "os.h"
#include "GDM_TextureMgr.h"
#include "Layer.h"
#include "../gdem_gis/GIS_Node.h"
#include "../gdem_gis/GIS_Polygon.h"
#include "../gdem_gis/GIS_Area.h"
#include "Geo3dObjectSceneNode.h"
#include "../gdem_render/GDM_CommandMgr.h"
#include "../gdem_client/gdemclient.h"
#include "../gdem_libmain/Building.h"
#include "../gds_database/GDSDatabaseInterface.h"
#include "../gds_database/GDSGeoNameDBManager.h"

#define GDEM_CREATE_FONT_SIZE	20

namespace geo
{
	#define COLOR_YELLOW		video::SColor(255, 255, 255, 20)
	#define COLOR_WHITE			video::SColor(255, 255, 255, 255)
	#define COLOR_GREEN			video::SColor(255, 100, 255, 100)
	#define COLOR_SOFT_BLUE		video::SColor(255, 50, 255, 255)
	#define COLOR_BLUE			video::SColor(255, 20, 150, 255)
	#define COLOR_MAGEN			video::SColor(255, 255, 20, 255)
	#define COLOR_GRAY			video::SColor(255, 200, 200, 255)
	#define COLOR_RED			video::SColor(255, 255, 0, 0)

	namespace scene
	{

		CGeoNameSceneNode::CGeoNameSceneNode(ISceneNode* aParent, ISceneManager* aSceneMgr, s32 aId,
			const core::vector3df& aPos, video::SColor aColor)
			: ISceneNode(aParent, aSceneMgr, aId, aPos)
			, m_Color(aColor)
		{
#ifdef _DEBUG
			setDebugName("CGeoNameSceneNode");
#endif

			setAutomaticCulling(scene::EAC_OFF);

			_UserData		= NULL;

			m_pPyongyangTex = NULL;
			m_pCrossTex		= NULL;
			m_pSearchTex			= NULL;
			m_pSearchHighlightTex	= NULL;

			m_pKorFont		= NULL;
			m_pEngFont		= NULL;

			m_fontflag		= 0;

			
			InitTextures();
			if (FTinitialized==false)
			{
				int res = FT_Init_FreeType(&ftlibrary_);
				Q_ASSERT(res==0);
				res = 0;
				FTinitialized = true;
			}
		}

		CGeoNameSceneNode::~CGeoNameSceneNode(void)
		{
			DestroyFonts();
			DestroyTextures();
			if(FTinitialized)
			{
				FTinitialized=false;
				FT_Done_FreeType(ftlibrary_);
			}
		}

		void CGeoNameSceneNode::initFont(QString szKorFileName, QString szEngFileName, unsigned int flag)
		{
			m_pKorFont	= new CGeoFont(GDEM_CREATE_FONT_SIZE, szKorFileName);
			m_pEngFont	= new CGeoFont(GDEM_CREATE_FONT_SIZE + 4, szEngFileName);

			m_fFontScale = 1.0f / GDEM_CREATE_FONT_SIZE;
			m_fontflag = flag;
		}

		void CGeoNameSceneNode::changeFont(QString szKorFileName, QString szEngFileName, unsigned int flag)
		{
			DestroyFonts();
			initFont(szKorFileName, szEngFileName, flag);
		}

		void CGeoNameSceneNode::DestroyFonts()
		{
			if(m_pKorFont)
			{
				delete m_pKorFont;
				m_pKorFont = NULL;
			}
			if(m_pEngFont)
			{
				delete m_pEngFont;
				m_pEngFont = NULL;
			}
		}

		geo::video::ITexture* CGeoNameSceneNode::AddTexture(QString resName)
		{
			QString path;
	
			QString workDir = gdm_GetWorkPath();

			path = workDir+resName;
			
			return SceneManager->getVideoDriver()->getTexture(path.toUtf8());
		}

		void CGeoNameSceneNode::InitTextures()
		{
			m_pPyongyangTex = AddTexture("/res/scene/pyongyang.png");
			m_pCrossTex = AddTexture("/res/scene/crosshair.png");
			m_pSearchTex = AddTexture("/res/scene/cross-hairs.png");
			m_pSearchHighlightTex = AddTexture("/res/scene/cross-hairs_highlight.png");
			m_pPlaceTex= AddTexture("/res/icon/pushpin/ylw-pushpin.png");
			
			SceneManager->getVideoDriver()->makeColorKeyTexture(m_pPyongyangTex, core::position2d<s32>(0,0));
			SceneManager->getVideoDriver()->makeColorKeyTexture(m_pCrossTex, core::position2d<s32>(0,0));
			SceneManager->getVideoDriver()->makeColorKeyTexture(m_pSearchTex, core::position2d<s32>(0,0));
			SceneManager->getVideoDriver()->makeColorKeyTexture(m_pSearchHighlightTex, core::position2d<s32>(0,0));
					
			if (m_pPyongyangTex)
				m_pPyongyangTex->grab();
			if (m_pCrossTex)
				m_pCrossTex->grab();
			if (m_pSearchTex)
				m_pSearchTex->grab();
			if (m_pSearchHighlightTex)
				m_pSearchHighlightTex->grab();

			Layer* layer;
			QList<Layer*> layerlist=g_pGDMDataMgr->m_LayerHash.values();

			foreach(layer,layerlist)
			{
				if(layer->GetMainType()==E_GDM_MAINDATA_TYPE_NAME)
				{
					QString iconfilename=layer->GetIconFileName();

					if(iconfilename.isEmpty()) continue;

					PlaceNameLayer* namelayer=(PlaceNameLayer*)layer;

					video::ITexture* tex=NULL;
					
					tex=AddTexture("/res"+iconfilename);
					if(tex)
					{
						SceneManager->getVideoDriver()->makeColorKeyTexture(tex, core::position2d<s32>(0,0));
					}

					namelayer->SetTexture(tex);
				}
			}
		}

		void CGeoNameSceneNode::DestroyTextures()
		{
			if (m_pPyongyangTex)
				m_pPyongyangTex->drop();
			if (m_pCrossTex)
				m_pCrossTex->drop();
			if (m_pSearchTex)
				m_pSearchTex->drop();
			if (m_pSearchHighlightTex)
				m_pSearchHighlightTex->drop();
		}
		void CGeoNameSceneNode::OnRegisterSceneNode()
		{
			SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
		}

#define BLINKSTEP 5
		int g_BlinkIndex=0;

		void CGeoNameSceneNode::render()
		{
			CGDM_TextNodeArr nameList;
			CGDM_TextMgr	*pTextMgr;
			
			QString strName;
			geo::s32 nIndex = 0;
			geo::s32 nCount = 0;
			video::IVideoDriver* driver = SceneManager->getVideoDriver();

		  	video::SMaterial material;
			material.Lighting = false;
			material.BackfaceCulling = false;
			material.FrontfaceCulling = true;

			driver->setMaterial(material);

			glMatrixMode(GL_PROJECTION);

			const core::dimension2d<s32>& renderTargetSize = driver->getScreenSize();
			core::matrix4 m;
			m.buildProjectionMatrixOrthoLH(f32(renderTargetSize.Width), f32(-renderTargetSize.Height), -1.0f, 1.0f);
			m.setTranslation(core::vector3df(-1.f, 1.f, 0.f));
			glLoadMatrixf(m.pointer());

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();			
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);

			geo::video::ITexture* pTexture = NULL;
			int lableSize = RTS_NORMAL;

			pTextMgr = gdm_GetDataMgr()->m_pTextMgr;
			pTextMgr->GetDrawingText(nameList);
			nCount = nameList.size();

			unsigned long time = geo::os::Timer::getRealTime();

			QString iconName;

			for(geo::s32 i = 0; i < nCount; i++)
			{
				GDM_TextCell &nameCell = nameList[i].m_data;
				CGeoFont*	 pFont	   = m_pKorFont;

				if(!nameCell.blKorean)
					pFont	= m_pEngFont;

				if(pFont == NULL)
					continue;

				core::position2d<f32> aPos = core::position2d<f32>(nameCell.posIconX, nameCell.posIconY);
				core::rect<f32> aRc = core::rect<f32>(0.f, 0.f, 18.f, 18.f);

				aRc.LowerRightCorner.X = nameCell.iconSizeX;
				aRc.LowerRightCorner.Y = nameCell.iconSizeY;

				pTexture = NULL;
				m_Color = COLOR_WHITE;
				switch (nameCell.rendertype)
				{
				case RTT_PYONGYANG:
					{
						m_Color = GetAniColor(time);
						pTexture = m_pPyongyangTex;												
					}
					break;
				
				case RTT_PLACE_MARK_NORMAL:
					{
						m_Color = COLOR_WHITE; // White Color
						//by RSH 2013.7.25

						iconName = QString::fromUtf16(nameCell.iconName);

						if(iconName.isEmpty())
						{
							pTexture = m_pPlaceTex;
							break;
						}

						g_TextureListMutex.lock();

						pTexture=AddTexture(iconName);
						SceneManager->getVideoDriver()->makeColorKeyTexture(pTexture, core::position2d<s32>(0,0));
						g_TextureListMutex.unlock();

						if(!pTexture)
							pTexture = m_pPlaceTex;
					}
					break;
				case RTT_PLACE_MARK_SELECT:
					{
						if(g_BlinkIndex < BLINKSTEP)
							m_Color=COLOR_RED;
						else 
							m_Color=COLOR_YELLOW;
					
						//by RSH 2013.7.25

						iconName = QString::fromUtf16(nameCell.iconName);

						if(iconName.isEmpty())
						{
							pTexture = m_pPlaceTex;
							break;
						}

						g_TextureListMutex.lock();

						pTexture=AddTexture(iconName);

						g_TextureListMutex.unlock();

						if(!pTexture)
							pTexture = m_pPlaceTex;
					}
					break;
				case RTT_SEARCH_DATA_NORMAL:
				case RTT_SEARCH_DATA_SELECT:
					{
						pTexture = m_pSearchTex;
				  		
					}
					break;
				case RTT_PLACE_LENGTH:
				case RTT_PLACE_AREA:
					m_Color = COLOR_GREEN;
					break;
				default:
					{
						if(nameCell.datatype!=E_GDM_SUBDATA_TYPE_NONE)
						{
							Layer *layer=g_pGDMDataMgr->m_LayerHash.value(nameCell.datatype);
							Q_ASSERT(layer!=NULL);
							Q_ASSERT(layer->IsPlaceNameLayer());

							PlaceNameLayer* namelayer=(PlaceNameLayer*)layer;
							
							pTexture=(geo::video::ITexture*)namelayer->GetTexture();

							m_Color.setRed(namelayer->GetColor().red());
							m_Color.setGreen(namelayer->GetColor().green());
							m_Color.setBlue(namelayer->GetColor().blue());
						}
					}
					break;
				}

				aRc.LowerRightCorner.X = nameCell.iconSizeX;
				aRc.LowerRightCorner.Y = nameCell.iconSizeY;

				QString strText;
				strText = QString::fromUtf16(nameCell.strName);
				if(nameCell.datatype == E_GDM_SUBDATA_TYPE_NAME_MOUNTAIN && nameCell.focusScale > 0.1)
					strText = QString("%1(%2m)").arg(strText).arg((int)nameCell.height);

				if(nameCell.datatype != E_GDM_SUBDATA_TYPE_NONE && nameCell.focusScale > 0.05)
				{
					CGIS_Node* node=gdm_GetGISNode(nameCell.datatype,nameCell.key);
					
					QString prelen=QApplication::translate("PathDlg", "Length:", 0, QApplication::UnicodeUTF8);
					QString prearea=QApplication::translate("PolygonDlg", "Area:", 0, QApplication::UnicodeUTF8);
					
					if(node && node->Get_GisType()==E_GISNODE_TYPE_PATH)
					{
						CGIS_Polygon* p=(CGIS_Polygon*)node;
						double length = p->m_dLength;
						
						if(length<1000)
							strText=QString("%1(%2%3m)").arg(strText).arg(prelen).arg(length);
						else
							strText=QString("%1(%2%3km)").arg(strText).arg(prelen).arg(length/1000);
					}

					if(node && node->Get_GisType()==E_GISNODE_TYPE_AREA)
					{
						CGIS_Area* p=(CGIS_Area*)node;
						double length = p->m_dLength;
						double area=p->m_dArea;
						
						QString strlen,strarea;

						if(length<1000)
							strlen=QString("%1%2m").arg(prelen).arg(length);
						else
							strlen=QString("%1%2km").arg(prelen).arg(length/1000);

						if (area < 1000000)
							strarea = QString("%1%2m%3").arg(prearea).arg(area).arg(QChar(0x00B2));
						else
							strarea = QString("%1%2km%3").arg(prearea).arg(area/1000000).arg(QChar(0x00B2));
						
						strText=QString("%1(%2,%3)").arg(strText).arg(strlen).arg(strarea);
					}
				}

				if (nameCell.drawing)
				{
					float scale = pTextMgr->GetFontSize(nameCell.fontID) * m_fFontScale * (nameCell.drawScale + nameCell.focusScale);
					float maxscale = pTextMgr->GetFontSize(nameCell.fontID) * m_fFontScale;

					float dx = ((nameCell.right - nameCell.left) / 2.f) * (1.f - (scale / maxscale));
					float dy = ((nameCell.bottom - nameCell.top) / 2.f) * (1.f - (scale / maxscale));

					pFont->printOutlinedString(
						nameCell.posNameX,
						nameCell.posNameY,
						strText,
						false,
						dx,
						dy,
						m_Color,
						m_fontflag,
						scale);	

					if (pTexture)
					{
						renderImage(pTexture, aPos, aRc);
					}
					/*if (nameCell.type == RTT_PLACE_MARK_SELECT ||
						nameCell.type == RTT_SEARCH_DATA_SELECT)
					{
						long nTime = geo::os::Timer::getRealTime();
						if (m_pCrossTex && (nTime >> 9) & 0x01)
						{
							aPos.X -= 9;
							aPos.Y -= 9;
							aRc.LowerRightCorner.X += 18;
							aRc.LowerRightCorner.Y += 18;
							renderImage(m_pCrossTex, aPos, aRc);
						}
					}*/					
				}
								
			}
			glDisable(GL_CULL_FACE);

			if(g_Focused3dObjectID!=-1)
			{
				//®Â®ï§í®õ®Ì®© ¢ü®ë®Ë¢Û®º¢¥®Ì®¡§î¡¦¢ü®¦ ®Ê®Ð§â¢¥§ã®Þ
				CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
				MOUSE_POSITION pos=pCmdMgr->GetCurrentCmd()->GetCurrentLocation();

				GeoBuildingProperty prop;
				if(g_DBManager->GetGeoBuildingData(g_Focused3dObjectID,&prop))
				{
					float y=pos.y;

					if(!prop._Name.isEmpty())
					{
						m_pKorFont->printOutlinedString(pos.x,y,prop._Name,false,0,0,COLOR_YELLOW,0,1.0f);	

					}

				}


			}
			g_BlinkIndex++;
			if(g_BlinkIndex==2*BLINKSTEP)
				g_BlinkIndex=0;
		}

		void CGeoNameSceneNode::renderImage(video::ITexture* aTex, core::position2d<f32>& aPos, core::rect<f32>& aRc)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,
				static_cast<const video::COpenGLTexture*>(aTex)->getOpenGLTextureName());

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			
			glBegin(GL_QUADS);
			glTexCoord2f(0.f, 0.f);
			glVertex2f(GLfloat(aPos.X), GLfloat(aPos.Y));

			glTexCoord2f(1.f, 0.f);
			glVertex2f(GLfloat(aPos.X + aRc.getWidth()), GLfloat(aPos.Y));

			glTexCoord2f(1.f, 1.f);
			glVertex2f(GLfloat(aPos.X + aRc.getWidth()), GLfloat(aPos.Y + aRc.getHeight()));

			glTexCoord2f(0.f, 1.f);
			glVertex2f(GLfloat(aPos.X), GLfloat(aPos.Y + aRc.getHeight()));

			glEnd();
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

		}

		const core::aabbox3d<f32>& CGeoNameSceneNode::getBoundingBox() const
		{
			return m_Box;
		}

		void CGeoNameSceneNode::setText(QString text)
		{
			m_szText = text;
		}
		
		void CGeoNameSceneNode::setTextColor(video::SColor color)
		{
			m_Color = color;
		}
		
		video::SColor CGeoNameSceneNode::GetAniColor(unsigned long time)
		{
			double ang = (time % 5000) / 5000.0 * 2 * PAI;
			u32    clr = 255 * (sin(ang) + 1) / 2;

			video::SColor color(255, 255, clr, clr);

			return color;
		}

	} // end napesace scece
} // end namespace geo
