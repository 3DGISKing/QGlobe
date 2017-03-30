#include <QtGui>

#include "renderarea.h"
#include <QVector>

RenderArea::RenderArea(const QPainterPath &path, QWidget *parent)
: QWidget(parent), m_path(path)
{
	//m_penWidth = 1;
	setBackgroundRole(QPalette::Base);
	m_updated = true;
	m_left = 10000000000.0;
	m_right = -10000000000.0;
	m_top = -10000000000.0;
	m_bottom  = 10000000000.0;;
	//pixmap = new QPicture();
}
RenderArea::RenderArea()
{
	m_left = 10000000000.0;
	m_right = -10000000000.0;
	m_top = -10000000000.0;
	m_bottom  = 10000000000.0;
	m_updated = true;
}
void RenderArea::addPointTheme(QVector<QPointF> *pts, DBaseFile * dbaseData, QString fileName)
{
	PointTheme newPtTheme;
	for (int i=0; i<pts->size(); i++)
	{
		QPointF pt = pts->at(i);
		newPtTheme.addPoint(pt);
		m_left = qMin(m_left, pt.x());
		m_right = qMax(m_right, pt.x());
		m_bottom = qMin(m_bottom, pt.y());
		m_top = qMax(m_top, pt.y());
	} 
	newPtTheme.m_dBaseData = dbaseData;
	fileName = fileName.mid(fileName.lastIndexOf("/")+1);
	fileName = fileName.left(fileName.length()-4);
	newPtTheme.m_themeName = fileName;
	m_points.push_back(newPtTheme);
	m_updated = true;
	update();
}
void RenderArea::UpdateDisplay()
{
	m_updated = true;
	update();
}
void RenderArea::addPolygonTheme(QVector<QPolygonF> *polys, DBaseFile *dbaseData, QString fileName)
{
	PolygonTheme newPolyTheme;
	for (int i=0; i<polys->size(); i++)
	{
		QPolygonF poly = polys->at(i);
		newPolyTheme.addPolygon(poly);
		QRectF bndRect = poly.boundingRect();
		m_left = qMin(m_left, bndRect.left());
		m_right = qMax(m_right, bndRect.right());
		m_bottom = qMin(m_bottom, bndRect.top());
		m_top = qMax(m_top, bndRect.bottom());
	}
	newPolyTheme.m_dBaseData = dbaseData;
	fileName = fileName.mid(fileName.lastIndexOf("/")+1);
	fileName = fileName.left(fileName.length()-4);
	newPolyTheme.m_themeName = fileName;
	m_polygons.push_back(newPolyTheme);
	m_updated = true;
	update();
}

void RenderArea::addPolylineTheme(QVector<QPolygonF> *polys, DBaseFile *dbaseData, QString fileName)
{
	PolygonTheme newPolyTheme;
	for (int i=0; i<polys->size(); i++)
	{
		QPolygonF poly = polys->at(i);
		newPolyTheme.addPolygon(poly);
		QRectF bndRect = poly.boundingRect();
		m_left = qMin(m_left, bndRect.left());
		m_right = qMax(m_right, bndRect.right());
		m_bottom = qMin(m_bottom, bndRect.top());
		m_top = qMax(m_top, bndRect.bottom());
	}
	newPolyTheme.m_dBaseData = dbaseData;
	fileName = fileName.mid(fileName.lastIndexOf("/")+1);
	fileName = fileName.left(fileName.length()-4);
	newPolyTheme.m_themeName = fileName;
	m_polylines.push_back(newPolyTheme);
	m_updated = true;
	update();
}

void RenderArea::paintEvent(QPaintEvent *)
{
	if (m_updated)
	{
		QPainter painter(this);
		double renderWidth = m_right - m_left;
		double renderHeight = m_top - m_bottom;

		renderWidth = qMax(renderWidth, renderHeight);
		if (renderWidth == 0) renderWidth = 100.0;

		painter.scale(width()/renderWidth, height()/renderWidth);
		painter.translate(-m_left, -m_bottom);

		for (int i = 0; i < m_points.size(); i++)
		{
			PointTheme ptTheme = m_points.at(i);
			QColor color(200-i*10, i*3, 255-i*15, 200);
			painter.setPen(QPen(color, renderWidth/250.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

			QVector<QPointF> *ptArr = ptTheme.GetData();
			for (int k = 0; k < ptArr->size(); k++)
				painter.drawPoint(ptArr->at(k));
		}
		for (int i = 0; i<m_polylines.size(); i++)
		{
			QColor color(i*20, 200, 255-i*15, 200);
			painter.setPen(QPen(color, renderWidth/500.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			
			PolygonTheme polyTheme = m_polylines.at(i);
			QVector<QPolygonF>* polyArr = polyTheme.GetData();
			for (int k=0; k<polyArr->size(); k++)
				painter.drawPolyline(polyArr->at(k));
		}
		for (int i = 0; i<m_polygons.size(); i++)
		{
			QColor color(255-i*20, 200, i*15, 100);
			painter.setPen(QPen(color, renderWidth/1000.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			QColor bcolor(255-i*5-50, 200, i*10, 100);
			painter.setBrush(QBrush(bcolor));

			PolygonTheme polyTheme = m_polygons.at(i);
			QVector<QPolygonF> *polyArr = polyTheme.GetData();

			for (int k=0; k<polyArr->size(); k++)
				painter.drawPolygon(polyArr->at(k));
		}
		m_updated = false;
	}
}
void RenderArea::clearAll()
{
	m_updated = true;

	m_points.clear();
	m_polygons.clear();
	m_polylines.clear();
	m_left = 10000000000.0; m_right = -10000000000.0; m_top = -10000000000.0; m_bottom = 10000000000.0;;
	update();
}

RenderArea::~RenderArea()
{
	//if(pixmap) 
	//{
	//	delete pixmap;
	//	pixmap = NULL;
	//}
}


QSize RenderArea::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize RenderArea::sizeHint() const
{
	return QSize(100, 100);
}
