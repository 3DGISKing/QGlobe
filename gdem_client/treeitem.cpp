#include "gdemclient.h"
#include "treeitem.h"
#include "GIS_Doc.h"
#include "GIS_PlaceIcon.h"
#include "GIS_Path.h"
#include "GIS_Area.h"
#include "resulttreeview.h"

#define ICON_DOC			"earth.png"
#define ICON_OPEN_FOLDER    "openfolder.png"
#define ICON_CLOSE_FOLDER   "closedfolder.png"
#define ICON_NORMAL_ITEM    "item.png"
#define ICON_PLACEMARK_ITEM "placemark.png"
#define ICON_PLACEPATH_ITEM "placepath.png"
#define ICON_PLACEPOLY_ITEM "placepoly.png"
#define ICON_PLACEBOX_ITEM "placebox.png"

#define ICON_NAME_COUNTRY		"layers-country-name.png"
#define ICON_NAME_FIRSTADMIN	"layers-1st-name.png"
#define ICON_NAME_SECONDADMIN	"layers-2nd-name.png"
#define ICON_NAME_THIRDADMIN	"layers-3rd-name.png"
#define ICON_NAME_POPULAR		"layers-popular-name.png"
#define ICON_NAME_MOUNTAIN		"layers-mountain.png"
#define ICON_NAME_RIVER		    "layers-river.png"
#define ICON_NAME_SEA    		"layers-sea.png"
#define ICON_NAME_PORT			"layers-port.png"
#define ICON_NAME_SPA			"layers-spa.png"

TreeItem::TreeItem(const PLACEMARK_DATA &data, const QString& pagelist)
		:QStandardItem()
{
	setEditable(false);
	if (data.itemtype != IT_PAGE)
	{
		setCheckable(true);
		setCheckState(Qt::Checked);
	}

	setTextAlignment(Qt::AlignTop);

	qRegisterMetaTypeStreamOperators<PLACEMARK_DATA>("PLACEMARK_DATA");

	SetItemData(data, pagelist);
}

TreeItem::~TreeItem()
{
	PLACEMARK_DATA data  = GetItemData();
	CGIS_Node *pNode = gdm_GetGISDoc()->GetNodeFromKey(data.key);

	if(pNode)
		gdm_GetGISDoc()->DestroyNodeFromKey(data.key);
}

PLACEMARK_DATA TreeItem::GetItemData()
{
	PLACEMARK_DATA data = qVariantValue<PLACEMARK_DATA>(this->data(TI_DATA_ROLE));
	return data;
}

void TreeItem::SetItemData(const PLACEMARK_DATA &data, const QString& pagelist)
{
	// Main Data
	QVariant var;
	var.setValue(data);
	setData(var, TI_DATA_ROLE);

	// Display String

	QString name = QString::fromUtf16(data.name);
	QString des = QString::fromUtf16(data.description);
	name = name + QChar(0x0a) + des;
	var.setValue(name);
	setData(var, Qt::DisplayRole);

	// Page Role
	if (data.itemtype == IT_PAGE)
	{
		var.setValue(pagelist);
		setData(var, TI_PAGE_ROLE);
	}
	// Decoration Pixmap
	QString path = g_app->GetResource("/sidebar/");
	QString iconname;

	switch (data.itemtype)
	{
	case IT_DOC:
		iconname = ICON_DOC;
		break;
	case IT_FOLDER:
		iconname = ICON_CLOSE_FOLDER;
	case IT_PAGE:
		break;
	case IT_SEARCH:
		{
			iconname = ICON_PLACEMARK_ITEM;
			break;
		}
		break;
	//by ugi 2013.8.2
	/*case IT_PLACEMARK:
		iconname = ICON_PLACEMARK_ITEM;
		break;*/ 
	case IT_PATH:
		iconname = ICON_PLACEPATH_ITEM;
		break;
	case IT_POLYAREA:
		iconname = ICON_PLACEPOLY_ITEM;
		break;
	default:
		break;
	}

	if(data.itemtype==IT_PLACEMARK)
	{
		iconname=QString::fromUtf16(data.iconname);

		if(iconname.isEmpty())
			iconname=path+ICON_PLACEMARK_ITEM;
		else 
		{
			iconname=g_app->GetWorkDir()+QString::fromUtf16(data.iconname);

			if(!QFile::exists(iconname))
				iconname=path+ICON_PLACEMARK_ITEM;
		}
	}
	else
		iconname = path + iconname;
	QIcon icon(iconname);
	var.setValue(icon);
	setData(var, Qt::DecorationRole);
	
	// check state
	switch (data.showmark)
	{
	case 0:
		var.setValue(0);  // Qt::Unchecked
		break;
	case 1:
		var.setValue(1);  //Qt::PartiallyChecked
		break;
	default:
		var.setValue(2);  //Qt::Checked
	}
	if (data.itemtype != IT_PAGE)
		setData(var, Qt::CheckStateRole);
}

