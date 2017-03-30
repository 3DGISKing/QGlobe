#ifndef __GDM_CONTROL_SURFACE_H__
#define __GDM_CONTROL_SURFACE_H__

#include "GDM_FlightSimTypes.h"

class CGDM_ControlSurface 
{
public:
	CGDM_ControlSurface (void) ;
	~CGDM_ControlSurface (void);

public:
	void			SetAileron( FS_Fraction a_Aileron );
	void			SetAileronFactor( FS_Factor a_factor );
	FS_Fraction		GetAileron() { return m_Aileron; }

	void			SetElevator( FS_Fraction a_Elevator );
	void			SetElevatorFactor( FS_Factor a_factor );
	FS_Fraction		GetElevator() { return m_Elevator; }

	void			SetRudder( FS_Fraction a_Rudder );
	void			SetRudderFactor( FS_Factor a_factor );
	FS_Fraction		GetRudder() { return m_Rudder; }

	void			SetThrust( FS_Fraction a_Thrust );
	void			SetThurstFactor( FS_Factor a_factor );
	FS_Fraction		GetThrust() { return m_Thrust; }

	void			Reset();

private:

private:
	FS_Fraction		m_Aileron;
	FS_Fraction		m_Elevator;
	FS_Fraction		m_Rudder;
	FS_Fraction		m_Thrust;
};
#endif