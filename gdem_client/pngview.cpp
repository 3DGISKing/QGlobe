#include "pngview.h"
#include <QtGui/QHeaderView>
PngView::PngView(QWidget *parent)
	: QTableWidget(parent)
{

	setColumnCount(3);

	setShowGrid(false);
	m_IconSizeX=30;
	m_IconSizeY=30;

	setIconSize(QSize(m_IconSizeX,m_IconSizeY));
	setSelectionMode(QAbstractItemView::SingleSelection);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	verticalHeader()->setVisible(false);
	horizontalHeader()->setVisible(false);
}

PngView::~PngView()
{

}
