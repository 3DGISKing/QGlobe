#ifndef __RENDERVIEW_H__
#define __RENDERVIEW_H__

#include <QGLWidget>
#include <QMouseEvent>

#include "geoEngine.h"

class SceneManager;

class RenderView : public QGLWidget
{
	Q_OBJECT
public:
    RenderView(QWidget *parent=0);
   ~RenderView();

public:
	void SetRenderSceneMgr(SceneManager* sceneMgr) { _RenderSceneMgr = sceneMgr; }
	void OnInitializeGL();
	void OnResizeGL(int width, int height);

	void SetPolygonTexture(QPixmap* pixmap);

private:
	bool InitializeRenderEngine();
	bool renderScene();

private:
	SceneManager* _RenderSceneMgr;
	QImage _textureImage;
	bool _updateTexture;

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void timerEvent(QTimerEvent *event);
	virtual void paintGL();
	virtual void mouseMoveEvent(QMouseEvent *);
};

#endif // __RENDERVIEW_H__
