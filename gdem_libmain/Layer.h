#ifndef LAYER_H
#define LAYER_H

#include <QtCore/QString>
#include <QtGui/QColor>

#include "../gdem_common/gdem_protocol.h"
#include "../gdem_common/gdemclientbase.h"
#include "../gds_common/GDSDefine.h"

//this class represents general layer displayed in layerdlg

class Layer  
{
public:
	Layer(E_GDM_MAINDATA_TYPE maintype,E_GDM_SUBDATA_TYPE subtype,QString name, QString iconfilename="")
	{
		_mainType=maintype;
		_subType=subtype;
		_name=name;
		_iconfilename=iconfilename;
		_visible=false;
		_minlevel=MIN_LEVEL;
		_maxlevel=MAX_LEVEL;
	}

	virtual ~Layer()
	{

	}

	inline QString              GetName()         {return _name;}

	inline QString              GetIconFileName() {return _iconfilename;}

	inline bool                 GetVisible()      {return _visible;}
	inline void                 SetVisible(bool visible) {_visible=visible;}

	inline E_GDM_MAINDATA_TYPE  GetMainType()     {return _mainType;}
	inline E_GDM_SUBDATA_TYPE   GetSubType()      {return _subType;}

	inline unsigned int         GetMaxLevel()     {return _maxlevel;}
	inline unsigned int         GetMinLevel()     {return _minlevel;}
	inline void                 SetMaxLevel(unsigned int level){_maxlevel=level;}
	inline void                 SetMinLevel(unsigned int level){_minlevel=level;}

	inline QString              GetDataPath()      {return _dataPath;}
	virtual void                SetDataPath(QString path) {_dataPath=path;}
	inline void                 SetLevel(unsigned int min,unsigned int max)
	{
		_minlevel=min;
		_maxlevel=max;
	}

	inline bool                 IsTerrainLayer() {return _mainType==E_GDM_MAINDATA_TYPE_DEM;}
	inline bool                 IsImageLayer() {return _mainType==E_GDM_MAINDATA_TYPE_IMG;}
	inline bool                 IsPlaceNameLayer() {return _mainType==E_GDM_MAINDATA_TYPE_NAME;}
	inline bool                 IsRasterLayer() {return _mainType==E_GDM_MAINDATA_TYPE_RASTER;}

protected:
	QString             _name;
    QString             _iconfilename;  //absolute path
  
	E_GDM_MAINDATA_TYPE _mainType;
	E_GDM_SUBDATA_TYPE  _subType;
	QString             _dataPath;
	unsigned int        _minlevel;
	unsigned int        _maxlevel;
	bool                _visible;
};

class TerrainLayer:public Layer
{
public:
	TerrainLayer(E_GDM_SUBDATA_TYPE subtype,QString name,QString iconfilename="")
		:Layer(E_GDM_MAINDATA_TYPE_DEM,subtype,name,iconfilename)
	{
		_maxlevel=DEM_MAX_LEVEL;
	}

	static unsigned int         MaxTileSize;
};

class ImageLayer :public Layer
{
public:
	ImageLayer(E_GDM_SUBDATA_TYPE subtype,QString name,QString iconfilename="")
		:Layer(E_GDM_MAINDATA_TYPE_IMG,subtype,name,iconfilename)
	{
		_maxlevel=TEXTURE_MAX_LEVEL;
	}
	static unsigned int         MaxTileSize;
};

class PlaceNameLayer :public Layer
{
public:
	PlaceNameLayer(E_GDM_SUBDATA_TYPE subtype,QString name,QString iconfilename="");
	
	void SetProperty(QString tablename);

	inline QColor        GetColor()                     {return _color;};
	inline void          SetTexture(void* p)            {_texture=p;}
	inline void*         GetTexture()                   {return _texture;}
	inline void          SetFontSizeID(_FONT_SIZE_ID id) {_fontID=id;}
	inline _FONT_SIZE_ID GetFontSizeID()                 {return _fontID;}

	inline int           SetIconSize(int x,int y)        {_iconSizeX=x;_iconSizeY=y;}
	inline int           GetIconSizeX()                  {return _iconSizeX;}
	inline int           GetIconSizeY()                  {return _iconSizeY;}

	static unsigned int         MaxTileSize;
private:
	_FONT_SIZE_ID        _fontID;
	int                  _iconSizeX,_iconSizeY;
	QColor               _color;

	void*                _texture;
};

class PolylineLayer: public Layer
{
public:
	PolylineLayer(E_GDM_SUBDATA_TYPE subtype,QString name,QString iconfilename="")
		:Layer(E_GDM_MAINDATA_TYPE_SHAPE,subtype,name,iconfilename)
	{
	}

	static unsigned int         MaxTileSize;
};

class ObjectLayer: public Layer
{
public:
	ObjectLayer(E_GDM_SUBDATA_TYPE subtype,QString name,QString iconfilename="")
		:Layer(E_GDM_MAINDATA_TYPE_3D,subtype,name,iconfilename)
	{
		_minlevel=THREED_MIN_LEVEL;
		_maxlevel=THREED_MAX_LEVEL;
	}

	static unsigned int         MaxTileSize;
};

struct GDemRasterBandStatistics
{
	GDemRasterBandStatistics()
	{
		_minvalue=0.0;
		_maxvalue=0.0;
		_meanvalue=0.0;
		_stddev=0.0;
		_valid=false;
	};

	double _maxvalue;
	double _minvalue;
	double _meanvalue;
	double _stddev;
	bool _valid;
};

class IRasterRenderer;
class RasterLayer: public Layer
{
public:
	RasterLayer(E_GDM_SUBDATA_TYPE subtype,QString name,QString iconfilename="");
	~RasterLayer();

	inline  GDemRasterBandStatistics GetStatistics()              {return _statistics;}
	inline  IRasterRenderer*          GetRenderer()                {return _renderer;}
	inline  double                    GetNoDataValue()             {return _nodatavalue;} 
	virtual void                      SetDataPath(QString path);
	inline  QString                   GetUnitDescription()          {return _unitDescription;}
	inline  void                      SetUnitDescription(QString s) {_unitDescription=s;}

	static unsigned int         MaxTileSize;
private:

	GDemRasterBandStatistics _statistics;
	IRasterRenderer*          _renderer;
	double                    _nodatavalue;

	QString                   _unitDescription;
};


#endif