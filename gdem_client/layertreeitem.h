#ifndef LAYERTREEITEM_H
#define LAYERTREEITEM_H

#include <QtGui/QStandardItem>

class Layer;

class LayerTreeItem : public QStandardItem
{
public:
	LayerTreeItem(QString text);
	LayerTreeItem(QString text,QString iconfilename);
	LayerTreeItem(Layer* layer);

	~LayerTreeItem();
	Layer*                  GetLayer();
	void                    SetCheckState(Qt::CheckState state);
	inline void             SetExclusiveChildItem (LayerTreeItem* item);
	void                    PropagateStateToExclusiveChild(Qt::CheckState state);
	void                    PropagateStateToAllChilds(Qt::CheckState state);
	void                    OnClicked();
	void                    SetState(Qt::CheckState state);
	inline void             SetCheckedItemMustBeExist(bool val){m_checkedItemMustBeExist=val;}
	inline bool             GetCheckedItemMustBeExist() {return m_checkedItemMustBeExist;}
	void                    PropagateEnabledState(bool state);

private:
	LayerTreeItem*          Root();
	inline LayerTreeItem*   GetParent()  {return (LayerTreeItem*)parent();}
	inline LayerTreeItem*   ExclusiveChildItem() { return m_exclusiveChilditem;}

	void                    PropagateExclusiveChildToParent();
	void                    SetExclusiveCheckState(Qt::CheckState state);
	void                    PropagateStateToParent();

	//this member must be one of childs of this item
	LayerTreeItem*          m_exclusiveChilditem;
	bool                    m_checkedItemMustBeExist;
};

#endif