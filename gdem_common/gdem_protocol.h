#ifndef _GDEM_PROTOCOL_H_
#define _GDEM_PROTOCOL_H_


#define DS_ADDR_LEN 128

// request type
typedef enum
{
	E_REQUEST_TYPE_NONE,
	E_REQUEST_TYPE_DISTRIBUTE,
	E_REQUEST_TYPE_GEONAMELIST,
	E_REQUEST_TYPE_GEODATA,
	E_REQUEST_TYPE_DBCOMMAND,
} E_REQUEST_TYPE;
/************************************/

// geometric main data type
typedef enum {
	E_GDM_MAINDATA_TYPE_IMG		= 0,			/* terrain imagery data*/
	E_GDM_MAINDATA_TYPE_DEM		= 1,			/* terrain elevation height data*/
	E_GDM_MAINDATA_TYPE_SHAPE	= 2,			/* terrain elevation height data*/
	E_GDM_MAINDATA_TYPE_NAME	= 3,			/* terrain elevation height data*/
	E_GDM_MAINDATA_TYPE_3D		= 4,			/* 3D building, texture data */
	E_GDM_MAINDATA_TYPE_RASTER  =5,
	E_GDM_MAINDATA_TYPE_MAX,						/* max count type*/
	E_GDM_MAINDATA_TYPE_NONE
} E_GDM_MAINDATA_TYPE;


