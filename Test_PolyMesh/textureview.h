#ifndef TEXTUREVIEW_H
#define TEXTUREVIEW_H

#include <QWidget>
#include "ui_textureview.h"

class RenderView;

class TextureView : public QWidget
{
	Q_OBJECT

public:
	TextureView(QWidget *parent = 0);
	~TextureView();

	void setRenderView(RenderView* view) { _RenderView = view; }

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);

private:
	RenderView*	_RenderView;
	QPixmap		_Pixmap;

	QPolygonF	_polygon;
	QPainterPath _Path;
	QColor fillColor;
	int penWidth;
	QColor penColor;
	int	_selectIndex;
	bool _editPoint;

	void recalcPath();

private:
	Ui::TextureViewClass ui;
};

#endif // TEXTUREVIEW_H
