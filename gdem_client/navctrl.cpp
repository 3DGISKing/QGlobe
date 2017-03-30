#include <math.h>
#include "gdemclient.h"
#include "renderfrm.h"
#include "rendercursor.h"
#include "navbar.h"
#include "navctrl.h"
#include "gdemclientbase.h"

static double CalcAngle(int dx, int dy)
{
	if (dx == 0 && dy == 0)
		return 0.0;

	double alpha = atan2((double)dy, (double)dx);
	alpha = alpha * 180.0 / PAI;

	return alpha;
}

/*
 * class NavCtrl
 */
NavCtrl::NavCtrl()
{
	m_parent = NULL;
    m_type = -1;
    memset(m_imgs, 0, sizeof(m_imgs[0])*NS_MAX);
    memset(&m_reg, 0, sizeof(m_reg));
    memset(&m_spec, 0, sizeof(m_spec));
    m_visible = false;
	m_orgCursor = -1;
}

NavCtrl::~NavCtrl()
{
}

bool NavCtrl::Init(char **imgs)
{
    int i;
    for (i = 0; i < NS_MAX && imgs[i] != 0; i ++)
    {
        if (strlen(imgs[i]) > 0)
        {
            QString fname = QString("/nav/") + QString(imgs[i]);
            QString imgname = g_app->GetResource(fname);
            m_imgs[i] = new QImage(imgname);
        }
    }
    return true;
}

void NavCtrl::Release()
{
    int i;
    for (i = 0; i < NS_MAX; i ++)
    {
        if (m_imgs[i] != NULL)
        {
            delete m_imgs[i];
            m_imgs[i] = NULL;
        }
    }
}

QRect NavCtrl::GetItemRect()
{
	QRect rc;
	rc.setRect(m_reg.x-m_reg.w/2, m_reg.y-m_reg.h/2, m_reg.w, m_reg.h);
	return rc;
}

void NavCtrl::SetCursor(int state)
{
	QWidget *view = m_parent->GetView();
	RenderCursor *rcursor = ((RenderFrame*)view)->GetRenderCursor();

	if (state == NS_HOVER)
	{
		m_orgCursor = rcursor->GetCurrentCursor();
		rcursor->SetCursor(CURSOR_SHAPE_ARROW);
	}
	else if (state == -1 && m_orgCursor >= 0)
	{
		rcursor->SetCursor(m_orgCursor);
		m_orgCursor = -1;
	}
}

bool NavCtrl::Contains(QPoint &pos)
{
	if (m_spec.movType == NCMT_NO)
	{
		int x = m_reg.x;
		int y = m_reg.y + m_spec.r;
		QRect rc(x-m_spec.w/2, y-m_spec.h/2, m_spec.w, m_spec.h);
		return rc.contains(pos);
	}
	else if (m_spec.movType == NCMT_ROTATE)
    {
        QRect rc = GetItemRect();
        QRegion reg(rc, QRegion::Ellipse);
        return reg.contains(pos);
    }

    return false;
}

void NavCtrl::Paint(QPainter *painter, int state)
{
    if (state < 0 || state >= NS_MAX)
    {
#ifdef DEBUG
		qDebug("ERROR:NavCtrl::Paint: state=%d", state);
#endif
        return;
    }

	QImage *img = m_imgs[state];
	if (img)
	{
		QSize size = img->size();
		int x, y;
		x = m_reg.x - size.width()/2;
		y = m_reg.y - size.height()/2;

		painter->drawImage(x, y, *img);
	}
}

/*
 * class NavRingCtrl
 */
NavRingCtrl::NavRingCtrl()
{
	m_orgAngle = 0.0;
	m_curAngle = 0.0;
}
void NavRingCtrl::Paint(QPainter *painter, int state)
{
	QImage *img = m_imgs[state];
	if (!img)
		return;

	QSize size = img->size();
	double angle = 360 - m_curAngle;
	QMatrix matrix;

	matrix.translate((qreal)m_reg.x, (qreal)m_reg.y);
	matrix.rotate(angle);

	painter->save();
	painter->setMatrix(matrix);
	painter->drawImage(-size.width()/2, -size.height()/2, *img);
	painter->restore();
}

void NavRingCtrl::StartAction()
{
	QPoint pos = m_parent->GetMousePos();

	m_orgAngle = CalcAngle(pos.x()-m_reg.x, m_reg.y-pos.y());
	m_orgAngle -= m_curAngle;
}

void NavRingCtrl::Action()
{
	QPoint pos = m_parent->GetMousePos();
	double newAngle = CalcAngle(pos.x() - m_reg.x, m_reg.y - pos.y());

	m_curAngle = newAngle - m_orgAngle;
	g_app->ChangedCompassAngle(m_curAngle);
}

