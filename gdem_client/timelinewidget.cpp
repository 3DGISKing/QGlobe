#include "timelinewidget.h"

#include <QtGui/QPainter>
#include <QtGui/QMenu>
#include <QtGui/QResizeEvent>

#include "keyframedelegate.h"
#include "gdemclient.h"
#include "mainwindow.h"

#define DEFAULT_ROW_HEIGHT       35
#define DEFAULT_WIDTH_PER_SECOND 10

TimeLineDisplay::TimeLineDisplay(QWidget* parent/* =0 */)
:QLabel(parent)
{
	m_WidthPerSec=DEFAULT_WIDTH_PER_SECOND;

	m_TimeLineHeight=5;
	m_WidthPerOneScale=50;
	m_ScaleHeight=5;
	m_DigitHeight=3;
	m_OrigTime=0.0;
}

int TimeLineDisplay::totalScaleHeight()
{
	QFontMetrics metric(this->font());

	int yy=metric.height();
	return m_TimeLineHeight+m_ScaleHeight+m_DigitHeight+metric.height();
}

void TimeLineDisplay::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);

	painter.setBrush(QColor(0,0,0,127));
	painter.drawRect(0,0,width(),height());

	int w=width();
	int h=height();

	//draw bottom time line
	painter.drawLine(0,h-m_TimeLineHeight,w,h-m_TimeLineHeight);

	QFontMetrics metric(this->font());

	int rulergap=m_WidthPerOneScale;

	rulergap=(rulergap/m_WidthPerSec+1)*m_WidthPerSec;

	int gapcount=w/rulergap;

	for(float i=0;i<=gapcount;i++)
	{
		//draw scale
		painter.drawLine(i*rulergap,h-m_TimeLineHeight,i*rulergap,h-m_TimeLineHeight-m_ScaleHeight);

		double time=m_OrigTime+i*rulergap/m_WidthPerSec;

		QString label=QString::number(time);

		int labelwidth=metric.width(label);
		
		//draw number
		painter.drawText(i*rulergap-labelwidth/2+1,h-m_TimeLineHeight-m_ScaleHeight-m_DigitHeight,label);
	}
}

TimeLineWidget::TimeLineWidget(QWidget *parent)
:QTableWidget(parent)
{
	horizontalHeader()->setVisible(false);
	verticalHeader()->setVisible(false);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setRowCount(1);
	setRowHeight(0,DEFAULT_ROW_HEIGHT);
	
	m_WidthPerSecond=DEFAULT_WIDTH_PER_SECOND;
	setContextMenuPolicy(Qt::CustomContextMenu);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	m_TimeDisplay=new TimeLineDisplay(this);

	m_TimeDisplay->move(0,DEFAULT_ROW_HEIGHT);


	m_ScrollBar= horizontalScrollBar();
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

	setMinimumHeight(DEFAULT_ROW_HEIGHT+m_ScrollBar->height()+m_TimeDisplay->totalScaleHeight()-10);
	//setMaximumHeight(DEFAULT_ROW_HEIGHT+bar->height()+m_TimeDisplay->totalScaleHeight()-10);

	m_AddAction= new QAction(tr("Add"),this);
	m_DeleteAction =new QAction(tr("Delete"),this);
	connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),this, SLOT(onItemChanged(QTableWidgetItem*)));
	connect(m_AddAction,SIGNAL(triggered()),this,SLOT(onAdd()));
	connect(m_DeleteAction,SIGNAL(triggered()),this,SLOT(onDelete()));
	connect(m_ScrollBar,SIGNAL(valueChanged(int)),this,SLOT(onSliderChanged(int)));
}

void TimeLineWidget::resizeEvent(QResizeEvent* event)
{
	m_TimeDisplay->setGeometry(0,DEFAULT_ROW_HEIGHT,event->size().width(),m_TimeDisplay->totalScaleHeight());
}

TimeLineWidget::~TimeLineWidget()
{
	delete m_AddAction;
	delete m_DeleteAction;
	delete m_TimeDisplay;
};

double TimeLineWidget::getOrigTime()
{
	double val=m_ScrollBar->value();
	double time=val/(double)m_WidthPerSecond;
	return time;
}
void TimeLineWidget::onSliderChanged(int val)
{	
	double time=val/(double)m_WidthPerSecond;

	m_TimeDisplay->setOrigTime(time);
	emit sliderChanged(val);
}

void TimeLineWidget::onAdd()
{
	newFrame();
}

