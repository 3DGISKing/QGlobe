#ifndef CONFIG_H
#define CONFIG_H

#include "gdemclientbase.h"

#define CONF_SET_ORG        "IEEI"
#define CONF_SET_APP        "geobrowser"
#define CONF_SET_SRV_APP    "gdemserver"

#define CONF_SRVADDR_LEN    256

#define OPTION_VIEW_TAB				0
#define OPTION_CACHE_TAB			1
#define OPTION_NAVIGATION_TAB		2

#define OPTION_LABLE_MEDIUM			0
#define OPTION_LABLE_SMALL			1
#define OPTION_LABLE_LARGE			2

#define OPTION_LL_DMS				0
#define OPTION_LL_10NOTATION		1
#define OPTION_LL_10NOTATION_DM		2

#define OPTION_DISTANCE_METER_KILO	0
#define OPTION_DISTANCE_FEET_MILE	1

#define OPTION_TOUR_TILT_MIN		0.0
#define OPTION_TOUR_TILT_MAX		80.0
#define OPTION_TOUR_TILT_DEFAULT	45.0

#define OPTION_TOUR_RANGE_MIN		150.0
#define OPTION_TOUR_RANGE_MAX		5000.0
#define OPTION_TOUR_RANGE_DEFAULT	1000.0

#define OPTION_TOUR_SPEED_MIN		50.0
#define OPTION_TOUR_SPEED_MAX		1000.0
#define OPTION_TOUR_SPEED_DEFAULT	150.0

#include "gdem_config.h"

class Config
{
public:
    Config();
    ~Config();

    bool IsServer();
    void SetServer(char *addr, unsigned int port);

    void ReadSettings();
    void WriteSettings();


	void WriteLicence(QString licence);

public:
    //windows's position
    QPoint  m_wndPos;
    QSize   m_wndSize;
	int		m_sideBarWidth;

    //server info
	char		m_srvAddr[CONF_SRVADDR_LEN];
	unsigned int	m_srvPort;

    //show/hide toolbar, sidebar, statusbar
    bool m_showStandardToolbar;
	bool m_showSimulationToolbar;
	bool m_showAnimationToolbar;
    bool m_showSidebar;
    bool m_showStatusbar;

	//View Size
	bool m_fullScreen;

	//show/hide grid, all map, scale legend
    bool m_showGrid;

	bool            m_blTerrain;
	bool            m_blContour;
	unsigned int	m_layerTopography;

	//ContourDlg
	double			m_contourInterval;
	QRgb			m_ravineColor;
	QRgb			m_mainCurveColor;
	unsigned int	m_ravineThick;
	unsigned int	m_mainCurveThick;

	//show/hide ruler
	bool m_showRuler;
	bool m_showMeasureArea;
	bool m_showObjectAdd;

	//show/hide simulator
	bool m_showFlight;
	bool m_showCar;
	bool m_showTourPlay;

	//navigation
	int m_showNavMode;

	//show/hide placemark, placefolder dlg
	bool m_showPlaceMark;
	bool m_showPlaceFolder;
	bool m_showPath;
	bool m_showPolygon;

	//option dialog value
	unsigned int	m_optionTabID;
	bool			m_toolTipChecked;
	bool			m_gisInfoChecked;
	unsigned int	m_lableSize;
	unsigned int	m_llShowMode;
	unsigned int	m_distShowMode;
	QFont			m_mainFont;
	unsigned int	m_topoQuality;
	double			m_heightMagnify;
	unsigned int	m_memCacheSize;
	unsigned int	m_diskCacheSize;
	double			m_tourTiltAngle;
	double			m_tourRange;
	double			m_tourSpeed;
	double			m_moveSpeed;
	unsigned int	m_wheelSpeed;
	bool			m_wheelDirection;

	// Last Save & Open Directory
	QString	m_saveLastDir;
	QString	m_openLastDir;

