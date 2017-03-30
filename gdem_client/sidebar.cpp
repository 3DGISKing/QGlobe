#include <QtGui/QSplitter>
#include "gdemclient.h"
#include "sidebar.h"
#include "searchdlg.h"
#include "placedlg.h"
#include "mainwindow.h"
#include "resulttreeview.h"
#include "placemarkdlg.h"
#include "layersdlg.h"
#include "config.h"
#include "treeitem.h"

#include "GIS_Node.h"

#define SIDEBAR_MIN_WIDTH 250
#define LEFT_MARGIN		10
#define TOP_MARGIN		10
#define RIGHT_MARGIN	10

#define SIDEBAR_STYLE "QMainWindow{background-color:#7495DC;}"
#define SPLITTER_STYLE "QSplitter::handle{background-color:#7495DC;} QSplitter::handle:vertical{height:5px;}"

enum SidebarState {SS_NONE, SS_Search, SS_Place, SS_SearchPlace, SS_Layer, SS_SearchLayer,
				   SS_PlaceLayer, SS_All};
enum AcitiveView {AV_None, AV_SearchView, AV_PlaceView};

static StatePos s_statePos[] =
{
	//	state		Sender		Search 	  Place	  Layter
	{SS_Search,			-1,			0,		1,		1	},
	{SS_Place,			-1,			1,		0,		1	},
	{SS_SearchPlace,	ST_Search,	1,		0,		1	},
	{SS_SearchLayer,	ST_Layer,	0,		1,		1	},
	{SS_PlaceLayer,		ST_Search,	1,		0,		1	},
	{SS_PlaceLayer,		ST_Place,	1,		1,		0	},
	{SS_PlaceLayer,		ST_Layer,	1,		0,		1	},
	{SS_SearchPlace,	ST_Place,	0,		1,		1	},
	{SS_SearchPlace,	ST_Layer,	1,		0,		1	},
	{SS_Layer,			-1,			1,		1,		0	},
	{SS_SearchLayer,	ST_Search,	1,		1,		0	},
	{SS_SearchLayer,	ST_Place,	0,		1,		1	},

	{SS_All,			ST_Search,	1,		0,		1	},
	{SS_All,			ST_Place,	0,		1,		1	},
	{SS_All,			ST_Layer,	1,		0,		1	},
	-1
};

static PlaceMarkMap s_placemarkData ;

/*
 *   public
 */
SideBar::SideBar()
{
	m_searchDlg=NULL;
	m_layersDlg=NULL;
	m_placeDlg=NULL;
}

SideBar::~SideBar()
{	
}

bool SideBar::Init()
{
	m_activeView = AV_None;

	if (!CreateFrame())
		return false;
	setContentsMargins(LEFT_MARGIN,TOP_MARGIN,RIGHT_MARGIN,BOTTOM_MARGIN);
	setMinimumWidth(SIDEBAR_MIN_WIDTH);
	return true;


	int height1 = g_app->GetConfig()->m_posSearch;
	int height2 = g_app->GetConfig()->m_posPlace;
	int height3 = g_app->GetConfig()->m_posLayers;
	
	//setStyleSheet(SIDEBAR_STYLE);
	//m_splitter->setStyleSheet(SPLITTER_STYLE);
	return true;
}

void SideBar::Release()
{
	g_app->GetConfig()->m_sideBarWidth = width();

	if(m_searchDlg)
		g_app->GetConfig()->m_statusSearch = m_searchDlg->GetStatus();

	if(m_placeDlg)
	{
		g_app->GetConfig()->m_statusPlace = m_placeDlg->GetStatus();
		g_app->GetConfig()->m_heightPlace = m_placeDlg->GetRestoreHeight();
	}
	
	if(m_searchDlg)
		g_app->GetConfig()->m_heightSearch = m_searchDlg->GetRestoreHeight();
	
	DestroyFrame();
	s_placemarkData.clear();
}

