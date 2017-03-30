#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>

class SideBar;
class RenderFrame;
class SearchDlg;
class PlaceDlg;
class RulerDlg;
class BuildingPropDlg;
class VideoWebView;
class AnimationDlg;
class ObjectAddDlg; //asd 2014.4.3
class PeopleInfoDlg; //asd 2014.4.4
class ObjectPlaceEditDlg; //asd 2014.4.5
class MeasureAreaDlg;
class GoToDialog;
class QSplitter;
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	ObjectPlaceEditDlg *getObjectPlaceEditDlg() { return m_objectplaceEditDlg; } //asd 2014.3.31

	inline RenderFrame *GetRenderFrame()  { return m_renderFrame; }
	inline SideBar     *GetSideBar()	  {	return m_sideBar; }

    bool Init();
    void Release();
    void InitUpdate();
	void AfterUpdate();
	void UpdateContourState();
	void UpdateLayer();
	void UpdateRuler();
	void UpdateMeasureArea();
	void UpdatePlaceDlg(bool enable);
	void UpdateTourPlayLine();
	void UpdateFlightSim();
	void UpdateCarSim();
	void UpdateToolTip();
	void UpdateShortCutKey();
	void ShowContextMenu(const QPoint &point, bool sidebar = true);
	void ReleaseShortcutKey(bool release);
	void Search(QString key, int index = 0, int itemkey = 0);
	void DisableLicenceAction();
	void ShowBuildingPropDlg(quint64 buildingid);
	void ShowVideoWebView(const QString videopath,const QString htmlpath,bool indatabase=true,bool isanimation=false);//2014 2 6 by ugi
	void ShowPeopleDetail(QString cardno);
	void UpdateStatusbar();
	bool BeEditingDlg();
protected:
    virtual void closeEvent(QCloseEvent * event);
private slots:
	void MenuUpdate();
	void UpdateLayerMenu();

    // File Menu
    void OpenFile();
	void OpenFilePath(const QString &path);
	void MoveToMyPlace();
    void SaveAsPlaceMarkFile();
	void SaveMyPlace();
	void SaveImage();
    void Print();
    void Exit();

    // Edit Menu
    void Cut();
    void Copy();
	void CopyImageToClip();
    void Paste();
    void Delete();
    void ContentDelete();
    void FindHistoryDelete();
    void Property();

	//Find Menu
	void BuildingFind();
	void PeopleFind();

    // View Menu
	void ShowGrid();
	
    void ViewVGA();
    void ViewSVGA();
    void ViewNTSC();
    void ViewPAL();
    void ViewWideNTSC();
    void ViewWidePAL();
    void View1080pHDTV();
    void View720pHDTV();
    void View480pDVD();
    void ViewLetter();
    void ViewTabloid();
    void ViewPostCard1();
    void ViewPostCard2();
	void FullScreen();

    void ViewAutoNav();
    void ViewAlwaysNav();
    void ViewDialNav();
    void ViewNeverNav();

	void ShowHideStandardToolBar();
	void ShowHideSimulationToolBar();

	void ShowHideStatusbar();
	void ShowHideOverviewMap();
	void ShowHideSidebar();
	void ShowHideBuildingName();

	//Layer Menu
	void SetTerrain();
	void SetContour();
	void SetTexture();
	void SetAltitude();
	void SetGradient();
	void SetIncline();

	//Add Menu
	void AddPlaceFolder();
    void AddPlaceMark();
	void AddPath();
	void AddPolygon();
	void Add3DObject();	
	void ObjectPlaceEdit();	//asd 2014.4.7

    // Setting Menu
    void Option();
	void SetServer();
	void ContourSetting();
	void SetLicence();
	void SetObject(); //asd 2014.4.7
	void SetGeoshapeFormat(); //added by RSH 2013.3.16
	
	//Tool Menu
	void Ruler();
	void MeasureArea();
	void GoTo();
	void SaveStartPosition();
	void Animation();

	//Simulation Menu
	void FlightSimulator();
	void CarSimulator();
	void TourPlayLine();

    // Help Menu
    void HelpManual();
    void About();

	// Search ToolBar
	void SearchPlace();
	void SearchClick();

	// context menu that have same shortcut key
	void ShowSearchContextMenu(QPoint pos);

	void ZoomIn();
	void ZoomOut();
	void FullExtent();
	void Left();
	void Right();
	void Up();
	void Down();
	void SelectObject();

	void AniFileOpen();
	void OpenAniEditor();
	void AniStart();
	void AniStop();
	void OnAniStopped();
	
	void onAnimationToolbarVisibilityChanged(bool visible);

private:
	void SetIcon();
	void CreateMenus();
	bool CreateFrame();
	void DestroyFrame();
	bool CreateToolBars();
	bool CretateStandardToolBar();
	bool CreateSearchToolBar();
	bool CreateSimulationToolBar();
	bool CreateAnimationToolBar();
	void CreateProcessMng();
	void Reposition();
	void RewidthSideBar();
	void ResizeRenderFrame(int size);
	void UpdateMenus();
	void UpdateToolBars();
	void UpdateSideBar(bool show);
	void UpdateSearchToolBar(bool show);
	void UpdateGrid();
	void UpdateMenuWithSim(bool enable);
	void SetupStatusBar();

	Ui::MainWindow *m_ui;
	
	QSplitter		*m_splitter;
	SideBar			*m_sideBar;
	RenderFrame		*m_renderFrame;

	QToolBar		*m_simulationToolBar;
	QToolBar		*m_searchToolBar;
	QToolBar        *m_standardToolBar;
	QToolBar        *m_AnimationToolBar;
	QComboBox		*m_searchCombo;

	RulerDlg			*m_rulerDlg;
	BuildingPropDlg		*m_buildingPropDlg;
	VideoWebView		*m_videoWebView; 
	AnimationDlg		*m_animationDlg;
	ObjectAddDlg		*m_objectaddDlg; //asd
	PeopleInfoDlg		*m_peopleInfoDlg; //asd add 2014. 2 13
	ObjectPlaceEditDlg	*m_objectplaceEditDlg; //asd add 2014.4.1
	MeasureAreaDlg      *m_measureAreaDlg;
	GoToDialog          *m_goToDialog;
	
	//ToolBar
	QAction*        m_standardToolBarZoomIn;
	QAction*        m_standardToolBarZoomOut;
	QAction*        m_standardToolBarFullExtent;
	QAction*        m_standardToolBarLeft;
	QAction*        m_standardToolBarRight;
	QAction*        m_standardToolBarUp;
	QAction*        m_standardToolBarDown;
	
	QAction*        m_standardToolBarSelectObject;
	
	QAction*        m_searchToolBarSearch;

	QAction*        m_AniOpen;
	QAction*        m_OpenAniEditor;
	QAction*        m_AniStart;
	QAction*        m_AniStop;
#ifdef DEBUG
	QLabel*         m_statusbBarLeftLabel;
#endif
	QLabel*         m_statusbBarCenterLabel;
	QLabel*         m_statusbBarRightLabel;
};

#endif // MAINWINDOW_H
