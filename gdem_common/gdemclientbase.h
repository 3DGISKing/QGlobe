#ifndef _GDEM_CLIENTBASE_H_
#define _GDEM_CLIENTBASE_H_

#include <QMap>

#define PMD_STR_MAX_SIZE		254
#define FONT_FAMILY_MAX_SIZE	64
#define FONT_NAME_MAX_SIZE		256

enum ItemType {IT_DOC, IT_FOLDER, IT_SEARCH, IT_PLACEMARK, IT_PATH, IT_POLYAREA, IT_PAGE};
enum ShowMarkType {SMT_HIDE, SMT_PARTIALLY, SMT_SHOW};
enum PLACEMARK_STATE { 	PLACEMARK_NORMAL, PLACEMARK_HOVER, PLACEMARK_SELECT };

#define PLACEMARK_DEMHEIGHT_INVALID	-3000000

typedef struct _PLACEMARK_DATA
{
	unsigned int    type;					// country, capital, ...
	double          longitude;
	double          latitude;
	double			altitude;
	ushort			name[PMD_STR_MAX_SIZE];
	ushort			description[PMD_STR_MAX_SIZE];
	unsigned int	itemtype;
	double			view_longitude;
	double			view_latitude;
	double			view_range;
	double			view_heading;
	double			view_tilt;
	bool			snapshot;
	int				cx, cy;					//current position
	//int				x1, y1, x2, y2;			//rect of placemark pin
	bool			visible;
	unsigned int	state;
	ushort			showmark;				// 0 mark hide, 1 partially show, 2 mark show
	bool			start;					//starting location flag
	unsigned int	key;
	void*			item;
	double			orgDist;	//[ChaKI] for fast moveto
	/* for name search */
	int				db_index;	// server record index
	ushort			other_name[PMD_STR_MAX_SIZE];
	
	ushort			iconname[PMD_STR_MAX_SIZE]; //by RSH 2013.7.20
	double          iconscale;                  //by ugi 2013.8.1
	ushort          videopath[PMD_STR_MAX_SIZE]; //by ugi 2014 2 6
	ushort          htmlpath[PMD_STR_MAX_SIZE];  //by ugi 2014 2 7

}PLACEMARK_DATA;

typedef struct _mouse_pos
{
	double          longitude;	
	double          latitude;
	double			altitude;
	int				x;	// x coordinate in screen
	int				y;	// y coordinate in screen

	double			dX;	// x coordinate in world screen
	double			dY; // y coordinate in world screen
	double			dZ; // z coordinate in world screen

	// select range
	int				rangeInScreen;
	double			rangeInWorld;

	bool			blEnable;

}MOUSE_POSITION;

/*
* mouse and keyboard state
*/
enum KEY_STATE
{
	KS_NO		= 0x0000,
	KS_SHIFT	= 0x0001,
	KS_CTRL		= 0x0002,
	KS_ALT		= 0x0004,
	KS_SPACE	= 0x0008
};

enum MOUSE_BUTTON_STATE
{
	MBS_NO			= 0x0000,
	MBS_LEFT		= 0x0001,
	MBS_RIGHT		= 0x0002,
	MBS_MID			= 0x0004,
	MBS_CLICKED		= 0x0100,
	MBS_DCLICKED	= 0x0200
};

typedef struct _CursorState
{
	int x;
	int y;
	int delta; // for mouse wheel
	unsigned long buttons;
	unsigned long keys;
} CursorState;

typedef struct _KeyState
{
	bool bMoveKey;
	int key1;
	int key2;
	unsigned long keyFlags;
} KeyState;

typedef struct _RulerInfo
{
	bool	blLine;
	int		x;
	int		y;
	short	nStep;
} RulerInfo;

/*
* Layer State
*/

#define OPTION_MOUSE_WHELL_SPEED	20
#define ZOOM_SPEED_MAX		10


enum LAYER_TEXTURE_MODE 
{
	LTM_TEXTURE,
	LTM_RASTER,
	LTM_HEIGHT,
	LTM_RATEANGLE,
	LTM_RATEPLAN,
};

/*
* Contour State
*/
typedef struct _ContourStateInfo
{
	bool			contourChecked;
	double			contourInterval;
	unsigned int	ravineColor;
	unsigned int	mainCurveColor;
	unsigned int	ravineThick;
	unsigned int	mainCurveThick;
} ContourStateInfo;

/* Grid View Mode Info */
typedef struct _GridViewInfo
{
	bool			gridView;
	int			gridMode;
} GridViewInfo;

/*
* ruler information 
*/
#define MEASURE_MODE_LINE	0 //MeasureInfo->mode 0
#define MEASURE_MODE_PATH	1 //MeasureInfo->mode 1

