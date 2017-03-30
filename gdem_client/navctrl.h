#ifndef NavCtrl_H
#define NavCtrl_H

#include <QtGui>
#include <QImage>
#include "gdemclientbase.h"

#define NAV_TIMER_INTERVAL	40

class NavBar;

enum NavState  { NS_BACK, NS_NORMAL, NS_HOVER, NS_ACTIVE, NS_MAX };

enum NavCtrlType
{
	NCT_RING_N, NCT_RING, NCT_LOOK, NCT_MOVE,
	NCT_ZOOM, NCT_ZOOM_MINUS, NCT_ZOOM_PLUS, NCT_ZOOM_BAR,
    NCT_COMPASS,
    NCT_MAX
};

enum NavCtrlMovetype
{
    NCMT_NO, NCMT_ROTATE, NCMT_SLIDE
};

typedef struct _NavCtrlRegion
{
    int x;  //center x
    int y;  //center y;
    int w;
    int h;
} NavCtrlRegion;

typedef struct _NavCtrlSpec
{
    int movType;
    int r;
    int w;
    int h;
    int minVal;
    int maxVal;
} NavCtrlSpec;

class NavCtrl
{
public:
	NavCtrl();
	virtual ~NavCtrl();

public:
	void SetParent(NavBar *parent)
	{	m_parent = parent; }

    void SetItemType(int type)
    {   m_type = type; }

	int GetItemType()
    {   return m_type; }

    void SetItemPos(int x, int y)
    {   m_reg.x = x; m_reg.y = y;}

    QPoint GetItemPos()
    {   return QPoint(m_reg.x, m_reg.y); }

    void SetItemSize(int w, int h)
    {   m_reg.w = w; m_reg.h = h; }

    QSize GetItemSize()
    {   return QSize(m_reg.w, m_reg.y); }

    void SetVisible(bool flag)
    {   m_visible = flag; }

    void SetItemSpec(NavCtrlSpec *spec)
    {   m_spec = *spec; }

    void GetItemSpec(NavCtrlSpec *spec)
    {   *spec = m_spec; }

    bool IsVisible()
    {   return m_visible; }

    bool Init(char **imgs);
    void Release();
	QRect GetItemRect();

	virtual void SetCursor(int state);
	virtual bool Contains(QPoint &pos);
	virtual void Paint(QPainter *painter, int state);
	virtual void StartAction() {	RepeatAction();		}
	virtual void EndAction(NavCtrl *item = NULL) {}
	virtual void Action() {}
	virtual void RepeatAction() {}

protected:
	NavBar *m_parent;
	int		m_type;
    QImage  *m_imgs[NS_MAX];
    NavCtrlRegion m_reg;
    NavCtrlSpec m_spec;
    bool m_visible;
	int m_orgCursor;
};

class NavRingCtrl : public NavCtrl
{
public:
	NavRingCtrl();
	virtual ~NavRingCtrl() {}

	virtual void Paint(QPainter *painter, int state);
	virtual void StartAction();
	virtual void Action();

	void SetAngle(double angle)
	{	m_curAngle = angle; }

protected:
	double m_orgAngle;
	double m_curAngle;
};

class NavRingNCtrl : public NavRingCtrl
{
public:
	NavRingNCtrl() {}
	virtual ~NavRingNCtrl() {}

	virtual bool Contains(QPoint &pos);
	virtual void EndAction(NavCtrl *item = NULL);

protected:
};

class NavMoveCtrl : public NavCtrl
{
public:
	NavMoveCtrl() {}
	virtual ~NavMoveCtrl() {}
	virtual void Paint(QPainter *painter, int state);
	virtual void RepeatAction();
};

class NavLookCtrl : public NavMoveCtrl
{
public:
	NavLookCtrl() {}
	virtual ~NavLookCtrl() {}
	virtual void RepeatAction();
};

class NavZoomCtrl : public NavCtrl
{
public:
	NavZoomCtrl() {}
	virtual ~NavZoomCtrl() {}
	virtual void StartAction();
	virtual void EndAction(NavCtrl *item = NULL);
	virtual void RepeatAction();
};

class NavZoomBarCtrl : public NavZoomCtrl
{
public:
	NavZoomBarCtrl();
	virtual ~NavZoomBarCtrl() {}

	virtual bool Contains(QPoint &pos);
	virtual void Paint(QPainter *painter, int state);
	virtual void EndAction(NavCtrl *item = NULL);
	virtual void RepeatAction();

protected:
	int m_offset;
};

#endif // NavCtrl_H
