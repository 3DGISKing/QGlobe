#include "kmlmanagerclient.h"
#include "kml_creator.h"
#include "kml_treeitem.h"
#include "kml_objects.h"
#include "kml_parser.h"


#include "kml/dom.h"
#include "kml/engine.h"

// libkml types are in the kmldom and kmlengine namespace
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::ContainerPtr;
using kmldom::DocumentPtr;
using kmldom::FolderPtr;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::GeometryPtr;
using kmldom::MultiGeometryPtr;

using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::CoordinatesPtr;
using kmldom::LookAtPtr;
using kmldom::AltitudeModeEnum;

/* For parsing feature styles */
#if KML_USE_FEATURE_STYLES == 1
using kmldom::ListStylePtr;
using kmldom::IconStylePtr;
using kmldom::PairPtr;
using kmldom::StylePtr;
using kmldom::StyleMapPtr;
using kmldom::StyleSelectorPtr;
#endif // KML_USE_FEATURE_STYLES


// Static functions
static int KmlTreeToKmlDomTree(KmlTreeItem* itemMy, kmldom::FeaturePtr kmlDomParent, kmldom::KmlFactory* factory, kmldom::FeaturePtr& kmlDomRoot, QString kmlFileName);

kmldom::StylePtr CreateKmldomStyle(KmlStyle* curr, KML_StyleStateEnum type)
{
	kmldom::StylePtr style(KmlFactory::GetFactory()->CreateStyle());

	//set id
	if(KML_STYLESTATE_NORMAL == type)
		style->set_id(QStringToKml(curr->normal_styleurl));
	else if(KML_STYLESTATE_HIGHLIGHT == type)
		style->set_id(QStringToKml(curr->highlight_styleurl));

	if(curr->iconstyle)
	{
		kmldom::IconStylePtr iconstyle = KmlFactory::GetFactory()->CreateIconStyle();
		kmldom::HotSpotPtr hotspot = KmlFactory::GetFactory()->CreateHotSpot();
		kmldom::IconStyleIconPtr iconstyle_icon = KmlFactory::GetFactory()->CreateIconStyleIcon();

		if(KML_STYLESTATE_NORMAL == type)
			iconstyle->set_scale(curr->iconstyle->scale); //by ugi 2013.8.1
		else if(KML_STYLESTATE_HIGHLIGHT == type)
			iconstyle->set_scale(1.3*curr->iconstyle->scale); //by ugi 2013.8.1

		//by RSH 2013.7.23
		//iconstyle_icon->set_href(GOOGLE_EARTH_ICON_IMAGE_URL);

		QString iconname = curr->iconstyle->iconname;
		if (!iconname.isEmpty())
		{
			iconstyle_icon->set_href(QStringToKml(iconname));  //end
		}

		//2014 2 6 by ugi
		QString videopath=curr->iconstyle->videopath;
		if(!videopath.isEmpty())
		{
			iconstyle->set_video_path(QStringToKml(videopath));
		}
		//end
		//2014 2 7 by ugi
		QString htmlpath=curr->iconstyle->htmlpath;
		if(!htmlpath.isEmpty())
		{
			iconstyle->set_html_path(QStringToKml(htmlpath));
		}
		//end

		iconstyle->set_icon(iconstyle_icon);

		//iconstyle->set_heading(curr->iconstyle->heading);
		hotspot->set_x(20); hotspot->set_y(2); hotspot->set_xunits(KML_UNITS_PIXELS); hotspot->set_yunits(KML_UNITS_PIXELS);
		iconstyle->set_hotspot(hotspot);

		style->set_iconstyle(iconstyle);
	}

	if(curr->labelstyle)
	{
		kmldom::LabelStylePtr labelstyle = KmlFactory::GetFactory()->CreateLabelStyle();
		kmlbase::Color32	kmldom_color;

#if 0
		if(KML_STYLESTATE_NORMAL == type)
			labelstyle->set_scale(1.1/*curr->labelstyle->scale*/);
		else if(KML_STYLESTATE_HIGHLIGHT == type)
			labelstyle->set_scale(1.3/*curr->labelstyle->scale*/);
#endif

		kmldom_color.set_color_argb(0xff00ff00/*curr->labelstyle->color*/);
		labelstyle->set_color(kmldom_color);

		style->set_labelstyle(labelstyle);
	}

	if(curr->linestyle)
	{
		kmldom::LineStylePtr linestyle = KmlFactory::GetFactory()->CreateLineStyle();
		kmlbase::Color32	kmldom_color;
		
		kmldom_color.set_color_argb(curr->linestyle->color);
		linestyle->set_color(kmldom_color);
		linestyle->set_width(curr->linestyle->penwidth);

		style->set_linestyle(linestyle);
	}

	if(curr->ploystyle)
	{
		kmldom::PolyStylePtr polystyle = KmlFactory::GetFactory()->CreatePolyStyle();
		kmlbase::Color32	kmldom_color;

		kmldom_color.set_color_argb(curr->ploystyle->color);
		polystyle->set_color(kmldom_color);
		if(false == curr->ploystyle->fill)
			polystyle->set_fill(false);
		if(false == curr->ploystyle->outline)
			polystyle->set_outline(false);

		//by RSH 2013.8.1
		if (!(curr->ploystyle->polyTexture.isEmpty()))
			polystyle->set_polyTexture(QStringToKml(curr->ploystyle->polyTexture));
		//end

		style->set_polystyle(polystyle);
	}

	return style;
}

