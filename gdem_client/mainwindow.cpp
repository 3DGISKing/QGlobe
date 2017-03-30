#include <QHBoxLayout>
#include <QSplitter>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QtCore/QDir>

#include "gdemclient.h"
#include "config.h"
#include "mainwindow.h"
#include "sidebar.h"
#include "placedlg.h"
#include "searchdlg.h"
#include "layersdlg.h"
#include "sidebar.h"
#include "renderfrm.h"
#include "srvconfdlg.h"
#include "navbar.h"
#include "placemarkdlg.h"
#include "rulerdlg.h"
#include "aboutdlg.h"
#include "proxy.h"
#include "msgdata.h"
#include "filedialog.h"
#include "resulttreeview.h"
#include "guiuty.h"
#include "placemarkmgr.h"
#include "placefolderdlg.h"
#include "pathdlg.h"
#include "polygondlg.h"
#include "optiondlg.h"
#include "contourdlg.h"
#include "printdialog.h"
#include "ui_mainwindow.h"
#include "../gdem_common/gdem_config.h"
#include "GIS_Path.h"
#include "GIS_Doc.h"
#include "gdemcustomstyle.h"

#ifdef STANDARD_ALONE
#include "dbpathsetting.h"
#include "../gds_common/GDSConfigMgr.h"
#include "objectadddlg.h"

//added by RSH 2013.3.16
#include "shapecolorpick.h"
//end addition

//2013 7 28 ugi
#include "buildingpropdlg.h"
#include "peopleinfodlg.h" //asd 2014.4.1
#include "objectplaceeditdlg.h" //asd 2014.4.1
#include "objectplacesettingdlg.h"
#endif
#include "videowebview.h" //2014 2 6 by ugi
#include "animationdlg.h"
#include "animationmgr.h"

#include "measureareadlg.h"
#include "../gdem_libmain/GDM_Interface.h"
#include "../gds_core/GDSCoreInterface.h"
#include "gotodialog.h"

#define TB_ICON_WIDTH          16 // ToolBar Icon Width
#define TB_ICON_HEIGHT         16 // ToolBar Icon Height
#define TB_BACKGROUND	       0xD5EAFF

#define MARGIN_SIZE            2  //2cm (for Printing)

#define MAINWINDOW_ICON_NAME   "/geobrowser.png"
#define HELP_FILENAME		   "doc/GeoBrowser User Manual.pdf"

extern GDM_RENDER_HANDLE	g_pRender;

typedef struct _ViewSize {
	int w;
	int h;
	int fxHeight;
} ViewSize;

static ViewSize s_viewSize[] =
{
	{640, 480, -1},		// QVGA
	{640, 480, -1},		// VGA
	{800, 600, -1},		// SVGA
	{720, 486, -1},		// NTSC
	{720, 576, -1},		// PAL
	{864, 486, -1},		// NTSC 16:9
	{1024, 576, -1},	// PAL 16:9
	{1920, 1080, -1},	// 1080p HDTV
	{1280, 720, -1},	// 720p HDTV
	{852, 480, -1},		// DVD
	{110, 85, 600},		// Letter
	{17, 11, 600},		// Tabloid
	{7, 5, 480},		// PostCard1
	{6, 4, 480}			// PostCard2
};

enum ViewMode {VM_QVGA, VM_VGA, VM_SVGA, VM_NTSC, VM_PAL, VM_WIDE_NTSC, VM_WIDE_PAL, VM_1080P_HDTV,
			   VM_720P_HDTV, VM_480P_DVD, VM_LETTER, VM_TABLOID, VM_POSTCARD1, VM_POSTCARD2 };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

	m_splitter = NULL;
	m_sideBar = NULL;
	m_renderFrame = NULL;
	m_searchToolBar = NULL;
	m_standardToolBar=NULL;
	m_searchCombo = NULL;
	m_simulationToolBar = NULL;
	m_rulerDlg = NULL;
	m_buildingPropDlg = NULL;
	m_objectaddDlg = NULL;
	m_peopleInfoDlg = NULL; //asd add 2014 2 13
	m_measureAreaDlg=NULL;
	m_objectplaceEditDlg = NULL;
	m_goToDialog=NULL;
#ifdef DEBUG
	m_statusbBarLeftLabel=NULL;
#endif
	m_statusbBarCenterLabel=NULL;
	m_statusbBarRightLabel=NULL;
	m_videoWebView=NULL;
	m_animationDlg=NULL;

}

MainWindow::~MainWindow()
{
    delete m_ui;
	Release();
}

bool MainWindow::Init()
{
	QString workDir = g_app->GetWorkDir();

	setWindowFilePath(workDir);

	setWindowTitle(tr("GDEM Client"));
	SetIcon();
	
#ifdef STANDARD_ALONE
	QString serverDataPath = QDir::currentPath();
#ifdef MAC_DEBUG_APP_PATH
	serverDataPath += QString("/../../../");
	QDir dir(serverDataPath);
	serverDataPath = dir.absolutePath();
#endif
	InitializeGDSCore(serverDataPath);
#endif
	if (!g_app->InitProxy())
		return false;
	if (!CreateFrame())
		return false;

	CreateMenus();
	SetupStatusBar();

	m_rulerDlg = new RulerDlg(this);
	if (m_rulerDlg == NULL)
		return false;
	
	m_measureAreaDlg = new MeasureAreaDlg(this);
	if (m_measureAreaDlg == NULL)
		return false;

	m_buildingPropDlg=new BuildingPropDlg(this);
	if(m_buildingPropDlg==NULL)
		return false;

	m_videoWebView=new VideoWebView(this);
	if(m_videoWebView==NULL)
		return false;
	
	m_animationDlg=new AnimationDlg(this);
	if(m_animationDlg==NULL)
		return false;

	m_goToDialog=new GoToDialog(this);
	if(m_goToDialog==NULL)
		return false;

	// open file
	m_sideBar->m_placeDlg->GetPlaceView()->OpenMyPlaces();

#ifndef STANDARD_ALONE
	Config *conf = g_app->GetConfig();
	if (!conf->IsServer())
		SetServer();
#endif



	return true;
}

void MainWindow::Release()
{
	hide();	
	
	if(m_sideBar && m_sideBar->m_searchDlg)
		m_sideBar->m_searchDlg->WaiteStop();

	if (m_rulerDlg)
	{
		delete m_rulerDlg;
		m_rulerDlg = NULL;
	}

	if(m_measureAreaDlg)
	{
		delete m_measureAreaDlg;
		m_measureAreaDlg=NULL;
	}

	if(m_buildingPropDlg)
	{
		delete m_buildingPropDlg;
		m_buildingPropDlg=NULL;
	}

	if(m_videoWebView)
	{
		delete m_videoWebView;
		m_videoWebView=NULL;
	}

	if(m_animationDlg)
	{
		delete m_animationDlg;
		m_animationDlg=NULL;
	}

	if(m_goToDialog)
	{
		delete m_goToDialog;
		m_goToDialog=NULL;
	}

	if(m_objectaddDlg)
	{
		delete m_objectaddDlg;
		m_objectaddDlg=NULL;
	}
	
	DestroyFrame();
	
	if (g_app)
	{
		g_app->Release();
		g_app=NULL;
	}

	ReleaseGDSCore();
}

void MainWindow::InitUpdate()
{
	UpdateContourState();
	UpdateMenus();
	UpdateToolBars();
	Reposition();
}

void MainWindow::AfterUpdate()
{
	RewidthSideBar();
}

void MainWindow::UpdateContourState()
{
	Config *conf = g_app->GetConfig();

	if (conf->m_contourInterval < CONTOUR_INTERVAL_MIN ||
		conf->m_contourInterval > CONTOUR_INTERVAL_MAX)
		conf->m_contourInterval = 500;

	ContourStateInfo cti;
	memset(&cti, 0, sizeof(cti));
	cti.contourChecked = conf->m_blContour;
	cti.contourInterval = conf->m_contourInterval;
	cti.ravineColor = conf->m_ravineColor;
	cti.mainCurveColor = conf->m_mainCurveColor;
	cti.ravineThick = conf->m_ravineThick;
	cti.mainCurveThick = conf->m_mainCurveThick;

	g_app->GetRenderProxy()->SendMessage(PM_CONTOUR_STATE, (PrxParam)&cti);
}

void MainWindow::UpdateLayer()
{
	unsigned int topoState = g_app->GetConfig()->m_layerTopography;

	m_ui->actionTerrain->setChecked(g_app->GetConfig()->m_blTerrain);

	m_ui->actionContour->setChecked(g_app->GetConfig()->m_blContour);

	m_ui->actionRenderTexture->setChecked(topoState == LTM_TEXTURE);
	m_ui->actionRenderAltitude->setChecked(topoState == LTM_HEIGHT);
	m_ui->actionRenderGradient->setChecked(topoState == LTM_RATEANGLE);
	m_ui->actionRenderIncline->setChecked(topoState == LTM_RATEPLAN);

	m_sideBar->m_layersDlg->UpdateDialog();
}

void MainWindow::UpdateRuler()
{
	Config *conf = g_app->GetConfig();

	if (conf->m_showRuler)
	{
		m_ui->actionRuler->setChecked(true);
		m_rulerDlg->OnCheckCommand();
		m_rulerDlg->show();
	}
	else
	{
		m_ui->actionRuler->setChecked(false);
		m_rulerDlg->hide();
		m_rulerDlg->OnCheckCommand();
	}
}

void MainWindow::UpdateMeasureArea()
{
	Config *conf = g_app->GetConfig();

	if (conf->m_showMeasureArea)
	{
		m_ui->actionMeasureArea->setChecked(true);
		m_measureAreaDlg->OnCheckCommand();
		m_measureAreaDlg->show();
	}
	else
	{
		m_ui->actionMeasureArea->setChecked(false);
		m_measureAreaDlg->hide();
		m_measureAreaDlg->OnCheckCommand();
	}
}

void MainWindow::UpdatePlaceDlg(bool enable)
{
	Config *config = g_app->GetConfig();

	if (enable)
	{
		m_ui->menuBar->setEnabled(true);
		m_sideBar->m_placeDlg->GetPlaceView()->setEnabled(true);
		config->m_showPlaceMark = false;
		config->m_showPlaceFolder = false;
		config->m_showPath = false;
		config->m_showPolygon = false;
	}
	else
	{
		m_ui->menuBar->setDisabled(true);
		m_sideBar->m_placeDlg->GetPlaceView()->setDisabled(true);
		config->m_showPlaceMark = true;
		config->m_showPlaceFolder = true;
		config->m_showPath = true;
		config->m_showPolygon = true;
	}
}

