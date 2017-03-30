#ifndef RENDERCURSOR_H
#define RENDERCURSOR_H

#include <QCursor>
#include "gdemclientbase.h"

class RenderFrame;
class PointMark;

class RenderCursor
{
public:
	RenderCursor(RenderFrame *render);
	~RenderCursor();

	CURSOR_SHAPE GetCurrentCursor()
	{	return m_currentCursor; }

	void Init();
	void Release();
	void SetCursor(unsigned int shape = 0, CursorState *event = NULL);
	void PaintEvent(QPainter *painter);

protected:
	RenderFrame		*m_renderFrm;
	QCursor			m_shape[CURSOR_SHAPE_COUNT];
	CURSOR_SHAPE	m_currentCursor;
	PointMark		*m_pointMark;
};

#endif // RENDERCURSOR_H
