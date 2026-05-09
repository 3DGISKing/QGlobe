#pragma once
#include "../qglobe_base/QGlobe_Location.h"
#include "../qglobe_base/QGlobe_Vector3D.h"
#include "../qglobe_base/QGlobe_Coord3D.h"
#include "QGlobe_CameraAnimationHandler.h"

#define QGlobe_MAX_HEIGHT			8848

class QGLOBE_LIBMAIN_EXPORT CQGlobe_Camera
{
	friend class CQGlobe_PyramidMgr;
	friend class CQGlobe_CameraMoveToAnimationHandler;
	friend class CQGlobe_CameraRotAnimationHandler;
	friend class CQGlobe_CameraMoveForwardAnimationHandler;
public:
	CQGlobe_Camera(void);
	~CQGlobe_Camera(void);

	void           Initialize(int nWidth , int nHeight);
	bool           SetCameraCoord(QGlobe_Coord3D* pCoord , bool blCheckCollision = true);

	QGlobe_ERROR_NO   SetViewPortSize(int w , int h);
	inline void    GetBodyOffset(CQGlobe_Vector3DF& pt) {	pt = m_coord.m_org;	}

	// Get & Set coord
	void           GetCameraCoord(QGlobe_Coord3D* pCoord);

	bool           LookAt(CQGlobe_Point3DF* pOrg , CQGlobe_Point3DF* pTargetPos, CQGlobe_Vector3DF* pUpVt);
	QGlobe_ERROR_NO   GetHitVector(int x , int y , CQGlobe_Vector3DF* pVt);

	// Animation functions
	void           GetInfo(QGlobe_CAMERA_INFO *pCameraInfo, char bMode = 0);
	void           SetInfo(QGlobe_CAMERA_INFO *pCameraInfo, char bMode = 0);
	void           StartAutoMoveForward(QGlobe_MOVE_FORWARD_ANI_INFO* pInfo);
	void           StartAutoMoveTo(QGlobe_MOVETO_ANI_INFO* pInfo);
	void           StartAutoRotate(QGlobe_ROT_ANI_INFO *pInfo);
	bool           OnAnimation(double deltaHour);
	void           StopAnimation(){EndAnimation();};

	// control function
	void           RotateAroundVector(double angle , CQGlobe_Vector3DF* pVt);
	void		   UpdateCameraPos();
	QGlobe_ERROR_NO   GetHitLocation(int x , int y , CQGlobe_Location3D* pPt);
	QGlobe_ERROR_NO   GetHitLocation(int x , int y , double radius, CQGlobe_Location3D* pPt);
	MOUSE_POSITION GetMousePosition(){ return m_sMousePos;}		// mouse position cache

	// coordinate function
	inline void    GetLocation(QGlobe_LOCATION *pLoc){*pLoc = m_location;}
	QGlobe_ERROR_NO   ScreenToCamera(double* pScalar);
	QGlobe_ERROR_NO   ScreenToCamera(CQGlobe_Vector3DF *pVector);
	QGlobe_ERROR_NO   CameraToScreen(INOUT CQGlobe_Vector3DF *pPt);
	QGlobe_ERROR_NO   WorldToScreen(INOUT CQGlobe_Vector3DF* pPt);
	QGlobe_ERROR_NO   WorldToScreen(double Longitude, double Latitude, double height, CQGlobe_Vector3DF* pPt);
	QGlobe_ERROR_NO   WorldToScreen(double Longitude, double Latitude, CQGlobe_Vector3DF* pPt);
	inline void    GetViewFrustum(QGlobe_ViewFrustum *frustum) { *frustum = m_frustum;}
	double		   GetAreaInScreen(double minLongitude, double maxLongitude, double minLatitude, double maxLatitude);
	void           SetFOV(double fov);
public:
	QGlobe_Coord3D			m_coord;     // camera coordinate
	QGlobe_LOCATION		m_location;	 // camera location

	// View Frustum
	QGlobe_ViewFrustum		m_frustum;
	int                 m_nScreenWidth;
	int                 m_nScreenHeight;
	double              m_fScale;
	double              m_fScaleW;
	double          	m_fScaleH;

	// 0~1 cosine value of angle between view direction(m_coord.z ) and m_coord.org 's positionvector 
	// if m_fScalarOfDir equals to 0 , then viewer looks at towards earth 's center
	// if m_fScalarOfDir equals to 1 , then viewer looks at horizontal line

	double	            m_fScalarOfDir; 

	QGlobe_ANIMATION_TYPE	                     m_anyType;
	CQGlobe_CameraRotAnimationHandler           *m_pRotAnimationHandler;
	CQGlobe_CameraMoveToAnimationHandler        *m_pMoveToAnimationHandler;
	CQGlobe_CameraMoveForwardAnimationHandler   *m_pMoveForwardAnimationHandler;

	// view ranege
	double	            m_dMinLong1;
	double	            m_dMaxLong1;
	double	            m_dMinLong2;
	double	            m_dMaxLong2;
	double	            m_dMinLati;
	double	            m_dMaxLati;

private:
	void                Update();
	void                SetCameraLocation(QGlobe_LOCATION *pLoc);
	void                GetViewVector(CQGlobe_Vector3DF* pVt);
	void                EndAnimation();
	bool			    IsSameCoord(QGlobe_Coord3D* pCoord1 , QGlobe_Coord3D* pCoord2);
	void	            CalculateViewRange();
	double              ComputeNearClipDistance();
	double              ComputeFarClipDistance();
	void                UpdateScaleParamerters();

	// for fast select 
	MOUSE_POSITION		m_sMousePos;		// mouse position cache
	bool				m_blNeedCalcuHitPos;
	bool				m_bEnabled;			// HitPoint 
};