void MainWindow::UpdateTourPlayLine()
{
	Config *config = g_app->GetConfig();

	if (config->m_showTourPlay)
	{
		config->m_showTourPlay = false;

		m_renderFrame->DestroyTourPlayBar();
		g_app->GetRenderProxy()->GetRenderSrv()->EndSimulation();
	}
	else
	{
		// get location arr
		PlaceMarkMgr *pmMgr = m_renderFrame->GetPlaceMarkMgr();
		pmMgr->SetResultTreeView(m_sideBar->GetActiveView());
		PLACEMARK_DATA placeData;
		pmMgr->GetPropertyPlaceData(&placeData, false);
		CGIS_Node *pNode = gdm_GetGISDoc()->GetNodeFromKey(placeData.key);
		if (!pNode || (pNode && pNode->m_nGisType != E_GISNODE_TYPE_PATH))
		{
			gu_MessageBox(this, tr("Please select the path to tour"), tr("You must select one path to tour"), MSGBOX_ICONINFO);
			m_ui->actionTourPlay->setChecked(false);
			return;
		}

		if(pNode && pNode->Get_GisType() == E_GISNODE_TYPE_PATH)
		{			
			config->m_showTourPlay = true;
			m_renderFrame->CreateTourPlayBar();		

			CGDM_SimMgr* pSimMgr = g_app->GetRenderProxy()->GetRenderSrv()->m_pSimMgr;
			if (!pSimMgr)
				return;

			g_app->GetRenderProxy()->GetRenderSrv()->StartSimulation(SIM_TYPE_PATH);

			GDM_SIM_STATUS simStatus;
			memset(&simStatus, 0, sizeof(GDM_SIM_STATUS));
			simStatus.m_pathcameraslopangle = config->m_tourTiltAngle;
			simStatus.m_pathcameradistance = config->m_tourRange;
			simStatus.m_speed = config->m_tourSpeed;

			pSimMgr->SetSimulationStatus(&simStatus);

			GDM_CAMERA_INFO CameraInfo;
			//SetGISPath(IN CGIS_Path* a_pGISPath , OUT GDM_CAMERA_INFO* a_pCameraInfo)

			pSimMgr->SetGISPath((CGIS_Path*)pNode , &CameraInfo);

			g_app->GetRenderProxy()->GetRenderSrv()->m_pCameraController->AutoMoveTo(0, CameraInfo, 7000);
			m_renderFrame->setFocus();
		}
	}

	m_ui->actionTourPlay->setChecked(config->m_showTourPlay);
	m_ui->actionFlightSimulator->setDisabled(config->m_showTourPlay);
	m_ui->actionCarSimulator->setDisabled(config->m_showTourPlay);

	UpdateMenuWithSim(!config->m_showTourPlay);
}

void MainWindow::UpdateFlightSim()
{
	Config *config = g_app->GetConfig();

	if (!config->m_showFlight)
	{
		m_ui->actionFlightSimulator->setText(tr("Exit Flight Simulator"));
		config->m_showFlight = true;

		g_app->GetRenderProxy()->GetRenderSrv()->StartSimulation(SIM_TYPE_FLIGHT);
		m_renderFrame->CreateFlightSimCtrl();
		m_renderFrame->setFocus();
	}
	else
	{
		m_ui->actionFlightSimulator->setText(tr("Enter Flight Simulator"));
		config->m_showFlight = false;

		g_app->GetRenderProxy()->GetRenderSrv()->EndSimulation();
		m_renderFrame->DestroyFlightSimCtrl();
	}

	m_ui->actionFlightSimulator->setChecked(config->m_showFlight);
	m_ui->actionCarSimulator->setDisabled(config->m_showFlight);
	m_ui->actionTourPlay->setDisabled(config->m_showFlight);

	UpdateMenuWithSim(!config->m_showFlight);
}

void MainWindow::UpdateCarSim()
{
	Config *config = g_app->GetConfig();

	if (!config->m_showCar)
	{
		m_ui->actionCarSimulator->setText(tr("Exit Car Simulator"));
		config->m_showCar = true;
		m_renderFrame->CreateCarSimCtrl();
		m_renderFrame->setFocus();

		g_app->GetRenderProxy()->GetRenderSrv()->StartSimulation(SIM_TYPE_CAR);
#if 1
		GDM_CAMERA_INFO cameraInfo;	
		memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));
		gdm_CameraGetInfo(g_pRender, &cameraInfo);

		cameraInfo.m_orgDist = gdm_getExactHeight(cameraInfo.m_loc.m_dLongitude, cameraInfo.m_loc.m_dLatitude);
		cameraInfo.m_loc.m_dDist = cameraInfo.m_orgDist + 3;
 		//cameraInfo.m_roll = 0.05;
 		cameraInfo.m_pitch = gdm_PI / 2;

		g_app->GetRenderProxy()->GetRenderSrv()->m_pCameraController->AutoMoveTo(1, cameraInfo, 4000);
#else
		GDM_Coord3D		coord;
		GDM_LOCATION	location;		
		GDM_CAMERA_INFO cameraInfo;	
		gdm_CameraGetLocation( g_pRender, &location );
		location.m_dDist = gdm_getExactHeight(location.m_dLongitude, location.m_dLatitude) + 5;
		gdm_GetStandardCoord( &location, &coord );
		gdm_RotateAroundXAxis( &coord, gdm_PI / 2 );
		gdm_GetCoordInfo( &coord, &cameraInfo );

		g_app->GetRenderProxy()->GetRenderSrv()->m_pCamera->AutoMoveTo(1, cameraInfo, 5000);
#endif
	}
	else
	{
		m_ui->actionCarSimulator->setText(tr("Enter Car Simulator"));
		config->m_showCar = false;
		m_renderFrame->DestroyCarSimCtrl();

		g_app->GetRenderProxy()->GetRenderSrv()->EndSimulation();
	}

	m_ui->actionCarSimulator->setChecked(config->m_showCar);
	m_ui->actionFlightSimulator->setDisabled(config->m_showCar);
	m_ui->actionTourPlay->setDisabled(config->m_showCar);

	UpdateMenuWithSim(!config->m_showCar);
}

void MainWindow::UpdateToolTip()
{
	Config *config = g_app->GetConfig();

	if (config->m_toolTipChecked)
	{
		//ToolBar ToolTip
		m_standardToolBarZoomIn->setToolTip(tr("ZoomIn"));
		m_standardToolBarZoomOut->setToolTip(tr("ZoomOut"));
		m_standardToolBarFullExtent->setToolTip(tr("FullExtent"));
		m_standardToolBarLeft->setToolTip(tr("Pan Left"));
		m_standardToolBarRight->setToolTip(tr("Pan Right"));
		m_standardToolBarUp->setToolTip(tr("Pan Up"));
		m_standardToolBarDown->setToolTip(tr("Pan Down"));

		m_searchCombo->setToolTip(tr("Search Item"));
		m_searchToolBar->actions().at(1)->setToolTip(tr("Search Start"));

		if (config->m_showSidebar)
			m_ui->actionSidebar->setToolTip(tr("SideBar Hide"));
		else
			m_ui->actionSidebar->setToolTip(tr("SideBar Show"));

		if (config->m_IsShowOverviewMap)
			m_ui->actionShowOverviewMap->setToolTip(tr("OverviewMap Hide"));
		else
			m_ui->actionShowOverviewMap->setToolTip(tr("OverviewMap Show"));

		if (config->m_showRuler)
			m_ui->actionRuler->setToolTip(tr("Ruler Hide"));
		else
			m_ui->actionRuler->setToolTip(tr("Ruler Show"));

		m_ui->actionCut->setToolTip(tr("Cut"));
		m_ui->actionCopy->setToolTip(tr("Copy"));
		m_ui->actionPaste->setToolTip(tr("Paste"));
		m_ui->actionDelete->setToolTip(tr("Delete"));

		m_ui->actionAddFolder->setToolTip(tr("Folder Add"));
		m_ui->actionAddPlaceMark->setToolTip(tr("PlaceMark Add"));
		m_ui->actionAddPath->setToolTip(tr("Path Add"));
		m_ui->actionAddPolygon->setToolTip(tr("Polygon Add"));
				
		m_ui->actionFlightSimulator->setToolTip(tr("Enter Flight Simulator"));
		m_ui->actionCarSimulator->setToolTip(tr("Enter Car Simulator"));
		m_ui->actionTourPlay->setToolTip(tr("Tour Play"));
	}
	else
	{
		//ToolBar ToolTip Hide
		m_searchCombo->setToolTip(MSG_NO_TOOLTIP);
		foreach(QAction *action, m_standardToolBar->actions())
			action->setToolTip(MSG_NO_TOOLTIP);
		foreach(QAction *action, m_searchToolBar->actions())
			action->setToolTip(MSG_NO_TOOLTIP);
		foreach(QAction *action, m_simulationToolBar->actions())
			action->setToolTip(MSG_NO_TOOLTIP);
	}

	m_sideBar->UpdateToolTip();
}

void MainWindow::UpdateShortCutKey()
{
	MenuUpdate();
}

void MainWindow::ShowContextMenu(const QPoint &point, bool sidebar)
{
	MenuUpdate();
	QMenu menu(this);

	QMenu subMenu(tr("add"), &menu);
#ifdef Q_WS_MAC
	QAction *action = subMenu.addAction(tr("Folder") + 
		QLatin1String("\t   ") + QString(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N)));
	connect(action, SIGNAL(triggered()), this, SLOT(AddPlaceFolder()));
#else
	subMenu.addAction(m_ui->actionAddFolder);
#endif
	subMenu.addAction(m_ui->actionAddPlaceMark);
	subMenu.addSeparator();
	subMenu.addAction(m_ui->actionAddPath);
	subMenu.addAction(m_ui->actionAddPolygon);

	if (sidebar)
	{
		menu.addAction(subMenu.menuAction());
		menu.addSeparator();
	}

	if (m_sideBar->GetActiveView()->IsCutItem())
		menu.addAction(m_ui->actionCut);

#ifdef Q_WS_MAC
	action = menu.addAction(tr("Copy") + 
		QLatin1String("\t   ") + QString(QKeySequence(Qt::CTRL + Qt::Key_C)));
	connect(action, SIGNAL(triggered()), this, SLOT(Copy()));
#else
	menu.addAction(m_ui->actionCopy);
#endif
	menu.addAction(m_ui->actionPaste);
	if (m_sideBar->GetActiveView()->IsDeleteItem())
		menu.addAction(m_ui->actionDelete);
	if (m_sideBar->GetActiveView()->IsDeleteContents())
		menu.addAction(m_ui->actionContentDel);
	//menu.addAction(m_ui->actionRename);
	//menu.addSeparator();
	if (m_sideBar->IsMoveToMyPlace())
	{
		menu.addSeparator();
		menu.addAction(m_ui->actionMoveToMyPlace);
	}

	menu.addSeparator();
	menu.addAction(m_ui->actionSaveAsPlaceMarkFile);

	if (m_sideBar->GetActiveView()->IsSetProperty())
	{
		menu.addSeparator();
		menu.addAction(m_ui->actionProperty);
	}


	menu.exec(point);
}

void MainWindow::ReleaseShortcutKey(bool release)
{
	if (release)
	{
		m_ui->actionCut->setShortcut(QKeySequence::UnknownKey);
		m_ui->actionCopy->setShortcut(QKeySequence::UnknownKey);
		m_ui->actionPaste->setShortcut(QKeySequence::UnknownKey);
	}
	else
	{
		m_ui->actionCut->setShortcut(QKeySequence::Cut);
		m_ui->actionCopy->setShortcut(QKeySequence::Copy);
		m_ui->actionPaste->setShortcut(QKeySequence::Paste);
	}
}

void MainWindow::Search(QString key, int index, int itemkey)
{
	m_sideBar->m_searchDlg->Search(key, index, itemkey);
}

/*
*   protected
*/
void MainWindow::SetIcon()
{
		QString iconName = g_app->GetResource(MAINWINDOW_ICON_NAME);
        QIcon icon(iconName);
        if(!icon.isNull())
            setWindowIcon(icon);
#ifdef GDEM_DEBUG
        else
			qDebug("Mainwindow::SetIcon Failed!");
#endif
}

