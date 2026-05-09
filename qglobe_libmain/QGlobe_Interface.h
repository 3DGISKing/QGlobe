/*
* Name: QGlobe_Interface.h
*
* Description: 
*   Interface API definitions about 
*
* Function List:
*
*/

#ifndef _QGlobe_INTERFACE_H_
#define _QGlobe_INTERFACE_H_
#include "../qglobe_base/QGlobe_Define.h"
#include "../qglobe_base/QGlobe_Coord3D.h"

#include "qglobe_libmain_global.h"
#include "../qglobe_common/qglobeclientbase.h"
#include "QGlobe_Tile.h"

//added by RSH 2013.3.16
///*
#include "../qglobe_client/colorXmlParser.h"
//*/
//end addition

class QGLWidget;
class QPainter;
class QImage;

typedef enum
{
	ANI_NONE,
	ANI_MOVETO,
	ANI_ROTATE,
	ANI_MOVE_FORWARD
}QGlobe_ANIMATION_TYPE;

typedef struct  
{
	bool				m_blAuto;		// auto rotation
	CQGlobe_Vector3DF		m_aroundAxis;	// for rotation
	double				m_fOffsetAngle; // rotate step
	double				m_hour;			// hour to destination (mm)
	double				m_curhour;		// hour to current place (mm)
}QGlobe_ROT_ANI_INFO;

typedef struct  
{
	QGlobe_LOCATION m_loc;					// location
	double		m_orgDist;				// org dist of m_loc.dist
	double		m_roll;					// roll
	double		m_pitch;				// pitch
	double		m_yaw;					// yaw (always 0)
}QGlobe_CAMERA_INFO;


typedef struct  
{
	int					m_pathType;			// 0 -> by name search , 1 -> by mouse dblclick
	int					m_moveType;			// 0 -> uniform , 1 -> accel & deccel , 2 -> 
	QGlobe_CAMERA_INFO		m_dstLocation;
	double				m_hour;			// hour to destination (mm)
	double				m_curhour;		// hour to current place (mm)
}QGlobe_MOVETO_ANI_INFO;

typedef struct  
{
	double				m_fOffsetLen;	// offset step
	double				m_hour;			// hour to destination (mm)
	double				m_curhour;		// hour to current place (mm)
	int					m_moveType;			// 0 -> uniform , 1 -> accel & deccel , 2 -> 
}QGlobe_MOVE_FORWARD_ANI_INFO;

typedef struct 
{
	double	m_dNear;
	double  m_dFar;
	double  m_dFovy;
	double  m_dAspect;
}QGlobe_ViewFrustum;


typedef struct _QGlobeOption
{
	// navigation
	float		        nav_moveto_rate;
	float		        nav_wheel_rate;
	int			        nav_wheel_direction;

	//cache
	unsigned int	    cache_mem_size;
	unsigned int	    cache_file_size;

	//view
	double		        dem_detail_rate;			 // 0.5-3.0
	double		        dem_quality_rate;			 // 0-1

	LAYER_TEXTURE_MODE	layerTextureMode;			// show layer texture state	

	int			        dist_ViewMode;				// mile:0,  kilo:1
	bool		        showLenArea;

	// contour setting
	ContourStateInfo    contourOption;				// contour option	

	// grid
	GridViewInfo 		gridViewInfo;

	//added by RSH 2013.3.14
	///*
	ShapeAttribute      m_geoShapeAttr[GEO_SHAPE_COUNT];
	//*/
	//end addition	

	//determine whether or not consider terrain
	bool                blTerrain;
	bool                blContour;
}QGlobe_OPTION;


#define QGlobe_RENDER_HANDLE void *

//	declare export function
// initialize dll

QGLOBE_LIBMAIN_EXPORT void				qglobe_GetFSP(QGlobe_RENDER_HANDLE handle , int& fsp, int &real_fsp, int &meshs, int &lines);

QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraInit(QGlobe_RENDER_HANDLE handle , int nWidth , int nHeight);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraGetCoord(QGlobe_RENDER_HANDLE handle , QGlobe_Coord3D *pCoord);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraSetCoord(QGlobe_RENDER_HANDLE handle , QGlobe_Coord3D coord);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraGetLocation(QGlobe_RENDER_HANDLE handle , QGlobe_LOCATION *pLoc);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraGetInfo(QGlobe_RENDER_HANDLE handle , QGlobe_CAMERA_INFO *pCameraInfo, char bMode = 0);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraSetInfo(QGlobe_RENDER_HANDLE handle , QGlobe_CAMERA_INFO *pCameraInfo, char bMode = 0);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraGetHitVector(QGlobe_RENDER_HANDLE handle , int x , int y , CQGlobe_Vector3DF* pVt);

QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraOnAnimation(QGlobe_RENDER_HANDLE handle , double deltaTime);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraStartAutoMoveForward(QGlobe_RENDER_HANDLE handle , QGlobe_MOVE_FORWARD_ANI_INFO* pInfo);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraStartAutoRotate(QGlobe_RENDER_HANDLE handle , QGlobe_ROT_ANI_INFO* pInfo);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraStartAutoMoveTo(QGlobe_RENDER_HANDLE handle , QGlobe_MOVETO_ANI_INFO* pInfo);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraStopAuto(QGlobe_RENDER_HANDLE handle);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraSetViewPortSize(QGlobe_RENDER_HANDLE handle , int w , int h);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraRotate(QGlobe_RENDER_HANDLE handle , double fOffsetAngle , CQGlobe_Vector3DF *pAxis);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CameraGetFrustumInfo(QGlobe_RENDER_HANDLE handle , QGlobe_ViewFrustum& frustumInfo);

QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_ScreenToCamera(QGlobe_RENDER_HANDLE handle , double* pScalar);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_ScreenToCamera(QGlobe_RENDER_HANDLE handle , CQGlobe_Vector3DF *pVector);

// get hit point
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_GetHitLocation(int x , int y , CQGlobe_Location3D* pPt);
// end

QGLOBE_LIBMAIN_EXPORT short				qglobe_getHeight(double dLongitude, double dLatitude);
QGLOBE_LIBMAIN_EXPORT short				qglobe_getMaxHeight(double dLongitude, double dLatitude);
QGLOBE_LIBMAIN_EXPORT double				qglobe_getExactHeight(double dLongitude, double dLatitude);
QGLOBE_LIBMAIN_EXPORT double              qglobe_getExactHeightInTile(double dLongitude, double dLatitude, QGlobe_DEM_TILE* pTile);
QGLOBE_LIBMAIN_EXPORT QGlobe_DIRECTION_INFO	qglobe_getRateInfo(double dLongitude, double dLatitude);

QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_SceneGetImage(QGlobe_RENDER_HANDLE handle , QImage* image);

QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CacheMemClear(QGlobe_RENDER_HANDLE handle);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CacheFileClear(QGlobe_RENDER_HANDLE handle);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_SimpleBuildingCacheClear(QGlobe_RENDER_HANDLE handle);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_CacheChangeSize(QGlobe_RENDER_HANDLE handle, int nMemCacheSize, int nFileCacheSize);
QGLOBE_LIBMAIN_EXPORT void				qglobe_GetRenderTextLableSize(QGlobe_RENDER_HANDLE handle, int nSize);

QGLOBE_LIBMAIN_EXPORT void				qglobe_GetRenderOption(QGlobe_RENDER_HANDLE handle , QGlobe_OPTION* option);
QGLOBE_LIBMAIN_EXPORT	void				qglobe_UpdateTerrainLayerState(bool state);
QGLOBE_LIBMAIN_EXPORT	void				qglobe_UpdateContourState(bool state);
QGLOBE_LIBMAIN_EXPORT	void				qglobe_UpdateLayerTextureState(unsigned int state);
QGLOBE_LIBMAIN_EXPORT	void				qglobe_SetContourOptin(ContourStateInfo* contourOptin);
QGLOBE_LIBMAIN_EXPORT	void				qglobe_SetGridOtion(int gridView, int gridMode);

