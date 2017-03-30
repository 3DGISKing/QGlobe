#include "kmlmanagerclient.h"
#include "kml_parser.h"
#include "kml_treeitem.h"
#include "kml_objects.h"

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

// Static functions for Kml Dom Objects 
static const FeaturePtr GetRootFeature(const ElementPtr& root);
static void VisitContainer(const ContainerPtr& container, KmlTreeItem* itemParent);
static void VisitFeature(const FeaturePtr& feature, KmlTreeItem* itemParent);

/* For parsing feature styles */
#if KML_USE_FEATURE_STYLES == 1
static void VisitFeatureStyle(const FeaturePtr& feature);
static void VisitGeometry(const GeometryPtr& geometry);
static void VisitListStyle(const ListStylePtr& liststyle);
static void VisitPlacemark(const PlacemarkPtr& placemark);
static void VisitStyle(const StylePtr& style);
static void VisitStyleMap(const StyleMapPtr& stylemap, KmlStyle *pstyle);
static void VisitStyleSelector(const StyleSelectorPtr& styleselector, KmlStyle *pstyle);
#endif // KML_USE_FEATURE_STYLES

KmlStyle *g_kmlstyle = NULL;

void init_style(KmlStyle *style)
{
	if(!style)
		return;

	style->styleid				= "";
	style->styleMapid			= "";
	style->normal_styleurl		= "";
	style->highlight_styleurl	= "";

	style->iconstyle	= NULL;
	style->labelstyle	= NULL;
	style->linestyle	= NULL;
	style->ploystyle	= NULL;

	style->next			= NULL;
	style->prev			= NULL;
}

KmlStyle* new_kmlstyle()
{
	KmlStyle *new_style;

	new_style = new KmlStyle;
	if(!new_style)
		return NULL;

	init_style(new_style);

	return new_style;
}

void AddToKmlStyleTree(KmlStyle *new_style)
{
	KmlStyle *curr;
	KmlStyle *tail;

	if(!new_style)
		return;

	curr = GetStyleTreeRoot();

	if(!curr) // root is null
	{
		SetStyleTreeRoot(new_style);
	}
	else // root is not null
	{
		while(curr)
		{
			tail = curr;
			curr = curr->next;
		}

		tail->next = new_style;
		new_style->prev = tail;
	}

}

void copy_kml_style(KmlStyle *src, KmlStyle *dest)
{
	if( !src || !dest)
		return;

	dest->styleid		= src->styleid;
	dest->styleMapid	= src->styleMapid;
	dest->normal_styleurl = src->normal_styleurl;
	dest->highlight_styleurl = src->highlight_styleurl;
	dest->next = dest->prev = NULL;

	if(src->iconstyle)
	{
		dest->iconstyle = new KmlIconStyleElement;
		if(!dest->iconstyle)
			return;

		dest->iconstyle->iconname	= src->iconstyle->iconname;
		dest->iconstyle->color		= src->iconstyle->color;
		dest->iconstyle->colormode	= src->iconstyle->colormode;
		dest->iconstyle->heading	= src->iconstyle->heading;
		dest->iconstyle->penwidth	= src->iconstyle->penwidth;
		dest->iconstyle->scale		= src->iconstyle->scale;
		
		dest->iconstyle->videopath  =src->iconstyle->videopath; //2014 2 6 by ugi
		dest->iconstyle->htmlpath  =src->iconstyle->htmlpath; //2014 2 7 by ugi
	}

	if(src->labelstyle)
	{
		dest->labelstyle = new KmlStyleElement;
		if(!dest->labelstyle)
			return;

		dest->labelstyle->color		= src->labelstyle->color;
		dest->labelstyle->colormode = src->labelstyle->colormode;
		dest->labelstyle->penwidth	= src->labelstyle->penwidth;
		dest->labelstyle->scale		= src->labelstyle->scale;
	}

	if(src->linestyle)
	{
		dest->linestyle = new KmlLineStyleElement;
		if(!dest->linestyle)
			return;

		dest->linestyle->color		= src->linestyle->color;
		dest->linestyle->colormode	= src->linestyle->colormode;
		dest->linestyle->penwidth	= src->linestyle->penwidth;
	}

	if(src->ploystyle)
	{
		dest->ploystyle = new KmlPolyStyleElement;
		if(!dest->ploystyle)
			return;

		dest->ploystyle->color		= src->ploystyle->color;
		dest->ploystyle->colormode	= src->ploystyle->colormode;
		dest->ploystyle->fill		= src->ploystyle->fill;
		dest->ploystyle->outline	= src->ploystyle->outline;

		//by RSH 2013.8.1
		dest->ploystyle->polyTexture = src->ploystyle->polyTexture;
		//end
	}
}

