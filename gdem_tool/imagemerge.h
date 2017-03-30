#ifndef IMAGEMERGE_H
#define IMAGEMERGE_H

#include <QDialog>
#include "ui_imagemerge.h"
#include "common.h"

class ImageMerge : public QDialog
{
	Q_OBJECT

public:
	ImageMerge(QWidget *parent = 0);
	~ImageMerge();
	void Init();

private:
	Ui::ImageMergeClass ui;

	double _Left,_Right,_Top,_Bottom;
	double _destLeft,_destRight,_destTop,_destBottom;
	double _xSolution,_ySolution;
	int _Level;
	int totalJpgW,totalJpgH;
	rect_XY m_rectXYInlevel;
	bool m_blWriteJgw;
	bool m_blDatabaseUpgrage;
	bool m_blUseLowerImage;
	int  m_imageSize;
	int m_opType;
	bool    m_blStop;
	QStringList jpgfiles;
	QList<QRectF>	GetRectsInTotal(int size,QRectF rect);	
	QStringList		GetJpgfilesInRect(QRectF rect, QStringList list);
	QStringList		GetJpgFilesWithinUseLowerlevel(QRectF rect);
	QString			GetElementFile(QString str);
	void	HopeImgMake(QRectF rect,QStringList &list);
	void	WriteJgw(int x,int y,int level,int imgSize,QString filepath);
	QRect	getPixRect_InTotal(QRectF totalRect,QRectF workRect,int level);
	void	CopyFilesFromSourceToTmpFolder(QStringList lst);
	bool	makeFileTotmpFolderUsingUpperLevel(int x,int y,int level);
	void	DeleteFilesInTmpFolder(QStringList lst);
	void	saveJpg( QImage saveimg,QString path);
	double	CalcFromLatLongtoXY(double latlong,int level);
	double	CalcFromXYtoLongLat(int x,int level);

private slots:
		void getSourceDir();
		void getDestDir();
		void getImgQuality();
		void setImageSize();
		void setJGWMake();
		void setDatabaseUpgrade();
		void setUseLowerImage();
		void Convert();

		void setOpTargetType();
		void setOpSourceType();
		void setOpCrossType();
		void setOpSumType();
};

#endif // IMAGEMERGE_H
