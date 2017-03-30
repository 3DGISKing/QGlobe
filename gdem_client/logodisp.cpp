#include "logodisp.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "renderfrm.h"

LogoDisp::LogoDisp()
{
	memset(m_logoImgs, 0, sizeof(m_logoImgs[0]) * LOGO_IMG_COUNT);
}

LogoDisp::~LogoDisp()
{
}

bool LogoDisp::Init()
{
	for (int i = 0; i < LOGO_IMG_COUNT; i++)
	{
		QString num = QString("%1").arg(i, 2, 10, QLatin1Char('0'));
		QString fname = QString("/logo/logomark-") + num + QString(".bmp");
		QString imgname = g_app->GetResource(fname);
		m_logoImgs[i] = new QImage(imgname);
		if (m_logoImgs[i] == NULL)
			return false;

		fname = QString("/logo/logomark-alpha-") + num + QString(".bmp");
		imgname = g_app->GetResource(fname);
		m_logoImgs[i]->setAlphaChannel(QImage(imgname));
	}

	return true;
}

void LogoDisp::Release()
{
	for (int i = 0; i < LOGO_IMG_COUNT; i++)
	{
		if (m_logoImgs[i] != NULL)
		{
			delete m_logoImgs[i];
			m_logoImgs[i] = NULL;

		}
	}
}

void LogoDisp::Paint(QPainter *painter)
{
#define LOGO_SCALE_SCREEN_WIDTH	1024
#define LOGO_SCALE_IMAGE_WIDTH	80


	QSize s = g_app->GetMainWindow()->GetRenderFrame()->size();
	int width = s.width()*LOGO_SCALE_IMAGE_WIDTH/LOGO_SCALE_SCREEN_WIDTH;
	int height = m_logoImgs[0]->height()*width/m_logoImgs[0]->width();
	int x = s.width() - width;
	int y = s.height() - height;
	QRect rect(x-5, y-5, width, height);

	QTime time = QTime::currentTime();
	unsigned long msec = time.hour()*3600000 + time.minute()*60000 + time.second()*1000 + time.msec();
	//unsigned long msec = gdm_GetTickTimer();
	int i = (int) (msec / LOGO_TIMER_INTERVAL) %  LOGO_IMG_COUNT;

	painter->save();

	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter->drawImage(rect, *m_logoImgs[i]);

	painter->restore();
}
