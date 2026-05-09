#ifndef QGLOBE_TOOL_H
#define QGLOBE_TOOL_H

#include <QMainWindow>
#include <QSplitter>
#include "ui_qglobe_tool.h"

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

class qglobe_tool : public QMainWindow
{
	Q_OBJECT

public:
	qglobe_tool(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	~qglobe_tool();

private:
	Ui::qglobe_toolClass ui;

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

#endif // QGLOBE_TOOL_H