void MainWindow::CreateMenus()
{
	// menu Update
	connect(m_ui->menuFile, SIGNAL(aboutToShow()), this, SLOT(MenuUpdate()));
	connect(m_ui->menuEdit, SIGNAL(aboutToShow()), this, SLOT(MenuUpdate()));
	connect(m_ui->menuLayer, SIGNAL(aboutToShow()), this, SLOT(UpdateLayerMenu()));
	connect(m_ui->menuTool, SIGNAL(aboutToShow()), this, SLOT(UpdateLayerMenu()));

    // File Menu
    connect(m_ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(OpenFile()));
	connect(m_ui->actionMoveToMyPlace, SIGNAL(triggered()), this, SLOT(MoveToMyPlace()));
    connect(m_ui->actionSaveAsPlaceMarkFile, SIGNAL(triggered()), this, SLOT(SaveAsPlaceMarkFile()));
	connect(m_ui->actionSaveMyPlace, SIGNAL(triggered()), this, SLOT(SaveMyPlace()));
	connect(m_ui->actionSaveImage, SIGNAL(triggered()), this, SLOT(SaveImage()));
    connect(m_ui->actionPrint, SIGNAL(triggered()), this, SLOT(Print()));
    connect(m_ui->actionExit, SIGNAL(triggered()), this, SLOT(Exit()));

	m_ui->actionOpenFile->setIcon(QIcon(g_app->GetResource("/toolbar/open.png")));
	m_ui->actionSaveMyPlace->setIcon(QIcon(g_app->GetResource("/toolbar/save.png")));
	m_ui->actionPrint->setIcon(QIcon(g_app->GetResource("/toolbar/print.png")));

    // Edit Menu
    connect(m_ui->actionCut, SIGNAL(triggered()), this, SLOT(Cut()));
    connect(m_ui->actionCopy, SIGNAL(triggered()), this, SLOT(Copy()));
	connect(m_ui->actionPaste, SIGNAL(triggered()), this, SLOT(Paste()));
    connect(m_ui->actionDelete, SIGNAL(triggered()), this, SLOT(Delete()));
    connect(m_ui->actionContentDel, SIGNAL(triggered()), this, SLOT(ContentDelete()));    
    connect(m_ui->actionProperty, SIGNAL(triggered()), this, SLOT(Property()));

	connect(m_ui->actionFindHistoryDel, SIGNAL(triggered()), this, SLOT(FindHistoryDelete()));
	connect(m_ui->actionImageCopy, SIGNAL(triggered()), this, SLOT(CopyImageToClip()));

	m_ui->actionCut->setIcon(QIcon(g_app->GetResource("/toolbar/cut.png")));
	m_ui->actionCopy->setIcon(QIcon(g_app->GetResource("/toolbar/copy.png")));
	m_ui->actionPaste->setIcon(QIcon(g_app->GetResource("/toolbar/paste.png")));
	m_ui->actionDelete->setIcon(QIcon(g_app->GetResource("/toolbar/delete.png")));
	m_ui->actionImageCopy->setIcon(QIcon(g_app->GetResource("/toolbar/copyimage.png")));

    // View Menu
	connect(m_ui->actionGrid, SIGNAL(triggered()), this, SLOT(ShowGrid()));
	m_ui->actionGrid->setIcon(QIcon(g_app->GetResource("/toolbar/grid.png")));

	// View->ViewSize
	connect(m_ui->actionVGA, SIGNAL(triggered()), this, SLOT(ViewVGA()));
	connect(m_ui->actionSVGA, SIGNAL(triggered()), this, SLOT(ViewSVGA()));
	connect(m_ui->actionNTSC, SIGNAL(triggered()), this, SLOT(ViewNTSC()));
	connect(m_ui->actionPAL, SIGNAL(triggered()), this, SLOT(ViewPAL()));
	connect(m_ui->actionWideNTSC, SIGNAL(triggered()), this, SLOT(ViewWideNTSC()));
	connect(m_ui->actionWidePAL, SIGNAL(triggered()), this, SLOT(ViewWidePAL()));
	connect(m_ui->action1080pHDTV, SIGNAL(triggered()), this, SLOT(View1080pHDTV()));
	connect(m_ui->action720pHDTV, SIGNAL(triggered()), this, SLOT(View720pHDTV()));
	connect(m_ui->action480pDVD, SIGNAL(triggered()), this, SLOT(View480pDVD()));
	connect(m_ui->actionLetter, SIGNAL(triggered()), this, SLOT(ViewLetter()));
	connect(m_ui->actionTabloid, SIGNAL(triggered()), this, SLOT(ViewTabloid()));
	connect(m_ui->actionPostCard1, SIGNAL(triggered()), this, SLOT(ViewPostCard1()));
	connect(m_ui->actionPostCard2, SIGNAL(triggered()), this, SLOT(ViewPostCard2()));

	connect(m_ui->actionFullScreen, SIGNAL(triggered()), this, SLOT(FullScreen()));
	m_ui->actionFullScreen->setIcon(QIcon(g_app->GetResource("/toolbar/fullscreen.png")));

	// View->NavigationDisplay
	connect(m_ui->actionNavAuto, SIGNAL(triggered()), this, SLOT(ViewAutoNav()));
	connect(m_ui->actionNavAlways, SIGNAL(triggered()), this, SLOT(ViewAlwaysNav()));
	connect(m_ui->actionNavDial, SIGNAL(triggered()), this, SLOT(ViewDialNav()));
	connect(m_ui->actionNavNever, SIGNAL(triggered()), this, SLOT(ViewNeverNav()));

	//View->ToolBar
	connect(m_ui->actionShowStandardToolBar, SIGNAL(triggered()), this, SLOT(ShowHideStandardToolBar()));
	connect(m_ui->actionShowSimulationToolBar, SIGNAL(triggered()), this, SLOT(ShowHideSimulationToolBar()));

	connect(m_ui->actionStatusbar, SIGNAL(triggered()), this, SLOT(ShowHideStatusbar()));
	connect(m_ui->actionSidebar, SIGNAL(triggered()), this, SLOT(ShowHideSidebar()));
	m_ui->actionSidebar->setIcon(QIcon(g_app->GetResource("/toolbar/sidebar.png")));
	
	connect(m_ui->actionShowOverviewMap, SIGNAL(triggered()), this, SLOT(ShowHideOverviewMap()));
	m_ui->actionShowOverviewMap->setIcon(QIcon(g_app->GetResource("/sidebar/layers-boundary.png")));


	connect(m_ui->actionBuildingName, SIGNAL(triggered()), this, SLOT(ShowHideBuildingName()));

	//Layer Menu
	connect(m_ui->actionTerrain, SIGNAL(triggered()), this, SLOT(SetTerrain()));
	connect(m_ui->actionContour, SIGNAL(triggered()), this, SLOT(SetContour()));
	connect(m_ui->actionRenderTexture, SIGNAL(triggered()), this, SLOT(SetTexture()));
	connect(m_ui->actionRenderAltitude, SIGNAL(triggered()), this, SLOT(SetAltitude()));
	connect(m_ui->actionRenderGradient, SIGNAL(triggered()), this, SLOT(SetGradient()));
	connect(m_ui->actionRenderIncline, SIGNAL(triggered()), this, SLOT(SetIncline()));

	//Add Menu
	connect(m_ui->actionAddFolder, SIGNAL(triggered()), this, SLOT(AddPlaceFolder()));
    connect(m_ui->actionAddPlaceMark, SIGNAL(triggered()), this, SLOT(AddPlaceMark()));
	connect(m_ui->actionAddPath, SIGNAL(triggered()), this, SLOT(AddPath()));
	connect(m_ui->actionAddPolygon, SIGNAL(triggered()), this, SLOT(AddPolygon()));
	connect(m_ui->actionAdd3DObject, SIGNAL(triggered()), this, SLOT(Add3DObject()));
	connect(m_ui->actionObjectPlaceEdit, SIGNAL(triggered()), this, SLOT(ObjectPlaceEdit())); //asd 2014.4.1
	
	m_ui->actionAddFolder->setIcon(QIcon(g_app->GetResource("/toolbar/folder.png")));
	m_ui->actionAddPlaceMark->setIcon(QIcon(g_app->GetResource("/toolbar/pin.png")));
	m_ui->actionAddPath->setIcon(QIcon(g_app->GetResource("/toolbar/path.png")));
	m_ui->actionAddPolygon->setIcon(QIcon(g_app->GetResource("/toolbar/polygon.png")));

    // Tool Menu
    connect(m_ui->actionRuler, SIGNAL(triggered()), this, SLOT(Ruler()));
	m_ui->actionRuler->setIcon(QIcon(g_app->GetResource("/toolbar/ruler.png")));
	connect(m_ui->actionMeasureArea, SIGNAL(triggered()), this, SLOT(MeasureArea()));
	connect(m_ui->actionGoTo, SIGNAL(triggered()), this, SLOT(GoTo()));
	connect(m_ui->actionSaveStartPos, SIGNAL(triggered()), this, SLOT(SaveStartPosition()));
	connect(m_ui->actionAnimation, SIGNAL(triggered()), this, SLOT(Animation()));

	//Simulation Menu
	connect(m_ui->actionFlightSimulator, SIGNAL(triggered()), this, SLOT(FlightSimulator()));
	connect(m_ui->actionCarSimulator, SIGNAL(triggered()), this, SLOT(CarSimulator()));
	connect(m_ui->actionTourPlay, SIGNAL(triggered()), this, SLOT(TourPlayLine()));

	m_ui->actionFlightSimulator->setIcon(QIcon(g_app->GetResource("/toolbar/flight.png")));
	m_ui->actionCarSimulator->setIcon(QIcon(g_app->GetResource("/toolbar/car.png")));
	m_ui->actionTourPlay->setIcon(QIcon(g_app->GetResource("/toolbar/tourplayline.png")));

	//Setting Menu
    connect(m_ui->actionOption, SIGNAL(triggered()), this, SLOT(Option()));
	connect(m_ui->actionServer, SIGNAL(triggered()), this, SLOT(SetServer()));
	connect(m_ui->actionLicence,SIGNAL(triggered()),this,SLOT(SetLicence()));
	connect(m_ui->actionObjectSetting, SIGNAL(triggered()), this, SLOT(SetObject())); //asd 2014.4.7

	if(g_app->GetConfig()->m_IsLicence==true)
		m_ui->actionLicence->setEnabled(false);

	//added by RSH 2013.3.16 for change of shape
	connect(m_ui->actionGeoshapeFormat, SIGNAL(triggered()), this, SLOT(SetGeoshapeFormat()));
	//end
#ifdef STANDARD_ALONE
        m_ui->actionServer->setText(tr("Set DBPath"));
#endif
	connect(m_ui->actionContourSetting, SIGNAL(triggered()), this, SLOT(ContourSetting()));

    // Help Menu
    connect(m_ui->actionHelpManual, SIGNAL(triggered()), this, SLOT(HelpManual()));
	m_ui->actionHelpManual->setIcon(QIcon(g_app->GetResource("/toolbar/help.png")));

	connect(m_ui->actionAbout, SIGNAL(triggered()), this, SLOT(About()));

	// Drag and Drop
	connect(m_renderFrame, SIGNAL(dropfile(QString)), this, SLOT(OpenFilePath(QString)));

	//Find Menu
	connect(m_ui->actionBuildingFind,SIGNAL(triggered()),this,SLOT(BuildingFind()));
	connect(m_ui->actionPeopleFind,SIGNAL(triggered()),this,SLOT(PeopleFind()));

}

