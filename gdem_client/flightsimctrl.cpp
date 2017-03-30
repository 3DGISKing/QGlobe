#include "flightsimctrl.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "renderfrm.h"
#include "rendercursor.h"
#include "config.h"
#include "guiuty.h"
#include "msgdata.h"
#include "proxy.h"
#include "GDM_SimMgr.h"

FlightSimCtrl::FlightSimCtrl()
{
	InitDrawPath();

	m_speed = 0;
	m_heading = 0;
	m_bankangle = 0;
	m_verticalspeed = 0;
	m_throttle = 0;
	m_rudder = 0;
	m_aileron = 0;
	m_elevator = 0;
	m_flap = 0;
	m_gear = false;
	m_pitchangle = 0;
	m_altitude = 0;
	m_stall = false;
	m_ended = false;
	m_collided = false;

	m_showMessageBox = false;

	m_exitRegion = false;
	m_timeLine.setDuration(500);
	m_timeLine.setFrameRange(0, 500);
	m_timeLine.setUpdateInterval(100);

	connect(&m_timeLine, SIGNAL(finished()), this, SLOT(ExitFlightSimulator()));
}

FlightSimCtrl::~FlightSimCtrl()
{
}

void FlightSimCtrl::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_collidTimer.timerId())
	{
		m_collidTimer.stop();

		if(m_showMessageBox)
		{
 			int result = gu_MessageBox(g_app->GetMainWindow(), 
							tr("Your aircraft has crashed."), tr("Do you want start flight simulator?"),
							MSGBOX_ICONQUESTION | MSGBOX_IDYES | MSGBOX_IDNO);

			m_showMessageBox = false;
  			if (result == MSGBOX_IDNO)
				ExitFlightSimulator();
  			else if (result == MSGBOX_IDYES)
			{
  				g_app->GetRenderProxy()->GetRenderSrv()->StartSimulation(SIM_TYPE_FLIGHT, true);
				g_app->GetMainWindow()->GetRenderFrame()->setFocus();
			}
		}
	}
}

void FlightSimCtrl::Paint(QPainter *painter)
{
	RenderFrame *render = g_app->GetMainWindow()->GetRenderFrame();
	if (!render)
		return;

	if (!GetParam())
		return;

	if (m_collided && !m_showMessageBox)
	{
		m_collidTimer.start(500, this);
		m_showMessageBox = true;
	}

	if (m_ended)
	{
		ExitFlightSimulator();
		return;
	}
	if ( !m_showHelp )
		return;

	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
	
	double scale = g_app->GetMainWindow()->GetRenderFrame()->height();
	scale /= 956;
	double size = scale * 12;

	//QFont font = g_app->GetConfig()->m_mainFont;
	QFont font(g_app->GetFontName(true));
	font.setPointSize(g_app->GetDrawFontSize(size));
	font.setStyleStrategy(QFont::NoAntialias);
	QPen pen(Qt::green, 2);

	painter->setFont(font);
	painter->setPen(pen);

	for (int i = 0; i < FS_AREA_COUNT; i++)
	{
		(this->*PaintArea[i])(painter);
	}

	painter->restore();
}

bool FlightSimCtrl::mouseMoveEvent(const CursorState *event)
{
	m_exitRegion = IsExitRegion(event->x, event->y);

	if (m_exitRegion)
	{
		RenderCursor *cursor = g_app->GetMainWindow()->GetRenderFrame()->GetRenderCursor();
		cursor->SetCursor(CURSOR_SHAPE_ARROW);
	}

	return m_exitRegion;
}

bool FlightSimCtrl::mousePressEvent(const CursorState *event)
{
	if (IsExitRegion(event->x, event->y))
	{
		m_timeLine.stop();
		m_timeLine.start();

		return true;
	}

	return false;
}

