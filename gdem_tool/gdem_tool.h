#ifndef GDEM_TOOL_H
#define GDEM_TOOL_H

#include <QtGui/QMainWindow>
#include <QSplitter>
#include "ui_gdem_tool.h"

class GDemDbExtractDlg;
class SmpBuildingProDlg;
class HopeDBToGDem;
class WriteRemoveJGW;
class LatLongToHopeImage;
class ImageMerge;
class TileLatLong;
class Browser;
class VectorConverter;
class ShapeDraw;
class KeyGenerator;
class ImgTileMergeDlg;

class gdem_tool : public QMainWindow
{
	Q_OBJECT

public:
	gdem_tool(QWidget *parent = 0, Qt::WFlags flags = 0);
	~gdem_tool();

private:
	Ui::gdem_toolClass ui;

	QSplitter*         _splitter;
	GDemDbExtractDlg*  _gdemExtractDlg;
	SmpBuildingProDlg* _smpBuildingProDlg;
	HopeDBToGDem*      _hopeDBtoGDem;
	WriteRemoveJGW*    _writeRemoveJgw;
	LatLongToHopeImage*_latlongToImage;
	ImageMerge*        _imageMerge;
	TileLatLong*       _tileLatLong;
	Browser*           _browser;
	VectorConverter*   _geoshapeConverter;   
	ShapeDraw*         _shapeToKml;
	KeyGenerator*      _KeyGenerator;
	ImgTileMergeDlg*    _imgTileMergeDlg;

private slots:
	void showGDemDbExtractDlg();
	void showSmpBuildingProDlg();
	void showHopeDBtoGDem();
	void showWriteRemoveJgw();
	void showLatLongToImage();
	void showImageMerge();
	void showTileLatLong();
	void showBrowser();
	void showVectorConverter();
	void showKMLMaker();
	void showKeyGenerator();
	void showImgTileMergeDlg();
	void onExit();
};

#endif // GDEM_TOOL_H