bool MainWindow::CreateFrame()
{
    m_sideBar = new SideBar;
	if (m_sideBar == NULL)
		return false;
	if (!m_sideBar->Init())
		return false;

	if (!CreateToolBars())
		return false;

	m_renderFrame = new RenderFrame(this);
	if (m_renderFrame == NULL)
		return false;
	if (!m_renderFrame->Init())
		return false;
	
	m_splitter = new QSplitter(Qt::Horizontal);
	if (m_splitter == NULL)
		return false;

	m_splitter->addWidget(m_sideBar);
	m_splitter->addWidget(m_renderFrame);
	setCentralWidget(m_splitter);
	m_splitter->setChildrenCollapsible(false);

	return true;
}

void MainWindow::DestroyFrame()
{
	if (m_sideBar)
	{
		m_sideBar->Release();
		delete m_sideBar;
		m_sideBar = NULL;
	}

	if (m_renderFrame)
	{
		m_renderFrame->Release();
		delete m_renderFrame;
		m_renderFrame=NULL;
	}

	if(m_splitter)
	{
		delete m_splitter;
		m_splitter=NULL;
	}

#ifdef DEBUG
	if(m_statusbBarLeftLabel)
	{
		delete m_statusbBarLeftLabel;
		m_statusbBarLeftLabel=NULL;
	}
#endif
	if(m_statusbBarCenterLabel)
	{
		delete m_statusbBarCenterLabel;
		m_statusbBarCenterLabel=NULL;
	}

	if(m_statusbBarRightLabel)
	{
		delete m_statusbBarRightLabel;
		m_statusbBarRightLabel=NULL;
	}

}
bool MainWindow::CreateSearchToolBar()
{
	//Search ToolBar
	m_searchToolBar = new QToolBar(tr("Search"),this);
	if (m_searchToolBar == NULL)
		return false;
	m_searchToolBar->setMovable(false);
	m_searchToolBar->setVisible(false);
	this->addToolBar(Qt::TopToolBarArea, m_searchToolBar);
	m_searchToolBar->setIconSize(QSize(TB_ICON_WIDTH, TB_ICON_HEIGHT));
	
	//Completer
	/*QCompleter *completer = new QCompleter(w);
	if (completer == NULL)
		return false;
	completer->setModel(g_app->GetSearchModel());
	completer->setCaseSensitivity(Qt::CaseSensitive);
	completer->setCompletionMode(QCompleter::InlineCompletion);*/

	m_searchCombo = new QComboBox(this);
	//	m_searchCombo->setCompleter(completer);
	m_searchCombo->setAutoCompletion(false);
	m_searchCombo->setInsertPolicy(QComboBox::InsertAtTop);
	m_searchCombo->setModel(g_app->GetSearchModel());
	m_searchCombo->setEditable(true);
	m_searchCombo->setFixedWidth(200);
	m_searchCombo->lineEdit()->setContextMenuPolicy(Qt::CustomContextMenu);
	m_searchCombo->setToolTip(MSG_NO_TOOLTIP);

	m_searchToolBar->addWidget(m_searchCombo);

	QString iconName = g_app->GetResource("/sidebar/search.png");
	m_searchToolBarSearch = m_searchToolBar->addAction(QIcon(iconName), MSG_NO_TOOLTIP);

	connect(m_searchCombo->lineEdit(), SIGNAL(returnPressed()), this, SLOT(SearchPlace()));
	connect(m_searchCombo->lineEdit(), SIGNAL(customContextMenuRequested(QPoint)), 
		                                     this, SLOT(ShowSearchContextMenu(QPoint)));
	connect(m_searchToolBarSearch, SIGNAL(triggered()), this, SLOT(SearchClick()));
	return true;
}

bool MainWindow::CreateSimulationToolBar()
{
	m_simulationToolBar = new QToolBar(tr("Simulation"),this);
	if(m_simulationToolBar == NULL)
		return false;
	this->addToolBar(Qt::TopToolBarArea, m_simulationToolBar);
	m_simulationToolBar->setIconSize(QSize(TB_ICON_WIDTH, TB_ICON_HEIGHT));
		
	m_simulationToolBar->addSeparator();

	m_simulationToolBar->addAction(m_ui->actionFlightSimulator);
    m_simulationToolBar->addAction(m_ui->actionCarSimulator);
	m_simulationToolBar->addAction(m_ui->actionTourPlay);

	return true;
}

bool MainWindow::CreateAnimationToolBar()
{
	m_AnimationToolBar=new QToolBar(tr("Animation"),this);
	if(!m_AnimationToolBar)
		return false;

	QMenu* animenu=new QMenu(tr("Animation"),m_AnimationToolBar);

	m_AniOpen=animenu->addAction(tr("Open Animation File..."));
	m_AniOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
	m_OpenAniEditor=animenu->addAction(tr("Open Animation Editor..."));

	m_AnimationToolBar->addAction(animenu->menuAction());

	this->addToolBar(Qt::TopToolBarArea, m_AnimationToolBar);
	m_AnimationToolBar->setIconSize(QSize(TB_ICON_WIDTH, TB_ICON_HEIGHT));

	QString iconName = g_app->GetResource("/toolbar/flight.png");
	m_AniStart = m_AnimationToolBar->addAction(style()->standardIcon(QStyle::SP_MediaPlay), MSG_NO_TOOLTIP);
	
	m_AniStop= m_AnimationToolBar->addAction(style()->standardIcon(QStyle::SP_MediaStop), MSG_NO_TOOLTIP);

	m_AniStart->setEnabled(false);
	m_AniStop->setEnabled(false);

	connect(m_AniOpen,SIGNAL(triggered()),this,SLOT(AniFileOpen()));
	connect(m_OpenAniEditor,SIGNAL(triggered()),this,SLOT(OpenAniEditor()));
	connect(m_AniStart,SIGNAL(triggered()),this,SLOT(AniStart()));
	connect(m_AniStop,SIGNAL(triggered()),this,SLOT(AniStop()));
	return true;
}

void MainWindow::AniFileOpen()
{
	m_AniStart->setEnabled(true);
	m_animationDlg->open();
}

void MainWindow::OpenAniEditor()
{
	if(!m_animationDlg->isVisible())
		m_animationDlg->show();
}

void MainWindow::AniStart()
{
	if(!m_animationDlg->checkVideoFiles())
		return ;

	if(m_animationDlg->isVisible())
		m_animationDlg->hide();
	if(m_sideBar->isVisible())
		m_sideBar->hide();

	m_AniStart->setEnabled(false);
	m_AniStop->setEnabled(true);

	m_animationDlg->play();

	connect(g_pAnimationMgr,SIGNAL(animationStopped()),this,SLOT(OnAniStopped()));
}

void MainWindow::OnAniStopped()
{
	if (g_app->GetConfig()->m_showSidebar)
		m_sideBar->show();

	m_AniStart->setEnabled(true);
	m_AniStop->setEnabled(false);

	m_videoWebView->reset();
	m_videoWebView->hide();
}

void MainWindow::AniStop()
{
	if (g_app->GetConfig()->m_showSidebar)
		m_sideBar->show();
	
	m_AniStart->setEnabled(true);
	m_AniStop->setEnabled(false);
	m_animationDlg->stop();

	m_videoWebView->reset();
	m_videoWebView->hide();
}

bool MainWindow::CretateStandardToolBar()
{
	m_standardToolBar= new QToolBar(tr("Standard"),this);
	if(!m_standardToolBar)	
		return false;

	this->addToolBar(Qt::TopToolBarArea, m_standardToolBar);
	m_standardToolBar->setIconSize(QSize(TB_ICON_WIDTH, TB_ICON_HEIGHT));
	
	m_standardToolBar->addAction(m_ui->actionOpenFile);
	m_standardToolBar->addAction(m_ui->actionSaveMyPlace);
	m_standardToolBar->addSeparator();
	m_standardToolBar->addAction(m_ui->actionImageCopy);
	m_standardToolBar->addAction(m_ui->actionPrint);
	m_standardToolBar->addSeparator();

	QString iconName = g_app->GetResource("/toolbar/zoomin.png");
	m_standardToolBarZoomIn = m_standardToolBar->addAction(QIcon(iconName), MSG_NO_TOOLTIP);

	iconName = g_app->GetResource("/toolbar/zoomout.png");
	m_standardToolBarZoomOut = m_standardToolBar->addAction(QIcon(iconName), MSG_NO_TOOLTIP);

	iconName = g_app->GetResource("/toolbar/fullextent.png");
	m_standardToolBarFullExtent = m_standardToolBar->addAction(QIcon(iconName), MSG_NO_TOOLTIP);

	iconName = g_app->GetResource("/toolbar/left.png");
	m_standardToolBarLeft = m_standardToolBar->addAction(QIcon(iconName), MSG_NO_TOOLTIP);

	iconName = g_app->GetResource("/toolbar/right.png");
	m_standardToolBarRight = m_standardToolBar->addAction(QIcon(iconName), MSG_NO_TOOLTIP);

	iconName = g_app->GetResource("/toolbar/up.png");
	m_standardToolBarUp = m_standardToolBar->addAction(QIcon(iconName), MSG_NO_TOOLTIP);

	iconName = g_app->GetResource("/toolbar/down.png");
	m_standardToolBarDown = m_standardToolBar->addAction(QIcon(iconName), MSG_NO_TOOLTIP);
	m_standardToolBar->addSeparator();

	connect(m_standardToolBarZoomIn, SIGNAL(triggered()), this, SLOT(ZoomIn()));
	connect(m_standardToolBarZoomOut, SIGNAL(triggered()), this, SLOT(ZoomOut()));
	connect(m_standardToolBarFullExtent, SIGNAL(triggered()), this, SLOT(FullExtent()));
	connect(m_standardToolBarLeft, SIGNAL(triggered()), this, SLOT(Left()));
	connect(m_standardToolBarRight, SIGNAL(triggered()), this, SLOT(Right()));
	connect(m_standardToolBarUp, SIGNAL(triggered()), this, SLOT(Up()));	
	connect(m_standardToolBarDown, SIGNAL(triggered()), this, SLOT(Down()));

	m_standardToolBar->addAction(m_ui->actionSidebar);
	m_standardToolBar->addAction(m_ui->actionGrid);
	m_standardToolBar->addAction(m_ui->actionShowOverviewMap);

	m_standardToolBar->addSeparator();

	m_standardToolBar->addAction(m_ui->actionRuler);
	m_standardToolBar->addSeparator();

	iconName = g_app->GetResource("/toolbar/select.png");
	m_standardToolBarSelectObject = m_standardToolBar->addAction(QIcon(iconName), MSG_NO_TOOLTIP);
	m_standardToolBarSelectObject->setCheckable(true);
	m_standardToolBar->addSeparator();

	connect(m_standardToolBarSelectObject, SIGNAL(triggered()), this, SLOT(SelectObject()));

	m_standardToolBar->addAction(m_ui->actionCut);
	m_standardToolBar->addAction(m_ui->actionCopy);
	m_standardToolBar->addAction(m_ui->actionPaste);
	m_standardToolBar->addAction(m_ui->actionDelete);

	m_standardToolBar->addSeparator();

	m_standardToolBar->addAction(m_ui->actionAddFolder);
	m_standardToolBar->addAction(m_ui->actionAddPlaceMark);
    m_standardToolBar->addAction(m_ui->actionAddPath);
	m_standardToolBar->addAction(m_ui->actionAddPolygon);

	return true;
}

