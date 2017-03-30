#include "GDM_ControlSurface.h"


CGDM_ControlSurface::CGDM_ControlSurface(void)
{
	Reset();
}

CGDM_ControlSurface::~CGDM_ControlSurface()
{

}

void CGDM_ControlSurface::Reset()
{
	m_Aileron	= 0.0f;
	m_Elevator	= 0.0f;
	m_Rudder	= 0.0f;
	m_Thrust	= 0.0f;
}

void
CGDM_ControlSurface::SetAileron( FS_Fraction a_Aileron )
{
	m_Aileron = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, a_Aileron );
}

void
CGDM_ControlSurface::SetElevator( FS_Fraction a_Elevator )
{
	m_Elevator = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, a_Elevator );
}

void
CGDM_ControlSurface::SetRudder( FS_Fraction a_Rudder )
{
	m_Rudder = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, a_Rudder );
}

void
CGDM_ControlSurface::SetThrust( FS_Fraction a_Thrust )
{
	m_Thrust= FS_Clamp( (FS_Fraction)0.0f, FS_MAX_CS_FRACTION, a_Thrust );
}

void
CGDM_ControlSurface::SetAileronFactor( FS_Factor a_factor )
{
	m_Aileron *= a_factor;
	m_Aileron = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, m_Aileron );
}

void
CGDM_ControlSurface::SetElevatorFactor( FS_Factor a_factor )
{
	m_Elevator *= a_factor;
	m_Elevator = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, m_Elevator );
}

void
CGDM_ControlSurface::SetRudderFactor( FS_Factor a_factor )
{
	m_Rudder *= a_factor;
	m_Rudder = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, m_Rudder );
}

void
CGDM_ControlSurface::SetThurstFactor( FS_Factor a_factor )
{
	m_Thrust *= a_factor;
	m_Thrust = FS_Clamp( -FS_MAX_CS_FRACTION, FS_MAX_CS_FRACTION, m_Thrust );
}