#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QPainterPath>
#include <QWidget>
#include "renderarea.h"
#include <QPolygonF>
#include "dbasefile.h"

class PointTheme
{
public:
	PointTheme(){m_dBaseData = NULL; m_data.clear(); m_nCount=0;}
	void addPoint(QPointF pt){m_data.push_back(pt); m_nCount++;}
	int GetCount(){return m_nCount;}
	QVector<QPointF>* GetData(){return &m_data;}
	void addPoints(QVector<QPointF> ptArr)
	{
		for (int i=0; i<ptArr.size(); i++)
		{
			QPointF pt = ptArr.at(i);
			m_data.push_back(pt);
			m_nCount++;
		}
	}
	void addPoints(QPointF *ptArr, int size)
	{
		for (int i=0; i<size; i++)
		{
			QPointF pt = ptArr[i];
			m_data.push_back(pt);
			m_nCount++;
		}
	}
private:
	QVector<QPointF> m_data;
	int m_nCount;
	QColor m_color;

public:
	DBaseFile *m_dBaseData;
	QString m_themeName;


};
class PolygonTheme
{
public:
	PolygonTheme(){m_dBaseData = NULL; m_data.clear(); m_nCount=0;}
	void addPolygon(QPolygonF poly){m_data.push_back(poly); m_nCount++;}
	int GetCount(){return m_nCount;}
	QVector<QPolygonF>* GetData(){return &m_data;}
	void addPolygons(QVector<QPolygonF> polyArr)
	{
		for (int i=0; i<polyArr.size(); i++)
		{
			QPolygonF pt = polyArr.at(i);
			m_data.push_back(pt);
			m_nCount++;
		}
	}
	void addPolygons(QPolygonF *polyArr, int size)
	{
		for (int i=0; i<size; i++)
		{
			QPolygonF pt = polyArr[i];
			m_data.push_back(pt);
			m_nCount++;
		}
	}
private:
	QVector<QPolygonF> m_data;
	int m_nCount;
	QColor m_color;

public:
	DBaseFile *m_dBaseData;
	QString m_themeName;

};
class RenderArea : public QWidget
{
	Q_OBJECT

public:
	RenderArea(const QPainterPath &path, QWidget *parent = 0);
	RenderArea();
	~RenderArea();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

public slots:
	void clearAll();
	void addPointTheme(QVector<QPointF> *pts, DBaseFile* dbase, QString fileName);
	void addPolygonTheme(QVector<QPolygonF> *polys, DBaseFile* dbase, QString fileName);
	void addPolylineTheme(QVector<QPolygonF> *polys, DBaseFile* dbase, QString fileName);
	void UpdateDisplay();

public:
	QVector<PointTheme> * GetPointData(){return &m_points;}
	QVector<PolygonTheme> * GetPolygonData(){return &m_polygons;}
	QVector<PolygonTheme> * GetPolylineData(){return &m_polylines;}

protected:
	void paintEvent(QPaintEvent *event);

private:
	QPainterPath m_path;
	QColor m_fillColor;
	int m_penWidth;
	QColor m_penColor;
	QPicture *m_pixmap;

	bool m_updated;
	double m_left, m_right, m_top, m_bottom;

	QVector<PointTheme> m_points;
	QVector<PolygonTheme> m_polygons;
	QVector<PolygonTheme> m_polylines;
};

#endif