void MainWindow::ZoomIn()
{
	g_app->ZoomView(10);
}

void MainWindow::ZoomOut()
{
	g_app->ZoomView(-10);
}

void MainWindow::FullExtent()
{
	g_app->FullExtent();
}

void MainWindow::Left()
{
	g_app->MoveView(1,0);
}

void MainWindow::Right()
{
	g_app->MoveView(-1,0);
}

void MainWindow::Up()
{
	g_app->MoveView(0,1);
}

void MainWindow::Down()
{
	g_app->MoveView(0,-1);
}

bool MainWindow::CreateToolBars()
{
	if(!CretateStandardToolBar()) return false;
	if(!CreateSearchToolBar()) return false;
	if(!CreateSimulationToolBar()) return false;
	if(!CreateAnimationToolBar()) return false;
	
	connect(m_AnimationToolBar,SIGNAL(visibilityChanged(bool)),this,SLOT(onAnimationToolbarVisibilityChanged(bool)));
	return true;
}

void MainWindow::onAnimationToolbarVisibilityChanged(bool visible)
{
	Config* conf=g_app->GetConfig();
	conf->m_showAnimationToolbar=visible;
}
static void SetCheckedNavMenu(Ui::MainWindow *ui, int mode)
{
	ui->actionNavAuto->setChecked(mode == NM_AUTO);
	ui->actionNavAlways->setChecked(mode == NM_ALWAYS);
	ui->actionNavDial->setChecked(mode == NM_COMPASS);
	ui->actionNavNever->setChecked(mode == NM_NODISP);
}

void MainWindow::Reposition()
{
	//window reposition
    QPoint pos = g_app->GetConfig()->m_wndPos;
    QSize size = g_app->GetConfig()->m_wndSize;

    if (size.isEmpty())
    {
        QDesktopWidget desktop;
        QRect rc = desktop.availableGeometry();
        pos.setX(rc.width() / 10);
        pos.setY(rc.height() / 10);
        size.setWidth(pos.x() * 8);
        size.setHeight(pos.y() * 8);

        move(pos);
        resize(size);

		bool fullscreen = g_app->GetConfig()->m_fullScreen;
		if (fullscreen)
			showFullScreen();
		else
			setWindowState(Qt::WindowMaximized);

		m_ui->actionFullScreen->setChecked(fullscreen);
    }
    else
    {
        move(pos);
        resize(size);
    }
}

void MainWindow::RewidthSideBar()
{
	QList <int> widthList = m_splitter->sizes();
	int w = g_app->GetConfig()->m_sideBarWidth;

	if (w > 0)
	{
		widthList[1] = widthList[0] + widthList[1] - w;
		widthList[0] = w;
	}

	m_splitter->setSizes(widthList);
}

void MainWindow::ResizeRenderFrame(int size)
{
	int width, height;
	int dw = geometry().size().width() - m_renderFrame->width();
	int dh = geometry().size().height() - m_renderFrame->height();

	QList <int> widthList = m_splitter->sizes();

	if (isFullScreen() || isMaximized())
	{
		gu_MessageBox(this, tr("Info"), tr("When FullScreen or Maximized Window, can't adjust view size."), MSGBOX_ICONINFO);
		return;
	}

	if (s_viewSize[size].fxHeight != -1)
	{
		width = s_viewSize[size].fxHeight * s_viewSize[size].w / s_viewSize[size].h;
		height = s_viewSize[size].fxHeight;
	}
	else
	{
		width = s_viewSize[size].w;
		height = s_viewSize[size].h;
	}

	widthList[1] = width + dw - widthList[0];

	resize(width + dw, height + dh);
	m_splitter->setSizes(widthList);
}

void MainWindow::UpdateMenus()
{
	Config *conf = g_app->GetConfig();

	//Initially, Sidebar show or hide
	UpdateSideBar(conf->m_showSidebar);

	//Initially, Statusbar show or hide
	if (conf->m_showStatusbar)
	{
		m_ui->actionStatusbar->setChecked(true);
		 statusBar()->show();
	}
	else
	{
		m_ui->actionStatusbar->setChecked(false);
		statusBar()->hide();
	}

	//layer
	UpdateLayer();

	//grid
	UpdateGrid();

	//sale legend
	m_standardToolBarSelectObject->setChecked(conf->m_IsSelectObject);
	m_ui->actionBuildingName->setChecked(g_app->GetConfig()->m_IsShowBuildingName);

	//Initially, Ruler Dialog show or hide
	UpdateRuler();

	//set navigation mode
	SetCheckedNavMenu(m_ui, conf->m_showNavMode);
	m_renderFrame->GetNavBar()->ChangedMode(conf->m_showNavMode);

	//tooltip show/hide
	UpdateToolTip();
}

void MainWindow::UpdateSideBar(bool show)
{
	if (g_app->GetConfig()->m_showFlight || g_app->GetConfig()->m_showCar || g_app->GetConfig()->m_showTourPlay)
		return;

	if (show)
		m_sideBar->show();
	else
		m_sideBar->hide();

	m_ui->actionSidebar->setChecked(show);
	UpdateSearchToolBar(!show);
}

void MainWindow::UpdateToolBars()
{
	Config *conf = g_app->GetConfig();
	if(conf->m_showStandardToolbar)
	{
		m_ui->actionShowStandardToolBar->setChecked(true);
		m_standardToolBar->show();
	}
	else
	{
		m_ui->actionShowStandardToolBar->setChecked(false);
		m_standardToolBar->hide();
	}

	if(conf->m_showSimulationToolbar)
	{
		m_ui->actionShowSimulationToolBar->setChecked(true);
		m_simulationToolBar->show();
	}
	else
	{
		m_ui->actionShowSimulationToolBar->setChecked(false);
		m_simulationToolBar->hide();
	}

	if(conf->m_showAnimationToolbar)
	{
		m_AnimationToolBar->show();
	}
	else
	{
		m_AnimationToolBar->hide();
	}

	if (conf->m_showSidebar)
		UpdateSearchToolBar(false);
    else
   		UpdateSearchToolBar(true);
}

void MainWindow::UpdateSearchToolBar(bool show)
{
	if (show && m_sideBar->isHidden())
		m_searchToolBar->show();
	else
		m_searchToolBar->hide();
}

void MainWindow::UpdateGrid()
{
	bool state = g_app->GetConfig()->m_showGrid;
	m_ui->actionGrid->setChecked(state);
	g_app->GetRenderProxy()->SendMessage(PM_GRID_STATE, (PrxParam)&state);
}

void MainWindow::UpdateMenuWithSim(bool enable)
{
	static unsigned int navmode = 0;
	Config *config = g_app->GetConfig();

	if (enable)
	{
		m_ui->menuEdit->setEnabled(true);
		m_ui->menuLayer->setEnabled(true);
		m_ui->menuAdd->setEnabled(true);

		//file menu
		m_ui->actionOpenFile->setEnabled(true);
		m_ui->actionPrint->setEnabled(true);

		//view menu
		m_ui->actionSidebar->setEnabled(true);
		m_ui->menuNavigation->setEnabled(true);
		m_ui->actionSaveStartPos->setEnabled(true);

		//tool menu
		m_ui->actionRuler->setEnabled(true);
		m_ui->actionServer->setEnabled(true);
		m_ui->actionContourSetting->setEnabled(true);
		m_ui->actionOption->setEnabled(true);

		if (navmode == NM_AUTO)
			ViewAutoNav();
		else if (navmode == NM_ALWAYS)
			ViewAlwaysNav();
		else if (navmode == NM_COMPASS)
			ViewDialNav();
		else if (navmode == NM_NODISP)
			ViewNeverNav();

		UpdateToolBars();
		UpdateSideBar(config->m_showSidebar);
	}
	else
	{
		//enter simulation
		
		m_standardToolBar->hide();
		m_searchToolBar->hide();
		m_simulationToolBar->hide();
		m_sideBar->hide();

		navmode = config->m_showNavMode;
		ViewNeverNav();

		m_ui->menuEdit->setDisabled(true);
		m_ui->menuLayer->setDisabled(true);
		m_ui->menuAdd->setDisabled(true);

		//file menu
		m_ui->actionOpenFile->setDisabled(true);
		m_ui->actionPrint->setDisabled(true);

		//view menu
		m_ui->actionSidebar->setDisabled(true);
		m_ui->menuNavigation->setDisabled(true);
		m_ui->actionSaveStartPos->setDisabled(true);

		//tool menu
		m_ui->actionRuler->setDisabled(true);
		m_ui->actionServer->setDisabled(true);
		m_ui->actionContourSetting->setDisabled(true);
		m_ui->actionOption->setDisabled(true);
	}
}

bool MainWindow::BeEditingDlg()
{
	Config *config = g_app->GetConfig();

	if (config->m_showPlaceFolder)
		return true;

	if (config->m_showPlaceMark)
		return true;

	if (config->m_showPath)
		return true;

	if (config->m_showPolygon)
		return true;

	if(config->m_showMeasureArea)
		return true;

	if(config->m_showObjectAdd)
		return true;

	if(config->m_showRuler)
		return true;

	return false;
}

/*
 * Protected
 */

void MainWindow::closeEvent(QCloseEvent *event)
{
	// Check Editing Dialog
	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Info"), tr("You must finish with your current item before closing window!"), MSGBOX_ICONINFO);
		event->ignore();
		return;
	}

	// Save My Places before exit program
	if (!m_sideBar->m_placeDlg->TempToMyPlaces())
	{
		event->ignore();
		return;
	}

	// Save My Places before exit program
	m_sideBar->m_placeDlg->SaveMyPlaces();

	// Free Libkml Memory manually
	m_sideBar->m_placeDlg->GetPlaceView()->FreeLibkmlMemory();	

    Release();
    event->accept();
}

/*
 * Private
 */

void MainWindow::MenuUpdate()
{
	ResultTreeView *treeView = m_sideBar->GetActiveView();

	if (treeView->IsCutItem())
		m_ui->actionCut->setEnabled(true);
	else
		m_ui->actionCut->setDisabled(true);

	if (treeView->IsCopyItem())
		m_ui->actionCopy->setEnabled(true);
	else
		m_ui->actionCopy->setDisabled(true);

	if (treeView->IsPasteItem())
		m_ui->actionPaste->setEnabled(true);
	else
		m_ui->actionPaste->setDisabled(true);

	if (treeView->IsDeleteItem())
		m_ui->actionDelete->setEnabled(true);
	else
		m_ui->actionDelete->setDisabled(true);

	if (treeView->IsDeleteContents())
		m_ui->actionContentDel->setEnabled(true);
	else
		m_ui->actionContentDel->setDisabled(true);

	if (treeView->IsSetProperty())
		m_ui->actionProperty->setEnabled(true);
	else
		m_ui->actionProperty->setDisabled(true);

	if (m_sideBar->IsMoveToMyPlace())
		m_ui->actionMoveToMyPlace->setEnabled(true);
	else
		m_ui->actionMoveToMyPlace->setDisabled(true);

	if (m_sideBar->IsSaveAsPlaceMarkFile())
		m_ui->actionSaveAsPlaceMarkFile->setEnabled(true);
	else
		m_ui->actionSaveAsPlaceMarkFile->setDisabled(true);



}

