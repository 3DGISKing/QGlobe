#ifndef TEST_POLYMESH_H
#define TEST_POLYMESH_H

#include <QtGui/QMainWindow>
#include "ui_test_polymesh.h"

class RenderView;
class SceneManager;

class Test_PolyMesh : public QMainWindow
{
	Q_OBJECT

public:
	Test_PolyMesh(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Test_PolyMesh();

	bool initWindow();

	RenderView* getRenderView() { return _renderView; }

private:
	Ui::Test_PolyMeshClass ui;

protected:
	RenderView*	_renderView;
	SceneManager*	_sceneManager;
};

#endif // TEST_POLYMESH_H
