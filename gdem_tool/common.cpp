
#include "common.h"
#include <QMessageBox>
#include <fstream>

void messageBox(QString msg)
{
	QMessageBox msgBox;
	msgBox.setText(msg);
	msgBox.exec();
}

void writeJgw(int x,int y,int level,QString filepath)
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



