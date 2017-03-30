#include "animationdlg.h"
#include "timelinewidget.h"

#include <QtGui/QSlider>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QFileDialog>

#include "Proxy.h"
#include "gdemclient.h"
#include "guiuty.h"

AnimationDlg::AnimationDlg(QWidget *parent)
	: QDialog(parent)
{
	m_CurrentTime=0;
	m_TimeLineSlider=new QSlider(this);
	m_TimeLineSlider->setOrientation(Qt::Horizontal);

	m_ScaleSlider=new QSlider(this);
	m_ScaleSlider->setOrientation(Qt::Horizontal);
	m_ScaleSlider->setRange(1,100);
	m_ScaleSlider->setSliderPosition(10);

	m_CameraTimeLine=new CameraTimeLineWidget(this);
	m_VideoTimeLine= new VideoTimeLineWidget(this);
	m_TimeLineDisplay=new TimeLineDisplay(this);
	m_TimeLineDisplay->setMinimumHeight(m_TimeLineDisplay->totalScaleHeight());
	
	QVBoxLayout* layout1=new QVBoxLayout();

	layout1->addWidget(m_CameraTimeLine);
	layout1->addWidget(m_VideoTimeLine);
	layout1->addWidget(m_TimeLineSlider);
	layout1->addWidget(m_TimeLineDisplay);

	QSize buttonSize(34, 28);

	m_ClearButton=new QPushButton(this);
	m_ClearButton->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/clear.png"));
	m_ClearButton->setMinimumSize(buttonSize);

	m_SaveButton=new QPushButton(this);
	m_SaveButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	m_SaveButton->setMinimumSize(buttonSize);

	m_CameraCaptureButton=new QPushButton(this);
	m_CameraCaptureButton->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/camera.png"));
	m_CameraCaptureButton->setMinimumSize(buttonSize);

	m_DeleteButton=new QPushButton(this);
	m_DeleteButton->setIcon(QIcon(g_app->GetWorkDir()+"/res/toolbar/delete.png"));
	m_DeleteButton->setMinimumSize(buttonSize);

	QHBoxLayout* toplayout=new QHBoxLayout();

	toplayout->addWidget(m_ClearButton);
	toplayout->addWidget(m_SaveButton);
	toplayout->addWidget(m_CameraCaptureButton);
	toplayout->addWidget(m_DeleteButton);
	toplayout->addWidget(m_ScaleSlider);

	QVBoxLayout* totallayout=new QVBoxLayout();

	totallayout->addLayout(toplayout);
	totallayout->addLayout(layout1);
	setLayout(totallayout);

	setMaximumHeight(buttonSize.height()+m_CameraTimeLine->height()+m_TimeLineSlider->height()+m_TimeLineDisplay->height());
	setMinimumWidth(800);

	connect(m_ScaleSlider,SIGNAL(valueChanged(int)),this,SLOT(scaleSliderValueChaned(int)));
	connect(m_ClearButton,SIGNAL(clicked()),this,SLOT(onClear()));
	connect(m_SaveButton,SIGNAL(clicked()),this,SLOT(onSave()));
	connect(m_CameraCaptureButton,SIGNAL(clicked()),this,SLOT(onCameraCapture()));
	connect(m_DeleteButton,SIGNAL(clicked()),this,SLOT(onDelete()));

	connect(m_CameraTimeLine,SIGNAL(sliderChanged(int)),this,SLOT(onCameraSliderChanged(int)));
	connect(m_VideoTimeLine,SIGNAL(sliderChanged(int)),this,SLOT(onVideoSliderChanged(int)));
}

AnimationDlg::~AnimationDlg()
{
	delete m_TimeLineSlider;
	delete m_ScaleSlider;
	delete m_CameraTimeLine;
	delete m_VideoTimeLine;
	delete m_TimeLineDisplay;


	delete m_SaveButton;
	delete m_CameraCaptureButton;
	delete m_DeleteButton; 
	delete m_ClearButton;
}


void AnimationDlg::onClear()
{
	m_CameraTimeLine->clear();
	m_CameraTimeLine->setColumnCount(0);
	
	m_VideoTimeLine->clear();
	m_VideoTimeLine->setColumnCount(0);
}

#include "animationmgr.h"

void AnimationDlg::play()
{
	g_pAnimationMgr->setCameraFrame(cameraFrameList());
	g_pAnimationMgr->setVideoFrame(videoFrameList());
	g_pAnimationMgr->setCurrentTime(currentTime()*1000);
	g_pAnimationMgr->start();
}