void FlightSimCtrl::InitDrawPath()
{
// 	PaintArea[FS_FIXED_AREA] =			&FlightSimCtrl::PaintFixedArea;
// 	PaintArea[FS_SPEED_AREA] =			&FlightSimCtrl::PaintSpeedArea;
// 	PaintArea[FS_HEADING_AREA] =		&FlightSimCtrl::PaintHeadingArea;
// 	PaintArea[FS_BANKINGANGLE_AREA] =	&FlightSimCtrl::PaintBankAngleArea;
// 	PaintArea[FS_VERTICALSPEED_AREA] =	&FlightSimCtrl::PaintVerticalSpeedArea;
// 	PaintArea[FS_THROTTLE_AREA] =		&FlightSimCtrl::PaintThrottleArea;
// 	PaintArea[FS_RUDDER_AREA] =			&FlightSimCtrl::PaintRudderArea;
// 	PaintArea[FS_AILERON_AREA] =		&FlightSimCtrl::PaintAileronArea;
// 	PaintArea[FS_ELEVATOR_AREA] =		&FlightSimCtrl::PaintElevatorArea;
// 	PaintArea[FS_FLAPGEAR_AREA] =		&FlightSimCtrl::PaintFlapGearArea;
// 	PaintArea[FS_PITCHANGLE_AREA] =		&FlightSimCtrl::PaintPitchAngleArea;
// 	PaintArea[FS_ALTITUDE_AREA] =		&FlightSimCtrl::PaintAltitudeArea;
	PaintArea[FS_EXITBUTTON_AREA] =		&FlightSimCtrl::PaintExitButtonArea;
}

bool FlightSimCtrl::GetParam()
{
	CGDM_SimMgr *simMgr = g_app->GetRenderProxy()->GetRenderSrv()->m_pSimMgr;
	if (!simMgr)
		return false;

	GDM_SIM_STATUS simStatus;
	memset(&simStatus, 0, sizeof(GDM_SIM_STATUS));
	simMgr->GetSimulationStatus(&simStatus);

	m_speed = simStatus.m_speed;
	m_heading = simStatus.m_heading;
	m_bankangle = simStatus.m_bankangle;
	m_verticalspeed = simStatus.m_verticalspeed;
	m_throttle = simStatus.m_throttle;
	m_rudder = simStatus.m_rudder;
	m_aileron = simStatus.m_aileron;
	m_elevator = simStatus.m_elevator;
	m_flap = simStatus.m_flap;
	m_gear = simStatus.m_gear;
	m_pitchangle = simStatus.m_pitchangle;
	m_altitude = simStatus.m_altitude;
	m_stall = simStatus.m_stall;
	m_ended = simStatus.m_ended;
	m_showHelp = simStatus.m_showHelp;
	m_collided = simStatus.m_collided;

	return true;
}

void FlightSimCtrl::PaintFixedArea(QPainter *painter)
{
	int cx = g_app->GetMainWindow()->GetRenderFrame()->width() / 2;
	int cy = g_app->GetMainWindow()->GetRenderFrame()->height() / 2;

	QPoint airplane[4] = {
		QPoint(cx, cy), 
		QPoint(cx-SCALE_X(25), cy+SCALE_Y(20)),
		QPoint(cx, cy+SCALE_Y(8)),
		QPoint(cx+SCALE_X(25), cy+SCALE_Y(20))
	};

	painter->drawConvexPolygon(airplane, 4);
}