void MainWindow::UpdateLayerMenu()
{
	UpdateLayer();
}
//added by RSH 2013.3.16
void MainWindow::SetGeoshapeFormat()
{
	shapeColorPick *dlg;
	dlg = new shapeColorPick;
	dlg->show();

}
//end addition by RSH
void MainWindow::OpenFile()
{
	QStringList		fileNames;
	QString			fileName;
	QString			openDir;
	
	// Get open directory
	openDir = g_app->GetConfig()->m_openLastDir;

	// Get the file names as QStringList
#ifndef APP_USE_CUSTOM_DIALOG
	fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),
											  openDir, tr("KML Files *.kml;;All Files *.*"));
#else
	fileNames = FileDialog::GetOpenFileNames(this, tr("Open File"),
											  openDir, tr("KML Files *.kml;;All Files *.*"));
#endif
	// Open all of the files
	if (!fileNames.isEmpty())
	{
		foreach (fileName, fileNames)
		{
			// Open one KML file
			if (m_sideBar->m_placeDlg->GetPlaceView()->OpenFile(fileName) == 0)
			{
				// Set config value
				g_app->GetConfig()->m_openLastDir = QFileInfo(fileName).absolutePath();
			}
			// Error
			else
			{
				gu_MessageBox(this, tr("GDEM Client"), tr("File open error."), MSGBOX_ICONCRITICAL);
			}
		}
	}
}

void MainWindow::OpenFilePath(const QString &path)
{
	m_sideBar->m_placeDlg->GetPlaceView()->OpenFile(path);
}

void MainWindow::MoveToMyPlace()
{
	m_sideBar->MoveToMyPlace();
}

void MainWindow::SaveAsPlaceMarkFile()
{
	QString saveDir;
	QString defaultFName = "";
	QString	fileName;

	// Get save directory
	saveDir = g_app->GetConfig()->m_saveLastDir;

	// Get the default file name of selected Item in TreeView
	if (m_sideBar->GetActiveView()->GetFNameToSaveItem(defaultFName) == 0)	
	{
		defaultFName = saveDir + "/" + defaultFName;		

		// Select the file name
#ifndef APP_USE_CUSTOM_DIALOG
		fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
												defaultFName, tr("KML Files *.kml"));
#else
		fileName = FileDialog::GetSaveFileName(this, tr("Save File"),
												defaultFName, tr("KML Files *.kml"));
#endif
		// Save file
		if (!fileName.isEmpty())
		{
			if (m_sideBar->GetActiveView()->SaveAsItem(fileName) == 0)
			{
				// Set config value
				g_app->GetConfig()->m_saveLastDir = QFileInfo(fileName).absolutePath();
			}
			// Error
			else
			{
				gu_MessageBox(this, tr("GDEM Client"), tr("File save error."), MSGBOX_ICONCRITICAL);
			}
		}
	}
	// Error (No item is selected)
	else
	{
		gu_MessageBox(this, tr("GDEM Client"), tr("No item is selected."), MSGBOX_ICONWARNING);
	}
}

void MainWindow::SaveMyPlace()
{
	// Save My Places
	if (m_sideBar->m_placeDlg->GetPlaceView()->SaveMyPlaces() != 0)
	{
		QMessageBox msgBox;
		msgBox.setText("ERROR: Failed to save my places.");
		msgBox.exec();
	}
}

void MainWindow::SaveImage()
{
	// Get Image for save
	QImage image;
	RenderProxy *render = g_app->GetRenderProxy();
	render->SendMessage(PM_GET_IMAGE, (PrxParam)&image);

	if (image.isNull())
		return;

	//Save Image as .bmp
	QString saveDir = g_app->GetConfig()->m_saveLastDir;
	QString defaultFName = saveDir + "/" + tr("UnNamedImage") +".bmp";
	
#ifndef APP_USE_CUSTOM_DIALOG
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
												defaultFName, tr("Bitmap Images *.bmp"));
#else
		QString fileName = FileDialog::GetSaveFileName(this, tr("Save File"),
												defaultFName, tr("Bitmap Images *.bmp"));
#endif

	if (!fileName.isEmpty())
	{
		if (!fileName.right(4).contains(".bmp", Qt::CaseInsensitive))
			fileName += ".bmp";

		if(image.save(fileName, "BMP"))
			g_app->GetConfig()->m_saveLastDir = QFileInfo(fileName).absolutePath();
	}
}

