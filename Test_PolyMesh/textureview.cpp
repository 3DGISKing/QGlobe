#include "textureview.h"
#include "renderview.h"
#include <QMouseEvent>
#include <QPainter>

TextureView::TextureView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_RenderView = NULL;

	penColor = QColor(0, 0, 0, 255);
	fillColor = QColor(0, 0, 255, 255);
	penWidth = 1;
	_selectIndex = -1;
	_editPoint = false;

	_Pixmap = QPixmap(256, 256);
	_Pixmap.fill(QColor(Qt::transparent));
}

TextureView::~TextureView()
{
	_polygon.clear();
}

void TextureView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
/*
	//! [8] //! [9]
	//painter.scale(width() / 100.0, height() / 100.0);
	//painter.translate(50.0, 50.0);
	//painter.rotate(-rotationAngle);
	//painter.translate(-50.0, -50.0);

	//! [9] //! [10]
	painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));
	painter.setBrush(QBrush(fillColor));
	painter.drawPath(_Path);
*/
	QRectF dest(0, 0, 256, 256);
	QRectF src(0, 0, 256, 256);

	painter.fillRect(rect(), QBrush(QColor(128, 128, 128, 128), Qt::DiagCrossPattern));
	//painter.drawPixmap(dest, _Pixmap, src);

	QImage image = _Pixmap.toImage();

	painter.drawImage(dest, image);

	for(int i=0 ; i<_polygon.size() ; i++)
	{
		QColor color = QColor(255, 0, 0, 255);
		if(i == _selectIndex)
			color = QColor(0, 0, 255, 255);
		painter.fillRect(_polygon[i].x() - 2, _polygon[i].y() - 2, 4, 4, color);
	}
}

void TextureView::mousePressEvent(QMouseEvent *event)
{
	_editPoint = false;
	_selectIndex = -1;

	for(int i=0 ; i<_polygon.size() ; i++)
	{
		QRectF rect;
		rect.setRect(_polygon[i].x() - 3, _polygon[i].y() - 3, 6, 6);
		
		if(rect.contains(event->posF()))
		{
			_selectIndex = i;
			_editPoint = true;
			break;
		}
	}

	repaint();
}

void TextureView::mouseReleaseEvent(QMouseEvent *event)
{

	if(event->button() == Qt::RightButton)
	{
		if(_editPoint)
		{
			_polygon.remove(_selectIndex);
			recalcPath();
			_selectIndex = -1;
			_editPoint = false;
		}
		return;
	}

	if(!_editPoint)
	{
		QPoint pt = event->pos();
		pt.setX(pt.x() % 256);
		pt.setY(pt.y() % 256);
		_polygon.append(pt);
	}
	recalcPath();
}

void TextureView::mouseMoveEvent(QMouseEvent *event)
{
	if(_editPoint)
	{
		_polygon[_selectIndex].setX(event->posF().x());
		_polygon[_selectIndex].setY(event->posF().y());
		recalcPath();
	}
}

void TextureView::recalcPath()
{
	QPainterPath path;
	if(_polygon.size() > 0)
	{
		path.moveTo(_polygon[0]);

		for(int i=1 ; i<_polygon.size() ; i++)
			path.lineTo(_polygon[i]);

		path.closeSubpath();
	}

	_Path = path;

	_Pixmap.fill(QColor(0, 0, 0, 0));

	QPainter painter(&_Pixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	//painter.setPen(QPen(penColor, 0, Qt::SolidLine, Qt::RoundCap,
	//	Qt::RoundJoin));
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(fillColor));
	painter.drawPath(_Path);

	repaint();

	_RenderView->SetPolygonTexture(&_Pixmap);
}
