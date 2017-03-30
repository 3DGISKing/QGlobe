#include "gdemdbextractdlg.h"
#include <QFileDialog>
#include <math.h>


#include "../gdem_compress/LzmaLib.h"
#include "common.h"

#define JPG_IMAGE_SIZE 256


bool getTileInfoFromFileName(QString filename ,int *x,int *y,int *level)
{
	//for example filename 0_0_1.gt
	QStringList list;
	list=filename.split("_");//list 0,0,1.gt
	if(list.size()<3)
		return false;

	QString str;
	bool ok;

	str=list.at(0);
	*x=str.toInt(&ok);
	if(!ok)
		return false;

	str=list.at(1);
	*y=str.toInt(&ok);
	if(!ok)
		return false;

	str=list.at(2);//1.gt

	list=str.split(".");

	*level=list.at(0).toInt(&ok);


	if(!ok)
		return false;
	return true;
}

int getTileCount(int level)
{
	if(level>7)
		return 128*128;
	else
		return (pow(4.0,level));
}



void deCompressBuffer(int inBufLen, void* iBuf, int outBufLen, void *oBuf)
{
	size_t	lBufferSizeCpr = 0;
	lBufferSizeCpr = inBufLen - LZMA_PROPS_SIZE - sizeof(size_t);
	LzmaUncompress((unsigned char *)oBuf, 
		(size_t*)&outBufLen, 
		(unsigned char *)iBuf + LZMA_PROPS_SIZE + sizeof(size_t), 
		&lBufferSizeCpr, 
		(unsigned char *)iBuf + sizeof(size_t),
		*((size_t*)iBuf));
}

GDemDbExtractDlg::GDemDbExtractDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	_isRunning=false;
	_dataType=Image;
	_fileExtension=GT_FILE_EXTENSION;

	if(ui.radioButtonImage->isChecked())
	{
		_fileExtension=GT_FILE_EXTENSION;
		_dataType=Image;
	}
	if(ui.radioButtonDem->isChecked())
	{
		_fileExtension=DEM_FILE_EXTENSION;
		_dataType=Dem;
	}
	if (ui.radioButtonShape->isChecked())
	{
		_fileExtension=SHAPE_FILE_EXTENSION;
		_dataType=Shape;
	}

	if (ui.actionJGWmaker->isChecked())
		_blWriteJgw=true;
	else
		_blWriteJgw=false;

	if(ui.groupBoxBounding->isChecked())
		_blBounding=true;
	else
		_blBounding=false;

	_strInputPath=QDir::rootPath();
	ui.lineEditInputPath->setText(_strInputPath);

	_strSavePath=QDir::tempPath();
	ui.lineEditSavePath->setText(_strSavePath);

	ui.progressBarGDemCount->setVisible(false);
	ui.progressBarTileCount->setVisible(false);
	ui.labelGDemFileName->setVisible(false);
	ui.labelTileFileName->setVisible(false);
	ui.pushButtonCancel->setVisible(false);

	connect(ui.pushButtonBrowserInput,SIGNAL(clicked()),this,SLOT(browserInput()));
	connect(ui.pushButtonBrowserSave,SIGNAL(clicked()),this,SLOT(browserSave()));
	connect(ui.pushButtonExtract,SIGNAL(clicked()),this,SLOT(extract()));
	connect(ui.pushButtonCancel,SIGNAL(clicked()),this,SLOT(cancel()));
	connect(ui.radioButtonImage,SIGNAL(toggled(bool)),this,SLOT(onToggleImage(bool)));
	connect(ui.radioButtonDem,SIGNAL(toggled(bool)),this,SLOT(onToggleDem(bool)));
	connect(ui.radioButtonShape,SIGNAL(toggled(bool)),this,SLOT(onToggleShape(bool)));
	connect(ui.actionJGWmaker,SIGNAL(toggled(bool)),this,SLOT(onToggleJgw(bool)));

	connect(ui.lineEditLeft,SIGNAL(textChanged(const QString&)),this,SLOT(onLeftChanged(const QString &)));
	connect(ui.lineEditRight,SIGNAL(textChanged(const QString&)),this,SLOT(onRightChanged(const QString &)));

	connect(ui.lineEditTop,SIGNAL(textChanged(const QString&)),this,SLOT(onTopChanged(const QString &)));
	connect(ui.lineEditBottom,SIGNAL(textChanged(const QString&)),this,SLOT(onBottomChanged(const QString &)));


	connect(ui.lineEditInputPath,SIGNAL(textChanged(const QString&)),this,SLOT(onInputPathChanged(const QString &)));
	connect(ui.lineEditSavePath,SIGNAL(textChanged(const QString&)),this,SLOT(onSavePathChanged(const QString &)));

	
	connect(ui.groupBoxBounding,SIGNAL(toggled(bool)),this,SLOT(onToggleBounding(bool)));
}

