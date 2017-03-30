#include <QtGui>
#include <QtGlobal>
#include "resulttreeview.h"
#include "treeitemmodel.h"
#include "treeitem.h"
#include "treeitemdelegate.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "proxy.h"
#include "placemarkdlg.h"
#include "placefolderdlg.h"
#include "sidebar.h"
#include "msgdata.h"
#include "guiuty.h"
#include "gdemclient.h"
#include "renderfrm.h"
#include "GIS_Doc.h"
#include "GIS_PlaceIcon.h"

// for Kml
#include "kmlmanagerclient.h"

enum ModifyType	{ MT_Remove, MT_Add, MT_Modify, MT_Uncheck , MT_Check};
#define SEARCH_NO_LINK "<span style=\" font-size:8pt; color:#ff0000;\">%1"
#define SEARCH_LINK "<a href=\"http://pic.com/maps?q=%1&start=%2^itemkey=%3\"><span style=\" text-decoration: underline; color:#0000ff;\">%4"
#define SPACE_TAG	"&nbsp;"
#define NEXT_PAGE "&gt;"
#define URL_MARK	"/"

static int KmlTreeToTreeView(KmlTreeItem* kmlItemMy, TreeItem* itemParent,
							 bool addMyFlag, int row = -1); // Add Kml tree to Treeview
static int TreeViewToKmlTree(TreeItem* itemMy, KmlTreeItem* kmlItemParent);	// Create Kml tree from Treeview
static int SaveAsItemRoot(QString fileName, TreeItem *itemRoot);			// Create Kml tree from TreeView, Create kml, Save it
static void GetMyPlacesFName(QString& fname);								// Get the MyPlceas file name
static void GetMyPlacesTempFName(QString& fname);							// Get the MyPlceas Temp file name
static void GetMyPlacesBackupFName(QString& fname);							// Get the MyPlceas Backup file name

static unsigned int Key(bool search = false);

static unsigned int s_key = 0;
static unsigned int Key(bool search)
{
	s_key++;
	unsigned int key = s_key;
	if (search)
		return key;
	int count = sizeof(unsigned int) * 8 - 1;
	unsigned int prefix = (0x1 << count);
	key += prefix;
	return key;
}

int	ResultTreeView::GetNewKey()
{
	return Key();
}
/*
 *   public
 */
unsigned int ResultTreeView::GetKey()
{
	bool search = ( m_viewType == VT_Search ? true : false);
	return Key(search);
}

ResultTreeView::ResultTreeView(int type)
	:QTreeView()
{
	m_viewType = type;
    Init();
}
ResultTreeView::~ResultTreeView()
{
	if(m_model)
		delete m_model;
}

void ResultTreeView::SetSideBar(SideBar *bar)
{
	m_sideBar = bar;
}

TreeItem* ResultTreeView::GetSearchNodeItem(int itemkey)
{
	TreeItem *parentItem = static_cast<TreeItem*>(m_model->invisibleRootItem());
	if (!parentItem)
		return 0;

	int childcount = parentItem->rowCount();
	TreeItem *itemChild;
	for (int i = 0; i < childcount; i++)
	{
		itemChild = (TreeItem *)parentItem->child(i);

		if (itemChild != NULL )
		{
			int key = itemChild->GetItemData().key;
			if (itemkey == key)
				return itemChild;
		}
	}
	return NULL;
}

#if 1
void ResultTreeView::ReplaceTable(PLACEMARK_DATA_LIST *data, QString key, int itemkey)
{
	QString folder = key;
	PLACEMARK_DATA itemHeaderData;
	int childcount;

	memset(&itemHeaderData, 0, sizeof(PLACEMARK_DATA));
	itemHeaderData.key		= itemkey;
	itemHeaderData.itemtype = IT_FOLDER;
	itemHeaderData.showmark = SMT_SHOW;
	folder += QString("(%1 - %2)").arg(data->start_index + 1).arg(data->end_index + 1);
	gu_qstring2ushort(folder, itemHeaderData.name, PMD_STR_MAX_SIZE);

	TreeItem *itemHeader = GetSearchNodeItem(itemkey);
	if (!itemHeader)
		return;
	itemHeader->SetItemData(itemHeaderData);

	if (itemHeader->hasChildren())
	{
		childcount = itemHeader->rowCount();
		itemHeader->removeRows(0, childcount);
	}

	setExpanded(m_model->indexFromItem(itemHeader), true);

	int start = (int)data->start_index;
	int end = (int)data->end_index;
	PLACEMARK_DATA *sendSearchData = NULL;
	if (end >= start)
	{
		sendSearchData = (PLACEMARK_DATA *) malloc(sizeof(PLACEMARK_DATA) * (end-start+1));
		memset(sendSearchData, 0, sizeof(PLACEMARK_DATA) * (end-start+1));
	}

	PLACEMARK_DATA itemdata;
	PLACEMARK_DATA senddata;
	for (int i = start; i < end+1; ++i)
	{
		memcpy(&itemdata, (data->data_list)+(i-start), sizeof(PLACEMARK_DATA));
		senddata = itemdata;
		gu_MakeRenderData(&senddata);
		memcpy(sendSearchData+(i-start), &senddata, sizeof(PLACEMARK_DATA));

		// set key
		itemdata.key = Key(true);

		TreeItem *row = new TreeItem(itemdata);

		// add gis node
		CGIS_PlaceIcon *pNode = new CGIS_PlaceIcon();
		pNode->SetKey(itemdata.key);
		pNode->m_nGisType = E_GISNODE_TYPE_PLACESEARCH;
		pNode->SetPt(itemdata.longitude * PAI / 180, itemdata.latitude * PAI / 180, itemdata.altitude);
		memcpy(pNode->m_sName, itemdata.name, PMD_STR_MAX_SIZE);
		gdm_GetGISDoc()->AddNode(pNode);
		// end

		itemHeader->appendRow(row);
		if (i == end)
		{
			//PLACEMARK_DATA pagedata = {0, 0, 0, 0,L"", L"", IT_PAGE, 0, 0, 0, 0, 0, 2};
			PLACEMARK_DATA pagedata;
			memset(&pagedata, 0, sizeof(PLACEMARK_DATA));
			pagedata.itemtype = IT_PAGE;
			pagedata.showmark = SMT_SHOW;
			QString pagetext("");
			QString pagehtml("");
			pagehtml = GetPageHtml(data->total_count, start, end, key, itemHeaderData.key, pagetext);
			if (pagehtml.isEmpty())
				break;
			//pagetext.toWCharArray(pagedata.name);
			gu_qstring2ushort(pagetext, pagedata.name, PMD_STR_MAX_SIZE);
			TreeItem *page = new TreeItem(pagedata, pagehtml);
			itemHeader->appendRow(page);
			openPersistentEditor(m_model->indexFromItem(page));
		}
	}

	if (sendSearchData)
	{
		RenderProxy *rendersrv = g_app->GetRenderProxy();
		if (rendersrv)
			rendersrv->SendMessage(PM_SEARCH_PLACE, (PrxParam) sendSearchData, (PrxParam)end-start+1);
		free(sendSearchData);
		sendSearchData = NULL;
	}
}
#endif

