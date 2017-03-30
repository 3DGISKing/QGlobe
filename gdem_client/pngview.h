#ifndef ICON_VIEW_H
#define ICON_VIEW_H

#include <QtGui/QTableWidget>

class PngView :public QTableWidget
{
	Q_OBJECT
public:
	PngView(QWidget* parent=0);
	~PngView();

private:
	int m_IconSizeX,m_IconSizeY;
};

#endif