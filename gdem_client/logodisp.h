#ifndef LOGODISP_H
#define LOGODISP_H

#define LOGO_IMG_COUNT	31
#define LOGO_TIMER_INTERVAL	150

class LogoDisp
{
public:
    LogoDisp();
	~LogoDisp();

	bool Init();
	void Release();
	void Paint(QPainter *painter);

protected:
	QImage	*m_logoImgs[LOGO_IMG_COUNT];

};

#endif // LOGODISP_H
