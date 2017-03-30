#include "gdemclient.h"
#include "gdemclientbase.h"
#include "navbar.h"
#include "navctrl.h"

#include "navctrldata.h"

NavBar::NavBar()
{
    m_view = NULL;
    m_rect = QRect(0,0,0,0);
	m_mode = NM_AUTO;
    m_state = NS_BACK;
	m_mousePos = QPoint(0,0);
    m_curItem = NULL;
    memset(m_items, 0, sizeof(m_items[0])*NCT_MAX);

	m_timeLine.setDuration(NAV_TIMELINE_DURATION);
	m_timeLine.setFrameRange(0, NAV_TIMELINE_DURATION);
	m_timeLine.setUpdateInterval(NAV_TIMELINE_INTERVAL);
	m_curRule = -1;

	connect(&m_timeLine, SIGNAL(frameChanged(int)), this, SLOT(SetFrame(int)));
	connect(&m_timeLine, SIGNAL(finished()), this, SLOT(UpdateState()));
}

NavBar::~NavBar()
{
    Release();
}

bool NavBar::Init()
{
    int i;

    for (i = 0; i < NCT_MAX; i ++)
    {
		NavCtrl *item = NULL;
		switch (i)
		{
		case NCT_RING:
			item = new NavRingCtrl;
			break;

		case NCT_RING_N:
			item = new NavRingNCtrl;
			break;

		case NCT_LOOK:
			item = (NavCtrl*)new NavLookCtrl;
			break;

		case NCT_MOVE:
			item = (NavCtrl*)new NavMoveCtrl;
			break;

		case NCT_ZOOM_PLUS:
		case NCT_ZOOM_MINUS:
			item = (NavCtrl*)new NavZoomCtrl;
			break;

		case NCT_ZOOM_BAR:
			item = (NavCtrl*)new NavZoomBarCtrl;
			break;

		case NCT_COMPASS:
			item = (NavCtrl*)new NavRingCtrl;
			break;

		default:
			item = new NavCtrl;
			break;
		}
		item->SetParent(this);
        item->SetItemType(i);
        item->SetItemSpec(&s_ctrlSpec[i]);
        item->Init(s_imgNames[i]);
        m_items[i] = item;
    }

    ResetItems();
    return true;
}

void NavBar::Release()
{
    int i;
    for (i = 0; i < NCT_MAX; i ++)
    {
		NavCtrl *item = m_items[i];
        if (item)
        {
            item->Release();
            delete item;
            m_items[i] = 0;
        }
    }
}

void NavBar::ChangedViewSize(int w, int h)
{
    int i;
    for (i = 0; i < NCT_MAX; i++)
    {
		NavCtrlRegion *reg = &s_ctrlRegion[i];
		NavCtrl *item = m_items[i];
        item->SetItemPos(w-reg->x, reg->y);
        item->SetItemSize(reg->w, reg->h);
    }

    RecalcRect();
}

void NavBar::ChangedMode(int mode)
{
    if (m_mode == mode)
        return;

	m_curRule = FindStateRule(m_mode, mode);
	m_mode = mode;

	ResetItems();

	m_timeLine.stop();
	m_timeLine.start();
}

void NavBar::SetCompassAngle(double angle)
{
	((NavRingCtrl *)m_items[NCT_RING])->SetAngle(angle);
	((NavRingCtrl *)m_items[NCT_RING_N])->SetAngle(angle);
	((NavRingCtrl *)m_items[NCT_COMPASS])->SetAngle(angle);
}

void NavBar::Paint(QPainter *painter)
{
	ResetCtrlParam();

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

	if (m_timeLine.state() == QTimeLine::NotRunning)
		PaintDefault(painter);
	else
		PaintTimeLine(painter);
}