void DeleteKmlStyleTree()
{
	KmlStyle *curr = GetStyleTreeRoot();
	KmlStyle *tail;

	while(curr)
	{
		tail = curr->next;

		//remove kml style object
		{
			if(curr->iconstyle)
				delete curr->iconstyle;
			if(curr->labelstyle)
				delete curr->labelstyle;
			if(curr->linestyle)
				delete curr->linestyle;
			if(curr->ploystyle)
				delete curr->ploystyle;
		}

		delete curr;

		curr = tail;
	}

	SetStyleTreeRoot(NULL);
}

KmlStyle * GetStyleTreeRoot ()
{
	return g_kmlstyle;
}

void  SetStyleTreeRoot (KmlStyle *root)
{
	g_kmlstyle = root;
}

KmlStyle * FindStyle (KmlStyle *collection, QString StyleMapUrl)
{
	QString  styleurl;
	KmlStyle *curr;
	KmlStyle *new_style;

	curr = collection;

	while (curr)
	{
		if (!StyleMapUrl.compare (curr->styleMapid))
		{
			styleurl = curr->normal_styleurl;
			
			curr = collection;

			while (curr)
			{
				if (!styleurl.compare (curr->styleid))
				{
					new_style = new_kmlstyle();
					if(!new_style)
						return NULL;

					copy_kml_style (curr, new_style);
					return new_style;
				}

				curr = curr->next;
			}

			return NULL;
		}
		else if(!StyleMapUrl.compare (curr->styleid))
		{
			new_style = new_kmlstyle();
			if(!new_style)
				return NULL;

			copy_kml_style (curr, new_style);
			return new_style;
		}

		curr = curr->next;
	}

	return NULL;
}

/********************************************************
	Parsing kmldom Objects (static functions)
********************************************************/
//--------------------------------------------------- Get Root Element
static const FeaturePtr GetRootFeature(const ElementPtr& root)
{
	const KmlPtr kml = kmldom::AsKml(root);

	if ((kml) && (kml->has_feature())) 
	{
		return kml->get_feature();
	}

	return kmldom::AsFeature(root);
}

//--------------------------------------------------- Container (Folder, Document)
static void VisitContainer(const ContainerPtr& container, KmlTreeItem* itemParent)
{
	int i;
	int asize = container->get_feature_array_size();

	for (i = 0; i < asize; ++i)
	{
		VisitFeature(container->get_feature_array_at(i), itemParent);
	}
}

//--------------------------------------------------- Feature (Folder, Document, PlaceMark)
static void VisitFeature(const FeaturePtr& feature, KmlTreeItem* itemParent)
{
	KmlTreeItem*	itemMy = NULL;

	/* For parsing feature styles */
#if KML_USE_FEATURE_STYLES == 1
	// Visit FeatureStyle
	if(!GetStyleTreeRoot())
		VisitFeatureStyle(feature);
#endif

	// Add Folder and Document
	if (const ContainerPtr container = kmldom::AsContainer(feature))
	{		
		// Add Folder
		if (const FolderPtr folder = kmldom::AsFolder(feature))
		{
			itemMy	= new KmlTreeItem(KML_IT_FOLDER);

			if (KmlParseDomObject(folder, itemMy) == 0)
			{
				itemMy->AddToKmlTree(itemParent);
			}
		}
		// Add Document
		else if (const DocumentPtr doc = kmldom::AsDocument(feature))
		{
			itemMy	= new KmlTreeItem(KML_IT_DOCUMET);

			if (KmlParseDomObject(doc, itemMy) == 0)
			{
				itemMy->AddToKmlTree(itemParent);
			}
		}

		// Visit childs
		VisitContainer(container, itemMy);
	}
	// Add PlaceMark
	else if (const PlacemarkPtr placemark = kmldom::AsPlacemark(feature))
	{		
		kmldom::PointPtr	p_point			= NULL;
		kmldom::PolygonPtr	p_polygon		= NULL;
		kmldom::LineStringPtr p_linestring	= NULL;

		if (p_point = kmldom::AsPoint(placemark->get_geometry()))
		{
			itemMy = new KmlTreeItem(KML_IT_PLACEMARK);
		}
		else if ((p_polygon = kmldom::AsPolygon(placemark->get_geometry())))
		{
			itemMy = new KmlTreeItem(KML_IT_POLYAREA);
		}
		else if (p_linestring = kmldom::AsLineString(placemark->get_geometry()))
		{
			itemMy = new KmlTreeItem(KML_IT_PATH);
		}
		else
		{
			itemMy = NULL;
			return;
		}
	
		//Set Style
		QString w_styleurl;
		KmlStyle *w_style;

		if(placemark->has_styleurl())
		{
			w_styleurl = KmlToQString(placemark->get_styleurl());
			w_styleurl = w_styleurl.right(w_styleurl.length()-strlen("#"));
			itemMy->SetStyleUrl(w_styleurl);
			w_style = FindStyle(GetStyleTreeRoot(), w_styleurl);
			if(w_style)
				itemMy->SetStyle(w_style);
		}

		//Parsing and adding to kmltree
		if (itemMy && KmlParseDomObject(placemark, itemMy) == 0)
		{
			itemMy->AddToKmlTree(itemParent);
		}

		/* For parsing feature styles */
#if KML_USE_FEATURE_STYLES == 1
		// Visit
		VisitPlacemark(placemark);  // Model
#endif
	}
	/* For parsing feature styles */
#if 0//KML_USE_FEATURE_STYLES == 1
	else if (const NetworkLinkPtr networklink = kmldom::AsNetworkLink(feature))
	{
		PrintNetworkLinkHref(networklink);
	} 
	else if (const OverlayPtr overlay = kmldom::AsOverlay(feature))
	{
		PrintOverlayIconHref(overlay);
	}
#endif

	// Change parent's visibility type
	if ((itemParent->GetVisibleType() != KML_VISTYPE_HIDE)
		&& ((itemMy->GetVisibleType() == KML_VISTYPE_HIDE) || (itemMy->GetVisibleType() == KML_VISTYPE_PARTIALLY)))
	{
		itemParent->SetVisibleType(KML_VISTYPE_PARTIALLY);
	}
}

