#include "latlongtohopeimage.h"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include "common.h"

#define IMAGE_FILTER "*.bmp"
#define IMAGE_EXT    "bmp"   

LatLongToHopeImage::LatLongToHopeImage(QWidget *parent)
	: QDialog(parent)
	, m_dirSelect(false)
{
	ui.setupUi(this);
	ui.actionLeftEdit->setMaxLength(19);
	ui.actionRightEdit->setMaxLength(19);
	ui.actionTopEdit->setMaxLength(19);
	ui.actionBottomEdit->setMaxLength(19);
	ui.actionDestLeftEdit->setMaxLength(19);
	ui.actionDestRightEdit->setMaxLength(19);
	ui.actionDestTopEdit->setMaxLength(19);
	ui.actionDestBottomEdit->setMaxLength(19);

	_destLeft = ui.actionDestLeftEdit->text().toDouble();
	_destRight = ui.actionDestRightEdit->text().toDouble();
	_destTop = ui.actionDestTopEdit->text().toDouble();
	_destBottom = ui.actionDestBottomEdit->text().toDouble();

	connect(ui.actionSourceFileBtn,SIGNAL(clicked()),this,SLOT(getSource()));
	connect(ui.actionDestinateDirBtn,SIGNAL(clicked()),this,SLOT(getDestinateFolder()));
	connect(ui.actionImgQualityEdit,SIGNAL(textChanged(QString)),this,SLOT(getImgQuality()));
	connect(ui.actionIgnoreColorEdit,SIGNAL(textChanged(QString)),this,SLOT(getIgnoreColor()));
	connect(ui.actionDirectorySelect,SIGNAL(clicked()),this,SLOT(setDirectorySelect()));
	connect(ui.convertBtn,SIGNAL(clicked()),this,SLOT(Convert()));
	connect(ui.tmpDirBtn, SIGNAL(clicked()), this, SLOT(setTmpDir()));
	connect(ui.jgwChkBox, SIGNAL(clicked()), this, SLOT(setJGW_Append()));
	connect(ui.marginProcessBtn, SIGNAL(clicked()), this, SLOT(ProcessingMarginImages()));

	m_dirSelect = true;
	ui.label->setText("Source Directory");
	m_writeJGW = false;

	m_tmpDir = "d:/tmp";
}


LatLongToHopeImage::~LatLongToHopeImage()
{

	
}


void LatLongToHopeImage::getSourceFolder(void)
{
	QString strOpenPath = QFileDialog::getExistingDirectory(this,	tr("Open folder to  converted jpg file!"), QDir::rootPath());
	if (!strOpenPath.isEmpty()) 
		ui.actionSourceFileEdit->setText(strOpenPath);

	QDir _dirOpen = QDir(strOpenPath);
	QStringList fileNameList;
	QString ext = IMAGE_FILTER;
	fileNameList = _dirOpen.entryList(QStringList(ext),QDir::Files | QDir::NoSymLinks);
	if(fileNameList.size()==0)
	{
		QMessageBox(QMessageBox::Warning,"Source Dir","No  jpg files!",QMessageBox::Ok,this,Qt::Dialog).exec();
		return;
	}
	
	for (int i=0; i<fileNameList.size(); i++)
	{
		QString jpgFile = strOpenPath + "/" + fileNameList.at(i);
		QString strTemp = jpgFile.left(jpgFile.length()-4);
		QString jgwFile = strTemp +".jgw";

		if (!QFile::exists(jgwFile))
			continue;

		fileNameListInDir << fileNameList.at(i);
	}
}
void LatLongToHopeImage::getSource(void)
{
	if (m_dirSelect)
		getSourceFolder();
	else
		getSourceFiles();
}
void LatLongToHopeImage::getSourceFiles(void)
{
	
	QFileDialog dialog(this);
	QStringList filters;
	filters << "jpg (*.jpg)" << "All files (*.*)";
	dialog.setNameFilters(filters);

	dialog.setDirectory(ui.actionSourceFileEdit->text());

	if(dialog.exec() == QDialog::Accepted) {
		m_srcImgPath = dialog.selectedFiles().at(0);
		ui.actionSourceFileEdit->setText(m_srcImgPath);
	}
}
void LatLongToHopeImage::writeFileInfoData(QString strLeft,	QString strTop,
										   QString strRight,QString strBottom)
{
	QString _jgwLeft,_jgwTop,_jgwxSolution,_jgwySolution;
	QString _txtLeft,_txtRight,_txtTop,_txtBottom,_txtXS,_txtYS;
	QString _proLeft,_proRight,_proTop,_proBottom;

	QString jpgFile = ui.actionSourceFileEdit->text();
	QString strTemp = jpgFile.left(jpgFile.length()-4);
	QString jgwFile = strTemp + ".jgw";
	QString txtFile = strTemp + ".txt";
	//open rikr.txt
	QFile rikr(strTemp+"_Rikr.txt");
	QString temp;
	const char enter = (char)10;
	
	if (rikr.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&rikr);
		temp = QString("Name : " +strTemp+".jpg");
		out << temp << enter<<enter;
		//get jgw Info
		temp = QString("  ----------    jgw File     ---------- ");
		out << temp << enter;
		QFile _jgwfile(jgwFile);
		if (!_jgwfile.exists())
		{
			temp = QString("  ----------   No exist jgw File !!    ---------- ");
			out << temp << enter;
		}
		else
		{
			if (_jgwfile.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QTextStream in(&_jgwfile);
				QString _jgwline;

				_jgwline=in.readLine();			_jgwxSolution = _jgwline;
				_jgwline=in.readLine();			_jgwline=in.readLine();	
				_jgwline=in.readLine();			_jgwySolution = _jgwline;
				_jgwLeft=in.readLine();			_jgwTop=in.readLine();				
			}
			
			out << _jgwxSolution << enter << "0.0" << enter <<"0.0"<< enter;
			out << _jgwySolution << enter << _jgwLeft << enter << _jgwTop <<enter<<enter;
		}
		//get text info
		QFile _txtfile(txtFile);
		temp = QString("  --------------------          txt File     ----------------------------------- ");
		out << temp << enter;
		if (!_txtfile.exists())
		{
			temp = QString("  --------------------       No exist  txt File     ----------------------------------- ");
			out << temp << enter;
		}
		else
		{
				if (_txtfile.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					QTextStream in_txt(&_txtfile);
					QString _txtline;

					_txtline=in_txt.readLine();	
					QStringList lt =_txtline.split(":");
					QString tmp(lt.at(1));
					 lt = tmp.split(",");
					 _txtLeft = lt.at(0); _txtBottom = lt.at(1);

					 _txtline=in_txt.readLine();	
					 _txtline=in_txt.readLine();	
					 lt =_txtline.split(":");
					 tmp = lt.at(1);
					 lt = tmp.split(",");
					  _txtRight = lt.at(0); _txtTop = lt.at(1);
					  _txtXS.setNum((_txtRight.toDouble()-_txtLeft.toDouble())/totalJpgW,'f',15);
					  _txtYS.setNum((_txtBottom.toDouble()-_txtTop.toDouble())/totalJpgH,'f',15);
				}
				
				out << "left :	" << _txtLeft <<  "		Top : 		" << _txtTop <<enter;
				out << "Right :	" << _txtRight << "		Bottom :	" << _txtBottom <<enter<<enter;
		}
		temp = QString("  --------------------    program calculation     ------------------------- ");
		out << temp << enter;
		out << "left :	" << strLeft <<  "		Top	:	" << strTop <<enter;
		out << "Right :	" << strRight << "		Bottom :" << strBottom <<enter<<enter;
		if (_txtfile.exists())
		{						
				temp = QString("  --------------------    Compare     ------------------------- ");
				out << temp << enter;
				out << "				left			right				Top				Bottom " << enter;
				out << "	txt :	"<<_txtLeft <<"	"<< _txtRight <<"	"<< _txtTop<<"	"<< _txtBottom<< enter;
				out << "	cal :	"<<strLeft <<"	"<< strRight <<"	"<< strTop<<"	"<< strBottom<< enter;
				double l,r,t,b;
				l = _txtLeft.toDouble() - strLeft.toDouble(); r = _txtRight.toDouble() - strRight.toDouble();
				t = _txtTop.toDouble() - strTop.toDouble(); b = _txtBottom.toDouble() - strBottom.toDouble();
				
				strLeft.setNum(l,'f',15);		strTop.setNum(t,'f',15);
				strBottom.setNum(b,'f',15);		strRight.setNum(r,'f',15);
				out << "	dif:	"<<strLeft <<"	"<< strRight  <<"	"<<
					strTop <<"	"<< strBottom << enter;
				temp = QString("  --------------------    Compare   jgw   ------------------------- ");
				out << temp << enter;
				
				out << "	txt :	"<<_txtXS <<"	"<< _txtYS << enter;
				out << "	jgw :	"<<_jgwxSolution <<"	"<< _jgwySolution << enter;

		}

	}
	rikr.close();
}