void FlightSimCtrl::PaintSpeedArea(QPainter *painter)
{
	//draw vertical line
	QPoint top(SCALE_X(150), SCALE_Y(80));
	QPoint bottom(SCALE_X(150), SCALE_Y(360));
	painter->drawLine(top, bottom);

	//draw triangle
	QPoint triangle[3] = {
		QPoint(top.x(), bottom.y() - (bottom.y()-top.y())/2),
		QPoint(top.x()+SCALE_X(15), bottom.y() - (bottom.y()-top.y())/2 - SCALE_Y(10)),
		QPoint(top.x()+SCALE_X(15), bottom.y() - (bottom.y()-top.y())/2 + SCALE_Y(10))
	};
	painter->drawConvexPolygon(triangle, 3);

	QPen bodyPen(Qt::green);
	QPen outlinePen(Qt::black);
	int thick = 1;
	QString text = QString("(%1KPH)").arg(m_speed);
	QRect testRect(SCALE_X(170), SCALE_Y(210), SCALE_X(100), SCALE_Y(20));
	gu_DrawTextEx(painter, testRect, Qt::AlignVCenter | Qt::AlignLeft, text, 
		thick, bodyPen, outlinePen);

	int longLine = (m_speed % 10 == 0 || m_speed % 10 > 5) ? (0) : (1); //long line number

	//draw horizontal line
	for (int i = 0; i < 8; i++)
	{
		int x1, y1, x2, y2;
		y1 = (m_speed % 5 == 0) ? (bottom.y() - i*SCALE_Y(35)) 
								: (bottom.y() - i*SCALE_Y(35) - (5 - m_speed % 5)*SCALE_Y(7));
		x2 = bottom.x();
		y2 = y1;

		if (i % 2 == longLine) //long horizontal line
		{
			x1 = bottom.x() - SCALE_X(30);

			//draw text
			int delta = (m_speed % 10 == 0) ? (20) : (10);
			int speed = (m_speed/10) * 10 - delta;
			text = QString("%1").arg(speed + (i/2)*10);
			QRect textRect(x1-SCALE_X(120), y1-SCALE_Y(10), SCALE_X(115), SCALE_Y(20));
			gu_DrawTextEx(painter, textRect, Qt::AlignVCenter | Qt::AlignRight, text,
						thick, bodyPen, outlinePen);
		}
		else //short horizontal line
		{
			x1 = bottom.x() - SCALE_X(20);
		}

		painter->drawLine(x1, y1, x2, y2);
	}
}

void FlightSimCtrl::PaintHeadingArea(QPainter *painter)
{
	//draw horizontal line
	QPoint left(SCALE_X(415), SCALE_Y(30));
	QPoint right(SCALE_X(865), SCALE_Y(30));

	QPen bodyPen(Qt::green);
	QPen outlinePen(Qt::black);
	int thick = 1;

	int step = (int) (m_heading);
	step = (step < 45) ? (360 + (step - 45)) : (step - 45);
	int longLine = (step % 10 == 0 || step % 10 > 5) ? (0) : (1); //longLine Number

	//draw vertical line
	for (int i = 0; i < 18; i++)
	{
		int x1, y1, x2, y2;
		x1 = (step % 5 == 0) ? (left.x() + i*SCALE_X(25)) 
							: (left.x() + i*SCALE_X(25) + (5 - step % 5)*SCALE_X(5));
		x2 = x1;
		y2 = left.y();

		if (i % 2 == longLine) //long vertical line
		{
			y1 = left.y() + SCALE_Y(20);

			//draw text
			step += 10;
			if (step >= 360)
				step -= 360;
			int heading = (step % 10 == 0) ? (step / 10 - 1) : (step / 10);

			QString text;
			if (heading % 9 == 0)
			{
				if (heading/9 == 0)
					text = MSG_FLIGHT_SIM_NORTH;
				else if (heading/9 == 1)
					text = MSG_FLIGHT_SIM_EAST;
				else if (heading/9 == 2)
					text = MSG_FLIGHT_SIM_SOUTH;
				else if (heading/9 == 3)
					text = MSG_FLIGHT_SIM_WEST;
			}
			else if (heading % 3 == 0)
			{
				text = QString("%1").arg(heading);
			}
			
			QRect textRect(x1-SCALE_X(100), y1+SCALE_Y(5), SCALE_X(200), SCALE_Y(20));
			gu_DrawTextEx(painter, textRect, Qt::AlignVCenter | Qt::AlignHCenter, text,
						thick, bodyPen, outlinePen);
		}
		else //short vertical line
		{
			y1 = left.y() + SCALE_Y(10);
		}

		painter->drawLine(x1, y1, x2, y2);
	}
}