int ResultTreeView::InsertTable(PLACEMARK_DATA_LIST *data, QString key)
{
	collapseAll();
	QString folder = key;

    TreeItem *parentItem = static_cast<TreeItem*>(m_model->invisibleRootItem());
	if (!parentItem)
		return 0;

	PLACEMARK_DATA itemHeaderData;
	memset(&itemHeaderData, 0, sizeof(PLACEMARK_DATA));
	itemHeaderData.key		= Key(true);
	itemHeaderData.itemtype = IT_FOLDER;
	itemHeaderData.showmark = SMT_SHOW;
	folder += QString("(%1 - %2)").arg(data->start_index + 1).arg(data->end_index + 1);
	//folder.toWCharArray(itemHeaderData.name);
	gu_qstring2ushort(folder, itemHeaderData.name, PMD_STR_MAX_SIZE);

	TreeItem *itemHeader = new TreeItem(itemHeaderData);
	if (!itemHeader)
		return 0;

	parentItem->insertRow(0, itemHeader);
	setExpanded(m_model->indexFromItem(itemHeader), true);

	int start = (int)data->start_index;
	int end = (int)data->end_index;
	PLACEMARK_DATA *sendSearchData = NULL;
	if (end >= start)
	{
		sendSearchData = (PLACEMARK_DATA *) malloc(sizeof(PLACEMARK_DATA) * (end-start+1));
		memset(sendSearchData, 0, sizeof(PLACEMARK_DATA) * (end-start+1));
	}

	PLACEMARK_DATA itemdata;
	PLACEMARK_DATA senddata;
	for (int i = start; i < end+1; ++i)
	{
		memcpy(&itemdata, (data->data_list)+(i-start), sizeof(PLACEMARK_DATA));
		senddata = itemdata;
		gu_MakeRenderData(&senddata);
		memcpy(sendSearchData+(i-start), &senddata, sizeof(PLACEMARK_DATA));

		// set key
		itemdata.key = Key(true);

		TreeItem *row = new TreeItem(itemdata);

		// add gis node
		CGIS_PlaceIcon *pNode = new CGIS_PlaceIcon();
		pNode->SetKey(itemdata.key);
		pNode->m_nGisType = E_GISNODE_TYPE_PLACESEARCH;
		pNode->SetPt(itemdata.longitude * PAI / 180, itemdata.latitude * PAI / 180, itemdata.altitude);
		memcpy(pNode->m_sName, itemdata.name, PMD_STR_MAX_SIZE);
		gdm_GetGISDoc()->AddNode(pNode);
		// end

		itemHeader->appendRow(row);
		if (i == end)
		{
			//PLACEMARK_DATA pagedata = {0, 0, 0, 0,L"", L"", IT_PAGE, 0, 0, 0, 0, 0, 2};
			PLACEMARK_DATA pagedata;
			memset(&pagedata, 0, sizeof(PLACEMARK_DATA));
			pagedata.itemtype = IT_PAGE;
			pagedata.showmark = SMT_SHOW;
			QString pagetext("");
			QString pagehtml("");
			pagehtml = GetPageHtml(data->total_count, start, end, key, itemHeaderData.key, pagetext);
			if (pagehtml.isEmpty())
				break;
			//pagetext.toWCharArray(pagedata.name);
			gu_qstring2ushort(pagetext, pagedata.name, PMD_STR_MAX_SIZE);
			TreeItem *page = new TreeItem(pagedata, pagehtml);
			itemHeader->appendRow(page);
			openPersistentEditor(m_model->indexFromItem(page));
		}
	}

	if (sendSearchData)
	{
		RenderProxy *rendersrv = g_app->GetRenderProxy();
		if (rendersrv)
			rendersrv->SendMessage(PM_SEARCH_PLACE, (PrxParam) sendSearchData, (PrxParam)end-start+1);
		free(sendSearchData);
		sendSearchData = NULL;
	}

	return itemHeaderData.key;
}

bool ResultTreeView::InitMyPlace()
{
	TreeItem *parentItem = static_cast<TreeItem*>(m_model->invisibleRootItem());
	PLACEMARK_DATA mydata;
	memset(&mydata, 0, sizeof(PLACEMARK_DATA));
	mydata.itemtype = IT_DOC;

	QString title1(tr("My Places"));
	
	gu_qstring2ushort(title1, mydata.name, PMD_STR_MAX_SIZE);
	TreeItem *itemMyData = new TreeItem(mydata);
	if (!itemMyData)
		return false;

	itemMyData->setDragEnabled(false);
	parentItem->appendRow(itemMyData);

	//PLACEMARK_DATA tempdata = {0, 0, 0, 0,L"", L"", IT_FOLDER};
	PLACEMARK_DATA tempdata;
	memset(&tempdata, 0, sizeof(PLACEMARK_DATA));
	tempdata.itemtype = IT_FOLDER;

	QString title2(tr("Temporary Places"));
	
	gu_qstring2ushort(title2, tempdata.name, PMD_STR_MAX_SIZE);
	TreeItem *itemTemp = new TreeItem(tempdata);
	if (!itemTemp)
		return false;

	itemTemp->setDragEnabled(false);
	parentItem->appendRow(itemTemp);

	return true;
}

void ResultTreeView::ClearTable()
{
    m_model->clear();
	m_sideBar->ClearSearch();

}

TreeItem * ResultTreeView::GetSelectItem(bool force)
{
	QModelIndex index = this->selectionModel()->currentIndex();
	if (!index.isValid())
	{
		if (m_viewType == VT_MyPlace && force == true)
			index = GetMyPlaceIndex();
		else
			return NULL;
	}
	
	TreeItem *item =static_cast<TreeItem*> (m_model->itemFromIndex(index));
	if (hasFocus())
		return item;
	else
		return NULL;
}

TreeItem * ResultTreeView::GetMyPlaceItem()
{
	QModelIndex index = GetMyPlaceIndex();
	TreeItem *item = NULL;
	if (index.isValid())
	{
		item =static_cast<TreeItem*> (m_model->itemFromIndex(index));
	}
	return item;
}

TreeItem * ResultTreeView::GetTempPlaceItem()
{
	QModelIndex index = GetTemporaryIndex();
	TreeItem *item = NULL;
	if (index.isValid())
	{
		item =static_cast<TreeItem*> (m_model->itemFromIndex(index));
	}
	return item;
}

