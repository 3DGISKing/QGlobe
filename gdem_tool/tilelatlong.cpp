#include "tilelatlong.h"
#include "common.h"
#include <QString>

TileLatLong::TileLatLong(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.lineEdit_ULLONG->setMaxLength(19);
	ui.lineEdit_ULLAT->setMaxLength(19);
	ui.lineEdit_RBLONG->setMaxLength(19);
	ui.lineEdit_RBLAT->setMaxLength(19);
	m_tileX = 27 ;
	m_tileY = 19 ;
	m_tileLevel = 5 ;
	ui.lineEdit_X->setText(QString::number(m_tileX,10));
	ui.lineEdit_Y->setText(QString::number(m_tileY,10));
	ui.lineEdit_Level->setText(QString::number(m_tileLevel,10));
	m_LLLat = 45 ;
	m_LLLong = 123 ;
	m_LLtileLevel = 5 ;
	ui.lineEdit_Lat->setText(QString::number(45,10));
	ui.lineEdit_Long->setText(QString::number(123,10));
	ui.lineEdit_LLlevel->setText(QString::number(5,10));
	m_ulLat = 0.0 ;
	m_ulLong = 0.0 ;
	m_rbLat = 0.0 ;
	m_rbLong = 0.0 ;
	
}

TileLatLong::~TileLatLong()
{

}

void TileLatLong::Change_X()
{
	bool ok;
	QString xIndex = ui.lineEdit_X->text();
	m_tileX = xIndex.toInt(&ok,10); 

	/*** Getting longitude  corresponding x,level ***/
	m_ulLong = CalcFromXYtoLongLat(m_tileX,m_tileLevel);
	QString longstr;
	longstr.setNum(m_ulLong,'f',16);
	ui.lineEdit_ULLONG->setText(longstr);
	longstr.setNum(CalcFromXYtoLongLat(m_tileX+1,m_tileLevel),'f',16);
	ui.lineEdit_RBLONG->setText(longstr);

	Change_derived();
}

void TileLatLong::Change_Y()
{
	bool ok;
	QString yIndex = ui.lineEdit_Y->text();
	m_tileY = yIndex.toInt(&ok,10); 
	/*** Getting longitude and latitude corresponding x,y,level ***/
	m_ulLat = CalcFromXYtoLongLat(m_tileY+1,m_tileLevel);
	QString latstr;
	latstr.setNum(m_ulLat,'f',16);
	ui.lineEdit_ULLAT->setText(latstr);
	latstr.setNum(CalcFromXYtoLongLat(m_tileY,m_tileLevel),'f',16);
	ui.lineEdit_RBLAT->setText(latstr);

	Change_derived();

}

void TileLatLong::Change_Level()
{
	bool ok;
	QString levelIndex = ui.lineEdit_Level->text();
	m_tileLevel = levelIndex.toInt(&ok,10); 
	Change_X();
	Change_Y();
}
void TileLatLong::Change_LLlevel()
{
	bool ok;
	QString levelIndex = ui.lineEdit_LLlevel->text();
	m_LLtileLevel = levelIndex.toInt(&ok,10); 
	Change_Lat();
	Change_Long();
}

void TileLatLong::Change_Lat()
{
	bool ok;
	QString xIndex = ui.lineEdit_Lat->text();
	m_LLLat = xIndex.toDouble(&ok);

	/*** Getting x  corresponding Long,level ***/
	m_LLtileY = CalcFromLatLongtoXY(m_LLLat,m_LLtileLevel);
	m_M=m_LLtileY/128;
	ui.txtTileNum->setText(QString::number(m_LLtileX,10) + "_" +QString::number(m_LLtileY,10) +"_" +QString::number(m_LLtileLevel,10)+"_"+QString("%1_%2").arg(m_N).arg(m_M)  );	
}

void TileLatLong::Change_Long()
{
	bool ok;
	QString xIndex = ui.lineEdit_Long->text();
	m_LLLong = xIndex.toDouble(&ok);

	/*** Getting x  corresponding Long,level ***/
	m_LLtileX = CalcFromLatLongtoXY(m_LLLong,m_LLtileLevel);

	m_N=m_LLtileX/128;
	ui.txtTileNum->setText(QString::number(m_LLtileX,10) + "_" +QString::number(m_LLtileY,10) +"_" +QString::number(m_LLtileLevel,10)+"_"+QString("%1_%2").arg(m_N).arg(m_M)  );	

}

void TileLatLong::Change_derived()
{
	QString latstr;

	m_derived = CalcFromXYtoLongLat(m_tileX+1,m_tileLevel) - CalcFromXYtoLongLat(m_tileX,m_tileLevel);
	latstr.setNum(m_derived,'f',16);
	ui.lbl_derived->setText("Tile Width : "+latstr);

}