kmldom::StyleMapPtr CreateKmldomStyleMap(KmlStyle* curr)
{
	QString pre_symbol(STYLE_URL_PRIFIX_SYMBOL);
	QString styleUrl;

	kmldom::StyleMapPtr style_map(KmlFactory::GetFactory()->CreateStyleMap());
	kmldom::PairPtr normal_pair = KmlFactory::GetFactory()->CreatePair();
	kmldom::PairPtr highlight_pair = KmlFactory::GetFactory()->CreatePair();

	style_map->set_id(QStringToKml(curr->styleMapid));

	normal_pair->set_key(KML_STYLESTATE_NORMAL);
	styleUrl = curr->normal_styleurl;
	normal_pair->set_styleurl(QStringToKml(styleUrl.prepend(pre_symbol)));

	highlight_pair->set_key(KML_STYLESTATE_HIGHLIGHT);
	styleUrl = curr->highlight_styleurl;
	highlight_pair->set_styleurl(QStringToKml(styleUrl.prepend(pre_symbol)));

	style_map->add_pair(normal_pair);
	style_map->add_pair(highlight_pair);

	return style_map;
}

void CreateKmldomStyleFromKmlItem(KmlStyle* kmlstyletree, kmldom::FeaturePtr feature)
{

	KmlStyle *curr, *next;
	kmldom::DocumentPtr doc = kmldom::AsDocument(feature);

	if (!kmlstyletree || !feature)
		return;

	curr = kmlstyletree;

	while(curr)
	{
		kmldom::StylePtr normal_style;
		kmldom::StylePtr highlight_style;
		kmldom::StyleMapPtr style_map;

		style_map = CreateKmldomStyleMap(curr);
		doc->add_styleselector(style_map);

		normal_style = CreateKmldomStyle(curr, KML_STYLESTATE_NORMAL);
		doc->add_styleselector(normal_style);

		highlight_style = CreateKmldomStyle(curr, KML_STYLESTATE_HIGHLIGHT);
		doc->add_styleselector(highlight_style);

		curr = curr->next;
	}

	//free kmlstyletree
	DeleteKmlStyleTree();
}
/********************************************************
	Add Kml Dom Object to Kml Dom Tree
********************************************************/
static int KmlTreeToKmlDomTree(KmlTreeItem* itemMy, kmldom::FeaturePtr kmlDomParent, 
							   kmldom::KmlFactory* factory, kmldom::FeaturePtr& kmlDomRoot, QString kmlFileName)
{
	int ret;
	kmldom::FeaturePtr kmlDomMy = NULL;

	//-------------------------- Check param
	if (itemMy == NULL)
		return -1;	// return error (Param error)

	//-------------------------- Create My Kml Dom Object
	KmlTreeItemType itemType = itemMy->GetType();


	if (GetStyleTreeRoot() && itemType != KML_IT_DOCUMET)		// Document and style create
	{
		kmlDomMy = factory->CreateDocument();

		CreateKmldomStyleFromKmlItem(GetStyleTreeRoot(), kmlDomMy);

		QString symbol("/");
		kmlDomMy->set_name(QStringToKml(kmlFileName.right(kmlFileName.length() - kmlFileName.lastIndexOf(symbol) - 1)));

		if(KML_IT_DOCUMET == itemType)
			if ((ret = KmlCreateDomObject(itemMy, kmlDomMy)) != 0)
				return ret;	// return error (Create Kml Dom Object)	

		if (kmlDomParent == NULL)
		{
			kmlDomRoot = kmlDomMy;
			kmlDomParent = kmlDomMy;
		}
		else
		{
			const ContainerPtr container = kmldom::AsContainer(kmlDomParent);
			container->add_feature(kmlDomMy);
		}
	}

	if (itemType == KML_IT_DOCUMET)				// Folder
	{
		kmlDomMy = factory->CreateDocument();

		if (GetStyleTreeRoot())
			CreateKmldomStyleFromKmlItem(GetStyleTreeRoot(), kmlDomMy);

		if ((ret = KmlCreateDomObject(itemMy, kmlDomMy)) != 0)
			return ret;	// return error (Creatome Kml Dom Object)	
	}
	else if (itemType == KML_IT_FOLDER)				// Folder
	{
		kmlDomMy = factory->CreateFolder();
		
		if ((ret = KmlCreateDomObject(itemMy, kmlDomMy)) != 0)
			return ret;	// return error (Create Kml Dom Object)	
	}
	else if (itemType == KML_IT_PLACESEARCH)		// PlaceMark (Search)
	{		
		kmlDomMy = factory->CreatePlacemark();

		if ((ret = KmlCreateDomObject(itemMy, kmlDomMy)) != 0)
			return ret;	// return error (Create Kml Dom Object)	
	}
	else if (itemType == KML_IT_PLACEMARK || KML_IT_POLYAREA == itemType || KML_IT_PATH == itemType)
	{		
		kmlDomMy = factory->CreatePlacemark();

		if ((ret = KmlCreateDomObject(itemMy, kmlDomMy)) != 0)
			return ret;	// return error (Create Kml Dom Object)	
	}
	else if (itemType == KML_IT_PAGE)			// Page
	{
		kmlDomMy = factory->CreateFolder();

		if ((ret = KmlCreateDomObject(itemMy, kmlDomMy)) != 0)
			return ret;	// return error (Create Kml Dom Object)	
	}

	//-------------------------- Set Dom Root	
	if (kmlDomParent == NULL)
	{
		kmlDomRoot = kmlDomMy;
	}
	//-------------------------- Add My Kml Dom Object to Dom Parent
	else
	{
		const ContainerPtr container = kmldom::AsContainer(kmlDomParent);

		container->add_feature(kmlDomMy);
	}

	//-------------------------- Add the Child Objects
	if (itemMy->m_childHead != NULL)
	{
		KmlTreeItem* itemChild		= itemMy->m_childHead;
		KmlTreeItem* itemNextChild	= NULL;

		while (itemChild != NULL)
		{
			itemNextChild = itemChild->m_next; // NOTE: Because itemChild is free after KmlTreeToKmlDomTree()

			KmlTreeToKmlDomTree(itemChild, kmlDomMy, factory, kmlDomRoot, kmlFileName);

			itemChild = itemNextChild;
		}		
	}

	//-------------------------- Free Memory
	if (itemMy != NULL)
		delete itemMy;

	return 0;	// return ok
}