void ResultTreeView::Inform(bool isAdd)
{
	m_storeItem = GetSelectItem(true);
	if (!isAdd && !m_storeItem)
		return;

	PLACEMARK_DATA itemdata;
	if (!isAdd)
		itemdata = m_storeItem->GetItemData();

	itemdata.key = GetKey();

	if (!m_storeItem)
	{
		if (m_viewType == VT_MyPlace)
		{
			m_storeItem = GetMyPlaceItem();
		}
		else
		{
#ifdef DEBUG
			qDebug("Create Folder must be modify");
#endif
			return;
		}
	}

	if (isAdd && !IsContainer(m_storeItem))
		m_storeItem = static_cast<TreeItem*>(m_storeItem->parent());
}

void ResultTreeView::GetPlaceMarkInfo(PLACEMARK_DATA &pmd, bool isAdd)
{
	if (isAdd)
		m_storeItem = GetSelectItem(true);
	else
		m_storeItem = GetSelectItem();

	if (!isAdd && !m_storeItem)
		return;

	PLACEMARK_DATA itemdata = pmd;

	if (!isAdd)
		itemdata = m_storeItem->GetItemData();
	else
		itemdata.key = GetKey();

	itemdata.showmark = SMT_SHOW;
	pmd = itemdata;
	if (!isAdd)
		m_sideBar->ModifyPlaceMark(itemdata);
	else
		m_sideBar->AddPlaceMark(itemdata);

	if (!m_storeItem)
	{
		if (m_viewType == VT_MyPlace)
		{
			m_storeItem = GetMyPlaceItem();
		}
		else
		{
#ifdef DEBUG
			qDebug("Create Folder must be modify");
#endif
			return;
		}
	}

	if (isAdd && !IsContainer(m_storeItem))
		m_storeItem = static_cast<TreeItem*>(m_storeItem->parent());
}

void ResultTreeView::SetPlaceMarkInfo(const PLACEMARK_DATA &pmd, bool isAdd)
{
	PLACEMARK_DATA itemdata = pmd;
	if (isAdd)
	{
		TreeItem *newitem = new TreeItem(itemdata);
		m_storeItem->appendRow(newitem);
		itemdata = newitem->GetItemData();
		itemdata.item = newitem;
		if(pmd.key != itemdata.key)
		{
			m_sideBar->RemovePlaceMark(pmd);
			m_sideBar->AddPlaceMark(itemdata);
		}
		else
		{
			m_sideBar->ModifyPlaceMark(itemdata);
		}
	}
	else
	{
		m_storeItem->SetItemData(itemdata);
		if (itemdata.itemtype == IT_SEARCH || itemdata.itemtype == IT_PLACEMARK || itemdata.itemtype == IT_PATH || itemdata.itemtype == IT_POLYAREA)
			SendShowMarkItem(m_storeItem, MT_Modify);
	}
}

void ResultTreeView::RemovePlaceMarkInfo(const PLACEMARK_DATA &pmd, bool isAdd )
{
	if (isAdd)
		m_sideBar->RemovePlaceMark(pmd);
	else
		m_sideBar->ModifyPlaceMark(pmd);
}

int ResultTreeView::OpenFile(const QString &path)
{
	TreeItem *item = GetTempPlaceItem();
	if (!item)
		return -1;

	int result = OpenItem(item, path, true);
	if (result != 0)
		return result;

	int row = item->rowCount();
	TreeItem *openitem = static_cast<TreeItem*>(item->child(row-1));
	if (!openitem)
		return result;

	QModelIndex index = m_model->indexFromItem(openitem);
	if (index.isValid())
	{
		setCurrentIndex(index);
		MovePlace(openitem);
	}
	return result;
}

void ResultTreeView::CutItem()
{
	CopyItem();
	if (IsDeleteItem())
		DeleteItem(false);
}

void ResultTreeView::CopyItem()
{
	QClipboard *cb = QApplication::clipboard();
	QModelIndexList indexes = this->selectionModel()->selectedIndexes();

	if (indexes.count() > 0)
	{
		QMimeData *data = m_model->mimeData(indexes);
		if (!data)
			return;
		cb->setMimeData(data);
	}
}

void ResultTreeView::PasteItem()
{
	if (!IsPasteItem())
		return;

	QModelIndex index = this->selectionModel()->currentIndex();
	if (!index.isValid())
		return;

	TreeItem * item = static_cast<TreeItem*> (m_model->itemFromIndex(index));
	PLACEMARK_DATA data = item->GetItemData();

	int row = 0;
	if (data.itemtype != IT_DOC && data.itemtype != IT_FOLDER)
	{
		row = index.row() + 1;
		index = index.parent();
	}

	QClipboard *cb = QApplication::clipboard();
	const QMimeData *mimeData = cb->mimeData();
	m_model->dropMimeData(mimeData, Qt::MoveAction, row, 0, index);
	DropInsertRows(index, row, 0);
}

void ResultTreeView::DeleteItem(bool message)
{
	if (!IsDeleteItem())
		return;

	QModelIndex index = this->selectionModel()->currentIndex();
	if (!index.isValid())
		return;
	TreeItem * item = static_cast<TreeItem*> (m_model->itemFromIndex(index));
	QString contents;
	if (item->hasChildren())
		contents = tr("Are you sure you want to delete? This will also remove all descendants.");
	else
		contents = tr("Are you sure you want to delete?");

	QString itemName = QString::fromUtf16(item->GetItemData().name);
	contents = QString("%1%2%3 %4").arg(QChar(0x226A))
									.arg(itemName)
									.arg(QChar(0x226B))
									.arg(contents);

	if (message)
	{
		int result = gu_MessageBox(this, tr("GDEM Client"), contents,
				  MSGBOX_ICONQUESTION|MSGBOX_IDYES|MSGBOX_IDNO);
		if ( result == MSGBOX_IDNO)
			return;
	}
	m_model->removeRow(index.row(), index.parent());
}

void ResultTreeView::DeleteContents()
{
	if (!IsDeleteContents())
		return;

	TreeItem *item = GetSelectItem();
	if (!item || !item->hasChildren())
		return;

	if (MSGBOX_IDNO == gu_MessageBox(this, tr("GDEM Client"), tr("Do yo want to delete all the contents?"),
				  MSGBOX_ICONQUESTION|MSGBOX_IDYES|MSGBOX_IDNO))
		return;

	int childcount = item->rowCount();
	item->removeRows(0, childcount);
}

void ResultTreeView::RenameItem()
{
	TreeItem *item = GetSelectItem();
	if (!item)
		return;
	item->setEditable(true);
}

void ResultTreeView::MoveMyPlaces()
{
	if (m_viewType != VT_MyPlace)
		return;
	QModelIndex indexMyPlace = GetMyPlaceIndex();
	if (!indexMyPlace.isValid())
		return;
	TreeItem *item = static_cast<TreeItem*> (m_model->itemFromIndex(indexMyPlace));
	if (!item)
		return;

	int row = item->rowCount();
	QClipboard *cb = QApplication::clipboard();
	const QMimeData *mimeData = cb->mimeData();
	m_model->dropMimeData(mimeData, Qt::MoveAction, row, 0, indexMyPlace);
	DropInsertRows(indexMyPlace, row, 0);

}

