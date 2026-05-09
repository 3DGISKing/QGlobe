#include "qglobe_tool.h"
#include "qglobedbextractdlg.h"
#include "smpbuildingprodlg.h"
#include "hopedbtogdem.h"
#include "writeremovejgw.h"
#include "latlongtohopeimage.h"
#include "imagemerge.h"
#include "tilelatlong.h"
#include "browser.h"
#include "vectorconverter.h"
#include "shapedraw.h"
#include "keygenerator.h"
#include "imgtilemergedlg.h"

qglobe_tool::qglobe_tool(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	connect(ui.actionShowGDemExtractDlg,SIGNAL(triggered()),this,SLOT(showGDemDbExtractDlg()));
	connect(ui.actionShowSimpleBuildingProDlg,SIGNAL(triggered()),this,SLOT(showSmpBuildingProDlg()));
	connect(ui.actionHopeDBToGDem,SIGNAL(triggered()),this,SLOT(showHopeDBtoGDem()));
	connect(ui.actionWriteRemoveJgw,SIGNAL(triggered()),this,SLOT(showWriteRemoveJgw()));
	connect(ui.actionLatLongToImage,SIGNAL(triggered()),this,SLOT(showLatLongToImage()));
	connect(ui.actionImageMerge,SIGNAL(triggered()),this,SLOT(showImageMerge()));
	connect(ui.actionTileLatLong,SIGNAL(triggered()),this,SLOT(showTileLatLong()));
	connect(ui.actionSqlBrowser,SIGNAL(triggered()),this,SLOT(showBrowser()));
	connect(ui.actionGeoShape,SIGNAL(triggered()),this,SLOT(showVectorConverter()));
	connect(ui.actionKMLMaker,SIGNAL(triggered()),this,SLOT(showKMLMaker()));
	connect(ui.actionKeyGenerator,SIGNAL(triggered()),this,SLOT(showKeyGenerator()));
	connect(ui.actionMergeJpgFiles,SIGNAL(triggered()),this,SLOT(showImgTileMergeDlg()));
	connect(ui.actionExit,SIGNAL(triggered()),this,SLOT(onExit()));

	_gdemExtractDlg=NULL;
	_smpBuildingProDlg=NULL;
	_hopeDBtoGDem=NULL;
	_writeRemoveJgw=NULL;
	_latlongToImage=NULL;
	_imageMerge=NULL;
	_tileLatLong=NULL;
	_browser=NULL;
	_geoshapeConverter=NULL;
	_shapeToKml=NULL;
	_KeyGenerator=NULL;
	_imgTileMergeDlg=NULL;
	setMaximumSize(16777215,32);
}

qglobe_tool::~qglobe_tool()
{

}

void qglobe_tool::showGDemDbExtractDlg()
{
	if(!_gdemExtractDlg)
		_gdemExtractDlg=new GDemDbExtractDlg(this);
	_gdemExtractDlg->show();
}

void qglobe_tool::showSmpBuildingProDlg()
{
	if(!_smpBuildingProDlg)
		_smpBuildingProDlg=new SmpBuildingProDlg(this);
	_smpBuildingProDlg->show();
}

void qglobe_tool::showHopeDBtoGDem()
{
	if(!_hopeDBtoGDem)
		_hopeDBtoGDem=new HopeDBToGDem(this);
	_hopeDBtoGDem->show();
}

void qglobe_tool::showWriteRemoveJgw()
{
	if(!_writeRemoveJgw)
		_writeRemoveJgw=new WriteRemoveJGW(this);

	_writeRemoveJgw->show();
}

void qglobe_tool::showLatLongToImage()
{
	if(!_latlongToImage)
		_latlongToImage=new LatLongToHopeImage(this);

	_latlongToImage->show();
}

void qglobe_tool::showImageMerge()
{
	if(!_imageMerge)
		_imageMerge=new ImageMerge(this);

	_imageMerge->show();
}

void qglobe_tool::showTileLatLong()
{
	if(!_tileLatLong)
		_tileLatLong=new TileLatLong(this);

	_tileLatLong->show();
}

void qglobe_tool::showBrowser()
{
	if(!_browser)
		_browser=new Browser();

	_browser->show();
}

void qglobe_tool::showVectorConverter()
{
	if (!_geoshapeConverter)
		_geoshapeConverter = new VectorConverter(this);
	
	_geoshapeConverter->show();
};

void qglobe_tool::showKMLMaker()
{
	if (!_shapeToKml) 
		_shapeToKml = new ShapeDraw(this);

	_shapeToKml->show();
}

void qglobe_tool::showKeyGenerator()
{
	if (!_KeyGenerator) 
		_KeyGenerator = new KeyGenerator(this);

	_KeyGenerator->show();
}

void qglobe_tool::showImgTileMergeDlg()
{
	if(!_imgTileMergeDlg)
		_imgTileMergeDlg=new ImgTileMergeDlg(this);

	_imgTileMergeDlg->show();
}

void qglobe_tool::onExit()
{
	qApp->quit();
}