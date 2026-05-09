#pragma once
#include "../qglobe_base/QGlobe_Location.h"
#include "../qglobe_base/QGlobe_Vector3D.h"
#include "../qglobe_base/QGlobe_Coord3D.h"
#include "QGlobe_Interface.h"

#define ANI_MAX_STEP		10

class CQGlobe_Camera;

class CQGlobe_CameraAnimationHandler
{
public:
	CQGlobe_CameraAnimationHandler(CQGlobe_Camera* pCamera);
	virtual ~CQGlobe_CameraAnimationHandler();
	void StartAnimation();
	void EndAnimation();
protected:
	double GetRepRate(double totalTime , double midTimeRate , double curTime, char RepType);
protected:
	CQGlobe_Camera* m_pCamera;
	QGlobe_Coord3D	 m_orgCoord;
	// For Animation
	QGlobe_ANIMATION_TYPE	m_eAniType;
};

class CQGlobe_CameraMoveForwardAnimationHandler : public CQGlobe_CameraAnimationHandler
{
public:
	CQGlobe_CameraMoveForwardAnimationHandler(CQGlobe_Camera* pCamera);
	virtual ~CQGlobe_CameraMoveForwardAnimationHandler();
	void SetAniInfo(QGlobe_MOVE_FORWARD_ANI_INFO* pInfo);
	bool OnAnimation(double deltaHour);
private:
	QGlobe_MOVE_FORWARD_ANI_INFO	m_aniInfo; // for animation
	double						m_totalOffset;
	int							m_nStep;
};


class CQGlobe_CameraRotAnimationHandler : public CQGlobe_CameraAnimationHandler
{
public:
	CQGlobe_CameraRotAnimationHandler(CQGlobe_Camera* pCamera);
	virtual ~CQGlobe_CameraRotAnimationHandler();
	void SetAniInfo(QGlobe_ROT_ANI_INFO* pInfo);
	bool OnAnimation(double deltaHour);
private:
	QGlobe_ROT_ANI_INFO		m_aniInfo; // for rotation
	double						m_totalOffset;
};

class CQGlobe_CameraMoveToAnimationHandler : public CQGlobe_CameraAnimationHandler
{
public:
	CQGlobe_CameraMoveToAnimationHandler(CQGlobe_Camera* pRender);
	virtual ~CQGlobe_CameraMoveToAnimationHandler();
	void SetAniInfo(QGlobe_MOVETO_ANI_INFO* pInfo);
	bool OnAnimation(double deltaHour);
private:
	void LerpCameraInfo(double type , QGlobe_CAMERA_INFO* resultInfo);
private:
	QGlobe_MOVETO_ANI_INFO		m_aniInfo; // for move to
	int						m_nStepCount;
	QGlobe_CAMERA_INFO			m_cameraInfo[ANI_MAX_STEP];
	double					m_hour[ANI_MAX_STEP];

	QGlobe_CAMERA_INFO			m_startInfo;
	QGlobe_CAMERA_INFO			m_endInfo;

	int						m_nCurStep;
};

