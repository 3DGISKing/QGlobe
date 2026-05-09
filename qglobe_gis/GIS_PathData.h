#ifndef __GIS_PATHDATA_H__
#define __GIS_PATHDATA_H__

#include "qglobe_gis_global.h"
#include <QtGui/QPainter>
#include <QtGui/QPolygonF>
#include <QtGui/QPainterPath>

class QGLOBE_GIS_EXPORT GIS_PathData
{
public:
	GIS_PathData();
	virtual ~GIS_PathData();

	QPainterPath* GetPath() { return &_Path; }
public:
	bool Append(double u, double v); //�浵/���� �����߰�.
	bool Insert(int nIndex, double u, double v); //�浵/���� ���� ����.
	bool Change(int nIndex, double u, double v); //�浵/���� ���� ����.
	bool Remove(int nIndex); //�������
	bool Clear();

	int GetCount() const { return _Polygon.size(); }

	bool Contains(QRectF & rect) const;
	bool Intersect(QRectF & rect) const;

	// operator 
	GIS_PathData &		operator=(const GIS_PathData & other);

protected:
	QPainterPath	_Path;
	QPolygonF		_Polygon;

	bool GeneratePath();

private:
	bool	_Changed;
};

#endif //__GIS_PATHDATA_H__
