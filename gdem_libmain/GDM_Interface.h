/*
* Name: GDM_Interface.h
*
* Description: 
*   Interface API definitions about 
*
* Function List:
*
*/

#ifndef _GDM_INTERFACE_H_
#define _GDM_INTERFACE_H_
#include "../gdem_base/GDM_Define.h"
#include "../gdem_base/GDM_Coord3D.h"

#include "gdem_libmain_global.h"
#include "../gdem_common/gdemclientbase.h"
#include "GDM_Tile.h"

//added by RSH 2013.3.16
///*
#include "../gdem_client/colorXmlParser.h"
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
}GDM_ANIMATION_TYPE;

typedef struct  
{
	bool				m_blAuto;		// auto rotation
	CGDM_Vector3DF		m_aroundAxis;	// for rotation
	double				m_fOffsetAngle; // rotate step
	double				m_hour;			// hour to destination (mm)
	double				m_curhour;		// hour to current place (mm)
}GDM_ROT_ANI_INFO;

typedef struct  
{
	GDM_LOCATION m_loc;					// location
	double		m_orgDist;				// org dist of m_loc.dist
	double		m_roll;					// roll
	double		m_pitch;				// pitch
	double		m_yaw;					// yaw (always 0)
}GDM_CAMERA_INFO;


typedef struct  
{
	int					m_pathType;			// 0 -> by name search , 1 -> by mouse dblclick
	int					m_moveType;			// 0 -> uniform , 1 -> accel & deccel , 2 -> 
	GDM_CAMERA_INFO		m_dstLocation;
	double				m_hour;			// hour to destination (mm)
	double				m_curhour;		// hour to current place (mm)
}GDM_MOVETO_ANI_INFO;

typedef struct  
{
	double				m_fOffsetLen;	// offset step
	double				m_hour;			// hour to destination (mm)
	double				m_curhour;		// hour to current place (mm)
	int					m_moveType;			// 0 -> uniform , 1 -> accel & deccel , 2 -> 
}GDM_MOVE_FORWARD_ANI_INFO;

typedef struct 
{
	double	m_dNear;
	double  m_dFar;
	double  m_dFovy;
	double  m_dAspect;
}GDM_ViewFrustum;


typedef struct _gdmOption
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
}GDM_OPTION;


#define GDM_RENDER_HANDLE void *

//	declare export function
// initialize dll

GDEM_LIBMAIN_EXPORT void				gdm_GetFSP(GDM_RENDER_HANDLE handle , int& fsp, int &real_fsp, int &meshs, int &lines);

GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraInit(GDM_RENDER_HANDLE handle , int nWidth , int nHeight);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraGetCoord(GDM_RENDER_HANDLE handle , GDM_Coord3D *pCoord);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraSetCoord(GDM_RENDER_HANDLE handle , GDM_Coord3D coord);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraGetLocation(GDM_RENDER_HANDLE handle , GDM_LOCATION *pLoc);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraGetInfo(GDM_RENDER_HANDLE handle , GDM_CAMERA_INFO *pCameraInfo, char bMode = 0);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraSetInfo(GDM_RENDER_HANDLE handle , GDM_CAMERA_INFO *pCameraInfo, char bMode = 0);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraGetHitVector(GDM_RENDER_HANDLE handle , int x , int y , CGDM_Vector3DF* pVt);

GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraOnAnimation(GDM_RENDER_HANDLE handle , double deltaTime);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraStartAutoMoveForward(GDM_RENDER_HANDLE handle , GDM_MOVE_FORWARD_ANI_INFO* pInfo);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraStartAutoRotate(GDM_RENDER_HANDLE handle , GDM_ROT_ANI_INFO* pInfo);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraStartAutoMoveTo(GDM_RENDER_HANDLE handle , GDM_MOVETO_ANI_INFO* pInfo);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraStopAuto(GDM_RENDER_HANDLE handle);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraSetViewPortSize(GDM_RENDER_HANDLE handle , int w , int h);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraRotate(GDM_RENDER_HANDLE handle , double fOffsetAngle , CGDM_Vector3DF *pAxis);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CameraGetFrustumInfo(GDM_RENDER_HANDLE handle , GDM_ViewFrustum& frustumInfo);

GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_ScreenToCamera(GDM_RENDER_HANDLE handle , double* pScalar);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_ScreenToCamera(GDM_RENDER_HANDLE handle , CGDM_Vector3DF *pVector);

// get hit point
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_GetHitLocation(int x , int y , CGDM_Location3D* pPt);
// end

GDEM_LIBMAIN_EXPORT short				gdm_getHeight(double dLongitude, double dLatitude);
GDEM_LIBMAIN_EXPORT short				gdm_getMaxHeight(double dLongitude, double dLatitude);
GDEM_LIBMAIN_EXPORT double				gdm_getExactHeight(double dLongitude, double dLatitude);
GDEM_LIBMAIN_EXPORT double              gdm_getExactHeightInTile(double dLongitude, double dLatitude, GDM_DEM_TILE* pTile);
GDEM_LIBMAIN_EXPORT GDM_DIRECTION_INFO	gdm_getRateInfo(double dLongitude, double dLatitude);

GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_SceneGetImage(GDM_RENDER_HANDLE handle , QImage* image);

GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CacheMemClear(GDM_RENDER_HANDLE handle);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CacheFileClear(GDM_RENDER_HANDLE handle);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_SimpleBuildingCacheClear(GDM_RENDER_HANDLE handle);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_CacheChangeSize(GDM_RENDER_HANDLE handle, int nMemCacheSize, int nFileCacheSize);
GDEM_LIBMAIN_EXPORT void				gdm_GetRenderTextLableSize(GDM_RENDER_HANDLE handle, int nSize);

GDEM_LIBMAIN_EXPORT void				gdm_GetRenderOption(GDM_RENDER_HANDLE handle , GDM_OPTION* option);
GDEM_LIBMAIN_EXPORT	void				gdm_UpdateTerrainLayerState(bool state);
GDEM_LIBMAIN_EXPORT	void				gdm_UpdateContourState(bool state);
GDEM_LIBMAIN_EXPORT	void				gdm_UpdateLayerTextureState(unsigned int state);
GDEM_LIBMAIN_EXPORT	void				gdm_SetContourOptin(ContourStateInfo* contourOptin);
GDEM_LIBMAIN_EXPORT	void				gdm_SetGridOtion(int gridView, int gridMode);

GDEM_LIBMAIN_EXPORT void				gdm_GetPlacemarkViewerData(GDM_RENDER_HANDLE handle, PLACEMARK_DATA* pData);

GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_GetLength(GDM_RENDER_HANDLE handle , GDM_LOCATION *pStart, GDM_LOCATION *pEnd , double* length);

GDEM_LIBMAIN_EXPORT	GDM_ERROR_NO		gdm_HitPoint(CGDM_Point3DF orgPt , CGDM_Vector3DF vt , CGDM_Point3DF* pPt, bool isDem = true);
GDEM_LIBMAIN_EXPORT	GDM_ERROR_NO		gdm_HitPoint(CGDM_Point3DF orgPt , CGDM_Vector3DF vt , double rad , CGDM_Point3DF* pPt);
GDEM_LIBMAIN_EXPORT	void				gdm_SetMoveToSpeed(GDM_RENDER_HANDLE handle , double speed);
GDEM_LIBMAIN_EXPORT	void				gdm_SetTopographyQuality(float quality);
GDEM_LIBMAIN_EXPORT void				gdm_SetDistShowMode(double distviewmode);
GDEM_LIBMAIN_EXPORT	void				gdm_SetHeightMagnify(double rate);

GDEM_LIBMAIN_EXPORT void				gdm_GetCoordInfo(GDM_Coord3D* pCoord , GDM_CAMERA_INFO *pCameraInfo , char bMode = 0);

GDEM_LIBMAIN_EXPORT GDM_ERROR_NO        gdm_LocationToScreen(double longitude, double latitude, double height, CGDM_Point3DF* pOutPt);

GDEM_LIBMAIN_EXPORT GDM_ERROR_NO        gdm_IsModified(OUT bool& blModified);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_intersectRayDEM(CGDM_Point3DF& rayOrg , CGDM_Vector3DF& rayDir , CGDM_Point3DF* pOutPt);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_intersectRayDEM2(CGDM_Point3DF& rayOrg , CGDM_Vector3DF& rayDir , CGDM_Point3DF* pOutPt);
GDEM_LIBMAIN_EXPORT	double				gdm_IntersectHeight(double dLongitude, double dLatitude);


GDEM_LIBMAIN_EXPORT void				gdm_GetStandardCoord(GDM_LOCATION *pLoc , GDM_Coord3D *pCoord);
GDEM_LIBMAIN_EXPORT void				gdm_GetCoord(GDM_CAMERA_INFO *pCameraInfo , GDM_Coord3D *pCoord, char bMode = 0);
GDEM_LIBMAIN_EXPORT void				gdm_GetOptimzeCameraCoord(IN GDM_LOCATION ltPos, IN GDM_LOCATION rbPos, OUT GDM_Coord3D *pCoord);

GDEM_LIBMAIN_EXPORT bool				gdm_IsBackFace(double x , double y, double z);
GDEM_LIBMAIN_EXPORT bool				gdm_IsPtInDem(CGDM_Point3DF pt);
GDEM_LIBMAIN_EXPORT GDM_ERROR_NO		gdm_WorldToScreen(INOUT CGDM_Point3DF* pOutPt);
GDEM_LIBMAIN_EXPORT void				gdm_SetWorkPath(IN QString &path);
GDEM_LIBMAIN_EXPORT QString				gdm_GetWorkPath();

//kimyi. Global Time Function
GDEM_LIBMAIN_EXPORT	unsigned long gdm_GetTickTimer();
GDEM_LIBMAIN_EXPORT void				gdm_CheckRequest();
#endif //_GDM_INTERFACE_H_