void FlightSimCtrl::PaintBankAngleArea(QPainter *painter)
{
	RenderFrame *render = g_app->GetMainWindow()->GetRenderFrame();
	double side = qMin(render->width(), render->height());

	painter->save();
	
	painter->translate(render->width()/2, render->height()/2);
	painter->scale(side/400, side/400);

	QPen pen(Qt::green, 1);
	painter->setPen(pen);
	painter->setBrush(Qt::NoBrush);

	QPoint fixTri[3] = { QPoint(0, -95), QPoint(-5, -88), QPoint(5, -88) };
	painter->drawConvexPolygon(fixTri, 3);

	QPoint triangle[3] = { QPoint(0, -105), QPoint(-5, -112), QPoint(5, -112) };
	QPoint longLine[2] = { QPoint(100, 0), QPoint(110, 0) };
	QPoint shortLine[2] = { QPoint(100, 0), QPoint(106, 0) };

	QRect arcRect(-100, -100, 200, 200);
	painter->rotate(-m_bankangle);
	painter->drawArc(arcRect, 30*16, 120*16);
	painter->drawConvexPolygon(triangle, 3);
	
	painter->rotate(-150);
	painter->drawLine(longLine[0].x(), longLine[0].y(), longLine[1].x(), longLine[1].y());
	painter->rotate(15);
	painter->drawLine(shortLine[0].x(), shortLine[0].y(), shortLine[1].x(), shortLine[1].y());
	painter->rotate(15);
	painter->drawLine(longLine[0].x(), longLine[0].y(), longLine[1].x(), longLine[1].y());
	painter->rotate(10);
	painter->drawLine(shortLine[0].x(), shortLine[0].y(), shortLine[1].x(), shortLine[1].y());
	painter->rotate(10);
	painter->drawLine(shortLine[0].x(), shortLine[0].y(), shortLine[1].x(), shortLine[1].y());
	painter->rotate(20);
	painter->drawLine(shortLine[0].x(), shortLine[0].y(), shortLine[1].x(), shortLine[1].y());
	painter->rotate(10);
	painter->drawLine(shortLine[0].x(), shortLine[0].y(), shortLine[1].x(), shortLine[1].y());
	painter->rotate(10);
	painter->drawLine(longLine[0].x(), longLine[0].y(), longLine[1].x(), longLine[1].y());
	painter->rotate(15);
	painter->drawLine(shortLine[0].x(), shortLine[0].y(), shortLine[1].x(), shortLine[1].y());
	painter->rotate(15);
	painter->drawLine(longLine[0].x(), longLine[0].y(), longLine[1].x(), longLine[1].y());
	
	painter->restore();
}

void FlightSimCtrl::PaintVerticalSpeedArea(QPainter *painter)
{
	QString text = QString("%1").arg(m_verticalspeed, 10, 'f', 1);
	QRect textRect(SCALE_X(1000), SCALE_Y(50), SCALE_X(200), SCALE_Y(20));
	QPen bodyPen(Qt::green);
	QPen outlinePen(Qt::black);
	int thick = 1;
	gu_DrawTextEx(painter, textRect, Qt::AlignVCenter | Qt::AlignHCenter, text,
				thick, bodyPen, outlinePen);
}

