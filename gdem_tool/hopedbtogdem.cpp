#include "hopedbtogdem.h"

#include <QFileDialog>
#include "common.h"

#define	index_header	sizeof(qint32)*2

HopeDBToGDem::HopeDBToGDem(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Init();
}

HopeDBToGDem::~HopeDBToGDem()
{
}

void HopeDBToGDem::Init()
{	
	_strInputIEEIDBPath = QDir::rootPath();
	_strSaveGTPath = QDir::tempPath();
	ui.textEditInputIEEIDBPath->setText(_strInputIEEIDBPath);
	ui.textsaveGTPath->setText(_strSaveGTPath);

	connect(ui.pushButtonBrowserInput, SIGNAL(clicked()), this, SLOT(BrowserInput()));
	connect(ui.pushButtonBrowserSave, SIGNAL(clicked()), this, SLOT(BrowserSave()));
	connect(ui.ConvertBtn, SIGNAL(clicked()), this, SLOT(Convert()));
}
void HopeDBToGDem::BrowserInput()
{
	QString tmppath=QFileDialog::getExistingDirectory(this,	tr("Open folder where exists Gt file!"), _strInputIEEIDBPath);
	
	if(!tmppath.isEmpty())
	{
		_strInputIEEIDBPath = tmppath;
		ui.textEditInputIEEIDBPath->setText(_strInputIEEIDBPath);
	}
}

void HopeDBToGDem::BrowserSave()
{
	QString tmppath=QFileDialog::getExistingDirectory(this,	tr("Open folder to save converted jpg file!"),_strSaveGTPath);

	if(!tmppath.isEmpty())
	{
		_strSaveGTPath =tmppath; 
		ui.textsaveGTPath->setText(_strSaveGTPath);
	}
}
void HopeDBToGDem::Convert()
{
	QDir inputDir = QDir(_strInputIEEIDBPath);        
	if(!inputDir.exists())
	{
		messageBox("Input directory does not exist! Please try once again");
		return;
	}
		
	ui.ConvertBtn->setEnabled(false);

	//get jpg file
	QString extension="*.jpg";
	QStringList ieeifiles;
	ieeifiles = inputDir.entryList(QStringList(extension),QDir::Files | QDir::NoSymLinks);

	if(ieeifiles.size()==0)
	{
		messageBox("Jpg files does not exist!");
		ui.ConvertBtn->setEnabled(true);
		return;
	}
	
	ui.progressBarGtCount->setRange(0,ieeifiles.size()-1);

	for(int i=0;i<ieeifiles.size();i++)
	{
		qApp->processEvents();
		ui.progressBarGtCount->setValue(i);
		renameAndCopyToTmp(ieeifiles.at(i));
	}

	ieeifiles.clear();

	//add changed filename to list

	QDir changeDir = QDir(_strInputIEEIDBPath+"/tmp/");  
	ieeifiles = changeDir.entryList(QStringList(extension),QDir::Files | QDir::NoSymLinks);
	ieeifiles.sort();
						
	// add to gt

	makeGtFileFromJpgFiles(&ieeifiles);
	
	ui.ConvertBtn->setEnabled(true);

	messageBox("Finished!");
	ui.progressBarGtCount->setValue(0);
}
void HopeDBToGDem::makeGtFileFromJpgFiles(QStringList* lst)
{
	QStringList gtList;
	QString		strTemp,strJpg;
	//------    gt¡ƒÀŒÀÀ∂ß∑ªµ‚(gtlist) –±®   ------
	for (int i=0;i<lst->size();i++)
	{
		strTemp = getGtFileNameFromString(lst->at(i));
		if (!gtList.contains(strTemp,Qt::CaseInsensitive))
			gtList << strTemp;
	}
	//------    gt¡ƒÀŒ¥… ΩÛ±®√˘  ------
	qint32 level,filecount;
	qint32 offset,size;
	offset =0;
	size = 0;
	for(int i=0;i<gtList.size();i++)
	{
		strTemp = _strInputIEEIDBPath + "/tmp/" + gtList.at(i);
		if (QFile::exists(strTemp))
			QFile::remove(strTemp);
		QFile gtFile(strTemp);
		
		if(!gtFile.open(QIODevice::ReadWrite))
			return;
		{
			QDataStream out(&gtFile);
			out.setByteOrder(QDataStream::LittleEndian);
			level = (qint32)getLevelFromString(gtList.at(i));
			out << level;
			if (level>7)
				filecount = 128 * 128;
			else
				filecount = pow(2.0,level)*pow(2.0,level);
			out << filecount;
			for(int j=0;j<filecount;j++)
				out << offset << size;
		}
		gtFile.close();
	}
	//gt¡ƒÀŒÀÊ jpg¥…Àæ  æ°∞°
	qint32 body_pointer = index_header + sizeof(qint32)*2*filecount;	
	qint32 index_pointer = index_header;
	int number;
	for( int i=0;i<lst->size();i++)
	{
		strJpg = _strInputIEEIDBPath + "/tmp/" + lst->at(i);
		QFile jpgFile(strJpg);
		size = jpgFile.size();
		if(!jpgFile.open(QIODevice::ReadOnly))
			return;
		unsigned char* datapointer=NULL;
		datapointer=jpgFile.map(0,size);

		number= getNumberFromString(lst->at(i));		
		index_pointer = sizeof(qint32)*2*(number+1) + index_header;

		strTemp = getGtFileNameFromString(lst->at(i));
		QFile gtFile(_strInputIEEIDBPath + "/tmp/" + strTemp);
		if(!gtFile.exists())
			return;
		if(!gtFile.open(QIODevice::ReadWrite))
			return;
		{
			QDataStream out(&gtFile);
			out.setByteOrder(QDataStream::LittleEndian);
			gtFile.seek(index_pointer);
			out << body_pointer;
			out << size;
			gtFile.seek(body_pointer);
			gtFile.write((const char*)datapointer,size);
			body_pointer = body_pointer + size;
		}
		jpgFile.unmap(datapointer);
		
		gtFile.close();
		jpgFile.close();
		QFile::remove(strJpg);
	}
	//  move of gtFiles
	for(int i=0;i<gtList.size();i++)
	{
		QString gtFileOldName,gtFileNewName;
		gtFileOldName = _strInputIEEIDBPath + "/tmp/" + gtList.at(i);
		gtFileNewName = _strSaveGTPath + "/" + gtList.at(i);
		if (QFile::exists(gtFileOldName))
		{
			QFile::copy(gtFileOldName,gtFileNewName);
			QFile::remove(gtFileOldName);
		}
	}
	QDIR_RMDIR(_strInputIEEIDBPath + "/tmp");
}

