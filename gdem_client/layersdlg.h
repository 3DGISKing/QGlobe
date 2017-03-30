#ifndef LAYERSDLG_H
#define LAYERSDLG_H

#include <QtGui/QDialog>
#include <QtCore/QBasicTimer>

class SideBar;
class LayerTreeItem;
class LayersTreeView;

namespace Ui {
	class LayersDlg;
}

class LayersDlg : public QDialog 
{
	Q_OBJECT
public:
	LayersDlg(QWidget *parent = 0);
	~LayersDlg();
	bool Init();
	void Release();
	void UpdateDialog();
	void KeyPressEventOfTree(QModelIndex &index);

protected:
	virtual void reject();
	void timerEvent(QTimerEvent *event);

private slots:
	void OnItemClicked(QModelIndex index);
	void OnItemDoubleClicked(QModelIndex index);
private:
	bool CreateFrame();
	void DestroyFrame();
	bool AddItems();
	bool AddTerrainItem();
	bool AddRasterItems();
	bool AddRenderItem();
	bool AddAdminCenters();
	bool AddSocialPlaceNames();
	bool AddPlaceNames();
	bool AddSeaPlaceNames();
	bool AddAdminBoundary();
	bool AddTransportNetwork();
	bool AddWaterNetwork();
	bool AddLanduseBoundary();
	bool Add3dObject();
	bool AddRevolItems();

	LayerTreeItem* RootItem();
	void SetEnabledState(LayerTreeItem* item,int level);

private:
	Ui::LayersDlg *m_ui;
	SideBar				*m_parent;
	LayersTreeView		*m_layersView;
	QVBoxLayout			*m_frameLayout;

	QStandardItemModel	*m_itemModel;
	
	LayerTreeItem*      m_TerrainItem; 
	LayerTreeItem*      m_ImageItem; 
	LayerTreeItem*      m_RasterItem; 

	QBasicTimer		    m_Timer;
};

#endif // LAYERSDLG_H
