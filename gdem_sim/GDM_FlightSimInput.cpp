#include "GDM_FlightSim.h"
#include "stable.h"

#define FS_KEY_SHIFT		KS_SHIFT
#define FS_KEY_CONTROL		KS_CTRL
#define FS_KEY_LEFT			Qt::Key_Left
#define FS_KEY_UP			Qt::Key_Up
#define FS_KEY_RIGHT		Qt::Key_Right
#define FS_KEY_DOWN			Qt::Key_Down
#define FS_KEY_SPACE		Qt::Key_Space
#define	FS_KEY_F1			Qt::Key_F1
#define	FS_KEY_ESCAPE		Qt::Key_Escape
#define FS_KEY_PAUSED		FS_KEY_SPACE
#define FS_KEY_HELP			FS_KEY_F1
#define	FS_KEY_A			Qt::Key_A
#define	FS_KEY_S			Qt::Key_S
#define	FS_KEY_W			Qt::Key_W
#define	FS_KEY_D			Qt::Key_D
#define	FS_KEY_a			0x61
#define	FS_KEY_s			0x73
#define	FS_KEY_w			0x77
#define	FS_KEY_d			0x66

#define FS_MOUSE_MARGIN		10
#define FS_FRACTION_STEP	0.05f
#define FS_FRACTION_MOUSE(range,val)	(2.0f * (val) / (range) - 1.0f)
bool	
CGDM_FlightSim::OnMousePress(CursorState* mouseEvent)
{
	if ( m_Paused || m_Ended )
		return true;

	m_UseMouse = !m_UseMouse;

	return true;
}

bool	
CGDM_FlightSim::OnMouseMove(CursorState* mouseEvent)
{
	if ( !m_UseMouse || m_Paused || m_Ended )
		return true;

	int w = m_screenWidth + FS_MOUSE_MARGIN * 2;
	int h = m_screenHeight + FS_MOUSE_MARGIN * 2;

#if 0
	if ( mouseEvent->keys == FS_KEY_CONTROL)
	{
		FS_Fraction thrust = -FS_FRACTION_MOUSE(h, mouseEvent->y);
		thrust /= 2.0f;
		thrust += 0.5f;
		m_ControlSurface.SetThrust( thrust );
	}
	else if ( mouseEvent->keys == FS_KEY_SHIFT)
	{
		FS_Fraction rudder = FS_FRACTION_MOUSE(w, mouseEvent->x);
		m_ControlSurface.SetRudder( rudder );
	}
	else
#endif
	{
		FS_Fraction aileron =   FS_FRACTION_MOUSE(w, mouseEvent->x);
		FS_Fraction elevator = -FS_FRACTION_MOUSE(h, mouseEvent->y);
		m_ControlSurface.SetAileron(  aileron );
		m_ControlSurface.SetElevator( elevator );
		m_LastInputDevice = eFS_INPUT_DEVICE_MOUSE;
	}

	return true;
}

void 
CGDM_FlightSim::HandleKeyEvent( int a_key, 
							   FS_Fraction* a_aileron, FS_Fraction* a_elevator, 
							   FS_Fraction* a_rudder, FS_Fraction* a_thrust )
{
	switch ( a_key )
	{
	case FS_KEY_LEFT:
		{
			*a_aileron -= FS_FRACTION_STEP;
			m_LastInputDevice = eFS_INPUT_DEVICE_KEYBOARD;
		}
		break;
	case FS_KEY_RIGHT:
		{
			*a_aileron += FS_FRACTION_STEP;
			m_LastInputDevice = eFS_INPUT_DEVICE_KEYBOARD;
		}
		break;
	case FS_KEY_UP:
		{
			*a_elevator += FS_FRACTION_STEP;
			m_LastInputDevice = eFS_INPUT_DEVICE_KEYBOARD;
		}
		break;
	case FS_KEY_DOWN:
		{
			*a_elevator -= FS_FRACTION_STEP;
			m_LastInputDevice = eFS_INPUT_DEVICE_KEYBOARD;
		}
		break;
	case FS_KEY_A:
	case FS_KEY_a:
		*a_rudder -= FS_FRACTION_STEP;
		break;
	case FS_KEY_D:
	case FS_KEY_d:
		*a_rudder += FS_FRACTION_STEP;
		break;
	case FS_KEY_W:
	case FS_KEY_w:
		*a_thrust += 0.05f;
		break;
	case FS_KEY_S:
	case FS_KEY_s:
		*a_thrust -= 0.05f;
		break;
	case FS_KEY_PAUSED:
		m_Paused = !m_Paused;
		return;
	case FS_KEY_HELP:
		m_ShowHelp = !m_ShowHelp;
		break;
	case FS_KEY_ESCAPE:
		{
			m_Ended = true;
			m_simStatus.m_ended	= m_Ended;
		}
		break;
	}

}

void	
CGDM_FlightSim::OnKeyPressEvent(KeyState keyState)
{
	if ( m_Ended )
		return ;
	if ( m_Paused && keyState.key1 != FS_KEY_PAUSED && keyState.key1 != FS_KEY_ESCAPE)
		return;

	FS_Fraction aileron, elevator, rudder, thrust;
	aileron		= m_ControlSurface.GetAileron();
	elevator	= m_ControlSurface.GetElevator();
	rudder		= m_ControlSurface.GetRudder();
	thrust		= m_ControlSurface.GetThrust();


	HandleKeyEvent( keyState.key1, &aileron, &elevator, &rudder, &thrust );
	HandleKeyEvent( keyState.key2, &aileron, &elevator, &rudder, &thrust );
	
//	if ( !m_UseMouse )
	{
		m_ControlSurface.SetAileron ( aileron );
		m_ControlSurface.SetElevator( elevator );
	}
	m_ControlSurface.SetRudder  ( rudder );
	m_ControlSurface.SetThrust  ( thrust );
}


