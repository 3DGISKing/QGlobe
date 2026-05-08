#ifndef VIDEOKEYFRAMEDLG_H
#define VIDEOKEYFRAMEDLG_H

#include <QDialog>
#include "ui_videokeyframedlg.h"

#include <QVideoWidget>
#include <QMediaPlayer>

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

	void stateChanged(QMediaPlayer::State newstate);
	void onTotalTimeChanged();

private:
	Ui::VideoKeyFrameDlg ui;
	VideoKeyFrame* m_VideoKeyFrame;

	QVideoWidget *m_videoWidget;
	QMediaPlayer m_MediaObject;
};

#endif // VIDEOKEYFRAMEDLG_H