	//Sidebar info
	unsigned int	m_statusSearch;
	unsigned int	m_statusPlace;
	unsigned int	m_statusLayers;
	unsigned int	m_heightSearch;
	unsigned int	m_heightPlace;
	unsigned int	m_heightLayers;
	unsigned int	m_posSearch;
	unsigned int	m_posPlace;
	unsigned int	m_posLayers;

    QString         m_strDataBasePath;

	//2013 2 27
	//ËË ±¨¸Ë´ÉË¼ ¶Á¼èº÷Àâ¶®Ëæ ¸ó±ã´ó¼è Ê±²÷³Þ.
#ifdef DEBUG
	bool            m_isShowTerrainWireframe;  //¼èÂô·²Ë¾ WireframeËºµá ·Æº¡Â×²÷°¡¶¦ °Î¼³Â×²÷ ±¨¸Ë¸êºã	F1
	bool            m_isShowRenderTileName;	   //¶Ã³ó¶µ´ó²÷ À°ËÎÌ© ËË¶§Ë¾ Âï»¤Â×²÷°¡¶¦ °Î¼³Â×²÷ ±¨¸Ë¸êºã F2 
	bool            m_isShowRenderTileBoundry; //¶Ã³ó¶µ´ó²÷ À°ËÎÌ© °Ò±Ë¶¦ Âï»¤Â×²÷°¡¶¦ °Î¼³Â×²÷ ±¨¸Ë¸êºã F3
	bool            m_isShowShapeNodes;        //¶Ã³ó¶µ´ó²÷ ¹ÖÀË¶£´ÉÌ© ·ô¼°Ë¾ Âï»¤Â×±Ê²÷°¡¶¦ °Î¼³Â×²÷ ±¨¸Ë¸êºã F4
	bool            m_isShowTerrainInner;      //¼èÂô·²³­¹¢¶¦ ¸ó±Ê²÷°¡¶¦ °Î¼³Â×²÷ ±¨¸Ë¸êºã	F5
	bool            m_isRendering;           //¶Ã³ó¶µÂ×±Ê²÷°¡¶¦ °Î¼³Â×²÷ ±¨¸Ë¸êºã	F6
#endif


	QString         m_recentOpendPathFor3DObjectAdding;//Êï¹¶½¤ÀâºªËÓË¾ Ì¡ÃÍº· °¡»þ ¾×°ýËæ ÊâÊÝ³õ ´Ðµâ¹¢
	QString         m_recentOpendPathFor3DObjectForAvi;//Êï¹¶½¤ÀâºªËÓ»¤ ´´Ãùº¬Ë¾ ´ÝËÄ»¤¿Ô±¨ Ì¡ÃÍ °¡»þ ¾×°ýËæ ÊâÊÝ³õ ´Ðµâ¹¢
	int             m_recentSelectedProvinceFor3DObjectAdding;//Êï¹¶½¤ÀâºªËÓ»¤ °¡»þ ¾×°ýËæ ººÀõÂ×Êî³õ ´ª
	int             m_recentSelectedCountyFor3DObjectAdding;//Êï¹¶½¤ÀâºªËÓ»¤ °¡»þ ¾×°ýËæ ººÀõÂ×Êî³õ °ë
	int             m_recentSelectedVilliageFor3DObjectAdding;//Êï¹¶½¤ÀâºªËÓ»¤ °¡»þ ¾×°ýËæ ººÀõÂ×Êî³õ ¶®

	//by ugi 2013.8.10 
	bool          m_IsLicence;

	//by ugi 2013 10.6
	bool          m_IsShowBuildingName;
	bool          m_IsShowOverviewMap;
	bool          m_IsSelectObject;

	bool          m_SunLightEnabled;

	double        m_SunLightLongitude;  //in degree
	double        m_SunLightLatitude;   //in degree
	double        m_SunLightHeight;     //in kilometer  
	QColor        m_SunLightColor;
	QColor        m_AmbientLightColor;
	double        m_LightAttenuation;

	double        m_CameraFOV;	        // in degree
};

#endif // CONFIG_H
