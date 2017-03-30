#include "imagemerge.h"
#include <math.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <fstream>
enum	OperationState  { OP_TARGET = 0,  OP_SOURCE, OP_SUM,OP_CROSS};
#define MSG_TMP_COPY	"copy to temporary Folder"
#define MSG_DB_COPY	"make to dbForlder"


ImageMerge::ImageMerge(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Init();
	connect(ui.actionSourceDirBtn,SIGNAL(clicked()),this,SLOT(getSourceDir()));
	connect(ui.actionDestDirBtn,SIGNAL(clicked()),this,SLOT(getDestDir()));
	connect(ui.actionImgQualityEdit,SIGNAL(textChanged(QString)),this,SLOT(getImgQuality()));
	connect(ui.actionRunBtn,SIGNAL(clicked()),this,SLOT(Convert()));
	connect(ui.actionSizeCombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(setImageSize()));
	connect(ui.actionJGWCheckBox,SIGNAL(clicked()),this,SLOT(setJGWMake()));
	connect(ui.actiondatabaseUpgradeCheckBox,SIGNAL(clicked()),this,SLOT(setDatabaseUpgrade()));
	connect(ui.actionUseLowerImageCheckBox,SIGNAL(clicked()),this,SLOT(setUseLowerImage()));

	connect(ui.actionOperationSumBtn, SIGNAL(clicked()), this, SLOT(setOpSumType()));
	connect(ui.actionOperationCrossBtn, SIGNAL(clicked()), this, SLOT(setOpCrossType()));
	connect(ui.actionOperationSourceBtn, SIGNAL(clicked()), this, SLOT(setOpSourceType()));
	connect(ui.actionOperationTargetBtn, SIGNAL(clicked()), this, SLOT(setOpTargetType()));
}
void ImageMerge::Init()
{	
	m_blWriteJgw =false;
	m_blDatabaseUpgrage = false;
	m_blUseLowerImage = true;
	m_opType = OP_TARGET;

	m_blStop = false;
	m_imageSize = pow(2.0,8+ui.actionSizeCombo->currentIndex());

		_destLeft = ui.actiontargetLeftEdit->text().toDouble();
		_destRight = ui.actiontargetRightEdit->text().toDouble();
		_destTop = ui.actiontargetTopEdit->text().toDouble();
		_destBottom = ui.actiontargetBottomEdit->text().toDouble();
	
}
ImageMerge::~ImageMerge()
{

}
 double  ImageMerge::CalcFromLatLongtoXY(double latlong,int level)
{
	return ((latlong + 180.0f)/360.0f*pow(2.0,level));
}
 double  ImageMerge::CalcFromXYtoLongLat(int x,int level)
{
	return ((double)x/pow(2.0,(double)level)*360.0f - 180.0f);
}
void ImageMerge::getImgQuality()
{
	int imageQuality = ui.actionImgQualityEdit->text().toInt();
	if (imageQuality<1)		imageQuality =1;
	if (imageQuality>100)		imageQuality =100;
	QString str;
	str.setNum(imageQuality);
	ui.actionImgQualityEdit->setText(str);
}
void ImageMerge::Convert(void)
{
	if (ui.actionSourceDirEdit->text() == "")
	{
		QMessageBox(QMessageBox::Warning,"Source Dir","No  Source Dir!",QMessageBox::Ok,this,Qt::Dialog).exec();
		return;
	}
	if (ui.actionDestDirEdit->text() == "")
	{
		QMessageBox(QMessageBox::Warning,"Destinate Dir","No  Destinate Dir!",QMessageBox::Ok,this,Qt::Dialog).exec();
		return;
	}
	bool ok = 0;
	_Left = ui.actionLeftEdit->text().toDouble();
	_Right = ui.actionRightEdit->text().toDouble();
	_Top = ui.actionTopEdit->text().toDouble();
	_Bottom = ui.actionBottomEdit->text().toDouble();
	_destLeft = ui.actiontargetLeftEdit->text().toDouble();
	_destRight = ui.actiontargetRightEdit->text().toDouble();
	_destTop = ui.actiontargetTopEdit->text().toDouble();
	_destBottom = ui.actiontargetBottomEdit->text().toDouble();
	_Level = ui.actionFromLevelEdit->text().toInt( &ok,10);

	QRectF totalRect,tmpRect;
	if (m_opType == OP_CROSS)
	{
		if (_destLeft < _Left)			_destLeft = _Left;
		if (_destRight > _Right)			_destRight  = _Right;
		if (_destTop >_Top)			_destTop = _Top;
		if (_destBottom < _Bottom)			_destBottom = _Bottom;
	}
	if (m_opType == OP_SUM)
	{
		if (_destLeft > _Left)			_destLeft = _Left;
		if (_destRight < _Right)			_destRight  = _Right;
		if (_destTop <_Top)			_destTop = _Top;
		if (_destBottom >_Bottom)			_destBottom = _Bottom;
	}

	if (m_opType == OP_SOURCE)
	{
		_destLeft = _Left;
		_destRight  = _Right;
		_destTop = _Top;
		_destBottom = _Bottom;
	}
	
	
	totalRect.setLeft(_destLeft);
	totalRect.setRight(_destRight);
	totalRect.setTop(_destBottom);
	totalRect.setBottom(_destTop);
	if (totalRect.isEmpty())
	{
		QMessageBox(QMessageBox::NoIcon,"Execute","Sucess Ok!",QMessageBox::Ok,this,Qt::Dialog).exec();
		return;
	}

	QList<QRectF> rectList;
	ui.actionRunBtn->setEnabled(false);
	rectList = GetRectsInTotal(m_imageSize,totalRect);
	ui.actionDestProgress->setMinimum(0);
	ui.actionDestProgress->setMaximum(rectList.size());
	
	if (rectList.size()!=0)
	{

		for (int i=0;i<rectList.size();i++)
		{
			QStringList lst;
			if(!m_blUseLowerImage)
						lst = GetJpgfilesInRect(rectList.at(i), jpgfiles);
			else
						lst = GetJpgFilesWithinUseLowerlevel(rectList.at(i));
			CopyFilesFromSourceToTmpFolder(lst);
			HopeImgMake(rectList.at(i),lst);
			DeleteFilesInTmpFolder(lst);
			
			ui.actionDestProgress->setValue(i+1);
		}
	}
	
	QMessageBox(QMessageBox::NoIcon,"Execute","Sucess Ok!",QMessageBox::Ok,this,Qt::Dialog).exec();
	ui.actionDestProgress->setValue(0);
	ui.actionlbldestfile->setText("");
	ui.actionRunBtn->setEnabled(true);
	
	return;
}
QList<QRectF>	 ImageMerge::GetRectsInTotal(int size,QRectF rect)
{
	QList<QRectF> result;
	double m_curDiv = 360.f/pow(2.0,(double)_Level);
	double m_div = 360.f/pow(2.0,(double)_Level-ui.actionSizeCombo->currentIndex());//m_curDiv * m_imageSize /256;
	double startX,startY;
	startX = rect.left();
	if (fmod(rect.left(),m_div) != 0.0)
					startX = (int)(rect.left()/m_div)*m_div;
	startY = rect.top();
	if (fmod(rect.top(),m_div) != 0.0)
					startY = (int)(rect.top()/m_div)*m_div;
		

	QPointF firstP(startX,startY);
	QSizeF sampleSize( m_div, m_div);

	int cntX=0;
	while ((startX+m_div*cntX)<rect.right())
			cntX++;
	int cntY=0;
	while ((startY+m_div*cntY)<rect.bottom())
		cntY++;
	QRectF rt;
	for (int i=0;i<cntX;i++)
		for (int j=0;j<cntY;j++)
		{
			rt.setLeft(startX+m_div*i);
			rt.setRight(rt.left()+m_div);
			rt.setTop(startY+m_div*j);
			rt.setBottom(rt.top()+m_div);
			result <<rt;
		}
	return result;
}
void	ImageMerge::DeleteFilesInTmpFolder(QStringList lst)
{
	QString str,tmpFolder(ui.actionDestDirEdit->text()+"/tmp");
	
	for (int i=0;i<lst.size();i++)
	{
		QFile::remove(tmpFolder + "/"+lst.at(i));
	}
	//QDir::rmdir(dir);

}
void ImageMerge::CopyFilesFromSourceToTmpFolder(QStringList lst)
{
	if (lst.size()==0)
		return;

	ui.actionSourceProgress->setMinimum(0);
	ui.actionSourceProgress->setMaximum(lst.size());
	ui.actionlbltmpfile->setText(MSG_TMP_COPY);

	QString DBFolder(ui.actionDestDirEdit->text());
	QString tmpFolder(ui.actionDestDirEdit->text()+"/tmp");
	QString SourceDBFolder;
	QString str(ui.actionSourceDirEdit->text());
	QStringList strlst;
	
	int pp = str.lastIndexOf("geotexture",-1,Qt::CaseInsensitive);
	SourceDBFolder = str.left(pp+QString("geotexture").length());
	if(!QFile::exists(tmpFolder) )
	{
		QDir tmp;
		tmp.mkdir(tmpFolder);
	}
	bool ok;
	int x,y,level;
		for (int i=0;i<lst.size();i++)
		{
			str =lst.at(i).left(lst.at(i).length()-4);
			QStringList lt =str.split("_");
			x=lt.at(0).toInt(&ok,10);
			y=lt.at(1).toInt(&ok,10);
			level = lt.at(2).toInt(&ok,10);
			
			QString subdir;
			if( level > 7 )
			{
				int nblock_x = x / 128;
				int nblock_y = y / 128;				
				subdir = SourceDBFolder+QString("/%1/%2_%3_%4").arg(level).arg(nblock_x).arg(nblock_y).arg(level);
			}
			else
				subdir = SourceDBFolder+QString("/%1").arg(level);
			QString copyFile(subdir+QString("/%1_%2_%3.jpg").arg(x).arg(y).arg(level));
			QString pasteFile(tmpFolder+QString("/%1_%2_%3.jpg").arg(x).arg(y).arg(level));
			qApp->processEvents();
			ui.actionlblSourcefile->setText(copyFile);
				if(QFile::exists(copyFile))
				{
							QFile::copy(copyFile,pasteFile);
				}
				else
				{
					if (m_blUseLowerImage)
							makeFileTotmpFolderUsingUpperLevel(x,y,level);		
				}
			ui.actionSourceProgress->setValue(i+1);
	}
		ui.actionlblSourcefile->setText("");
		ui.actionSourceProgress->setValue(0);
		ui.actionlbltmpfile->setText("");
}
bool ImageMerge::makeFileTotmpFolderUsingUpperLevel(int x1,int y1,int level1)
{
	QString tmpFolder(ui.actionDestDirEdit->text()+"/tmp");
	QString DestFolder(ui.actionDestDirEdit->text());
	QString SourceDBFolder;
	QString str(ui.actionSourceDirEdit->text());
	QStringList strlst;
	
	int pp = str.lastIndexOf("geotexture",-1,Qt::CaseInsensitive);
	SourceDBFolder = str.left(pp+QString("geotexture").length());

	QString subdir;
	QRectF destRect(	CalcFromXYtoLongLat(x1,level1), CalcFromXYtoLongLat(y1+1,level1),
						CalcFromXYtoLongLat(x1+1,level1) - CalcFromXYtoLongLat(x1,level1),
						CalcFromXYtoLongLat(y1+1,level1) - CalcFromXYtoLongLat(y1,level1));
	int x =x1;
	int y = y1;
	int level = level1;
	for (int i = level-1 ; i > -1 ; i--)
	{
		x /= 2;
		y /= 2;
		level = i;
	
		if( level > 7 )
		{
			int nblock_x = x / 128;
			int nblock_y = y / 128;				
			subdir = SourceDBFolder+QString("/%1/%2_%3_%4").arg(level).arg(nblock_x).arg(nblock_y).arg(level);
		}
		else
			subdir = SourceDBFolder+QString("/%1").arg(level);
		
		QString upLevelFile(subdir+QString("/%1_%2_%3.jpg").arg(x).arg(y).arg(level));

		if(QFile::exists(upLevelFile) )
		{
			QImage sourceImg(upLevelFile);
			/*QRectF sourceRect(	CalcFromXYtoLongLat(x,level), CalcFromXYtoLongLat(y,level),
												CalcFromXYtoLongLat(x+1,level) - CalcFromXYtoLongLat(x,level),
												CalcFromXYtoLongLat(y+1,level) - CalcFromXYtoLongLat(y,level));*/
			QRectF sourceRect(	CalcFromXYtoLongLat(x,level), CalcFromXYtoLongLat(y+1,level),
												CalcFromXYtoLongLat(x+1,level) - CalcFromXYtoLongLat(x,level),
												CalcFromXYtoLongLat(y+1,level) - CalcFromXYtoLongLat(y,level));
			QRect pixRect = getPixRect_InTotal(sourceRect,destRect,level);
			
			QImage img(pixRect.width()-1,pixRect.height()-1, QImage::Format_RGB888);
			QRgb  rgb;

			for (int i =pixRect.left();i<pixRect.right()+1;i++)

				for (int j = pixRect.top();j<pixRect.bottom()+1;j++)
				{
					rgb = sourceImg.pixel(i,j);
					img.setPixel(i-pixRect.left(),j-pixRect.top(),rgb);
				}
			QImage saveimg = img.scaled(256,256,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

			subdir = tmpFolder+QString("/%1_%2_%3.jpg").arg(x1).arg(y1).arg(level1);
			saveimg.save(subdir,"jpg",100);
			return true;
		}
	}
	return false;
}
QStringList ImageMerge::GetJpgfilesInRect(QRectF totalRect, QStringList list)
{
	QStringList fileList;
	QString str;
	QRectF workRect;
	QRect		areaRect;
	int x,y;
	bool ok;
	for (int i=0;i<list.size();i++)
	{
		str =list.at(i).left(list.at(i).length()-4);
		QStringList lst =str.split("_");
		x=lst.at(0).toInt(&ok,10);
		y=lst.at(1).toInt(&ok,10);
		workRect.setLeft(CalcFromXYtoLongLat(x,_Level));
		workRect.setRight(CalcFromXYtoLongLat(x+1,_Level));
		workRect.setTop(CalcFromXYtoLongLat(y,_Level));
		workRect.setBottom(CalcFromXYtoLongLat(y+1,_Level));
		if (	totalRect.contains(workRect.topLeft())  && totalRect.contains(workRect.bottomRight())  )
			fileList<<list.at(i);
	}
	return fileList;
}
void ImageMerge::HopeImgMake(QRectF rect,QStringList &list)
{
	QString tmpFolder(ui.actionDestDirEdit->text()+"/tmp");
	QString DestFolder(ui.actionDestDirEdit->text());

	bool ok;
	int minX,minY, maxX,maxY;
	int level = 0;

	
	minX = minY = maxX = maxY = 0;
	if (list.size() ==0)
			return;
	minX  =	CalcFromLatLongtoXY(rect.left(), _Level);
	maxX =	CalcFromLatLongtoXY(rect.right(),_Level);
	minY  =	CalcFromLatLongtoXY(rect.top(),_Level);
	maxY =	CalcFromLatLongtoXY(rect.bottom(),_Level);

	QString destPath = ui.actionDestDirEdit->text();
	int mul = m_imageSize/256;
	int destLevel = ui.actionToLevelEdit->text().toInt(&ok,10);

	if (m_blDatabaseUpgrage)
	{
		destPath.append("/" + QString("%1").arg(destLevel));	
		if(!QFile::exists(destPath))
		{
			QDir tmp;
			tmp.mkdir(destPath);
		}
		if( destLevel > 7 )
		{
			int nblock_x = ( minX / mul )  / 128;
			int nblock_y = ( minY / mul  ) / 128;
			QString subdir;
			subdir = QString("%1_%2_%3").arg(nblock_x).arg(nblock_y).arg(destLevel);
			destPath.append( "/" +subdir);
			if(!QFile::exists(destPath) )
			{
				QDir tmp;
				tmp.mkdir(destPath);
			}
		 }	
	  }
	destPath.append("/"+QString("%1_%2_%3.jpg").arg( minX / mul ).arg(minY / mul).arg(destLevel));
	
	ui.actionlbldestfile->setText(destPath);
	ui.actionlbltmpfile->setText(MSG_DB_COPY);

	QImage img(m_imageSize,m_imageSize,QImage::Format_RGB888);			img.fill(0);	
	QString tmpPath(destPath);	
	QImage tmpImg;
	if(QFile::exists(tmpPath))
	{
		tmpImg.load(tmpPath,"jpg");
		img=tmpImg.scaled(m_imageSize,m_imageSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	}

	ui.actionSourceProgress->setMinimum(0);
	ui.actionSourceProgress->setMaximum(list.size());

	QString str,tmpStr;
	for (int i=0;i<list.size();i++)
	{
		str =list.at(i).left(list.at(i).length()-4);
		QStringList lst =str.split("_");
		int x=lst.at(0).toInt(&ok,10);
		int y=lst.at(1).toInt(&ok,10);

		
		ui.actionlblSourcefile->setText(tmpFolder + QString("/%1_%2_%3.jpg").arg(x).arg(y).arg(_Level));
		qApp->processEvents();
		tmpImg.load(tmpFolder + QString("/%1_%2_%3.jpg").arg(x).arg(y).arg(_Level),"jpg");

		 for (int wx=0;wx<tmpImg.width();wx++)
			 for (int wy = 0;wy<tmpImg.height();wy++)
			 {
				 QRgb rgb(tmpImg.pixel(wx,wy));
				 int dx = (x-minX)*256+wx;
				 int dy = (maxY-y-1)*256+wy;
				 img.setPixel(dx,dy,rgb);
			 }
		ui.actionSourceProgress->setValue(i+1);
	}
	
	img.setDotsPerMeterY(2834.6);
	img.setDotsPerMeterX(2834.6);
	int outputSize;
	if (m_blDatabaseUpgrage)
			outputSize = 256;
	else
			outputSize =m_imageSize;

	QImage saveimg=img.scaled(outputSize,outputSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	int imageQuality = ui.actionImgQualityEdit->text().toInt();	
	QFile::remove(tmpPath);
	saveJpg(saveimg, destPath);
	//saveimg.save(destPath,"jpg",imageQuality);
	

	if (m_blWriteJgw)
	{
		QString	destJGW(destPath.left(destPath.length()-4));
		destJGW.append(".jgw");
		WriteJgw(minX / mul,minY / mul,destLevel, outputSize,destJGW);
	}
	ui.actionlblSourcefile->setText("");
	ui.actionSourceProgress->setValue(0);	
	ui.actionlbltmpfile->setText("");
}
void ImageMerge::saveJpg( QImage saveimg,QString path)
{
	///*bool bGray = false;
	//int col, i;

	//struct jpeg_compress_struct cinfo;

	//struct jpeg_error_mgr jerr;

	//FILE * outfile;		/* target file */
	//int row_stride;		/* physical row width in image buffer */
	//JSAMPARRAY buffer;		/* Output row buffer */

	//cinfo.err = jpeg_std_error(&jerr);
	//jpeg_create_compress(&cinfo);

	//QByteArray strBuffer = path.toAscii();
	//const char *lpszFileName = strBuffer.constData();

	//if ((outfile = fopen(lpszFileName, "wb")) == NULL) {
	//	return ;
	//}
	//jpeg_stdio_dest(&cinfo, outfile);


	//cinfo.image_width = saveimg.width(); 	// image width and height, in pixels
	//cinfo.image_height = saveimg.height();
	//if(saveimg.depth() == 8)
	//{
	//	cinfo.input_components = 1; 	// # of color components per pixel
	//	cinfo.in_color_space = JCS_GRAYSCALE; 	/* colorspace of input image */
	//	bGray = true;
	//}
	//else
	//{
	//	cinfo.input_components = 3; 	// # of color components per pixel
	//	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	//}

	//jpeg_set_defaults(&cinfo);

	//jpeg_start_compress(&cinfo, TRUE);

	//row_stride = saveimg.width()*cinfo.input_components;	/* JSAMPLEs per row in image_buffer */

	//buffer = (*cinfo.mem->alloc_sarray)
	//	((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	//int line = 0;
	//if (bGray) 
	//{
	//	unsigned char * ptr;
	//	while (cinfo.next_scanline < cinfo.image_height) 
	//	{
	//		ptr = saveimg.scanLine( line++ );
	//		memcpy(buffer[0], ptr, row_stride);
	//		(void) jpeg_write_scanlines(&cinfo, buffer, 1);
	//	}
	//}
	//else
	//{

	//	while (cinfo.next_scanline < cinfo.image_height) 
	//	{
	//		for(col=0, i=0 ; i<row_stride ; col++, i+=3)
	//		{
	//			QRgb rgb(saveimg.pixel(col,line));
	//			buffer[0][i] = (char)qRed(rgb);
	//			buffer[0][i+1] = (char)qGreen(rgb);
	//			buffer[0][i+2] = (char)qBlue(rgb);
	//		}
	//		line++;
	//		(void) jpeg_write_scanlines(&cinfo, buffer, 1);
	//	}

	//}

	//jpeg_finish_compress(&cinfo);

	//fclose(outfile);

	//jpeg_destroy_compress(&cinfo);

	//return ;*/

}
void ImageMerge::getDestDir(void)
{
	QString strSavePath = ui.actionDestDirEdit->text();
	if (strSavePath.length()==0)		strSavePath = "d:/";
	
	strSavePath = QFileDialog::getExistingDirectory(this,	tr("Open folder to save converted jpg file!"), strSavePath);

	if (!strSavePath.isEmpty()) 
		ui.actionDestDirEdit->setText(strSavePath);	
}

void ImageMerge::getSourceDir(void)
{
	QString strOpenPath = ui.actionSourceDirEdit->text();
	if (strOpenPath.length()==0)		strOpenPath = "d:/";
	strOpenPath = QFileDialog::getExistingDirectory(this,	tr("Open folder of Source jpg file!"), strOpenPath);

	if (strOpenPath.isEmpty()) 
		return;

	ui.actionSourceDirEdit->setText(strOpenPath);

	QStringList ss = strOpenPath.split("/");
	int pp = strOpenPath.lastIndexOf("geotexture",-1,Qt::CaseInsensitive);
	if (pp<0)
	{
		QMessageBox(QMessageBox::Warning,"Source Dir","No  DatabaseDirectory(including Geotexture)!",QMessageBox::Ok,this,Qt::Dialog).exec();
		return;
	}
	QDir inputDir = QDir(strOpenPath);        
	if(!inputDir.exists())
	{
		QMessageBox(QMessageBox::Warning,"Source Dir","No  Source Dir!",QMessageBox::Ok,this,Qt::Dialog).exec();
		return;
	}

	QString extension="*.jpg";
	jpgfiles = inputDir.entryList(QStringList(extension),QDir::Files | QDir::NoSymLinks);
	if(jpgfiles.size()==0)
	{
		QMessageBox(QMessageBox::Warning,"Source Dir","No  jpg files!",QMessageBox::Ok,this,Qt::Dialog).exec();
	
		return;
	}
	

	bool ok;
	int minX,minY;
			minX = minY=0;
	int maxX,maxY;
			maxX = maxY = 0;
	int level = 0;
	QString str;

	QStringList lst = QString(jpgfiles.at(0).left(jpgfiles.at(0).length()-4)).split("_");
	if (lst.size()>2)
	{
			minX =lst.at(0).toInt(&ok,10);
			maxX =lst.at(0).toInt(&ok,10);
			minY =lst.at(1).toInt(&ok,10);
			maxY =lst.at(1).toInt(&ok,10);
			level = lst.at(2).toInt(&ok,10);
	}
	str.setNum(level);
	ui.actionFromLevelEdit->setText(str);
	ui.actionToLevelEdit->setText(str);


	for (int i=1;i<jpgfiles.size();i++)
	{
		lst = QString(jpgfiles.at(i).left(jpgfiles.at(i).length()-4)).split("_");
		if (lst.size()>2)
		{
		
		int x =lst.at(0).toInt(&ok,10);
		int y =lst.at(1).toInt(&ok,10);
		_Level = lst.at(2).toInt(&ok,10);
		if (minX > x)
					minX = x;
		if (maxX < x)
					maxX = x;
		if (minY > y)
					minY = y;
		if (maxY < y)
					maxY = y;
		}
	}
	
	_Left=CalcFromXYtoLongLat(minX-1,level);			str.setNum(_Left,'f',8);
			ui.actiontargetLeftEdit->setText(str);
	_Left=CalcFromXYtoLongLat(minX,level);			str.setNum(_Left,'f',8);
			ui.actionLeftEdit->setText(str);
	_Right=CalcFromXYtoLongLat(maxX+2,level);			str.setNum(_Right,'f',8);
			ui.actiontargetRightEdit->setText(str);
	_Right=CalcFromXYtoLongLat(maxX+1,level);			str.setNum(_Right,'f',8);
			ui.actionRightEdit->setText(str);
	_Bottom=CalcFromXYtoLongLat(minY-1,level);		str.setNum(_Bottom,'f',8);
			ui.actiontargetBottomEdit->setText(str);
	_Bottom=CalcFromXYtoLongLat(minY,level);		str.setNum(_Bottom,'f',8);
			ui.actionBottomEdit->setText(str);
	_Top=CalcFromXYtoLongLat(maxY+2,level);			str.setNum(_Top,'f',8);
			ui.actiontargetTopEdit->setText(str);	
	_Top=CalcFromXYtoLongLat(maxY+1,level);			str.setNum(_Top,'f',8);
			ui.actionTopEdit->setText(str);

		ui.actionToLevelEdit->setText(QString("%1").arg(_Level -ui.actionSizeCombo->currentIndex()));
	


}
void ImageMerge::setImageSize()
{
	m_imageSize = pow(2.0,8+ui.actionSizeCombo->currentIndex());
	ui.actionToLevelEdit->setText(QString("%1").arg(_Level -ui.actionSizeCombo->currentIndex()));
	QString multi;
		multi = "( "+QString("%1").arg((int)m_imageSize/256) +" * "+QString("%1").arg((int)m_imageSize/256)+" )";
	ui.actionMultiText->setText(multi);
}
void ImageMerge::setJGWMake()
{
	if (m_blWriteJgw)
		m_blWriteJgw = false; 
	else
		m_blWriteJgw = true;
}
void ImageMerge::setDatabaseUpgrade()
{
	if (m_blDatabaseUpgrage)
		m_blDatabaseUpgrage = false; 
	else
		m_blDatabaseUpgrage = true;
}
void ImageMerge::setUseLowerImage()
{
	if (m_blUseLowerImage)
		m_blUseLowerImage = false; 
	else
		m_blUseLowerImage = true;
}
void ImageMerge::WriteJgw(int x,int y,int level,int imgSize,QString filepath)
{
	/*** Getting longitude and latitude corresponding x,y,level ***/
	double longitude,latitude;
	QString longStr,latStr;
	longitude = CalcFromXYtoLongLat(x,level); 
	latitude = CalcFromXYtoLongLat(y+1,level);
	longStr.setNum(longitude,'f',16); 
	latStr.setNum(latitude,'f',16);

	/*** Getting additional informations about jgw file ***/
	QString longStepStr,latStepStr,zeroStr;
	double longStep,latStep;
	longStep = (CalcFromXYtoLongLat(x+1,level) - longitude)/imgSize;
	latStep = -longStep;
	longStepStr.setNum(longStep,'f',16); latStepStr.setNum(latStep,'f',16);
	zeroStr.setNum(0.0f,'f',16);


	QByteArray fnameArray = filepath.toAscii();
	const char *fnamedata = fnameArray.constData();

	std::fstream fp;
	fp.open(fnamedata,std::ios::out);
	if (!fp.good())	return;

	const char enter = (char)10;
	QByteArray longArray = longStr.toAscii();
	const char *longdata = longArray.constData();

	QByteArray latArray = latStr.toAscii();
	const char *latdata = latArray.constData();

	QByteArray longStepArray = longStepStr.toAscii();
	const char *longStepdata = longStepArray.constData();

	QByteArray latStepArray = latStepStr.toAscii();
	const char *latStepdata = latStepArray.constData();

	QByteArray zeroArray = zeroStr.toAscii();
	const char *zerodata = zeroArray.constData();

	fp.write(longStepdata,longStepArray.length());
	fp.write(&enter,1);
	fp.write(zerodata,zeroArray.length());
	fp.write(&enter,1);
	fp.write(zerodata,zeroArray.length());
	fp.write(&enter,1);
	fp.write(latStepdata,latStepArray.length());
	fp.write(&enter,1);
	fp.write(longdata,longArray.length());
	fp.write(&enter,1);
	fp.write(latdata,latArray.length());
	fp.close();
}
void ImageMerge::setOpTargetType()
{
	m_opType = OP_TARGET;
	ui.actionOperationSumBtn->setChecked(false);
	ui.actionOperationSourceBtn->setChecked(false);
	ui.actionOperationCrossBtn->setChecked(false);
	ui.actionOperationTargetBtn->setChecked(true);
}
void ImageMerge::setOpSourceType()
{
	m_opType = OP_SOURCE;
	ui.actionOperationSumBtn->setChecked(false);
	ui.actionOperationSourceBtn->setChecked(true);
	ui.actionOperationCrossBtn->setChecked(false);
	ui.actionOperationTargetBtn->setChecked(false);

}
void ImageMerge::setOpCrossType()
{
	m_opType = OP_SUM;
	ui.actionOperationSumBtn->setChecked(false);
	ui.actionOperationSourceBtn->setChecked(false);
	ui.actionOperationCrossBtn->setChecked(true);
	ui.actionOperationTargetBtn->setChecked(false);
}
void ImageMerge::setOpSumType()
{
	m_opType = OP_CROSS;
	ui.actionOperationSumBtn->setChecked(true);
	ui.actionOperationSourceBtn->setChecked(false);
	ui.actionOperationCrossBtn->setChecked(false);
	ui.actionOperationTargetBtn->setChecked(false);

}
QStringList ImageMerge::GetJpgFilesWithinUseLowerlevel(QRectF rect)
{
	int left = CalcFromLatLongtoXY(rect.left(),_Level);
	int right = CalcFromLatLongtoXY(rect.right(),_Level);
	int top = CalcFromLatLongtoXY(rect.bottom(),_Level);
	int bottom = CalcFromLatLongtoXY(rect.top(),_Level);

	QStringList fileList;
	QString str;
	for (int x= left; x<right; x++)
		for (int y=bottom; y< top; y++)
		{
			str = QString("%1_%2_%3.jpg").arg(x).arg(y).arg(_Level);
			fileList  << str;
		}
		return fileList;
}
QString ImageMerge::GetElementFile(QString str)
{
		QString destPath, subdir;
		QString tmpFolder(ui.actionDestDirEdit->text()+"/tmp/");
		QString destFile(str.left(str.length()-4));
		bool ok;
		QStringList lst =destFile.split("_");
		int x = lst.at(0).toInt(&ok,10);
		int y = lst.at(1).toInt(&ok,10);	
		int level = lst.at(2).toInt(&ok,10);	

		QRectF destRect(  CalcFromXYtoLongLat(x,level), CalcFromXYtoLongLat(y+1,level),
										CalcFromXYtoLongLat(x+1,level) - CalcFromXYtoLongLat(x,level),
										CalcFromXYtoLongLat(y,level) - CalcFromXYtoLongLat(y+1,level));
		int nblock_x ;
		int nblock_y ;

		destPath = ui.actionDestDirEdit->text();
		if( level > 7 )
		{
			nblock_x = x  / 128;
			nblock_y = y  / 128;
			subdir = QString("/%1/%2_%3_%4").arg(level).arg(nblock_x).arg(nblock_y).arg(level);
		}
			destPath.append( subdir+destFile+".jpg" );

			if(QFile::exists(destPath) )
			{
				QFile::copy ( destPath, tmpFolder +destFile );
					return  "/tmp/" +destFile;
			}
			for (int i = level-1 ; i > 4 ; i--)
			{
				x /= 2;
				y /= 2;
				level = i;
				if( level > 7 )
				{
					nblock_x = x  / 128;
					nblock_y = y  / 128;
					subdir = QString("/%1/%2_%3_%4").arg(level).arg(nblock_x).arg(nblock_y).arg(level);
				}
				destPath = ui.actionDestDirEdit->text();
				destPath.append( subdir+destFile );
				if(QFile::exists(destPath) )
				{
					QImage sourceImg(destPath);
					QRectF sourceRect(	CalcFromXYtoLongLat(x,level), CalcFromXYtoLongLat(y+1,level),
														CalcFromXYtoLongLat(x+1,level) - CalcFromXYtoLongLat(x,level),
														CalcFromXYtoLongLat(y,level) - CalcFromXYtoLongLat(y+1,level));
					QRect pixRect = getPixRect_InTotal(sourceRect,destRect,level);
				}
					return  destPath;

			}

		return destPath;
}
QRect ImageMerge::getPixRect_InTotal(QRectF totalRect,QRectF workRect,int level)
{
	QRect rect;		//loading pixel/line rectangle
	double _Solution = 360.0f/pow(2, (double)level)/256;
	rect.setLeft( (workRect.left()-totalRect.left())/_Solution);
	rect.setRight( rect.left()+ workRect.width()/_Solution);

	rect.setTop((totalRect.bottom()-workRect.bottom())/_Solution);
	rect.setBottom( rect.top()+workRect.height()/_Solution);

	return rect;
}