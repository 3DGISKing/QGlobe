#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <math.h>

#define GT_FILE_EXTENSION     "gt"
#define DEM_FILE_EXTENSION    "gd"
#define SHAPE_FILE_EXTENSION  "gs"
#define JPG_FILE_EXTENSION    "jpg"

#define IMAGE_TILE_SIZE        256

#define QDIR_MKDIR(dirName)         \
	QDir tmp;						\
	if(!tmp.exists(dirName))       \
		tmp.mkdir(dirName);         

#define QDIR_RMDIR(dirName)         \
	QDir tmp;						\
	if(tmp.exists(dirName))         \
		tmp.rmdir(dirName);         
	

#define QFILE_REMOVE(fileName)      \
	if(QFile::exists(fileName))     \
	    QFile::remove(fileName);   

#define QFILE_COPY(origFileName,newFileName) \
	QFile file(origFileName);                \
	file.copy(newFileName);


typedef  struct rect_XY 
{
	int x1;
	int x2;
	int y1;
	int y2;
}_rect_XY;

#define JPG_IMAGE_SIZE 256
void messageBox(QString msg);
void writeJgw(int x,int y,int level,QString filepath);
inline double CalcFromLatLongtoXY(double latlong,int level)
{
	return ((latlong + 180.0f)/360.0f*pow(2.0,level));
}
inline double CalcFromXYtoLongLat(int x,int level)
{
	return ((double)x/pow(2.0,(double)level)*360.0f - 180.0f);
}


#endif
