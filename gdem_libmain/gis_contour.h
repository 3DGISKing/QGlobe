#ifndef __GIS_CONTOUR_H__
#define __GIS_CONTOUR_H__

#define FUDGE_EXACT	0.001
#define JOIN_DIST	0.0001

#define SKIP_POINTS_11		120
#define SKIP_POINTS_12		40
#define CHECK_POINTS	3
#define PERM_THRESHOLD	0.1

#define MIN(a,b)	((a<b) ? a : b)
#define MAX(a,b)	((a>b) ? a : b)

typedef enum
{
    CTE_NONE	= 0,
    CTE_DEBUG	= 1,
    CTE_WARNING	= 2,
    CTE_FAILURE	= 3,
    CTE_FATAL	= 4

} CT_ERR;

typedef struct _gis_point
{
	float x;
	float y;
	float z;
}GIS_POINT;

class GDALContourItem
{
public:
    int    bRecentlyAccessed;
    double dfLevel;

    int  nPoints;
    int  nMaxPoints;
    double *padfX;		
    double *padfY;
	double *padfAngle;

    double dfTailX;

	GIS_POINT *padPt;

    GDALContourItem();
    ~GDALContourItem();

    int    AddSegment( double dfXStart, double dfYStart,
                       double dfXEnd, double dfYEnd );
    void   MakeRoomFor( int );
    int    Merge( GDALContourItem * );
	void   GenerateLabelInfo (int bCalc, int nLevel);
};

//#define 	MAX_CONTOUR_ITEM_COUNT	500
#define MAX_CONTOUR_POLYGON_COUNT 800

class GDALContour
{
public:
	GDALContour();
	~GDALContour();
public:
	int					nCount;
	GDALContourItem		items[MAX_CONTOUR_POLYGON_COUNT];

	double				minLongitude;
	double				maxLongitude;
	double				minLatitude;
	double				maxLatitude;
	double				gridWidth;
};

class GDALContourLevel 
{
    double dfLevel;

    int nEntryMax;
    int nEntryCount;
    GDALContourItem **papoEntries;
    
public:
    GDALContourLevel( double );
    ~GDALContourLevel();

    double GetLevel() { return dfLevel; }
    int    GetContourCount() { return nEntryCount; }
    GDALContourItem *GetContour( int i) { return papoEntries[i]; }
    void   AdjustContour( int );
    void   RemoveContour( int );
    int    FindContour( double dfX, double dfY );
    int    InsertContour( GDALContourItem * );
};

class GDALContourGenerator
{
    int    nWidth;
    int    nHeight;
    int    iLine;

    double *padfLastLine;
    double *padfThisLine;

    int    nLevelMax;
    int    nLevelCount;

    int    iLastLevel;
    
    int     bNoDataActive;
    double  dfNoDataValue;

    int     bFixedLevels;
    double  dfContourInterval;
    double  dfContourOffset;

    CT_ERR AddSegment( double dfLevel, 
                       double dfXStart, double dfYStart,
                       double dfXEnd, double dfYEnd );

    CT_ERR ProcessPixel( int iPixel );
    CT_ERR ProcessRect( double, double, double, 
                        double, double, double, 
                        double, double, double,
                        double, double, double );

    void   Intersect( double, double, double, 
                      double, double, double, 
                      double, double, int *, double *, double * );

    GDALContourLevel *FindLevel( double dfLevel );

public:
	GDALContourLevel **papoLevels;

    GDALContourGenerator( int nWidth, int nHeight );
    ~GDALContourGenerator();

    void	SetNoData( double dfNoDataValue );
    void	SetContourLevels( double dfContourInterval, double dfContourOffset = 0.0 ) { 
				this->dfContourInterval = dfContourInterval; this->dfContourOffset = dfContourOffset; }
    void	SetFixedLevels( int, double * );
    CT_ERR 	FeedLine( double *padfScanline, GDALContour *pContour );
    CT_ERR  EjectContours( int bOnlyUnused, GDALContour *pContour );
	int GetLevelCount() {return nLevelCount;}
};

#endif /* __GIS_CONTOUR_H__ */