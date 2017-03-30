#ifndef VIDEOKEYFRAMEDLG_H
#define VIDEOKEYFRAMEDLG_H

#include <QDialog>
#include "ui_videokeyframedlg.h"

#include <phonon/videowidget.h>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>

class VideoKeyFrame;

class VideoKeyFrameDlg : public QDialog
{
	Q_OBJECT

public:
	VideoKeyFrameDlg(QWidget *parent = 0);
	~VideoKeyFrameDlg();
	void setData(VideoKeyFrame* vkf);
	VideoKeyFrame* getData() {return m_VideoKeyFrame;}
private slots:
	void onOk();
	void onCancel(){reject();}
	void onBrowser();

	void stateChanged(Phonon::State newstate, Phonon::State oldstate);
	void onTotalTimeChanged();

private:
	Ui::VideoKeyFrameDlg ui;
	VideoKeyFrame* m_VideoKeyFrame;

	Phonon::VideoWidget *m_videoWidget;
	Phonon::MediaObject m_MediaObject;
	Phonon::AudioOutput m_AudioOutput;
};

#endif // VIDEOKEYFRAMEDLG_H
