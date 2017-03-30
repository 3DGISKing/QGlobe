#include "tourplaybar.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "renderfrm.h"
#include "config.h"
#include "guiuty.h"
#include "rendersrv.h"
#include "rendercursor.h"
#include "GDM_PathSim.h"

TourPlayBar::TourPlayBar()
{
	memset(m_imgs, 0, sizeof(m_imgs[0])*TPCT_MAX*TPS_MAX);
	RegisterButton();
	InitState();
	
	m_timeLine.setDuration(TOURPLAY_TIMELINE_DURATION);
	m_timeLine.setFrameRange(0, TOURPLAY_TIMELINE_DURATION);
	m_timeLine.setUpdateInterval(TOURPLAY_TIMELINE_INTERVAL);

	connect(&m_timeLine, SIGNAL(finished()), this, SLOT(ClosePlayBar()));

	m_ended = false;
}

TourPlayBar::~TourPlayBar()
{
}

bool TourPlayBar::Init()
{
 	for (int i = 0; i < TPCT_MAX; i++)
	{
		for (int j = 0; j < TPS_MAX; j++)
		{
			if (strlen(s_tourPlayImgNames[i][j]) > 0)
			{
				QString fname = QString("/tourplay/") + QString(s_tourPlayImgNames[i][j]);
				QString imgname = g_app->GetResource(fname);
				m_imgs[i*3 + j] = new QImage(imgname);
			}
		}
	}
	return true;
}

void TourPlayBar::Release()
{
	for (int i = 0; i < TPCT_MAX*TPS_MAX; i++)
    {
		if (m_imgs[i] != NULL)
		{
			delete m_imgs[i];
			m_imgs[i] = NULL;
		}
    }
}

void TourPlayBar::Paint(QPainter *painter)
{
	if (!GetSimulatorStatus())
		return;

	if (m_ended)
	{
		ClosePlayBar();
		return;
	}

	painter->save();

	double z = 0.0;
	if (m_timeLine.state() == QTimeLine::Running)
		z = (double)m_timeLine.currentTime() / (double)TOURPLAY_TIMELINE_DURATION;

	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

	//QFont font = g_app->GetConfig()->m_mainFont;
	QFont font(g_app->GetFontName(true));
	font.setPointSize(g_app->GetDrawFontSize(10));
	font.setStyleStrategy(QFont::NoAntialias);
	QBrush brush(Qt::black, Qt::SolidPattern);
	QPen pen(Qt::white);

	painter->setOpacity(0.7 - 0.7 * z);
	painter->setFont(font);
	painter->setBrush(brush);
	painter->setPen(pen);
	
	QRect renderRect = g_app->GetMainWindow()->GetRenderFrame()->rect();
	QRect rect(TOURPLAYBAR_LEFT, renderRect.height() - TOURPLAYBAR_HEIGHT - TOURPLAYBAR_BOTTOM,
				TOURPLAYBAR_WIDTH, TOURPLAYBAR_HEIGHT);
	painter->drawRect(rect);
#ifdef Q_WS_MAC
        painter->drawPoint(rect.x()-1, rect.y()+rect.height()); // Bug 514 Fixed
#else
	painter->drawPoint(rect.x()+rect.width(), rect.y()-1); // Bug 514 Fixed
#endif

	painter->setOpacity(1.0 - z);

	for (int i = 0; i < TPCT_MAX; i++)
	{
		if (m_state[i] == TPS_NO)
			continue;

		painter->drawImage(s_tourRegion[i].x1, 
							renderRect.height() - s_tourRegion[i].y1, 
							*m_imgs[i*3 + m_state[i]]);
	}

	QRect textRect(247, renderRect.height() - TOURPLAYBAR_HEIGHT - TOURPLAYBAR_BOTTOM, 
					57, TOURPLAYBAR_HEIGHT);
	QPen bodyPen(QColor(qRgb(255,255,255)));
	QPen outlinePen(QColor(qRgb(0,0,0)));
	int thick = 1;
	QString seekTime = gu_msec2hms((int)m_curTime);

	gu_DrawTextEx(painter, textRect, Qt::AlignVCenter | Qt::AlignRight, seekTime,
				thick, bodyPen, outlinePen, z);

	painter->restore();
}

