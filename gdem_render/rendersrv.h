#ifndef RENDERSRV_H
#define RENDERSRV_H

#include "../gdem_libmain/GDM_Interface.h"
#include "../gdem_common/gdemclientbase.h"
#include "render_global.h"
#include "../gdem_sim/GDM_SimMgr.h"

extern GDM_RENDER_HANDLE	g_pRender;

class QGLWidget;
class RenderView;
class CameraController;
class GDM_SceneManager;
class DataSrv;
class CGDM_DataMgr;
class CGDM_CommandMgr;
class CGDM_SimMgr;
class CGIS_Doc;
class AnimationMgr;

class RENDER_EXPORT RenderSrv
{
public:
    RenderSrv();
    ~RenderSrv();

    void SetDataSrv(DataSrv *dataSrv);

    bool Init();

	bool Paint(GDM_RENDER_INFO *renderInfo);

	void MoveForward(int speed, int nFrom);
    void ChangedViewSize(int w, int h);
    void MoveView(int x, int y);
    void RotateView(int direct);
    void TiltView(int direct);
	void LookView(int x , int y);
	void ResetTilt();
	void ResetAll();
	void StopView();
	void ResetView();
	void MoveToPlace(PLACEMARK_DATA* placeData, int count);
	void FastMoveToPlace(PLACEMARK_DATA* placeData);

	void GetCompassAngle(double* angle);
	void ChangedCompassAngle(double* angle) ;
	void InitializeGL(QGLWidget *view);
	void SetRenderView(QGLWidget* view);

	/* Cache Processing */
	void MemCacheClear();
	void FileCacheClear();
	void SimpleBuildingCacheClear();
	void ChangeCacheSize(int nMemSize, int nDiskSize);

	/* Label info setting */
	void ChangeLabelSize(int nSize);

	void SetContourState(bool state);
	void SetTerrainLayerState(bool state);
	void SetLayerTextureState(int state);
	void SetContourOptin(ContourStateInfo* contourOptin);
	void SetGridOption(int gridView, int gridMode);

	bool GetRenderImage(QImage* image);

	void GetPlacemarkData(PLACEMARK_DATA* data, bool blPos, bool blCamera);

	void SetTopographyQuality(unsigned int quality);
	void SetDistShowMode(unsigned int distviewmode);
	void SetHeightMagnify(double rate);
	void OnChangeMoveToSpeed(double* speed);

	/* Font Info */
	void InitializeFontInfo(QString *korName, QString *engName);
	void SetFontSize(int size);

	void OnChangeServer();

	CGDM_CommandMgr* GetCommandMgr(){return m_pCmdMgr;}

	void StartSimulation (GDM_SIM_TYPE simType, bool restart = false);
	void EndSimulation();
	void ShowLengthArea(bool blShow);
	void DataThreadInit();
	void ClearRasterCache();

	void SetSunLightEnabled(bool* enabled);
	void SetSunLightColor(QColor* color);
	void SetAmbientColor(QColor* color);
	void SetSunPosition(double* pos);
	void SetLightAttenuation(double c,double l,double q);
	void SetCameraFOV(double fov);
public:
    RenderView		*m_pView;
	CameraController*m_pCameraController;
	bool			m_blFirstPaint;
	GDM_SceneManager *m_pSceneMgr;
	CGDM_DataMgr	*m_pGDMDataMgr;
	CGDM_CommandMgr *m_pCmdMgr;
	CGDM_SimMgr		*m_pSimMgr;
	CGIS_Doc		*m_pGisDoc;
	AnimationMgr    *m_pAnimationMgr; 
private:
	GDM_ERROR_NO	OnRender(GDM_RENDER_INFO *renderInfo);
};

#endif // RENDERSRV_H
