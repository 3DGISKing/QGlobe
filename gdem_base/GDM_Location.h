#pragma once

struct GDM_LOCATION
{
	double	m_dLongitude; //�浵
	double	m_dLatitude; // ����
	double	m_dDist; // �߽������κ����� �Ÿ�
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
	double	m_dYaw; // Y�� ���� ȸ����
	double	m_dPitch; // X�� ���� ȸ����
};

struct GDM_POSITION
{
	double	longitude;	//	�浵
	double	latitude;	//	����
	double	height;		//  ��������
	double	rate;		//	��簢
};
