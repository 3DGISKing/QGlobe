#include <QSettings>
#include <QDesktopServices>

#include "gdemclient.h"
#include "mainwindow.h"
#include "config.h"
#include "gdemclientbase.h"
#include "sidebar.h"

#define CONF_SET_MAINWNDPOS			"main_wnd_pos"
#define CONF_SET_MAINWNDSIZE		"main_wnd_size"
#define CONF_SET_SIDEBARWIDTH		"sidebar_width"
#define CONF_SET_SRVADDR			"srv_addr"
#define CONF_SET_SRVPORT			"srv_port"
#define CONF_SET_SHOWSTANDARDTOOLBAR "show_standardtoolbar"
#define CONF_SET_SHOWSIMULATIONTOOLBAR "show_simulationtoolbar"
#define CONF_SET_SHOWANIMATIONTOOLBAR "show_animationtoolbar"
#define CONF_SET_SHOWSIDEBAR		"show_sidebar"
#define CONF_SET_SHOWSTATUSBAR		"show_statusbar"
#define CONF_SET_SHOWGRID			"show_grid"
#define CONF_SET_LAYEROBJECT		"layer_object_state"
#define CONF_SET_LAYERTOPOGRAPHY	"layer_topography_state"
#define CONF_SET_LAYEREXPANDED		"layer_node_expanded"
#define CONF_SET_CONTOURINTERVAL	"contour_interval"
#define CONF_SET_RAVINECOLOR		"contour_ravine_color"
#define CONF_SET_MAINCURVECOLOR		"contour_maincurve_color"
#define CONF_SET_RAVINETHICK		"contour_ravine_thick"
#define CONF_SET_MAINCURVETHICK		"contour_maincurve_thick"
#define CONF_SET_SHOWRULER			"show_ruler"
#define CONF_SET_SHOWFULLSCREEN		"show_fullscreen"
#define CONF_SET_SHOWNAVMODE		"show_nav_mode"
#define CONF_SET_OPTION_TABID		"option_tabid"
#define	CONF_SET_OPTION_TOOLTIP		"tooltip_checked"
#define CONF_SET_OPTION_GISINFO		"gis_info_checked"
#define CONF_SET_OPTION_LABLE		"lable_size"
#define CONF_SET_OPTION_LL			"latitude_longitude_mode"
#define CONF_SET_OPTION_DISTANCE	"distance_mode"
#define CONF_SET_OPTION_FONT_FAMILY	"font_family"
#define CONF_SET_OPTION_FONT_SIZE	"font_size"
#define CONF_SET_OPTION_FONT_BOLD	"font_bold"
#define CONF_SET_OPTION_FONT_ITALIC	"font_italic"
#define CONF_SET_OPTION_FONT_STRIKEOUT	"font_strikeout"
#define CONF_SET_OPTION_FONT_UNDERLINE	"font_underline"
#define CONF_SET_OPTION_QUALITY		"topography_quality"
#define CONF_SET_OPTION_MAGNIFY		"height_magnification"
#define CONF_SET_OPTION_MEMCACHE	"memory_cache_size"
#define CONF_SET_OPTION_DISKCACHE	"disk_cache_size"
#define CONF_SET_OPTION_TOURTILT	"tour_camera_tilt_angle"
#define CONF_SET_OPTION_TOURRANGE	"tour_camera_range"
#define CONF_SET_OPTION_TOURSPEED	"tour_camera_speed"
#define CONF_SET_OPTION_MOVESPEED	"mousemove_speed"
#define CONF_SET_OPTION_WHEELSPEED	"mousewheel_speed"
#define CONF_SET_OPTION_WHEELDIR	"mousewheel_direction"
#define CONF_SET_SAVELASTDIR		"save_last_dir"
#define CONF_SET_OPENLASTDIR		"open_last_dir"
#define CONF_SET_STATUS_SEARCH		"search_panel_status"
#define	CONF_SET_STATUS_PLACE		"place_panel_status"
#define CONF_SET_STATUS_LAYERS		"layers_panel_status"
#define CONF_SET_HEIGHT_SEARCH		"search_panel_height"
#define CONF_SET_HEIGHT_PLACE		"place_panel_height"
#define CONF_SET_HEIGHT_LAYERS		"layers_panel_height"
#define CONF_SET_POSITION_SEARCH	"search_panel_splitter"
#define CONF_SET_POSITION_PLACE		"place_panel_splitter"
#define CONF_SET_POSITION_LAYERS	"layers_panel_splitter"
#define CONF_SET_RECENT_3DS_PATH     "recent_3ds_path"

