#include "imgtilemergedlg.h"
#include <QFileDialog>
#include "common.h"

ImgTileMergeDlg::ImgTileMergeDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	_inputPath=QDir::rootPath();
	_savePath=QDir::tempPath();

	ui.lineEditInput->setText(_inputPath);
	ui.lineEditSave->setText(_savePath);

	_minX=_minY=_maxX=_maxY=0;
	ui.progressBarJpgFile->setVisible(false);
	
	connect(ui.pushButtonInput,SIGNAL(clicked()),this,SLOT(onClickedInput()));
	connect(ui.pushButtonSave,SIGNAL(clicked()),this,SLOT(onClickedSave()));
	connect(ui.pushButtonMerge,SIGNAL(clicked()),this,SLOT(onClickedMerge()));
}	

ImgTileMergeDlg::~ImgTileMergeDlg()
{
	
}

void ImgTileMergeDlg::onClickedInput()
{
	QString tmppath=QFileDialog::getExistingDirectory(this,	tr("Select folder where exists jpg files!"), _inputPath);

	if(!tmppath.isEmpty())
	{
		_inputPath = tmppath;
		ui.lineEditInput->setText(_inputPath);
	}
}

void ImgTileMergeDlg::onClickedSave()
{
	QString tmppath=QFileDialog::getExistingDirectory(this,	tr("Select folder where will be saved merged jpg file!"), _savePath);

	if(!tmppath.isEmpty())
	{
		_savePath = tmppath;
		ui.lineEditSave->setText(_savePath);
	}
}

void ImgTileMergeDlg::onClickedMerge()
{
	QDir dir(_inputPath);
	QString dirname=dir.dirName();

	QStringList tmp;
	tmp=dirname.split("_");

	if(tmp.size()<3) 
	{
		messageBox("You selected invalid directory! Select correct directory such as 17\\881_630_17 ");
		return;
	}

	ui.pushButtonInput->setEnabled(false);
	ui.pushButtonSave->setEnabled(false);
	ui.pushButtonMerge->setEnabled(false);
	ui.progressBarJpgFile->setVisible(true);

	merge();
	ui.progressBarJpgFile->setVisible(false);
	ui.pushButtonInput->setEnabled(true);
	ui.pushButtonSave->setEnabled(true);
	ui.pushButtonMerge->setEnabled(true);

	messageBox("finished!");
}

// for example dir= "D:\database\geotexture\17\881_630_17" 	
//optional: path dir must be exist

void ImgTileMergeDlg::determineTileInfo(QString path)
{
	QStringList jpgfiles;

	QDir dir(path);

	jpgfiles = dir.entryList(QStringList("*.jpg"),QDir::Files | QDir::NoSymLinks);
	jpgfiles.sort();

	QString first=jpgfiles.first();
	QString last=jpgfiles.last();

	QStringList tmp=first.split("_");

	_minX=tmp[0].toInt();
	_minY=tmp[1].toInt();

	tmp=last.split("_");

	_maxX=tmp[0].toInt();
	_maxY=tmp[1].toInt();

	_level=tmp[2].left(tmp[2].length()-4).toInt();
}

void ImgTileMergeDlg::merge()
{
	determineTileInfo(_inputPath);

	QImage im((_maxX-_minX+1)*256,(_maxY-_minY+1)*256,QImage::Format_RGB32);

	ui.progressBarJpgFile->setRange(0,(_maxX-_minX+1)*(_maxY-_minY+1)-1);

	int filecount=0;

	for(int x=_minX;x<=_maxX;x++)
		for(int y=_maxY;y>=_minY;y--)
		{
			qApp->processEvents();
			ui.progressBarJpgFile->setValue(filecount); filecount++;

			QString filename=QString("%1_%2_%3.jpg").arg(x).arg(y).arg(_level);
			
			QString path=_inputPath+"\\"+filename;

			if(!QFile::exists(path))		continue;

			QImage clip;

			if(!clip.load(path,"jpg"))		continue;

			for(int xx=0;xx<256;xx++)
				for(int yy=0;yy<256;yy++)
				{
					QRgb p=clip.pixel(xx,yy);

					im.setPixel((x-_minX)*256+xx,(_maxY-y)*256+yy,p);
				}
		}

	im.save(_savePath+"/merged.jpg","jpg");
	writeJgw(_minX,_maxY,_level,_savePath+"/merged.jgw");
}

