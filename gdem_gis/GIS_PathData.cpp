#include "GIS_PathData.h"
#include "GDM_Math.h"

GIS_PathData::GIS_PathData()
{
	_Changed = false;
}

GIS_PathData::~GIS_PathData()
{
}

bool GIS_PathData::Append(double u, double v)
{
	_Polygon.append(QPointF(u+gdm_PI, gdm_PI-v));
	_Changed = true;
	GeneratePath();

	return true;
}

bool GIS_PathData::Insert(int nIndex, double u, double v)
{
	_Polygon.insert(nIndex, QPointF(u+gdm_PI, gdm_PI-v));
	_Changed = true;
	GeneratePath();
	return true;
}

bool GIS_PathData::Change(int nIndex, double u, double v)
{
	double x = u+gdm_PI;
	double y = gdm_PI-v;
	
	if(nIndex >= _Polygon.count())
	{
		Append(u, v);
		return true;
	}

	if(nIndex < 0 || nIndex >= _Polygon.count())
		return false;

	if(_Polygon[nIndex].x() == x && _Polygon[nIndex].x() == x)
		return true;

	_Polygon[nIndex].setX(x);
	_Polygon[nIndex].setY(y);
	_Changed = true;
	GeneratePath();
	return true;
}

bool GIS_PathData::Remove(int nIndex)
{
	_Polygon.remove(nIndex);
	_Changed = true;
	GeneratePath();
	return true;
}

bool GIS_PathData::Clear()
{
	_Polygon.clear();
	_Changed = true;
	GeneratePath();
	return true;
}

bool GIS_PathData::GeneratePath()
{
	if(!_Changed)
		return false;

	QPainterPath path;
	if(_Polygon.size() > 0)
	{
		path.moveTo(_Polygon[0]);

		for(int i=1 ; i<_Polygon.size() ; i++)
			path.lineTo(_Polygon[i]);

		path.closeSubpath();
	}

	_Path = path;
	_Changed = false;

	return true;
}

bool GIS_PathData::Contains(QRectF & rect) const
{
	return _Path.contains(rect);
}

bool GIS_PathData::Intersect(QRectF & rect) const
{
	return _Path.intersects(rect);
}

GIS_PathData&	GIS_PathData::operator=(const GIS_PathData & other)
{
	_Polygon = other._Polygon;
	_Path = other._Path;

	return *this;
}