void FlightSimCtrl::PaintExitButtonArea(QPainter *painter)
{
	painter->save();
	double scale = g_app->GetMainWindow()->GetRenderFrame()->height();
	scale /= 956;
	double size = scale * 10;

	//QFont font = g_app->GetConfig()->m_mainFont;
	QFont font(g_app->GetFontName(true));
        font.setPointSize(g_app->GetDrawFontSize(10));
	font.setStyleStrategy(QFont::NoAntialias);

	double z = 0.0;
	if (m_timeLine.state() == QTimeLine::Running)
		z = (double)m_timeLine.currentTime() / 100;

	painter->setOpacity(0.7 - 0.7*z);
	painter->setFont(font);
	if (m_exitRegion)
	{
		painter->setBrush(Qt::darkGray);
		painter->setPen(Qt::darkGray);
	}
	else
	{
		painter->setBrush(Qt::gray);
		painter->setPen(Qt::gray);
	}

	QString text = tr("Exit Flight Simulator");
	QRect rect(SCALE_X(1000), SCALE_Y(20), SCALE_X(250), SCALE_Y(20));
	m_exitRect = painter->boundingRect(rect, Qt::AlignVCenter | Qt::AlignRight, text);
	QRect drawRect = m_exitRect;
        drawRect.setX(m_exitRect.x() - 5);
        drawRect.setY(m_exitRect.y() - 2);
        drawRect.setWidth(m_exitRect.width() + 10);
        drawRect.setHeight(m_exitRect.height() + 4);

	QPen bodyPen(Qt::white);
	QPen outlinePen(Qt::black);
	int thick = 1;

	painter->drawRect(drawRect);
        gu_DrawTextEx(painter, m_exitRect, Qt::AlignVCenter | Qt::AlignRight, text,
				thick, bodyPen, outlinePen);
	painter->restore();
}

void FlightSimCtrl::PaintThrottleArea(QPainter *painter)
{
	double throttle = m_throttle * 20;

	//base line
	double x1 = SCALE_X(45), y1 = SCALE_Y(736);
	double x2 = SCALE_X(30), y2 = y1;
	double x3 = x2, y3 = SCALE_Y(876);
	double x4 = x1, y4 = y3;

	painter->drawLine(x1, y1, x2, y2);
	painter->drawLine(x2, y2, x3, y3);
	painter->drawLine(x3, y3, x4, y4);

	//move triangle
	double step = throttle * ((y3 - y2) / 20); 
	double tx1 = SCALE_X(30), ty1 = SCALE_Y(876) - step;
	double tx2 = tx1 + SCALE_X(9), ty2 = ty1 - SCALE_Y(6);
	double tx3 = tx2, ty3 = ty1 + SCALE_Y(6);

	painter->drawLine(tx1, ty1, tx2, ty2);
	painter->drawLine(tx2, ty2, tx3, ty3);
	painter->drawLine(tx3, ty3, tx1, ty1);
}

void FlightSimCtrl::PaintRudderArea(QPainter *painter)
{
	double rudder = m_rudder*10 + 10;

	//base line
	double x1 = SCALE_X(60), y1 = SCALE_Y(851);
	double x2 = x1, y2 = SCALE_Y(836);
	double x3 = SCALE_X(200), y3 = y2;
	double x4 = x3, y4 = y1;

	painter->drawLine(x1, y1, x2, y2);
	painter->drawLine(x2, y2, x3, y3);
	painter->drawLine(x3, y3, x4, y4);

	//move triangle
	double step = rudder * ((x3 - x2) / 20); 
	double tx1 = SCALE_X(60) + step, ty1 = SCALE_Y(836);
	double tx2 = tx1 - SCALE_X(6), ty2 = ty1 + SCALE_Y(9);
	double tx3 = tx1 + SCALE_X(6), ty3 = ty2;

	painter->drawLine(tx1, ty1, tx2, ty2);
	painter->drawLine(tx2, ty2, tx3, ty3);
	painter->drawLine(tx3, ty3, tx1, ty1);
}

