
#include "SimSceneNode.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "SColor.h"
#include "vector3d.h"
#include "GDM_Mem.h"
#include "gdem_protocol.h"
#include "GDM_SimMgr.h"
#include "GeoFont.h"
#include "COpenGLTexture.h"


ushort TEXT_FLIGHT_SIM_NORTH []	= {0xBD81, 0x0000}; //	TR("N")
ushort TEXT_FLIGHT_SIM_EAST	 [] = {0xB3D9, 0x0000}; //	TR("E")
ushort TEXT_FLIGHT_SIM_SOUTH []	= {0xB0a8, 0x0000}; //	TR("S")
ushort TEXT_FLIGHT_SIM_WEST	 [] = {0xC11C, 0x0000};	//  TR("W")

namespace geo
{
	const float			gTextScale	= 0.8f;

	const video::SColor	gColor		= video::SColor(255, 0, 255, 0);
	const GLfloat		gColorRed	= 0.0f;
	const GLfloat		gColorGreen = 1.0f;
	const GLfloat		gColorBlue	= 0.0f;
	
	typedef core::position2d<f32>	SPoint;
	
	#define ARC_DIV_NUM	20
	float				gArcPoints[(ARC_DIV_NUM+1) * 2];

	namespace scene
	{
		#define SCALE_X(x)	(((x)*(m_simStatus->m_screenWidth))/1280)
		#define SCALE_Y(y)	(((y)*(m_simStatus->m_screenHeight))/956)


		// constructor
		CSimSceneNode::CSimSceneNode(ISceneNode* parent, ISceneManager* mgr, 
			s32 id, const core::vector3df& position)
			: ISceneNode(parent, mgr, id, position)
		{
#ifdef _DEBUG
			setDebugName("CSimSceneNode");
#endif
			setAutomaticCulling(scene::EAC_OFF);

			mat.Lighting = false;
			mat.Wireframe = true;
			mat.ZBuffer = true;
			mat.Thickness = 1;

			m_pFont = NULL;
			m_pSimMgr = NULL;
			m_simStatus	= NULL;
			m_simStatus = (GDM_SIM_STATUS*) malloc( sizeof( GDM_SIM_STATUS ) );

			float angDeg = 30, angStep = 120 / ARC_DIV_NUM, angRad;
			int n = 0;
			for ( int i = 0; i <= ARC_DIV_NUM; i++ )
			{
				angRad = degree2radian( angDeg );
				gArcPoints[n++] = 100 * cos(angRad);
				gArcPoints[n++] = -100 * sin(angRad);
				angDeg += angStep;
			}
		}

		CSimSceneNode::~CSimSceneNode()
		{
			if ( m_simStatus )
				free( m_simStatus );
		}

		//! pre render event
		void CSimSceneNode::OnRegisterSceneNode()
		{
			if (IsVisible)
				SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);

			ISceneNode::OnRegisterSceneNode();
		}


		//! render
		void CSimSceneNode::render()
		{
			if ( m_pSimMgr == NULL || m_pFont == NULL )
				return;


			m_painter = SceneManager->getVideoDriver();
			if ( m_painter == NULL )
				return;
	
			m_painter->setMaterial(mat);

			// Initialize
			m_pSimMgr->GetSimulationStatus( m_simStatus );
			if ( m_simStatus->m_type == SIM_TYPE_NONE )
				return;
			switch ( m_simStatus->m_type )
			{
			case SIM_TYPE_FLIGHT:
				{
					if ( !m_simStatus->m_showHelp )
						return;
					PrePaint();

					PaintFixedArea_Flight();
					PaintSpeedArea_Flight();
					PaintHeadingArea_Flight();
					PaintBankAngleArea_Flight();
					PaintVerticalSpeedArea_Flight();
					PaintAltitudeArea_Flight();
					PaintPitchAngleArea_Flight();
					PaintRudderArea_Flight();
					PaintAileronArea_Flight();
					PaintElevatorArea_Flight();
					PaintThrottleArea_Flight();
					PaintFlapGearArea_Flight();

					PostPaint();
				}
				break;
			case SIM_TYPE_CAR:
				{
					return;
					PrePaint();

					PaintSpeedArea_Car();
					PaintTravelTimeArea_Car();
					PaintTravelDistanceArea_Car();
					PaintDirectionAngleArea_Car();

					PostPaint();
				}
				break;
			default:
				return;
			}

		}
		