// geometric subject data type
typedef enum {
	E_GDM_SUBDATA_TYPE_NONE					    = 0,

	//Ãùº¬
	E_GDM_SUBDATA_TYPE_IMG					    = 1,
	E_GDM_SUBDATA_TYPE_IMG_NASA				    = 2,
	E_GDM_SUBDATA_TYPE_IMG2011                  = 60 ,

	//²ÚËË·ºÂô
	E_GDM_SUBDATA_TYPE_DEM					    = 3,

	//3½ÓÌº»ôµê
	E_GDM_SUBDATA_TYPE_3D_BUILDING			    = 4,
	E_GDM_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING	= 5,

	//ÃÔ¼³¼Ú»ª¼è
	E_GDM_SUBDATA_TYPE_NAME_COUNTRY			    = 6,
	E_GDM_SUBDATA_TYPE_NAME_POPULAR			    = 7,
	E_GDM_SUBDATA_TYPE_NAME_FIRSTADMIN		    = 8,
	E_GDM_SUBDATA_TYPE_NAME_SECONDADMIN		    = 9,
	E_GDM_SUBDATA_TYPE_NAME_THIRDADMIN		    = 10,

    //º¡¼¬´Ýº¬
	E_GDM_SUBDATA_TYPE_NAME_BROMZEITEM			= 11,	
	E_GDM_SUBDATA_TYPE_NAME_HISTORICSITEITEM	= 12,
	E_GDM_SUBDATA_TYPE_NAME_BATTLESITEITEM		= 13,
	E_GDM_SUBDATA_TYPE_NAME_MONUMENTITEM		= 14,
	E_GDM_SUBDATA_TYPE_NAME_MUSEUMITEM			= 15,	
	E_GDM_SUBDATA_TYPE_NAME_POSITIONITEM		= 16,
	E_GDM_SUBDATA_TYPE_NAME_GUIDANCEITEM		= 17,

	//¼è··
	E_GDM_SUBDATA_TYPE_NAME_MOUNTAIN	        = 18,
	E_GDM_SUBDATA_TYPE_NAME_SEA		            = 19,
	E_GDM_SUBDATA_TYPE_NAME_ISLAND			    = 20,
	E_GDM_SUBDATA_TYPE_NAME_RIVER			    = 21,
	E_GDM_SUBDATA_TYPE_NAME_AIRPORT			    = 22,
	
	E_GDM_SUBDATA_TYPE_SHAPE_COUNTRY		    = 23,		//±ýµ¹°Ò±Ë
	E_GDM_SUBDATA_TYPE_SHAPE_FIRSTADMIN		    = 24,		//´ª°Ò±Ë
	E_GDM_SUBDATA_TYPE_SHAPE_SECONDADMIN	    = 25,		//°ë°Ò±Ë
	E_GDM_SUBDATA_TYPE_SHAPE_THIRDADMIN		    = 26,		//¶®°Ò±Ë

	E_GDM_SUBDATA_TYPE_SHAPE_HIGHWAY		    = 27,		
	E_GDM_SUBDATA_TYPE_SHAPE_1STLOAD		    = 28,		
	E_GDM_SUBDATA_TYPE_SHAPE_2STLOAD    	    = 29,
	E_GDM_SUBDATA_TYPE_SHAPE_3STLOAD		    = 30,
	E_GDM_SUBDATA_TYPE_SHAPE_4STLOAD    	    = 31,
	E_GDM_SUBDATA_TYPE_SHAPE_5STLOAD		    = 32,

	E_GDM_SUBDATA_TYPE_SHAPE_WIDERAILWAY   	    = 33,
	E_GDM_SUBDATA_TYPE_SHAPE_NARROWRAILWAY	    = 34,

	E_GDM_SUBDATA_TYPE_SHAPE_WIDERIVER   	    = 35,
	E_GDM_SUBDATA_TYPE_SHAPE_RIVER       	    = 36,
	E_GDM_SUBDATA_TYPE_SHAPE_LAKE        	    = 37,

	E_GDM_SUBDATA_TYPE_SHAPE_FARMLAND   	    = 38,
	E_GDM_SUBDATA_TYPE_SHAPE_NONFARMLAND   	    = 39,
	E_GDM_SUBDATA_TYPE_SHAPE_FORESTLAND   	    = 40,
	
	//ÃÍÊÈ´Ýº¬
	E_GDM_SUBDATA_TYPE_NAME_HARBOUR             = 41,//Âß°é
	E_GDM_SUBDATA_TYPE_NAME_DOCK                = 42,//¹¢´¸
	E_GDM_SUBDATA_TYPE_NAME_FIREWAY             = 43,//Âßµá
	E_GDM_SUBDATA_TYPE_NAME_TIDELAND            = 44,//°£º¸¼è
	E_GDM_SUBDATA_TYPE_NAME_FISHFARM            = 45,//ÊÈÊÌ»þ
	E_GDM_SUBDATA_TYPE_NAME_SEAFARM             = 46,//ÊÈ»¥»þ
	E_GDM_SUBDATA_TYPE_NAME_FISHERY             = 47,//ÊÌ»þ
	E_GDM_SUBDATA_TYPE_NAME_MARINEFACOTRY       = 48,//ºãº¤º¡ÊÕºÏ
	E_GDM_SUBDATA_TYPE_NAME_SEAWATERFACTORY     = 49,//¸Æ³Þ·Ï¶®Ë¦º¡ÊÕºÏ
	E_GDM_SUBDATA_TYPE_NAME_IRRIGATIONFACTORY   = 50,//±ã±¶¶®Ë¦º¡ÊÕºÏ
	E_GDM_SUBDATA_TYPE_NAME_BEAUTY              = 51,//··»£¼è
	E_GDM_SUBDATA_TYPE_NAME_SEABATHY            = 52,//ÃÍºãÊþ»þ
	E_GDM_SUBDATA_TYPE_NAME_SPA                 = 53,//Êñ½ç
	E_GDM_SUBDATA_TYPE_NAME_MISC_PROTECTION_POINT    = 54,//±¨À°¸óÂö´Ýº¬(¼°)
	E_GDM_SUBDATA_TYPE_NAME_MISC_PROTECTION_POLYGON  = 55,//±¨À°¸óÂö´Ýº¬(·²)
	E_GDM_SUBDATA_TYPE_NAME_ACCIDENT            = 56,//º¡°ÖÌ¡¾·
	E_GDM_SUBDATA_TYPE_NAME_POLUTANT            = 57,//ÊïÊåºãÊß
	
	E_GDM_SUBDATA_TYPE_RASTER_SUNENERGY          = 58,//ÊïÊåºãÊß
	E_GDM_SUBDATA_TYPE_RASTER_SUNENERGY1         = 59,//ÊïÊåºãÊß

	
	E_GDM_SUBDATA_TYPE_MAX                      = 61

} E_GDM_SUBDATA_TYPE;
/************************************/

