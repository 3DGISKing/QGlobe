#ifndef VECTORCONVERTER_H
#define VECTORCONVERTER_H

#include <QtGui/QMainWindow>
#include "ui_vectorconverter.h"

#define		AV_NULL_SHP				0
#define		AV_POINT_SHP			1
#define		AV_POLYLINE_SHP			3
#define		AV_POLYGON_SHP			5

class VectorConverter : public QMainWindow
{
	Q_OBJECT

public:
	VectorConverter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~VectorConverter();

private:
	Ui::VectorConverterClass ui;

private:
	void DeleteFolderContent(QString dirStr);
	void EnableAllButtons(bool flag);
	void Updatedata(QString saveDir);

private slots:
	void SetShapeFileName();
	void UpdateShapeDB();
	void CloseShpUpdateDlg();
	void ChangeIDWriteStatus();
	void SetSaveFolderName();
private:
	QString m_shpFileStr;
	QString m_saveFolderStr;
	bool m_bWriteID;
	int m_iStartLevel;
	int m_iEndLevel;
	int m_iTolerancePixels;
	int m_maxFSize;
	int m_maxFeatureNum;
};

#endif // VECTORCONVERTER_H