void ResultTreeView::TempToMyPlaces()
{
	QModelIndex indexTempPlace = GetTemporaryIndex();
	if (!indexTempPlace.isValid())
		return;

	QModelIndexList indexes;
	QModelIndex childindex;

	int row = 0;
	do
	{
		childindex = indexTempPlace.child(row,0);
		row++;
		if (childindex.isValid())
			indexes << childindex;
	}while (childindex.isValid());

	if (indexes.count() < 1)
		return;

	QMimeData *data = m_model->mimeData(indexes);

	QModelIndex indexMyPlace = GetMyPlaceIndex();
	if (!indexMyPlace.isValid())
		return;
	QStandardItem * item = m_model->itemFromIndex(indexMyPlace);
	if (!item)
		return;

	int rowcount = item->rowCount();
	m_model->dropMimeData(data, Qt::MoveAction, rowcount, 0, indexMyPlace);
	DropInsertRows(indexMyPlace, row, 0);

	TreeItem *itemTemp = static_cast<TreeItem*> (m_model->itemFromIndex(indexTempPlace));
	if (itemTemp)
	{
		itemTemp->removeRows(0, itemTemp->rowCount());
	}
}

// Create Kml tree from TreeView, Create kml, Save it
int ResultTreeView::SaveAsItem(QString fileName)
{
	// Get the selected TreeItem in TreeView
	QModelIndex index = this->selectionModel()->currentIndex();
	TreeItem *itemMy  =static_cast<TreeItem*> (m_model->itemFromIndex(index));

	// Append file extension
	if ((fileName.length() < strlen(KML_FILE_EXT)) || 
		(fileName.right(strlen(KML_FILE_EXT)).compare(KML_FILE_EXT, Qt::CaseInsensitive) != 0))
	{
		fileName = fileName + KML_FILE_EXT;
	}

	// Create Kml tree of TreeItemRoot
	return SaveAsItemRoot(fileName, itemMy);
}

/*------------------------------------------------------
	Open MyPlaces from KML File
--------------------------------------------------------*/
int ResultTreeView::OpenMyPlaces()
{
	TreeItem* itemMyPlacesRoot = GetMyPlaceItem();
	if (!itemMyPlacesRoot)
		return -1;

	QString myFName;
	int ret = 0;

	// Get file name
	GetMyPlacesFName(myFName);


	// Create Kml tree of TreeItemRoot (Note: No add MyPlaces item)
	if (OpenItem(itemMyPlacesRoot, myFName, false) != 0)
	{
		QString myTempFName;
		GetMyPlacesTempFName(myTempFName);

		// Open temp file
		if (OpenItem(itemMyPlacesRoot, myTempFName, false) == 0)
		{
			// Delete old my file
			QFile::remove(myFName);

			// Copy temp file ---> my file
			QFile::copy(myTempFName, myFName);
		}
		else
		{
			QString myBackupFName;
			GetMyPlacesBackupFName(myBackupFName);

			// Open backup file
			if (OpenItem(itemMyPlacesRoot, myBackupFName, false) == 0)
			{
				// Delete old my file
				QFile::remove(myFName);

				// Copy backup file ---> my file
				QFile::copy(myBackupFName, myFName);
			}
			else
			{
				ret = -1;	// return error (File open error)
			}
		}
	}

	return ret;
}

/*------------------------------------------------------
	Save MyPlaces to KML File
--------------------------------------------------------*/
int ResultTreeView::SaveMyPlaces()
{
	TreeItem* itemMyPlacesRoot = GetMyPlaceItem();
	if (!itemMyPlacesRoot)
		return -1;

	QString myFName;
	QString myTempFName;
	QString myBackupFName;
	int ret = 0;

	// Get file name
	GetMyPlacesFName(myFName);
	GetMyPlacesTempFName(myTempFName);
	GetMyPlacesBackupFName(myBackupFName);

	// Create Kml tree of TreeItemRoot ---> Temp File
	if (SaveAsItemRoot(myTempFName, itemMyPlacesRoot) == 0)
	{
		// Delete old backup file
		QFile::remove(myBackupFName);

		// Move old my file ---> backup file
		QFile::rename(myFName, myBackupFName);

		// Copy temp file ---> my file
		QFile::copy(myTempFName, myFName);
	}
	// Error
	else
	{
		ret = -1;	// return error (File save error)
	}

	return ret;
}

// Get the default file name to save Kml
int ResultTreeView::GetFNameToSaveItem (QString& defaultFName)
{
	// Get the selected TreeItem in TreeView
	QModelIndex index = this->selectionModel()->currentIndex();
	TreeItem *itemMy  =static_cast<TreeItem*> (m_model->itemFromIndex(index));

	if (itemMy == NULL)
		return -1;		// return error (No item is selected)

	// Get name of selected item
	PLACEMARK_DATA data = itemMy->GetItemData();

	if (data.name == NULL)
		return -2;		// return error (Selected item has no name)

	defaultFName = QString::fromUtf16(data.name);

	return 0;			// return ok
}

// Open Kml file, Parse it, Add Kml tree to TreeView
int ResultTreeView::OpenItem(TreeItem* itemParent, QString fileName, bool addMyFlag, int row)
{
	int	ret	= 0;
	KmlTreeItem* kmlItemRoot;

	// Check param
	if (itemParent == NULL)
		return -1;	// return error (Param error)

	// Parse Kml file
	kmlItemRoot = new KmlTreeItem();

	if ((ret = KmlManagerClient::ParseKmlFile(fileName, kmlItemRoot)) == 0)
	{
		if ((kmlItemRoot != NULL) && (kmlItemRoot->m_childHead != NULL))
		{
			// Add Kml Tree to TreeView
			if ((ret = KmlTreeToTreeView(kmlItemRoot->m_childHead, itemParent, addMyFlag, row)) != 0)
			{
#ifdef DEBUG
				// Error
				qDebug("KML ERROR: Failed to Add KmlTree.");
#endif
			}
		}
		else
		{
			ret = -4;	// return error (Parsing error 2)
		}
	}
#ifdef DEBUG
	else
	{
		// Error
		qDebug("KML ERROR: Failed to Parse KmlFile.");
	}
#endif

	// Free Memory
	if (kmlItemRoot != NULL)
		delete kmlItemRoot;

	return ret;
}

/*------------------------------------------------------
			Free Libkml Memory manually
--------------------------------------------------------*/
void ResultTreeView::FreeLibkmlMemory()
{
	KmlManagerClient::FreeLibkmlMemory();
}