#define CONF_SET_SUNLIGHT_ENABLED    "sunlight_enabled"
#define CONF_SET_SUNLIGHT_LONGITUDE  "sunlight_longitude"
#define CONF_SET_SUNLIGHT_LATITUDE   "sunlight_latitude"
#define CONF_SET_SUNLIGHT_HEIGHT     "sunlight_height"
#define CONF_SET_SUNLIGHT_COLOR      "sunlight_color"
#define CONF_SET_AMBIENT_COLOR       "ambient_color"
#define CONF_SET_LIGHT_ATTENUATION   "light_attenuation"


#define CONF_SET_DATABASEPATH		"db_path"
#define CONF_SET_LICENCE            "licence"

Config::Config()
{
	m_sideBarWidth = 0;
	m_srvAddr[0] = 0;
	m_srvPort = 0;
	m_showStandardToolbar = true;
	m_showSimulationToolbar=false;
	m_showAnimationToolbar=false;
	m_showSidebar = true;
	m_showStatusbar = true;
	m_showGrid = false;
	m_layerTopography = LTM_TEXTURE;
	m_contourInterval = 100;
	m_ravineColor = 0x0000FF;
	m_mainCurveColor = 0xBEBEBE;
	m_ravineThick = 2;
	m_mainCurveThick = 1;
	m_showRuler = false;
	m_showFlight = false;
	m_showCar = false;
	m_showTourPlay = false;
	m_fullScreen = false;
	m_showPlaceMark = false;
	m_showPlaceFolder = false;
	m_showPath = false;
	m_showPolygon = false;

	m_optionTabID = OPTION_VIEW_TAB;
	m_toolTipChecked = true;

	/**************************************
	2013 3 7 ugi
	GIS Node´ÉÌ© ±¬ËËÌ® ·²¼¬ËË ¼è¼«¹¤Â×±Â ±ýÀ°±ý·àµá ±¨¼³Ëºµá ±ýÀ°±ý¼è Ê±±Â Â×Êî³Þ.
	***************************************/

	m_gisInfoChecked = false;

	m_lableSize = OPTION_LABLE_MEDIUM;
	m_llShowMode = OPTION_LL_DMS;
	m_distShowMode = OPTION_DISTANCE_METER_KILO;
	m_mainFont = QFont("PRK P Gothic", 10);
	m_topoQuality = 50;
	m_heightMagnify = 1.0;
	m_memCacheSize = 300;
	m_diskCacheSize = 500;
	m_tourTiltAngle = OPTION_TOUR_TILT_DEFAULT;
	m_tourRange = OPTION_TOUR_RANGE_DEFAULT;
	m_tourSpeed = OPTION_TOUR_SPEED_DEFAULT;
	m_moveSpeed =  0.10;
	m_wheelSpeed = OPTION_MOUSE_WHELL_SPEED;
	m_wheelDirection = false;

	m_saveLastDir = "";
	m_openLastDir = "";

	m_statusSearch = BS_Open;
	m_statusPlace  = BS_Open;;
	m_statusLayers  = BS_Open;;
	m_heightSearch = 0;
	m_heightPlace = 0;
	m_heightLayers = 0;
	m_posSearch = 0;
	m_posPlace = 0;
	m_posLayers = 0;

	m_strDataBasePath = "";
#ifdef DEBUG
	m_isShowTerrainWireframe=false;
	m_isShowRenderTileName=false;
	m_isShowRenderTileBoundry=false;
	m_isShowShapeNodes=false;
	m_isShowTerrainInner=false;
	m_isRendering=true;
#endif
	m_recentOpendPathFor3DObjectAdding=QDir::homePath();
	m_recentOpendPathFor3DObjectForAvi=QDir::homePath();

	m_recentSelectedProvinceFor3DObjectAdding =-1;
	m_recentSelectedCountyFor3DObjectAdding   =-1;
	m_recentSelectedVilliageFor3DObjectAdding =-1;

	m_IsLicence=false;
	m_IsShowBuildingName=false;
	m_IsShowOverviewMap=true;
	m_IsSelectObject=false;
	m_blTerrain=true;
	m_blContour=false;
	m_showMeasureArea=false;
	m_showObjectAdd=false;

	m_SunLightEnabled=true;
	m_SunLightLongitude=140;
	m_SunLightLatitude=0.0;
	m_SunLightHeight=50000;
	m_SunLightColor.setRgb(255,255,255);
	m_AmbientLightColor.setRgb(0,0,0);
	m_LightAttenuation=1.0;	

	m_CameraFOV =50;
}

