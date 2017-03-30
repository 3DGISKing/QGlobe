
#ifndef TINYWEBBROWSER_H
#define TINYWEBBROWSER_H

#include <QtWebKit/QWebView>

class TinyWebBrowser : public QFrame
{
    Q_OBJECT

 public:
    explicit TinyWebBrowser( QWidget* parent = 0 );
    ~TinyWebBrowser();

	void load(QUrl url);
	void setUrl(QUrl url);
	QString getCurrentTitle();
private slots:
	void onLoadFinished(bool);

signals:
	void titleChanged(QString);
private:
	QWebView* m_WebView;
	QString   m_CurrentTitle;

};


#endif
