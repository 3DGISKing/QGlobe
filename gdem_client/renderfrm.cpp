#include <QPainter>

#include "gdemclient.h"
#include "config.h"
#include "mainwindow.h"
#include "renderfrm.h"
#include "rendercursor.h"
#include "navbar.h"
#include "rendertext.h"
#include "rulerdlg.h"
#include "placemarkmgr.h"
#include "placemarkdlg.h"
#include "pointmark.h"
#include "logodisp.h"
#include "tourplaybar.h"
#include "flightsimctrl.h"
#include "carsimctrl.h"
#include "GDM_CommandMgr.h"
#include "proxy.h"
#include "../gdem_scene/GDM_SceneManager.h"//2013 2 1 mapscenenode
#include "../gdem_scene/MapSceneNode.h"    //2013 2 1 mapscenenode
#define URL_MARK	"/"

RenderFrame::RenderFrame(QWidget *parent)
: RenderView(parent)
{
	memset(&m_curState, 0, sizeof(m_curState));
	memset(&m_clickCur, 0, sizeof(m_clickCur));
	m_renderKeys = 0;

	m_renderCursor = NULL;
	m_navBar = NULL;
	m_renderText = NULL;
	m_placeMarkMgr = NULL;
	m_logoDisp = NULL;
	m_tourPlayBar = NULL;
	m_flightSimCtrl = NULL;
	m_carSimCtrl = NULL;

	setMinimumSize(640, 480);
	setAutoFillBackground(false);
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
	setAcceptDrops(true);

	connect(this, SIGNAL(clicked()), this, SLOT(mouseClickEventSlot()));
}

RenderFrame::~RenderFrame()
{
}

bool RenderFrame::Init()
{
	m_renderCursor = new RenderCursor(this);
	if (m_renderCursor == NULL)
		return false;
	m_renderCursor->SetCursor(CURSOR_SHAPE_NORMAL);

	m_placeMarkMgr = new PlaceMarkMgr();
	if (m_placeMarkMgr == NULL)
		return false;

	//m_logoDisp = new LogoDisp();
	//if (m_logoDisp == NULL)
	//	return false;
	//if (!m_logoDisp->Init())
	//	return false;

	//crate navigation control
	m_navBar = new NavBar;
	if (m_navBar == NULL)
		return false;
	m_navBar->SetView(this);
	if (!m_navBar->Init())
		return false;

	//create render text
	m_renderText = new RenderText;
	if (m_renderText == NULL)
		return false;
	m_renderText->SetView(this);
	if (!m_renderText->Init())
		return false;

	//crete timer
	m_moveTimer.start(NAV_TIMER_INTERVAL, this);

	/*********************************************
	2013 3 7 ugi
	logoÅé ÆÉÎ§Ê«»Ì Í±Äñ °ÇÎ¨ÎÌ Óª Ë²Óª¶¹Åé ±Ì³ÝÆðÌÂ É¤Íþ°¡ ÍÖÆíÄû³¿.
	*********************************************/
	//m_logoTimer.start(LOGO_TIMER_INTERVAL, this);

	return true;
}

void RenderFrame::Release()
{
	m_moveTimer.stop();
	m_logoTimer.stop();

	if (m_renderCursor)
	{
		delete m_renderCursor;
		m_renderCursor = NULL;
	}

	if(m_placeMarkMgr)
	{
		delete m_placeMarkMgr;
		m_placeMarkMgr = NULL;
	}

	if (m_logoDisp)
	{
		m_logoDisp->Release();
		delete m_logoDisp;
		m_logoDisp = NULL;
	}

	if (m_navBar)
	{
		m_navBar->Release();
		delete m_navBar;
		m_navBar = NULL;
	}

	if (m_tourPlayBar)
		DestroyTourPlayBar();

	if (m_flightSimCtrl)
		DestroyFlightSimCtrl();

	if (m_carSimCtrl)
		DestroyCarSimCtrl();

	if (m_renderText)
	{
		m_renderText->Release();
		delete m_renderText;
		m_renderText = NULL;
	}
}

void RenderFrame::initializeGL()
{
	makeCurrent();
	g_app->InitializeGL();

	setAttribute( Qt::WA_OpaquePaintEvent );
}

