#include "pointmark.h"
#include "gdemclient.h"
#include "renderfrm.h"
#include "rendercursor.h"

#define MARK_IMAGE_SIZE 30

PointMark::PointMark(RenderFrame *render)
{
	m_renderFrm = render;
	QString img = g_app->GetResource("/pointmark.png");
	m_img = QImage(img);
}

PointMark::~PointMark()
{
}

void PointMark::SetPosition(CursorState *event)
{
	m_x = event->x - MARK_IMAGE_SIZE;
	m_y = event->y - MARK_IMAGE_SIZE;
}

void PointMark::PaintEvent(QPainter *painter)
{
	painter->drawImage(m_x, m_y, m_img);
}
