#include "test_polymesh.h"
#include "textureview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Test_PolyMesh w;

	w.initWindow();
	w.show();

	TextureView textureView;
	textureView.setRenderView(w.getRenderView());
	textureView.show();

	return a.exec();
}