void TimeLineWidget::onDelete()
{
	QList<QTableWidgetItem*> lst=selectedItems();

	QTableWidgetItem* item;

	foreach(item,lst)
		removeColumn(item->column());
}

void TimeLineWidget::setWidthPerSec(int value)
{
	m_WidthPerSecond=value;

	m_TimeDisplay->setWidthPerSec(value);
	m_TimeDisplay->update();

	for(int i=0;i<columnCount();i++)
		setColumnWidth(i,frameTimeSeconds(i)*m_WidthPerSecond);
}

qint64 TimeLineWidget::totalTime()
{
	qint64 res=0;

	for(int i=0;i<columnCount();i++)
		res+=frameTime(i);
	return res;
}

void TimeLineWidget::setScrollPos(int value)
{
	m_ScrollBar->setValue(value);
}

void TimeLineWidget::onItemChanged(QTableWidgetItem* item)
{
	setColumnWidth(item->column(),frameTimeSeconds(item->column())*m_WidthPerSecond);
}

CameraTimeLineWidget::CameraTimeLineWidget(QWidget *parent)
:TimeLineWidget(parent)
{
	setItemDelegate(new CameraKeyFrameDelegate());
	m_GotoAction=new QAction(tr("Goto"),this);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(showContextMenu(QPoint)));
	connect(m_GotoAction,SIGNAL(triggered()),this,SLOT(onGoto()));
}

CameraTimeLineWidget::~CameraTimeLineWidget()
{
	QAbstractItemDelegate* delegate=itemDelegate();
	
	delete delegate;
	delete m_GotoAction;
};



void CameraTimeLineWidget::pushFrame(KeyFrame *para)
{
	Q_ASSERT(para->GetType()==KeyFrame::Camera);

	CameraKeyFrame* keyframe=(CameraKeyFrame*)para;

	int colcount=this->columnCount();
	setColumnCount(colcount+1);

	QString Name=keyframe->getName();

	if(Name=="")
		keyframe->setName(QString("Camera%1").arg(colcount));

	QTableWidgetItem * item=new QTableWidgetItem(keyframe->label());

	setColumnWidth(colcount,m_WidthPerSecond*keyframe->totalTimeSeconds());

	QVariant var;
	var.setValue(*keyframe);
	item->setData(Qt::UserRole,var);

	setItem(0,colcount,item);
	item->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/camera.png"));
}

void CameraTimeLineWidget::newFrame()
{
	CameraKeyFrame c;

	QList<QTableWidgetItem *> itemList=selectedItems();

	if(itemList.empty())
		pushFrame(&c);
	else
	{
		if(itemList[0]->column()+1==columnCount()) //user select last frame
			pushFrame(&c);
		else
			insertFrame(&c,itemList[0]->column());
	}
};

void CameraTimeLineWidget::insertFrame(KeyFrame* para,int i)
{
	Q_ASSERT(para->GetType()==KeyFrame::Camera);

	CameraKeyFrame* keyframe=(CameraKeyFrame*)para;

	this->insertColumn(i);

	QString Name=keyframe->getName();

	if(Name=="")
		keyframe->setName(QString("Camera%1").arg(i));

	QTableWidgetItem * item=new QTableWidgetItem(keyframe->label());

	setColumnWidth(i,m_WidthPerSecond*keyframe->totalTime()/1000);

	QVariant var;
	var.setValue(*keyframe);
	item->setData(Qt::UserRole,var);

	setItem(0,i,item);
	item->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/camera.png"));
}

CameraKeyFrame CameraTimeLineWidget::getFrame(int i)
{
	Q_ASSERT(i<columnCount());

	QTableWidgetItem* item=this->item(0,i);

	QVariant v;
	v=item->data(Qt::UserRole);
	CameraKeyFrame ckf=v.value<CameraKeyFrame>();
	return ckf;
}


void CameraTimeLineWidget::onGoto()
{
	QList<QTableWidgetItem*> lst=selectedItems();

	Q_ASSERT(lst.size()>0);

	CameraKeyFrame ckf=getFrame(lst[0]->column());

	GDM_CAMERA_INFO cameraInfo;	
	memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));
	cameraInfo.m_loc.m_dDist=ckf.getHeight();
	cameraInfo.m_loc.m_dLongitude=ckf.getLongitude()*gdm_DEGTORAD;
	cameraInfo.m_loc.m_dLatitude=ckf.getLatitude()*gdm_DEGTORAD;
	
	cameraInfo.m_orgDist=ckf.getAltitude();
	cameraInfo.m_roll=ckf.getHeading()*gdm_DEGTORAD;
	cameraInfo.m_pitch=ckf.getTilt()*gdm_DEGTORAD;
	
	g_app->GetRenderProxy()->GetRenderSrv()->m_pCameraController->AutoMoveTo(0, cameraInfo, ckf.getGotoTime());
}