//	27_10_12_4567.jpg   =>  27_10_12.gt 
QString HopeDBToGDem::getGtFileNameFromString(QString str)
{
 	QString fStr =str.left(str.length()-4);
	QStringList lt =fStr.split("_");
	int x=lt.at(0).toInt();
	int y=lt.at(1).toInt();
	int level = lt.at(2).toInt();
	return QString("%1_%2_%3.gt").arg(x).arg(y).arg(level);
};

//	27_10_12.gt     =>  12  
int HopeDBToGDem::getLevelFromString(QString str)
{
	QString fStr =str.left(str.length()-3);
	QStringList lt =fStr.split("_");
	return lt.at(2).toInt();	
};

//	27_10_12_4567.jpg     =>  4567  
int HopeDBToGDem::getNumberFromString(QString str)
{
	QString fStr =str.left(str.length()-4);
	QStringList lt =fStr.split("_");
	return lt.at(3).toInt();
};

void HopeDBToGDem::renameAndCopyToTmp(QString fileName)
{
	QString str =fileName.left(fileName.length()-4);
	QStringList listtmp =str.split("_");

	int x=listtmp.at(0).toInt();
	int y=listtmp.at(1).toInt();
	int level = listtmp.at(2).toInt();

	int x1,y1,level1,index;
		
	x1 = x / 128;
	y1 = y / 128;
	level1= level ;

	if (level1>7)
		index = (y % 128)*128+(x % 128)-1;
	else
		index = y*pow(2.0,level)+x-1;
	
	QString newFileName = QString("%1_%2_%3_%4.jpg").arg(x1).arg(y1).arg(level1).arg(index);

	QDIR_MKDIR(_strInputIEEIDBPath +"/tmp/")
	QFILE_REMOVE(_strInputIEEIDBPath +"/tmp/"+newFileName)

	QFILE_COPY(_strInputIEEIDBPath+"/"+fileName,_strInputIEEIDBPath +"/tmp/"+newFileName);
}