void ResultTreeView::SaveStartPosition()
{
	PLACEMARK_DATA pmd;
	memset(&pmd, 0, sizeof(pmd));
	if (m_startItem)
	{
		pmd = m_startItem->GetItemData();
	}
	else
	{
		QString name = tr("Starting Location");
		//name.toWCharArray(pmd.name);
		gu_qstring2ushort(name, pmd.name, PMD_STR_MAX_SIZE);
		pmd.itemtype = IT_PLACEMARK;
		pmd.snapshot = true;
		pmd.showmark = SMT_SHOW;
		pmd.key = GetKey();
		pmd.start = true;
	}

	pmd.cx = g_app->GetMainWindow()->GetRenderFrame()->rect().width() / 2;
	pmd.cy = g_app->GetMainWindow()->GetRenderFrame()->rect().height() / 2;

	RenderProxy *render = g_app->GetRenderProxy();
	if (render)
		render->SendMessage(PM_NEW_PLACEMARK, (PrxParam)&pmd);

	// set place mark position
	render->SendMessage(PM_MOVE_PLACEMARK, (PrxParam)&pmd);
	CGIS_Node *pNode = gdm_GetGISDoc()->GetNodeFromKey(pmd.key);
	if(pNode && pNode->Get_GeoType() == E_GEONODE_TYPE_POINT)
		((CGIS_Point*)pNode)->SetPt(pmd.longitude * PAI / 180, pmd.latitude * PAI / 180, pmd.altitude);

	if (m_startItem)
	{
		m_startItem->SetItemData(pmd);
		SendShowMarkItem(m_startItem, MT_Modify);
		return;
	}

	m_startItem = new TreeItem(pmd);
	TreeItem *myplace = GetMyPlaceItem();
	if (!myplace)
		return;

	if (m_startItem)
		myplace->insertRow(0, m_startItem);
}

void ResultTreeView::MoveStartPosition()
{
	PLACEMARK_DATA data;
	if (m_startItem)
	{
		data = m_startItem->GetItemData();
		gu_MakeRenderData(&data);
	}

	RenderProxy *rendersrv = g_app->GetRenderProxy();
	if (!rendersrv)
		return;
	if (m_startItem)
	{
		rendersrv->SendMessage(PM_SEARCH_PLACE, (PrxParam) &data, (PrxParam)1);
		SetSelectItem(m_startItem);
	}
	else
	{
		rendersrv->SendMessage(PM_SEARCH_PLACE, (PrxParam) NULL, (PrxParam)1);
		SetSelectItem(GetMyPlaceItem());
	}
}

void ResultTreeView::SetSelectItem(void *item)
{
	TreeItem *treeItem = static_cast<TreeItem*>(item);
	if (!treeItem)
	{
#ifdef DEBUG
		qDebug() << "set select error";
#endif
		return;
	}
	QModelIndex index = m_model->indexFromItem(treeItem);
	if (index.isValid())
		setCurrentIndex(index);
}

bool ResultTreeView::IsCutItem()
{
	TreeItem *item = GetSelectItem();
	if (!item || item->GetItemData().start)
		return false;

	if (m_viewType == VT_MyPlace && !item->parent())
		return false;

	return true;
}

bool ResultTreeView::IsCopyItem()
{
	TreeItem *item = GetSelectItem();
	if (!item || item->GetItemData().start)
		return false;

	return true;
}

bool ResultTreeView::IsPasteItem()
{
	TreeItem *item = GetSelectItem();
	if (!item)
		return false;

	if (m_viewType == VT_Search)
		return false;

	QClipboard *cb = QApplication::clipboard();
	const QMimeData *mimeData = cb->mimeData();
	if (mimeData && mimeData->hasFormat("application/x-qabstractitemmodeldatalist"))
		return true;

	return false;
}

bool ResultTreeView::IsDeleteItem()
{
	TreeItem *item = GetSelectItem();
	if (!item)
		return false;

	if (m_viewType == VT_MyPlace && !item->parent())
		return false;

	return true;
}

bool ResultTreeView::IsDeleteContents()
{
	TreeItem *item = GetSelectItem();
	if (!item)
		return false;

	if (item->hasChildren())
		return true;

	return false;
}

bool ResultTreeView::IsSaveMyPlaces()
{
	if (!GetSelectItem())
		return false;
	return true;
}

bool ResultTreeView::IsSetProperty()
{
	TreeItem *item = GetSelectItem();
	if (!item)
		return false;

	if (m_viewType == VT_MyPlace && !item->parent())
		return false;

	return true;
}

bool ResultTreeView::IsTempToMyPlaces()
{
	TreeItem *item = GetTempPlaceItem();
	if (!item)
		return false;

	if (item->hasChildren())
		return true;

	return false;
}

bool ResultTreeView::IsSearchData()
{
	TreeItem *item = GetSelectItem();
	if (item == NULL)
	{
#ifdef DEBUG
		qDebug("IsPlaceMark error");
#endif
		return false;
	}

	PLACEMARK_DATA data = item->GetItemData();
	if (data.itemtype == IT_SEARCH)
		return true;

	return false;
}

bool ResultTreeView::IsPlaceIcon()
{
	TreeItem *item = GetSelectItem();
	if (item == NULL)
	{
#ifdef DEBUG
		qDebug("IsPlaceMark error");
#endif
		return false;
	}

	PLACEMARK_DATA data = item->GetItemData();
	if (data.itemtype == IT_PLACEMARK)
		return true;

	return false;
}

bool ResultTreeView::IsPlacePath()
{
	TreeItem *item = GetSelectItem();
	if (item == NULL)
	{
		qDebug("IsPlaceMark error");
		return false;
	}

	PLACEMARK_DATA data = item->GetItemData();
	if (data.itemtype == IT_PATH)
		return true;

	return false;
}

bool ResultTreeView::IsPlaceArea()
{
	TreeItem *item = GetSelectItem();
	if (item == NULL)
	{
#ifdef DEBUG
		qDebug("IsPlaceMark error");
#endif
		return false;
	}

	PLACEMARK_DATA data = item->GetItemData();
	if (data.itemtype == IT_POLYAREA)
		return true;

	return false;
}

/*
 *   protected
 */

void ResultTreeView::contextMenuEvent(QContextMenuEvent *event)
{
	m_sideBar->SetSelectItem(NULL);

	TreeItem *parentItem = static_cast<TreeItem*>(m_model->invisibleRootItem());
	if (!parentItem->hasChildren())
		return;
	QModelIndex index = indexAt(event->pos());
	if (!index.isValid())
		return;

	g_app->GetMainWindow()->ShowContextMenu(event->globalPos());
}