QGLOBE_LIBMAIN_EXPORT void				qglobe_GetPlacemarkViewerData(QGlobe_RENDER_HANDLE handle, PLACEMARK_DATA* pData);

QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_GetLength(QGlobe_RENDER_HANDLE handle , QGlobe_LOCATION *pStart, QGlobe_LOCATION *pEnd , double* length);

QGLOBE_LIBMAIN_EXPORT	QGlobe_ERROR_NO		qglobe_HitPoint(CQGlobe_Point3DF orgPt , CQGlobe_Vector3DF vt , CQGlobe_Point3DF* pPt, bool isDem = true);
QGLOBE_LIBMAIN_EXPORT	QGlobe_ERROR_NO		qglobe_HitPoint(CQGlobe_Point3DF orgPt , CQGlobe_Vector3DF vt , double rad , CQGlobe_Point3DF* pPt);
QGLOBE_LIBMAIN_EXPORT	void				qglobe_SetMoveToSpeed(QGlobe_RENDER_HANDLE handle , double speed);
QGLOBE_LIBMAIN_EXPORT	void				qglobe_SetTopographyQuality(float quality);
QGLOBE_LIBMAIN_EXPORT void				qglobe_SetDistShowMode(double distviewmode);
QGLOBE_LIBMAIN_EXPORT	void				qglobe_SetHeightMagnify(double rate);

QGLOBE_LIBMAIN_EXPORT void				qglobe_GetCoordInfo(QGlobe_Coord3D* pCoord , QGlobe_CAMERA_INFO *pCameraInfo , char bMode = 0);

QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO        qglobe_LocationToScreen(double longitude, double latitude, double height, CQGlobe_Point3DF* pOutPt);

QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO        qglobe_IsModified(OUT bool& blModified);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_intersectRayDEM(CQGlobe_Point3DF& rayOrg , CQGlobe_Vector3DF& rayDir , CQGlobe_Point3DF* pOutPt);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_intersectRayDEM2(CQGlobe_Point3DF& rayOrg , CQGlobe_Vector3DF& rayDir , CQGlobe_Point3DF* pOutPt);
QGLOBE_LIBMAIN_EXPORT	double				qglobe_IntersectHeight(double dLongitude, double dLatitude);


QGLOBE_LIBMAIN_EXPORT void				qglobe_GetStandardCoord(QGlobe_LOCATION *pLoc , QGlobe_Coord3D *pCoord);
QGLOBE_LIBMAIN_EXPORT void				qglobe_GetCoord(QGlobe_CAMERA_INFO *pCameraInfo , QGlobe_Coord3D *pCoord, char bMode = 0);
QGLOBE_LIBMAIN_EXPORT void				qglobe_GetOptimzeCameraCoord(IN QGlobe_LOCATION ltPos, IN QGlobe_LOCATION rbPos, OUT QGlobe_Coord3D *pCoord);

QGLOBE_LIBMAIN_EXPORT bool				qglobe_IsBackFace(double x , double y, double z);
QGLOBE_LIBMAIN_EXPORT bool				qglobe_IsPtInDem(CQGlobe_Point3DF pt);
QGLOBE_LIBMAIN_EXPORT QGlobe_ERROR_NO		qglobe_WorldToScreen(INOUT CQGlobe_Point3DF* pOutPt);
QGLOBE_LIBMAIN_EXPORT void				qglobe_SetWorkPath(IN QString &path);
QGLOBE_LIBMAIN_EXPORT QString				qglobe_GetWorkPath();

//kimyi. Global Time Function
QGLOBE_LIBMAIN_EXPORT	unsigned long qglobe_GetTickTimer();
QGLOBE_LIBMAIN_EXPORT void				qglobe_CheckRequest();
#endif //_QGlobe_INTERFACE_H_
