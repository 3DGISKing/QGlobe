#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <QGLWidget>

#include "gdem_scene_global.h"
#include "geoEngine.h"
#include "gdemclientbase.h"
#include "GDM_Coord3D.h"
#include "GDM_Camera.h"

class GDM_SceneManager;

class GDEM_SCENE_EXPORT RenderView : public QGLWidget
{
public:
    RenderView(QWidget *parent=0);
   ~RenderView();

public:
	void SetRenderSceneMgr(GDM_SceneManager* sceneMgr) { _RenderSceneMgr = sceneMgr; }
	void OnInitializeGL();
	void OnResizeGL(int width, int height);
	void OnPaint(GDM_RENDER_INFO *renderInfo);

	bool GetFrameBuffer(QImage *qImage);
	void UpdateCameraInfo(GDM_Coord3D& coord, GDM_ViewFrustum& viewFrustum);

private:
	bool InitializeRenderEngine();

private:
	GDM_SceneManager* _RenderSceneMgr;

};

#endif // RENDERVIEW_H