bool AnimationDlg::checkVideoFiles()
{
	QList<VideoKeyFrame> videoframlist;

	videoframlist=videoFrameList();

	VideoKeyFrame vkf;
	foreach(vkf,videoframlist)
	{
		if(!vkf.getFileName().isEmpty() && !QFile::exists(vkf.getFileName()))
		{
			gu_MessageBox(NULL,tr("Error"),tr("can not file: ")+vkf.getFileName(),MSGBOX_ICONCRITICAL);
			return false;
		}
	}

	return true;
}
void AnimationDlg::stop()
{
	g_pAnimationMgr->stop();
}

void AnimationDlg::onCameraCapture()
{
	PLACEMARK_DATA pmd ;

	RenderProxy *render = g_app->GetRenderProxy();
	render->SendMessage(PM_SNAPSHOT_PLACEDATA, (PrxParam)&pmd);

	CameraKeyFrame ckf;
	ckf.setLongitude(pmd.view_longitude);
	ckf.setLatitude(pmd.view_latitude);
	ckf.setHeight(pmd.view_range+pmd.orgDist);
	ckf.setHeading(pmd.view_heading);
	ckf.setTilt(pmd.view_tilt);
	ckf.setAltitude(pmd.orgDist);

	m_CameraTimeLine->pushFrame(&ckf);
}

void AnimationDlg::onDelete()
{
	QList<QTableWidgetItem*> lst=m_CameraTimeLine->selectedItems();
	
	QTableWidgetItem* item;

	foreach(item,lst)
		m_CameraTimeLine->removeColumn(item->column());
}

QList<VideoKeyFrame> AnimationDlg::videoFrameList()
{
	QList<VideoKeyFrame> res;

	for(int i=0;i<m_VideoTimeLine->columnCount();i++)
	{
		QTableWidgetItem* item=m_VideoTimeLine->item(0,i);

		QVariant v;
		v=item->data(Qt::UserRole);
		VideoKeyFrame vkf=v.value<VideoKeyFrame>();
		res.push_back(vkf);
	}
	return res;
}

QList<CameraKeyFrame> AnimationDlg::cameraFrameList()
{
	QList<CameraKeyFrame> res;

	for(int i=0;i<m_CameraTimeLine->columnCount();i++)
	{
		QTableWidgetItem* item=m_CameraTimeLine->item(0,i);

		QVariant v;
		v=item->data(Qt::UserRole);
		CameraKeyFrame ckf=v.value<CameraKeyFrame>();
		res.push_back(ckf);
	}
	return res;
}

#define SPACE " "
#define CAMERA_FRAME_INDENTIFIER "Camera"
#define VIDEO_FRAME_INDENTIFIER  "Video"

void AnimationDlg::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Save File"),
		"C:/",
		tr("Ani Files (*.ani)"));

	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

	QTextStream in(&file);

	QString line;
	QStringList tokenlist;

	QList<CameraKeyFrame> cameraframelist;
	QList<VideoKeyFrame> videoframelist;

	while(!in.atEnd())
	{
		line = in.readLine();	
		line.trimmed();

		if(line=="") 
			continue;
		tokenlist=line.split(SPACE);

		if(tokenlist.size()==0)
			continue;
		
		if(tokenlist[0]==CAMERA_FRAME_INDENTIFIER)
		{
			CameraKeyFrame ckf;
			ckf.setName(tokenlist[1]);
			ckf.setLongitude(tokenlist[2].toDouble());
			ckf.setLatitude(tokenlist[3].toDouble());
			ckf.setAltitude(tokenlist[4].toDouble());
			ckf.setHeight(tokenlist[5].toDouble());
			ckf.setHeading(tokenlist[6].toDouble());
			ckf.setTilt(tokenlist[7].toDouble());
			ckf.setGotoTime(tokenlist[8].toInt());
			ckf.setStopTime(tokenlist[9].toInt());
			cameraframelist.push_back(ckf);

		}
		else if(tokenlist[0]==VIDEO_FRAME_INDENTIFIER)
		{
			VideoKeyFrame vkf;
			vkf.setFileName(tokenlist[1]);
			vkf.setTotalTime(tokenlist[2].toInt());
			videoframelist.push_back(vkf);

			if(!QFile::exists(vkf.getFileName()) && !vkf.getFileName().isEmpty())
			{
				gu_MessageBox(NULL,tr("Error"),tr("can not file: ")+vkf.getFileName(),MSGBOX_ICONCRITICAL);
			}
		}
		else
			continue;
	}

	file.close();

	if(!cameraframelist.isEmpty())
		setCameraFrameList(cameraframelist);

	if(!videoframelist.isEmpty())
		setVideoFrameList(videoframelist);
}