void SideBar::MoveToMyPlace()
{
	if (!IsMoveToMyPlace())
		return;
	if (GetActiveView()->GetSelectItem() == m_placeDlg->GetPlaceView()->GetTempPlaceItem())
	{
		m_placeDlg->GetPlaceView()->TempToMyPlaces();
		return;
	}
	GetActiveView()->CutItem();
	m_placeDlg->GetPlaceView()->MoveMyPlaces();
}

bool SideBar::IsMoveToMyPlace()
{
	TreeItem *selectItem = GetActiveView()->GetSelectItem();
	//TreeItem *myplaceItem = m_placeDlg->GetPlaceView()->GetMyPlaceItem();
	//if (selectItem && myplaceItem && selectItem != myplaceItem)
	//	return true;
	if (!selectItem)
		return false;

	TreeItem *tempItem = m_placeDlg->GetPlaceView()->GetTempPlaceItem();

	if (tempItem->hasChildren())
	{
		int row = selectItem->index().row();
		TreeItem *child = (TreeItem*) tempItem->child(row);
		
		if (selectItem && child && child == selectItem)
			return true;
	}

	return false;
}

bool SideBar::IsSaveAsPlaceMarkFile()
{
	if (GetActiveView()->GetSelectItem())
		return true;

	return false;
}

void SideBar::MoveStartPosition()
{
	m_placeDlg->GetPlaceView()->MoveStartPosition();
}

void SideBar::SetSelectItem(const PLACEMARK_DATA *data, bool focus)
{
	if (!data)
	{
		m_activeView = AV_None;
		return;
	}

	int count = sizeof(unsigned int) * 8 - 1;
	unsigned int prefix = (0x1 << count);
	if (data->key < prefix)
	{
		m_searchDlg->GetSearchView()->SetSelectItem(data->item);
		m_activeView = AV_SearchView;

		if (focus)
			m_searchDlg->GetSearchView()->setFocus(Qt::OtherFocusReason);
	}
	else
	{
		m_placeDlg->GetPlaceView()->SetSelectItem(data->item);
		m_activeView = AV_PlaceView;

		if (focus)
			m_placeDlg->GetPlaceView()->setFocus(Qt::OtherFocusReason);
	}
}

PlaceMarkMap* SideBar::GetPlaceMarkMap()
{
	return &s_placemarkData;
}

ResultTreeView* SideBar::GetActiveView()
{
	if (IsActiveSearchView())
		return m_searchDlg->GetSearchView();

	return m_placeDlg->GetPlaceView();
}

void SideBar::ChangePanel(int sender)
{
	
}

void SideBar::AddPlaceMark(const PLACEMARK_DATA &data)
{
	int removecount = 0;
	QString str = QString::fromUtf16(data.name);
	if (data.showmark == SMT_HIDE)
	{
		removecount = s_placemarkData.remove(data.key);
		if ( removecount > 0 )
#ifdef DEBUG
		qDebug() << "remove key = " << data.key << ", name = " << str << ", count= " << removecount;
#endif
		return;
	}
	s_placemarkData.insert(data.key, data);

#ifdef DEBUG
	qDebug() << "add key = " << data.key << ", name = " << str;
#endif
}

void SideBar::RemovePlaceMark(const PLACEMARK_DATA &data)
{
	QString str = QString::fromUtf16(data.name);
	//PlaceMarkMap::iterator i = s_placemarkData.find(data.key);
	//s_placemarkData.erase(i);
	int removecount = s_placemarkData.remove(data.key);
#ifdef DEBUG
	qDebug() << "remove key = " << data.key << ", name = " << str << ", count= " << removecount;;
#endif
}