void LatLongToHopeImage::getDestinateFolder(void)
{
	QString strSavePath = QFileDialog::getExistingDirectory(this,	tr("Open folder to save converted jpg file!"),QDir::rootPath());

	if (!strSavePath.isEmpty()) 
		ui.actionDestinateDirEdit->setText(strSavePath);	
}
void LatLongToHopeImage::ConvertFile(QString imgPath)
{
	if (!JGWRead(imgPath, &_Left, &_Top, &_xSolution, &_ySolution))
	{
		QMessageBox(QMessageBox::Warning,"jgw","No Exist .jgw file!",QMessageBox::Ok,this,Qt::Dialog).exec();
		return;
	}
	ui.convertBtn->setEnabled(false);
	//GetJPEGDimension(imgPath, totalJpgW, totalJpgH);

	QImage	*jpgImg;
	jpgImg = new QImage;
	if(!jpgImg->load(imgPath,IMAGE_EXT))
	{
		QMessageBox(QMessageBox::Warning,"Load failed","Cannot load JPG file!",QMessageBox::Ok,this,Qt::Dialog).exec();
		ui.convertBtn->setEnabled(true);
		return;
	}

	totalJpgW = jpgImg->width();
	totalJpgH = jpgImg->height();
	_Right = _xSolution * (totalJpgW) +  _Left;
	_Bottom = _ySolution * (totalJpgH) + _Top;

	RegisterInformation();
	QRectF totalRect;
	totalRect.setLeft(_destLeft);
	totalRect.setRight(_destRight);
	totalRect.setBottom(_destTop);
	totalRect.setTop(_destBottom);

	if (totalRect.isEmpty())
	{
		QMessageBox(QMessageBox::NoIcon,"Execute","Sucess Ok!",QMessageBox::Ok,this,Qt::Dialog).exec();
		ui.actionlblfilename->setText("");
		ui.convertBtn->setEnabled(true);
		return;
	}

	for (int level = m_startLevel; level<= m_endLevel; level++)
	{	
		getTotalRectofXYsInLevel( _destLeft, _destRight, _destTop, _destBottom, level);
		int totalNum = (m_rectXYInlevel.x2-m_rectXYInlevel.x1+1)*(m_rectXYInlevel.y2 - m_rectXYInlevel.y1+1);
		ui.actionProgressBar->setMinimum(0);
		ui.actionProgressBar->setMaximum(totalNum);
		MakeMarginImages(jpgImg, level);

		for (int y = m_rectXYInlevel.y1; y<m_rectXYInlevel.y2+1; y++)			
		{	
			for (int x =m_rectXYInlevel.x1;  x<m_rectXYInlevel.x2+1; x++)
			{
				QRect areaRect;
				QPointF bottomLeft, topRight;
				bottomLeft.setX(CalcFromXYtoLongLat(x, level));
				bottomLeft.setY(CalcFromXYtoLongLat(y, level));
				topRight.setX(CalcFromXYtoLongLat(x+1, level));
				topRight.setY(CalcFromXYtoLongLat(y+1, level));
				QString saveFullPath;
				if (totalRect.contains(bottomLeft))
				{
					if (totalRect.contains(topRight))
					{
						int var;
						var = GetNumberOfPixelBetweenTwoPts(_Left, bottomLeft.x(), _xSolution);
						areaRect.setLeft(var);
						var = GetNumberOfPixelBetweenTwoPts(_Left, topRight.x(), _xSolution);
						areaRect.setRight(var-1);
						var = GetNumberOfPixelBetweenTwoPts(topRight.y(), _Top, _ySolution);
						areaRect.setTop(var);
						var = GetNumberOfPixelBetweenTwoPts(bottomLeft.y(), _Top,  _ySolution);
						areaRect.setBottom(var -1);
						loadPixelData(jpgImg, areaRect,x,y,level);
					}  // end of if-totalRect.contains(topRight)
				}  // end of if-totalRect.contains(bottomLeft)
				int curNum = (m_rectXYInlevel.x2 - m_rectXYInlevel.x1) * (y - m_rectXYInlevel.y1) + x - m_rectXYInlevel.x1 + 1;
				ui.actionProgressBar->setValue(curNum);
			} // end of for - x	
			
			qApp->processEvents();
		}  // end of for - y
		ui.actionProgressBar->setValue(totalNum);

	}  // end of for - level
	delete jpgImg;
	ui.convertBtn->setEnabled(true);
	if (!m_dirSelect)
		QMessageBox(QMessageBox::NoIcon,"Execute","Sucess Ok!",QMessageBox::Ok,this,Qt::Dialog).exec();

	/*
	if (sImgLoaded)
		delete sImg;
	*/
}
void LatLongToHopeImage::MakeMarginImages(QImage*srcImg, int level)
{
	double gap;
	QRect subArea;
	double tileWidth = CalcFromXYtoLongLat(1, level) + 180.0f;
	double pixWidth = tileWidth/256.0f;
	double topCoord, leftCoord;

	int imgWidth, imgHeight, i, pixNum;
	imgHeight = srcImg->height(); imgWidth = srcImg->width();

	gap =_Left  - CalcFromXYtoLongLat(m_rectXYInlevel.x1, level);
	if (gap  > _xSolution)
	{
		gap = CalcFromXYtoLongLat(m_rectXYInlevel.x1 + 1, level) - _Left;
		pixNum = gap/_xSolution; leftCoord = _Left;
		if (pixNum > 0)
		{
			subArea.setLeft(0); subArea.setRight(pixNum-1);
			for (int y = m_rectXYInlevel.y1; y < m_rectXYInlevel.y2+1; y++)
			{
				double yFCoord = CalcFromXYtoLongLat(y, level); double ySCoord = CalcFromXYtoLongLat(y+1, level);
				QString ext;
				int vExtPix, hExtPix; hExtPix = gap/tileWidth*256;
				if (yFCoord < _Bottom)
				{
					int rPixNum = (_Top - ySCoord)/abs(_ySolution); ext = "_TR.jpg";
					subArea.setTop(rPixNum); subArea.setBottom(imgHeight-1);
					vExtPix = (ySCoord - _Bottom)/tileWidth*256;
					topCoord = ySCoord;
				}
				else if (ySCoord > _Top)
				{
					int rPixNum = (_Top - yFCoord)/abs(_ySolution); ext = "_BR.jpg";
					subArea.setTop(0); subArea.setBottom(rPixNum-1);
					vExtPix = (_Top-yFCoord)/tileWidth*256;
					topCoord = _Top;
				}
				else
				{
					int pixNum1 = (_Top-yFCoord)/abs(_ySolution); int pixNum2 = (_Top - ySCoord)/abs(_ySolution); ext = "_RI.jpg";
					subArea.setTop(pixNum2); subArea.setBottom(pixNum1-1);
					vExtPix = 256; topCoord = ySCoord;
				}
				QString saveStr = m_tmpDir + "/" + QString().setNum(level) + "/";
				if (level > 7)
				{
					int xS = m_rectXYInlevel.x1/128;
					int yS = y/128;
					saveStr = saveStr + QString().setNum(xS) + "_" + QString().setNum(yS) + "_" + QString().setNum(level) + "/";
				}
				if (!QFile::exists(saveStr)) {
					QDir tmp;
					tmp.mkpath(saveStr);
				}
				saveStr = saveStr + QString().setNum(m_rectXYInlevel.x1) + "_" + QString().setNum(y) + "_" +QString().setNum(level) + ext;
				QImage mgImg(subArea.size(), QImage::Format_RGB888);
				mgImg = srcImg->copy(subArea);
				QImage svImg = mgImg.scaled(hExtPix, vExtPix, Qt::IgnoreAspectRatio, Qt::FastTransformation);
				if (svImg.save(saveStr, "jpg", 100))
					WriteMarginImageInfo(saveStr, leftCoord, topCoord, pixWidth);
			}
		}
	}

	gap = _Right - CalcFromXYtoLongLat(m_rectXYInlevel.x2, level);
	if(gap > _xSolution)
	{
		pixNum = gap/_xSolution;
		subArea.setLeft(imgWidth-pixNum-1); subArea.setRight(imgWidth-1);

		if (pixNum > 0)
		{
			leftCoord = CalcFromXYtoLongLat(m_rectXYInlevel.x2, level);
			for (int y = m_rectXYInlevel.y1; y < m_rectXYInlevel.y2+1; y++)
			{
				double yFCoord = CalcFromXYtoLongLat(y, level); double ySCoord = CalcFromXYtoLongLat(y+1, level);
				QString ext; 
				int vExtPix, hExtPix; hExtPix = gap/tileWidth*256;
				if (yFCoord < _Bottom)
				{
					int rPixNum = (_Top - ySCoord)/abs(_ySolution); ext = "_TL.jpg";
					subArea.setTop(rPixNum); subArea.setBottom(imgHeight-1);
					vExtPix = (ySCoord - _Bottom)/tileWidth*256; topCoord = ySCoord;
				}
				else if (ySCoord > _Top)
				{
					int rPixNum = (_Top - yFCoord)/abs(_ySolution); ext = "_BL.jpg";
					subArea.setTop(0); subArea.setBottom(rPixNum-1);
					vExtPix = (_Top-yFCoord)/tileWidth*256; topCoord = _Top;
				}
				else
				{
					int pixNum1 = (_Top-yFCoord)/abs(_ySolution); int pixNum2 = (_Top - ySCoord)/abs(_ySolution); ext = "_LE.jpg";
					subArea.setTop(pixNum2); subArea.setBottom(pixNum1-1);
					vExtPix = 256; topCoord = ySCoord;
				}
				QString saveStr = m_tmpDir + "/" + QString().setNum(level) + "/";
				if (level > 7)
				{
					int xS = m_rectXYInlevel.x2/128;
					int yS = y/128;
					saveStr = saveStr + QString().setNum(xS) + "_" + QString().setNum(yS) + "_" + QString().setNum(level) + "/";
				}
				if (!QFile::exists(saveStr)) {
					QDir tmp;
					tmp.mkpath(saveStr);
				}
				saveStr = saveStr + QString().setNum(m_rectXYInlevel.x2) + "_" + QString().setNum(y) + "_" +QString().setNum(level) + ext;
				QImage mgImg(subArea.size(), QImage::Format_RGB888);
				mgImg = srcImg->copy(subArea);
				QImage svImg = mgImg.scaled(hExtPix, vExtPix, Qt::IgnoreAspectRatio, Qt::FastTransformation);
				if (svImg.save(saveStr, "jpg", 100))
					WriteMarginImageInfo(saveStr, leftCoord, topCoord, pixWidth);
			}
		}
	}

	gap = _Bottom - CalcFromXYtoLongLat(m_rectXYInlevel.y1, level);
	if (gap > abs(_ySolution))
	{
		gap = CalcFromXYtoLongLat(m_rectXYInlevel.y1+1, level) - _Bottom;
		pixNum = gap/abs(_ySolution);
		subArea.setTop(imgHeight-pixNum-1); subArea.setBottom(imgHeight-1);
		topCoord = CalcFromXYtoLongLat(m_rectXYInlevel.y1 + 1, level);
		for (int x = m_rectXYInlevel.x1; x < m_rectXYInlevel.x2 + 1; x++)
		{
			double xFCoord = CalcFromXYtoLongLat(x, level);	double xSCoord = CalcFromXYtoLongLat(x+1, level);
			int vExtPix, hExtPix;
			vExtPix = 256*gap/tileWidth; 
			QString ext;
			if (xFCoord < _Left)
			{
				int rPixNum = (xSCoord - _Left)/_xSolution; ext = "_TL.jpg";
				subArea.setLeft(0); subArea.setRight(rPixNum);  
				hExtPix = 256*(xSCoord - _Left)/tileWidth; leftCoord = _Left;
			}
			else if (xSCoord > _Right)
			{
				int leftPixNum = (xFCoord - _Left)/_xSolution; hExtPix = (_Right - xFCoord)/tileWidth*256;
				subArea.setLeft(leftPixNum); subArea.setRight(imgWidth-1);
				ext = "_TR.jpg"; leftCoord = xFCoord;
			}
			else
			{
				int hPixNum1 = (xFCoord - _Left)/_xSolution;	int hPixNum2 = (xSCoord - _Left)/_xSolution;
				subArea.setLeft(hPixNum1); subArea.setRight(hPixNum2); 
				ext = "_TO.jpg"; hExtPix = 256;  leftCoord = xFCoord;
			}
			QString saveStr = m_tmpDir + "/" + QString().setNum(level) + "/";
			if (level > 7)
			{
				int xS = x/128;
				int yS = m_rectXYInlevel.y1/128;
				saveStr = saveStr + QString().setNum(xS) + "_" + QString().setNum(yS) + "_" + QString().setNum(level) + "/";
			}
			if (!QFile::exists(saveStr)) {
				QDir tmp;
				tmp.mkpath(saveStr);
			}
			saveStr = saveStr + QString().setNum(x) + "_" + QString().setNum(m_rectXYInlevel.y1) + "_" +QString().setNum(level) + ext;
			QImage mgImg(subArea.size(), QImage::Format_RGB888);
			mgImg = srcImg->copy(subArea);
			QImage svImg = mgImg.scaled(hExtPix, vExtPix, Qt::IgnoreAspectRatio, Qt::FastTransformation);
			if (svImg.save(saveStr, "jpg", 100))
				WriteMarginImageInfo(saveStr, leftCoord, topCoord, pixWidth);
		}
	}

	gap = _Top - CalcFromXYtoLongLat(m_rectXYInlevel.y2, level);
	if (gap > abs(_ySolution))
	{
		pixNum = gap/abs(_ySolution);
		topCoord = _Top;	
		subArea.setTop(0); subArea.setBottom(pixNum-1);

		for (int x = m_rectXYInlevel.x1; x < m_rectXYInlevel.x2 + 1; x++)
		{
			double xFCoord = CalcFromXYtoLongLat(x, level);	double xSCoord = CalcFromXYtoLongLat(x+1, level);
			int vExtPix, hExtPix;
			vExtPix = 256*gap/tileWidth; 
			QString ext;
			if (xFCoord < _Left)
			{
				int rPixNum = (xSCoord - _Left)/_xSolution; ext = "_BL.jpg";
				subArea.setLeft(0); subArea.setRight(rPixNum);
				hExtPix = 256*(xSCoord - _Left)/tileWidth; leftCoord = _Left;
			}
			else if (xSCoord > _Right)
			{
				int leftPixNum = (xFCoord - _Left)/_xSolution; hExtPix = (_Right - xFCoord)/tileWidth*256;
				subArea.setLeft(leftPixNum); subArea.setRight(imgWidth-1);
				ext = "_BR.jpg"; leftCoord = xFCoord;
			}
			else
			{
				int hPixNum1 = (xFCoord - _Left)/_xSolution;	int hPixNum2 = (xSCoord - _Left)/_xSolution;
				subArea.setLeft(hPixNum1); subArea.setRight(hPixNum2); 
				ext = "_BO.jpg"; hExtPix = 256; leftCoord =xFCoord;
			}
			QString saveStr = m_tmpDir + "/" + QString().setNum(level) + "/";
			if (level > 7)
			{
				int xS = x/128;
				int yS = m_rectXYInlevel.y2/128;
				saveStr = saveStr + QString().setNum(xS) + "_" + QString().setNum(yS) + "_" + QString().setNum(level) + "/";
			}
			if (!QFile::exists(saveStr)) {
				QDir tmp;
				tmp.mkpath(saveStr);
			}
			saveStr = saveStr + QString().setNum(x) + "_" + QString().setNum(m_rectXYInlevel.y2) + "_" +QString().setNum(level) + ext;
			QImage mgImg(subArea.size(), QImage::Format_RGB888);
			mgImg = srcImg->copy(subArea);
			QImage svImg = mgImg.scaled(hExtPix, vExtPix, Qt::IgnoreAspectRatio, Qt::FastTransformation);
			if (svImg.save(saveStr, "jpg", 100))
				WriteMarginImageInfo(saveStr, leftCoord, topCoord, pixWidth);
		}
	}
}
void LatLongToHopeImage::ProcessingMarginImages()
{
	ui.marginProcessBtn->setEnabled(false);
	QDir tmpDir(m_tmpDir);
	QString saveDir = ui.actionDestinateDirEdit->text();
	QStringList levelDirList = tmpDir.entryList();
	for (int ii = 0; ii<levelDirList.size(); ii++)
	{
		QString levelFolder = levelDirList.at(ii);
		if ((levelFolder == ".") || (levelFolder == "..")) continue;
		QDir levelDir(m_tmpDir + "/" + levelFolder);
		bool ok;
		int level = levelFolder.toInt(&ok);
		if (!ok) continue;
		QStringList subDirList = levelDir.entryList();

		ui.actionTotalProgressBar->setMinimum(0);
		ui.actionTotalProgressBar->setMaximum(subDirList.size());
		for (int k = 0; k < subDirList.size(); k++)
		{
			QString subFolder = subDirList.at(k);
			if ((subFolder == ".") || (subFolder == "..")) continue;
			QString curDirPath = m_tmpDir + "/" + levelFolder + "/" + subFolder;
			QDir workDir(curDirPath);
			QString ext = "*.jpg";
			QStringList imgFileList = workDir.entryList(QStringList(ext));
			QStringList doneFileList;

			QString savePath = saveDir + "/" + levelFolder + "/" + subFolder+ "/";
			ui.actionProgressBar->setMinimum(0);
			ui.actionProgressBar->setMaximum(imgFileList.size());
			for (int i = 0; i< imgFileList.size(); i++)
			{
				QImage curImg;
				QString imgName = imgFileList.at(i);
				if (doneFileList.contains(imgName));
				if (imgName.split("_").size() < 4) continue;
				QString orientStr = imgName.split("_").at(3);
				orientStr = orientStr.left(orientStr.length() - 4);	
				QImage saveImage(256,256,QImage::Format_RGB888);
				QString pairFileName = imgName.left(imgName.length() - 7);
				bool fourTile = false; bool processed= false;

				if (orientStr == "BO")
				{

					pairFileName = pairFileName + "_TO.jpg";
					QString pairFilePath =curDirPath + "/" + pairFileName;
					if (!QFile::exists(pairFilePath)) continue;
					QImage pairImg;
					if (!pairImg.load(pairFilePath)) continue;
					if (!curImg.load(curDirPath + "/" + imgName)) continue;
					pairImg = pairImg.scaled(256, 256 - curImg.height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
					for (int wx = 0; wx < 256; wx++)
					{
						for (int wy = 0; wy < pairImg.height(); wy++) {
							QRgb pix(pairImg.pixel(wx, wy));
							saveImage.setPixel(wx, wy, pix);
						}
						for (int wy = 0; wy < curImg.height(); wy++){
							QRgb pix(curImg.pixel(wx, wy));
							saveImage.setPixel(wx, wy + pairImg.height(), pix);
						}
					} // end of for - wx
					processed = true;
				} // if (orientStr == "BO")
				else if (orientStr == "LE")
				{
					pairFileName = pairFileName + "_RI.jpg";
					QString pairFilePath =curDirPath + "/" + pairFileName;
					if (!QFile::exists(pairFilePath)) continue;
					QImage pairImg;
					if (!pairImg.load(pairFilePath)) continue;
					if (!curImg.load(curDirPath + "/" + imgName)) continue;
					pairImg = pairImg.scaled(256 - curImg.width(), 256, Qt::IgnoreAspectRatio, Qt::FastTransformation);
					for (int wy = 0; wy < 256; wy++)
					{	
						for (int wx = 0; wx < curImg.width(); wx++){
							QRgb pix(curImg.pixel(wx, wy));
							saveImage.setPixel(wx, wy, pix);
						}
						for (int wx = 0; wx < pairImg.width(); wx++) {
							QRgb pix(pairImg.pixel(wx, wy));
							saveImage.setPixel(wx+curImg.width(), wy, pix);
						}
					} // end of for - wy
					processed = true;
				}// else if (orientStr == "LE")

				if (orientStr == "BL")
				{
					fourTile = true;
					QString brName = pairFileName + "_BR.jpg"; 	QString tlName = pairFileName + "_TL.jpg"; 	QString trName = pairFileName + "_TR.jpg";
					QString brPath = curDirPath + "/" + brName; QString tlPath = curDirPath +  "/" + tlName; 	QString trPath = curDirPath +  "/" + trName;
					if (!QFile::exists(brPath)) continue;	if (!QFile::exists(trPath)) continue;	if (!QFile::exists(tlPath)) continue;
					QImage brImg, tlImg, trImg;
					if (!brImg.load(brPath)) continue; 	if (!trImg.load(trPath)) continue;	if (!tlImg.load(tlPath)) continue;
					if (!curImg.load(curDirPath + "/" + imgName)) continue;
					trImg = trImg.scaled(trImg.width(), 256 - curImg.height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
					tlImg = tlImg.scaled(256 - trImg.width(), 256 - curImg.height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
					brImg = brImg.scaled(256 - curImg.width(), curImg.height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
					for (int wx=0; wx<tlImg.width(); wx++){
						for (int wy=0; wy<tlImg.height(); wy++){
							QRgb pix(tlImg.pixel(wx, wy));
							saveImage.setPixel(wx,wy, pix);
						}
					}
					for (int wx=0; wx<trImg.width(); wx++){
						for (int wy=0; wy<trImg.height(); wy++){
							QRgb pix(trImg.pixel(wx, wy));
							saveImage.setPixel(wx + tlImg.width(),wy, pix);
						}
					}
					for (int wy = 0; wy<curImg.height(); wy++)	{
						for (int wx=0; wx<curImg.width(); wx++) {
							QRgb pix(curImg.pixel(wx,wy));
							saveImage.setPixel(wx, wy + trImg.height(), pix);
						}
						for (int wx=0; wx<brImg.width(); wx++){
							QRgb pix(brImg.pixel(wx,wy));
							saveImage.setPixel(wx + curImg.width(), wy + trImg.height(), pix);
						}
					}
					processed = true;
					doneFileList<<brName; doneFileList<<tlName; doneFileList<<trName;
				} // if (orientStr == "BL")
				if (processed)
				{
					if (!QFile::exists(savePath))
					{
						QDir ttmp;
						ttmp.mkpath(savePath);
					}
					QString saveFPath = savePath + imgName.left(imgName.length() - 7) + ".jpg";
					//saveImage.save(savePath, "jpg", 100);
					//Normalize(saveImage, saveFPath);
					if (!fourTile) doneFileList<<pairFileName;
					doneFileList<<imgName;
					if (m_writeJGW)
					{
						int xVal = imgName.split("_").at(0).toInt();
						int yVal = imgName.split("_").at(1).toInt();
						QString jgwFilePath = saveFPath.left(saveFPath.length() - 4) + ".jgw";
						WriteJgwFile(xVal, yVal, level, jgwFilePath);
					}
				}
				ui.actionProgressBar->setValue(i+1);
				qApp->processEvents();
			} // for (int i = 0; i< imgFileList.size(); i++) 
			for (int i = 0; i< doneFileList.size(); i++)
			{
				QString imgFileName = curDirPath + "/" + doneFileList.at(i);
				workDir.remove(imgFileName);
				workDir.remove(imgFileName.left(imgFileName.length()-4) + ".jgw");

			}
			levelDir.rmdir(m_tmpDir + "/" + levelFolder + "/" + subDirList.at(k));

			ui.actionTotalProgressBar->setValue(k+1);
		} // for (int k = 0; k < subDirList.size(); k++)
		levelDir.rmdir(m_tmpDir + "/" + levelFolder);
	} // for (int ii = 0; ii<levelDirList.size(); ii++)
	tmpDir.rmdir(m_tmpDir);
	QMessageBox(QMessageBox::Information,"Process","Sueccess OK!!!",QMessageBox::Ok,this,Qt::Dialog).exec();
	ui.actionProgressBar->setValue(0);
	ui.actionTotalProgressBar->setValue(0);
	ui.marginProcessBtn->setEnabled(true);
}

void LatLongToHopeImage::WriteMarginImageInfo(QString imgPath, double left, double top, double pixWidth)
{
	QString filepath = imgPath.left(imgPath.length() - 4) + ".jgw";

	QString _leftStr, _topStr;
	_leftStr.setNum(left,'f',16); _topStr.setNum(top,'f',16);
	QString _xResolStr, _yResolStr, zeroStr;
	_xResolStr.setNum(pixWidth,'f',16); _yResolStr = "-" + _xResolStr;
	zeroStr.setNum(0.0f, 'f',16);

	QByteArray jgwNameArray = filepath.toAscii();
	const char *jgwNamedata = jgwNameArray.constData();

	std::fstream jgwfp;
	jgwfp.open(jgwNamedata,std::ios::out);
	if (!jgwfp.good())	return;

	const char _enter = (char)10;
	const char *_leftdata = _leftStr.toAscii().constData();
	const char *_topdata = _topStr.toAscii().constData();
	const char *_xResoldata = _xResolStr.toAscii().constData();
	const char *_yResoldata = _yResolStr.toAscii().constData();
	const char *zeroStrdata = zeroStr.toAscii().constData();

	jgwfp.write(_xResoldata, _xResolStr.toAscii().length()); jgwfp.write(&_enter,1);	
	jgwfp.write(zeroStrdata, zeroStr.toAscii().length()); jgwfp.write(&_enter,1);	
	jgwfp.write(zeroStrdata, zeroStr.toAscii().length()); jgwfp.write(&_enter,1);	
	jgwfp.write(_yResoldata, _yResolStr.toAscii().length()); 	jgwfp.write(&_enter,1);	
	jgwfp.write(_leftdata, _leftStr.toAscii().length());	jgwfp.write(&_enter,1);
	jgwfp.write(_topdata, _topStr.toAscii().length());
	jgwfp.close();
	
}
void LatLongToHopeImage::RegisterInformation()
{
	ui.actionLeftEdit->setText(QString().setNum(_Left)); ui.actionTopEdit->setText(QString().setNum(_Top));

	_destLeft = _Left;  _destRight  = _Right;
	_destTop = _Top;	 _destBottom = _Bottom;	

	QString tmpStr;
	tmpStr.setNum(_Right);	ui.actionRightEdit->setText(tmpStr);  ui.actionDestRightEdit->setText(tmpStr);
	tmpStr.setNum(_Bottom);  ui.actionBottomEdit->setText(tmpStr);  ui.actionDestBottomEdit->setText(tmpStr);
	tmpStr.setNum(_Left);  ui.actionDestLeftEdit->setText(tmpStr);
	tmpStr.setNum(_Top);  ui.actionDestTopEdit->setText(tmpStr);

	bool ok = 0;
	m_startLevel = ui.actionStartLevelEdit->text().toInt( &ok,10);
	m_endLevel = ui.actionEndLevelEdit->text().toInt( &ok,10);
}

int LatLongToHopeImage::GetNumberOfPixelBetweenTwoPts(double startCoord, double endCoord, double resolution)
{
	int num = (endCoord - startCoord)/abs(resolution);
	return num;
}
bool LatLongToHopeImage::JGWRead(QString imgPath, double*left, double*top, double*xResolution, double*yResolution)
{
	QString jgwFile = imgPath.left(imgPath.length() - 4) + ".jgw";
	QFile file(jgwFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	else
	{
		QTextStream in(&file);
		QString line;

		double val[6];
		int k=0;
		do 
		{
			line=in.readLine();	
			if (!line.isEmpty())			
			{
				val[k] = line.toDouble();
				k++;
			}
		} while (!line.isNull());

		*left = val[4];
		*top = val[5];

		*xResolution = val[0];
		*yResolution = val[3];
		//in.flush();
	}
	file.close();
	
	return true;
}
void LatLongToHopeImage::ConvertFolder()
{	
	if (fileNameListInDir.isEmpty())
		return;
	int fileNum =fileNameListInDir.size();
	ui.actionTotalProgressBar->setMinimum(0);
	ui.actionTotalProgressBar->setMaximum(fileNum);
	for (int i=0;  i<fileNum;  i++)
	{
		QString  imgPath= fileNameListInDir.at(i);
		imgPath = ui.actionSourceFileEdit->text() + "/" + imgPath;
		ConvertFile(imgPath);
		QFile pgrFile("d:/imgWork.txt");
		if (pgrFile.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream out(&pgrFile);
			out<<i+1;
			out<<imgPath;
		}
		pgrFile.close();

		ui.actionTotalProgressBar->setValue(i+1);
	}

	ui.actionTotalProgressBar->setValue(fileNum);
	QMessageBox(QMessageBox::Information,"Process","Sueccess OK!!!",QMessageBox::Ok,this,Qt::Dialog).exec();
	ui.actionlblfilename->setText("");
	ui.convertBtn->setEnabled(true);

}
void LatLongToHopeImage::Convert(void)
{
	if (ui.actionDestinateDirEdit->text() == "")
	{
		QMessageBox(QMessageBox::Warning,"Warning","Please select destination folder!",QMessageBox::Ok,this,Qt::Dialog).exec();
		return;
	}

	if (ui.actionSourceFileEdit->text() == "")
	{
		QMessageBox(QMessageBox::Warning,"Warning","No  Source file!",QMessageBox::Ok,this,Qt::Dialog).exec();
		return;
	}

	if (m_dirSelect)
		ConvertFolder();
	else
		ConvertFile(m_srcImgPath);
	
}

void LatLongToHopeImage::getTotalRectofXYsInLevel(double left,double right,double top,double bottom,int level)
{
	
	m_rectXYInlevel.x1 = CalcFromLatLongtoXY(left,level);
	m_rectXYInlevel.x2 = CalcFromLatLongtoXY(right,level);
	m_rectXYInlevel.y1 = CalcFromLatLongtoXY(bottom,level);
	m_rectXYInlevel.y2 = CalcFromLatLongtoXY(top,level);

}

void LatLongToHopeImage::HopeImgMake(int x, int y,int level,QRectF totalRect)
{

	QRectF	workRect;
	QRect	areaRect;
			
			workRect.setLeft(CalcFromXYtoLongLat(x,level));
			workRect.setRight(CalcFromXYtoLongLat(x+1,level)-_xSolution);
			workRect.setTop(CalcFromXYtoLongLat(y,level));
			workRect.setBottom(CalcFromXYtoLongLat(y+1,level)-_ySolution);

			int L,R,B,T;
			if (	totalRect.contains(workRect.topLeft())  && totalRect.contains(workRect.bottomRight())  )
			{
			//	areaRect = getPixRect_InTotal(totalRect,workRect);
				//loadPixelData(areaRect,x,y,level);
			}
			else
			{
				/*areaRect = getPixRect_InTotalEx(totalRect,workRect);
				loadPixelDataEx(workRect,areaRect,x,y,level);*/
			}
	return;
}
QRect LatLongToHopeImage::getPixRect_InTotal(QRectF totalRect,QRectF workRect)
{
	QRect rect;		//loading pixel/line rectangle
				rect.setLeft( (workRect.left()-totalRect.left())/abs(_xSolution));
				rect.setRight( rect.left()+ workRect.width()/abs(_xSolution));
	
				rect.setTop((totalRect.bottom()-workRect.bottom())/abs(_ySolution));
				rect.setBottom( rect.top()+workRect.height()/abs(_ySolution));

	return rect;
}
QRect LatLongToHopeImage::getPixRect_InTotalEx(QRectF totalRect,QRectF workRect)
{
	QRectF rect;
	rect = totalRect.intersected(workRect);
	
	QRect rt;		//loading pixel/line rectangle
	rt.setLeft( (rect.left()-totalRect.left())/abs(_xSolution));
	rt.setRight( rect.left()+ rect.width()/abs(_xSolution));

	rt.setTop((totalRect.bottom()-rect.bottom())/abs(_ySolution));
	rt.setBottom( rect.top()+rect.height()/abs(_ySolution));
	return rt;
}
void LatLongToHopeImage::loadPixelData(QImage *srcImg, QRect areaRect,int xNum,int yNum, int level)
{
	QImage img(areaRect.size(),QImage::Format_RGB888);
	img = srcImg->copy(areaRect);
	/*QRgb  rgb;
	int x1,y1;
	for (int x =areaRect.left();x<areaRect.right()+1;x++)
		for (int y = areaRect.top();y<areaRect.bottom()+1;y++)
		{
			rgb = jpgImg.pixel(x,y);
			img.setPixel(x-areaRect.left(),y-areaRect.top(),rgb);
		}*/
	QImage saveimg = img.scaled(256,256,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

	QString strLevel;
	strLevel.setNum(level);
	QString path(ui.actionDestinateDirEdit->text());
	path.append("/" + strLevel);	

	if(!QFile::exists(path))
	{
		QDir tmp;
		tmp.mkpath(path);
	}
	if(level>7)
	{
		int nblock_x = xNum / 128;
		int nblock_y = yNum / 128;

		QString subdir;
		subdir = QString("%1_%2_%3").arg(nblock_x).arg(nblock_y).arg(level);
		path.append( "/" +subdir);
		if(!QFile::exists(path) )
		{
			QDir tmp;
			tmp.mkpath(path);
		}
	}

	QString jgwPath = path;
	jgwPath.append("/"+QString("%1_%2_%3.jgw").arg(xNum).arg(yNum).arg(level));

	path.append("/"+QString("%1_%2_%3.jpg").arg(xNum).arg(yNum).arg(level));
	
	ui.actionlblfilename->setText(QString("Level %1 : ").arg(level) + path);
	qApp->processEvents();

	int imageQuality = ui.actionImgQualityEdit->text().toInt();

	//if (!imgTestinvalid(saveimg))
	//	Normalize(saveimg,path);
	saveimg.save(path,"jpg",imageQuality);
	
	if (m_writeJGW)
		WriteJgwFile(xNum, yNum, level, jgwPath);
}


void LatLongToHopeImage::WriteJgwFile(int x,int y,int level,QString filepath)
{
	/*** Getting longitude and latitude corresponding x,y,level ***/
	double longitude,latitude;
	QString longStr,latStr;
	longitude = CalcFromXYtoLongLat(x,level); latitude = CalcFromXYtoLongLat(y+1,level);
	longStr.setNum(longitude,'f',16); latStr.setNum(latitude,'f',16);

	/*** Getting additional informations about jgw file ***/
	QString longStepStr,latStepStr,zeroStr;
	double longStep,latStep;
	longStep = (CalcFromXYtoLongLat(x+1,level) - longitude)/JPG_IMAGE_SIZE;
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
void LatLongToHopeImage::loadPixelDataEx(QRectF workRt,QRect areaRect,int xNum,int yNum, int level)
{
	QString strLevel;
	strLevel.setNum(level);
	QString path(ui.actionDestinateDirEdit->text());
	path.append("/" + strLevel);	
	if(!QFile::exists(path))
	{
		QDir tmp;
		tmp.mkdir(path);
	}
	if(level>7)
	{
		int nblock_x = xNum / 128;
		int nblock_y = yNum / 128;

		QString subdir;
		subdir = QString("%1_%2_%3").arg(nblock_x).arg(nblock_y).arg(level);
		path.append( "/" +subdir);
		if(!QFile::exists(path) )
		{
			QDir tmp;
			tmp.mkdir(path);
		}
	}
	path.append("/"+QString("%1_%2_%3.jpg").arg(xNum).arg(yNum).arg(level));

	QImage targetImg(256,256,QImage::Format_RGB888);
	if (!QFile::exists(path))
		targetImg.load(path);
	else
		targetImg.fill(0);
	
	QRect target;
	target.setWidth(workRt.width()/abs(_xSolution));
	target.setHeight(workRt.height()/abs(_ySolution));

	QImage img(target.size(),QImage::Format_RGB888);
	if(!QFile::exists(path))
	{
		QDir tmp;
		tmp.mkdir(path);
	}
	QRgb  rgb;
	int x1,y1;

	for (int x =areaRect.left();x<areaRect.right()+1;x++)
		for (int y = areaRect.top();y<areaRect.bottom()+1;y++)
		{
			rgb = jpgImg.pixel(x,y);
			img.setPixel(x-areaRect.left(),y-areaRect.top(),rgb);
		}

	QImage saveimg = img.scaled(256,256,Qt::IgnoreAspectRatio,Qt::FastTransformation);

	ui.actionlblfilename->setText(QString("Level %1 : ").arg(level) + path);
	qApp->processEvents();
	int imageQuality = ui.actionImgQualityEdit->text().toInt();

	saveimg.save(path,"jpg",imageQuality);
}
bool LatLongToHopeImage::imgTestinvalid(QImage img)
{
	double R = 0.0;
	double G = 0.0;
	double B = 0.0;
	QRgb  rgb;
	QSize imgSize;
	imgSize = img.size();
	for (int x =0;x<imgSize.width();x++)
		for (int y = 0;y<imgSize.height();y++)
		{
			rgb = img.pixel(x,y);
			R += (double)qRed(rgb);
			G += (double)qGreen(rgb);
			B += (double)qBlue(rgb);
		}
		int iR,mR = R /(imgSize.width() * imgSize.height());
		int iG,mG = G /(imgSize.width() * imgSize.height());
		int iB,mB = B /(imgSize.width() * imgSize.height());
		//number test
		QString strVal(ui.actionIgnoreColorEdit->text());
		QString hex("0x");
		if (strVal.indexOf(hex,0,Qt::CaseInsensitive) == 0)//hex number
		{
			QString sR = strVal.mid(strVal.length()-6,2);
			QString sG = strVal.mid(strVal.length()-4,2);
			QString sB = strVal.mid(strVal.length()-2,2);
			bool ok;
			iR = sR.toInt( &ok,16);
			iG = sG.toInt( &ok,16);
			iB = sB.toInt( &ok,16);
		}	
		else		//decimal number
		{
				bool ok;
				double ignorecolor = strVal.toDouble(&ok);
				iR = qRed( (QRgb)ignorecolor);
				iG = qGreen( (QRgb)ignorecolor);
				iB = qBlue( (QRgb)ignorecolor);
		}
		
		if ((iR>mR*0.98)&& (iR<mR*1.02)&&
			(iG>mG*0.98)&& (iG<mG*1.02)&&
			(iB>mB*0.98)&& (iB<mB*1.02))
			return true;
		return false;
}
void LatLongToHopeImage::getImgQuality()
{
	int imageQuality = ui.actionImgQualityEdit->text().toInt();
	if (imageQuality<1)		imageQuality =1;
	if (imageQuality>100)		imageQuality =100;
	QString str;
	str.setNum(imageQuality);
	ui.actionImgQualityEdit->setText(str);
}
void LatLongToHopeImage::getIgnoreColor()
{
	
}
void LatLongToHopeImage::setDirectorySelect()
{
	if (m_dirSelect)
	{
		m_dirSelect = false;
		ui.label->setText("Source File");
	}
	else
	{
		m_dirSelect = true;
		ui.label->setText("Source Directory");
	}
}

void LatLongToHopeImage::setJGW_Append()
{
	if (m_writeJGW)
		m_writeJGW = false;
	else
		m_writeJGW = true;
}
void LatLongToHopeImage::setTmpDir(void)
{
	QString strSavePath = QFileDialog::getExistingDirectory(this,	tr("Select folder to save marginal jpg file!"), tr("D:/tmp"));

	if (!strSavePath.isEmpty()) 
	{
		ui.tmpDirEdit->setText(strSavePath);	
		m_tmpDir = strSavePath;
	}
}