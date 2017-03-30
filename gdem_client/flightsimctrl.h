#ifndef FLIGHTSIMCTRL_H
#define FLIGHTSIMCTRL_H

#include <QObject>
#include "gdemclientbase.h"

enum FLIGHT_SIM_AREA_TYPE {
// 	FS_FIXED_AREA,
// 	FS_SPEED_AREA,
// 	FS_HEADING_AREA,
// 	FS_BANKINGANGLE_AREA,
// 	FS_VERTICALSPEED_AREA,
// 	FS_THROTTLE_AREA,
// 	FS_RUDDER_AREA,
// 	FS_AILERON_AREA,
// 	FS_ELEVATOR_AREA,
// 	FS_FLAPGEAR_AREA,
// 	FS_PITCHANGLE_AREA,
// 	FS_ALTITUDE_AREA,
	FS_EXITBUTTON_AREA,

	FS_AREA_COUNT
};

class FlightSimCtrl : QObject
{
	Q_OBJECT
public:
	FlightSimCtrl();
	~FlightSimCtrl();

	void Paint(QPainter *painter);
	bool mouseMoveEvent(const CursorState *event);
    bool mousePressEvent(const CursorState *event);
	void timerEvent(QTimerEvent *event);

protected:
	int			m_speed;
	double		m_heading;
	double		m_bankangle;
	double		m_verticalspeed;
	double		m_throttle;	// 0 ~ 1
	double		m_rudder;	// -1 ~ 1
	double		m_aileron;	// -1 ~ 1
	double		m_elevator;	// -1 ~ 1
	int			m_flap;
	bool		m_gear;
	double		m_pitchangle;
	int			m_altitude;
	bool		m_stall;
	bool		m_ended;
	bool		m_showHelp;
	bool		m_collided;

	QRect		m_exitRect;
	bool		m_exitRegion;
	QTimeLine	m_timeLine;

	bool		m_showMessageBox;
	QBasicTimer		m_collidTimer;

	
	void InitDrawPath();
	bool GetParam();

	void (FlightSimCtrl::*PaintArea[FS_AREA_COUNT])(QPainter *painter);
	void PaintFixedArea(QPainter *painter);
	void PaintSpeedArea(QPainter *painter);
	void PaintHeadingArea(QPainter *painter);
	void PaintBankAngleArea(QPainter *painter);
	void PaintVerticalSpeedArea(QPainter *painter);
	void PaintExitButtonArea(QPainter *painter);
	void PaintThrottleArea(QPainter *painter);
	void PaintRudderArea(QPainter *painter);
	void PaintAileronArea(QPainter *painter);
	void PaintElevatorArea(QPainter *painter);
	void PaintFlapGearArea(QPainter *painter);
	void PaintPitchAngleArea(QPainter *painter);
	void PaintAltitudeArea(QPainter *painter);

	bool IsExitRegion(int x, int y);

private slots:
	void ExitFlightSimulator();
};

#endif // FLIGHTSIMCTRL_H