GDemDbExtractDlg::~GDemDbExtractDlg()
{

}

void GDemDbExtractDlg::browserInput()
{
	QString tmppath=QFileDialog::getExistingDirectory(this,	tr("Select folder where exists GDem files!"), _strInputPath);

	if(!tmppath.isEmpty())
	{
		_strInputPath = tmppath;
		ui.lineEditInputPath->setText(_strInputPath);
	}
}

void GDemDbExtractDlg::browserSave()
{
	QString tmppath=QFileDialog::getExistingDirectory(this,	tr("Open folder where exists Gt file!"), _strSavePath);

	if (!tmppath.isEmpty()) 
	{
		_strSavePath =tmppath;
		ui.lineEditSavePath->setText(_strSavePath);
	}
}

void GDemDbExtractDlg::onToggleImage(bool checked)
{
	if(checked)
	{
		_dataType=Image;
		_fileExtension=GT_FILE_EXTENSION;
		ui.actionJGWmaker->setEnabled(true);
	}
	else
		ui.actionJGWmaker->setEnabled(false);
}


void GDemDbExtractDlg::onToggleDem(bool checked)
{
	if(checked)
	{
		_dataType=Dem;
		_fileExtension=DEM_FILE_EXTENSION;
	}
}


void GDemDbExtractDlg::onToggleShape(bool checked)
{
	if(checked)
	{
		_dataType=Shape;
		_fileExtension=SHAPE_FILE_EXTENSION;
	}
}


void GDemDbExtractDlg::onToggleBounding(bool checked)
{
	if(checked)
		_blBounding=true;
	else
		_blBounding=false;
}
void GDemDbExtractDlg::onToggleJgw(bool checked)
{
	if(checked)
		_blWriteJgw=true;
	else
		_blWriteJgw=false;
}


void GDemDbExtractDlg::onLeftChanged(const QString& text)
{
	bool ok;

	double tmp=text.toDouble(&ok);

	if(!ok)
	{
		ui.lineEditLeft->selectAll();
		messageBox("Invalid!");
		return;
	}

	if(tmp<-180 || tmp>180)
	{
		ui.lineEditLeft->selectAll();
		messageBox("Invalid!");
		return;
	}
	_left=tmp;
}

void GDemDbExtractDlg::onRightChanged(const QString& text)
{
	bool ok;

	double tmp=text.toDouble(&ok);

	if(!ok)
	{
		ui.lineEditRight->selectAll();
		messageBox("Invalid!");
		return;
	}

	if(tmp<-180 || tmp>180)
	{
		ui.lineEditRight->selectAll();
		messageBox("Invalid!");
		return;
	}
	_right=tmp;
}

void GDemDbExtractDlg::onTopChanged(const QString& text)
{
	bool ok;

	double tmp=text.toDouble(&ok);

	if(!ok)
	{
		ui.lineEditTop->selectAll();
		messageBox("Invalid!");
		return;
	}

	if(tmp<-90 || tmp>90)
	{
		ui.lineEditTop->selectAll();
		messageBox("Invalid!");
		return;
	}
	_top=tmp;
}

