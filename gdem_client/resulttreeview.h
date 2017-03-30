#ifndef RESULTTREEVIEW_H
#define RESULTTREEVIEW_H

#include <QTreeView>
#include "treeitem.h"

enum ViewType {VT_Search, VT_MyPlace};
class SideBar;
class TreeItemModel;

class ResultTreeView : public QTreeView
{
    Q_OBJECT
public:
	unsigned int GetKey();
	ResultTreeView(int type = VT_Search);
    ~ResultTreeView();
	void SetSideBar(SideBar * bar);
	int InsertTable(PLACEMARK_DATA_LIST *data, QString key );
	void ReplaceTable(PLACEMARK_DATA_LIST *data, QString key, int itemkey);
	bool InitMyPlace();
	void ClearTable();
	TreeItem *GetSearchNodeItem(int itemkey);
	TreeItem *GetSelectItem(bool force = false);
	TreeItem *GetMyPlaceItem();
	TreeItem *GetTempPlaceItem();
	void Inform(bool isAdd = true);
	void GetPlaceMarkInfo(PLACEMARK_DATA &pmd, bool isAdd = true);
	void SetPlaceMarkInfo(const PLACEMARK_DATA &pmd, bool isAdd = true);
	void RemovePlaceMarkInfo(const PLACEMARK_DATA &pmd, bool isAdd = true);

	int OpenFile(const QString &path);
	void CutItem();
	void CopyItem();
	void PasteItem();
	void DeleteItem(bool message = true);
	void DeleteContents();
	void RenameItem();
	void MoveMyPlaces();
	void TempToMyPlaces();
	int SaveAsItem(QString fileName);							// Save selected Item To Kml File
	int OpenMyPlaces();											// Open MyPlaces from KML File
	int SaveMyPlaces();											// Save MyPlaces to   KML File
	int GetFNameToSaveItem (QString& defaultFName);				// Get the default file name to save Kml

	int OpenItem(TreeItem* itemRoot, QString fileName, bool addMyFlag, int row = -1);	// Open Kml File

	void FreeLibkmlMemory();									// Free Libkml Memory manually

	void SaveStartPosition();
	void MoveStartPosition();
	void SetSelectItem(void *item);

	bool IsCutItem();
	bool IsCopyItem();
	bool IsPasteItem();
	bool IsDeleteItem();
	bool IsDeleteContents();
	bool IsSaveMyPlaces();
	bool IsSetProperty();
	bool IsTempToMyPlaces();
	bool IsSearchData();
	bool IsPlaceIcon();
	bool IsPlacePath();
	bool IsPlaceArea();

	static int	GetNewKey();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
	void dropEvent(QDropEvent *event);
	void dragMoveEvent ( QDragMoveEvent * event );
	virtual void keyPressEvent(QKeyEvent *event);

private:
	TreeItemModel *m_model;
	SideBar *m_sideBar;
	int m_viewType;

	PLACEMARK_DATA m_sendData;
	TreeItem *m_storeItem;
	TreeItem *m_startItem;
	unsigned int m_keyStart;

	void Init();
	void SetupTreeView();
	bool IsContainer(TreeItem *item);
	QModelIndex GetMyPlaceIndex();
	QModelIndex GetTemporaryIndex();
	QString GetPageHtml(int totalcount, int startindex, int endindex, QString key, int itemkey, QString &page);
	void SendShowMarkItem(TreeItem *item,  int type );
	void SetCheckedItem(TreeItem *item, int state);
	void UpdateParentItem(TreeItem * item, bool fromself = false );
	void DropInsertRows( const QModelIndex & parent, int row, int col );
	void MovePlace(TreeItem *item);

private slots:
   // void UpdownTreeItem(QModelIndex index);
    void ViewPlace(QModelIndex index);
	void ViewMark(QModelIndex index);
	void InsertedRows ( const QModelIndex & parent, int start, int end );
	void BeforeRemoveRows ( const QModelIndex & parent, int start, int end );
	void RemovedRows ( const QModelIndex & parent, int start, int end );
	void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );

};

#endif // RESULTTREEVIEW_H
