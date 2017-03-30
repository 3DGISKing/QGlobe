#include "writeremovejgw.h"
#include <QMessageBox>
#include <fstream>
#include <QProgressDialog>
#include <QFileDialog>
#include "common.h"

WriteRemoveJGW::WriteRemoveJGW(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.Open_Folder_pushButton,SIGNAL(clicked()),this,SLOT(OpenFolder()));
	connect(ui.Run_pushButton,SIGNAL(clicked()),this,SLOT(Run()));
	connect(ui.actionJGWRemove,SIGNAL(clicked()),this,SLOT(SetJGWRemoveFlag()));
	m_removeflag = false;
}

WriteRemoveJGW::~WriteRemoveJGW()
{

}

void WriteRemoveJGW::OpenFolder()
{
	_OpenPath = QFileDialog::getExistingDirectory(this,	tr("Open folder where exists jpg files!"), tr("E:/database"));

	if (!_OpenPath.isEmpty()) 
		ui.Open_Folder->setText(_OpenPath);
}


void WriteRemoveJGW::SetJGWRemoveFlag(void)
{
	if (m_removeflag)
		m_removeflag =false ;
	else
		m_removeflag = true;
}

void WriteRemoveJGW::writeJGWfiles()
{
	QDir curDir = QDir(_OpenPath);
	QStringList fileNameList;
	QString ext = "*.jpg";
	fileNameList = curDir.entryList(QStringList(ext),QDir::Files | QDir::NoSymLinks);

	QProgressDialog progressDialog(this);
	progressDialog.setCancelButtonText("Cancel");
	progressDialog.setRange(0,fileNameList.size());
	progressDialog.setWindowTitle(tr("Write jgw files..."));
	progressDialog.show();

	for (int i=0;i<fileNameList.size();i++)
	{
		progressDialog.setValue(i);
		qApp->processEvents();

		QString fName = fileNameList.at(i).left(fileNameList.at(i).length() - 4);
		int xIndex = fName.indexOf("_");
		int yIndex = fName.indexOf("_", xIndex + 1);
		int levelIndex = fName.indexOf("_",yIndex +1);
		int x = fName.left(xIndex).toInt(); 
		int y = fName.mid(xIndex+1,yIndex - xIndex - 1).toInt(); 
		int level = fName.mid(yIndex + 1).toInt();

		QString filepath = _OpenPath + "/" + fName + ".jgw";

		writeJgw(x,y,level,filepath);
	}
}
void WriteRemoveJGW::removeJGWfiles()
{
	QDir curDir = QDir(_OpenPath);
	QStringList fileNameList;
	QString ext = "*.jgw";
	fileNameList = curDir.entryList(QStringList(ext),QDir::Files | QDir::NoSymLinks);

	QProgressDialog progressDialog(this);
	progressDialog.setCancelButtonText("Cancel");
	progressDialog.setRange(0,fileNameList.size()-1);
	progressDialog.setWindowTitle(tr("Removing jgw files..."));
	progressDialog.show();

	for (int i=0;i<fileNameList.size();i++)
	{
		progressDialog.setValue(i);
		qApp->processEvents();
		
		QString filepath = _OpenPath + "/" + fileNameList.at(i);
		QFile tmp;
		tmp.remove(filepath);
	}
}

void WriteRemoveJGW::Run()
{
	if (m_removeflag)
		removeJGWfiles();
	else
		writeJGWfiles();
}