//--------------------------------------------------- Feature Styles
/*
NOTE: (2009.08.20, RiSungChol)
	Now this code is not used, But this will be used for parsing feature styles in the future.
	So this code must be not deleted.
*/
/* For parsing feature styles */
#if KML_USE_FEATURE_STYLES == 1
static void VisitGeometry(const GeometryPtr& geometry)
{
	const MultiGeometryPtr multigeometry = kmldom::AsMultiGeometry(geometry);

	if (multigeometry)
	{
		int i;
		int asize = multigeometry->get_geometry_array_size();

		for (i = 0; i < asize; ++i)
		{
			VisitGeometry(multigeometry->get_geometry_array_at(i));
		}
	}
	/*
	else if (const ModelPtr model = kmldom::AsModel(geometry))
	{
	//PrintModelLinkHref(model);
	}
	*/
}

static void VisitPlacemark(const PlacemarkPtr& placemark)
{
	if (placemark->has_geometry())
	{
		VisitGeometry(placemark->get_geometry());
	}
}

static void VisitListStyle(const ListStylePtr& liststyle)
{
	int i;
	int asize = liststyle->get_itemicon_array_size();

	for (i = 0; i < asize; ++i)
	{
		// ItemIcon is not a BasicLink.
		if (liststyle->get_itemicon_array_at(i)->has_href())
		{
			//cout << "ItemIcon " << liststyle->get_itemicon_array_at(i)->get_href() << endl;
		}
	}
}

static void VisitStyle(const StylePtr& style, KmlStyle *pstyle)
{
	kmlbase::Color32	w_color;

	if(style->has_id())
		pstyle->styleid = KmlToQString(style->get_id());

	if (style->has_iconstyle())
	{
		kmldom::IconStylePtr p_iconstyle = NULL;
		p_iconstyle = style->get_iconstyle();

		pstyle->iconstyle = new KmlIconStyleElement;

		w_color = p_iconstyle->get_color();
		pstyle->iconstyle->color = w_color.get_color_argb();
		pstyle->iconstyle->scale = p_iconstyle->get_scale();
		pstyle->iconstyle->colormode = p_iconstyle->get_colormode();
		pstyle->iconstyle->heading = p_iconstyle->get_heading();

		//2013 3 17 by ugi
		if(p_iconstyle->has_icon())
		{
			kmldom::IconStyleIconPtr p_icon=p_iconstyle->get_icon();
			pstyle->iconstyle->iconname=KmlToQString(p_icon->get_href());
		}
		//2014 2 6 by ugi
		if(p_iconstyle->has_video_path())
		{
			kmldom::IconStyleIconPtr p_icon=p_iconstyle->get_icon();
			pstyle->iconstyle->videopath=KmlToQString(p_iconstyle->get_video_path());
		}
		//2014 2 7 by ugi
		if(p_iconstyle->has_html_path())
		{
			kmldom::IconStyleIconPtr p_icon=p_iconstyle->get_icon();
			pstyle->iconstyle->htmlpath=KmlToQString(p_iconstyle->get_html_path());
		}

	}

	if(style->has_labelstyle())
	{
		kmldom::LabelStylePtr p_labelstyle = NULL;
		p_labelstyle = style->get_labelstyle();

		pstyle->labelstyle = new KmlStyleElement;

		w_color = p_labelstyle->get_color();
		pstyle->labelstyle->color = w_color.get_color_argb();
		pstyle->labelstyle->scale = p_labelstyle->get_scale();
		pstyle->labelstyle->colormode = p_labelstyle->get_colormode();
	}

	if (style->has_polystyle())
	{
		kmldom::PolyStylePtr p_polystyle = NULL;
		p_polystyle = style->get_polystyle();

		pstyle->ploystyle = new KmlPolyStyleElement;

		w_color = p_polystyle->get_color();
		pstyle->ploystyle->color = w_color.get_color_argb();
		pstyle->ploystyle->colormode = p_polystyle->get_colormode();
		pstyle->ploystyle->outline = p_polystyle->get_outline();
		pstyle->ploystyle->fill = p_polystyle->get_fill();

		//by RSH 2013.8.1
		pstyle->ploystyle->polyTexture = KmlToQString(p_polystyle->get_polyTexture());
		//end
	}

	if(style->has_linestyle())
	{
		kmldom::LineStylePtr p_linestyle = NULL;
		p_linestyle = style->get_linestyle();

		pstyle->linestyle = new KmlLineStyleElement;

		w_color = p_linestyle->get_color();
		pstyle->linestyle->color = w_color.get_color_argb();
		pstyle->linestyle->penwidth = p_linestyle->get_width();
		pstyle->linestyle->colormode = p_linestyle->get_colormode();
	}

	if(style->has_liststyle())
	{
		VisitListStyle(style->get_liststyle());
	}
}

