#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "../qglobe_libmain/QGlobe_Interface.h"
#include "../qglobe_common/qglobeclientbase.h"

#include "render_global.h"

class RenderSrv;

class CQGlobe_Camera;

class RENDER_EXPORT CameraController
{
public:
    CameraController(RenderSrv	*pRenderSrv);
    ~CameraController();

	bool Init();
	void OnAnimation();
	void CheckRequest();
	void MoveInitPos();
	void FastMoveToPlace(PLACEMARK_DATA* placeData);
	void MoveToPlace(PLACEMARK_DATA* placeData);
	void AutoMoveTo(int pathType , QGlobe_CAMERA_INFO cameraInfo, double moveTime, unsigned char moveType = 1);
	void MoveToBound(CQGlobe_Rect2D bound);

	void OnChangedViewSize(int w, int h);

	void MoveForward(int speed, int nFrom); // 0 -> wheel , 1 -> keyboard , 2 -> joistick
	void MoveView(int x , int y);
	void RotateView(int direct, char blMode = 0); // 0 -> HitPoint Mode, 1 ->org Mode
	void TiltView(int direct, char blMode = 0);// 0 -> HitPoint Mode, 1 ->org Mode
	void LookView(int x , int y);

	bool HitPoint(int x , int y , CQGlobe_Point3DF* pPt, bool isDem = true);

	void ResetTilt();
	void ResetAll();
	void StopView();
	void ResetView();
	void EndAnimation();

	void SetCursorPt(CursorState* mouseEvent);
	void GetCursorPos(MOUSE_POSITION &loc) {loc = m_sCursorPt;}

	void ChangedCompassAngle(double* angle);
	void GetCompassAngle(double* angle);
	void GetStatusInfo(StatusBarInfo* pInfo);

	void MousePress(CursorState* mouseEvent);
	void MouseMove(CursorState* mouseEvent);
	void MouseRelease(CursorState* mouseEvent);
	void MouseDblClick(CursorState* mouseEvent);

	int  GetMode(){return m_bMode;}; // 0 -> None , 1 -> Left Button , 2 -> Mid Button
	void SetTopViewState(double longi,double lat);
private:
	void Release();
	void AutoRotateView(int x1 , int y1 , int x2, int y2, double time);
	void FreeRotateView(int x1 , int y1 , int x2, int y2);
	void FreeRotatePitchRoll(int x , int y , int x1, int y1);
	void MoveToSnapshot(PLACEMARK_DATA* placeData);
	bool GetCursorPt(CQGlobe_Point3DF &pt);
	void ResetSelectRange();
	void SetCursorPt(int x, int y);
	bool GetRotateInfo(int x1 , int y1 , int x2, int y2 , double& fOffsetAngle , CQGlobe_Vector3DF *pRotAxis);
	void StartAnimation(QGlobe_ANIMATION_TYPE aniType);
	double GetMoveToHeight(PLACEMARK_DATA* placeData);
	void UpdateTiltRotAxis();
	bool IsSamePlace(QGlobe_CAMERA_INFO info1 , QGlobe_CAMERA_INFO info2);
	bool IsValidCoord(QGlobe_Coord3D coord , double offsetPitch , char bMode);
	void OffsetOrientation(double offsetPitchAngle, double offsetRollAngle, double offsetYawAngle, char blMode);
private:
	bool			m_bFromTilt;
	bool			m_blFirstMoveTo;

	int             m_nWidth, m_nHeight;
	int             m_x, m_y;
	int             m_curX, m_curY;
	int             m_prevX,m_prevY;
	int             m_bMode; // 0 -> None , 1 -> 

	double	        m_dTickCount;
	double	        m_dAutoRotateTickCount;
	double          m_rad;

	CQGlobe_Vector3DF	m_tiltRotOrg;
	CQGlobe_Point3DF   m_pt;

	QGlobe_CAMERA_INFO m_initCameraInfo;
	QGlobe_ANIMATION_TYPE	m_aniType;

	RenderSrv	   *m_pRenderSrv;
	MOUSE_POSITION	m_sCursorPt;
};

#endif // CAMERA_H
