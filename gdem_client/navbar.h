#ifndef NAVBAR_H
#define NAVBAR_H

#include <qobject.h>
#include <qbasictimer.h>
#include <qtimeline.h>
#include "gdemclientbase.h"
#include "navctrl.h"

#define NAV_TIMER_INTERVAL		40
#define NAV_TIMELINE_DURATION	500
#define NAV_TIMELINE_INTERVAL	100

enum NavMode   { NM_AUTO, NM_ALWAYS, NM_COMPASS, NM_NODISP };

typedef struct _NavStateRule
{
	int curMode;
	int newMode;
	int joyDel;
	int joyDisp;
	int comDel;
	int comDisp;
} NavStateRule;

class NavBar : QObject
{
	Q_OBJECT
public:
	NavBar();
	~NavBar();

    void SetView(QWidget *view)
    {   m_view = view; }

	QWidget* GetView()
	{	return m_view; }

    QRect GetRect()
    {   return m_rect; }

	QPoint GetMousePos()
	{	return m_mousePos; }

	bool IsJoysticGroup(int num)
	{	return (num >= 0 && num <= NCT_ZOOM_BAR) ? true : false; }

	bool IsCompassGroup(int num)
	{	return (num == NCT_COMPASS) ? true : false; }

    bool Init();
    void Release();
    void ChangedViewSize(int w, int h);
    void ChangedMode(int mode);
	void SetCompassAngle(double angle);
    void Paint(QPainter *painter);

    bool mouseMoveEvent(QMouseEvent *event);
    bool mousePressEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
	bool MouseClickEvent(const CursorState *event);

protected:
	virtual void timerEvent(QTimerEvent *event);

private slots:
	void SetFrame(int frame);
	void UpdateState();

protected:
    QWidget *m_view;
    QRect   m_rect;
    int     m_mode;
    int     m_state;
	QPoint	m_mousePos;
	QPoint	m_mousePressPos;


	NavCtrl *m_curItem;
	NavCtrl *m_items[NCT_MAX];

	QBasicTimer m_timer;
	QTimeLine	m_timeLine;
	int			m_curRule;

    void ResetItems();
    void RecalcRect();
    void Update(QRect *org = NULL);
	NavCtrl* FindItem(QPoint &pos);
	int FindStateRule(int curMode, int newMode);
	NavStateRule* GetStateRule(int num);
	void ResetCtrlParam();

	void PaintDefault(QPainter *painter);
	void PaintTimeLine(QPainter *painter);
};

#endif // NAVBAR_H
