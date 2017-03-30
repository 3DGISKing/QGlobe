#ifndef PLACEMARKWIN_H 

#define PLACEMARKWIN_H

#include <QtGui/QDialog>

class QPushButton;
class QHBoxLayout;
class TinyWebBrowser;
class MediaPlayer;
class QLabel;

class VideoWebView:	public QDialog
{
	Q_OBJECT
public:
	VideoWebView( QWidget * parent = 0, Qt::WindowFlags flags = 0);
	~VideoWebView(void);

	void setContent(QString videofilename,QString htmlfilename);
	void setVideo(QString videofilename,bool inanimation=false);
	void setHtml(QString htmlfilename,bool inanimation=false);
	void reset();
protected:
	virtual void hideEvent(QHideEvent* event);
private slots:
	void switchContent();
	void onHtmlTitleChanged(QString);
private:
	void setTitle(QString t);

private:
	TinyWebBrowser* m_WebBrowser;
	MediaPlayer*    m_MediaPlayer;
	QLabel*         m_TitleLabel;

	QPushButton* m_SwitchButton;
};

#endif
