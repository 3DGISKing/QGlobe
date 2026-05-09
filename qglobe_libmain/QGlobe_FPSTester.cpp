#include "QGlobe_FPSTester.h"
#include "QGlobe_Interface.h"

CQGlobe_FPSTester g_FPSTester;

QGLOBE_LIBMAIN_EXPORT CQGlobe_FPSTester* qglobe_GetFPSTester()
{
	return &g_FPSTester;
}

CQGlobe_FPSTester::CQGlobe_FPSTester(void)
{
	m_time = 0;
	m_frame = 0;
	m_curFrame = 0;
	m_realFrame = 0;
	m_realTime = 0;
	m_realCount = 0;
	m_blRealDraw = false;
}

CQGlobe_FPSTester::~CQGlobe_FPSTester(void)
{
}

void CQGlobe_FPSTester::BeginDraw(long time)
{
	unsigned long newTime = time;
	if(m_blRealDraw)
	{
		m_realTime += (newTime - m_oldTime);
		m_realCount++;
		if(m_realTime > 200)
		{
			m_realFrame = m_realCount * 1000 / m_realTime;
			m_realTime = 0;
			m_realCount = 0;
		}
#ifdef DEBUG
		qDebug("[BeginDraw] Real FPS Count = %d\n" , m_realCount);
#endif
		m_blRealDraw = false;
	}
	m_oldTime = newTime;
}

void CQGlobe_FPSTester::EndDraw(long time)
{
	unsigned long newTime = time;

	m_time += (newTime - m_oldTime);
	m_frame++;
	if(m_time > 200)
	{
		m_curFrame = m_frame * 1000 / m_time;
		m_time = 0;
		m_frame = 0;
	}
	m_blRealDraw = true;
}