void GDemDbExtractDlg::onBottomChanged(const QString& text)
{
	bool ok;

	double tmp=text.toDouble(&ok);

	if(!ok)
	{
		ui.lineEditBottom->selectAll();
		messageBox("Invalid!");
		return;
	}

	if(tmp<-90 || tmp>90)
	{
		ui.lineEditBottom->selectAll();
		messageBox("Invalid!");
		return;
	}
	_bottom=tmp;
}

bool GDemDbExtractDlg::intersectGDemFile(int m,int n,int level)
{
	int minx=m*128;
	int maxx=minx+127;
	int miny=n*128;
	int maxy=miny+127;

	double left=CalcFromXYtoLongLat(minx,level);
	double right=CalcFromXYtoLongLat(maxx,level);
	double bottom=CalcFromXYtoLongLat(miny,level);
	double top=CalcFromXYtoLongLat(maxy,level);

	QRectF rect1; 
	rect1.setLeft(left); rect1.setRight(right);
	rect1.setTop(top);   rect1.setBottom(bottom);

	QRectF rect2;

	rect2.setLeft(_left); rect2.setRight(_right);
	rect2.setTop(_top);   rect2.setBottom(_bottom);
	
	return rect1.intersects(rect2);
}

bool GDemDbExtractDlg::intersectTile(int col,int row,int level)
{
	double left=CalcFromXYtoLongLat(col,level);
	double right=CalcFromXYtoLongLat(col+1,level);
	double bottom=CalcFromXYtoLongLat(row,level);
	double top=CalcFromXYtoLongLat(row+1,level);

	QRectF rect1; 
	rect1.setLeft(left); rect1.setRight(right);
	rect1.setTop(top);   rect1.setBottom(bottom);

	QRectF rect2;

	rect2.setLeft(_left); rect2.setRight(_right);
	rect2.setTop(_top);   rect2.setBottom(_bottom);

	return rect1.intersects(rect2);
}

void GDemDbExtractDlg::extract()
{
	QDir inputDir = QDir(_strInputPath);        //for example "geotexture/13"
	if(!inputDir.exists())
	{
		messageBox(QString("Directory: %1 does not exist! Please try once again").arg(_strInputPath));
		return;
	}

	QString extension="*."+_fileExtension;
	QStringList gdemfiles;
	gdemfiles = inputDir.entryList(QStringList(extension),QDir::Files | QDir::NoSymLinks);

	if(gdemfiles.size()==0)
	{
		messageBox("No file does not exist!");
		return;
	}

	_isRunning=true;
	
	setRunnningState(true);
	ui.progressBarGDemCount->setRange(0,gdemfiles.size()-1);

	for(int i=0;i<gdemfiles.size();i++)
	{
		qApp->processEvents();
		ui.progressBarGDemCount->setValue(i);
		extractOneGDemFile(gdemfiles.at(i));
	}

	_isRunning=false;
	setRunnningState(false);
	messageBox("Finished!");
}

void GDemDbExtractDlg::cancel()
{
	_isRunning=false;
	setRunnningState(false);
}

