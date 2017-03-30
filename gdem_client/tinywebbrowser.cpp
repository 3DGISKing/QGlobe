
#include "TinyWebBrowser.h"
#include <QtWebkit/QWebFrame>

TinyWebBrowser::TinyWebBrowser( QWidget* parent )
    : QFrame( parent )
     
{
	m_WebView=new QWebView(this);

	m_WebView->move(0,-25);
	
	QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	connect(m_WebView,SIGNAL(loadFinished(bool)),this,SLOT(onLoadFinished(bool)));
}

TinyWebBrowser::~TinyWebBrowser()
{
	delete m_WebView;

}

void TinyWebBrowser::load(QUrl url)
{
	m_WebView->load(url);
}

void TinyWebBrowser::setUrl(QUrl url)
{
	m_WebView->setUrl(url);
}

void TinyWebBrowser::onLoadFinished(bool ok)
{
	if(ok)
	{
		m_CurrentTitle=getCurrentTitle();
		emit titleChanged(m_CurrentTitle);
	}
	else
		m_CurrentTitle=tr("Failed to load html");
}
QString TinyWebBrowser::getCurrentTitle()
{
	QString html=m_WebView->page()->currentFrame()->toHtml();

	QString tmpStr, title;
	int startpos, endpos;

	startpos = html.indexOf("<!--Title Begin-->");
	if(startpos >= 0) 
	{
		endpos = html.indexOf("<!--Title End-->");
		tmpStr = html.mid(startpos, endpos - startpos);
		tmpStr = tmpStr.replace("<br>", "\n");
		tmpStr = tmpStr.replace("&nbsp;", " ");
		tmpStr = tmpStr.remove(QRegExp("<[^<]*>&nbsp;</[^<]*>"));
		tmpStr = tmpStr.remove(QRegExp("<[^<]*>|</[^<]*>"));
		title = tmpStr.trimmed();
	}

	return title;
}






