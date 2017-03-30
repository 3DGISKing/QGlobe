#include <QString>

#include "importmanager.h"
#include "shapefile.h"
#include <QFile>
#include "dbasefile.h" 
#include <QDir>

#define LONG_SIZE (sizeof(long))

ShapeFileImporter::ShapeFileImporter()
{
	needToReadBuf = true;
	_pShpFileData=NULL;
	_pShxFileData=NULL;
	_ShapeCount=0;
	_ShapeType=-1;
}
ShapeFileImporter::ShapeFileImporter(QString shpFileName)
{
	needToReadBuf = true;
	_pShpFileData=NULL;
	_pShxFileData=NULL;
	_ShapeCount=0;
	_ShapeType=-1;
	m_shpFileName = shpFileName;
}
ShapeFileImporter::~ShapeFileImporter()
{
	delete [] _pShpFileData;
	delete [] _pShxFileData;
}

bool ShapeFileImporter::ReadBuffer(QString shapeFileName)
{
	needToReadBuf = false;
	int index=shapeFileName.lastIndexOf("/");
	int len=shapeFileName.length();

	QString path=shapeFileName.left(index);

	QString layername=shapeFileName.mid(index+1,len-index-5);

	/////////////////
	QString dbase_file_name=path+"/"+layername+DBASE_FILE_SUFFIX;
	_pDBaseFile=new DBaseFile(dbase_file_name);
	
	QString shp_file_name=path+"/"+layername+SHAPE_FILE_SUFFIX;
	QString shx_file_name=path+"/"+layername+INDEX_FILE_SUFFIX;

	QFile ShpFile(shp_file_name);
	QFile ShxFile(shx_file_name);

	if(!ShpFile.open(QIODevice::ReadOnly)) return false;
	if(!ShxFile.open(QIODevice::ReadOnly)) return false;

	unsigned char* lpData = NULL;

	lpData=ShpFile.map(0,ShpFile.size());

	if (lpData != NULL)
	{
		_pShpFileData=new unsigned char[ShpFile.size()];
		memcpy(_pShpFileData, lpData, ShpFile.size());
		ShpFile.unmap(lpData);
	}

	lpData = NULL;

	lpData=ShxFile.map(0,ShxFile.size());

	if (lpData != NULL)
	{
		_pShxFileData=new unsigned char[ShxFile.size()];
		memcpy(_pShxFileData, lpData, ShxFile.size());
		ShxFile.unmap(lpData);
	}

	_ShapeCount=(ShxFile.size()-SHP_FILE_HEADER_SIZE)/SHX_FILE_RECORD_SIZE;

	ShpFileHeader *shp_file_header;

	shp_file_header=(ShpFileHeader*)_pShpFileData;

	shp_file_header->_FileCode=BigToLittle(shp_file_header->_FileCode);
	shp_file_header->_FileLength=BigToLittle(shp_file_header->_FileLength);

	if(shp_file_header->_FileCode!=SHP_FILE_CODE) return false;//Invalid Shape File 

	_ShapeType=shp_file_header->_ShapeType;


	ShpFile.close();
	ShxFile.close();
	return true;
}

