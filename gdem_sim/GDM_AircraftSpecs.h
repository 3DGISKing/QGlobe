#ifndef __GDM_AIRCRFAT_SPECS_H__
#define __GDM_AIRCRFAT_SPECS_H__

#include "GDM_FlightSimTypes.h"

enum FS_Aircraft_Type 
{
	eFS_AIRCRAFT_TYPE_P51,
	eFS_AIRCRAFT_TYPE_A4,
	eFS_AIRCRAFT_TYPE_MIRAGE2000,
};

enum FS_EngineType { eEngineType_PROPELLER, eEngineType_JET, eEngineType_ROCKET };

class CGDM_AircraftSpecs 
{
public:
	CGDM_AircraftSpecs (void) ;
	~CGDM_AircraftSpecs (void);
	
public:
	// Mass:
	FS_Kilogram		m_Mass;					// kg (between empty and max weight)

	// Thrust:
	FS_NewtonMag	m_MaxThrustMag;			// newton
	FS_EngineType	m_EngineType;			// engine type

	// Drag:
	FS_Factor		m_DragAofALimit;		// limit AoA's contribution to drag (fraction)
	FS_Degree		m_DragAofADegree;		// large divisor so small AofA causes negligible drag

	// Lift:
	FS_SpeedKPH		m_LiftoffSpeed;			// kph
	FS_Degree		m_MaxAofA;				// angle-of-attack (degrees)
	FS_Factor		m_RangeOfLiftFactor;	// limits the total lift factor
	FS_Factor		m_RangeOfLiftFactorSpeed;// lift factor from speed
	FS_Factor		m_RangeOfLiftFactorAofA;// lift factor from angle-of-attack

	// Aircraft:
	FS_Meter		m_Length;				// meter
	FS_Meter		m_Height;				// meter
	FS_SpeedKPH		m_MaxSpeed;				// kph
	FS_SpeedKPH		m_StallSpeed;			// kph
	FS_Degree		m_RollRate;				// deg/sec
	FS_Degree		m_PitchRate;			// deg/sec
	FS_Degree		m_YawRate;				// deg/sec
	FS_Degree		m_RotateRate;			// deg/sec

public:
	void			ChooseAircraftType ( FS_Aircraft_Type a_aircraftType );

private:
	void			SetAircraftSpecs_P51();
	void			SetAircraftSpecs_A4();
	void			SetAircraftSpecs_MIRAGE2000();

private:

};
#endif