void FlightSimCtrl::PaintAileronArea(QPainter *painter)
{
	double aileron = m_aileron*10 + 10;

	//base line
	double x1 = SCALE_X(60), y1 = SCALE_Y(791);
	double x2 = x1, y2 = SCALE_Y(776);
	double x3 = SCALE_X(200), y3 = y2;
	double x4 = x3, y4 = y1;

	painter->drawLine(x1, y1, x2, y2);
	painter->drawLine(x2, y2, x3, y3);
	painter->drawLine(x3, y3, x4, y4);

	//move triangle
	double step = aileron * ((x3 - x2) / 20); 
	double tx1 = SCALE_X(60) + step, ty1 = SCALE_Y(776);
	double tx2 = tx1 - SCALE_X(6), ty2 = ty1 + SCALE_Y(9);
	double tx3 = tx1 + SCALE_X(6), ty3 = ty2;

	painter->drawLine(tx1, ty1, tx2, ty2);
	painter->drawLine(tx2, ty2, tx3, ty3);
	painter->drawLine(tx3, ty3, tx1, ty1);
}

void FlightSimCtrl::PaintElevatorArea(QPainter *painter)
{
	double elevator = m_elevator*10 + 10;

	//base line
	double x1 = SCALE_X(245), y1 = SCALE_Y(736);
	double x2 = SCALE_X(230), y2 = y1;
	double x3 = x2, y3 = SCALE_Y(876);
	double x4 = x1, y4 = y3;

	painter->drawLine(x1, y1, x2, y2);
	painter->drawLine(x2, y2, x3, y3);
	painter->drawLine(x3, y3, x4, y4);

	//move triangle
	double step = elevator * ((y3 - y2) / 20); 
	double tx1 = SCALE_X(230), ty1 = SCALE_Y(876) - step;
	double tx2 = tx1 + SCALE_X(9), ty2 = ty1 - SCALE_Y(6);
	double tx3 = tx2, ty3 = ty1 + SCALE_Y(6);

	painter->drawLine(tx1, ty1, tx2, ty2);
	painter->drawLine(tx2, ty2, tx3, ty3);
	painter->drawLine(tx3, ty3, tx1, ty1);
}

void FlightSimCtrl::PaintFlapGearArea(QPainter *painter)
{
	QPen bodyPen(Qt::green);
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
	}
}

void FlightSimCtrl::PaintPitchAngleArea(QPainter *painter)
{
	//pitchangle(from -360 ~ 360 to -90 ~ 90)
	if (m_pitchangle < 0)
		m_pitchangle = 360 + m_pitchangle;

	if (m_pitchangle > 270)
		m_pitchangle = m_pitchangle - 360;
	else if (m_pitchangle > 180)
		m_pitchangle = 180 - m_pitchangle;
	else if (m_pitchangle > 90)
		m_pitchangle  = 180 - m_pitchangle;

	RenderFrame *render = g_app->GetMainWindow()->GetRenderFrame();
	double side = qMin(render->width(), render->height());

	painter->save();
	
	painter->translate(render->width()/2, render->height()/2);
	painter->scale(side/400, side/400);
	painter->rotate(-m_bankangle);

	QPen pen(Qt::green, 1);
	painter->setPen(pen);
	
	int step = m_pitchangle*8;
	int longLine =  (step % 80 == 0 || step % 80 > 40) ? (1) : (0);

	for (int i = 0; i < 6; i++)
	{
		int x = (i % 2 == longLine) ? (40) : (20);
		int y = (step % 40 == 0) ? (120 - i*40) 
							: (120 - i*40 - (40 - (step%40))*1);
		if (y < -85 || y >= 85)
			continue;
		painter->drawLine(-x, y, x, y);
		
		int value = (((int)m_pitchangle)/5)*5;
		value = (step % 40 == 0) ? (value - 15 + i*5) : (value - 10 + i*5); 

		if (value > 90)
			value = 180 - value;
		else if (value < -90)
			value = 180 + value;
		
		painter->save();
		//QFont font = g_app->GetConfig()->m_mainFont;
		QFont font(g_app->GetFontName(true));
		font.setPointSize(g_app->GetDrawFontSize(5));
		font.setStyleStrategy(QFont::ForceOutline);
		painter->setFont(font);
		painter->setBrush(Qt::black);
		painter->rotate(m_bankangle);

		QString text = QString("%1").arg(value);
		QRect textRect = painter->boundingRect(-200, -200, 400, 400, Qt::AlignVCenter | Qt::AlignHCenter, text);
		double angle = m_bankangle;

		int x1 = (x+5) * cos(degree2radian(-angle)) - (y) * sin(degree2radian(-angle));
		int y1 = (y) * cos(degree2radian(-angle)) + (x+5) * sin(degree2radian(-angle));
		int x2 = (-x-5-textRect.width()) * cos(degree2radian(-angle)) - (y) * sin(degree2radian(-angle));
		int y2 = (y) * cos(degree2radian(-angle)) + (-x-5-textRect.width()) * sin(degree2radian(-angle));

		painter->drawText(x1, y1, text);
		painter->drawText(x2, y2, text);
		painter->restore();
	}

	painter->restore();
}

