#pragma once

struct GDM_LOCATION
{
	double	m_dLongitude; //경도
	double	m_dLatitude; // 위도
	double	m_dDist; // 중심점으로부터의 거리
};
struct GDM_ORIENTATION
{
	double m_yaw;
	double m_pitch;
	double m_roll;
};

struct GDM_CAMERA_LOCATION
{
	GDM_LOCATION m_pos;
	double	m_dYaw; // Y축 주위 회전각
	double	m_dPitch; // X축 주위 회전각
};

struct GDM_POSITION
{
	double	longitude;	//	경도
	double	latitude;	//	위도
	double	height;		//  지형높이
	double	rate;		//	경사각
};