bool TourPlayBar::mouseMoveEvent(const CursorState *event)
{
	if (m_state[TPCT_THUMB] == TPS_ACTIVE && event->buttons == MBS_LEFT)
	{
		SeekTo(event->x);
		return true;
	}

	for (int i = 0; i < TPCT_MAX; i++)
	{
		if (m_state[i] == TPS_NO)
			continue;

		if (IsInRegion(event->x, event->y, i))
		{
			if (m_state[i] == TPS_NORMAL)
				m_state[i] = TPS_HOVER;

			return true;
		}
		else
		{
			m_state[i] = TPS_NORMAL;
		}
	}	

	if (IsInBarRegion(event->x, event->y))
	{
		RenderCursor *cursor = g_app->GetMainWindow()->GetRenderFrame()->GetRenderCursor();
		cursor->SetCursor(CURSOR_SHAPE_ARROW);
		return true;
	}

	return false;
}

bool TourPlayBar::mousePressEvent(const CursorState *event)
{
	bool res = false;

	if (event->buttons != MBS_LEFT)
		return false;

	m_pressPos.setX(event->x);
	m_pressPos.setY(event->y);

	for (int i = 0; i < TPCT_MAX; i++)
	{
		if (m_state[i] == TPS_NO)
			continue;

		if (IsInRegion(event->x, event->y, i))
		{
			if (i == TPCT_SLIDER)
				SeekTo(event->x);

			m_state[i] = TPS_ACTIVE;
			res = true;
		}
	}

    return res;
}

bool TourPlayBar::mouseReleaseEvent(const CursorState *event)
{
	bool res = false;

	if (event->buttons != MBS_LEFT)
		return false;

	if (abs(m_pressPos.x()-event->x) > 3 || abs(m_pressPos.y()-event->y) > 3)
		return false;

	for (int i = 0; i < TPCT_MAX; i++)
	{
		if (m_state[i] == TPS_NO)
			continue;

		if (IsInRegion(event->x, event->y, i))
		{
			res = (this->*ButtonAction[i])();
			if (res && i != TPCT_SLIDER)
				return true;
		}
	}

    return res;
}


/*
 * private slots
 */
void TourPlayBar::ClosePlayBar()
{
	MainWindow *mainWnd = g_app->GetMainWindow();
	if (!mainWnd)
		return;

	mainWnd->UpdateTourPlayLine();
}


/*
 * Protected
 */
void TourPlayBar::InitState()
{
	for (int i = 0; i < TPCT_MAX; i++)
	{
		m_state[i] = TPS_NORMAL;
	}

	m_state[TPCT_PREV_ON] = TPS_NO;
	m_state[TPCT_PLAY_ON] = TPS_NO;
	m_state[TPCT_NEXT_ON] = TPS_NO;
	m_state[TPCT_LOOP_ON] = TPS_NO;

	s_tourRegion[TPCT_THUMB].x1 = TOURPLAY_SEEK_START;
	m_playRate = 0;
	m_totalTime = 0;
	m_curTime = 0;
}

void TourPlayBar::RegisterButton()
{
	ButtonAction[TPCT_PREV_ON]	= &TourPlayBar::PrevOn;
	ButtonAction[TPCT_PREV_OFF]	= &TourPlayBar::PrevOff;
	ButtonAction[TPCT_PLAY_ON]	= &TourPlayBar::PlayOn;
	ButtonAction[TPCT_PLAY_OFF]	= &TourPlayBar::PlayOff;
	ButtonAction[TPCT_NEXT_ON]	= &TourPlayBar::NextOn;
	ButtonAction[TPCT_NEXT_OFF]	= &TourPlayBar::NextOff;
	ButtonAction[TPCT_SLIDER]	= &TourPlayBar::Slider;
	ButtonAction[TPCT_THUMB]	= &TourPlayBar::Thumb;
	ButtonAction[TPCT_LOOP_ON]	= &TourPlayBar::LoopOn;
	ButtonAction[TPCT_LOOP_OFF] = &TourPlayBar::LoopOff;
	ButtonAction[TPCT_EXIT]		= &TourPlayBar::Exit;
}