QDataStream & operator<< (QDataStream &out, const PLACEMARK_DATA &item)
{
	out << item.type;
	out << item.longitude;
	out << item.latitude;
	out << item.altitude;
	out << item.name;
	out << item.description;
	out << item.itemtype;
	out << item.view_longitude;
	out << item.view_latitude;
	out << item.view_range;
	out << item.view_heading;
	out << item.view_tilt;
	out << item.snapshot;
	out << item.visible;
	out << item.showmark;
	out << item.start;
	out << item.key;
	out << item.item;

	out << item.iconname;//by RSH 2013.8.2
	out <<item.iconscale;//2014 2 7 by ugi
    out <<item.videopath;//2014 2 7 by ugi
	out <<item.htmlpath;//2014 2 7 by ugi

	CGIS_Doc *pGisDoc = gdm_GetGISDoc();
	CGIS_Node *pNode;

	switch(item.itemtype)
	{
	case IT_PLACEMARK:
	case IT_PATH:
	case IT_POLYAREA:
	case IT_SEARCH:
		pNode = pGisDoc->GetNodeFromKey(item.key);
		if(pNode)
			pNode->Write(out);
		break;
	}

	return out;
}

QDataStream & operator>> (QDataStream &in, PLACEMARK_DATA &item)
{
	in >> item.type;
	in >> item.longitude;
	in >> item.latitude;
	in >> item.altitude;
	in >> item.name;
	in >> item.description;
	in >> item.itemtype;
	in >> item.view_longitude;
	in >> item.view_latitude;
	in >> item.view_range;
	in >> item.view_heading;
	in >> item.view_tilt;
	in >> item.snapshot;
	in >> item.visible;
	in >> item.showmark;
	in >> item.start;
	in >> item.key;
	in >> item.item;
	in >> item.iconname; //by RSH 2013.8.2
	in >>item.iconscale; //2014 2 7 by ugi
	in >>item.videopath; //2014 2 7 by ugi
	in >>item.htmlpath; //2014 2 7 by ugi

	item.key = ResultTreeView::GetNewKey(); // new key for copy, paste, move

	CGIS_Doc *pGisDoc = gdm_GetGISDoc();
	CGIS_Node *pNode = pGisDoc->GetNodeFromKey(item.key);
	if(item.itemtype == IT_PLACEMARK || item.itemtype == IT_SEARCH)	// Place (User Add)
	{
		if(pNode == NULL)
		{
			pNode = new CGIS_PlaceIcon();
			pNode->SetKey(item.key);
			memcpy(pNode->m_sName, item.name, PMD_STR_MAX_SIZE);

			CGIS_PlaceIcon* placeicon=(CGIS_PlaceIcon*)pNode;

			placeicon->m_IconScale=item.iconscale;
			memcpy(placeicon->m_sIconName, item.iconname, PMD_STR_MAX_SIZE);
			memcpy(placeicon->m_sVideoPath, item.videopath, PMD_STR_MAX_SIZE);
			memcpy(placeicon->m_sHtmlPath, item.htmlpath, PMD_STR_MAX_SIZE);

			pGisDoc->AddNode(pNode);
		}

		pNode->Read(in);
	}
	else if (item.itemtype == IT_PATH)	    // Path(User Add)
	{
		if(pNode == NULL)
		{
			pNode = new CGIS_Path();
			pNode->SetKey(item.key);
			pGisDoc->AddNode(pNode);
		}

		pNode->Read(in);
	}
	else if (item.itemtype == IT_POLYAREA)	    // Polygon(User Add)
	{
		if(pNode == NULL)
		{
			pNode = new CGIS_Area();
			pNode->SetKey(item.key);
			pGisDoc->AddNode(pNode);
		}

		pNode->Read(in);
	}

	if(pNode)
		pNode->SetKey(item.key);

	return in;
}

QDataStream & operator<< (QDataStream &out, const ushort* text)
{
	int i = 0;
	do
	{
		out << text[i];
	}
	while (text[i++] > 0);

	return out;
}

QDataStream & operator>> (QDataStream &in, ushort* text)
{
	int i = 0;
	quint16 code;
	while (!in.atEnd())
	{
		in >> code;
		*(text+i) = code;
		if (!code)
			break;
		i++;
	}
	return in;
}

QDataStream & operator<< (QDataStream &out, const void *item)
{
 quint32 data = (quint32)item;
 out << data;
 return out;
}

QDataStream & operator>> (QDataStream &in, void *item)
{
 quint32 data;
 in >> data;
 item = (void *)data;
 return in;
}
