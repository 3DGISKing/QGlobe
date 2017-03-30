#include "keyframe.h"
#include <QtCore/QFile>

KeyFrame::KeyFrame()
{
}

CameraKeyFrame::CameraKeyFrame()
:KeyFrame()
{
	m_Type=Camera;
	m_Longitude=0;
	m_Latitude=0;
	m_Altitude=0;
	m_Height=0;
	m_Tilt=0;
	m_Heading=0;
	m_GotoTime=4000;
	m_StopTime=5000;
};

CameraKeyFrame::CameraKeyFrame(const CameraKeyFrame& ckf)
{
	m_Type=ckf.m_Type;
	m_Name=ckf.m_Name;
	m_Longitude=ckf.m_Longitude;
	m_Latitude=ckf.m_Latitude;
	m_Altitude=ckf.m_Altitude;
	m_Height=ckf.m_Height;
	m_Heading=ckf.m_Heading;
	m_Tilt=ckf.m_Tilt;
	m_GotoTime=ckf.m_GotoTime;
	m_StopTime=ckf.m_StopTime;
}

QString CameraKeyFrame::label()
{
	QString label=m_Name;

	label=label+" : ";
	label=label+QString::number(m_GotoTime/1000);
	label+=" : ";
	label=label+QString::number(m_StopTime/1000);
	return label;
}

int CameraKeyFrame::totalTime()
{
	return m_GotoTime+m_StopTime;
}

VideoKeyFrame::VideoKeyFrame()
:KeyFrame()
{
	m_Type=Video;
	m_TotalTime=4000;
}

VideoKeyFrame::VideoKeyFrame(const VideoKeyFrame &vkf)
{
	m_Type=vkf.m_Type;
	m_VideoFileName=vkf.m_VideoFileName;
	m_TotalTime=vkf.m_TotalTime;
}


int VideoKeyFrame::totalTime()
{
	return m_TotalTime;
}

QString VideoKeyFrame::label()
{
	if(m_VideoFileName=="")
		return "Invalid";

	return m_VideoFileName.right(m_VideoFileName.length() - m_VideoFileName.lastIndexOf('/') - 1);
}