void ResultTreeView::dropEvent(QDropEvent *event)
{
	QModelIndex index = indexAt(event->pos());
	if (index == QModelIndex() || !index.isValid())
	{
		event->ignore();
		return;
	}
	TreeItem * item = static_cast<TreeItem*> (m_model->itemFromIndex(index));
	PLACEMARK_DATA data = item->GetItemData();

	int row = 0;
	if (data.itemtype != IT_DOC && data.itemtype != IT_FOLDER)
	{
		row = index.row() + 1;
		index = index.parent();
		item = static_cast<TreeItem*> (m_model->itemFromIndex(index));
	}

	const QMimeData *mimeData = event->mimeData();
	if (!mimeData)
		return;

	if ( mimeData->hasFormat(QLatin1String("application/x-qabstractitemmodeldatalist")))
	{
			m_model->dropMimeData(mimeData, Qt::MoveAction, row, 0, index);
	}

	if ( mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		QString url = urlList.at(0).path();
		QString first;
		int count = url.count();
		if (count > 0)
		{
#ifdef Q_WS_WIN
			first = url.left(1);
			if ( first == QString(URL_MARK))
				url = url.right(count - 1);
#endif
			if (OpenItem(item, url, true, -1) != 0)
				return;

			int newRow = item->rowCount();
			TreeItem *openitem = static_cast<TreeItem*>(item->child(newRow-1));
			if (!openitem)
				return;

			QModelIndex openIndex = m_model->indexFromItem(openitem);
			if (openIndex.isValid())
			{
				setCurrentIndex(openIndex);
				MovePlace(openitem);
			}
		}
	}

	event->accept();
	this->setAutoScroll(false);
	setState(NoState);
	viewport()->update();
	DropInsertRows(index, row, 0);
}

void ResultTreeView::dragMoveEvent ( QDragMoveEvent * event )
{
	QModelIndex index = indexAt(event->pos());
	if (index == QModelIndex() || !index.isValid())
	{
		event->ignore();
		return;
	}
	return QTreeView::dragMoveEvent (event);
}

void ResultTreeView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space)
	{
		TreeItem *item = static_cast<TreeItem*> (m_model->itemFromIndex(currentIndex()));
		if (!item)
			return;

		if (item->checkState() == Qt::Checked)
			item->setCheckState(Qt::Unchecked);
		else
			item->setCheckState(Qt::Checked);

		ViewMark(currentIndex());
	}
	else
	{
		QTreeView::keyPressEvent(event);
	}
}

/*
 *   private
 */
void ResultTreeView::Init()
{
	m_storeItem = NULL;
	m_sideBar = NULL;
	m_startItem = NULL;
	m_keyStart = 0;
	SetupTreeView();
}

void ResultTreeView::SetupTreeView()
{
	setHeaderHidden(true);
	setWordWrap(true);
	m_model = new TreeItemModel;
	setModel(m_model);
	m_model->setSupportedDragActions(Qt::MoveAction);

	setItemDelegate(new TreeItemDelegate(this));

	setDragEnabled(true);
	if (m_viewType == VT_Search)
		setDragDropMode(QAbstractItemView::DragOnly);
	else
	{
		setDragDropMode(QAbstractItemView::DragDrop);
		setAcceptDrops(true);
		setDragDropOverwriteMode(false);
	}

	// connect(this, SIGNAL(expanded(QModelIndex)), SLOT(UpdownTreeItem(QModelIndex)));
   // connect(this, SIGNAL(collapsed(QModelIndex)), SLOT(UpdownTreeItem(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), SLOT(ViewPlace(QModelIndex)));
	connect(this, SIGNAL(clicked(QModelIndex)), SLOT(ViewMark(QModelIndex)));
	connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(InsertedRows(QModelIndex,int,int)));
	connect(m_model, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), SLOT(BeforeRemoveRows(QModelIndex,int,int)));
	connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(RemovedRows(QModelIndex,int,int)));
}

bool ResultTreeView::IsContainer(TreeItem *item)
{
	PLACEMARK_DATA data = item->GetItemData();
	if (item->hasChildren())
		return true;
	if (data.itemtype == IT_DOC || data.itemtype == IT_FOLDER)
		return true;

	return false;
}

QModelIndex ResultTreeView::GetMyPlaceIndex()
{
	return m_model->index(0, 0, QModelIndex());
}

QModelIndex ResultTreeView::GetTemporaryIndex()
{
	return m_model->index(1, 0, QModelIndex());
}

QString ResultTreeView::GetPageHtml(int totalcount, int startindex, int endindex, QString key, int itemkey, QString &pagelist)
{
	QString html = "";
	pagelist = QString("");
	int pages =( totalcount + TI_MAX_SEARCH_COUNT - 1 ) / TI_MAX_SEARCH_COUNT;
	if (pages < 2)
		return html;

	int current = (startindex  + TI_MAX_SEARCH_COUNT) / TI_MAX_SEARCH_COUNT;
	int startpage = (current < 5 ? 1 : current - 4);
	int maxpage = current + 4;
	if (maxpage > pages)
		maxpage = pages;

	for (int i = startpage; i < maxpage + 1; i++)
	{
		if (i != current)
		{
			int start = (i - 1) * TI_MAX_SEARCH_COUNT;
			if ( i == maxpage && maxpage < pages)
			{
				html += QString(SEARCH_LINK).arg(key).arg(start).arg(itemkey).arg(NEXT_PAGE);
				pagelist += QString(">");
			}
			else
			{
				html += QString(SEARCH_LINK).arg(key).arg(start).arg(itemkey).arg(i);
				pagelist += QString("%1").arg(i);
			}

			html += QString("</span></a>");
		}
		else
		{
			html += QString(SEARCH_NO_LINK).arg(i);
			pagelist += QString("%1").arg(i);
			html += QString("</span>");
		}
		html += QString(SPACE_TAG);
		pagelist += QString(" ");
	}
	return html;
}

