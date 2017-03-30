#include "stable.h"
#include "videokeyframedlg.h"
#include "keyframe.h"

VideoKeyFrameDlg::VideoKeyFrameDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowModality(Qt::ApplicationModal);
	m_VideoKeyFrame=new VideoKeyFrame();

	m_videoWidget=new Phonon::VideoWidget(this);
	m_videoWidget->setVisible(false);
	Phonon::createPath(&m_MediaObject, m_videoWidget);
	Phonon::createPath(&m_MediaObject, &m_AudioOutput);
	connect(&m_MediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
	connect(&m_MediaObject, SIGNAL(currentSourceChanged(const Phonon::MediaSource&)), this, SLOT(sourceChanged(const Phonon::MediaSource&)));
	connect(&m_MediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(onTotalTimeChanged()));

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

	m_MediaObject.setCurrentSource(Phonon::MediaSource(fileName));
}


void VideoKeyFrameDlg::stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
	switch (newstate)
	{
	case Phonon::ErrorState:
		//QMessageBox::warning(this, "Phonon Mediaplayer", m_MediaObject.errorString(), QMessageBox::Close);
		break;

	case Phonon::StoppedState:
		{

		}
		break;
	case Phonon::PausedState:

		break;
	case Phonon::PlayingState:

		// Fall through
	case Phonon::BufferingState:

		break;
	case Phonon::LoadingState:

		break;
	}
}

void VideoKeyFrameDlg::onTotalTimeChanged()
{
	Phonon::MediaSource s=m_MediaObject.currentSource();

	QString fileName=s.fileName();

	if(fileName=="") 
		return;

	qint64 totoltime=m_MediaObject.totalTime();
	if(totoltime==0)
		return ;

	ui.lineEditFileName->setText(fileName);
	ui.spinBoxTime->setValue(totoltime);

	m_MediaObject.setCurrentSource(Phonon::MediaSource(""));
}