void RenderFrame::resizeGL(int w, int h)
{
	if (m_navBar)
		m_navBar->ChangedViewSize(w, h);

	g_app->ChangedRenderSize(w, h);
}

void RenderFrame::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	GDM_RENDER_INFO rednerInfo;

	RenderProxy *renderProxy = g_app->GetRenderProxy();
	if (renderProxy)
	{
		rednerInfo.qPainter = &painter;
		renderProxy->SendMessage(PM_PAINT, (PrxParam)&rednerInfo);
	}

	g_app->GetMainWindow()->UpdateStatusbar();
	DrawItem(&painter);
}

void RenderFrame::keyPressEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
		return;

    int key = event->key();
	switch(key)
	{
	case Qt::Key_Underscore:
		{
			key = Qt::Key_Minus;
			break;
		}
	case Qt::Key_Equal:
		{
			key = Qt::Key_Plus;
			break;
		}
	}


	KeyState keyState;
	memset(&keyState , 0 , sizeof(KeyState));
	keyState.keyFlags = m_curState.keys;
	keyState.key1 = key;
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();

    switch (key)
    {
        case Qt::Key_Control:
			m_curState.keys |= KS_CTRL;
            break;

        case Qt::Key_Shift:
			m_curState.keys |= KS_SHIFT;
            break;

        case Qt::Key_Alt:
			m_curState.keys |= KS_ALT;
            break;

        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
		case Qt::Key_A:
		case Qt::Key_D:
		case Qt::Key_W:
		case Qt::Key_S:
			SetRenderKey(key, true);
			break;
		case Qt::Key_Plus:
		case Qt::Key_Minus:
		case Qt::Key_PageUp:
		case Qt::Key_PageDown:
			{
				if(!(m_curState.keys & KS_CTRL) && !(m_curState.keys & KS_SHIFT))
				{
					SetRenderKey(key, true);
				}
				break;
			}

		//case Qt::Key_N:
		//	if (m_curState.keys == 0)
		//		g_app->ResetView();
		//	break;

		//case Qt::Key_U:
		//	if (m_curState.keys == 0)
		//		g_app->ResetTilt();
		//	break;

		//case Qt::Key_R:
		//	if (m_curState.keys == 0)
		//		g_app->ResetAll();
		//	break;

		case Qt::Key_Space:
			m_curState.keys |= KS_SPACE;
			pCmdMgr->OnKeyPressEvent(keyState);
			break;

#ifdef DEBUG
		case Qt::Key_F1	:
			g_app->GetConfig()->m_isShowTerrainWireframe=!(g_app->GetConfig()->m_isShowTerrainWireframe);
			g_app->GetConfig()->m_isShowTerrainInner=false;
			break;
		case Qt::Key_F2	:
			g_app->GetConfig()->m_isShowRenderTileName=!(g_app->GetConfig()->m_isShowRenderTileName);
			break;
		case Qt::Key_F3	:
			g_app->GetConfig()->m_isShowRenderTileBoundry=!(g_app->GetConfig()->m_isShowRenderTileBoundry);
			break;
		case Qt::Key_F4:
			g_app->GetConfig()->m_isShowShapeNodes=!(g_app->GetConfig()->m_isShowShapeNodes);
			break;
		case Qt::Key_F5:
			g_app->GetConfig()->m_isShowTerrainInner=!(g_app->GetConfig()->m_isShowTerrainInner);
			break;
		case Qt::Key_F6:
			g_app->GetConfig()->m_isRendering=!(g_app->GetConfig()->m_isRendering);
			break;
		
#endif
		default:
			pCmdMgr->OnKeyPressEvent(keyState);
			break;
	}
}

