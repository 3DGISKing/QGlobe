#pragma once
#include "../gdem_base/GDM_Location.h"
#include "../gdem_base/GDM_Vector3D.h"
#include "../gdem_base/GDM_Coord3D.h"
#include "GDM_CameraAnimationHandler.h"

#define GDM_MAX_HEIGHT			8848

class GDEM_LIBMAIN_EXPORT CGDM_Camera
{
	friend class CGDM_PyramidMgr;
	friend class CGDM_CameraMoveToAnimationHandler;
	friend class CGDM_CameraRotAnimationHandler;
	friend class CGDM_CameraMoveForwardAnimationHandler;
public:
	CGDM_Camera(void);
	~CGDM_Camera(void);

	void           Initialize(int nWidth , int nHeight);
	bool           SetCameraCoord(GDM_Coord3D* pCoord , bool blCheckCollision = true);

	GDM_ERROR_NO   SetViewPortSize(int w , int h);
	inline void    GetBodyOffset(CGDM_Vector3DF& pt) {	pt = m_coord.m_org;	}

	// Get & Set coord
	void           GetCameraCoord(GDM_Coord3D* pCoord);

	bool           LookAt(CGDM_Point3DF* pOrg , CGDM_Point3DF* pTargetPos, CGDM_Vector3DF* pUpVt);
	GDM_ERROR_NO   GetHitVector(int x , int y , CGDM_Vector3DF* pVt);

	// Animation functions
	void           GetInfo(GDM_CAMERA_INFO *pCameraInfo, char bMode = 0);
	void           SetInfo(GDM_CAMERA_INFO *pCameraInfo, char bMode = 0);
	void           StartAutoMoveForward(GDM_MOVE_FORWARD_ANI_INFO* pInfo);
	void           StartAutoMoveTo(GDM_MOVETO_ANI_INFO* pInfo);
	void           StartAutoRotate(GDM_ROT_ANI_INFO *pInfo);
	bool           OnAnimation(double deltaHour);
	void           StopAnimation(){EndAnimation();};

	// control function
	void           RotateAroundVector(double angle , CGDM_Vector3DF* pVt);
	void		   UpdateCameraPos();
	GDM_ERROR_NO   GetHitLocation(int x , int y , CGDM_Location3D* pPt);
	GDM_ERROR_NO   GetHitLocation(int x , int y , double radius, CGDM_Location3D* pPt);
	MOUSE_POSITION GetMousePosition(){ return m_sMousePos;}		// mouse position cache

	// coordinate function
	inline void    GetLocation(GDM_LOCATION *pLoc){*pLoc = m_location;}
	GDM_ERROR_NO   ScreenToCamera(double* pScalar);
	GDM_ERROR_NO   ScreenToCamera(CGDM_Vector3DF *pVector);
	GDM_ERROR_NO   CameraToScreen(INOUT CGDM_Vector3DF *pPt);
	GDM_ERROR_NO   WorldToScreen(INOUT CGDM_Vector3DF* pPt);
	GDM_ERROR_NO   WorldToScreen(double Longitude, double Latitude, double height, CGDM_Vector3DF* pPt);
	GDM_ERROR_NO   WorldToScreen(double Longitude, double Latitude, CGDM_Vector3DF* pPt);
	inline void    GetViewFrustum(GDM_ViewFrustum *frustum) { *frustum = m_frustum;}
	double		   GetAreaInScreen(double minLongitude, double maxLongitude, double minLatitude, double maxLatitude);
	void           SetFOV(double fov);
public:
	GDM_Coord3D			m_coord;     // camera coordinate
	GDM_LOCATION		m_location;	 // camera location

	// View Frustum
	GDM_ViewFrustum		m_frustum;
	int                 m_nScreenWidth;
	int                 m_nScreenHeight;
	double              m_fScale;
	double              m_fScaleW;
	double          	m_fScaleH;

	// 0~1 cosine value of angle between view direction(m_coord.z ) and m_coord.org 's positionvector 
	// if m_fScalarOfDir equals to 0 , then viewer looks at towards earth 's center
	// if m_fScalarOfDir equals to 1 , then viewer looks at horizontal line

	double	            m_fScalarOfDir; 

	GDM_ANIMATION_TYPE	                     m_anyType;
	CGDM_CameraRotAnimationHandler           *m_pRotAnimationHandler;
	CGDM_CameraMoveToAnimationHandler        *m_pMoveToAnimationHandler;
	CGDM_CameraMoveForwardAnimationHandler   *m_pMoveForwardAnimationHandler;

	// view ranege
	double	            m_dMinLong1;
	double	            m_dMaxLong1;
	double	            m_dMinLong2;
	double	            m_dMaxLong2;
	double	            m_dMinLati;
	double	            m_dMaxLati;

private:
	void                Update();
	void                SetCameraLocation(GDM_LOCATION *pLoc);
	void                GetViewVector(CGDM_Vector3DF* pVt);
	void                EndAnimation();
	bool			    IsSameCoord(GDM_Coord3D* pCoord1 , GDM_Coord3D* pCoord2);
	void	            CalculateViewRange();
	double              ComputeNearClipDistance();
	double              ComputeFarClipDistance();
	void                UpdateScaleParamerters();

	// for fast select 
	MOUSE_POSITION		m_sMousePos;		// mouse position cache
	bool				m_blNeedCalcuHitPos;
	bool				m_bEnabled;			// HitPoint 
};

