#ifndef POINTMARK_H
#define POINTMARK_H

#include "gdemclientbase.h"

// 2009.11.27 Modified by ChaKI

class RenderFrame;

class PointMark
{
public:
	PointMark(RenderFrame *render);
	~PointMark();

	void SetPosition(CursorState *event);
	void PaintEvent(QPainter *painter);

protected:
	RenderFrame	*m_renderFrm;
	QImage		m_img;
	int			m_x;
	int			m_y;

};

#endif // POINTMARK_H