void RenderFrame::keyReleaseEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
		return;

	int key = event->key();
	switch(key)
	{
	case Qt::Key_Underscore:
		{
			key = Qt::Key_Minus;
			break;
		}
	case Qt::Key_Equal:
		{
			key = Qt::Key_Plus;
			break;
		}
	}
	switch (key)
	{
		case Qt::Key_Control:
			m_curState.keys &= ~KS_CTRL;
			break;

		case Qt::Key_Shift:
			m_curState.keys &= ~KS_SHIFT;
			break;

		case Qt::Key_Alt:
			m_curState.keys &= ~KS_ALT;
			break;
		case Qt::Key_Space:
			m_curState.keys &= ~KS_SPACE;
			break;
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_A:
		case Qt::Key_D:
		case Qt::Key_W:
		case Qt::Key_S:
			SetRenderKey(key, false);
			break;
	}
	// Rlease +/- key when any key was released
	SetRenderKey(Qt::Key_PageUp , false);
	SetRenderKey(Qt::Key_PageDown , false);
	SetRenderKey(Qt::Key_Plus , false);
	SetRenderKey(Qt::Key_Minus , false);
}

static void MakeCursorState(QMouseEvent *event, CursorState *curState)
{
	curState->x = event->pos().x();
	curState->y = event->pos().y();
	curState->buttons = event->buttons();
}

void RenderFrame::mouseMoveEvent(QMouseEvent *event)
{
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	MakeCursorState(event, &m_curState);
	if (m_navBar && m_navBar->mouseMoveEvent(event))
		return;
	if (m_tourPlayBar && m_tourPlayBar->mouseMoveEvent(&m_curState))
		return;
	if (m_flightSimCtrl && m_flightSimCtrl->mouseMoveEvent(&m_curState))
		return;
	if (m_carSimCtrl && m_carSimCtrl->mouseMoveEvent(&m_curState))
		return;
	pCmdMgr->OnMouseMove(&m_curState);
	CURSOR_SHAPE cursorShape = pCmdMgr->GetCursorShape();
	m_renderCursor->SetCursor(cursorShape);
}

void RenderFrame::mousePressEvent(QMouseEvent *event)
{
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	MakeCursorState(event, &m_curState);
	if (m_navBar && m_navBar->mousePressEvent(event))
		return;
	if (m_tourPlayBar && m_tourPlayBar->mousePressEvent(&m_curState))
		return;
	if (m_flightSimCtrl && m_flightSimCtrl->mousePressEvent(&m_curState))
		return;
	if (m_carSimCtrl && m_carSimCtrl->mousePressEvent(&m_curState))
		return;
	pCmdMgr->OnMousePress(&m_curState);
	CURSOR_SHAPE cursorShape = pCmdMgr->GetCursorShape();
	m_renderCursor->SetCursor(cursorShape, &m_curState);
	
	g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pGeoMapNode->mousePressEvent(&m_curState);//2013 2 1 mapscenenode
}

void RenderFrame::mouseReleaseEvent(QMouseEvent *event)
{
	CursorState org = m_curState;
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	if (m_navBar && m_navBar->mouseReleaseEvent(event))
	    return;
	if (m_tourPlayBar && m_tourPlayBar->mouseReleaseEvent(&m_curState))
		return;
	MakeCursorState(event, &m_curState);
	CheckClicked(&org, &m_curState);
	pCmdMgr->OnMouseRelease(&m_curState);
	CURSOR_SHAPE cursorShape = pCmdMgr->GetCursorShape();
	m_renderCursor->SetCursor(cursorShape);
}

void RenderFrame::mouseDoubleClickEvent(QMouseEvent *event)
{
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	MakeCursorState(event, &m_curState);
	if (m_navBar && m_navBar->mousePressEvent(event))
		return;
	m_curState.buttons |= MBS_DCLICKED;
	pCmdMgr->OnMouseDblClick(&m_curState);
}

void RenderFrame::wheelEvent(QWheelEvent *event)
{
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	m_curState.delta = event->delta();
	m_curState.x = event->pos().x();
	m_curState.y = event->pos().y();
	pCmdMgr->OnMouseWheelEvent(&m_curState);

}

void RenderFrame::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_moveTimer.timerId())
		 CheckRenderKey(false);

	/*********************************************
	2013 3 7 ugi
	logoÅé ÆÉÎ§Ê«»Ì Í±Äñ °ÇÎ¨ÎÌ ³¿ÒþÓ· Êì²úÌ­Æ¹Óª É¤ÍþÍÖÆíÄû³¿.
	*********************************************/
	/*if (event->timerId() == m_logoTimer.timerId())
		UpdateLogo();*/

	update();
}

