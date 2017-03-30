#ifndef IMPORTMANAGER_H
#define IMPORTMANAGER_H

#include <QVector>
#include <QPointF>
#include <QPolygonF>
#include "shapefile.h"

class DBaseFile;

struct ShapeTypeAndCount
{
	unsigned char ShapeType;
	unsigned int ShapeCount;
};

class ShapeFileImporter
{
private:
	unsigned char* _pShpFileData;
	unsigned char* _pShxFileData;
	unsigned int   _ShapeCount;
	unsigned char  _ShapeType;
	DBaseFile *    _pDBaseFile;

	ShxFileRecord* _pShxFileRecord;
	ArcPolyLineHeader *_pArcPolylineHeader;
	ArcPoint*          _pPoint;
	bool needToReadBuf;
	QString m_shpFileName;

	bool ReadBuffer(QString shapeFileName);
	bool ReadPolygon(unsigned int record_index, QVector<QPolygonF> *pGISpolygon,int& offset);

public:
	ShapeFileImporter();
	ShapeFileImporter(QString shapeFileName);
	~ShapeFileImporter();

	ShapeTypeAndCount ReadShapeFileHeader();
	bool ReadPointShape(ShapeTypeAndCount typeAndCount, QVector<QPointF> * pts, DBaseFile* &dbase);
	bool ReadPolygonShape(ShapeTypeAndCount typeAndCount, QVector<QPolygonF>* polys, DBaseFile* &dbase);
	void SetShapeFileName(QString shpFileName);
};

#endif //IMPORTMANAGER_H