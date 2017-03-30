#ifndef CARSIMCTRL_H
#define CARSIMCTRL_H

#include <QObject>
#include "gdemclientbase.h"

class CarSimCtrl : QObject
{
	Q_OBJECT
public:
	CarSimCtrl();
	~CarSimCtrl();

	void Paint(QPainter *painter);
	bool mouseMoveEvent(const CursorState *event);
    bool mousePressEvent(const CursorState *event);

protected:
	double	m_speed;
	double	m_travelTime;
	double	m_travelDistance;
	bool	m_ended;
	bool	m_showHelp;

	QRect		m_exitRect;
	QTimeLine	m_timeLine;
	bool		m_exitRegion;

	bool GetSimulatorStatus();
	void PaintCompass(QPainter *painter);
	void PaintDashboard(QPainter *painter);
	void PaintExitButton(QPainter *painter);

private slots:
	void ExitCarSimulator();
};

#endif // CARSIMCTRL_H