void RenderFrame::dragEnterEvent(QDragEnterEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasUrls())
		event->acceptProposedAction();
}

void RenderFrame::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	if (!mimeData->hasUrls())
		event->ignore();


	QList<QUrl> urlList = mimeData->urls();
	QString url = urlList.at(0).path();
	QString first;
	int count = url.count();
	if (count < 1)
		return;

#ifdef Q_WS_WIN
	first = url.left(1);
	if (first == QString(URL_MARK))
		url = url.right(count - 1);
#endif
	emit dropfile(url);
	event->acceptProposedAction();
}

void RenderFrame::mouseClickEventSlot()
{
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	m_curState.buttons |= MBS_CLICKED;
	if (m_navBar && m_navBar->MouseClickEvent(&m_curState))
		return;
	pCmdMgr->OnMouseClickEvent(&m_curState);

	m_placeMarkMgr && m_placeMarkMgr->MouseClickEvent(&m_curState);
}

void RenderFrame::DrawItem(QPainter *painter)
{
	if (m_renderCursor)
		m_renderCursor->PaintEvent(painter);

	/*if (m_logoDisp)
		m_logoDisp->Paint(painter);*/

	if (m_navBar)
		m_navBar->Paint(painter);

	if (m_tourPlayBar)
		m_tourPlayBar->Paint(painter);

	if (m_flightSimCtrl)
		m_flightSimCtrl->Paint(painter);

	if (m_carSimCtrl)
		m_carSimCtrl->Paint(painter);

}

void RenderFrame::CreateTourPlayBar()
{
	m_tourPlayBar = new TourPlayBar();
	if (m_tourPlayBar)
		m_tourPlayBar->Init();
}

void RenderFrame::DestroyTourPlayBar()
{
	if (m_tourPlayBar)
	{
		m_tourPlayBar->Release();
		delete m_tourPlayBar;
		m_tourPlayBar = NULL;
	}
}

void RenderFrame::CreateFlightSimCtrl()
{
	m_flightSimCtrl = new FlightSimCtrl();
}

void RenderFrame::DestroyFlightSimCtrl()
{
	if (m_flightSimCtrl)
	{
		delete m_flightSimCtrl;
		m_flightSimCtrl = NULL;
	}
}

void RenderFrame::CreateCarSimCtrl()
{
	m_carSimCtrl = new CarSimCtrl();
}

void RenderFrame::DestroyCarSimCtrl()
{
	if (m_carSimCtrl)
	{
		delete m_carSimCtrl;
		m_carSimCtrl = NULL;
	}
}

void RenderFrame::UpdateLogo()
{
	update();
}

static bool IsEqualPoint(const CursorState *org, const CursorState *cur)
{
	int dx = abs(org->x - cur->x);
	int dy = abs(org->x - cur->x);

	if (dx < 3 && dy < 3)
		return true;
	else
		return false;
}

#define CLICK_DELTA	350
void RenderFrame::CheckClicked(const CursorState *org, const CursorState *cur)
{
	static int delta = -1;
	static QTime clickTime;

	if (IsEqualPoint(cur, org))
	{
		if (delta != -1)
			delta = clickTime.elapsed();

		clickTime.start();

		if (delta == -1)
			delta = CLICK_DELTA;

		if (delta < CLICK_DELTA && IsEqualPoint(&m_clickCur, cur))
		{
			delta = -1;
		}
		else
		{
			m_curState.buttons = org->buttons;
			m_clickCur = *cur;
			emit clicked();
		}
	}
}

#define KEY_TABLE_LEN	4
typedef struct _KeyTable
{
	unsigned int key;
	unsigned int flag;
	unsigned int groupKey;
	unsigned int count;
} KeyTable;