void CameraTimeLineWidget::showContextMenu(QPoint p)
{
	QMenu menu(this);

	menu.addAction(m_AddAction);

	QList<QTableWidgetItem*> list=selectedItems();
	if(!list.isEmpty())
	{
		menu.addAction(m_DeleteAction);
		menu.addSeparator();
		menu.addAction(m_GotoAction);
	}

	QPoint p1=this->mapToGlobal(p);
	menu.exec(p1);
}

int   CameraTimeLineWidget::frameTime(int i)
{
	CameraKeyFrame ckf=getFrame(i);
	return ckf.totalTime();
}

VideoTimeLineWidget::VideoTimeLineWidget(QWidget *parent)
:TimeLineWidget(parent)
{
	setItemDelegate(new VideoKeyFrameDelegate());
	connect(this, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(showContextMenu(QPoint)));
}

VideoTimeLineWidget::~VideoTimeLineWidget()
{
	delete itemDelegate();
}


void VideoTimeLineWidget::pushFrame(KeyFrame *para)
{
	Q_ASSERT(para->GetType()==KeyFrame::Video);

	VideoKeyFrame* keyframe=(VideoKeyFrame*)para;

	int colcount=this->columnCount();
	setColumnCount(colcount+1);

	QTableWidgetItem * item=new QTableWidgetItem(keyframe->label());

	setColumnWidth(colcount,m_WidthPerSecond*keyframe->totalTimeSeconds());

	QVariant var;
	var.setValue(*keyframe);
	item->setData(Qt::UserRole,var);

	setItem(0,colcount,item);

	if(QFile::exists(keyframe->getFileName()))
		item->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/movie.png"));
	else
		if(keyframe->getFileName()!="")
			item->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/deletedmovie.png"));
}

void VideoTimeLineWidget::newFrame()
{
	VideoKeyFrame c;

	QList<QTableWidgetItem *> itemList=selectedItems();

	if(itemList.empty())
		pushFrame(&c);
	else
	{
		if(itemList[0]->column()+1==columnCount()) //user select last frame
			pushFrame(&c);
		else
			insertFrame(&c,itemList[0]->column());
	}
}

void VideoTimeLineWidget::insertFrame(KeyFrame* para,int i)
{
	Q_ASSERT(para->GetType()==KeyFrame::Video);

	VideoKeyFrame* keyframe=(VideoKeyFrame*)para;

	this->insertColumn(i);

	QTableWidgetItem * item=new QTableWidgetItem(keyframe->label());

	setColumnWidth(i,m_WidthPerSecond*keyframe->totalTimeSeconds());

	QVariant var;
	var.setValue(*keyframe);
	item->setData(Qt::UserRole,var);

	setItem(0,i,item);

	if(QFile::exists(keyframe->getFileName()))
		item->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/movie.png"));
	else
		if(keyframe->getFileName()!="")
			item->setIcon(QIcon(g_app->GetWorkDir()+"/res/animation/deletedmovie.png"));
}

VideoKeyFrame VideoTimeLineWidget::getFrame(int i)
{
	Q_ASSERT(i<columnCount());

	QTableWidgetItem* item=this->item(0,i);

	QVariant v;
	v=item->data(Qt::UserRole);
	VideoKeyFrame vkf=v.value<VideoKeyFrame>();
	return vkf;
}
void VideoTimeLineWidget::showContextMenu(QPoint p)
{
	QMenu menu(this);

	menu.addAction(m_AddAction);

	QList<QTableWidgetItem*> list=selectedItems();
	if(!list.isEmpty())
		menu.addAction(m_DeleteAction);

	QPoint p1=this->mapToGlobal(p);
	menu.exec(p1);
}

int   VideoTimeLineWidget::frameTime(int i)
{
	VideoKeyFrame vkf=getFrame(i);
	return vkf.totalTime();
}

void VideoTimeLineWidget::onItemChanged(QTableWidgetItem* item)
{
	TimeLineWidget::onItemChanged(item);

	VideoKeyFrame vkf=getFrame(item->column());


}