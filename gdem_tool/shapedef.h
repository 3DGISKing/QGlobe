//ShapeDef.h
//2011 3 13 created by ugi
#include "QList"
#include "../gdem_base/GDM_Vector3D.h"

#include <QVector>
#include "../gdem_base/GDM_Rect.h"

//added by RSH 2012.5.6
#include <QStringList>

#define		AV_NULL_SHP				0
#define		AV_POINT_SHP			1
#define		AV_POLYLINE_SHP			3
#define		AV_POLYGON_SHP			5
#define		AV_MULTIPOINT_SHP		8
#define		AV_POINT_Z_SHP			11
#define		AV_POLYLINE_Z_SHP		13
#define		AV_POLYGON_Z_SHP		15
#define		AV_MULTIPOINT_Z_SHP		18
#define		AV_POINT_M_SHP			21
#define		AV_POLYLINE_M_SHP		23
#define		AV_POLYGON_M_SHP		25
#define		AV_MULTIPOINT_M_SHP		28
#define		AV_MULTIPATCH_SHP		31

#define		AV_FILE_CODE			9994
#define		AV_FILE_VER			    1000
#define		AV_FILE_HDR_SIZE		100

struct AVF_HDR	{
	long	nFileCode;			//  Big endian 
	long	nReserved[5];		//  Big endian 
	long	nFileLength;		//  Big endian 
	long	nVer;				
	long	nShapeType;
	double  Xmin;
	double  Ymin;
	double  Xmax;
	double  Ymax;
	double  Zmin;
	double	Zmax;
	double	Mmin;
	double	Mmax;
};

struct AV_DATA_REC_HDR	{
	long	nRecNum;			//  Big endian 
	long	nContentLength;		//  Big endian 
	long	nShapeType;
};   

struct AV_POINT {	
	double	x;
	double	y;
};	
struct INT_POINT {
	int x;
	int y;
};
struct AV_POLY_HDR	{	
	double	BXmin;
	double  BYmin;
	double  BXmax;
	double  BYmax;
	long	nPartCount;
	long	nPointCount;
};	

struct AV_POLYGON	{
	struct AV_POLY_HDR	polyHd;
	//long				*nFirstPtIdx;		// length = nPartCount
	struct AV_POINT		*pPoints;			// length = nPointCount
};
//end of RSH
class Point
{
	double _X;
	double _Y;
public:
	Point()                  {_X=0;_Y=0;}
	void   Set (double x,double y)  {_X=x;_Y=y;}
	void   SetX(double x)           {_X=x;}
	void   SetY(double y)           {_Y=y;}
	double X()                      {return _X;}
	double Y()                      {return _Y;}; 
};

class Line
{
	QList<Point> _PointList;
public:
	Line()                          {_PointList.clear();}
	~Line()                         {_PointList.clear();}
	void         AddPoint(Point p)  {_PointList.append(p);}
	void         Clear()            {_PointList.clear();}
	Point        GetPoint(int index){return _PointList[index];}
	unsigned int GetPointCount()    {return (unsigned int)_PointList.size();}
};

class PolyLine
{
	QList<Line> _LineList;
public:
	void         AddLine(Line l)               {_LineList.append(l);}
	void         AddPolyline(PolyLine polyline)
	{
		Line line;
		for(unsigned int i=0;i<polyline.GetLineCount();i++)
		{
			line=polyline.GetLine(i);
			_LineList.append(line);
		}
	}
	void         Clear()					{_LineList.clear();}
	unsigned int GetLineCount()				{return (unsigned int)_LineList.count();}
	Line         GetLine(unsigned int index){return _LineList[index];}
	bool         IsEmpty()		            {return _LineList.empty();}
};


//added by RSH 2013.3.3 for bounding box inclusion
void GetBoundingRect(CGDM_LocationArr ptArr, CGDM_Rect2D *bound);
//end addition by RSH

bool ReadPolylineFromShapeFile(QString filename,PolyLine& polyline);
bool WriteShapeFile(QString filename,PolyLine polyline, bool hasID);
bool WriteShapeFile(int x,int y,int level,CGDM_LocationPolygonArr polygonarray, bool hasID);
bool ReadShapeFile(QString filename,CGDM_LocationPolygonArr& linearray);
//added by RSH 2012.5.6
int GetShapeFileType(QString srcFileName);
long _ConvertBigToLittle(long op);
void ReadDataHeader(FILE * i_pfi, struct AVF_HDR * avHdr);
void ReadAvPolyHdr(FILE * i_pfi,struct AV_POLY_HDR * hdr);
void ReadAvPoint(FILE * i_pfi, struct AV_POINT * pt);
void ReadAvDataRecHdr(FILE * i_pfi,struct AV_DATA_REC_HDR * avDRhdr);

QVector<AV_POINT> GetPointFileData(FILE * i_pfi, struct AVF_HDR * hd);
QVector<AV_POLYGON> GetPolyFileData(FILE * i_pfi, struct AVF_HDR * hd);

//Shape ÃùËÎµá¹¢À¾ À°ËÎ»ôµê´ÉË¾ 
void ConvertShapeFile(QString srcFileName, QString dstDirStr, int level, bool hasID, int tolPixNum, int maxFSize, int maxNum);
bool ConvertShapeToGS(QString shpFileDir, QString saveDir);

bool isIntersecting(double Xmin, double Xmax, double Ymin, double Ymax, double minX, double maxX, double minY, double maxY);
QString MakeFileName(int nX, int nY, int level, QString extension);
void ProcessPolyData(QVector<AV_POLYGON> pPolyData, struct AVF_HDR * hd, QString dstDir, int level, 
					 bool hasID, int tolPixNum, int maxFSize, int maxFeatureNum);
void PostProcessPolyData(QVector<AV_POLYGON> pPolyData);
bool WriteShapeFile(int x,int y,int level, CGDM_LocationPolygonArr polygonarray, QString dstDir,QVector<int> idArr, 
					bool hasID, int maxFSize);
CGDM_LocationPolygonArr GetReducedPolygonArray(CGDM_LocationPolygonArr inArray, int level);

bool WriteOneBlockShapeFile(QString srcShpDirStr, QStringList srcShpFileList, QString saveDirStr);
QString getDirName(QString filename);
//end of RSH