static KeyTable s_keyFlags[] =
{//		key				flag	groupKey		count
	{Qt::Key_Left,		0x0001,	Qt::Key_Left,	2},
	{Qt::Key_Right,		0x0002, Qt::Key_Left,	2},

	{Qt::Key_Up,		0x0004,	Qt::Key_Up,		2},
	{Qt::Key_Down,		0x0008,	Qt::Key_Up,		2},

	{Qt::Key_PageUp,	0x0010,	Qt::Key_PageUp,	5},
	{Qt::Key_Plus,		0x0040,	Qt::Key_PageUp,	5},
	{Qt::Key_PageDown,	0x0080,	Qt::Key_PageUp,	5},
	{Qt::Key_Minus,		0x0100,	Qt::Key_PageUp,	5},
	{Qt::Key_Equal,		0x0200, Qt::Key_PageUp, 5},

	{Qt::Key_A,			0x0400,	Qt::Key_A,		2},
	{Qt::Key_D,			0x0800,	Qt::Key_A,		2},

	{Qt::Key_W,			0x1000,	Qt::Key_W,		2},
	{Qt::Key_S,			0x2000,	Qt::Key_W,		2},

	0
};

static int FindKeyTable(unsigned int key)
{
	for (int i = 0; s_keyFlags[i].key != 0; ++i)
	{
		if (s_keyFlags[i].key == key)
			return i;
	}
	return -1;
}

void RenderFrame::ResetRenderKey()
{
	m_renderKeys = 0;
}

void RenderFrame::SetRenderKey(unsigned int key, bool enable)
{
	if (key == 0)
		return;

	int index = FindKeyTable(key);
	if (index < 0)
		return;

	unsigned int flag = s_keyFlags[index].flag;

	if (enable)
	{
		if (m_renderKeys & flag)
			return;
	}
	else
	{
		m_renderKeys &= ~flag;
		return;
	}

	int groupIndex = FindKeyTable(s_keyFlags[index].groupKey);
	if (groupIndex < 0)
		return;
	int groupCount = s_keyFlags[groupIndex].count;

	//remove other flag
	int i;
	for (i = groupIndex; i < groupIndex + groupCount; ++i)
		m_renderKeys &= ~s_keyFlags[i].flag;

	m_renderKeys |= flag;
	CheckRenderKey(true);
}

void RenderFrame::CheckRenderKey(bool force)
{
	int moveX = 0;
	int moveY = 0;
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	KeyState keyState;
	memset(&keyState , 0 , sizeof(KeyState));
	keyState.keyFlags = m_curState.keys;

	for (int i = 0; s_keyFlags[i].key != 0; ++i)
	{
		if (!(m_renderKeys & s_keyFlags[i].flag))
			continue;
		keyState.key1 = s_keyFlags[i].key;
		switch (s_keyFlags[i].key)
		{
		case Qt::Key_Left:
			if ((m_curState.keys & KS_SHIFT) || (m_curState.keys & KS_CTRL))
				pCmdMgr->OnKeyPressEvent(keyState);
			else{
				moveX = -1;
			}
			break;

		case Qt::Key_Right:
			if ((m_curState.keys & KS_SHIFT) || (m_curState.keys & KS_CTRL))
				pCmdMgr->OnKeyPressEvent(keyState);
				//g_app->RotateView(-1);
			else
				moveX = 1;
			break;

		case Qt::Key_Up:
			if ((m_curState.keys & KS_SHIFT) || (m_curState.keys & KS_CTRL))
				pCmdMgr->OnKeyPressEvent(keyState);
				//g_app->TiltView(-1);
			else
				moveY = 1;
			break;

		case Qt::Key_Down:
			if ((m_curState.keys & KS_SHIFT) || (m_curState.keys & KS_CTRL))
				pCmdMgr->OnKeyPressEvent(keyState);
				//g_app->TiltView(1);
			else
				moveY = -1;
			break;
		case Qt::Key_A:
		case Qt::Key_D:
		case Qt::Key_W:
		case Qt::Key_S:
		case Qt::Key_PageUp:
		case Qt::Key_Plus:
		case Qt::Key_PageDown:
		case Qt::Key_Minus:
			pCmdMgr->OnKeyPressEvent(keyState);
			break;
		}
	}

	if (moveX != 0 || moveY != 0)
	{
		keyState.bMoveKey = true;
		if(moveX)
			keyState.key1 = moveX > 0 ? Qt::Key_Right : Qt::Key_Left;
		if(moveY)
			keyState.key2 = moveY > 0 ? Qt::Key_Up : Qt::Key_Down;
		pCmdMgr->OnKeyPressEvent(keyState);
		//g_app->MoveView(moveX, moveY);
	}
}