void SideBar::ModifyPlaceMark(const PLACEMARK_DATA &data)
{
	if (data.showmark == SMT_HIDE)
		return;

	QString str = QString::fromUtf16(data.name);
	if (!s_placemarkData.contains(data.key))
	{
#ifdef DEBUG
		qDebug() << "modify key is Empty "  << ", name = " << str;
#endif
		return;
	}

	s_placemarkData.insert(data.key, data);
#ifdef DEBUG
	qDebug() << "modify key = " << data.key << ", name = " << str;
#endif
}

void SideBar::ClearSearch()
{
	PLACEMARK_DATA val;
	int count = sizeof(unsigned int) * 8 - 1;
	unsigned int prefix = (0x1 << count);

	PlaceMarkMap::iterator i = s_placemarkData.begin();
	PlaceMarkMap::iterator temp;
	while (i != s_placemarkData.end())
	{
		val = i.value();
		if (val.key < prefix)
		{
			temp = i + 1;
			s_placemarkData.erase(i);
			i = temp;
		}
		else
			++i;
	}
}

/*
 *   private
 */

bool SideBar::CreateFrame()
{
    m_searchDlg = new SearchDlg(this);
	if (!m_searchDlg)
		return false;
	if (!m_searchDlg->Init())
		return false;
	m_searchDlg->SetStatus(g_app->GetConfig()->m_statusSearch);
	m_searchDlg->SetRestoreHeight(g_app->GetConfig()->m_heightSearch);

    m_placeDlg = new PlaceDlg(this);
	if (!m_placeDlg)
		return false;
	if (!m_placeDlg->Init())
		return false;
	m_placeDlg->SetStatus(g_app->GetConfig()->m_statusPlace);
	m_placeDlg->SetRestoreHeight(g_app->GetConfig()->m_heightPlace);

	m_layersDlg = new LayersDlg(this);
	if (!m_layersDlg)
		return false;
	if (!m_layersDlg->Init())
		return false;

	QString iconName = g_app->GetResource("/sidebar/database.png");
	addTab(m_layersDlg,QIcon(iconName),tr("Database"));

	iconName=g_app->GetResource("/sidebar/earth.png");
	addTab(m_placeDlg,QIcon(iconName),tr("My Place"));

	iconName=g_app->GetResource("/sidebar/search.png");
	addTab(m_searchDlg,QIcon(iconName),tr("Search"));

	setTabPosition(QTabWidget::South);
	UpdateToolTip();

	return true;
}

void SideBar::DestroyFrame()
{
	if (m_searchDlg)
	{
		m_searchDlg->Release();
		delete m_searchDlg;
		m_searchDlg = NULL;
	}
	if (m_placeDlg)
	{
		m_placeDlg->Release();
		delete m_placeDlg;
		m_placeDlg = NULL;
	}
	if (m_layersDlg)
	{
		m_layersDlg->Release();
		delete m_layersDlg;
		m_layersDlg = NULL;
	}
}

bool SideBar::IsActiveSearchView()
{
	switch (m_activeView)
	{
	case AV_None:
		return m_searchDlg->IsActiveView();
	case AV_SearchView:
		return true;
	case AV_PlaceView:
		return false;
	}
	return false;
}

void SideBar::UpdateToolTip()
{
	if (m_searchDlg && m_placeDlg && m_layersDlg)
	{
		m_searchDlg->UpdateToolTip();
		m_placeDlg->UpdateToolTip();
	}
}

StatePos* SideBar::GetStatePos(int state, int sender)
{
	if (state < 0)
		return NULL;
	StatePos *sr = NULL;

	for (int i=0; s_statePos[i].state>=0; i++)
	{
		if (s_statePos[i].state == state && s_statePos[i].sender == sender)
			sr = &s_statePos[i];
	}
	return sr;
}

void SideBar::setSearchRegion(CGIS_Node *region)
{
	setCurrentIndex(2);
	m_searchDlg->setSearchStyle(1);
	m_searchDlg->setSearchRegion(region);
	m_searchDlg->searchRangeData();
}

void SideBar::setSearchStyle(int i)
{
	m_searchDlg->setSearchStyle(i);
}