void AnimationDlg::onSave()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		"C:/",
		tr("Ani Files (*.ani)"));

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return ;

	QTextStream out(&file);

	QList<CameraKeyFrame> cameraframelist=cameraFrameList();
	
	CameraKeyFrame ckf;

	foreach(ckf,cameraframelist)
	{
		out<<CAMERA_FRAME_INDENTIFIER<<SPACE;

		out<<ckf.getName()<<SPACE;
		out<<QString::number(ckf.getLongitude(),'f',15)<<SPACE;
		out<<QString::number(ckf.getLatitude(),'f',15)<<SPACE;
		out<<QString::number(ckf.getAltitude(),'f',6)<<SPACE;
		out<<QString::number(ckf.getHeight(),'f',6)<<SPACE;
		out<<QString::number(ckf.getHeading(),'f',10)<<SPACE;
		out<<QString::number(ckf.getTilt(),'f',10)<<SPACE;
		out<<QString::number(ckf.getGotoTime())<<SPACE;
		out<<QString::number(ckf.getStopTime())<<SPACE;
		out<<'\n';
	}
	
	QList<VideoKeyFrame> videoframelist=videoFrameList();

	VideoKeyFrame vkf;

	foreach(vkf,videoframelist)
	{
		out<<VIDEO_FRAME_INDENTIFIER<<SPACE;

		out<<vkf.getFileName()<<SPACE;
		out<<QString::number(vkf.totalTime())<<SPACE;
		out<<'\n';
	}
	file.close();
}

//l must not be empty 
void AnimationDlg::setCameraFrameList(QList<CameraKeyFrame> l)
{
	m_CameraTimeLine->clear();
	m_CameraTimeLine->setColumnCount(l.size());

	CameraKeyFrame ckf;

	for(int i=0;i<l.size();i++)
	{
		ckf=l[i];

		QTableWidgetItem* item=new QTableWidgetItem(ckf.label());

		QVariant var;
		var.setValue(ckf);
		item->setData(Qt::UserRole,var);

		m_CameraTimeLine->setItem(0,i,item);
		item->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/camera.png"));
	}
}

//l must not be empty 
void AnimationDlg::setVideoFrameList(QList<VideoKeyFrame> l)
{
	m_VideoTimeLine->clear();
	m_VideoTimeLine->setColumnCount(l.size());

	VideoKeyFrame vkf;

	for(int i=0;i<l.size();i++)
	{
		vkf=l[i];

		QTableWidgetItem* item=new QTableWidgetItem(vkf.label());

		QVariant var;
		var.setValue(vkf);
		item->setData(Qt::UserRole,var);

		m_VideoTimeLine->setItem(0,i,item);

		if(QFile::exists(vkf.getFileName()))
			item->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/movie.png"));
		else
		{
			if(vkf.getFileName()!="")
				item->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/deletedmovie.png"));
		}
	}
}

void AnimationDlg::scaleSliderValueChaned(int value)
{
	m_CameraTimeLine->setWidthPerSec(value);
	m_VideoTimeLine->setWidthPerSec(value);
}
int AnimationDlg::currentTime()
{
	//m_ScaleSlider->value() is identical with width for 1s
	double totaltime=m_TimeLineSlider->width()/m_ScaleSlider->value();
	m_CurrentTime=totaltime*m_TimeLineSlider->value()/m_TimeLineSlider->maximum();
	m_CurrentTime=m_CurrentTime+m_CameraTimeLine->getOrigTime();
	return m_CurrentTime;
}

void AnimationDlg::onCameraSliderChanged(int val)
{
	m_VideoTimeLine->setScrollPos(val);
	m_TimeLineDisplay->setOrigTime(m_CameraTimeLine->getOrigTime());
	m_TimeLineDisplay->update();
}

void AnimationDlg::onVideoSliderChanged(int val)
{
	m_CameraTimeLine->setScrollPos(val);
	m_TimeLineDisplay->setOrigTime(m_VideoTimeLine->getOrigTime());
	m_TimeLineDisplay->update();
}