static void VisitStyleSelector(const StyleSelectorPtr& styleselector, KmlStyle *pstyle)
{
	if (const StylePtr style = kmldom::AsStyle(styleselector))
	{  // for IconStyle
		VisitStyle(style, pstyle);
	} else if (const StyleMapPtr stylemap = kmldom::AsStyleMap(styleselector))
	{
		VisitStyleMap(stylemap, pstyle);
	}
}

static void VisitStyleMap(const StyleMapPtr& stylemap, KmlStyle *pstyle)
{
	int i;
	int asize = stylemap->get_pair_array_size();
	QString		w_styleurl;

	pstyle->styleMapid = KmlToQString(stylemap->get_id());

	asize = asize >= MAX_STYLE_URL_NUM ? MAX_STYLE_URL_NUM : asize;
	for (i = 0; i < asize; ++i)
	{
 		kmldom::PairPtr p_pair = NULL;

 		p_pair = stylemap->get_pair_array_at(i);
		if (p_pair)
		{
			w_styleurl = KmlToQString(p_pair->get_styleurl());
			w_styleurl = w_styleurl.right(w_styleurl.length()-strlen("#"));

			if(NORMAL_STYLE_URL_POS == i)
			{
				pstyle->normal_styleurl = w_styleurl;
			}
			else if(HIGHLIGHT_STYLE_URL_POS == i)
			{
				pstyle->highlight_styleurl = w_styleurl;
			}
		}
		
#if 0
		if(p_pair->has_styleselector())
		{
			VisitStyleSelector(stylemap->get_pair_array_at(i)->get_styleselector());

		}
#endif
	}
}

static void VisitFeatureStyle(const FeaturePtr& feature)
{
	KmlStyle *p_style;

	// visit list if Document
	if (const DocumentPtr document = kmldom::AsDocument(feature))
	{
		int i;
		int asize = document->get_styleselector_array_size();

		for (i = 0; i < asize; ++i)
		{
			p_style = new_kmlstyle();
			if(!p_style)
				return;

			VisitStyleSelector(document->get_styleselector_array_at(i), p_style);

			AddToKmlStyleTree(p_style);
		}
	}
	else if (feature->has_styleselector())
	{
		p_style = new_kmlstyle();
		if(!p_style)
			return;

		VisitStyleSelector(feature->get_styleselector(), p_style);

		AddToKmlStyleTree(p_style);
	}

}
#endif // KML_USE_FEATURE_STYLES

/********************************************************
	Parse Kml data To Kml Tree
	IN:		kmlData			(kml data string)	
	OUT:	itemRoot		(Root Item of Kml Tree)	
********************************************************/
int KmlParse_DataToKmlTree(std::string kmlData, KmlTreeItem* itemRoot)
{
	//---------------------------------------------------- Root Element
	std::string errors;
	kmldom::ElementPtr root = kmldom::Parse(kmlData, &errors);

	if (!root)
	{
		return -1; // return error (Not exist Root element. -> No kml file)
	}

	if (const FeaturePtr rootFeature = GetRootFeature(root))
	{
		// START parsing Kml Data String
		VisitFeature(rootFeature, itemRoot);

		//remove global kml style object
		DeleteKmlStyleTree();
	}
	else
	{
		return -1; // return error (Not exist Root element. -> No kml file)
	}

	return 0; // return ok
}