/*
 * class NavRingNCtrl
 */
bool NavRingNCtrl::Contains(QPoint &pos)
{
	double curAngle = m_curAngle + 90.0;
	double rad = degree2radian(curAngle);
	int dy = (int)(sin(rad) * m_spec.r);
	int dx = (int)(cos(rad) * m_spec.r);
	QPoint p = GetItemPos();
	QRect rc;
	rc.setRect(p.x()+dx-m_spec.w/2, p.y()-dy-m_spec.h/2, m_spec.w, m_spec.h);
	return rc.contains(pos);
}

void NavRingNCtrl::EndAction(NavCtrl *item)
{
	if (item == this)
		g_app->ResetView();
}

/*
 * class NavMoveCtrl
 */
void NavMoveCtrl::Paint(QPainter *painter, int state)
{
	if (state < NS_HOVER)
	{
		NavCtrl::Paint(painter, state);
	}
	else
	{
		NavCtrl::Paint(painter, NS_NORMAL);

		QImage *img = m_imgs[state];
		if (img)
		{
			QPoint mousePos = m_parent->GetMousePos();
			QSize size = img->size();

			double angle = 360.0 - CalcAngle(mousePos.x() - m_reg.x, m_reg.y - mousePos.y());

			painter->save();
			painter->setWindow(0, 0, 1, 1);
			painter->setViewport(m_reg.x, m_reg.y, 1, 1);
			painter->rotate(angle);
			painter->drawImage(-size.width()/2, -size.height()/2, *img);
			painter->restore();
		}
	}
}

void NavMoveCtrl::RepeatAction()
{
	QPoint pos = m_parent->GetMousePos();
	int dx = pos.x() - m_reg.x;
	int dy = m_reg.y - pos.y();

	g_app->MoveView(dx, dy);
}

/*
 * class NavLookCtrl
 */
void NavLookCtrl::RepeatAction()
{
	QPoint pos = m_parent->GetMousePos();
	int dx = pos.x() - m_reg.x;
	int dy = m_reg.y - pos.y();

	g_app->LookView(dx, dy);
}

/*
 * class NavZoomCtrl
 */
void NavZoomCtrl::StartAction()
{
	QWidget *view = m_parent->GetView();
	RenderCursor *rcursor = ((RenderFrame*)view)->GetRenderCursor();

	rcursor->SetCursor(CURSOR_SHAPE_SIZEVERT);
	RepeatAction();
}

void NavZoomCtrl::EndAction(NavCtrl *item)
{
	QWidget *view = m_parent->GetView();
	RenderCursor *rcursor = ((RenderFrame*)view)->GetRenderCursor();

	rcursor->SetCursor(CURSOR_SHAPE_ARROW);
}

void NavZoomCtrl::RepeatAction()
{
	int speed = (m_type == NCT_ZOOM_PLUS) ? ZOOM_SPEED_MAX : -ZOOM_SPEED_MAX;
	g_app->ZoomView(speed);
}

/*
 * class NavZoomBarCtrl
 */
NavZoomBarCtrl::NavZoomBarCtrl()
{
	m_offset = 0;
}

bool NavZoomBarCtrl::Contains(QPoint &pos)
{
	int x = m_reg.x;
	int y = m_reg.y - m_offset;
	QRect rc(x-m_spec.w/2, y-m_spec.h/2, m_spec.w, m_spec.h);
	return rc.contains(pos);
}

void NavZoomBarCtrl::Paint(QPainter *painter, int state)
{
	QImage *img = m_imgs[state];
	if (img)
	{
		QSize size = img->size();
		int x, y;
		x = m_reg.x - size.width()/2;
		y = m_reg.y - size.height()/2 - m_offset;

		painter->drawImage(x, y, *img);
	}
}

void NavZoomBarCtrl::RepeatAction()
{
	QPoint pos = m_parent->GetMousePos();
	int dy = m_reg.y - pos.y();
	if (dy < m_spec.minVal)
		dy = m_spec.minVal;
	if (dy > m_spec.maxVal)
		dy = m_spec.maxVal;
	m_offset = dy;
	int delta = m_spec.maxVal / ZOOM_SPEED_MAX;
	int speed = abs(dy)/delta;
	if (speed > 0)
	{
		if (dy < 0)
			speed = -speed;
		g_app->ZoomView(speed);
	}
}

void NavZoomBarCtrl::EndAction(NavCtrl *item)
{
	NavZoomCtrl::EndAction(item);
	m_offset = 0;
}
