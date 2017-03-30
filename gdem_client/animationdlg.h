#ifndef ANIMATIONDLG_H
#define ANIMATIONDLG_H

#include <QtCore/QList>
#include <QtGui/QDialog>
#include "keyframe.h"

class QSlider;
class CameraTimeLineWidget;
class VideoTimeLineWidget;
class TimeLineDisplay;
class QPushButton;

class AnimationDlg : public QDialog
{
	Q_OBJECT

public:
	AnimationDlg(QWidget *parent = 0);
	~AnimationDlg();
	void open();
	void play();
	void stop();

	bool checkVideoFiles();
private slots:
	void onClear();
	void onSave();
	void onCameraCapture();
	void onDelete();

	void scaleSliderValueChaned(int);
	int  currentTime();  //in second
	void onCameraSliderChanged(int);
	void onVideoSliderChanged(int);
private:
	QList<VideoKeyFrame>  videoFrameList();
	QList<CameraKeyFrame> cameraFrameList();
	void setVideoFrameList(QList<VideoKeyFrame> l);
	void setCameraFrameList(QList<CameraKeyFrame> l);
	
private:
	QSlider*              m_TimeLineSlider;
	QSlider *             m_ScaleSlider;

	CameraTimeLineWidget* m_CameraTimeLine;
	VideoTimeLineWidget*  m_VideoTimeLine;
	TimeLineDisplay*      m_TimeLineDisplay; 

	QPushButton*          m_ClearButton; 
	QPushButton*          m_SaveButton; 
	QPushButton*          m_CameraCaptureButton; 
	QPushButton*          m_DeleteButton; 

	int                   m_CurrentTime; //in seconds
};
#endif // ANIMATIONDLG_H