bool NavBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mode == NM_COMPASS || m_mode == NM_NODISP)
        return false;

    Qt::MouseButtons buttons = event->buttons();
    QPoint pos = event->pos();
    QRect rc = m_rect;
	NavCtrl *item = FindItem(pos);

	m_mousePos = pos;
	if (m_mode == NM_AUTO && m_state == NS_BACK && buttons == 0)
    {
        if (rc.contains(pos))
        {
            m_state = NS_NORMAL;
			m_curRule = FindStateRule(NM_AUTO, NM_ALWAYS);
			m_timeLine.stop();
			m_timeLine.start();
            return true;
        }
    }
	else if (m_state == NS_NORMAL && buttons == 0)
    {
		if (m_mode == NM_AUTO && !rc.contains(pos))
        {
            m_state = NS_BACK;
			m_curRule = FindStateRule(NM_ALWAYS, NM_AUTO);
			m_timeLine.stop();
			m_timeLine.start();
			return true;
        }
		else if (item)
        {
            m_state = NS_HOVER;
            m_curItem = item;
			m_curItem->SetCursor(NS_HOVER);
			Update();
			return true;
        }
    }
    else if (m_state == NS_HOVER && buttons == 0)
    {
        if (item == NULL)
        {
            m_state = NS_NORMAL;
			m_curItem->SetCursor(-1);
            m_curItem = NULL;
        }
		else if (m_curItem != item)
        {
			m_curItem->SetCursor(-1);
            m_curItem = item;
			m_curItem->SetCursor(NS_HOVER);
		}
        Update();
        return true;
    }
    else if (m_state == NS_ACTIVE)
    {
		m_curItem->Action();
        Update();
        return true;
    }

    return false;
}

bool NavBar::mousePressEvent(QMouseEvent *event)
{
    if (m_mode == NM_COMPASS || m_mode == NM_NODISP)
        return false;

    Qt::MouseButtons buttons = event->buttons();
	QPoint pos = event->pos();

    if (m_state == NS_HOVER && buttons == Qt::LeftButton)
    {
		m_state = NS_ACTIVE;
		m_mousePressPos = pos;
		m_curItem->StartAction();
		m_timer.start(NAV_TIMER_INTERVAL, this);
		Update();
		return true;
    }

    return false;
}

bool NavBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_mode == NM_COMPASS || m_mode == NM_NODISP)
        return false;

    Qt::MouseButtons buttons = event->buttons();

    if (m_state == NS_ACTIVE)
    {
		QPoint pos = event->pos();
		NavCtrl *item = FindItem(pos);
		if(m_curItem->GetItemType() == NCT_RING_N)
		{
			//[2009.11.28][ChaKI] Inserted
			int dx = abs(m_mousePressPos.x() - m_mousePos.x());
			int dy = abs(m_mousePressPos.y() - m_mousePos.y());
			bool blChanged = (dx > 2 || dy > 2) ? true : false;
			if(!blChanged)
				m_curItem->EndAction(item);
		} 
		else
			m_curItem->EndAction(item);
		m_state = NS_HOVER;
		m_timer.stop();
        Update();
		return true;
    }

    return false;
}

bool NavBar::MouseClickEvent(const CursorState *event)
{
	if (m_state == NS_HOVER || m_state == NS_ACTIVE)
		return true;

	return false;
}

void NavBar::timerEvent(QTimerEvent *event)
{
	if (m_state == NS_ACTIVE && m_curItem != NULL)
		m_curItem->RepeatAction();
}

/*
 * private slots
 */
void NavBar::SetFrame(int frame)
{
	Update();
}

void NavBar::UpdateState()
{
	m_curRule = -1;
}

/*
 * Protected
 */
void NavBar::ResetItems()
{
	int i = 0;
	for (i = 0; i < NCT_MAX; i++)
		m_items[i]->SetVisible(false);

	m_state = (m_mode == NM_ALWAYS) ? NS_NORMAL : NS_BACK;
	m_curItem = NULL;

	if (m_mode == NM_AUTO || m_mode == NM_ALWAYS)
	{
		for (int i = 0; i < NCT_COMPASS; i ++)
			m_items[i]->SetVisible(true);
	}
	else if (m_mode == NM_COMPASS)
	{
		m_items[NCT_COMPASS]->SetVisible(true);
	}
}

void NavBar::RecalcRect()
{
	m_rect.setRect(0,0,0,0);

	int i;
	for (i = 0; i < NCT_MAX; i ++)
	{
		NavCtrl *item = m_items[i];
		QRect rc = item->GetItemRect();
		if (m_rect.isEmpty())
		{
			m_rect = rc;
		}
		else
		{
			if (m_rect.left() > rc.left())
				m_rect.setLeft(rc.left());
			if (m_rect.top() > rc.top())
				m_rect.setTop(rc.top());
			if (m_rect.right() < rc.right())
				m_rect.setRight(rc.right());
			if (m_rect.bottom() < rc.bottom())
				m_rect.setBottom(rc.bottom());
		}
	}
}

