#include "test_polymesh.h"
#include "renderview.h"
#include "SceneManager.h"

Test_PolyMesh::Test_PolyMesh(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	_renderView = NULL;
	_sceneManager = new SceneManager;
}

Test_PolyMesh::~Test_PolyMesh()
{
	if(_sceneManager)
		delete _sceneManager;
}

bool Test_PolyMesh::initWindow()
{
	if(!_renderView)
	{
		_renderView = new RenderView(this);
		if (_renderView == NULL)
			return false;

		_renderView->SetRenderSceneMgr(_sceneManager);
		setCentralWidget(_renderView);
	}

	return true;
}