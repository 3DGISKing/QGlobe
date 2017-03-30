#ifndef TOURPLAYBAR_H
#define TOURPLAYBAR_H

#include <QObject>
#include "gdemclientbase.h"
#include "tourplayctrldata.h"
#include "GDM_SimMgr.h"

#define TOURPLAYBAR_WIDTH			353
#define TOURPLAYBAR_HEIGHT			40
#define TOURPLAYBAR_LEFT			5
#define TOURPLAYBAR_BOTTOM			45

#define TOURPLAY_TIMER_INTERVAL		40
#define TOURPLAY_TIMELINE_DURATION	500
#define TOURPLAY_TIMELINE_INTERVAL	100

#define TOURPLAY_SEEK_START			95
#define	TOURPLAY_MAX_PLAY_RATE		10

enum TPB_EVENT_TYPE { TPB_MOUSE_MOVE, TPB_MOUSE_PRESS, TPB_MOUSE_RELEASE };

class TourPlayBar : QObject
{
	Q_OBJECT
public:
	TourPlayBar();
	~TourPlayBar();

	bool Init();
	void Release();

	void Paint(QPainter *painter);
	bool mouseMoveEvent(const CursorState *event);
    bool mousePressEvent(const CursorState *event);
    bool mouseReleaseEvent(const CursorState *event);

private slots:
	void ClosePlayBar();

protected:
	QImage		*m_imgs[TPCT_MAX*TPS_MAX];
	int			m_state[TPCT_MAX];
	bool		(TourPlayBar::*ButtonAction[TPCT_MAX])(void);
	QTimeLine	m_timeLine;
	QPoint		m_pressPos;

	int			m_playRate;
	double		m_totalTime;
	double		m_curTime;

	bool	m_ended;

	void InitState();
	void RegisterButton();
	bool GetSimulatorStatus();
	void SetSimulatorStatus(GDM_SIM_CONTROL type);
	bool IsInRegion(int cX, int cY, int idx);
	void SeekTo(const int x);
	bool IsInBarRegion(int x, int y);

	bool PrevOn();
	bool PrevOff();
	bool PlayOn();
	bool PlayOff();
	bool NextOn();
	bool NextOff();
	bool Slider();
	bool Thumb();
	bool LoopOn();
	bool LoopOff();
	bool Exit();
};

#endif // TOURPLAYBAR_H
