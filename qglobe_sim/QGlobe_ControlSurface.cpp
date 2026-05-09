#include "QGlobe_ControlSurface.h"


CQGlobe_ControlSurface::CQGlobe_ControlSurface(void)
{
	Reset();
}

CQGlobe_ControlSurface::~CQGlobe_ControlSurface()
{

}

void CQGlobe_ControlSurface::Reset()
{
	m_Aileron	= 0.0f;
	m_Elevator	= 0.0f;
	m_Rudder	= 0.0f;
	m_Thrust	= 0.0f;
}

void
CQGlobe_ControlSurface::SetAileron( FS_Fraction a_Aileron )
{
	m_Aileron = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, a_Aileron );
}

void
CQGlobe_ControlSurface::SetElevator( FS_Fraction a_Elevator )
{
	m_Elevator = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, a_Elevator );
}

void
CQGlobe_ControlSurface::SetRudder( FS_Fraction a_Rudder )
{
	m_Rudder = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, a_Rudder );
}

void
CQGlobe_ControlSurface::SetThrust( FS_Fraction a_Thrust )
{
	m_Thrust= FS_Clamp( (FS_Fraction)0.0f, FS_MAX_CS_FRACTION, a_Thrust );
}

void
CQGlobe_ControlSurface::SetAileronFactor( FS_Factor a_factor )
{
	m_Aileron *= a_factor;
	m_Aileron = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, m_Aileron );
}

void
CQGlobe_ControlSurface::SetElevatorFactor( FS_Factor a_factor )
{
	m_Elevator *= a_factor;
	m_Elevator = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, m_Elevator );
}

void
CQGlobe_ControlSurface::SetRudderFactor( FS_Factor a_factor )
{
	m_Rudder *= a_factor;
	m_Rudder = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, m_Rudder );
}

void
CQGlobe_ControlSurface::SetThurstFactor( FS_Factor a_factor )
{
	m_Thrust *= a_factor;
	m_Thrust = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, m_Thrust );
}