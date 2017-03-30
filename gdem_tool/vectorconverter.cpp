#include "vectorconverter.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include "shapedef.h"


VectorConverter::VectorConverter(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	connect(ui.btnBrowseSrc, SIGNAL(clicked()), this, SLOT(SetShapeFileName()));
	connect(ui.btnOK, SIGNAL(clicked()), this, SLOT(UpdateShapeDB()));
	connect(ui.btnNo, SIGNAL(clicked()), this, SLOT(CloseShpUpdateDlg()));
	connect(ui.btnBrowseSave, SIGNAL(clicked()), this, SLOT(SetSaveFolderName()));
	connect(ui.chkIDWrite, SIGNAL(clicked()), this, SLOT(ChangeIDWriteStatus()));

	ui.progressBar->setValue(0);
	m_bWriteID = false;
	ui.lblStatus->setText("Shape file convert program.");
	m_bWriteID = true;
}

VectorConverter::~VectorConverter()
{

}

void VectorConverter::UpdateShapeDB()
{
	m_shpFileStr = ui.txtDirSrc->text();
	m_saveFolderStr = ui.txtDirSave->text();
	QMessageBox msgBox;
	if (!QFile::exists(m_shpFileStr))
	{
		msgBox.setText("Cannot find selected file in disk!");
		msgBox.exec();
		return;
	}
	if (m_saveFolderStr.isEmpty())
	{
		msgBox.setText("Destination folder empty! Please designate save folder!");
		msgBox.exec();
		return;
	}
	bool startLevel, endLevel;
	QString levelStr = ui.txtStartLevel->text();
	m_iStartLevel = levelStr.toInt(&startLevel);
	levelStr = ui.txtEndLevel->text();
	m_iEndLevel = levelStr.toInt(&endLevel);
	if (!startLevel || !endLevel)
	{
		msgBox.setText("Wrong number in start or end level box!");
		msgBox.exec();
		return;
	}
	levelStr = ui.txtToleranceNum->text();
	m_iTolerancePixels = levelStr.toInt(&startLevel);
	if (!startLevel)
	{
		msgBox.setText("Invalid character in tolerance box. Pls write number!");
		msgBox.exec();
		return;
	}
	m_maxFSize = ui.txtMaxFileSize->text().toInt(&startLevel);
	if (m_maxFSize < 200 || !startLevel)
	{
		msgBox.setText("Maximum file size is too small!");
		msgBox.exec();
		ui.txtMaxFileSize->setFocus();
		return;
	}
	m_maxFeatureNum = ui.txtMaxFeatureNum->text().toInt(&startLevel);
	if (m_maxFeatureNum < 800 || !startLevel)
	{
		msgBox.setText("Too small number of features!");
		msgBox.exec();
		ui.txtMaxFeatureNum->setFocus();
		return;
	}
	if (m_iTolerancePixels > 5)
	{
		QMessageBox msgBox;
		msgBox.setText("Too large tolerance! Maybe result in coase shape data!");
    	 msgBox.setInformativeText("Do you want to continue anyway?");
		 msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		 msgBox.setDefaultButton(QMessageBox::Ok);
		 int ret = msgBox.exec();
		 if (ret != QMessageBox::Ok)
		 {
			 ui.txtToleranceNum->setText("");	
			 ui.txtToleranceNum->setFocus();
			 return;
		 }
	}
	EnableAllButtons(false);
	
	ui.lblStatus->setText("Converting is in progress. Please wait!");

	Updatedata(m_saveFolderStr);
	EnableAllButtons(true);
	ui.lblStatus->setText("Conversion completed!!!");
}

void VectorConverter::Updatedata(QString saveDir)
{
	//  1+2+3+ ... + 12 = 78 + 24 = 82
	int maxVal = 0;
	for (int level = m_iStartLevel; level<m_iEndLevel+1; level++)
	{
		maxVal = maxVal + level;
	}
	maxVal = maxVal + 30;
	ui.progressBar->setMaximum(maxVal);
	ui.progressBar->setValue(0);
	int pgrBarVal=0;

	//DeleteFolderContent(saveDir);

	QString tmpDirStr = saveDir + "/tmp";
	for (int level = m_iStartLevel; level < m_iEndLevel+1; level++)
	{
		pgrBarVal += level;
		ConvertShapeFile(m_shpFileStr, tmpDirStr, level, m_bWriteID, m_iTolerancePixels, m_maxFSize, m_maxFeatureNum);
		ui.progressBar->setValue(pgrBarVal);
	}
	ConvertShapeToGS(tmpDirStr, saveDir);
	pgrBarVal += 29;
	ui.progressBar->setValue(pgrBarVal);

	//write code here to remove temporary files and folders
	DeleteFolderContent(tmpDirStr);
	QDir dstDir(saveDir);
	dstDir.refresh();	
	pgrBarVal++;
	ui.progressBar->setValue(pgrBarVal);
	
	EnableAllButtons(true);
	//ui.progressBar->setValue(0);
}
void VectorConverter::DeleteFolderContent(QString dirStr)
{
	QFileInfo curFile(dirStr);
	QDir curDir(dirStr);
	if (!curFile.exists())
		return;
	if (!curFile.isFile())
	{
		QFileInfoList subList = curDir.entryInfoList();
		for (int i=0; i<subList.size(); i++)
		{
			QFileInfo subFolder = subList.at(i);
			QString subFName = subFolder.absoluteFilePath();
			QString dirName = subFName.mid(subFName.lastIndexOf("/")+1);
			if (dirName == "." || dirName == "..")
				continue;
			DeleteFolderContent(subFName);
		}
		curDir.rmdir(dirStr);
	}
	else
	{
		curDir.remove(dirStr);
	}
}
void VectorConverter::EnableAllButtons(bool flag)
{
	ui.btnBrowseSrc->setEnabled(flag);
	ui.btnBrowseSave->setEnabled(flag);
	ui.btnOK->setEnabled(flag);
	ui.txtDirSrc->setEnabled(flag);
	ui.txtDirSave->setEnabled(flag);
}

void VectorConverter::SetShapeFileName()
{
	m_shpFileStr = QFileDialog::getOpenFileName(this, tr("Open Shape File"),	"/home",		tr("Shape files (*.shp)"));
	if (!m_shpFileStr.isEmpty())
	{
		ui.txtDirSrc->setText(m_shpFileStr);
		if (!m_saveFolderStr.isEmpty()) ui.btnOK->setEnabled(true);
		int type = GetShapeFileType(m_shpFileStr);
		if (type == AV_NULL_SHP)
		{
			QMessageBox msg; msg.setText("Invalid source file!"); msg.exec();
			ui.btnOK->setEnabled(false); m_shpFileStr = ""; return;
		}
		else if (type == AV_POLYGON_SHP)
		{
			ui.lblStatus->setText("Polygon file selected to process!");
			ui.lblSource->setText(ui.lblSource->text() + "  : Polygon shape file chosen");
		}
		else if (type == AV_POINT_SHP)
			ui.lblSource->setText("Point theme selected. Nothing will be done!");
	}	
}
void VectorConverter::SetSaveFolderName()
{
	m_saveFolderStr = QFileDialog::getExistingDirectory(this, tr("Select Folder to save"), "/home");
	if (!m_saveFolderStr.isEmpty())
	{
		ui.txtDirSave->setText(m_saveFolderStr);
		ui.btnOK->setEnabled(true);
	}
}
void VectorConverter::ChangeIDWriteStatus()
{
	m_bWriteID = !m_bWriteID;
}

void VectorConverter::CloseShpUpdateDlg()
{
	QMainWindow::close();
}