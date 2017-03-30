#include "GDM_AircraftSpecs.h"

CGDM_AircraftSpecs::CGDM_AircraftSpecs( void )
{
	ChooseAircraftType( eFS_AIRCRAFT_TYPE_A4 );
}

CGDM_AircraftSpecs::~CGDM_AircraftSpecs( void )
{

}

void 
CGDM_AircraftSpecs::ChooseAircraftType( FS_Aircraft_Type a_aircraftType )
{
	switch ( a_aircraftType )
	{
	case eFS_AIRCRAFT_TYPE_P51:
		SetAircraftSpecs_P51();
		break;
	case eFS_AIRCRAFT_TYPE_A4:
		SetAircraftSpecs_A4();
		break;
	case eFS_AIRCRAFT_TYPE_MIRAGE2000:
		SetAircraftSpecs_MIRAGE2000();
		break;
	default:
		SetAircraftSpecs_P51();
		break;
	}
}

void CGDM_AircraftSpecs::SetAircraftSpecs_P51()
{
	// Mass:
	m_Mass					= 4175.0f;					

	// Thrust:
	m_MaxThrustMag			= 60000;//22000.0f;			
	m_EngineType			= eEngineType_PROPELLER;

	// Drag:
	m_DragAofALimit			= 0.25f;			
	m_DragAofADegree		= 60.0f;		

	// Lift:
	m_LiftoffSpeed			= 160.0f;			
	m_MaxAofA				= 20.0f;				
	m_RangeOfLiftFactor		= 1.75f;		
	m_RangeOfLiftFactorSpeed = 1.25f;
	m_RangeOfLiftFactorAofA	= 2.0f;

	// Aircraft:
	m_Length				= 9.83f;				
	m_Height				= 4.17f;				
	m_MaxSpeed				= 703.0f;				
	m_StallSpeed			= 160.0f;			
	m_RollRate				= 30.0f;				
	m_PitchRate				= 15.0f;				
	m_YawRate				= 10.0f;				
	m_RotateRate			= 2.0f;
}

void CGDM_AircraftSpecs::SetAircraftSpecs_A4()
{
	// Mass:
	m_Mass					= 7000.0f;

	// Thrust:
	m_MaxThrustMag			= 41000;
	m_EngineType			= eEngineType_JET;

	// Drag:
	m_DragAofALimit			= 0.25f;			
	m_DragAofADegree		= 60.0f;		

	// Lift:
	m_LiftoffSpeed			= 120.0f;			
	m_MaxAofA				= 16.0f;				
	m_RangeOfLiftFactor		= 4.0f;
	m_RangeOfLiftFactorSpeed= 1.5f;
	m_RangeOfLiftFactorAofA	= 3.5f;

	// Aircraft:
	m_Length				= 12.22f;				
	m_Height				= 4.57f;				
	m_MaxSpeed				= 1077.0f;				
	m_StallSpeed			= 120.0f;			
	m_RollRate				= 80.0f;				
	m_PitchRate				= 30.0f;				
	m_YawRate				= 16.0f;				
	m_RotateRate			= 2.0f;
}

void CGDM_AircraftSpecs::SetAircraftSpecs_MIRAGE2000()
{
	// Mass:
	m_Mass					= 13800.0f;					

	// Thrust:
	m_MaxThrustMag			= 190000.0f;			
	m_EngineType			= eEngineType_PROPELLER;

	// Drag:
	m_DragAofALimit			= 0.25f;			
	m_DragAofADegree		= 60.0f;		

	// Lift:
	m_LiftoffSpeed			= 160.0f;			
	m_MaxAofA				= 20.0f;				
	m_RangeOfLiftFactor		= 4.0f;		
	m_RangeOfLiftFactorSpeed = 1.5f;
	m_RangeOfLiftFactorAofA	= 3.5f;

	// Aircraft:
	m_Length				= 14.36f;				
	m_Height				= 5.3f;				
	m_MaxSpeed				= 2600.0f;				
	m_StallSpeed			= 160.0f;			
	m_RollRate				= 80.0f;				
	m_PitchRate				= 30.0f;				
	m_YawRate				= 20.0f;				
	m_RotateRate			= 2.0f;
}

