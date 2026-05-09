#pragma once

#include "qglobe_libmain_global.h"

class QGLOBE_LIBMAIN_EXPORT CQGlobe_FPSTester
{
public:
	CQGlobe_FPSTester();
	~CQGlobe_FPSTester();
	void BeginDraw(long time);
	void EndDraw(long time);

public:
	long m_curFrame;
	long m_realFrame;
	long m_totalMeshCount;
	long m_totalLineCount;
private:
	unsigned long m_oldTime;
	unsigned long m_time;
	unsigned long m_realTime;
	int		m_realCount;
	int		m_frame;
	bool	m_blRealDraw;
};

QGLOBE_LIBMAIN_EXPORT CQGlobe_FPSTester* qglobe_GetFPSTester();
