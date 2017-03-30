#ifndef PROXY_H
#define PROXY_H

#include "../gdem_common/gdem_protocol.h"

#define PACKET_MAX_BUFFER_SIZE		(1024 * 300)

typedef unsigned long PrxParam;
typedef unsigned long PrxRes;

#include "../gdem_render/rendersrv.h"
#include "../gdem_render/cameracontroller.h"

enum PROXY_MESSAGE
{
/*
 * Proxy Message
 */
	PM_FAIL,
	PM_TRUE,

	PM_INIT,
	PM_RELEASE,
	PM_SET_DATASERVER,	//1) param1:addr, param2:port
						//2) param1:g_dataSrv
/*
 * DataSrv Message
 */
	PM_GET_DATASERVER,	//return:g_dataSrv
	PM_GET_SEARCHRESULT,	//return search list

/*
 * RenderSrv Message
 */
	PM_SET_RENDERVIEW,
	PM_PAINT,				//param1:painter
	PM_CHANGED_VIEWSIZE,	//param1:width, param2:hight
	PM_RESET_TILT,			//
	PM_RESET_ALL,			//
	PM_STOP_VIEW,			//
	PM_CHANGED_COMPASSANGLE,//param1:double *radian
	PM_GET_COMPASSANGLE,	//param1:double *radian

	PM_ON_CHANGE_SERVER,	//no param

	//Image
	PM_GET_IMAGE,			//param1:QImage *

	//Layer State
	PM_TERRAIN_LAYER_STATE,
	PM_UPDATE_CONTOUR_STATE,
	PM_LAYER_TEXTURE_STATE,	//param1:LAYER_STATE *

	//Grid Show/Hide
	PM_GRID_STATE,			//param1:bool

	//ContourState
	PM_CONTOUR_STATE,		//param1:ContourStateInfo *

	//StatusBar
	PM_GET_STATUS_BAR,		//param1:StatusBarInfo *sbi

	//side bar
	PM_SEARCH_PLACE,		//param1: PLACEMARK_DATA*, param2:count

	//PlaceMark
	PM_NEW_PLACEMARK,			//param1: PLACEMARK_DATA*
	PM_MOVE_PLACEMARK,			//param1: PLACEMARK_DATA*
	PM_MOVE_PLACEMARKVIEW,		//param1: PLACEMARK_DATA*
	PM_SNAPSHOT_PLACEDATA,		//param1: PLACEMARK_DATA*

	//OptionDlg
	PM_SHOW_LENGTH_AREA,		//param1: bool (1: show, 0: hide)
	PM_CHANGE_DISTMODE,			//param1: bool (1: feet/mile, 0: meter/kilo)
	PM_CHANGE_LABLESIZE,		//param1: unsigned int lablesize
	PM_TOPOGRAPHY_QUALITY,		//param1: unsigned int (1 ~ 100)
	PM_HEIGHT_MAGNIFY,			//param1: double (0.5 ~ 3.0)
	PM_INIT_FONT,				//param1: FontInfo*
	PM_INIT_FONT_SIZE,			//param1: FontInfo*
	PM_CHANGE_FONT,				//param1: FontInfo*
	PM_CAMERA_MOVESPEED,		//param1: double *speed(range:0.05 ~ 5.00 step:0.05)

	//Cache Manager
	PM_CHANGE_CACHESIZE,		//param1: unsigned int memCacheSize, param2: unsigned int diskCacheSize
	PM_CLEAR_MEMORYCACHE,		//
	PM_CLEAR_DISKCACHE,			//
	PM_CLEAR_SIMPLEBUILDINGCACHE,
	PM_DATA_THREAD_INIT,
	PM_CLEAR_RASTER_CACHE,

	//Graphics
	PM_SUNLIGHT_ENABLED,     //param1: bool
	PM_SUN_POSITION,         //param1: double[3]
	PM_SUNLIGHT_COLOR,       //param1: QColor
	PM_AMBIENT_COLOR,        //param1: QColor
	PM_LIGHT_ATTENUATION,     //param1: double 

	PM_CAMERA_FOV            //param1: double 

};

class DataProxy 
{
public:
	DataProxy();
	virtual ~DataProxy();
	PrxRes SendMessage(int msg, PrxParam param1 = 0, PrxParam param2 = 0);
};

class RenderProxy 
{
public:
	RenderProxy();
	virtual ~RenderProxy();
	PrxRes SendMessage(int msg, PrxParam param1 = 0, PrxParam param2 = 0);
	RenderSrv *GetRenderSrv(){return m_renderSrv;}
private:
	RenderSrv *m_renderSrv;
};

#endif // PROXY_H