void ResultTreeView::SendShowMarkItem(TreeItem *item, int type )
{
	if (item == NULL)
		return;

	CGIS_Node *pNode;

	m_sendData = item->GetItemData();
	int childcount = item->rowCount();
	if (childcount == 0)
	{
		pNode = gdm_GetGISDoc()->GetNodeFromKey(m_sendData.key);

		if (m_sendData.itemtype == IT_SEARCH || m_sendData.itemtype == IT_PLACEMARK || 
			m_sendData.itemtype == IT_PATH || m_sendData.itemtype == IT_POLYAREA)
		{
			switch (type)
			{
			case MT_Remove:
				if (m_sendData.start && m_keyStart == m_sendData.key)
				{
					m_startItem = NULL;
				}
				m_sideBar->RemovePlaceMark(m_sendData);
				gdm_GetGISDoc()->DestroyNodeFromKey(m_sendData.key);
				break;
			case MT_Add:
			case MT_Check:
				if (m_sendData.start)
				{
					if (!pNode)
					{
						CGIS_PlaceIcon *startNode = new CGIS_PlaceIcon();
						startNode->SetKey(m_sendData.key);
						startNode->m_nGisType = E_GISNODE_TYPE_PLACEMARK;
						startNode->SetPt(m_sendData.longitude * PAI / 180, m_sendData.latitude * PAI / 180, m_sendData.altitude);
						memcpy(startNode->m_sName, m_sendData.name, PMD_STR_MAX_SIZE);
						gdm_GetGISDoc()->AddNode(startNode);
					}
				}

				//m_sendData.key = GetKey(); // for duplicate key. RYM
				m_sendData.item = item;
				item->SetItemData(m_sendData);
				m_sideBar->AddPlaceMark(m_sendData);
				if (m_sendData.start)
				{
					m_keyStart = m_sendData.key;
					m_startItem = item;
				}
				if(pNode)
				{
					pNode->SetEnable(m_sendData.showmark);
					pNode->SetKey(m_sendData.key);
					gdm_GetGISDoc()->SetModifyFlag(true);
				}
				break;
			case MT_Modify:
				m_sideBar->ModifyPlaceMark(m_sendData);
				break;
			case MT_Uncheck:
				m_sideBar->RemovePlaceMark(m_sendData);
				if(pNode)
				{
					pNode->SetEnable(false);
					gdm_GetGISDoc()->SetModifyFlag(true);
				}
				break;
			}
		}
	}

	TreeItem *itemChild;
	for (int i = 0; i < childcount; i++)
	{
		itemChild = (TreeItem *)item->child(i);

		if (itemChild != NULL)
		{
			SendShowMarkItem(itemChild, type);
		}
	}
}

void ResultTreeView::SetCheckedItem(TreeItem *item, int state)
{
	PLACEMARK_DATA data = item->GetItemData();

	if (state == data.showmark)
		return;

	data.showmark = state;
	item->SetItemData(data);
	int childcount = item->rowCount();
	if (childcount == 0)
	{
		if (data.showmark == SMT_SHOW)
			SendShowMarkItem(item, MT_Check);
		else if (data.showmark == SMT_HIDE)
			SendShowMarkItem(item, MT_Uncheck);
	}

	TreeItem *itemChild;
	for (int i = 0; i < childcount; i++)
	{
		itemChild = (TreeItem *)item->child(i);

		if (itemChild != NULL)
		{
			SetCheckedItem(itemChild, state);
		}
	}
}

void ResultTreeView::UpdateParentItem(TreeItem * item, bool fromself)
{
	TreeItem *parent;
	if (fromself)
		parent = item;
	else
		parent = (TreeItem *)item->parent();

	if (parent == NULL)
		return;

	PLACEMARK_DATA data = parent->GetItemData();
	int childcount = parent->rowCount();
	int checkedcount = 0;
	int partiallycount = 0;
	TreeItem *itemChild;
	for (int i = 0; i < childcount; i++)
	{
		itemChild = (TreeItem *)parent->child(i);

		if (itemChild != NULL )
		{
			int state = itemChild->GetItemData().showmark;
			if (state  == SMT_SHOW)
				checkedcount ++;
			else if (state == SMT_PARTIALLY)
				partiallycount ++;
		}
	}

	if (childcount == 0)
	{
		data.showmark = SMT_SHOW;
	}
	else if (partiallycount > 0)
	{
		data.showmark = SMT_PARTIALLY;
	}
	else if (checkedcount == 0)
	{
		data.showmark = SMT_HIDE;
	}
	else if (checkedcount < childcount)
	{
		data.showmark = SMT_PARTIALLY;
	}
	else if (checkedcount == childcount)
	{
		data.showmark = SMT_SHOW;
	}

	parent->SetItemData(data);
	UpdateParentItem(parent);
}

/*void ResultTreeView::UpdownTreeItem(QModelIndex index)
{
	TreeItem * item = static_cast<TreeItem*> (m_model->itemFromIndex(index));
	PLACEMARK_DATA data = item->GetItemData();
	if (data.itemtype != IT_FOLDER)
		return;

    if (isExpanded(index))
        item->setIcon(QIcon(m_openIconName));
    else
		item->setIcon(QIcon(m_closeIconName));
	return;
}*/

void ResultTreeView::ViewPlace(QModelIndex index)
{
	TreeItem * item = static_cast<TreeItem*> (m_model->itemFromIndex(index));
	MovePlace(item);
}

void ResultTreeView::ViewMark(QModelIndex index)
{
	TreeItem * item = static_cast<TreeItem*> (m_model->itemFromIndex(index));
	if (!item)
		return;

	PLACEMARK_DATA data = item->GetItemData();
	int state;
	switch (item->checkState())
	{
	case Qt::Unchecked:
		state = SMT_HIDE;
		break;
	case Qt::PartiallyChecked:
		state = SMT_PARTIALLY;
		break;
	default:
		state = SMT_SHOW; //Qt::Checked
	}

	CGIS_Node * node=gdm_GetGISDoc()->GetNodeFromKey(data.key);

	gdm_GetGISDoc()->SetSelectNode(node);

	if (state == data.showmark)
		return;
	SetCheckedItem(item, state);
	UpdateParentItem(item);
}

void ResultTreeView::InsertedRows ( const QModelIndex & parent, int start, int end )
{
	QModelIndex index;
	TreeItem *item = NULL;
	PLACEMARK_DATA data;
	for (int i = start; i < end + 1; i++)
	{
		index = parent.child(i, 0);
		if (index.isValid())
		{
			item = static_cast<TreeItem *>(m_model->itemFromIndex(index));
			data = item->GetItemData();
			SendShowMarkItem(item, MT_Add);
			UpdateParentItem(item);
		}
	}
#ifdef GDEM_DEBUG
		qDebug("row inserted");
#endif
}

void ResultTreeView::DropInsertRows( const QModelIndex & parent, int row, int col )
{
	TreeItem *item = NULL;

	QModelIndex index = parent.child(row, 0);
	if (index.isValid())
	{
		item = static_cast<TreeItem *>(m_model->itemFromIndex(index));
		SendShowMarkItem(item, MT_Add);
		UpdateParentItem(item);
	}
	qDebug("after row inserted");
}