bool GDemDbExtractDlg::extractOneGDemFile(QString filename)
{
	int x,y,level;

	if(!getTileInfoFromFileName(filename,&x,&y,&level))
		return false;
	
	if(_blBounding)
	if(!intersectGDemFile(x,y,level))
		return false;

	QString strlevel;
	strlevel.sprintf("%d",level);

	QString savepath=QString(_strSavePath+"/%1").arg(level);
	if(!QFile::exists(savepath)) //make save directory. For example "C:/tmp/13" 
	{
		QDir tmp;
		tmp.mkdir(savepath);
	}

	if(level>7)
	{
		savepath=savepath+"/"+QString("%1_%2_%3").arg(x).arg(y).arg(level);
	
		if(!QFile::exists(savepath)) //make save directory. For example "C:/tmp/13" 		
		{
			QDir tmp;
			tmp.mkdir(savepath);
		}
	}

	int filecount=getTileCount(level);

	QFile gdemfile(_strInputPath+"/"+filename);

	if(!gdemfile.exists()) return false;

	if(!gdemfile.open(QIODevice::ReadOnly)) return false;

	ui.labelGDemFileName->setText("Processing "+_strInputPath+"/"+filename+"...");

	QDataStream in(&gdemfile);
	in.setByteOrder(QDataStream::LittleEndian);
	int readlevel,readfilecount;

	in>>(qint32)readlevel;
	in>>(qint32)readfilecount;

	if(level!=readlevel || filecount!= readfilecount) 
		return false;

	int rows;
	if(level>7)	
		rows=128;
	else
		rows=pow(2.0,level);

	int offset ,size;

	ui.progressBarTileCount->setRange(0,rows*rows);

	for(int i=0;i<filecount;i++)
	{
		qApp->processEvents();
		if(!_isRunning)
		{
			gdemfile.close();
			return true;
		}
		ui.progressBarTileCount->setValue(i+1);

		in>>(qint32)offset;
		in>>(qint32)size;

		if(size==0) continue;

		int col=i % rows;
		int row=i / rows;

		if(level>7)
		{
			col+=x*128;
			row+=y*128;
		}
		
		if(_blBounding)
		if(!intersectTile(col,row,level))
			continue;

		QString filename;
		filename.sprintf("%d_%d_%d.",col,row,level);
		if(_dataType==Image)
			filename=filename+"jpg";

		QFile file(savepath+"/"+filename);

		ui.labelTileFileName->setText("Generating "+savepath+"/"+filename+"...");

		if(QFile::exists(savepath+"/"+filename))
			QFile::remove(savepath+"/"+filename);

		if(!file.open(QIODevice::ReadWrite)) 
			continue;

		unsigned char* datapointer=NULL;
		datapointer=gdemfile.map(offset,size);

		if(!datapointer)  continue;

		unsigned char *file_raw_data=NULL;

		if(_dataType==Dem)
		{
			file_raw_data=new unsigned char[2*129*129];
			memset(file_raw_data, 0,2*129*129 );

			deCompressBuffer(size,datapointer,33456,file_raw_data);
			file.write((const char*)file_raw_data,2*129*129);
		}
		else
		{
			file_raw_data=new unsigned char[size]; 
			memcpy(file_raw_data,datapointer,size);

			file.write((const char*)file_raw_data,size);
		}

		file.close();
		delete	[] file_raw_data;
		gdemfile.unmap(datapointer);

		if(_dataType==Image && _blWriteJgw)
		{
			QString jgwfilename;
			jgwfilename.sprintf("%d_%d_%d.jgw",col,row,level);

			jgwfilename=savepath+"/"+jgwfilename;
			writeJgw(col,row,level,jgwfilename);
		}
	}

	gdemfile.close();
	return true;
}

void GDemDbExtractDlg::onInputPathChanged(const QString& text)
{
	_strInputPath=text;
}

void GDemDbExtractDlg::onSavePathChanged(const QString& text)
{
	_strSavePath=text;
}

void GDemDbExtractDlg::setRunnningState(bool state)
{
	if(state)
	{
		ui.pushButtonExtract->setVisible(false);
		ui.pushButtonBrowserInput->setEnabled(false);
		ui.pushButtonBrowserSave->setEnabled(false);
		ui.pushButtonCancel->setVisible(true);
		ui.progressBarGDemCount->setVisible(true);
		ui.progressBarTileCount->setVisible(true);
		ui.labelGDemFileName->setVisible(true);
		ui.labelTileFileName->setVisible(true);
	}
	else
	{
		ui.pushButtonExtract->setVisible(true);
		ui.pushButtonBrowserInput->setEnabled(true);
		ui.pushButtonBrowserSave->setEnabled(true);
		ui.pushButtonCancel->setVisible(false);
		ui.progressBarGDemCount->setVisible(false);
		ui.progressBarTileCount->setVisible(false);
		ui.labelGDemFileName->setVisible(false);
		ui.labelTileFileName->setVisible(false);
	}
}