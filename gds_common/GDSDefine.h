#ifndef __GDSDEFINE_H__
#define __GDSDEFINE_H__

/*************************************************
2013 3 6 ugi
3dÀ°ËÎÌ© ¿Í±¨°¡ ¿Í±¨ÆÁ·ÍËæ ¾×´Ýµá ËÏË¾ºã ËØ²÷ À°ËÎÌ© ¿Í±¨¶¦ ºã¼³Â×¼è Ê±Ëº·² Ê¯´ô³Þ
GDS_MAX_TRANSFER_SIZE²÷ MAX_TILE_SIZE_3DÌ® ¿Í±¨°¡ °¯»¡³£³Þ.
**************************************************/
#ifndef GDS_MAX_TRANSFER_SIZE
#define GDS_MAX_TRANSFER_SIZE	(1024*1024*20)	// 10MB
//#define GDS_MAX_TRANSFER_SIZE	(150*1024)	    // 150KB
#endif
typedef enum E_LOG_OUTPUT_TYPE
{
	E_LOG_OUTPUT_NONE = 0,
	E_LOG_OUTPUT_FILE,
	E_LOG_OUTPUT_QDEBUG,
};

typedef enum E_LOG_OUTPUT_LEVEL
{
	E_LOG_LEVEL_0 = 0,				 // only error log
	E_LOG_LEVEL_1,				     // error and general
};

typedef enum E_LOG_LEVEL
{
	E_LOG_ERROR,				   // only error log
	E_LOG_TRACK,				   // error and general
};

#define DEM_BLOCKFILE_PATH_FMT					"/%d/%d_%d_%d.gd"
#define TEXTURE_BLOCKFILE_PATH_FMT				"/%d/%d_%d_%d.gt"
#define SHAPE_COUNTRY_BLOCKFILE_PATH_FMT		"/%d/%d_%d_%d.gs"
#define MESH_BUILDING_FILE_PATH_FMT             "/geoobject/building/complex/geomesh/%d/%d_%d_%d/%d_%d_%d.geomeshs"
#define SIMPLEBUILDING_FILE_PATH                "/geoobject/building/simple/%d/%d_%d_%d/%d_%d_%d.shp"
#define RASTER_FILE_PATH_FMT					"/%d/%d_%d_%d/%d_%d_%d.ras"
#define RASTER_FILE_PATH_FMTEX					"/%d/%d_%d_%d.ras"

#define TILE_ROW_COUNT_IN_BLOCK			128

#define MIN_LEVEL					    0
#define MAX_LEVEL					    18

#define DEM_MAX_LEVEL			        13
#define TEXTURE_MAX_LEVEL               19

#define SHAPE_MIN_LEVEL		            4
#define SHAPE_MAX_LEVEL                 17


#define THREED_MIN_LEVEL                15
#define THREED_MAX_LEVEL				18

#define SHAPE_COUNTRY_MAX_LEVEL         18
#define SHAPE_FIRSTADMIN_MAX_LEVEL      14
#define SHAPE_FIRSTADMIN_MIN_LEVEL      5
#define SHAPE_SECONDADMIN_MAX_LEVEL     18
#define SHAPE_SECONDADMIN_MIN_LEVEL     9
#define SHAPE_THIRDADMIN_MAX_LEVEL      18
#define SHAPE_THIRDADMIN_MIN_LEVEL      11

#define SHAPE_HIGHWAY_MAX_LEVEL         18
#define SHAPE_HIGHWAY_MIN_LEVEL         8
#define SHAPE_1STLOAD_MAX_LEVEL         18
#define SHAPE_1STLOAD_MIN_LEVEL         8

#define SHAPE_2STLOAD_MAX_LEVEL         18
#define SHAPE_2STLOAD_MIN_LEVEL         8

#define SHAPE_3STLOAD_MAX_LEVEL         18
#define SHAPE_3STLOAD_MIN_LEVEL         10

#define SHAPE_4STLOAD_MAX_LEVEL         18
#define SHAPE_4STLOAD_MIN_LEVEL         12

#define SHAPE_5STLOAD_MAX_LEVEL         18
#define SHAPE_5STLOAD_MIN_LEVEL         13

#define SHAPE_WIDERAILWAY_MAX_LEVEL     18
#define SHAPE_WIDERAILWAY_MIN_LEVEL     7
#define SHAPE_NARROWRAILWAY_MAX_LEVEL   18
#define SHAPE_NARROWRAILWAY_MIN_LEVEL   7


#define SHAPE_WIDERIVER_MAX_LEVEL       18

#define SHAPE_RIVER_MAX_LEVEL           18
#define SHAPE_LAKE_MAX_LEVEL            18

#define SHAPE_FARMLAND_MAX_LEVEL        18
#define SHAPE_FARMLAND_MIN_LEVEL        14

#define SHAPE_NONFARMLAND_MAX_LEVEL     18
#define SHAPE_NONFARMLAND_MIN_LEVEL     14

#define SHAPE_FORESTLAND_MAX_LEVEL      18
#define SHAPE_FORESTLAND_MIN_LEVEL      14

#define GEONAME_MAX_COUNT_IN_TILE		200

#define INVALID_GDSFILEHANDLE -1

#endif // __GDSDEFINE_H__