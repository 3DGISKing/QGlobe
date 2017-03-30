#include <QtGui>
#include "gdemclient.h"
#include "placedlg.h"
#include "resulttreeview.h"
#include "sidebar.h"
#include "guiuty.h"
#include "msgdata.h"
#include "config.h"
#include "ui_placedlg.h"

#define TREE_VIEW_STYLE "QTreeView{border: 0px }"
#define DEFAULT_KMLFILE	"/myplace.kml"

PlaceDlg::PlaceDlg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::PlaceDlg)
{
    m_ui->setupUi(this);
    m_parent = (SideBar*)parent;	
}

PlaceDlg::~PlaceDlg()
{
    delete m_ui;
}

bool PlaceDlg::Init()
{
	setContentsMargins(0,0,0,BOTTOM_MARGIN);
	m_visible = true;

	if (!CreateFrame())
		return false;

	SetStatus(BS_Open);
	m_oldsize = this->size().height();

	return true;
}

void PlaceDlg::Release()
{
	DestroyFrame();
}
bool PlaceDlg::IsActiveView()
{
	return m_placeView->hasFocus();
}

void PlaceDlg::SetStatus(int status)
{
    m_status = status;

    if (m_status == BS_Close)
    {
        m_visible = false;
        m_frame->hide();
		setContentsMargins(0,0,0,0);
    }
    else if (m_status != BS_Animation)
    {
        m_visible = true;
        m_frame->show();
		setContentsMargins(0,0,0,BOTTOM_MARGIN);
    }

  	UpdateToolTip();
}

int PlaceDlg::GetHeight(int sender)
{
	int height = m_oldsize;
    if (!m_visible)
    {
		m_oldsize = this->size().height();
    }
	else if (sender != ST_Place)
		height = this->size().height();
    return height;
}

bool PlaceDlg::TempToMyPlaces()
{
	if (!m_placeView->IsTempToMyPlaces())
		return true;

	int result = gu_MessageBox(this, tr("GDEM Client"), tr("You have unsaved items in your \"Temporary Places\" folder. Would you like to save them to your \"My Places\" folder?"),
						  MSGBOX_ICONQUESTION|MSGBOX_IDYES|MSGBOX_IDCANCEL|MSGBOX_IDNO);

	if (result == MSGBOX_IDCANCEL)
		return false;
	if (result == MSGBOX_IDYES)
		m_placeView->TempToMyPlaces();
	return true;
}

void PlaceDlg::SaveMyPlaces()
{
	m_placeView->SaveMyPlaces();
}

void PlaceDlg::SaveStartPosition()
{
	m_placeView->SaveStartPosition();
}

void PlaceDlg::UpdateToolTip()
{
	if (g_app->GetConfig()->m_toolTipChecked)
	{
		
	}
	else
	{
		
	}
}

/*
 *	protected
 */
void PlaceDlg::keyPressEvent ( QKeyEvent * event )
{
	if (event->key() == Qt::Key_Escape)
		event->ignore();
	else
		event->accept();
}

bool PlaceDlg::CreateFrame()
{
	QString path = g_app->GetResource("/sidebar/");

	QVBoxLayout *frameLayout = new QVBoxLayout;
	if (!frameLayout)
		return false;

	m_frame = new QFrame(this);
	if (!m_frame)
		return false;

	m_placeView = new ResultTreeView(VT_MyPlace);
	if (!m_placeView)
		return false;

	m_placeView->SetSideBar(m_parent);
	frameLayout->addWidget(m_placeView);
	//m_placeView->setStyleSheet(TREE_VIEW_STYLE);
	frameLayout->setContentsMargins(5,0,5,0);
	m_frame->setLayout(frameLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	if (!mainLayout)
		return false;

	mainLayout->addWidget(m_frame);
	mainLayout->setContentsMargins(0,BOTTOM_MARGIN,0,BOTTOM_MARGIN);
	setLayout(mainLayout);

	if (!m_placeView->InitMyPlace())
		return false;

	return true;
}

void PlaceDlg::DestroyFrame()
{
	if (m_placeView)
	{
		delete m_placeView;
		m_placeView = NULL;
	}

	if (m_frame)
	{
		delete m_frame;
		m_frame = NULL;
	}
}

/*
 *	protected slot
 */

int PlaceDlg::ChangeState(bool press)
{
   bool pressed = press;
   if (m_status == BS_Disable)
       return 0;

   if (pressed == true)
   {
       SetStatus(BS_Animation);
   }
   else
   {
       if (m_visible)
       {
           SetStatus(BS_Close);
		   m_parent->ChangePanel(ST_Place);
       }
       else
       {
           SetStatus(BS_Open);
		   m_parent->ChangePanel(ST_Place);
       }
   }

   return 1;
}


void PlaceDlg::DeleteItem(bool press)
{
	QString path = g_app->GetResource("/sidebar/");

	if (press)
	{
	
	}
	else
	{
		ResultTreeView *treeView = m_parent->GetActiveView();

		if (treeView && treeView->IsDeleteItem() && m_placeView->hasFocus())
		{
			treeView->DeleteItem();
		}
	}
}