/********************************************************
	Create Kml data string from Kml Tree
	IN:		itemRoot		(Root Item of Kml Tree)	
	OUT:	kmlData			(kml data string)		
********************************************************/
int KmlCreate_DataFromKmlTree(KmlTreeItem* itemRoot, std::string& kmlData, QString kmlFileName)
{
	int ret;
	KmlFactory* factory				= KmlFactory::GetFactory();
	kmldom::FeaturePtr kmlDomRoot	= NULL;

	// Convert Kml Tree To Dom Tree
	if (itemRoot->m_childHead == NULL)
		return -1; // return error (Empty KmlTree)

	if ((ret = KmlTreeToKmlDomTree(itemRoot->m_childHead, NULL, factory, kmlDomRoot, kmlFileName)) != 0)
		return ret;	// return error

	// Get kml data string from Dom Tree	
	KmlPtr kml = factory->CreateKml();
	kml->set_feature(kmlDomRoot);
	kmlData = kmldom::SerializePretty(kml);

	// Add Kml file Header
	kmlData = KML_FILE_HEADER_STR_UTF8 + kmlData;

	return 0; // return ok
}

/********************************************************
	Free libkml memory manually
********************************************************/
void FreeLibkmlMemoryManual()
{
	// Free memory
	kmldom::FreeMemoryManual();
}