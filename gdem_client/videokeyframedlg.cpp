#include "stable.h"
#include "videokeyframedlg.h"
#include "keyframe.h"
#include <QMediaContent>
#include <QUrl>

VideoKeyFrameDlg::VideoKeyFrameDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowModality(Qt::ApplicationModal);
	m_VideoKeyFrame=new VideoKeyFrame();

	m_videoWidget=new QVideoWidget(this);
	m_videoWidget->setVisible(false);
	m_MediaObject.setVideoOutput(m_videoWidget);
	connect(&m_MediaObject, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateChanged(QMediaPlayer::State)));
	connect(&m_MediaObject, SIGNAL(durationChanged(qint64)), this, SLOT(onTotalTimeChanged()));

	connect(ui.pushButtonCancel,SIGNAL(clicked()),this,SLOT(onCancel()));
	connect(ui.pushButtonOk,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(ui.pushButtonVideoBrowser,SIGNAL(clicked()),this,SLOT(onBrowser()));
}

VideoKeyFrameDlg::~VideoKeyFrameDlg()
{
	delete m_VideoKeyFrame;
}

void VideoKeyFrameDlg::onOk()
{
	m_VideoKeyFrame->setFileName(ui.lineEditFileName->text());
	m_VideoKeyFrame->setTotalTime(ui.spinBoxTime->value());
	accept();
}
void VideoKeyFrameDlg::setData(VideoKeyFrame* vkf)
{
	*m_VideoKeyFrame=*vkf;
	ui.lineEditFileName->setText(vkf->getFileName());
	ui.spinBoxTime->setValue(vkf->totalTime());
}

void VideoKeyFrameDlg::onBrowser()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"/home",
		tr("Video File (*.avi *.mpg)"));

	if(fileName=="") return;

	m_MediaObject.setMedia(QUrl::fromLocalFile(fileName));
}


void VideoKeyFrameDlg::stateChanged(QMediaPlayer::State newstate)
{
	switch (newstate)
	{
	case QMediaPlayer::StoppedState:
		break;
	case QMediaPlayer::PausedState:
		break;
	case QMediaPlayer::PlayingState:
		break;
	}
}

void VideoKeyFrameDlg::onTotalTimeChanged()
{
	QString fileName = m_MediaObject.currentMedia().canonicalUrl().toLocalFile();

	if(fileName=="") 
		return;

	qint64 totoltime=m_MediaObject.duration();
	if(totoltime==0)
		return ;

	ui.lineEditFileName->setText(fileName);
	ui.spinBoxTime->setValue(totoltime);

	m_MediaObject.setMedia(QMediaContent());
}


