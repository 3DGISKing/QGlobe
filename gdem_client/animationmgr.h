#ifndef ANIMATIONMGR_H
#define ANIMATIONMGR_H

#include <QtCore/QObject>
#include <QtCore/QBasicTimer>
#include <QtCore/QList>
#include <QtCore/QBitArray>
#include "keyframe.h"


class AnimationMgr : public QObject
{
	Q_OBJECT

public:
	AnimationMgr(QObject *parent=0);
	virtual ~AnimationMgr();

	void start();
	void stop();

	void setVideoFrame(QList<VideoKeyFrame> l) ;  
	void setCameraFrame(QList<CameraKeyFrame>l) ;
	void setCurrentTime(int t){m_CurrentTime=t;}
protected:
	virtual void timerEvent(QTimerEvent *event);
signals:
	void animationStarted();
	void animationStopped();
private:
	void cameraAnimate();
	void videoAnimate();

	QBasicTimer		       m_Timer;
	int                    m_CurrentTime;   //in millisecond
	int                    m_TimerInterval; //in millisecond 
	int                    m_TotalTime;     //in millisecond

	QList<VideoKeyFrame>   m_VideoFrameList;
	QList<CameraKeyFrame>  m_CameraFrameList;
	QBitArray              m_CameraFramePassed;
	QBitArray              m_VideoFramePassed;

};
extern AnimationMgr* g_pAnimationMgr;

#endif // ANIMATIONMGR_H