void MainWindow::Print()
{
    // Get Image for print
    QImage image;
	RenderProxy *render = g_app->GetRenderProxy();
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	render->SendMessage(PM_GET_IMAGE, (PrxParam)&image);

	if (image.isNull())
	{
		QApplication::restoreOverrideCursor();
		return;
	}

    QPrinter printer(QPrinter::HighResolution);
/*    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();

        int metrics = printer.paintEngine()->paintDevice()-> logicalDpiX ();
        int margin = (int) ((MARGIN_SIZE / 2.54) * metrics);

        rect.setWidth(rect.width() - 2 * margin);
        rect.setHeight(rect.height() - 2 * margin);

        size.scale(rect.size(), Qt::KeepAspectRatio);
        printer.setFullPage(true);
        painter.setViewport(rect.x() + margin, rect.y() + margin, size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
*/
    PrintDialog printdlg(&printer, this);
	QApplication::restoreOverrideCursor();
    if (printdlg.exec())
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();

        int metrics = printer.paintEngine()->paintDevice()-> logicalDpiX ();
        int margin = (int) ((MARGIN_SIZE / 2.54) * metrics);

        rect.setWidth(rect.width() - 2 * margin);
        rect.setHeight(rect.height() - 2 * margin);

        size.scale(rect.size(), Qt::KeepAspectRatio);
        printer.setFullPage(true);
        painter.setViewport(rect.x() + margin, rect.y() + margin, size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
}

void MainWindow::Exit()
{
    close();
}

void MainWindow::Cut()
{
	m_sideBar->GetActiveView()->CutItem();
}

void MainWindow::Copy()
{
	m_sideBar->GetActiveView()->CopyItem();
}

void MainWindow::CopyImageToClip()
{
	// Get Image
	QImage image;
	RenderProxy *render = g_app->GetRenderProxy();
	render->SendMessage(PM_GET_IMAGE, (PrxParam)&image);

	if (image.isNull())
		return;

	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setImage(image, QClipboard::Clipboard);
}

void MainWindow::Paste()
{
	m_sideBar->GetActiveView()->PasteItem();
}

void MainWindow::Delete()
{
	m_sideBar->GetActiveView()->DeleteItem();
}

void MainWindow::ContentDelete()
{
	m_sideBar->GetActiveView()->DeleteContents();
}

void MainWindow::FindHistoryDelete()
{
	QStringListModel *model = g_app->GetSearchModel();
	if (!model)
		return;
	int rowcount = model->rowCount(QModelIndex());
	model->removeRows(0, rowcount, QModelIndex());
}

void MainWindow::Property()
{
	PlaceMarkMgr *pmMgr = m_renderFrame->GetPlaceMarkMgr();
	pmMgr->SetResultTreeView(m_sideBar->GetActiveView());

	if (m_sideBar->GetActiveView()->IsPlaceIcon())
	{
		PlaceMarkDlg *dlg = new PlaceMarkDlg(this, false);
		dlg->show();
	}
	else if (m_sideBar->GetActiveView()->IsSearchData())
	{
		PlaceMarkDlg *dlg = new PlaceMarkDlg(this, false);
		dlg->show();
	}
	else if (m_sideBar->GetActiveView()->IsPlacePath())
	{
		PathDlg *dlg = new PathDlg(this, false);
		dlg->show();
	}
	else if (m_sideBar->GetActiveView()->IsPlaceArea())
	{
		PolygonDlg *dlg = new PolygonDlg(this, false);
		dlg->show();
	}
	else
	{
		PlaceFolderDlg *dlg = new PlaceFolderDlg(this, false);
		dlg->show();
	}

	UpdatePlaceDlg(false);
}

bool g_action = false; // After FullScreen, Sidebar Show/Hide Action State
bool g_sideBar = true; // Before FullScreen, Sidebar Show State
bool g_maximum = false; // Before FullScreen, Window Maximum State

void MainWindow::ShowHideSidebar()
{
	if (g_app->GetConfig()->m_showSidebar)
	{
		g_app->GetConfig()->m_showSidebar = false;
		//m_toolBar->actions().at(0)->setToolTip(tr("SideBar Show"));
	}
	else
	{
		g_app->GetConfig()->m_showSidebar = true;
		//m_toolBar->actions().at(0)->setToolTip(tr("SideBar Hide"));
	}
	
	UpdateSideBar(g_app->GetConfig()->m_showSidebar);
	g_action = true;
	UpdateToolTip();
}

void MainWindow::FullScreen()
{
	if (g_app->GetConfig()->m_fullScreen)
	{
		if (g_maximum)
			showMaximized();
		else
			showNormal();

		g_app->GetConfig()->m_fullScreen = false;
		m_ui->actionFullScreen->setChecked(false);

		if (!g_action)
		{
			if (g_sideBar)
			{
				UpdateSideBar(true);
				g_app->GetConfig()->m_showSidebar = true;
			}
			else
			{
				UpdateSideBar(false);
				g_app->GetConfig()->m_showSidebar = false;
			}
		}
	}
	else
	{
		g_maximum = isMaximized();

		showFullScreen();
		g_app->GetConfig()->m_fullScreen = true;
		m_ui->actionFullScreen->setChecked(true);

		g_action = false;
		if (!g_app->GetConfig()->m_showSidebar)
			g_sideBar = false;
		else
			g_sideBar = true;

		UpdateSideBar(false);
		g_app->GetConfig()->m_showSidebar = false;
	}
}

void MainWindow::ViewVGA()
{
	ResizeRenderFrame(VM_VGA);
}

void MainWindow::ViewSVGA()
{
	ResizeRenderFrame(VM_SVGA);
}

void MainWindow::ViewNTSC()
{
	ResizeRenderFrame(VM_NTSC);
}

void MainWindow::ViewPAL()
{
	ResizeRenderFrame(VM_PAL);
}

void MainWindow::ViewWideNTSC()
{
	ResizeRenderFrame(VM_WIDE_NTSC);
}

void MainWindow::ViewWidePAL()
{
	ResizeRenderFrame(VM_WIDE_PAL);
}

void MainWindow::View1080pHDTV()
{
	ResizeRenderFrame(VM_1080P_HDTV);
}

void MainWindow::View720pHDTV()
{
	ResizeRenderFrame(VM_720P_HDTV);
}

void MainWindow::View480pDVD()
{
	ResizeRenderFrame(VM_480P_DVD);
}

void MainWindow::ViewLetter()
{
	ResizeRenderFrame(VM_LETTER);
}

void MainWindow::ViewTabloid()
{
	ResizeRenderFrame(VM_TABLOID);
}

void MainWindow::ViewPostCard1()
{
	ResizeRenderFrame(VM_POSTCARD1);
}

void MainWindow::ViewPostCard2()
{
	ResizeRenderFrame(VM_POSTCARD2);
}

void MainWindow::ViewAutoNav()
{
	m_renderFrame->GetNavBar()->ChangedMode(NM_AUTO);
	SetCheckedNavMenu(m_ui, NM_AUTO);
	g_app->GetConfig()->m_showNavMode = NM_AUTO;
}

void MainWindow::ViewAlwaysNav()
{
	m_renderFrame->GetNavBar()->ChangedMode(NM_ALWAYS);
	SetCheckedNavMenu(m_ui, NM_ALWAYS);
	g_app->GetConfig()->m_showNavMode = NM_ALWAYS;
}

void MainWindow::ViewDialNav()
{
	m_renderFrame->GetNavBar()->ChangedMode(NM_COMPASS);
	SetCheckedNavMenu(m_ui, NM_COMPASS);
	g_app->GetConfig()->m_showNavMode = NM_COMPASS;
}

void MainWindow::ViewNeverNav()
{
	m_renderFrame->GetNavBar()->ChangedMode(NM_NODISP);
	SetCheckedNavMenu(m_ui, NM_NODISP);
	g_app->GetConfig()->m_showNavMode = NM_NODISP;
}

void MainWindow::ShowHideStatusbar()
{
	if (g_app->GetConfig()->m_showStatusbar)
	{
		statusBar()->hide();
		g_app->GetConfig()->m_showStatusbar = false;
	}
	else
	{
		 statusBar()->show();
		g_app->GetConfig()->m_showStatusbar = true;
	}
	m_renderFrame->update();
}

void MainWindow::ShowGrid()
{
	g_app->GetConfig()->m_showGrid = !g_app->GetConfig()->m_showGrid;
	UpdateGrid();
}

void MainWindow::SaveStartPosition()
{
	m_sideBar->m_placeDlg->SaveStartPosition();
}

void MainWindow::ShowHideBuildingName()
{
	g_app->GetConfig()->m_IsShowBuildingName=!g_app->GetConfig()->m_IsShowBuildingName;
	m_ui->actionBuildingName->setChecked(g_app->GetConfig()->m_IsShowBuildingName);

}
void MainWindow::SetTerrain()
{
	if (g_app->GetConfig()->m_blTerrain)//3.28
		g_app->GetConfig()->m_blTerrain =false;
	else
		g_app->GetConfig()->m_blTerrain= true;

	UpdateLayer();
	g_app->GetRenderProxy()->SendMessage(PM_CLEAR_SIMPLEBUILDINGCACHE);
}

void MainWindow::SetContour()
{
	if (g_app->GetConfig()->m_blContour)
		g_app->GetConfig()->m_blContour = false;
	else
		g_app->GetConfig()->m_blContour = true;

	UpdateLayer();
}

void MainWindow::SetTexture()
{
	g_app->GetConfig()->m_layerTopography = LTM_TEXTURE;

	UpdateLayer();
}

void MainWindow::SetAltitude()
{
	g_app->GetConfig()->m_layerTopography = LTM_HEIGHT;

	UpdateLayer();
}

void MainWindow::SetGradient()
{
	g_app->GetConfig()->m_layerTopography = LTM_RATEANGLE;

	UpdateLayer();
}

void MainWindow::SetIncline()
{
	g_app->GetConfig()->m_layerTopography = LTM_RATEPLAN;

	UpdateLayer();
}

void MainWindow::AddPlaceFolder()
{
	if (!m_rulerDlg->isHidden())
	{
		gu_MessageBox(this, tr("Please Close The Ruler Dialog"), tr("You must close ruler dialog before editing a new one."), MSGBOX_ICONINFO);
		return;
	}
	
	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please close edit window"), tr("Before edit new item, must finish current item"), MSGBOX_ICONINFO);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	PlaceMarkMgr *pmMgr = m_renderFrame->GetPlaceMarkMgr();
	pmMgr->SetResultTreeView(m_sideBar->GetActiveView());

	PlaceFolderDlg *dlg = new PlaceFolderDlg(this);
	dlg->show();
	UpdatePlaceDlg(false);
}

void MainWindow::AddPlaceMark()
{
	if (!m_rulerDlg->isHidden())
	{
		gu_MessageBox(this, tr("Please Close The Ruler Dialog"), tr("You must close ruler dialog before editing a new one."), MSGBOX_ICONINFO);
		return;
	}
	
	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please close edit window"), tr("Before edit new item, must finish current item"), MSGBOX_ICONINFO);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	PlaceMarkMgr *pmMgr = m_renderFrame->GetPlaceMarkMgr();
	pmMgr->SetResultTreeView(m_sideBar->GetActiveView());

	PlaceMarkDlg *dlg = new PlaceMarkDlg(this);
	dlg->show();

	UpdatePlaceDlg(false);
}

void MainWindow::AddPath()
{
	if (!m_rulerDlg->isHidden())
	{
		gu_MessageBox(this, tr("Please Close The Ruler Dialog"), tr("You must close ruler dialog before editing a new one."), MSGBOX_ICONINFO);
		return;
	}
	
	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please Close The Edit Window"), tr("You must finish with your current item before editing a new one."), MSGBOX_ICONINFO);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	PlaceMarkMgr *pmMgr = m_renderFrame->GetPlaceMarkMgr();
	pmMgr->SetResultTreeView(m_sideBar->GetActiveView());
	PathDlg *dlg = new PathDlg(this);
	dlg->show();

	UpdatePlaceDlg(false);
}

void MainWindow::AddPolygon()
{
	if (!m_rulerDlg->isHidden())
	{
		gu_MessageBox(this, tr("Please Close The Ruler Dialog"), tr("You must close ruler dialog before editing a new one."), MSGBOX_ICONINFO);
		return;
	}
	
	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please Close The Edit Window"), tr("You must finish with your current item before editing a new one."), MSGBOX_ICONINFO);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	PlaceMarkMgr *pmMgr = m_renderFrame->GetPlaceMarkMgr();
	pmMgr->SetResultTreeView(m_sideBar->GetActiveView());
	PolygonDlg *dlg = new PolygonDlg(this);
	dlg->show();

	UpdatePlaceDlg(false);
}

void MainWindow::Ruler()
{
	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please close edit window"), tr("Before open ruler dialog, must finish current item."), MSGBOX_ICONINFO);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	g_app->GetConfig()->m_showRuler = !g_app->GetConfig()->m_showRuler;
	UpdateRuler();
	UpdateToolTip();
}

void MainWindow::MeasureArea()
{
	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please close edit window"), tr("Before open ruler dialog, must finish current item."), MSGBOX_ICONINFO);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	g_app->GetConfig()->m_showMeasureArea = !g_app->GetConfig()->m_showMeasureArea;
	UpdateMeasureArea();
	UpdateToolTip();
}

void MainWindow::FlightSimulator()
{
	if (!m_rulerDlg->isHidden())
	{
		gu_MessageBox(this, tr("Please Close The Ruler Dialog"), tr("You must close ruler dialog before enter simulator."), MSGBOX_ICONINFO);
		m_ui->actionFlightSimulator->setChecked(false);
		return;
	}

	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please close edit window"), tr("Before enter simulator, must finish current item"), MSGBOX_ICONINFO);
		m_ui->actionFlightSimulator->setChecked(false);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	UpdateFlightSim();
}

void MainWindow::CarSimulator()
{
	if (!m_rulerDlg->isHidden())
	{
		gu_MessageBox(this, tr("Please Close The Ruler Dialog"), tr("You must close ruler dialog before enter simulator."), MSGBOX_ICONINFO);
		m_ui->actionCarSimulator->setChecked(false);
		return;
	}

	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please close edit window"), tr("Before enter simulator, must finish current item"), MSGBOX_ICONINFO);
		m_ui->actionCarSimulator->setChecked(false);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	UpdateCarSim();
}

void MainWindow::TourPlayLine()
{
	if (!m_rulerDlg->isHidden())
	{
		gu_MessageBox(this, tr("Please Close The Ruler Dialog"), tr("You must close ruler dialog before enter simulator."), MSGBOX_ICONINFO);
		m_ui->actionTourPlay->setChecked(false);
		return;
	}

	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please close edit window"), tr("Before enter simulator, must finish current item"), MSGBOX_ICONINFO);
		m_ui->actionTourPlay->setChecked(false);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	UpdateTourPlayLine();
}

void MainWindow::Option()
{
	OptionDlg *dlg = new OptionDlg(this);
	dlg->show();
}

void MainWindow::SetServer()
{
#ifdef STANDARD_ALONE
        DBPathSetting dlg;
        if(dlg.exec() == QDialog::Accepted)
        {
        	g_app->GetRenderProxy()->SendMessage(PM_ON_CHANGE_SERVER);
		}
#else
        Config *conf = g_app->GetConfig();
        SrvConfDlg dlg;
	char addr[256];
	unsigned int port;

	dlg.SetServer(conf->m_srvAddr, conf->m_srvPort);
	if (dlg.exec()==QDialog::Accepted)
	{
		dlg.GetServer(addr, 256, &port);
		if (strcmp(conf->m_srvAddr, addr)!=0 || conf->m_srvPort != port)
		{
			conf->SetServer(addr, port);
			DataProxy *proxy = g_app->GetDataProxy();
			if (proxy)
				proxy->SendMessage(PM_SET_DATASERVER, (PrxParam)addr, (PrxParam)port);
		}
        }
#endif //STANDARD_ALONE
}

void MainWindow::ContourSetting()
{
	ContourDlg *dlg = new ContourDlg(this);
	dlg->show();
}

void MainWindow::HelpManual()
{
	QString docUrl = QString("file:///");

	docUrl += g_app->GetWorkDir() + QString("/");
	docUrl += QString(HELP_FILENAME);

	QDesktopServices::openUrl(QUrl(docUrl));
}

void MainWindow::About()
{
	AboutDlg dlg;
	dlg.exec();
}

void MainWindow::SearchPlace()
{
	QString key = m_searchCombo->lineEdit()->text();
	m_sideBar->m_searchDlg->Search(key);
}

void MainWindow::SearchClick()
{
	QString key = m_searchCombo->lineEdit()->text();
	if (m_searchCombo->findText(key) < 0)
		m_searchCombo->insertItem(0, key);
	m_sideBar->m_searchDlg->Search(key);
}

void MainWindow::ShowSearchContextMenu(QPoint pos)
{
	ReleaseShortcutKey(true);

	QPoint globalPos = m_searchCombo->lineEdit()->mapToGlobal(pos);
	QMenu *popup = m_searchCombo->lineEdit()->createStandardContextMenu();
	popup->exec(globalPos);

	ReleaseShortcutKey(false);
	delete popup;
}

void MainWindow::Add3DObject() 
{
	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please close edit window"), tr("Before open objectadd dialog, must finish current item."), MSGBOX_ICONINFO);
		return;
	}
	
	g_app->GetConfig()->m_showObjectAdd=true;

	if(m_objectaddDlg)
		delete m_objectaddDlg;

	m_objectaddDlg = new ObjectAddDlg(this);
	m_objectaddDlg->show();
}

void MainWindow::ObjectPlaceEdit() //asd 2014.4.3
{
	if (!m_rulerDlg->isHidden())
	{
		gu_MessageBox(this, tr("Please Close The Ruler Dialog"), tr("You must close ruler dialog before editing a new one."), MSGBOX_ICONINFO);
		return;
	}

	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please Close The Edit Window"), tr("You must finish with your current item before editing a new one."), MSGBOX_ICONINFO);
		return;
	}

	m_sideBar->m_searchDlg->setSearchStyle(0);

	PlaceMarkMgr *pmMgr = m_renderFrame->GetPlaceMarkMgr();
	pmMgr->SetResultTreeView(m_sideBar->GetActiveView());

	m_objectplaceEditDlg = new ObjectPlaceEditDlg(this);
	m_objectplaceEditDlg->show();

	UpdatePlaceDlg(false);
}