ShapeTypeAndCount ShapeFileImporter::ReadShapeFileHeader()
{
	ShapeTypeAndCount res;
	res.ShapeCount = 0;
	res.ShapeType = 0;

	if(!ReadBuffer(m_shpFileName)) return res;

	res.ShapeCount = _ShapeCount;
	res.ShapeType = _ShapeType;	
	return res;
}
void ShapeFileImporter::SetShapeFileName(QString shpFileName)
{
	m_shpFileName = shpFileName;
}
bool ShapeFileImporter::ReadPointShape(ShapeTypeAndCount typeAndCount, QVector<QPointF> * pts, DBaseFile * &dbase)
{
	if (needToReadBuf) 
		if (!ReadBuffer(m_shpFileName)) return false;
	dbase = _pDBaseFile;

	unsigned int offset1 =	SHP_FILE_HEADER_SIZE;
	for (int recNum=0; recNum<typeAndCount.ShapeCount; recNum++)
	{
		int offset=SHP_FILE_HEADER_SIZE+recNum*SHX_FILE_RECORD_SIZE;

		_pShxFileRecord=(ShxFileRecord*)(_pShxFileData+offset);
		_pShxFileRecord->_Offset=BigToLittle(_pShxFileRecord->_Offset);
		offset=_pShxFileRecord->_Offset*2+SHP_FILE_RECORD_HEADER_SIZE;
		ShpFileRecordHeader *p=(ShpFileRecordHeader*)(_pShpFileData+offset1);
		p->_ContentLength=BigToLittle(p->_ContentLength);
		p->_RecordNumber=BigToLittle(p->_RecordNumber);
		offset1+=SHP_FILE_RECORD_HEADER_SIZE;
		if(p->_ShapeType==NULL_SHAPE) return false;
		ArcPoint*arcpt = (ArcPoint*)(_pShpFileData+offset1);
		offset1 += ARC_POINT_SIZE;

		QPointF pt(arcpt->_X, -arcpt->_Y);
		pts->push_back(pt);

	}
	return true;
}
bool ShapeFileImporter::ReadPolygonShape(ShapeTypeAndCount typeAndCount, QVector<QPolygonF>* polys, DBaseFile * &dbase)
{
	if (needToReadBuf)
	{
		if (!ReadBuffer(m_shpFileName)) return false;
	}
	dbase = _pDBaseFile;

	int offset = SHP_FILE_HEADER_SIZE;
	for (unsigned int i =0; i<typeAndCount.ShapeCount; i++)
		ReadPolygon(i, polys, offset);
	return true;
}
bool ShapeFileImporter::ReadPolygon(unsigned int recored_index, QVector<QPolygonF>* polys,int &offset1)
{
	int offset=SHP_FILE_HEADER_SIZE+recored_index*SHX_FILE_RECORD_SIZE;

	_pShxFileRecord=(ShxFileRecord*)(_pShxFileData+offset);
	_pShxFileRecord->_Offset=BigToLittle(_pShxFileRecord->_Offset);

	offset=_pShxFileRecord->_Offset*2+SHP_FILE_RECORD_HEADER_SIZE;

	//ugi debug
	ShpFileRecordHeader *p=(ShpFileRecordHeader*)(_pShpFileData+offset1);
	p->_ContentLength=BigToLittle(p->_ContentLength);
	p->_RecordNumber=BigToLittle(p->_RecordNumber);

	offset1+=SHP_FILE_RECORD_HEADER_SIZE;
	
	//if(p->_ContentLength==0) return false;
	if(p->_ShapeType==NULL_SHAPE) return false;

	_pArcPolylineHeader=(ArcPolyLineHeader*)(_pShpFileData+offset1);

	offset1+=ARC_POLYLINE_HEADER_SIZE;
	long numparts=_pArcPolylineHeader->_NumParts;
	long numpoints=_pArcPolylineHeader->_NumPoints;	

	offset+=ARC_POLYLINE_HEADER_SIZE;

	long* pIndexOfFirstPointInPart=(long*)(_pShpFileData+offset1);
	offset+=numparts*LONG_SIZE;
	offset1+=numparts*LONG_SIZE;

	ArcPoint* pPoints=(ArcPoint*)(_pShpFileData+offset);

	int start_part_point_index,end_part_point_index;

	//for(int indexpart=0; indexpart<numparts; indexpart++)
	for(int indexpart=0; indexpart<1; indexpart++)
	{
		start_part_point_index=pIndexOfFirstPointInPart[indexpart];

		if(indexpart==numparts-1)
			end_part_point_index=numpoints;
		else
			end_part_point_index=pIndexOfFirstPointInPart[indexpart+1];

		QVector<QPointF> pts;
		for(int indexpoint=start_part_point_index;indexpoint<end_part_point_index;  indexpoint++)
		{
			QPointF pt(pPoints[indexpoint]._X, -pPoints[indexpoint]._Y);
			pts.push_back(pt);
		}
		QPolygonF poly(pts);
		polys->push_back(poly);
	}

	offset1+=numpoints*sizeof(double)*2;
	return true;
}