bool TourPlayBar::GetSimulatorStatus()
{
	CGDM_PathSim *pathSim = (CGDM_PathSim*)g_app->GetRenderProxy()->GetRenderSrv()->m_pSimMgr->GetCurSimulator();
	if (!pathSim)
		return false;

	GDM_SIM_STATUS status;
	memset(&status, 0, sizeof(GDM_SIM_STATUS));
	pathSim->GetSimulationStatus(&status);

	if (m_state[TPCT_PLAY_OFF] != TPS_NO)
		m_state[TPCT_PLAY_OFF] = (status.m_pathplaystatus == 0) ? (m_state[TPCT_PLAY_OFF]) : (TPS_NO);
	else
		m_state[TPCT_PLAY_OFF] = (status.m_pathplaystatus == 0) ? (TPS_NORMAL) : (TPS_NO);

	if (m_state[TPCT_PLAY_ON] != TPS_NO)
		m_state[TPCT_PLAY_ON] = (status.m_pathplaystatus == 0) ? (TPS_NO) : (m_state[TPCT_PLAY_ON]);
	else
		m_state[TPCT_PLAY_ON] = (status.m_pathplaystatus == 0) ? (TPS_NO) : (TPS_NORMAL);

	if (m_state[TPCT_PREV_ON] != TPS_NO)
		m_state[TPCT_PREV_ON] = (status.m_pathplaystatus < 0) ? (m_state[TPCT_PREV_ON]) : (TPS_NO);
	else
		m_state[TPCT_PREV_ON] = (status.m_pathplaystatus < 0) ? (TPS_NORMAL) : (TPS_NO);

	if (m_state[TPCT_PREV_OFF] != TPS_NO)
		m_state[TPCT_PREV_OFF] = (status.m_pathplaystatus < 0) ? (TPS_NO) : (m_state[TPCT_PREV_OFF]);
	else
		m_state[TPCT_PREV_OFF] = (status.m_pathplaystatus < 0) ? (TPS_NO) : (TPS_NORMAL);

	if (m_state[TPCT_NEXT_ON] != TPS_NO)
		m_state[TPCT_NEXT_ON] = (status.m_pathplaystatus > 1) ? (m_state[TPCT_NEXT_ON]) : (TPS_NO);
	else
		m_state[TPCT_NEXT_ON] = (status.m_pathplaystatus > 1) ? (TPS_NORMAL) : (TPS_NO);

	if (m_state[TPCT_NEXT_OFF] != TPS_NO)
		m_state[TPCT_NEXT_OFF] = (status.m_pathplaystatus > 1) ? (TPS_NO) : (m_state[TPCT_NEXT_OFF]);
	else
		m_state[TPCT_NEXT_OFF] = (status.m_pathplaystatus > 1) ? (TPS_NO) : (TPS_NORMAL);

	if (m_state[TPCT_LOOP_OFF] != TPS_NO)
		m_state[TPCT_LOOP_OFF] = (status.m_pathreplay == 0) ? (m_state[TPCT_LOOP_OFF]) : (TPS_NO);
	else
		m_state[TPCT_LOOP_OFF] = (status.m_pathreplay == 0) ? (TPS_NORMAL) : (TPS_NO);

	if (m_state[TPCT_LOOP_ON] != TPS_NO)
		m_state[TPCT_LOOP_ON] = (status.m_pathreplay == 1) ? (m_state[TPCT_LOOP_ON]) : (TPS_NO);
	else
		m_state[TPCT_LOOP_ON] = (status.m_pathreplay == 1) ? (TPS_NORMAL) : (TPS_NO);
	
	m_totalTime = status.m_pathtotaltime;
	m_curTime = status.m_pathcurrenttime;
	s_tourRegion[TPCT_THUMB].x1 = 95 + (int) ((m_curTime*137)/m_totalTime);

	m_ended	= status.m_ended;
	return true;
}

void TourPlayBar::SetSimulatorStatus(GDM_SIM_CONTROL type)
{
	CGDM_PathSim *pathSim = (CGDM_PathSim*)g_app->GetRenderProxy()->GetRenderSrv()->m_pSimMgr->GetCurSimulator();
	if (!pathSim)
		return;

	if (type == SIM_CONTROL_SCROLL)
	{
		double curTime = (double)(s_tourRegion[TPCT_THUMB].x1 - 95) / 137;
		curTime = curTime * m_totalTime;
		pathSim->ControlPlay(type, curTime);
	}
	else
	{
		pathSim->ControlPlay(type);
	}

}

bool TourPlayBar::IsInRegion(int cX, int cY, int idx)
{
	RenderFrame *render = g_app->GetMainWindow()->GetRenderFrame();
	if (!render)
		return false;

	int x1 = s_tourRegion[idx].x1;
	int y1 = render->height() - s_tourRegion[idx].y1;
	int x2 = x1 + s_tourRegion[idx].w;
	int y2 = y1 + s_tourRegion[idx].h;

	if (cX > x1 && cX < x2 && cY > y1 && cY < y2)
		return true;

	return false;
}