// response error code
typedef enum {
	E_RESPONSE_ERROR_NONE,
	E_RESPONSE_ERROR_UNKNOWN,
	E_RESPONSE_ERROR_NODATA,
	E_RESPONSE_ERROR_UNKNOWNREQUEST,
	E_RESPONSE_ERROR_NULLDBMANAGER,
	E_RESPONSE_ERROR_UNKNOWNGEODATATYPE,
	E_RESPONSE_ERROR_FILENAME,
	E_RESPONSE_ERROR_FILESIZE_ZERO,
	E_RESPONSE_ERROR_FILE_NOEXIST,
	E_RESPONSE_ERROR_FILE_OPEN,
	E_RESPONSE_ERROR_FILE_MAPPING,
	E_RESPONSE_ERROR_FILE_UNMAPPING,
	E_RESPONSE_ERROR_FILE_READ,
	E_RESPONSE_ERROR_BLOCKFILE_NOEXIST,
	E_RESPONSE_ERROR_TILE_NOFOUND,
	E_RESPONSE_ERROR_INVALID_LEVEL,
	E_RESPONSE_ERROR_INVALID_INDEX,
	E_RESPONSE_ERROR_BLOCKFILE_HEADERSIZE_ZERO,
	E_RESPONSE_ERROR_BLOCKFILE_LEVEL_DIFFERENT,
	E_RESPONSE_ERROR_BLOCKFILE_TILE_INDEX,
	E_RESPONSE_ERROR_SQL_QUERY,
	E_RESPONSE_ERROR_DATABASE_OPEN,
	E_RESPONSE_ERROR_FILE_LEVEL_NOT_EQUAL,
	E_RESPONSE_ERROR_BYTES_OVERFLOW,
	E_RESPONSE_ERROR_BLOCKFILE_INVALID,
	E_RESPONSE_ERROR_INVALID_DISTRIBUTE_PC,
	E_RESPONSE_ERROR_NAMECOUNT_OVERFLOW,
} E_RESPONSE_ERROR_TYPE;

extern const char* g_strErrorList[];

/************************************/

// define compress type
typedef enum {
	E_COMPRESS_NONE,
	E_COMPRESS_JPG,	
	E_COMPRESS_PNG,	
	E_COMPRESS_RAW,
	E_COMPRESS_7ZIP,
} E_COMPRESS_TYPE;
/************************************/

// define search type
typedef enum {
	E_SEARCH_TYPE_ID = 0,
	E_SEARCH_TYPE_DATA
} E_SEARCH_TYPE;
/************************************/

typedef struct tagPACKET_HEADER
{
	unsigned int bytes;				// sizeof(buffer) + owner
	unsigned char buffer[4];		// ST_REQUEST_HEADER, ST_RESPONSE_HEADER
} ST_PACKET_HEADER;


/******************************* Request Structure *********************************/
/***********************************************************************************/
typedef struct tagREQUEST_HEADER
{
	E_REQUEST_TYPE		type;		// request type
	unsigned char		body[4];	// ST_REQUEST_GEONAMESEARCH, ST_REQUEST_GEODATA, ST_REQUEST_DBCOMMAND
} ST_REQUEST_HEADER;

typedef struct tagREQUEST_GEONAMESEARCH
{
	int					version;	// subject version
	E_SEARCH_TYPE		type;
	char				reguest[4]; // ST_REQUEST_SEARCH_IDLIST, ST_REQUEST_SEARCH_DATA
} ST_REQUEST_GEONAMESEARCH;

typedef struct tagREQUEST_SEARCH_IDLIST
{
	unsigned int		level;
	float				left;
	float				top;
	float				right;
	float				bottom;
	unsigned int		req_index;
	unsigned int		length;					// <= 254
	unsigned short		key_string[2];
} ST_REQUEST_SEARCH_IDLIST;

typedef struct tagRECORD_ID
{
	E_GDM_SUBDATA_TYPE		sub_type;
	int						id;	
} ST_RECORD_ID;

typedef struct tagREQUEST_SEARCH_DATA
{
	unsigned int		count;					// <= 254
	ST_RECORD_ID		id_list[1];
} ST_REQUEST_SEARCH_DATA;

typedef struct _tagREQUEST_GEODATA
{
	int					version;				// subject version
	unsigned int		level;
	unsigned int		column;					// x
	unsigned int		row;					// y
	E_GDM_MAINDATA_TYPE		main_type;
	E_GDM_SUBDATA_TYPE		sub_type;
} ST_REQUEST_GEODATA;
/***********************************************************************************/

/******************************* Response Structure *********************************/
/***********************************************************************************/
typedef struct tagRESPONSE_HEADER
{
	E_REQUEST_TYPE				type;		// request type
	E_RESPONSE_ERROR_TYPE		result;		// response error code
	unsigned char				body[4];	// ST_RESPONSE_GEONAMELIST, ST_RESPONSE_GEODATA, ST_RESPONSE_DBCOMMAND
} ST_RESPONSE_HEADER;

