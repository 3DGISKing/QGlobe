#include "layerstreeview.h"
#include "layersdlg.h"

LayersTreeView::LayersTreeView(LayersDlg* parent)
:QTreeView((QWidget*)parent)
{
	
}

LayersTreeView::~LayersTreeView()
{
}

void LayersTreeView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space)
	{
		QModelIndex index = currentIndex();

		LayersDlg *dlg=(LayersDlg*) parentWidget();

		dlg->KeyPressEventOfTree(index);
	}
	else
	{
		QTreeView::keyPressEvent(event);
	}
}

void LayersTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
	m_DblClickedPos=event->pos();
	QTreeView::mouseDoubleClickEvent(event);
}