void ResultTreeView::MovePlace(TreeItem *item)
{
	if (!item)
		return;
	int childcount = item->rowCount();
	PLACEMARK_DATA data = item->GetItemData();
	gu_MakeRenderData(&data);
	RenderProxy *rendersrv = g_app->GetRenderProxy();
	if (!rendersrv)
		return;

	if (data.itemtype == IT_SEARCH || data.itemtype == IT_PLACEMARK || 
		data.itemtype == IT_PATH || data.itemtype == IT_POLYAREA || data.snapshot )
	{
		rendersrv->SendMessage(PM_SEARCH_PLACE, (PrxParam) &data, (PrxParam)1);
		return;
	}
	else if (!data.snapshot && childcount == 0)
	{
		return;
	}

	PLACEMARK_DATA *sendMoveData = sendMoveData = (PLACEMARK_DATA *) malloc(sizeof(PLACEMARK_DATA) * childcount);
	if (!sendMoveData)
		return;

	PLACEMARK_DATA senddata;
	for (int i = 0; i < childcount; i++)
	{
		senddata = static_cast<TreeItem*>(item->child(i))->GetItemData();
		gu_MakeRenderData(&senddata);
		memcpy(sendMoveData + i, &senddata, sizeof(PLACEMARK_DATA));
	}

	rendersrv->SendMessage(PM_SEARCH_PLACE, (PrxParam) sendMoveData, (PrxParam)childcount);
	free(sendMoveData);
}


void ResultTreeView::BeforeRemoveRows ( const QModelIndex & parent, int start, int end )
{
	QModelIndex index;
	TreeItem *item =NULL;
	for (int i = start; i < end + 1; i++)
	{
		index = m_model->index(i, 0, parent);
		if (index.isValid())
			item = static_cast<TreeItem *>(m_model->itemFromIndex(index));
		SendShowMarkItem(item, MT_Remove);
	}
#ifdef DEBUG
	qDebug("row removed");
#endif
}

void ResultTreeView::RemovedRows ( const QModelIndex & parent, int start, int end )
{
	if (!parent.isValid())
		return;
	TreeItem *item = static_cast<TreeItem *>(m_model->itemFromIndex(parent));
	UpdateParentItem(item, true);
}

void ResultTreeView::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{
	m_sideBar->SetSelectItem(NULL);
	g_app->GetMainWindow()->UpdateShortCutKey();
}

/********************************************************
	Open Kml file, Parse it, Add Kml tree to TreeView
********************************************************/
// Add Kml Tree to TreeView
static int KmlTreeToTreeView(KmlTreeItem* kmlItemMy, TreeItem* itemParent, bool addMyFlag, int row)
{
	//-------------------------- Check param
	if ((kmlItemMy == NULL) || (itemParent == NULL))
		return -1;	// return error (Param error)

	//-------------------------- Add My Item
	TreeItem* itemMy;

	// Normal : Add My TreeItem
	if (addMyFlag)
	{
		PLACEMARK_DATA data;

		memset(&data, 0, sizeof(data));

		data.key = Key();

		if (kmlItemMy->ConvertToTreeItemData(&data) ==0)
		{
			if (data.itemtype == IT_SEARCH || data.itemtype == IT_PLACEMARK || 
				data.itemtype == IT_PATH || data.itemtype == IT_POLYAREA)
			{
				if (data.itemtype != IT_SEARCH)
					data.state = PLACEMARK_NORMAL;
			}
			itemMy = new TreeItem(data);
			if (row == -1)
				itemParent->appendRow(itemMy);
			else
				itemParent->insertRow(row, itemMy);
		}
		else
		{
			return -1;	// return error (Not Set data To TreeItem Data)
		}
	}
	// Other: No Add My TreeItem
	else
	{
		itemMy = itemParent;
	}

	//-------------------------- Add the Child Items
	if (kmlItemMy->m_childHead != NULL)
	{
		KmlTreeItem* kmlItemChild		= kmlItemMy->m_childHead;
		KmlTreeItem* kmlItemNextChild	= NULL;

		while (kmlItemChild != NULL)
		{
			kmlItemNextChild = kmlItemChild->m_next; // NOTE: Because kmlItemChild is free after KmlTreeToTreeView()

			KmlTreeToTreeView(kmlItemChild, itemMy, true);

			kmlItemChild = kmlItemNextChild;
		}
	}

	//-------------------------- Free Memory
	if (kmlItemMy != NULL)
		delete kmlItemMy;

	return 0;	// return success
}

/********************************************************
	Create Kml tree from TreeView, Create kml, Save it
********************************************************/
// Create Kml Tree from TreeView
static int TreeViewToKmlTree(TreeItem* itemMy, KmlTreeItem* kmlItemParent)
{
	//-------------------------- Check param
	if ((itemMy == NULL) || (kmlItemParent == NULL))
		return -1;	// return error (Param error)

	//-------------------------- Add My Item
	PLACEMARK_DATA data = itemMy->GetItemData();

	KmlTreeItem* kmlItemMy = new KmlTreeItem();

	if (kmlItemMy->ConvertFromTreeItemData(&data) == 0)
	{
		kmlItemMy->AddToKmlTree(kmlItemParent);
	}
	else
	{
		return -1;	// return error (Not Set data To TreeItem Data)
	}

	//-------------------------- Add the Child Items
	int i;
	TreeItem *itemChild;
	int childct = itemMy->rowCount();

	for (i = 0; i < childct; i++)
	{
		itemChild = (TreeItem *)itemMy->child(i);

		// Add child item
		if (itemChild != NULL)
		{
			TreeViewToKmlTree(itemChild, kmlItemMy);
		}
	}

	return 0; // return ok
}

// Create Kml tree of TreeItemRoot, Create kml, Save it
static int SaveAsItemRoot(QString fileName, TreeItem *itemRoot)
{
	int	ret	= 0;
	KmlTreeItem* kmlItemRoot;

	// Create Kml tree from TreeView
	kmlItemRoot = new KmlTreeItem();

	if ((ret = TreeViewToKmlTree(itemRoot, kmlItemRoot)) == 0)
	{
		// Save Kml tree to Kml file
		if ((ret = KmlManagerClient::CreateKmlFile(kmlItemRoot, fileName)) != 0)
		{
#ifdef DEBUG
			// Error
			qDebug("KML ERROR: Failed to Create KmlTree.");
#endif
		}
	}
#ifdef DEBUG
	else
	{
		// Error
		qDebug("KML ERROR: Failed to Create KmlTree.");
	}
#endif

	// Free Memory
	if (kmlItemRoot != NULL)
		delete kmlItemRoot;

	return ret;
}

/********************************************************
	Get the KML file name for MyPlaces
********************************************************/
// Get the MyPlceas file name
static void GetMyPlacesFName(QString& fname)
{
	fname = g_app->GetUserDataDir();
	fname = fname + "/" + MY_PLACES_FNAME_PREFIX + KML_FILE_EXT;
}

// Get the MyPlceas Temp file name
static void GetMyPlacesTempFName(QString& fname)
{
	fname = g_app->GetUserDataDir();
	fname = fname + "/" + MY_PLACES_TEMP_FNAME_PREFIX + KML_FILE_EXT;
}

// Get the MyPlceas Backup file name
static void GetMyPlacesBackupFName(QString& fname)
{
	fname = g_app->GetUserDataDir();
	fname = fname + "/" + MY_PLACES_BACKUP_FNAME_PREFIX + KML_FILE_EXT;
}
