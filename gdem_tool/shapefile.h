#pragma once

#define SHAPE_FILE_SUFFIX            ".shp"
#define INDEX_FILE_SUFFIX            ".shx"

#define SHP_FILE_HEADER_SIZE         100
#define	SHP_FILE_CODE			     9994
#define	SHP_FILE_VER			     1000

#define	NULL_SHAPE					 0
#define	POINT_SHAPE			    	 1
#define	POLYLINE_SHAPE				 3
#define	POLYGON_SHAPE				 5
#define	MULTIPOINT_SHAPE			 8
#define	POINT_Z_SHAPE				 11
#define	POLYLINE_Z_SAHPE			 13
#define	POLYGON_Z_SHAPE		    	 15
#define	MULTIPOINT_Z_SHAPE			 18
#define	POINT_M_SHAPE				 21
#define	POLYLINE_M_SHAPE			 23
#define	POLYGON_M_SHAPE		    	 25
#define	MULTIPOINT_M_SHAPE			 28
#define	MULTIPATCH_SHAPE			 31

#define SHP_FILE_RECORD_HEADER_SIZE  12

#define SHX_FILE_RECORD_SIZE         8
#define ARC_POINT_SIZE               16
#define ARC_POLYLINE_HEADER_SIZE     40

#pragma pack(1)

struct ShpFileHeader {          	//  Byte Order 
	long	_FileCode;				//  Big 
	long	_UnUsed[5];		    	//  Big 
	long    _FileLength;        	//  Big
	long	_Version;		    	//  Little
	long	_ShapeType;         	//  Little
	double  _BoundingBoxMinX;   	//  Little
	double  _BoundingBoxMinY;   	//  Little
	double  _BoundingBoxMaxX;   	//  Little
	double  _BoundingBoxMaxY;   	//  Little
	double  _BoundingBoxMinZ;   	//  Little
	double	_BoundingBoxMaxZ;   	//  Little
	double	_BoundingBoxMinM;   	//  Little
	double	_BoundingBoxMaxM;   	//  Little
};

struct ShpFileRecordHeader
{
	long _RecordNumber;
	long _ContentLength;
	long _ShapeType;
};

struct ShxFileRecord
{
	long _Offset;
	long _ContentLength;
};

struct ArcPoint
{
	double _X;
	double _Y;
};

struct ArcMultiPoint
{
	double _BoundingBoxMaxX;
	double _BoundingBoxMinX;
	double _BoundingBoxMaxY;
	double _BoundingBoxMinY;
	long   _NumPoints;
	ArcPoint* _pPoints;    //The size of array referred by _pPoints will be _NumPoints  
};

struct ArcPolyLineHeader
{
	double _BoundingBoxMinX;
	double _BoundingBoxMinY;
	double _BoundingBoxMaxX;
	double _BoundingBoxMaxY;
	long   _NumParts;
	long   _NumPoints;
};

#pragma  pack()

inline long BigToLittle(long big)
{
	unsigned long little;

	little = 0x00000000;
	little = little | ((big >> 24) & 0x000000FF);  //                         11111111
	little = little | ((big >> 8)  & 0x0000FF00);  //                 1111111100000000
	little = little | ((big << 8)  & 0x00FF0000);  //         111111110000000000000000
	little = little | ((big << 24) & 0xFF000000);  // 11111111000000000000000000000000

	return (long)little;
}
