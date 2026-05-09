#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <QOpenGLWidget>

#include "qglobe_scene_global.h"
#include "geoEngine.h"
#include "qglobeclientbase.h"
#include "QGlobe_Coord3D.h"
#include "QGlobe_Camera.h"

class QGlobe_SceneManager;

class QGLOBE_SCENE_EXPORT RenderView : public QOpenGLWidget
{
public:
    RenderView(QWidget *parent=0);
   ~RenderView();

public:
	void SetRenderSceneMgr(QGlobe_SceneManager* sceneMgr) { _RenderSceneMgr = sceneMgr; }
	void OnInitializeGL();
	void OnResizeGL(int width, int height);
	void OnPaint(QGlobe_RENDER_INFO *renderInfo);

	bool GetFrameBuffer(QImage *qImage);
	void UpdateCameraInfo(QGlobe_Coord3D& coord, QGlobe_ViewFrustum& viewFrustum);

private:
	bool InitializeRenderEngine();

private:
	QGlobe_SceneManager* _RenderSceneMgr;

};

#endif // RENDERVIEW_H
