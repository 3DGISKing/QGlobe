#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "gdm_renderview.h"

class RenderCursor;
class NavBar;
class RenderText;
class RulerDlg;
class PlaceMarkMgr;
class LogoDisp;
class TourPlayBar;
class FlightSimCtrl;
class CarSimCtrl;

#define SCALE_X(x)	((x)*(g_app->GetMainWindow()->GetRenderFrame()->width()))/1280
#define SCALE_Y(y)	((y)*(g_app->GetMainWindow()->GetRenderFrame()->height()))/956

class RenderFrame : public RenderView
{
	Q_OBJECT
public:
    RenderFrame(QWidget *parent=0);
    ~RenderFrame();

public:
	RenderCursor* GetRenderCursor()
	{	return m_renderCursor; }

	NavBar* GetNavBar()
	{   return m_navBar; }

	RenderText* GetRenderText()
	{	return m_renderText; }

	PlaceMarkMgr* GetPlaceMarkMgr()
	{	return m_placeMarkMgr; }

	bool Init();
	void Release();
	void DrawItem(QPainter *painter);
	void CreateTourPlayBar();
	void DestroyTourPlayBar();
	void CreateFlightSimCtrl();
	void DestroyFlightSimCtrl();
	void CreateCarSimCtrl();
	void DestroyCarSimCtrl();
	void ResetRenderKey();

signals:
	void clicked();
	void dropfile(const QString &filename);

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);

	virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void timerEvent(QTimerEvent *event);

	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

protected slots:
	void mouseClickEventSlot();

private:
	void UpdateLogo();
	void CheckClicked(const CursorState *org, const CursorState *cur);
	void SetRenderKey(unsigned int key, bool enable);
	void CheckRenderKey(bool force=false);

	RenderCursor	*m_renderCursor;
	CursorState		m_curState;
	CursorState		m_clickCur;
	unsigned int	m_renderKeys;
	NavBar			*m_navBar;
	RenderText		*m_renderText;
	PlaceMarkMgr	*m_placeMarkMgr;
	
	LogoDisp		*m_logoDisp;
	QImage			m_logoImg;
	QBasicTimer		m_moveTimer;
	QBasicTimer		m_logoTimer;
	TourPlayBar		*m_tourPlayBar;
	FlightSimCtrl	*m_flightSimCtrl;
	CarSimCtrl		*m_carSimCtrl;
};

#endif // GLWIDGET_H