void MainWindow::SetObject() //asd 2014.4.7
{
	if (!m_rulerDlg->isHidden())
	{
		gu_MessageBox(this, tr("Please Close The Ruler Dialog"), tr("You must close ruler dialog before editing a new one."), MSGBOX_ICONINFO);
		return;
	}

	if (BeEditingDlg())
	{
		gu_MessageBox(this, tr("Please Close The Edit Window"), tr("You must finish with your current item before editing a new one."), MSGBOX_ICONINFO);
		return;
	}

	//PlaceMarkMgr *pmMgr = m_renderFrame->GetPlaceMarkMgr();
	//pmMgr->SetResultTreeView(m_sideBar->GetActiveView());

	ObjectPlaceSettingDlg *dlg = new ObjectPlaceSettingDlg(this);
	dlg->show();

	UpdatePlaceDlg(false);
}


#include "buildingsearchdlg.h"
void MainWindow::BuildingFind()
{
	if(!g_DBManager)
	{
		gu_MessageBox(this,tr("Error"),tr("Database is not open"),MSGBOX_ICONCRITICAL);
		return;
	}
	UpdatePlaceDlg(false);
	BuildingSearchDlg* dlg=new BuildingSearchDlg;
	dlg->show();
}

#include "peoplesearchdlg.h"
void MainWindow::PeopleFind()
{
	if(!g_DBManager)
	{
		gu_MessageBox(this,tr("Error"),tr("Database is not open"),MSGBOX_ICONCRITICAL);
		return;
	}
	this->UpdatePlaceDlg(false);

	PeopleSearchDlg *dlg=new PeopleSearchDlg;
	dlg->show();
}

#include "licencedlg.h"
void MainWindow::SetLicence()
{
	LicenceDlg dlg(this);
	dlg.exec();
}

void MainWindow::DisableLicenceAction()
{
	m_ui->actionLicence->setEnabled(false);
}

void MainWindow::ShowBuildingPropDlg(quint64 buildingid)
{
	if(!g_DBManager)
	{
		gu_MessageBox(NULL,("Error"),("Database is not open"),MSGBOX_ICONCRITICAL);
		return ;
	}

	GeoBuildingProperty p;
	if(g_DBManager->GetGeoBuildingData(buildingid,&p))
	{
		m_buildingPropDlg->setBuildingProp(&p);
		m_buildingPropDlg->show();
	}
	else
	{
		gu_MessageBox(NULL,
					  QApplication::translate("CGDM_CommandMgr", "GDMClient", 0, QApplication::UnicodeUTF8),
			          QApplication::translate("CGDM_CommandMgr", "Can not find data of given building in database", 0, QApplication::UnicodeUTF8));
	}
}

void MainWindow::ShowHideStandardToolBar()
{
	Config *config = g_app->GetConfig();
	config->m_showStandardToolbar=!config->m_showStandardToolbar;
	UpdateToolBars();
}

void MainWindow::ShowHideSimulationToolBar()
{
	Config *config = g_app->GetConfig();
	config->m_showSimulationToolbar=!config->m_showSimulationToolbar;
	UpdateToolBars();
}

void MainWindow::ShowHideOverviewMap()
{
	Config *config = g_app->GetConfig();
	config->m_IsShowOverviewMap=!config->m_IsShowOverviewMap;
	UpdateToolTip();
}

void MainWindow::SelectObject()
{
	Config* conf=g_app->GetConfig();
	conf->m_IsSelectObject=!conf->m_IsSelectObject;
}

//2014 2 7 by ugi
void MainWindow::ShowVideoWebView(QString videopath, QString htmlpath,bool indatabase,bool isanimation)
{
	if(indatabase)
	{
		if(!videopath.isEmpty())
			videopath=g_ServerConfig.GetFileDBPath()+videopath;
		if(!htmlpath.isEmpty())
			htmlpath=g_ServerConfig.GetFileDBPath()+htmlpath;
	}

	if(!videopath.isEmpty() &&  !QFile::exists(videopath))
	{
		gu_MessageBox(NULL,tr("Error"),
			tr("Can not find video file:")+videopath,
			MSGBOX_ICONCRITICAL);
		return;
	}

	if(!htmlpath.isEmpty() &&  !QFile::exists(htmlpath))
	{
		gu_MessageBox(NULL,tr("Error"),
			tr("Can not find html file:")+htmlpath,
			MSGBOX_ICONCRITICAL);
		return;
	}

	if(isanimation)
	{
		m_videoWebView->move(0,0);
		m_videoWebView->setEnabled(false);
	}
	else
	{
		//center the origin of mainwindow

		int x=width()-m_videoWebView->width();
		x=x/2;
		
		int y=height()-m_videoWebView->height();
		y=y/2;

		m_videoWebView->move(x,y);
		m_videoWebView->setEnabled(true);
	}

	if(!videopath.isEmpty() && !htmlpath.isEmpty())
		m_videoWebView->setContent(videopath,htmlpath);
	else if(!videopath.isEmpty())
		m_videoWebView->setVideo(videopath,isanimation);
	else
		m_videoWebView->setHtml(htmlpath,isanimation);

	m_videoWebView->show();
}


void MainWindow::Animation()
{
	m_animationDlg->show();
}

void MainWindow::ShowPeopleDetail(QString cardno)
{
	if(!g_DBManager)
	{
		gu_MessageBox(NULL,tr("Error"),tr("Database Error"),MSGBOX_ICONCRITICAL);
		return ;
	}

	if(!m_peopleInfoDlg)
		m_peopleInfoDlg = new PeopleInfoDlg(this);

	m_peopleInfoDlg->readData(cardno.toInt());
	m_peopleInfoDlg->show();
}

void MainWindow::SetupStatusBar()
{ 
	statusBar()->setSizeGripEnabled( true );
#ifdef DEBUG
	 m_statusbBarLeftLabel=new QLabel();
	 m_statusbBarLeftLabel->setIndent( 5 );
	 m_statusbBarLeftLabel->setFixedWidth(300);
	 statusBar()->addPermanentWidget(m_statusbBarLeftLabel);
#endif

	 m_statusbBarCenterLabel=new QLabel();
	 m_statusbBarCenterLabel->setFixedWidth(400);
	 m_statusbBarCenterLabel->setIndent(5);
	 statusBar()->addPermanentWidget(m_statusbBarCenterLabel);

	 m_statusbBarRightLabel=new QLabel();
	 m_statusbBarRightLabel->setFixedWidth(100);
	 m_statusbBarRightLabel->setIndent(5);
	 statusBar()->addPermanentWidget(m_statusbBarRightLabel);
}

QString angle2dir(double angle)
{
	QString res;

	if (angle <= 15 || angle > 345)
		res = MSG_STATUSBAR_NORTH;
	else if (angle > 15 && angle <= 75)
		res = MSG_STATUSBAR_NW;
	else if (angle > 75 && angle <= 105)
		res = MSG_STATUSBAR_WEST;
	else if (angle > 105 && angle <= 165)
		res = MSG_STATUSBAR_SW;
	else if (angle > 165 && angle <= 195)
		res = MSG_STATUSBAR_SOUTH;
	else if (angle > 195 && angle <= 255)
		res = MSG_STATUSBAR_SE;
	else if (angle > 255 && angle <= 285)
		res = MSG_STATUSBAR_EAST;
	else if (angle > 285 && angle <= 345)
		res = MSG_STATUSBAR_NE;

	return res;
}


void MainWindow::UpdateStatusbar()
{
	StatusBarInfo sbi;
	memset(&sbi, 0, sizeof(sbi));
	RenderProxy *render = g_app->GetRenderProxy();
	render->SendMessage(PM_GET_STATUS_BAR, (PrxParam)&sbi);
	
#ifdef DEBUG
	QString leftinfo = QString("Frame:%1fps,Real:%2fps,Mesh:%3,Line:%4")
		.arg(sbi.frames, 2)
		.arg(sbi.real_frames, 2)
		.arg(sbi.meshCount, 5)
		.arg(sbi.lineCount, 5);
	
	m_statusbBarLeftLabel->setText(leftinfo);
#endif

	QString lati, longi, distance, rate;

	sbi.longitude=radian2degree(sbi.longitude);
	sbi.latitude=radian2degree(sbi.latitude);
	switch(g_app->GetConfig()->m_llShowMode)
	{
	case OPTION_LL_10NOTATION:
		longi = QString("%1 %2%3").arg(tr("Longitutde"))
			.arg(sbi.longitude, 11, 'f', 6, QLatin1Char(' ')).arg(QChar(0x00B0));
		lati = QString("%1 %2%3").arg(tr("Latitude"))
			.arg(sbi.latitude, 11, 'f', 6, QLatin1Char(' ')).arg(QChar(0x00B0));
		break;
	case OPTION_LL_DMS:
		longi = gu_Degree2DMS(sbi.longitude, GEO_TYPE_LONGITUDE);
		lati = gu_Degree2DMS(sbi.latitude, GEO_TYPE_LATITUDE);
		break;
	case OPTION_LL_10NOTATION_DM:
		longi = gu_Degree2DM(sbi.longitude, GEO_TYPE_LONGITUDE);
		lati = gu_Degree2DM(sbi.latitude, GEO_TYPE_LATITUDE);
		break;
	default:
		break;
	}

	switch(g_app->GetConfig()->m_distShowMode)
	{
	case OPTION_DISTANCE_FEET_MILE:
		distance = QString("%1 %2ft").arg(tr("Distance"))
			.arg(sbi.distance*100/30.48, 4, 'f', 0, QLatin1Char(' '));
		break;
	case OPTION_DISTANCE_METER_KILO:
		distance = QString("%1 %2m").arg(tr("Distance"))
			.arg(sbi.distance, 4, 10, QLatin1Char(' '));
		break;
	default:
		break;
	}

	switch (g_app->GetConfig()->m_layerTopography)
	{
	case LTM_RATEANGLE:
		rate = QString("%1 %2%3").arg(tr("Gradient"))
			.arg((int)(sbi.rate_angle), 2, 10, QLatin1Char(' ')).arg(QChar(0x00B0));
		break;
	case LTM_RATEPLAN:
		rate = QString("%1 %2").arg(tr("Incline Dir"))
			.arg(angle2dir(sbi.dir_angle), 2, QLatin1Char(' '));
		break;
	}

	QString centerinfo = QString("%1 %2 %3 %4").arg(lati).arg(longi).arg(distance).arg(rate);

	if(sbi.longitude == 0 && sbi.latitude == 0 && sbi.distance == 0)
		m_statusbBarCenterLabel->setText("");
	else
		m_statusbBarCenterLabel->setText(centerinfo);


	QString rightinfo;

	switch(g_app->GetConfig()->m_distShowMode)
	{
	case OPTION_DISTANCE_FEET_MILE:
		if (sbi.height >= 35000)
			rightinfo = QString("%1 %2mi ").arg(tr("Height"))
			.arg(sbi.height/1609.3, 0, 'f', 2);
		else
			rightinfo = QString("%1 %2ft ").arg(tr("Height"))
			.arg(sbi.height*100/30.48, 0, 'f', 0);
		break;
	case OPTION_DISTANCE_METER_KILO:
		if (sbi.height >= 1000)
			rightinfo = QString("%1 %2km ").arg(tr("Height"))
			.arg(sbi.height/1000, 0, 'f', 2);
		else
			rightinfo = QString("%1 %2m ").arg(tr("Height"))
			.arg(sbi.height);
		break;
	default:
		break;
	}

	m_statusbBarRightLabel->setText(rightinfo);

}

void MainWindow::GoTo()
{
	m_goToDialog->show();
}