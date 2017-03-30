#include "videowebview.h"

#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include "tinywebbrowser.h"
#include "mediaplayer.h"

#define DEFAULT_VIDEO_WIDTH  640
#define DEFAULT_VIDEO_HEIGHT 565

#define DEFAULT_HTML_WIDTH   800
#define DEFAULT_HTML_HEIGHT  424


#define MINI_SCALE 0.7
#define MINI_VIDEO_WIDTH  DEFAULT_VIDEO_WIDTH*MINI_SCALE
#define MINI_VIDEO_HEIGHT DEFAULT_VIDEO_HEIGHT*MINI_SCALE
 
#define MINI_HTML_WIDTH   DEFAULT_HTML_WIDTH*MINI_SCALE
#define MINI_HTML_HEIGHT  DEFAULT_HTML_HEIGHT*MINI_SCALE

VideoWebView::VideoWebView(QWidget * parent, Qt::WindowFlags flags)
:QDialog(parent, flags)
{
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	setStyleSheet("background-image:url(:/image/placemarkcontent/back.png);background-repeat:repeat-xy;");

	QWidget* titlewidget=new QWidget(this);
	titlewidget->setStyleSheet("background-image:url(:/image/placemarkcontent/titlebar.png);background-repeat:repeat-x;");

	QSize buttonSize(41, 20);

	m_SwitchButton=new QPushButton(titlewidget);
	m_SwitchButton->setMaximumSize(buttonSize);
	m_SwitchButton->setStyleSheet("background-image:url(:/image/placemarkcontent/play.png);border: none;");

	QPushButton*    m_CloseButton=new QPushButton(titlewidget);
	m_CloseButton->setMaximumSize(buttonSize);
	m_CloseButton->setStyleSheet("background-image:url(:/image/placemarkcontent/close.png);border: none;");

	m_TitleLabel=new QLabel(titlewidget);
	
	m_TitleLabel->setAlignment(Qt::AlignCenter);

	QSpacerItem* horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

	QHBoxLayout *title_layout=new QHBoxLayout();

	title_layout->setContentsMargins(0, 0, 0, 0);
	title_layout->addWidget(m_TitleLabel);
	title_layout->addWidget(m_SwitchButton);
	title_layout->addWidget(m_CloseButton);
	title_layout->addItem(horizontalSpacer);

	titlewidget->setLayout(title_layout);
	titlewidget->setMaximumHeight(30);

	m_MediaPlayer=new MediaPlayer(this);
	m_WebBrowser=new TinyWebBrowser(this);

	QVBoxLayout* totoallayout=new QVBoxLayout;
	
	totoallayout->setSpacing(0);
	totoallayout->addWidget(titlewidget);
	totoallayout->addWidget(m_MediaPlayer);
	totoallayout->addWidget(m_WebBrowser);

	totoallayout->setContentsMargins(0, 5, 0, 0);

	setLayout(totoallayout);

	connect(m_CloseButton,SIGNAL(clicked()),this,SLOT(hide()));
	connect(m_SwitchButton,SIGNAL(clicked()),this,SLOT(switchContent()));
	connect(m_MediaPlayer,SIGNAL(videoFinished()),this,SLOT(hide()));
	connect(m_WebBrowser,SIGNAL(titleChanged(QString)),this,SLOT(onHtmlTitleChanged(QString)));

}

VideoWebView::~VideoWebView(void)
{
	
}

void VideoWebView::onHtmlTitleChanged(QString title)
{
	if(m_WebBrowser->isVisible())
		setTitle(m_WebBrowser->getCurrentTitle());
}

//precontition
//htmlfilename and videofilename must be existed and valid

void VideoWebView::setContent(QString videofilename,QString htmlfilename)
{
	resize(DEFAULT_HTML_WIDTH,DEFAULT_HTML_HEIGHT);

	m_MediaPlayer->show();
	m_MediaPlayer->setFile(videofilename);

	QString urlstr="file:///"+htmlfilename;
	m_WebBrowser->load(QUrl(urlstr));
	m_WebBrowser->hide();

	m_SwitchButton->show();
	setTitle(m_MediaPlayer->getCurrentTitle());
}

void VideoWebView::reset()
{
	m_MediaPlayer->setFile("");
	m_WebBrowser->load(QUrl(""));
}

void VideoWebView::switchContent()
{
	if(m_WebBrowser->isVisible())
	{
		m_WebBrowser->hide();

		m_MediaPlayer->show();
		m_MediaPlayer->play();
		setTitle(m_MediaPlayer->getCurrentTitle());
	}
	else if(m_MediaPlayer->isVisible())
	{
		m_MediaPlayer->hide();
		m_MediaPlayer->pause();

		m_WebBrowser->show();
		setTitle(m_WebBrowser->getCurrentTitle());
	}
}

//only html
void VideoWebView::setHtml(QString htmlfilename,bool movotocorner)
{
	if(movotocorner)
		resize(MINI_HTML_WIDTH,MINI_VIDEO_HEIGHT);
	else
		resize(DEFAULT_HTML_WIDTH,DEFAULT_HTML_HEIGHT);

	m_SwitchButton->hide();
	m_MediaPlayer->hide();
	
	QString urlstr="file:///"+htmlfilename;
	m_WebBrowser->load(QUrl(urlstr));
	m_WebBrowser->show();
	//setTitle(m_WebBrowser->getCurrentTitle());
}

//only video
void VideoWebView::setVideo(QString videofilename,bool inanimation)
{
	if(inanimation)
		resize(MINI_VIDEO_WIDTH,MINI_VIDEO_HEIGHT);
	else
		resize(DEFAULT_VIDEO_WIDTH,DEFAULT_VIDEO_HEIGHT);

	m_SwitchButton->hide();
	m_WebBrowser->hide();

	m_MediaPlayer->setFile(videofilename);
	m_MediaPlayer->show();
	setTitle(m_MediaPlayer->getCurrentTitle());
}

void VideoWebView::setTitle(QString t)
{
	m_TitleLabel->setText(t);
}

void VideoWebView::hideEvent(QHideEvent* event)
{
	m_MediaPlayer->pause();
}