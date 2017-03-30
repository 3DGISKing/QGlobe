#ifndef KML_TREEITEM_H
#define KML_TREEITEM_H

/****************************************************************

	NOTE:	GUI's ResultTreeView use all types in this file

****************************************************************/

#include <QString>

#include "gdemclientbase.h"
#include "gdem_protocol.h"


/********************************************************** 
					Constatns
**********************************************************/
#define TREEITEM_NAME_STR_MAX_LEN	(PMD_STR_MAX_SIZE)	// PMD_STR_MAX_SIZE is defined in "gdemclientbase.h" file
#define TREEITEM_DESC_STR_MAX_LEN	(PMD_STR_MAX_SIZE)


/********************************************************** 
					Enums
**********************************************************/
// Type of Kml TreeItems
typedef enum
{
	KML_IT_UNKNOWN		= -1,	// Unknown (Default)
	KML_IT_FOLDER		=  0,	// Folder
	KML_IT_DOCUMET		=  1,	// Document
	KML_IT_PLACESEARCH	=  2,	// Place (Search)
	KML_IT_PLACEMARK	=  3,	// Place (User Add)
	KML_IT_PAGE			=  4,	// Page
	KML_IT_POLYAREA		=  5,	// Place (User Add)
	KML_IT_PATH			=  6	// Place (User Add)
} KmlTreeItemType;

// Altitude Mode
typedef enum {
	KML_ALTITUDEMODE_CLAMPED_TO_GROUND,
	KML_ALTITUDEMODE_CLAMPED_TO_SEA,
	KML_ALTITUDEMODE_RELATIVE_TO_GROUND,
	KML_ALTITUDEMODE_RELATIVE_TO_SEA,
	KML_ALTITUDEMODE_ABSOLUTE
} KmlAltitudeMode;

typedef enum {
	KML_STYLESTATE_NORMAL = 0,
	KML_STYLESTATE_HIGHLIGHT
} KML_StyleStateEnum;

typedef enum {
	KML_UNITS_FRACTION = 0,
	KML_UNITS_PIXELS,
	KML_UNITS_INSETPIXELS
} KML_UnitsEnum;

// Altitude Mode
typedef enum {
	KML_GX_ALTITUDEMODE_CLAMPTOSEAFLOOR	= 0	,
	KML_GX_ALTITUDEMODE_RELATIVETOSEAFLOOR
} KmlGxAltitudeMode;

#define STYLE_URL_PRIFIX_SYMBOL					"#"
#define STYLE_URL_BASE_STRING					"_ylw-pushpin"
#define PLACEMARK_STYLE_URL_PREFIX_STRING		"msn"
#define NORMAL_STYLE_URL_PREFIX_STRING			"sn"
#define HIGHLIGHT_STYLE_URL_PREFIX_STRING		"sh"

/*
[REFERENCE] kmldom::AltitudeModeEnum is defined in LibKml library as:

typedef enum {
	ALTITUDEMODE_CLAMPTOGROUND = 0,
	ALTITUDEMODE_RELATIVETOGROUND,
	ALTITUDEMODE_ABSOLUTE
} AltitudeModeEnum;
*/

// Visibility Type
typedef enum
{
	KML_VISTYPE_HIDE		= 0,	// Hide (<visibility>0</visibility>, unchecked)
	KML_VISTYPE_PARTIALLY	= 1,	// Show (<visibility>1</visibility>, checked)
	KML_VISTYPE_SHOW		= 2		// Show (<visibility>1</visibility>, checked)
} KmlVisibeType ;


/********************************************************** 
					Structures
**********************************************************/
// KmlLookAt
typedef struct
{
	unsigned int		color;
	int					colormode;
	double				scale;
	float				penwidth;
} KmlStyleElement;

typedef struct
{
	QString				iconname;
	unsigned int		color;
	int					colormode;
	double				heading;
	double				scale;
	float				penwidth;
	QString             videopath;//2014 2 6 by ugi
	QString             htmlpath;//2014 2 7 by ugi
} KmlIconStyleElement;

typedef struct
{
	unsigned int		color;
	int					colormode;
	float				penwidth;
} KmlLineStyleElement;

typedef struct
{
	unsigned int		color;
	int					colormode;
	bool				outline;
	bool				fill;
		
	QString				polyTexture; //by RSH 2013.8.1
} KmlPolyStyleElement;