typedef struct tagST_GEODATA_NAME
{
	int					id;
	E_GDM_SUBDATA_TYPE	type;					// E_GDM_SUBDATA_TYPE
	float				longitude;
	float				latitude;
	float				height;
	float				view_height;
	unsigned int		length;					// <= 200
	unsigned int		length_ko;					// <= 200
	unsigned short		name[2];
} ST_GEODATA_NAME;

typedef struct tagRESPONSE_GEONAMESEARCH
{
	int					version;	// subject version
	E_SEARCH_TYPE		type;
	char				response[4]; // ST_RESPONSE_SEARCH_IDLIST, ST_GEODATA_NAMELIST
} ST_RESPONSE_GEONAMESEARCH;

typedef struct tagRESPONSE_SEARCH_IDLIST
{
	unsigned int		finished;		// != 0 : search finished, == 0 : search continue
	unsigned int		count;
	ST_RECORD_ID		id_list[1];
} ST_RESPONSE_SEARCH_IDLIST;

typedef struct tagGEODATA_NAMELIST
{
	unsigned int count;
	ST_GEODATA_NAME data_list[1];
} ST_GEODATA_NAMELIST;

typedef struct _tagRESPONSE_GEODATA
{
	int					version;	// subject version
	unsigned int		level;
	unsigned int		column;					// x
	unsigned int		row;					// y
	E_GDM_MAINDATA_TYPE		main_type;
	E_GDM_SUBDATA_TYPE		sub_type;
	unsigned char		data[4];				// ST_GEODATA_DEM, ST_GEODATA_TEXTURE, ST_GEODATA_SHAPELIST, ST_GEODATA_NAMELIST
} ST_RESPONSE_GEODATA;

typedef struct tagGEODATA_COMMON
{
	unsigned int		bytes;
	E_COMPRESS_TYPE		compress_type;
	unsigned char		buffer[4];				// height data,  jpg file, shape data, ST_GEODATA_NAMELIST
} ST_GEODATA_COMMON;

typedef ST_GEODATA_COMMON ST_GEODATA_DEM;	

typedef ST_GEODATA_COMMON ST_GEODATA_TEXTURE;

typedef ST_GEODATA_COMMON ST_GEODATA_SHAPE;
typedef ST_GEODATA_COMMON ST_GEODATA_3DMESH;

/***********************************************************************************/


// COMMAND TO DATABASE
////////////////////////////////////////////////////////////

typedef enum {
	E_DBCOMMAND_ADDNAME,
	E_DBCOMMAND_UPDATENAME,
	E_DBCOMMAND_DELETENAME,
} E_DBCOMMAND_TYPE;

typedef struct _tagREQUEST_DBCOMMAND
{
	int					version;				// subject version
	E_DBCOMMAND_TYPE	command_type;
	char				data[4];				// ST_ADDNAME_DATA, ST_UPDATENAME_DATA, ST_DELETENAME_DATA
} ST_REQUEST_DBCOMMAND;

typedef ST_GEODATA_NAME ST_ADDNAME_DATA;
typedef ST_GEODATA_NAME ST_UPDATENAME_DATA;
typedef ST_RECORD_ID ST_DELETENAME_DATA;


typedef struct _tagRESPONSE_DBCOMMAND
{
	int					version;				// subject version
	E_DBCOMMAND_TYPE	command_type;			
	int					success;				// 0 : fail, != 0 : added id
} ST_RESPONSE_DBCOMMAND;

////////////////////////////////////////////////////////////


//riym
/////////////////////////////////////////////////
typedef struct tagRENDER_REQUEST
{
	unsigned int		bytes;
	E_REQUEST_TYPE		type; // = REQUEST_TYPE_GEODATA
	ST_REQUEST_GEODATA	req;  // 	
} RENDER_REQUEST;

typedef struct tagRENDER_RESPONSE
{
	unsigned int				bytes;		// total byte of response data
	E_REQUEST_TYPE				type;		// request type
	E_RESPONSE_ERROR_TYPE		result;		// response error code
	unsigned char				body[4];	// ST_GEODATA_DEM, ST_GEODATA_SHAPELIST, ST_GEODATA_NAMELIST
} RENDER_RESPONSE;

/////////////////////////////////////////////////

#endif // _GDEM_PROTOCOL_H_