Config::~Config()
{
}

bool Config::IsServer()
{
	if (strlen(m_srvAddr) == 0)
		return false;
	if (m_srvPort == 0)
		return false;

	return true;
}

void Config::SetServer(char *addr, unsigned int port)
{
	strncpy_s(m_srvAddr, addr, CONF_SRVADDR_LEN);
	m_srvPort = port;
}

/*
* Protected
*/

#include "key.h"
void Config::ReadSettings()
{
	QSettings set(CONF_SET_ORG, CONF_SET_APP);

	bool res;

	//main window
	m_wndPos = set.value(CONF_SET_MAINWNDPOS, QPoint(0, 0)).toPoint();
	m_wndSize = set.value(CONF_SET_MAINWNDSIZE, QSize(0, 0)).toSize();

	//sideBar Window
	m_sideBarWidth = set.value(CONF_SET_SIDEBARWIDTH, m_sideBarWidth).toInt();

#ifndef STANDARD_ALONE
	//server
	QString srvAddr = set.value(CONF_SET_SRVADDR, QString("")).toString();
	QVariant srvPort = set.value(CONF_SET_SRVPORT, 0);
	QByteArray ba = srvAddr.toAscii();
	const char *tmp = ba.constData();
	strncpy_s(m_srvAddr, tmp, CONF_SRVADDR_LEN);
	m_srvPort = (unsigned short)srvPort.toInt(&res);
#endif

	//Show/Hide Toolbar, Sidebar, Statusbar
	m_showStandardToolbar = set.value(CONF_SET_SHOWSTANDARDTOOLBAR, m_showStandardToolbar).toBool();
	m_showSimulationToolbar = set.value(CONF_SET_SHOWSIMULATIONTOOLBAR, m_showSimulationToolbar).toBool();
	m_showAnimationToolbar = set.value(CONF_SET_SHOWANIMATIONTOOLBAR, m_showAnimationToolbar).toBool();

	m_showSidebar = set.value(CONF_SET_SHOWSIDEBAR, m_showSidebar).toBool();
	m_showStatusbar = set.value(CONF_SET_SHOWSTATUSBAR, m_showStatusbar).toBool();

	//View Size
	m_fullScreen = set.value(CONF_SET_SHOWFULLSCREEN, m_fullScreen).toBool();

	//Show/Hide Grid
	m_showGrid = set.value(CONF_SET_SHOWGRID, m_showGrid).toBool();

	//Layer State
	m_layerTopography = set.value(CONF_SET_LAYERTOPOGRAPHY, m_layerTopography).toUInt();

	//Contour
	m_contourInterval = set.value(CONF_SET_CONTOURINTERVAL, m_contourInterval).toDouble();
	m_ravineColor = set.value(CONF_SET_RAVINECOLOR, m_ravineColor).toUInt();
	m_mainCurveColor = set.value(CONF_SET_MAINCURVECOLOR, m_mainCurveColor).toUInt();
	m_ravineThick = set.value(CONF_SET_RAVINETHICK, m_ravineThick).toUInt();
	m_mainCurveThick = set.value(CONF_SET_MAINCURVETHICK, m_mainCurveThick).toUInt();

	//Show/Hide Ruler
	m_showRuler = set.value(CONF_SET_SHOWRULER, m_showRuler).toBool();

	//navigation mode
	m_showNavMode = set.value(CONF_SET_SHOWNAVMODE, 0).toInt(&res);

	//option dlg
	m_optionTabID = set.value(CONF_SET_OPTION_TABID, m_optionTabID).toUInt();
	m_toolTipChecked = set.value(CONF_SET_OPTION_TOOLTIP, m_toolTipChecked).toBool();
	m_gisInfoChecked = set.value(CONF_SET_OPTION_GISINFO, m_gisInfoChecked).toBool();
	m_lableSize = set.value(CONF_SET_OPTION_LABLE, m_lableSize).toUInt();
	m_llShowMode = set.value(CONF_SET_OPTION_LL, m_llShowMode).toUInt();
	m_distShowMode = set.value(CONF_SET_OPTION_DISTANCE, m_distShowMode).toUInt();
	m_mainFont.setFamily(set.value(CONF_SET_OPTION_FONT_FAMILY, m_mainFont.family()).toString());
	m_mainFont.setPointSize(set.value(CONF_SET_OPTION_FONT_SIZE, m_mainFont.pointSize()).toInt());
	m_mainFont.setBold(set.value(CONF_SET_OPTION_FONT_BOLD, m_mainFont.bold()).toBool());
	m_mainFont.setItalic(set.value(CONF_SET_OPTION_FONT_ITALIC, m_mainFont.italic()).toBool());
	m_mainFont.setStrikeOut(set.value(CONF_SET_OPTION_FONT_STRIKEOUT, m_mainFont.strikeOut()).toBool());
	m_mainFont.setUnderline(set.value(CONF_SET_OPTION_FONT_UNDERLINE, m_mainFont.underline()).toBool());
	m_topoQuality = set.value(CONF_SET_OPTION_QUALITY, m_topoQuality).toUInt();
	m_heightMagnify = set.value(CONF_SET_OPTION_MAGNIFY, m_heightMagnify).toDouble();
	m_memCacheSize = set.value(CONF_SET_OPTION_MEMCACHE, m_memCacheSize).toUInt();
	m_diskCacheSize = set.value(CONF_SET_OPTION_DISKCACHE, m_diskCacheSize).toUInt();
	m_tourTiltAngle = set.value(CONF_SET_OPTION_TOURTILT, m_tourTiltAngle).toDouble();
	m_tourRange = set.value(CONF_SET_OPTION_TOURRANGE, m_tourRange).toDouble();
	m_tourSpeed = set.value(CONF_SET_OPTION_TOURSPEED, m_tourSpeed).toDouble();
	m_moveSpeed = set.value(CONF_SET_OPTION_MOVESPEED, m_moveSpeed).toDouble();
	m_wheelSpeed = set.value(CONF_SET_OPTION_WHEELSPEED, m_wheelSpeed).toUInt();
	m_wheelDirection = set.value(CONF_SET_OPTION_WHEELDIR, m_wheelDirection).toBool();

	// Last Save & Open Directory
	QString defaultLastDir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
	m_saveLastDir = set.value(CONF_SET_SAVELASTDIR, defaultLastDir).toString();
	m_openLastDir = set.value(CONF_SET_OPENLASTDIR, defaultLastDir).toString();

	// sidebar info
	m_statusSearch = set.value(CONF_SET_STATUS_SEARCH, m_statusSearch).toUInt();
	m_statusPlace = set.value(CONF_SET_STATUS_PLACE, m_statusPlace).toUInt();
	m_statusLayers = set.value(CONF_SET_STATUS_LAYERS, m_statusLayers).toUInt();
	m_heightSearch = set.value(CONF_SET_HEIGHT_SEARCH, m_heightSearch).toUInt();
	m_heightPlace = set.value(CONF_SET_HEIGHT_PLACE, m_heightPlace).toUInt();
	m_heightLayers = set.value(CONF_SET_HEIGHT_LAYERS, m_heightLayers).toUInt();
	m_posSearch = set.value(CONF_SET_POSITION_SEARCH, m_posSearch).toUInt();
	m_posPlace = set.value(CONF_SET_POSITION_PLACE, m_posPlace).toUInt();
	m_posLayers = set.value(CONF_SET_POSITION_LAYERS, m_posLayers).toUInt();
	m_recentOpendPathFor3DObjectAdding=set.value(CONF_SET_RECENT_3DS_PATH).toString();

	if(set.contains(CONF_SET_SUNLIGHT_ENABLED))
		m_SunLightEnabled=set.value(CONF_SET_SUNLIGHT_ENABLED).toBool();

	if(set.contains(CONF_SET_SUNLIGHT_LONGITUDE))
		m_SunLightLongitude=set.value(CONF_SET_SUNLIGHT_LONGITUDE).toDouble();

	if(set.contains(CONF_SET_SUNLIGHT_LATITUDE))
		m_SunLightLatitude=set.value(CONF_SET_SUNLIGHT_LATITUDE).toDouble();

	if(set.contains(CONF_SET_SUNLIGHT_HEIGHT))
		m_SunLightHeight=set.value(CONF_SET_SUNLIGHT_HEIGHT).toDouble();

	if(set.contains(CONF_SET_SUNLIGHT_COLOR))
		m_SunLightColor.setNamedColor(set.value(CONF_SET_SUNLIGHT_COLOR).toString());

	if(set.contains(CONF_SET_AMBIENT_COLOR))
		m_AmbientLightColor.setNamedColor(set.value(CONF_SET_AMBIENT_COLOR).toString());

	if(set.contains(CONF_SET_LIGHT_ATTENUATION))
		m_LightAttenuation=set.value(CONF_SET_LIGHT_ATTENUATION).toDouble();

	QSettings setSrv(QSettings::SystemScope, CONF_SET_ORG, CONF_SET_SRV_APP);
	m_strDataBasePath = setSrv.value(CONF_SET_DATABASEPATH, m_strDataBasePath).toString();
	
	QString licence=set.value(CONF_SET_LICENCE,"").toString();

	QString formatedserial=FormatKey(GetPhysicalDrive0ModelNumber(), 16);

	QString key=FormatKey(GetKey(formatedserial),16);

	if(key!=licence)
		m_IsLicence=false;
	else
		m_IsLicence=true;
}