#define NORMAL_STYLE_URL_POS		0
#define HIGHLIGHT_STYLE_URL_POS		1
#define MAX_STYLE_URL_NUM			2

typedef struct KmlStyle_t
{
	QString					styleid;
	QString					styleMapid;
	QString					normal_styleurl;
	QString					highlight_styleurl;
	KmlIconStyleElement *	iconstyle;
	KmlStyleElement	*		labelstyle;
	KmlPolyStyleElement	*	ploystyle;
	KmlLineStyleElement	*	linestyle;
	KmlStyle_t			*	prev;
	KmlStyle_t			*	next;
} KmlStyle;

// KmlLookAt
typedef struct
{
	double				m_longitude;
	double				m_latitude;
	double				m_altitude;
	double				m_heading;
	double				m_range;
	double				m_tilt;
	KmlAltitudeMode		m_altitudeMode;
	KmlGxAltitudeMode	m_gx_altitudeMode;
} KmlLookAt;

// KmlPoint
typedef struct
{
	double	m_longitude;
	double	m_latitude;
	double	m_altitude;	
} KmlCoordPoint;


/********************************************************** 
					Class KmlTreeItem
**********************************************************/
class KmlTreeItem
{
public:
	KmlTreeItem();
	KmlTreeItem(KmlTreeItemType type);
        KmlTreeItem(KmlTreeItemType type, QString name, QString desc, QString styleurl, KmlVisibeType visibletype, KmlLookAt* lookat, KmlCoordPoint* coordpoint, unsigned int color, float penwidth);
	~KmlTreeItem();
	

	// public member functions
	void SetType(const KmlTreeItemType type);
	KmlTreeItemType GetType();	

	void SetSubType(const E_GDM_SUBDATA_TYPE subtype);
	E_GDM_SUBDATA_TYPE GetSubType();	

	void SetName(const QString name);
	void GetName(QString& name);	

	void SetDescription(const QString desc);
	void GetDescription(QString& desc);	

	void SetStyleUrl(const QString name);
	void GetStyleUrl(QString& name);	

	void SetVisibleType(const KmlVisibeType visibletype);
	KmlVisibeType GetVisibleType();

	void SetStartLocation(const bool startLocation);
	bool GetStartLocation();

	void SetStyle(KmlStyle *style);
	KmlStyle * GetStyle();

	void SetLookAt(KmlLookAt* lookat);
	KmlLookAt* GetLookAt();

	void SetCoordPoint(KmlCoordPoint* coordpoint);
	KmlCoordPoint* GetCoordPoint();	

	void SetCoordPointArraySize(int array_size);
	int	 GetCoordPointArraySize() { return m_coordpoint_array_size; }

	void SetExtrude(const bool extrude) { m_extrude = extrude; }
	bool GetExtrude() { return m_extrude; }

	void SetAltitudeMode(const KmlAltitudeMode mode) { m_altitudeMode = mode; }
	KmlAltitudeMode GetAltitudeMode() { return m_altitudeMode; }

	// public functions
	int AddToKmlTree(KmlTreeItem* itemParent);			// Add My KmlTreeItem to Kml Tree
	int ConvertToTreeItemData(PLACEMARK_DATA* tdata);	// Convert KmlTreeItem data ---> TreeItem data
	int ConvertFromTreeItemData(PLACEMARK_DATA* tdata);	// Convert TreeItem data ---> KmlTreeItem data


private:
	// private member variables
	KmlTreeItemType		m_type;
	E_GDM_SUBDATA_TYPE	m_subtype;
	QString				m_name;
	QString				m_desc;
	QString				m_styleurl;
	KmlVisibeType		m_visibletype;		// Visibility type
	bool				m_startLocation;	// Starting Location Flag
	KmlStyle *			m_style;
	KmlLookAt*			m_lookat;
	KmlCoordPoint*		m_coordpoint;
	int					m_coordpoint_array_size;
	bool				m_extrude;
	KmlAltitudeMode		m_altitudeMode;

//	unsigned int	m_color;				// (alpha << 24 | r << 16 | g << 8 | b). use for pen or brush
//	float			m_penwidth;				// the width of line

public:
	// public member variables
	KmlTreeItem*	m_childHead;
	KmlTreeItem*	m_childTail;
	KmlTreeItem*	m_next;
};

#endif // KML_TREEITEM_H
