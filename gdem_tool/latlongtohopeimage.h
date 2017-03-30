#ifndef LATLONGTOHOPEIMAGE_H
#define LATLONGTOHOPEIMAGE_H

#include <QDialog>
#include "ui_latlongtohopeimage.h"
#include "common.h"


class LatLongToHopeImage : public QDialog
{
	Q_OBJECT

public:
	LatLongToHopeImage(QWidget *parent = 0);
	~LatLongToHopeImage();

private:
	void getTotalRectofXYsInLevel(double left,double right,double top,double bottom,int level);
	void HopeImgMake(int x, int y,int level,QRectF totalRect);
	QRect getPixRect_InTotal(QRectF totalRect,QRectF workRect);
	void loadPixelData(QImage *srcImg, QRect areRect,int x,int y,int level);
	QRect getPixRect_InTotalEx(QRectF totalRect,QRectF workRect);
	void loadPixelDataEx(QRectF workRect,QRect areRect,int x,int y,int level);
	bool imgTestinvalid(QImage img);

	/*double CalcFromLatLongtoXY(double latlong,int level);
	double CalcFromXYtoLongLat(int x,int level);*/
	void getSourceFiles();
	void getSourceFolder();
	void ConvertFile(QString imgPath);
	void ConvertFolder();
	void writeFileInfoData(QString strLeft,	QString strTop,	QString strRight,QString strBottom);
	
	void WriteJgwFile(int x,int y,int level,QString filepath);
	bool JGWRead(QString imgPath, double*left, double*top, double*xResolution, double*yResolution);
	int GetNumberOfPixelBetweenTwoPts(double startCoord, double endCoord, double resolution);
	void RegisterInformation();
	void MakeMarginImages(QImage*srcImg, int level);	
	void WriteMarginImageInfo(QString imgPath, double left, double top, double pixWidth);

private slots:
	void getSource();
	void getDestinateFolder();
	void getImgQuality();
	void Convert();
	void getIgnoreColor();
	void setDirectorySelect();
	void setJGW_Append();
	void setTmpDir();
	void ProcessingMarginImages();
private:
	Ui::LatLongToHopeImageClass ui;

	bool m_dirSelect;
	bool m_writeJGW;
	QString m_tmpDir;

	QStringList fileNameListInDir;
	QString m_srcImgPath;
	QImage jpgImg;
	double _Left,_Right,_Top,_Bottom;
	double _destLeft,_destRight,_destTop,_destBottom;
	double _xSolution,_ySolution;
	int m_startLevel,m_endLevel;
	int totalJpgW,totalJpgH;
	rect_XY m_rectXYInlevel;

};

#endif // LATLONGTOHOPEIMAGE_H