typedef struct _MeasureInfo
{
	int mode;
	bool tracking;
	double length;
	float angle;
	int unit;
} MeasureInfo;

/*
* status bar information 
*/
typedef struct _StatusBarInfo
{
	double	longitude;
	double	latitude;
	int		distance;
	double	height;
	float	rate_angle; // rate angle			(0-360) 0:north, 90:west, 180:south, 270:east
	float	dir_angle;  // direction plan angle (0-90)
	int		pie;
	int		frames;
	int		real_frames;
	int		meshCount;
	int		lineCount;
} StatusBarInfo;

/*
* Font Information
*/
enum FONTSTYLE
{
	FONT_ITALIC		= 0x0001,
	FONT_BOLD		= 0x0002,
	FONT_STRIKEOUT	= 0x0004,
	FONT_UNDERLINE	= 0x0008
};

typedef struct _FontInfo
{
	ushort			family[FONT_FAMILY_MAX_SIZE];
	int				size;	
	ushort			fileName[FONT_NAME_MAX_SIZE];
	unsigned int	flag;
} FontInfo;

/*
* degree and radian converter function
*/
#define PAI 3.14159265358979323846

inline double degree2radian(double angle)
{	return (angle*PAI)/180; }

inline double radian2degree(double radian)
{	return (radian*180)/PAI; }

/*
* for display text
*/
//render text infomation
#define RTI_MAX_TEXTLEN		20

enum RENTER_TEXT_TYPE
{
	RTT_NONE,
	RTT_PYONGYANG,

	// place mark
	RTT_PLACE_MARK_SELECT,
	RTT_PLACE_MARK_NORMAL,
	RTT_SEARCH_DATA_NORMAL,
	RTT_SEARCH_DATA_SELECT,
	RTT_PLACE_LENGTH,
	RTT_PLACE_AREA,

	RTT_NAME_DEFAULT,

	// grid text
	RTT_GRID,
	RTT_CONTOUR,

	RTT_MAX
};

enum RENDER_TEXT_SIZE
{
	// render text size
	RTS_NORMAL,
	RTS_SMALL,
	RTS_LARGE,

	RTS_END
};

enum _FONT_SIZE_ID
{
	FSI_TOPBIG=0,
	FSI_BIG=1,
	FSI_LARGE=2,
	FSI_NORMAL=3,
	FSI_SAMLL=4,
	FSI_TINY=5,
	FSI_TOPTINY=6,
	FSI_END
};

#define FSI_TOPBIG_OFFSET	6
#define FSI_BIG_OFFSET		5
#define FSI_LARGE_OFFSET	4
#define FSI_NORMAL_OFFSET	3
#define FSI_SAMLL_OFFSET	2
#define FSI_TINY_OFFSET		1
#define FSI_TOPTINY_OFFSET	0

typedef struct _RenderTextInfo
{
	int				type;						// string type
	short			x;							// x position in screen
	short			y;							// y position in screen
	unsigned short	text[RTI_MAX_TEXTLEN];		// string in unicode
} RenderTextInfo;
// end string structure


class QPainter;
typedef struct  
{
	QPainter *qPainter;
}GDM_RENDER_INFO;

typedef QMap<unsigned int, PLACEMARK_DATA> PlaceMarkMap;

typedef enum 
{
	eGDM_CMD_NONE = 0,
	eGDM_CMD_SELECT,
	eGDM_CMD_CAMERA,
	eGDM_CMD_RULER,
	eGDM_CMD_MEASUREAREA,
	eGDM_CMD_PLACEMARK,
	eGDM_CMD_DRAW_POLYGON,
	eGDM_CMD_OBJECT_ADD,
	eGDM_CMD_SIMULATION,
	eGDM_CMD_DRAW_OBJECT //asd 14.4.11
}E_GDM_CMD_TYPE;


typedef enum //asd 14.4.11
{ 
	PLACE_POINT, 
	OBJECT_POINT, 
	OBJECT_LINE, 
	OBJECT_POLYGON
}SHAPE_TYPE;


typedef enum 
{
	CURSOR_SHAPE_NORMAL,
	CURSOR_SHAPE_SELECTED,
	CURSOR_SHAPE_POINTING,
	CURSOR_SHAPE_RULER,
	CURSOR_SHAPE_ARROW,
	CURSOR_SHAPE_SIZEVERT,
	CURSOR_SHAPE_CROSS,
	CURSOR_SHAPE_NO,

	// User Cursor
	CURSOR_SHAPE_POINTMARK,

	CURSOR_SHAPE_COUNT
}CURSOR_SHAPE;

#endif // _GDEM_CLIENTBASE_H_
