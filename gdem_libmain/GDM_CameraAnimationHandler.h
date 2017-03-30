#pragma once
#include "../gdem_base/GDM_Location.h"
#include "../gdem_base/GDM_Vector3D.h"
#include "../gdem_base/GDM_Coord3D.h"
#include "GDM_Interface.h"

#define ANI_MAX_STEP		10

class CGDM_Camera;

class CGDM_CameraAnimationHandler
{
public:
	CGDM_CameraAnimationHandler(CGDM_Camera* pCamera);
	virtual ~CGDM_CameraAnimationHandler();
	void StartAnimation();
	void EndAnimation();
protected:
	double GetRepRate(double totalTime , double midTimeRate , double curTime, char RepType);
protected:
	CGDM_Camera* m_pCamera;
	GDM_Coord3D	 m_orgCoord;
	// For Animation
	GDM_ANIMATION_TYPE	m_eAniType;
};

class CGDM_CameraMoveForwardAnimationHandler : public CGDM_CameraAnimationHandler
{
public:
	CGDM_CameraMoveForwardAnimationHandler(CGDM_Camera* pCamera);
	virtual ~CGDM_CameraMoveForwardAnimationHandler();
	void SetAniInfo(GDM_MOVE_FORWARD_ANI_INFO* pInfo);
	bool OnAnimation(double deltaHour);
private:
	GDM_MOVE_FORWARD_ANI_INFO	m_aniInfo; // for animation
	double						m_totalOffset;
	int							m_nStep;
};


class CGDM_CameraRotAnimationHandler : public CGDM_CameraAnimationHandler
{
public:
	CGDM_CameraRotAnimationHandler(CGDM_Camera* pCamera);
	virtual ~CGDM_CameraRotAnimationHandler();
	void SetAniInfo(GDM_ROT_ANI_INFO* pInfo);
	bool OnAnimation(double deltaHour);
private:
	GDM_ROT_ANI_INFO		m_aniInfo; // for rotation
	double						m_totalOffset;
};

class CGDM_CameraMoveToAnimationHandler : public CGDM_CameraAnimationHandler
{
public:
	CGDM_CameraMoveToAnimationHandler(CGDM_Camera* pRender);
	virtual ~CGDM_CameraMoveToAnimationHandler();
	void SetAniInfo(GDM_MOVETO_ANI_INFO* pInfo);
	bool OnAnimation(double deltaHour);
private:
	void LerpCameraInfo(double type , GDM_CAMERA_INFO* resultInfo);
private:
	GDM_MOVETO_ANI_INFO		m_aniInfo; // for move to
	int						m_nStepCount;
	GDM_CAMERA_INFO			m_cameraInfo[ANI_MAX_STEP];
	double					m_hour[ANI_MAX_STEP];

	GDM_CAMERA_INFO			m_startInfo;
	GDM_CAMERA_INFO			m_endInfo;

	int						m_nCurStep;
};

