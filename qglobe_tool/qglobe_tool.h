#ifndef QGLOBE_TOOL_H
#define QGLOBE_TOOL_H

#include <QMainWindow>
#include <QSplitter>
#include "ui_qglobe_tool.h"

class QGlobeDbExtractDlg;
class SmpBuildingProDlg;
class HopeDBToQGlobe;
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
	QGlobeDbExtractDlg*  _qglobeExtractDlg;
	SmpBuildingProDlg* _smpBuildingProDlg;
	HopeDBToQGlobe*      _hopeDBtoQGlobe;
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
	void showQGlobeDbExtractDlg();
	void showSmpBuildingProDlg();
	void showHopeDBtoQGlobe();
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