void NavBar::Update(QRect *org)
{
#ifdef APP_RENDER_IN_PAINT
	if (m_view)
    {
		m_view->update();
		/*
        if (org)
			m_view->repaint(*org);
		else
			m_view->repaint(m_rect);
		*/
    }
#endif
}

NavCtrl* NavBar::FindItem(QPoint &pos)
{
    int i = 0;
	for (i = 0; s_checkOrder[i] >= 0; i ++)
    {
		int num = s_checkOrder[i];
		if (m_items[num]->Contains(pos))
			return m_items[num];
    }
    return NULL;
}

int NavBar::FindStateRule(int curMode, int newMode)
{
	int i;
	int res = -1;
	for (i = 0; s_stateRule[i].curMode >= 0; i ++)
	{
		if (curMode == s_stateRule[i].curMode &&
			newMode == s_stateRule[i].newMode)
		{
			res = i;
			break;
		}
	}
	return res;
}

NavStateRule* NavBar::GetStateRule(int num)
{
	if (num < 0)
		return NULL;

	NavStateRule *sr = NULL;
	for (int i=0; s_stateRule[i].curMode>=0; i++)
	{
		if (i == num)
			sr = &s_stateRule[i];
	}
	return sr;
}

void NavBar::ResetCtrlParam()
{
	bool angleFlag = true;

	if (m_state == NS_ACTIVE)
	{
		if (m_curItem == m_items[NCT_RING] || m_curItem == m_items[NCT_RING_N])
			angleFlag = false;
	}

	if (angleFlag)
	{
		RenderProxy *render = g_app->GetRenderProxy();
		double angle = 0.0;
		render->SendMessage(PM_GET_COMPASSANGLE, (PrxParam)&angle);
		angle = radian2degree(angle);
		((NavRingCtrl*)m_items[NCT_RING])->SetAngle(angle);
		((NavRingCtrl*)m_items[NCT_RING_N])->SetAngle(angle);
		((NavRingCtrl*)m_items[NCT_COMPASS])->SetAngle(angle);
	}
}

void NavBar::PaintDefault(QPainter *painter)
{
	if (m_mode == NM_NODISP)
		return;

	int state = (m_mode==NM_ALWAYS || m_state>=NS_NORMAL) ? NS_NORMAL : NS_BACK;

	int i = 0;
	for (i = 0; i < NCT_MAX; i ++)
	{
		NavCtrl *item = m_items[i];

		if (!item->IsVisible())
			continue;

		if (m_curItem == item)
		{
			item->Paint(painter, m_state);
			continue;
		}

		if (m_curItem == m_items[NCT_RING] && i == NCT_RING_N)
			continue;
		if (m_curItem == m_items[NCT_RING_N] && i == NCT_RING)
			continue;

		if (m_curItem == m_items[NCT_ZOOM_PLUS])
		{
			if (i == NCT_ZOOM || i == NCT_ZOOM_MINUS)
				continue;
		}
		if (m_curItem == m_items[NCT_ZOOM_MINUS])
		{
			if (i == NCT_ZOOM || i == NCT_ZOOM_PLUS)
				continue;
		}

		item->Paint(painter, state);
	}
}

void NavBar::PaintTimeLine(QPainter *painter)
{
	NavStateRule *sr = GetStateRule(m_curRule);
	if (sr == NULL)
		return;
	double z = (double)m_timeLine.currentTime() / (double)NAV_TIMELINE_DURATION;
	int i = 0;

	for (i = 0; i < NCT_MAX; i ++)
	{
		NavCtrl *item = m_items[i];
		int state = -1;

		//delete item
		state = (IsJoysticGroup(i)) ? sr->joyDel : sr->comDel;
		if (state >= 0)
		{
			painter->setOpacity(1.0-z);
			item->Paint(painter, state);
		}

		//disp item
		state = (IsJoysticGroup(i)) ? sr->joyDisp : sr->comDisp;
		if (state >= 0)
		{
			painter->setOpacity(z);
			item->Paint(painter, state);
		}
	}

	painter->setOpacity(1.0);
}
