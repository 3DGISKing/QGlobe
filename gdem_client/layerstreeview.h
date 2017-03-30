#ifndef LAYERSTREEVIEW_H
#define LAYERSTREEVIEW_H

#include <QtGui/QTreeView>

class LayersDlg;

class LayersTreeView : public QTreeView
{
    Q_OBJECT
public:
	LayersTreeView(LayersDlg* parent);
	~LayersTreeView();
	QPoint GetDblClickedPoint(){return m_DblClickedPos;}

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
private:
	QPoint              m_DblClickedPos;
};

#endif //LAYERSTREEVIEW_H
