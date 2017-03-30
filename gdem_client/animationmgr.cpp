#include "animationmgr.h"

AnimationMgr* g_pAnimationMgr=NULL;
#include "gdemclient.h"
#include "mainwindow.h"

AnimationMgr::AnimationMgr(QObject *parent)
	: QObject(parent)
{
	m_CurrentTime=0;
	m_TotalTime=0;
	m_TimerInterval=100;
	g_pAnimationMgr=this;
}

AnimationMgr::~AnimationMgr()
{

}

void AnimationMgr::timerEvent(QTimerEvent *event)
{
	cameraAnimate();
	videoAnimate();
	m_CurrentTime+=m_TimerInterval;

	if(m_CurrentTime>=m_TotalTime)
		stop();
}

void AnimationMgr::cameraAnimate()
{
	int cameraframeidx;

	int time=0;

	for( cameraframeidx=0;cameraframeidx<m_CameraFrameList.size();cameraframeidx++)
	{
		time+=m_CameraFrameList[cameraframeidx].totalTime();
		if(m_CurrentTime<time)
			break;
	}

	if(cameraframeidx==m_CameraFrameList.size())
		return;

	if(m_CameraFramePassed.testBit(cameraframeidx))
		return;

	m_CameraFramePassed.setBit(cameraframeidx);

	CameraKeyFrame ckf;
	ckf=m_CameraFrameList[cameraframeidx];

	GDM_CAMERA_INFO cameraInfo;	
	memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));
	cameraInfo.m_loc.m_dDist=ckf.getHeight();
	cameraInfo.m_loc.m_dLongitude=ckf.getLongitude()*gdm_DEGTORAD;
	cameraInfo.m_loc.m_dLatitude=ckf.getLatitude()*gdm_DEGTORAD;

	cameraInfo.m_roll=ckf.getHeading()*gdm_DEGTORAD;
	cameraInfo.m_pitch=ckf.getTilt()*gdm_DEGTORAD;
	cameraInfo.m_orgDist=ckf.getAltitude();

	g_app->GetRenderProxy()->GetRenderSrv()->m_pCameraController->AutoMoveTo(0, cameraInfo, ckf.getGotoTime());
}

void AnimationMgr::start()
{
	emit animationStarted();
	m_VideoFramePassed.resize(m_VideoFrameList.size());
	m_CameraFramePassed.resize(m_CameraFrameList.size());
	m_VideoFramePassed.fill(false);
	m_CameraFramePassed.fill(false);

	m_Timer.start(m_TimerInterval,this);	
}

void AnimationMgr::stop()
{
	emit animationStopped();

	m_Timer.stop();
	m_CurrentTime=0;
	m_CameraFramePassed.fill(false);
	m_VideoFramePassed.fill(false);
}

void AnimationMgr::setCameraFrame(QList<CameraKeyFrame> l)
{
	m_CameraFrameList.clear();
	m_CameraFrameList=l;

	CameraKeyFrame ckf;

	int time=0;
	foreach(ckf,m_CameraFrameList)
	{
		time+=ckf.totalTime();
	}
	if(m_TotalTime<time)
		m_TotalTime=time;
}

void AnimationMgr::setVideoFrame(QList<VideoKeyFrame> l)
{
	m_VideoFrameList.clear();
	m_VideoFrameList=l;

	VideoKeyFrame vkf;

	int time=0;
	foreach(vkf,m_VideoFrameList)
	{
		time+=vkf.totalTime();
	}
	if(m_TotalTime<time)
		m_TotalTime=time;
}

void AnimationMgr::videoAnimate()
{
	int videoframeidx;

	int time=0;

	for( videoframeidx=0;videoframeidx<m_VideoFrameList.size();videoframeidx++)
	{
		time+=m_VideoFrameList[videoframeidx].totalTime();
		if(m_CurrentTime<time)
			break;
	}

	if(videoframeidx==m_VideoFrameList.size())
		return;

	if(m_VideoFramePassed.testBit(videoframeidx))
		return;

	m_VideoFramePassed.setBit(videoframeidx);

	VideoKeyFrame vkf;
	vkf=m_VideoFrameList[videoframeidx];

	if(!QFile::exists(vkf.getFileName()))
		return;

	g_app->GetMainWindow()->ShowVideoWebView(vkf.getFileName(),"",false,true);
}


