#pragma once

#include "gdem_libmain_global.h"

class GDEM_LIBMAIN_EXPORT CGDM_FPSTester
{
public:
	CGDM_FPSTester();
	~CGDM_FPSTester();
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

GDEM_LIBMAIN_EXPORT CGDM_FPSTester* gdm_GetFPSTester();