void FlightSimCtrl::PaintAltitudeArea(QPainter *painter)
{
	//draw vertical line
	QPoint top(SCALE_X(1100), SCALE_Y(80));
	QPoint bottom(SCALE_X(1100), SCALE_Y(380));
	painter->drawLine(top, bottom);

	//draw triangle
	QPoint triangle[3] = {
		QPoint(top.x(), bottom.y() - (bottom.y()-top.y())/2),
		QPoint(top.x()-SCALE_X(15), bottom.y() - (bottom.y()-top.y())/2 - SCALE_Y(10)),
		QPoint(top.x()-SCALE_X(15), bottom.y() - (bottom.y()-top.y())/2 + SCALE_Y(10))
	};
	painter->drawConvexPolygon(triangle, 3);

	//test code
	QString text = QString("(%1m)").arg(m_altitude);
	QRect testRect(SCALE_X(980), SCALE_Y(220), SCALE_X(100), SCALE_Y(20));
	QPen bodyPen(Qt::green);
	QPen outlinePen(Qt::black);
	int thick = 1;
	gu_DrawTextEx(painter, testRect, Qt::AlignVCenter | Qt::AlignRight, text, 
					thick, bodyPen, outlinePen);


	int firstLine = abs((m_altitude-150)%100);
	int longLine;
	if (m_altitude < 150) 
		longLine = firstLine/20;
	else
		longLine = (firstLine == 0 || firstLine > 80) ? (0) : (4 - (firstLine-1)/20);

	for (int i = 0; i < 15; i++)
	{
		int x1, y1, x2, y2;
		int firstLineDelta = abs((m_altitude-150)%20);
		x1 = bottom.x();
		if (m_altitude < 150) 
			y1 = (firstLineDelta == 0) ? (bottom.y() - i*SCALE_Y(20)) 
							: (bottom.y() - i*SCALE_Y(20) - firstLineDelta*SCALE_Y(1)) ;
		else
			y1 = (firstLineDelta == 0) ? (bottom.y() - i*SCALE_Y(20)) 
							: (bottom.y() - i*SCALE_Y(20) - (20 - firstLineDelta*SCALE_Y(1))) ;
		y2 = y1;

		if (i % 5 == longLine)
		{
			x2 = bottom.x() + SCALE_X(30);

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
			QRect textRect(x2+SCALE_X(5), y2-SCALE_Y(10), SCALE_X(140), SCALE_Y(20));
			gu_DrawTextEx(painter, textRect, Qt::AlignVCenter | Qt::AlignLeft, text,
						thick, bodyPen, outlinePen);
		}
		else
		{
			x2 = bottom.x() + SCALE_X(20);
		}

		painter->drawLine(x1, y1, x2, y2);
	}
}

bool FlightSimCtrl::IsExitRegion(int x, int y)
{
	if (x > m_exitRect.x() && x < m_exitRect.x()+m_exitRect.width() && 
		y > m_exitRect.y() && y < m_exitRect.y()+m_exitRect.height())
		return true;

	return false;
}

void FlightSimCtrl::ExitFlightSimulator()
{
	g_app->GetMainWindow()->UpdateFlightSim();
	g_app->GetRenderProxy()->GetRenderSrv()->StopView();
}