void TourPlayBar::SeekTo(const int x)
{
	int value = x;
	int start = s_tourRegion[TPCT_SLIDER].x1;
	int end = start + s_tourRegion[TPCT_SLIDER].w;

	if (x < start)
		value = start;
	if (x > end)
		value = end;
	
	s_tourRegion[TPCT_THUMB].x1 = value - 6;
	SetSimulatorStatus(SIM_CONTROL_SCROLL);
}

bool TourPlayBar::IsInBarRegion(int x, int y)
{
	QRect renderRect = g_app->GetMainWindow()->GetRenderFrame()->rect();
	QRect rect(TOURPLAYBAR_LEFT, renderRect.height() - TOURPLAYBAR_HEIGHT - TOURPLAYBAR_BOTTOM,
				TOURPLAYBAR_WIDTH, TOURPLAYBAR_HEIGHT);

	if (x > rect.x() && x < rect.x()+rect.width() && y > rect.y() && y < rect.y()+rect.height())
		return true;

	return false;
}

bool TourPlayBar::PrevOn()
{
	if (m_playRate == 0)
	{
		m_state[TPCT_PREV_ON] = TPS_NO;
		m_state[TPCT_PREV_OFF] = TPS_NORMAL;
	}
	else
	{
		if (m_playRate > -TOURPLAY_MAX_PLAY_RATE)
			m_playRate--;
		m_state[TPCT_PREV_ON] = TPS_NORMAL;

		SetSimulatorStatus(SIM_CONTROL_BACK);
	}

	return true;
}

bool TourPlayBar::PrevOff()
{
	m_playRate = 0;
	PlayOff();
	NextOn();

	m_state[TPCT_PREV_OFF] = TPS_NO;
	m_state[TPCT_PREV_ON] = TPS_NORMAL;

	m_playRate = -1;

	SetSimulatorStatus(SIM_CONTROL_BACK);

	return true;
}

bool TourPlayBar::PlayOn()
{
	m_state[TPCT_PLAY_ON] = TPS_NO;
	m_state[TPCT_PLAY_OFF] = TPS_NORMAL;

	m_state[TPCT_PREV_ON] = TPS_NO;
	m_state[TPCT_PREV_OFF] = TPS_NORMAL;
	m_state[TPCT_NEXT_ON] = TPS_NO;
	m_state[TPCT_NEXT_OFF] = TPS_NORMAL;
	m_state[TPCT_LOOP_ON] = TPS_NO;
	m_state[TPCT_LOOP_OFF] = TPS_NORMAL;

	m_playRate = 0;

	SetSimulatorStatus(SIM_CONTROL_STOP);

	return true;
}

bool TourPlayBar::PlayOff()
{
	m_state[TPCT_PLAY_OFF] = TPS_NO;
	m_state[TPCT_PLAY_ON] = TPS_NORMAL;

	SetSimulatorStatus(SIM_CONTROL_PLAY);

	return true;
}

bool TourPlayBar::NextOn()
{
	if (m_playRate == 0)
	{
		m_state[TPCT_NEXT_ON] = TPS_NO;
		m_state[TPCT_NEXT_OFF] = TPS_NORMAL;
	}
	else
	{
		if (m_playRate < TOURPLAY_MAX_PLAY_RATE)
			m_playRate++;
		m_state[TPCT_NEXT_ON] = TPS_NORMAL;

		SetSimulatorStatus(SIM_CONTROL_FORWARD);
	}

	return true;
}

bool TourPlayBar::NextOff()
{
	m_playRate = 0;
	PlayOff();
	PrevOn();

	m_state[TPCT_NEXT_OFF] = TPS_NO;
	m_state[TPCT_NEXT_ON] = TPS_NORMAL;

	m_playRate = 1;

	SetSimulatorStatus(SIM_CONTROL_FORWARD);

	return true;
}

bool TourPlayBar::Slider()
{
	return false;
}

bool TourPlayBar::Thumb()
{
	m_state[TPCT_THUMB] = TPS_NORMAL;

	return true;
}

bool TourPlayBar::LoopOn()
{
	m_state[TPCT_LOOP_ON] = TPS_NO;
	m_state[TPCT_LOOP_OFF] = TPS_NORMAL;

	SetSimulatorStatus(SIM_CONTROL_REPEAT);

	return true;
}

bool TourPlayBar::LoopOff()
{
	m_state[TPCT_LOOP_OFF] = TPS_NO;
	m_state[TPCT_LOOP_ON] = TPS_NORMAL;

	SetSimulatorStatus(SIM_CONTROL_REPEAT);

	return true;
}

bool TourPlayBar::Exit()
{
	m_timeLine.stop();
	m_timeLine.start();

	return true;
}
