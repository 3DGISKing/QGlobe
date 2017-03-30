#include "rendercursor.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "renderfrm.h"
#include "pointmark.h"

//rule cursor shafe image name
#define RC_RULER_SHAFENAME	"/cursor/cursor_ruler_normal.png"

RenderCursor::RenderCursor(RenderFrame *render)
{
	m_renderFrm = render;

	Init();
}

RenderCursor::~RenderCursor()
{
	Release();
}

void RenderCursor::Init()
{
	m_shape[CURSOR_SHAPE_NORMAL] = QCursor(Qt::OpenHandCursor);
	m_shape[CURSOR_SHAPE_SELECTED] = QCursor(Qt::ClosedHandCursor);
	m_shape[CURSOR_SHAPE_POINTING] = QCursor(Qt::PointingHandCursor);
	m_shape[CURSOR_SHAPE_RULER] = QCursor(g_app->GetResource(RC_RULER_SHAFENAME));
	m_shape[CURSOR_SHAPE_ARROW] = QCursor(Qt::ArrowCursor);
	m_shape[CURSOR_SHAPE_SIZEVERT] = QCursor(Qt::SizeVerCursor);
	m_shape[CURSOR_SHAPE_CROSS] = QCursor(Qt::CrossCursor);
	m_shape[CURSOR_SHAPE_NO] = QCursor(Qt::BlankCursor);

	SetCursor();
	m_pointMark = new PointMark(m_renderFrm);
	if (m_pointMark == NULL)
		return;
}

void RenderCursor::Release()
{
	if (m_pointMark)
	{
		delete m_pointMark;
		m_pointMark = NULL;
	}
}

void RenderCursor::SetCursor(unsigned int shape, CursorState *event)
{
	CURSOR_SHAPE cursorShape = (CURSOR_SHAPE) shape;
	if(m_currentCursor == cursorShape)
		return;
	m_currentCursor = cursorShape;
	if(m_currentCursor < CURSOR_SHAPE_POINTMARK)
		m_renderFrm->setCursor(m_shape[shape]);
	else
	{
		switch(m_currentCursor)
		{
		case CURSOR_SHAPE_POINTMARK:
			{
				if(m_pointMark && event)
				{
					m_pointMark->SetPosition(event);
				}
				break;
			}
		}
		m_renderFrm->update();
	}
}

void RenderCursor::PaintEvent(QPainter *painter)
{
	if(m_currentCursor < CURSOR_SHAPE_POINTMARK)
		return;
	switch(m_currentCursor)
	{
	case CURSOR_SHAPE_POINTMARK:
		{
			if(m_pointMark)
				m_pointMark->PaintEvent(painter);
			m_renderFrm->setCursor(m_shape[CURSOR_SHAPE_SIZEVERT]);
		}
		break;
	}
}