		//! returns the axis aligned bounding box of this node
		const core::aabbox3d<f32>& CSimSceneNode::getBoundingBox() const
		{
			return Box;
		}

		void CSimSceneNode::PrePaint()
		{
			core::position2d<s32> p(0,0);
			m_painter->draw2DLine( p, p, video::SColor(0) );
			
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();

			const core::dimension2d<s32>& renderTargetSize = m_painter->getScreenSize();
			core::matrix4 m;
			m.buildProjectionMatrixOrthoLH(f32(renderTargetSize.Width), f32(-renderTargetSize.Height), -1.0f, 1.0f);
			m.setTranslation(core::vector3df(-1.f, 1.f, 0.f));
			glLoadMatrixf(m.pointer());

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);

		}

		void CSimSceneNode::PostPaint()
		{
			glPopAttrib();
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
		}

		void CSimSceneNode::PaintFixedArea_Flight()
		{
			int cx = m_simStatus->m_screenWidth / 2;
			int cy = m_simStatus->m_screenHeight / 2;

			  float airplane[4*2] = {
				cx,				cy, 
				cx-SCALE_X(25), cy+SCALE_Y(20),
				cx,				cy+SCALE_Y(8),
				cx+SCALE_X(25), cy+SCALE_Y(20),
			};
 
			Draw2DPolygon( airplane, 4 );
		}

		void CSimSceneNode::PaintSpeedArea_Flight()
		{
			//draw vertical line
			SPoint top(SCALE_X(150), SCALE_Y(80));
			SPoint bottom(SCALE_X(150), SCALE_Y(360));
			Draw2DLine( top.X, top.Y, bottom.X, bottom.Y );

			//draw triangle
			float triangle[ 3 * 2] = {
				top.X,				bottom.Y - (bottom.Y - top.Y)/2,
				top.X+SCALE_X(15),	bottom.Y - (bottom.Y-top.Y)/2 - SCALE_Y(10),
				top.X+SCALE_X(15),	bottom.Y - (bottom.Y-top.Y)/2 + SCALE_Y(10),
			};

			Draw2DPolygon( triangle, 3 );

			int m_speed = m_simStatus->m_speed;

 			QString text = QString("(%1km/h)").arg(m_speed);
 			float xshift = 0.0f;			
 			DrawOutlineString( SCALE_X(170), SCALE_Y(227), text );
			

			int longLine = (m_speed % 10 == 0 || m_speed % 10 > 5) ? (0) : (1); //long line number

			//draw horizontal line
			for (int i = 0; i < 8; i++)
			{
				float x1, y1, x2, y2;
				y1 = (m_speed % 5 == 0) ? (bottom.Y - i*SCALE_Y(35)) 
					: (bottom.Y - i*SCALE_Y(35) - (5 - m_speed % 5)*SCALE_Y(7));
				x2 = bottom.X;
				y2 = y1;

				if (i % 2 == longLine) //long horizontal line
				{
					x1 = bottom.X - SCALE_X(30);

					//draw text
					int delta = (m_speed % 10 == 0) ? (20) : (10);
					int speed = (m_speed/10) * 10 - delta;
					text = QString("%1").arg(speed + (i/2)*10);
					xshift = - ( gTextScale * m_pFont->getStrLen( text ) );
					DrawOutlineString( x1-SCALE_X(5) + xshift, y1+SCALE_Y(7), text );
				}
				else //short horizontal line
				{
					x1 = bottom.X - SCALE_X(20);
				}

				Draw2DLine( x1, y1, x2, y2 );
			}
		}

		void CSimSceneNode::PaintHeadingArea_Flight()
		{
			//draw horizontal line
			SPoint left(SCALE_X(415), SCALE_Y(30));
			SPoint right(SCALE_X(865), SCALE_Y(30));

			float m_heading = m_simStatus->m_heading;

			int step = (int) (m_heading);
			step = (step < 45) ? (360 + (step - 45)) : (step - 45);
			int longLine = (step % 10 == 0 || step % 10 > 5) ? (0) : (1); //longLine Number
			float xshift = 0.0f;

			//draw vertical line
			for (int i = 0; i < 18; i++)
			{
				float x1, y1, x2, y2;
				x1 = (step % 5 == 0) ? (left.X + i*SCALE_X(25)) 
					: (left.X + i*SCALE_X(25) + (5 - step % 5)*SCALE_X(5));
				x2 = x1;
				y2 = left.Y;

				if (i % 2 == longLine) //long vertical line
				{
					y1 = left.Y + SCALE_Y(20);

					//draw text
					step += 10;
					if (step >= 360)
						step -= 360;
					int heading = (step % 10 == 0) ? (step / 10 - 1) : (step / 10);

					QString text;
					if (heading % 9 == 0)
					{
						if (heading/9 == 0)
							text = text.fromUtf16(TEXT_FLIGHT_SIM_NORTH);
						else if (heading/9 == 1)
							text = text.fromUtf16(TEXT_FLIGHT_SIM_EAST);
						else if (heading/9 == 2)
							text = text.fromUtf16(TEXT_FLIGHT_SIM_SOUTH);
						else if (heading/9 == 3)
							text = text.fromUtf16(TEXT_FLIGHT_SIM_WEST);
					}
					else if (heading % 3 == 0)
					{
						text = QString("%1").arg(heading);
					}

					xshift = -(gTextScale * m_pFont->getStrLen( text ) * 0.5f );
					DrawOutlineString( x1 + xshift, y1+SCALE_Y(20), text );

				}
				else //short vertical line
				{
					y1 = left.Y + SCALE_Y(10);
				}

				Draw2DLine( x1, y1, x2, y2 );
			}
		}

		void CSimSceneNode::PaintBankAngleArea_Flight()
		{
			float side = qMin(m_simStatus->m_screenWidth, m_simStatus->m_screenHeight);

			float m_bankangle = m_simStatus->m_bankangle;

			glMatrixMode ( GL_MODELVIEW );
			glPushMatrix();
			glTranslatef( m_simStatus->m_screenWidth / 2, m_simStatus->m_screenHeight / 2, 0.0f );
			glScalef( side / 400.0f, side / 400.0f, 1.0f );

			float fixTri[3*2] = {0, -95, 
							    -5, -88, 
							     5, -88 };
			Draw2DPolygon( fixTri, 3 );

			float triangle[3*2] = {0, -105, 
				  			      -5, -112, 
								   5, -112};
			float longLine[2*2] = {100, 0, 
								   110, 0 };
			float shortLine[2*2] = {100, 0, 
								    106, 0 };

			glRotated( -m_bankangle, 0.0f, 0.0f, 1.0f )						;

			Draw2DPolyLines( gArcPoints, ARC_DIV_NUM + 1 );

			Draw2DPolygon( triangle, 3 );
			glRotated( -150.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( longLine[0], longLine[1], longLine[2], longLine[3] );
			glRotated( 15.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( shortLine[0], shortLine[1], shortLine[2], shortLine[3] );
			glRotated( 15.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( longLine[0], longLine[1], longLine[2], longLine[3] );
			glRotated( 10.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( shortLine[0], shortLine[1], shortLine[2], shortLine[3] );
			glRotated( 10.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( shortLine[0], shortLine[1], shortLine[2], shortLine[3] );
			glRotated( 20.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( shortLine[0], shortLine[1], shortLine[2], shortLine[3] );
			glRotated( 10.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( shortLine[0], shortLine[1], shortLine[2], shortLine[3] );
			glRotated( 10.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( longLine[0], longLine[1], longLine[2], longLine[3] );
			glRotated( 15.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( shortLine[0], shortLine[1], shortLine[2], shortLine[3] );
			glRotated( 15.0f, 0.0f, 0.0f, 1.0f );
			Draw2DLine( longLine[0], longLine[1], longLine[2], longLine[3] );

			glPopMatrix();
		}

		void CSimSceneNode::PaintVerticalSpeedArea_Flight()
		{
			float m_verticalspeed = m_simStatus->m_verticalspeed;

			QString text = QString("%1m/s").arg(m_verticalspeed, 0, 'f', 1);
			float xshift = 0.0f;
			xshift = -( gTextScale * m_pFont->getStrLen( text ) / 2 );
			DrawOutlineString( SCALE_X(1100 + xshift), SCALE_Y(65), text );

		}

		void CSimSceneNode::PaintExitButtonArea_Flight()
		{
// 			painter->save();
// 			double scale = g_app->GetMainWindow()->GetRenderFrame()->height();
// 			scale /= 956;
// 			double size = scale * 10;
// 
// 			QFont font = g_app->GetConfig()->m_mainFont;
// 			font.setPointSize(size);
// 
// 			double z = 0.0;
// 			if (m_timeLine.state() == QTimeLine::Running)
// 				z = (double)m_timeLine.currentTime() / 100;
// 
// 			painter->setOpacity(0.7 - 0.7*z);
// 			painter->setFont(font);
// 			if (m_exitRegion)
// 			{
// 				painter->setBrush(Qt::darkGray);
// 				painter->setPen(Qt::darkGray);
// 			}
// 			else
// 			{
// 				painter->setBrush(Qt::gray);
// 				painter->setPen(Qt::gray);
// 			}
// 
// 			QString text = MSG_TOOLBAR_EXIT_FLIGHT;
// 			QRect rect(SCALE_X(1000), SCALE_Y(20), SCALE_X(250), SCALE_Y(20));
// 			m_exitRect = painter->boundingRect(rect, Qt::AlignVCenter | Qt::AlignRight, text);
// 			QRect drawRect = m_exitRect;
// 			drawRect.setX(m_exitRect.X - 3);
// 			drawRect.setWidth(m_exitRect.width() + 6);
// 
// 			QPen bodyPen(Qt::white);
// 			QPen outlinePen(Qt::black);
// 			int thick = 1;
// 
// 			painter->drawRect(drawRect);
// 			gu_DrawTextEx(painter, rect, Qt::AlignVCenter | Qt::AlignRight, text,
// 				thick, bodyPen, outlinePen);
// 			painter->restore();
		}

		void CSimSceneNode::PaintThrottleArea_Flight()
		{
			float throttle = m_simStatus->m_throttle * 20;


			//base line
			float x1 = SCALE_X(45), y1 = SCALE_Y(736);
			float x2 = SCALE_X(30), y2 = y1;
			float x3 = x2, y3 = SCALE_Y(876);
			float x4 = x1, y4 = y3;

			Draw2DLine( x1, y1, x2, y2 );
			Draw2DLine( x2, y2, x3, y3 );
			Draw2DLine( x3, y3, x4, y4 );

			//move triangle
			float step = throttle * ((y3 - y2) / 20); 
			float tx1 = SCALE_X(30), ty1 = SCALE_Y(876) - step;
			float tx2 = tx1 + SCALE_X(9), ty2 = ty1 - SCALE_Y(6);
			float tx3 = tx2, ty3 = ty1 + SCALE_Y(6);

			Draw2DLine( tx1, ty1, tx2, ty2 );
			Draw2DLine( tx2, ty2, tx3, ty3 );
			Draw2DLine( tx3, ty3, tx1, ty1 );

		}

		void CSimSceneNode::PaintRudderArea_Flight()
		{
			float rudder = m_simStatus->m_rudder*10 + 10;

			//base line
			float x1 = SCALE_X(60), y1 = SCALE_Y(851);
			float x2 = x1, y2 = SCALE_Y(836);
			float x3 = SCALE_X(200), y3 = y2;
			float x4 = x3, y4 = y1;
			Draw2DLine( x1, y1, x2, y2 );
			Draw2DLine( x2, y2, x3, y3 );
			Draw2DLine( x3, y3, x4, y4 );

			
			//move triangle
			float step = rudder * ((x3 - x2) / 20); 
			float tx1 = SCALE_X(60) + step, ty1 = SCALE_Y(836);
			float tx2 = tx1 - SCALE_X(6), ty2 = ty1 + SCALE_Y(9);
			float tx3 = tx1 + SCALE_X(6), ty3 = ty2;

			Draw2DLine( tx1, ty1, tx2, ty2 );
			Draw2DLine( tx2, ty2, tx3, ty3 );
			Draw2DLine( tx3, ty3, tx1, ty1 );
		}

		void CSimSceneNode::PaintAileronArea_Flight()
		{
			float aileron = m_simStatus->m_aileron*10 + 10;

			//base line
			float x1 = SCALE_X(60), y1 = SCALE_Y(791);
			float x2 = x1, y2 = SCALE_Y(776);
			float x3 = SCALE_X(200), y3 = y2;
			float x4 = x3, y4 = y1;

			Draw2DLine( x1, y1, x2, y2 );
			Draw2DLine( x2, y2, x3, y3 );
			Draw2DLine( x3, y3, x4, y4 );

			//move triangle
			float step = aileron * ((x3 - x2) / 20); 
			float tx1 = SCALE_X(60) + step, ty1 = SCALE_Y(776);
			float tx2 = tx1 - SCALE_X(6), ty2 = ty1 + SCALE_Y(9);
			float tx3 = tx1 + SCALE_X(6), ty3 = ty2;

			Draw2DLine( tx1, ty1, tx2, ty2 );
			Draw2DLine( tx2, ty2, tx3, ty3 );
			Draw2DLine( tx3, ty3, tx1, ty1 );
		}

		void CSimSceneNode::PaintElevatorArea_Flight()
		{
			float elevator = m_simStatus->m_elevator*10 + 10;

			//base line
			float x1 = SCALE_X(245), y1 = SCALE_Y(736);
			float x2 = SCALE_X(230), y2 = y1;
			float x3 = x2, y3 = SCALE_Y(876);
			float x4 = x1, y4 = y3;

			Draw2DLine( x1, y1, x2, y2 );
			Draw2DLine( x2, y2, x3, y3 );
			Draw2DLine( x3, y3, x4, y4 );

			//move triangle
			float step = elevator * ((y3 - y2) / 20); 
			float tx1 = SCALE_X(230), ty1 = SCALE_Y(876) - step;
			float tx2 = tx1 + SCALE_X(9), ty2 = ty1 - SCALE_Y(6);
			float tx3 = tx2, ty3 = ty1 + SCALE_Y(6);

			Draw2DLine( tx1, ty1, tx2, ty2 );
			Draw2DLine( tx2, ty2, tx3, ty3 );
			Draw2DLine( tx3, ty3, tx1, ty1 );
		}

		void CSimSceneNode::PaintFlapGearArea_Flight()
		{
			/*QPen bodyPen(Qt::green);
			QPen outlinePen(Qt::black);
			QPen stallPen(Qt::red);
			int thick = 1;

			//Flap
			if (m_flap > 0)
			{
				QString text = QString("%1 %2%3").arg(MSG_FLIGHT_SIM_FLAPS).arg(m_flap).arg(QChar(0x0025));
				QRect textRect(SCALE_X(50), SCALE_Y(520), SCALE_X(200), SCALE_Y(20));
				gu_DrawTextEx(painter, textRect, Qt::AlignVCenter | Qt::AlignHCenter, text,
					thick, bodyPen, outlinePen);
			}

			//Gear
			if (m_gear)
			{
				QString text = MSG_FLIGHT_SIM_GEAR;
				QRect gearRect(SCALE_X(50), SCALE_Y(570), SCALE_X(200), SCALE_Y(20));
				gu_DrawTextEx(painter, gearRect, Qt::AlignVCenter | Qt::AlignHCenter, text,
					thick, bodyPen, outlinePen);
			}

 			//Stall
 			if (m_stall)
 			{
 				QString text = MSG_FLIGHT_SIM_STALL;
 				QRect stallRect(SCALE_X(1000), SCALE_Y(520), SCALE_X(200), SCALE_Y(20));
 				gu_DrawTextEx(painter, stallRect, Qt::AlignVCenter | Qt::AlignHCenter, text,
 					thick, stallPen, outlinePen);
 			}*/
		}

		void CSimSceneNode::PaintPitchAngleArea_Flight()
		{
			float m_pitchangle = m_simStatus->m_pitchangle;
			float m_bankangle = m_simStatus->m_bankangle;

			//pitchangle(from -360 ~ 360 to -90 ~ 90)
			if (m_pitchangle < 0)
				m_pitchangle = 360 + m_pitchangle;

			if (m_pitchangle > 270)
				m_pitchangle = m_pitchangle - 360;
			else if (m_pitchangle > 180)
				m_pitchangle = 180 - m_pitchangle;
			else if (m_pitchangle > 90)
				m_pitchangle  = 180 - m_pitchangle;

			float side = qMin(m_simStatus->m_screenWidth, m_simStatus->m_screenHeight);
			float scale = side / 400.0f;

			glMatrixMode ( GL_MODELVIEW );
			glPushMatrix();

			glTranslatef( m_simStatus->m_screenWidth / 2, m_simStatus->m_screenHeight / 2, 0.0f);
			glScalef( scale, scale, 1.0f );
			glRotatef( -m_bankangle, 0.0f, 0.0f, 1.0f );

			SPoint p1, p2;
			int step = m_pitchangle*8 + 720;
			int longLine =  (step % 80 == 0 || step % 80 > 40) ? (1) : (0);

			for (int i = 0; i < 6; i++)
			{
				float x = (i % 2 == longLine) ? (40) : (20);
				float y = (step % 40 == 0) ? (120 - i*40) 
					: (120 - i*40 - (40 - (step%40))*1);
				if (y < -85 || y >= 85)
					continue;

				Draw2DLine( -x, y, x, y );

				int value = (((int)m_pitchangle)/5)*5;
				if (m_pitchangle >= 0)
					value = (step % 40 == 0) ? (value - 15 + i*5) : (value - 10 + i*5); 
				else
					value = value - 15 + i*5; 

				if (value > 90)
					value = 180 - value;
				else if (value < -90)
					value = -(180 + value);

				glMatrixMode ( GL_MODELVIEW );
				glPushMatrix();
				glRotated(m_bankangle, 0.0f, 0.0f, 1.0f );

				float textScale = 1 / scale;
				QString text = QString("%1").arg(value);
				float angle = degree2radian(-m_bankangle);

				//int textWidth = 10;
				//int x1 = (x+5) * cos(angle) - (y) * sin(angle);
				//int y1 = (y) * cos(angle) + (x+5) * sin(angle);
				//int x2 = (-x-5-textWidth) * cos(angle) - (y) * sin(angle);
				//int y2 = (y) * cos(angle) + (-x-5-textWidth) * sin(angle);

				int textWidth = 12;
				int x1 = (x+6)*cos(angle) - (y)*sin(angle);
				int y1 = (y)*cos(angle) + (x+6)*sin(angle);
				int x2 = (-x-6)*cos(angle) - (y)*sin(angle);
				int y2 = (y)*cos(angle) + (-x-6)*sin(angle);

				DrawOutlineString( x1-6, y1+2, text, textScale );
				DrawOutlineString( x2-6, y2+2, text, textScale );

				glPopMatrix();
			}
			glPopMatrix();

		}

		void CSimSceneNode::PaintAltitudeArea_Flight()
		{
			//draw vertical line
			SPoint top(SCALE_X(1100), SCALE_Y(80));
			SPoint bottom(SCALE_X(1100), SCALE_Y(380));
			Draw2DLine( top.X, top.Y, bottom.X, bottom.Y );

			//draw triangle
			float triangle[3*2] = {
				top.X,				bottom.Y - (bottom.Y-top.Y)/2,
				top.X-SCALE_X(15),	bottom.Y - (bottom.Y-top.Y)/2 - SCALE_Y(10),
				top.X-SCALE_X(15),	bottom.Y - (bottom.Y-top.Y)/2 + SCALE_Y(10)
			};
			Draw2DPolygon( triangle, 3 );

			int m_altitude = m_simStatus->m_altitude;
			//test code
			QString text = QString("(%1m)").arg(m_altitude);
			float xshift;
 			xshift = -( gTextScale * m_pFont->getStrLen( text ) );
 			DrawOutlineString(SCALE_X(1080)+xshift, SCALE_Y(237), text );

			int firstLine = abs((m_altitude-150)%100);
			int longLine;
			if (m_altitude < 150) 
				longLine = firstLine/20;
			else
				longLine = (firstLine == 0 || firstLine > 80) ? (0) : (4 - (firstLine-1)/20);

			for (int i = 0; i < 15; i++)
			{
				float x1, y1, x2, y2;
				int firstLineDelta = abs((m_altitude-150)%20);
				x1 = bottom.X;
				/*if (m_altitude < 150) 
					y1 = (firstLineDelta == 0) ? (bottom.Y - i*SCALE_Y(20)) 
					: (bottom.Y - i*SCALE_Y(20) - firstLineDelta*SCALE_Y(1)) ;
				else
					y1 = (firstLineDelta == 0) ? (bottom.Y - i*SCALE_Y(20)) 
					: (bottom.Y - i*SCALE_Y(20) - (20 - firstLineDelta*SCALE_Y(1))) ;*/

				if (m_altitude < 150) 
					y1 = (firstLineDelta == 0) ? (bottom.Y - SCALE_Y(i * 20)) 
					: (bottom.Y - SCALE_Y(i * 20 + firstLineDelta)) ;
				else
					y1 = (firstLineDelta == 0) ? (bottom.Y - SCALE_Y(i * 20)) 
					: (bottom.Y - SCALE_Y(i * 20 + 20 - firstLineDelta)) ;
					
				y2 = y1;

				if (i % 5 == longLine)
				{
					x2 = bottom.X + SCALE_X(30);

					//draw text
					int value;

					if (m_altitude < 150) 
					{
						if (m_altitude < 0)
							value = (firstLine > 50) ? ((m_altitude/100) * 100 - 100) : ((m_altitude/100) * 100 - 200);
						else
							value = (firstLine > 50) ? ((m_altitude/100) * 100) : ((m_altitude/100) * 100 - 100);
					}
					else
					{
						value = (firstLine >= 50) ? ((m_altitude/100) * 100 - 100) : ((m_altitude/100) * 100);
					}

					text = QString("%1").arg(value + (i/5)*100);
					DrawOutlineString( x2+SCALE_X(5), y2+SCALE_Y(7), text );
				}
				else
				{
					x2 = bottom.X + SCALE_X(20);
				}

				Draw2DLine( x1, y1, x2, y2 );
			}
		}

		void CSimSceneNode::PaintSpeedArea_Car()
		{

		}

		void CSimSceneNode::PaintTravelTimeArea_Car()
		{
			int time = m_simStatus->m_travelTime;

			QString text = QString("%1s").arg(time);
			float xshift = 0.0f;
			xshift = -( gTextScale * m_pFont->getStrLen( text ) / 2 );
			DrawOutlineString( SCALE_X(1100 + xshift), SCALE_Y(150), text );
		}

		void CSimSceneNode::PaintTravelDistanceArea_Car()
		{
			//draw vertical line
			SPoint top(SCALE_X(1100), SCALE_Y(200));
			SPoint bottom(SCALE_X(1100), SCALE_Y(500));
			Draw2DLine( top.X, top.Y, bottom.X, bottom.Y );

			//draw triangle
			float triangle[3*2] = {
				top.X,				bottom.Y - (bottom.Y-top.Y)/2,
				top.X-SCALE_X(15),	bottom.Y - (bottom.Y-top.Y)/2 - SCALE_Y(10),
				top.X-SCALE_X(15),	bottom.Y - (bottom.Y-top.Y)/2 + SCALE_Y(10)
			};
			Draw2DPolygon( triangle, 3 );

			int distance = m_simStatus->m_travelDistance;
			//test code
			QString text = QString("(%1m)").arg(distance);
			float xshift;
			xshift = -( gTextScale * m_pFont->getStrLen( text ) );
			DrawOutlineString(SCALE_X(1080)+xshift, SCALE_Y(357), text );

			int firstLine = abs((distance-150)%100);
			int longLine;
			if (distance < 150) 
				longLine = firstLine/20;
			else
				longLine = (firstLine == 0 || firstLine > 80) ? (0) : (4 - (firstLine-1)/20);

			for (int i = 0; i < 15; i++)
			{
				float x1, y1, x2, y2;
				int firstLineDelta = abs((distance-150)%20);
				x1 = bottom.X;
				if (distance < 150) 
					y1 = (firstLineDelta == 0) ? (bottom.Y - i*SCALE_Y(20)) 
					: (bottom.Y - i*SCALE_Y(20) - firstLineDelta*SCALE_Y(1)) ;
				else
					y1 = (firstLineDelta == 0) ? (bottom.Y - i*SCALE_Y(20)) 
					: (bottom.Y - i*SCALE_Y(20) - (20 - firstLineDelta*SCALE_Y(1))) ;
				y2 = y1;

				if (i % 5 == longLine)
				{
					x2 = bottom.X + SCALE_X(30);

					//draw text
					int value;

					if (distance < 150) 
					{
						if (distance < 0)
							value = (firstLine > 50) ? ((distance/100) * 100 - 100) : ((distance/100) * 100 - 200);
						else
							value = (firstLine > 50) ? ((distance/100) * 100) : ((distance/100) * 100 - 100);
					}
					else
					{
						value = (firstLine >= 50) ? ((distance/100) * 100 - 100) : ((distance/100) * 100);
					}
					int distVal = value + (i/5)*100;
					if ( distVal >= 0 )
					{
						text = QString("%1").arg( distVal );
						DrawOutlineString( x2+SCALE_X(5), y2+SCALE_Y(7), text );
					}
				}
				else
				{
					x2 = bottom.X + SCALE_X(20);
				}

				Draw2DLine( x1, y1, x2, y2 );
			}
		}

		void CSimSceneNode::PaintDirectionAngleArea_Car()
		{
			//draw horizontal line
			SPoint left(SCALE_X(735), SCALE_Y(750));
			SPoint right(SCALE_X(1185), SCALE_Y(750));

			float directionAngle = m_simStatus->m_directionAngle;

			int step = (int) (directionAngle);
			step = (step < 45) ? (360 + (step - 45)) : (step - 45);
			int longLine = (step % 10 == 0 || step % 10 > 5) ? (0) : (1); //longLine Number
			float xshift = 0.0f;

			//draw vertical line
			for (int i = 0; i < 18; i++)
			{
				float x1, y1, x2, y2;
				x1 = (step % 5 == 0) ? (left.X + i*SCALE_X(25)) 
					: (left.X + i*SCALE_X(25) + (5 - step % 5)*SCALE_X(5));
				x2 = x1;
				y2 = left.Y;

				if (i % 2 == longLine) //long vertical line
				{
					y1 = left.Y + SCALE_Y(20);

					//draw text
					step += 10;
					if (step >= 360)
						step -= 360;
					int heading = (step % 10 == 0) ? (step / 10 - 1) : (step / 10);

					QString text;
					if (heading % 9 == 0)
					{
						if (heading/9 == 0)
							text = text.fromUtf16(TEXT_FLIGHT_SIM_NORTH);
						else if (heading/9 == 1)
							text = text.fromUtf16(TEXT_FLIGHT_SIM_EAST);
						else if (heading/9 == 2)
							text = text.fromUtf16(TEXT_FLIGHT_SIM_SOUTH);
						else if (heading/9 == 3)
							text = text.fromUtf16(TEXT_FLIGHT_SIM_WEST);
					}
					else if (heading % 3 == 0)
					{
						text = QString("%1").arg(heading);
					}

					xshift = -(gTextScale * m_pFont->getStrLen( text ) * 0.5f );
					DrawOutlineString( x1 + xshift, y1+SCALE_Y(20), text );

				}
				else //short vertical line
				{
					y1 = left.Y + SCALE_Y(10);
				}

				Draw2DLine( x1, y1, x2, y2 );
			}
		}

		void CSimSceneNode::Draw2DLine( float x1, float y1, float x2, float y2 )
		{
			glLineWidth( 2.0f );
			glBegin(GL_LINES);
			glColor3f( gColorRed, gColorGreen, gColorBlue );
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glEnd();
		}

		void CSimSceneNode::Draw2DPolyLines( float pts[], int num )
		{
			glLineWidth( 2.0f );
			int n = 0;
			glBegin(GL_LINE_STRIP);
			for ( int i = 0; i < num ; i++ )
			{
				glColor3f( gColorRed, gColorGreen, gColorBlue );
				glVertex2f( pts[n], pts[n+1] );
				n += 2;
			}
			glEnd();
		}

		void CSimSceneNode::Draw2DPolygon( float pts[], int num )
		{
			glLineWidth( 2.0f );
			int n = 0;
			glBegin(GL_LINE_LOOP);
			for ( int i = 0; i < num ; i++ )
			{
				glColor3f( gColorRed, gColorGreen, gColorBlue );
				glVertex2f( pts[n], pts[n+1] );
				n += 2;
			}
			glEnd();
		}

		void CSimSceneNode::DrawOutlineString( float x, float y, QString& str, float scale )
		{
			
			m_pFont->printOutlinedString( x, y, str, 0, 0, 0, gColor, 0, gTextScale * scale );
		}


	} // end namespace scene
} // end namespace geo
