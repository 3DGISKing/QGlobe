#include "carsimctrl.h"
#include "gdemclient.h"
#include "guiuty.h"
#include "config.h"
#include "renderfrm.h"
#include "rendersrv.h"
#include "rendercursor.h"
#include "mainwindow.h"
#include "msgdata.h"
#include "GDM_PathSim.h"

CarSimCtrl::CarSimCtrl()
{
	m_speed = 0;
	m_travelTime = 0;
	m_travelDistance = 0;

	m_exitRegion = false;
	m_timeLine.setDuration(500);
	m_timeLine.setFrameRange(0, 500);
	m_timeLine.setUpdateInterval(100);

	connect(&m_timeLine, SIGNAL(finished()), this, SLOT(ExitCarSimulator()));
}

CarSimCtrl::~CarSimCtrl()
{
}

void CarSimCtrl::Paint(QPainter *painter)
{
	if (!GetSimulatorStatus())
		return;

	if ( m_ended )
	{
		g_app->GetMainWindow()->UpdateCarSim();
		return;
	}

	if ( !m_showHelp )
		return ;

	painter->save();

#if 1
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

	PaintCompass(painter);
	PaintDashboard(painter);

	QPen bodyPen(Qt::green);
	QPen outlinePen(Qt::black);
	int thick = 1;

	m_speed *= 3.6;
	int speed = m_speed;
	QRect speedRect(SCALE_X(150), SCALE_Y(100), SCALE_X(500), SCALE_Y(20));
	QString text;
	text = QString("%1 : %2 km/h").arg(tr("Running Speed")).arg(speed);
	gu_DrawTextEx(painter, speedRect, Qt::AlignVCenter | Qt::AlignLeft, text, 
				thick, bodyPen, outlinePen); 

	QRect travelRect(SCALE_X(150), SCALE_Y(130), SCALE_X(500), SCALE_Y(20));
	QString travellingTime = gu_sec2hms(m_travelTime);
	text = QString("%1 : %2").arg(tr("Running Time")).arg(travellingTime);
	gu_DrawTextEx(painter, travelRect, Qt::AlignVCenter | Qt::AlignLeft, text, 
				thick, bodyPen, outlinePen);

	m_travelDistance *= 0.001;
	QRect trochRect(SCALE_X(150), SCALE_Y(160), SCALE_X(500), SCALE_Y(20));
	text = QString("%1 : %2 km").arg(tr("Running Distance")).arg(m_travelDistance, 0, 'f', 2);
	gu_DrawTextEx(painter, trochRect, Qt::AlignVCenter | Qt::AlignLeft, text, 
				thick, bodyPen, outlinePen);
#endif
	PaintExitButton(painter);

	painter->restore();
}

bool CarSimCtrl::mouseMoveEvent(const CursorState *event)
{
	if (event->x > m_exitRect.x() && event->x < m_exitRect.x()+m_exitRect.width() && 
		event->y > m_exitRect.y() && event->y < m_exitRect.y()+m_exitRect.height())
		m_exitRegion = true;
	else
		m_exitRegion = false;

	if (m_exitRegion)
	{
		RenderCursor *cursor = g_app->GetMainWindow()->GetRenderFrame()->GetRenderCursor();
		cursor->SetCursor(CURSOR_SHAPE_ARROW);
	}

	return m_exitRegion;
}

bool CarSimCtrl::mousePressEvent(const CursorState *event)
{
	if (event->x > m_exitRect.x() && event->x < m_exitRect.x()+m_exitRect.width() && 
		event->y > m_exitRect.y() && event->y < m_exitRect.y()+m_exitRect.height())
	{
		m_timeLine.stop();
		m_timeLine.start();

		return true;
	}

	return false;
}

bool CarSimCtrl::GetSimulatorStatus()
{
	CGDM_Simulator *carSim = (CGDM_Simulator*)g_app->GetRenderProxy()->GetRenderSrv()->m_pSimMgr->GetCurSimulator();
	if (!carSim)
		return false;

	GDM_SIM_STATUS status;
	memset(&status, 0, sizeof(GDM_SIM_STATUS));
	carSim->GetSimulationStatus(&status);

	m_speed = status.m_speed;
	m_travelTime = status.m_travelTime;
	m_travelDistance = status.m_travelDistance;
	m_ended	= status.m_ended;
	m_showHelp = status.m_showHelp;

	return true;
}

void CarSimCtrl::PaintCompass(QPainter *painter)
{
	painter->save();

	//QRect rect(30, 700, 160, 160);
	////painter->drawArc(rect, 0, 360*16);

	////painter->drawLine(110, 700, 110, 710);
	////painter->drawLine(190, 780, 180, 780);
	////painter->drawLine(110, 860, 110, 850);
	////painter->drawLine(30, 780, 40, 780);

	//painter->drawText(102, 725, MSG_FLIGHT_SIM_NORTH);
	//painter->drawText(165, 785, MSG_FLIGHT_SIM_EAST);
	//painter->drawText(102, 845, MSG_FLIGHT_SIM_SOUTH);
	//painter->drawText(40, 785, MSG_FLIGHT_SIM_WEST);

	//QPen penN(Qt::red, 2);
	//painter->setPen(penN);
	//painter->drawLine(110, 735, 110, 780);

	//QPen penS(Qt::yellow, 2);
	//painter->setPen(penS);
	//painter->drawLine(110, 825, 110, 780);

	painter->restore();
}

void CarSimCtrl::PaintDashboard(QPainter *painter)
{
	painter->save();
	painter->restore();
}

void CarSimCtrl::PaintExitButton(QPainter *painter)
{
	double z = 0.0;
	if (m_timeLine.state() == QTimeLine::Running)
		z = (double)m_timeLine.currentTime() / 100;
	
	painter->save();

	//QFont font = g_app->GetConfig()->m_mainFont;
	QFont font(g_app->GetFontName(true));
	font.setPointSize(g_app->GetDrawFontSize(10));
	font.setStyleStrategy(QFont::NoAntialias);
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

	QString text = tr("Exit Car Simulator");
	QRect rect(SCALE_X(1000), SCALE_Y(20), SCALE_X(250), SCALE_Y(20));
	m_exitRect = painter->boundingRect(rect, Qt::AlignVCenter | Qt::AlignRight, text);
	QRect drawRect = m_exitRect;
        drawRect.setX(m_exitRect.x() - 5);
        drawRect.setY(m_exitRect.y() - 2);
        drawRect.setWidth(m_exitRect.width() + 10);
        drawRect.setHeight(m_exitRect.height() + 4);
	painter->drawRect(drawRect);

	QPen bodyPen(Qt::white);
	QPen outlinePen(Qt::black);
	int thick = 1;

        gu_DrawTextEx(painter, m_exitRect, Qt::AlignVCenter | Qt::AlignRight, text,
				thick, bodyPen, outlinePen);

	painter->restore();
}

void CarSimCtrl::ExitCarSimulator()
{
	g_app->GetMainWindow()->UpdateCarSim();
}
