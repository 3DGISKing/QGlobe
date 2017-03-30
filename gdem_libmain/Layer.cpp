#include "Layer.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

#include "../gds_database/GDSDatabaseInterface.h"
#include "../gds_database/GDSGeoNameDBManager.h"
#include "RasterStretchColorRampRenderer.h"
#include "../gds_common/GDSConfigMgr.h"

/*2013 2 27
ÂÙ±¶ shape À°ËÎÌ© ¾×´Ý¿Í±¨¶¦ 500kµá Ãú»þÂ×Êî³Þ.
ËË°ÃË¼ ·ôË§ ¹¦ÂÝ¶®ÂÙ ¸Ò¸âËË³Þ.
500kËËº¬´ó²÷ shapeÀ°ËÎËË ËØË¾ ÆÁ ÅüÂÙ ¸Æ°û°¡ ±ýÊò°ÃËË³Þ.
Ì¡ÂéË¼ GDSFileManager::ReadFileMapËæ ËØ³Þ.
ËË ÂÜºãÌ© ÁÄµ¹¸¡À¾ a_pBuffËæ²÷ memcpy¶¦ Ì¡ÂÙ ±¨ÊÍ°á°£ËË ¾©¹¤Â×¶®µ¹²÷ ÊÌÅîÂÙ ³è¸ó´ª ÊÖ³Þ.*/

unsigned int TerrainLayer::MaxTileSize     =   129*129*2;
unsigned int ImageLayer::MaxTileSize       =   256*256*3;
unsigned int PolylineLayer::MaxTileSize    =   500*1024;
unsigned int PlaceNameLayer::MaxTileSize   =   50000;
unsigned int ObjectLayer::MaxTileSize      =   1024*1024*20; 
unsigned int RasterLayer::MaxTileSize      =   256*256*4+72;

PlaceNameLayer::PlaceNameLayer(E_GDM_SUBDATA_TYPE subtype,QString name,QString iconfilename)
:Layer(E_GDM_MAINDATA_TYPE_NAME,subtype,name,iconfilename)
{
	_fontID=FSI_NORMAL;
	_iconSizeX=20;
	_iconSizeY=20;
	_color.setRgb(qRgb(255,255,0));
	_texture=NULL;
}

void PlaceNameLayer::SetProperty(QString tablename)
{
	if(!g_DBManager)
		return ;

	QSqlRecord record=g_DBManager->GetTableData(this->GetSubType());

	_minlevel=record.field("start_level").value().toUInt();	

	int fontid=record.field("font_id").value().toUInt();	

	if(fontid!=0)
	{
		Q_ASSERT(fontid>0 && fontid<FSI_END);
		_fontID=(_FONT_SIZE_ID)fontid;
	}

	_iconSizeX=_iconSizeY=record.field("icon_size").value().toUInt();	
	
	_iconfilename=record.field("icon_path").value().toString();

	if(_iconfilename!=NULL && _iconSizeX==0)
		qFatal("iconsize is not specified!");

	QRgb c=record.field("line_color").value().toUInt();

	if(c!=0)
		_color.setRgba(c);
}

RasterLayer::RasterLayer(E_GDM_SUBDATA_TYPE subtype,QString name,QString iconfilename)
  :Layer(E_GDM_MAINDATA_TYPE_RASTER,subtype,name,iconfilename)
{
	_nodatavalue=-9999;
	_renderer=new RasterStretchColorRampRender();

	_renderer->SetLayer(this);
}

RasterLayer::~RasterLayer()
{
	if(_renderer)
		delete _renderer;
}

#define RASTER_CONFIG_FILE_PATH "config.ini"

void RasterLayer::SetDataPath(QString path)
{
	Layer::SetDataPath(path);

	QString databasepath=g_ServerConfig.GetFileDBPath();
	
	QString filename(RASTER_CONFIG_FILE_PATH);

	path=databasepath+"/"+path+"/"+filename;

	if(!QFile::exists(path))
		return;

	QFile file(path);
	
	if(file.open(QFile::ReadOnly))
	{
		QTextStream inn(&file);

		while(!inn.atEnd())
		{
			QString line=inn.readLine();

			QStringList tokonlist=line.split(QRegExp("\\s+"));

			Q_ASSERT(tokonlist.size()==2);

            QString val=tokonlist[1];

			if (tokonlist[0]== "NoDataValue")
				_nodatavalue=val.toDouble();
			else if(tokonlist[0]== "MaxValue")
				_statistics._maxvalue=val.toDouble();
			else if(tokonlist[0]== "MinValue")
				_statistics._minvalue=val.toDouble();
			else if(tokonlist[0]== "MeanValue")
				_statistics._meanvalue=val.toDouble();
			else if(tokonlist[0]== "StdDev")
				_statistics._stddev=val.toDouble();
			else if(tokonlist[0]== "UnitDescription")
				SetUnitDescription(val);
			else
				qFatal("Unkonwn Token");
		}
	}
	file.close();
}