void Config::WriteSettings()
{
	QSettings set(CONF_SET_ORG, CONF_SET_APP);

	//main windows
	MainWindow *mainWnd = g_app->GetMainWindow();
	if (mainWnd && mainWnd->isEnabled())
	{
		QPoint pos = mainWnd->pos();
		QSize size = mainWnd->size();
		if (m_fullScreen || mainWnd->isMaximized() || mainWnd->isMinimized())
		{
			pos = QPoint(0,0);
			size = QSize(0,0);
		}
		set.setValue(CONF_SET_MAINWNDPOS, pos);
		set.setValue(CONF_SET_MAINWNDSIZE, size);
	}

	//SideBar Window Width
	set.setValue(CONF_SET_SIDEBARWIDTH, m_sideBarWidth);

	//server
	set.setValue(CONF_SET_SRVADDR, QString(m_srvAddr));
	set.setValue(CONF_SET_SRVPORT, QVariant(m_srvPort));

	//Show/Hide Toolbar, Sidebar, Statusbar
	set.setValue(CONF_SET_SHOWSTANDARDTOOLBAR, m_showStandardToolbar);
	set.setValue(CONF_SET_SHOWSIMULATIONTOOLBAR, m_showSimulationToolbar);
	set.setValue(CONF_SET_SHOWANIMATIONTOOLBAR, m_showAnimationToolbar);
	set.setValue(CONF_SET_SHOWSIDEBAR, m_showSidebar);
	set.setValue(CONF_SET_SHOWSTATUSBAR, m_showStatusbar);

	//View Size
	set.setValue(CONF_SET_SHOWFULLSCREEN, m_fullScreen);

	//Show/Hide Grid, AllMap
	set.setValue(CONF_SET_SHOWGRID, m_showGrid);

	//Layer State
	set.setValue(CONF_SET_LAYERTOPOGRAPHY, m_layerTopography);

	//Contour
	set.setValue(CONF_SET_CONTOURINTERVAL, m_contourInterval);
	set.setValue(CONF_SET_RAVINECOLOR, m_ravineColor);
	set.setValue(CONF_SET_MAINCURVECOLOR, m_mainCurveColor);
	set.setValue(CONF_SET_RAVINETHICK, m_ravineThick);
	set.setValue(CONF_SET_MAINCURVETHICK, m_mainCurveThick);

	//Show/Hide Ruler
	set.setValue(CONF_SET_SHOWRULER, m_showRuler);

	//navigaion mode
	set.setValue(CONF_SET_SHOWNAVMODE, m_showNavMode);

	//option dlg
	set.setValue(CONF_SET_OPTION_TABID, m_optionTabID);
	set.setValue(CONF_SET_OPTION_TOOLTIP, m_toolTipChecked);
	set.setValue(CONF_SET_OPTION_GISINFO, m_gisInfoChecked);
	set.setValue(CONF_SET_OPTION_LABLE, m_lableSize);
	set.setValue(CONF_SET_OPTION_LL, m_llShowMode);
	set.setValue(CONF_SET_OPTION_DISTANCE, m_distShowMode);
	set.setValue(CONF_SET_OPTION_FONT_FAMILY, m_mainFont.family());
	set.setValue(CONF_SET_OPTION_FONT_SIZE, m_mainFont.pointSize());
	set.setValue(CONF_SET_OPTION_FONT_BOLD, m_mainFont.bold());
	set.setValue(CONF_SET_OPTION_FONT_ITALIC, m_mainFont.italic());
	set.setValue(CONF_SET_OPTION_FONT_STRIKEOUT, m_mainFont.strikeOut());
	set.setValue(CONF_SET_OPTION_FONT_UNDERLINE, m_mainFont.underline());
	set.setValue(CONF_SET_OPTION_QUALITY, m_topoQuality);
	set.setValue(CONF_SET_OPTION_MAGNIFY, m_heightMagnify);
	set.setValue(CONF_SET_OPTION_MEMCACHE, m_memCacheSize);
	set.setValue(CONF_SET_OPTION_DISKCACHE, m_diskCacheSize);
	set.setValue(CONF_SET_OPTION_TOURTILT, m_tourTiltAngle);
	set.setValue(CONF_SET_OPTION_TOURRANGE, m_tourRange);
	set.setValue(CONF_SET_OPTION_TOURSPEED, m_tourSpeed);
	set.setValue(CONF_SET_OPTION_MOVESPEED, m_moveSpeed);
	set.setValue(CONF_SET_OPTION_WHEELSPEED, m_wheelSpeed);
	set.setValue(CONF_SET_OPTION_WHEELDIR, m_wheelDirection);

	// Last Save & Open Directory
	set.setValue(CONF_SET_SAVELASTDIR, m_saveLastDir);
	set.setValue(CONF_SET_OPENLASTDIR, m_openLastDir);

	// sidebar info
	set.setValue(CONF_SET_STATUS_SEARCH, m_statusSearch);
	set.setValue(CONF_SET_STATUS_PLACE, m_statusPlace);
	set.setValue(CONF_SET_STATUS_LAYERS, m_statusLayers);
	set.setValue(CONF_SET_HEIGHT_SEARCH, m_heightSearch);
	set.setValue(CONF_SET_HEIGHT_PLACE, m_heightPlace);
	set.setValue(CONF_SET_HEIGHT_LAYERS, m_heightLayers);
	set.setValue(CONF_SET_POSITION_SEARCH, m_posSearch);
	set.setValue(CONF_SET_POSITION_PLACE, m_posPlace);
	set.setValue(CONF_SET_POSITION_LAYERS, m_posLayers);
	set.setValue(CONF_SET_RECENT_3DS_PATH,m_recentOpendPathFor3DObjectAdding);
	set.setValue(CONF_SET_SUNLIGHT_ENABLED,m_SunLightEnabled);
	set.setValue(CONF_SET_SUNLIGHT_LONGITUDE,m_SunLightLongitude);
	set.setValue(CONF_SET_SUNLIGHT_LATITUDE,m_SunLightLatitude);
	set.setValue(CONF_SET_SUNLIGHT_HEIGHT,m_SunLightHeight);
	set.setValue(CONF_SET_SUNLIGHT_COLOR,m_SunLightColor.name());
	set.setValue(CONF_SET_AMBIENT_COLOR,m_AmbientLightColor.name());
	set.setValue(CONF_SET_LIGHT_ATTENUATION,m_LightAttenuation);

	QSettings setSrv(QSettings::SystemScope, CONF_SET_ORG, CONF_SET_SRV_APP);
	setSrv.setValue(CONF_SET_DATABASEPATH, m_strDataBasePath);
}

void Config::WriteLicence(QString licence)
{
	QSettings set(CONF_SET_ORG, CONF_SET_APP);

	set.setValue(CONF_SET_LICENCE,licence);
	m_IsLicence=true;
}
