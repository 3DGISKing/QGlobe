#include "kml_treeitem.h"
#include "kml_parser.h"
#include "GIS_Doc.h"
#include "GIS_PlaceIcon.h"
#include "GIS_Path.h"
#include "GIS_Area.h"

// static functions
// Convert QString to UShort Array
static void QStringToUShortArray(const QString &srcStr, ushort *dstArray, int maxLen)
{
	const ushort *srcData = srcStr.utf16();

	int textLen = (maxLen > 0) ? qMin(srcStr.length(), maxLen - 1) : srcStr.length();

	memcpy(dstArray, srcData, textLen * sizeof(ushort));
	dstArray[textLen] = 0;
}

KmlTreeItem::KmlTreeItem()
{ 
	m_type			= KML_IT_UNKNOWN;	// Default
	m_name			= "";
	m_desc			= "";
	m_styleurl		= "";
	m_visibletype	= KML_VISTYPE_SHOW;
	m_startLocation	= false;
	m_style			= NULL;
	m_lookat		= NULL;
	m_coordpoint	= NULL;
	m_coordpoint_array_size = 0;
	m_extrude		= false;
	m_altitudeMode	= KML_ALTITUDEMODE_CLAMPED_TO_GROUND;

	m_childHead		= NULL; 
	m_childTail		= NULL; 
	m_next			= NULL;	
}

KmlTreeItem::KmlTreeItem(KmlTreeItemType type)
{
	m_type			= type;
	m_name			= "";
	m_desc			= "";
	m_styleurl		= "";
	m_visibletype	= KML_VISTYPE_SHOW;
	m_startLocation	= false;
	m_style			= NULL;
	m_lookat		= NULL;
	m_coordpoint	= NULL;	
	m_coordpoint_array_size = 0;
	m_extrude		= false;
	m_altitudeMode	= KML_ALTITUDEMODE_CLAMPED_TO_GROUND;

	m_childHead		= NULL; 
	m_childTail		= NULL; 
	m_next			= NULL;	
}

KmlTreeItem::KmlTreeItem(KmlTreeItemType type, QString name, QString desc, QString styleurl, KmlVisibeType visibletype, KmlLookAt* lookat, KmlCoordPoint* coordpoint, unsigned int color, float penwidth)
{
	m_type			= type;
	m_name			= name;
	m_desc			= desc;
	m_styleurl		= styleurl;
	m_visibletype	= visibletype;
	m_startLocation	= false;
	m_style			= NULL;
	m_lookat		= lookat;
	m_coordpoint	= coordpoint;
	m_coordpoint_array_size = 0;
	m_extrude		= false;
	m_altitudeMode	= KML_ALTITUDEMODE_CLAMPED_TO_GROUND;
	
	m_childHead		= NULL; 
	m_childTail		= NULL; 
	m_next			= NULL;	
}

KmlTreeItem::~KmlTreeItem()
{
	// Free Memory
	if (m_lookat != NULL)
		delete m_lookat;

	if (m_coordpoint != NULL)
		delete m_coordpoint;

	if(m_style != NULL)
	{
		if(m_style->iconstyle)
			delete m_style->iconstyle;
		if(m_style->labelstyle)
			delete m_style->labelstyle;
		if(m_style->linestyle)
			delete m_style->linestyle;
		if(m_style->ploystyle)
			delete m_style->ploystyle;

		delete m_style;
	}
}

/********************************************************
	Set Type property
********************************************************/
void KmlTreeItem::SetType(const KmlTreeItemType type)
{
	m_type = type;
}

/********************************************************
	Get Type property
********************************************************/
KmlTreeItemType KmlTreeItem::GetType()
{
	return m_type;
}

/********************************************************
	Set SubType property
********************************************************/
void KmlTreeItem::SetSubType(const E_GDM_SUBDATA_TYPE subtype)
{
	m_subtype = subtype;
}

/********************************************************
	Get SubType property
********************************************************/
E_GDM_SUBDATA_TYPE KmlTreeItem::GetSubType()
{
	return m_subtype;
}

/********************************************************
	Set Name property
********************************************************/
void KmlTreeItem::SetName(const QString name)
{
	m_name = name;
}

/********************************************************
	Get Name property
********************************************************/
void KmlTreeItem::GetName(QString& name)
{	
	name = m_name;
}

/********************************************************
	Set Description property
********************************************************/
void KmlTreeItem::SetDescription(const QString desc)
{
	m_desc = desc;
}


/********************************************************
	Get Description property
********************************************************/
void KmlTreeItem::GetDescription(QString& desc)
{
	desc = m_desc;
}

/********************************************************
Set StyleUrl property
********************************************************/
void KmlTreeItem::SetStyleUrl(const QString styleurl)
{
	m_styleurl = styleurl;
}


/********************************************************
Get StyleUrl property
********************************************************/
void KmlTreeItem::GetStyleUrl(QString& styleurl)
{
	styleurl = m_styleurl;
}


/********************************************************
	Set Visibility Type property
********************************************************/
void KmlTreeItem::SetVisibleType(const KmlVisibeType visibletype)
{	
	m_visibletype = visibletype;
}

/********************************************************
	Get Visibility Type property
********************************************************/
KmlVisibeType KmlTreeItem::GetVisibleType()
{
	return m_visibletype;
}

/********************************************************
	Set Starting Location Flag property
********************************************************/
void KmlTreeItem::SetStartLocation(const bool startLocation)
{
	m_startLocation = startLocation;
}

/********************************************************
	Get Starting Location Flag property
********************************************************/
bool KmlTreeItem::GetStartLocation()
{
	return m_startLocation;
}

/********************************************************
Set Style property
********************************************************/
void KmlTreeItem::SetStyle(KmlStyle* style)
{
	// Delete
	if ((style == NULL) && (m_style != NULL))
		delete m_style;

	// Set
	m_style = style;
}

/********************************************************
Get Style property
********************************************************/
KmlStyle * KmlTreeItem::GetStyle()
{
	return m_style;
}

/********************************************************
	Set LookAt property
********************************************************/
void KmlTreeItem::SetLookAt(KmlLookAt* lookat)
{
	// Delete
	if ((lookat == NULL) && (m_lookat != NULL))
		delete m_lookat;

	// Set
	m_lookat = lookat;
}

/********************************************************
	Get LookAt property
********************************************************/
KmlLookAt* KmlTreeItem::GetLookAt()
{
	return m_lookat;
}

/********************************************************
	Set Coordinates Point property
********************************************************/
void KmlTreeItem::SetCoordPoint(KmlCoordPoint* coordpoint)
{
	// Delete
	if ((coordpoint == NULL) && (m_coordpoint != NULL))
		delete m_coordpoint;

	// Set
	m_coordpoint = coordpoint;
}

/********************************************************
Set Coordinates Point Array Size
********************************************************/
void KmlTreeItem::SetCoordPointArraySize(int array_size)
{
	m_coordpoint_array_size = array_size;
}

/********************************************************
	Get Coordinate Point property
********************************************************/
KmlCoordPoint* KmlTreeItem::GetCoordPoint()
{
	return m_coordpoint;
}

/********************************************************
	Add My KmlTreeItem to Kml Tree
********************************************************/
int KmlTreeItem::AddToKmlTree(KmlTreeItem* itemParent)
{
	if (itemParent == NULL)
		return -1;	// return error (Pointer to parenet item is NULL.)

	// Add first Child item
	if (itemParent->m_childTail == NULL)
	{
		itemParent->m_childHead			= this;
		itemParent->m_childTail			= this;
	}	
	// Child item is already exist
	else
	{		
		itemParent->m_childTail->m_next	= this;
		itemParent->m_childTail			= this;	
	}

	return 0;	// return ok
}

/********************************************************
	Convert KmlTreeItem data ---> TreeItem data
********************************************************/
int KmlTreeItem::ConvertToTreeItemData(PLACEMARK_DATA* tdata)
{
	// Type
	if (m_type == KML_IT_FOLDER)			// Folder
	{
		tdata->itemtype = IT_FOLDER;
	}
	else if (m_type == KML_IT_DOCUMET)		// Document
	{
		tdata->itemtype = IT_DOC;
	}
	else if (m_type == KML_IT_PLACESEARCH)	// Place (Search)
	{
		tdata->itemtype = IT_SEARCH;
	}
	else if (m_type == KML_IT_PLACEMARK)	// Place (User Add)
	{
		tdata->itemtype = IT_PLACEMARK;
	}
	else if (m_type == KML_IT_PATH)			// Path
	{
		tdata->itemtype = IT_PATH;
	}
	else if (m_type == KML_IT_POLYAREA)		// Polygon
	{
		tdata->itemtype = IT_POLYAREA;
	}
	else if (m_type == KML_IT_PAGE)			// Page
	{
		tdata->itemtype = IT_PAGE;
	}
	else if (m_type == KML_IT_UNKNOWN)		// Unkown
	{
		// No process
	}

	// SubType
	tdata->type = m_subtype;

	// Name, Description	
	QStringToUShortArray(m_name, tdata->name,			TREEITEM_NAME_STR_MAX_LEN);
	QStringToUShortArray(m_desc, tdata->description,	TREEITEM_DESC_STR_MAX_LEN);	

	// Visibility (Check) Flag
	tdata->showmark =  (m_visibletype == KML_VISTYPE_HIDE)		?	(SMT_HIDE)
					: ((m_visibletype == KML_VISTYPE_PARTIALLY) ?	(SMT_PARTIALLY)
					:												(SMT_SHOW));

	// Starting Location Flag
	tdata->start = m_startLocation;

	// Snapshot
	tdata->snapshot = (m_lookat != NULL) ? true : false;

	// LookAt		(Only snapshot)
	if (m_lookat != NULL)
	{
		tdata->view_longitude	= m_lookat->m_longitude;
		tdata->view_latitude	= m_lookat->m_latitude;
		tdata->view_heading		= (-1) * m_lookat->m_heading;	// for GoogleEarth
		tdata->view_range		= m_lookat->m_range; 
		tdata->view_tilt		= m_lookat->m_tilt; 
	}
	// Default		(Only PlaceSearch or PlaceMark)
	//else if ((m_type == KML_IT_PLACESEARCH) || (m_type == KML_IT_PLACEMARK))
	else if(m_coordpoint)
	{
		tdata->view_longitude	= m_coordpoint->m_longitude;
		tdata->view_latitude	= m_coordpoint->m_latitude;
		tdata->view_heading		= 0; 
		tdata->view_range		= 0; 
		tdata->view_tilt		= 0; 
	}

	// CoordPoint	(Only PlaceMark (User Add AND Search))
	if (m_coordpoint != NULL)
	{
		tdata->longitude	= m_coordpoint->m_longitude;
		tdata->latitude		= m_coordpoint->m_latitude;
		tdata->altitude		= m_coordpoint->m_altitude;
	}

	// create gis object
	CGIS_Doc *pGisDoc = gdm_GetGISDoc();
	if (m_type == KML_IT_PLACEMARK)	// Place (User Add)
	{
		CGIS_PlaceIcon *pNode = new CGIS_PlaceIcon();
		pNode->SetKey(tdata->key);

		//by RSH 2013.7.20
		if (!m_style->iconstyle->iconname.isEmpty())
		{
			QStringToUShortArray(m_style->iconstyle->iconname, tdata->iconname,	TREEITEM_NAME_STR_MAX_LEN);
			memcpy(pNode->m_sIconName, tdata->iconname, PMD_STR_MAX_SIZE);
		}
		else
			pNode->m_sIconName[0] = 0; //end edition by RSH

		//2014 2 6 by ugi
		if (!m_style->iconstyle->videopath.isEmpty())
		{
			QStringToUShortArray(m_style->iconstyle->videopath, tdata->videopath,	TREEITEM_NAME_STR_MAX_LEN);
			memcpy(pNode->m_sVideoPath, tdata->videopath, PMD_STR_MAX_SIZE);
		}
		else
			pNode->m_sVideoPath[0] = 0; //end 

		//2014 2 7 by ugi
		if (!m_style->iconstyle->htmlpath.isEmpty())
		{
			QStringToUShortArray(m_style->iconstyle->htmlpath, tdata->htmlpath,	TREEITEM_NAME_STR_MAX_LEN);
			memcpy(pNode->m_sHtmlPath, tdata->htmlpath, PMD_STR_MAX_SIZE);
		}
		else
			pNode->m_sHtmlPath[0] = 0; //end 



		pNode->m_IconScale=tdata->iconscale=m_style->iconstyle->scale;	//by ugi 2013.8.1
	
		// name
		memcpy(pNode->m_sName, tdata->name, PMD_STR_MAX_SIZE);

		// set location
		pNode->SetPt(m_coordpoint->m_longitude * PAI / 180, 
			m_coordpoint->m_latitude * PAI / 180, m_coordpoint->m_altitude);

		// set pen

		// add node
		pGisDoc->AddNode(pNode);
	}
	else if (m_type == KML_IT_PATH)			// Path
	{
		CGIS_Path *pNode = new CGIS_Path();
		pNode->SetKey(tdata->key);

		// set pen
		if(m_style)
		{
			if(m_style->linestyle)
			{
				pNode->m_sPen.color		= m_style->linestyle->color;
				pNode->m_sPen.width		= m_style->linestyle->penwidth;
			}
			else
			{
				pNode->m_sPen.color		= 0xffffffff;
				pNode->m_sPen.width		= 1.0;
			}

			// set brush
			if(m_style->ploystyle)
			{
				pNode->m_sBrush.color	= m_style->ploystyle->color;

				if(m_style->ploystyle->fill && m_style->ploystyle->outline)
				{
					pNode->m_nDrawType = E_GISDRAWPOLY_ALL;
				}
				else if(m_style->ploystyle->fill)
				{
					pNode->m_nDrawType = E_GISDRAWPOLY_FILL;
				}
				else if(m_style->ploystyle->outline)
				{
					pNode->m_nDrawType = E_GISDRAWPOLY_LINE;
				}
				else
				{
					pNode->m_nDrawType = E_GISDRAWPOLY_NONE;
				}
			}
			else
			{
				pNode->m_sBrush.color	= 0xffffffff;
				pNode->m_nDrawType		= E_GISDRAWPOLY_ALL;
			}
		}

		KmlAltitudeMode altitudeMode = GetAltitudeMode();
		switch (altitudeMode)
		{
		case KML_ALTITUDEMODE_CLAMPED_TO_GROUND:
		case KML_ALTITUDEMODE_CLAMPED_TO_SEA:
			pNode->m_nHeightType = E_GISPOLYHEIGHT_CLAMPED_TO_GROUND;
			break;
		case KML_ALTITUDEMODE_RELATIVE_TO_GROUND:
		case KML_ALTITUDEMODE_RELATIVE_TO_SEA:
			pNode->m_nHeightType = E_GISPOLYHEIGHT_RELATIVE_TO_GROUND;
			break;
		case KML_ALTITUDEMODE_ABSOLUTE:
			pNode->m_nHeightType = E_GISPOLYHEIGHT_ABSOLUTE;
			break;
		}

		//Setting Extrude
		pNode->m_blHeight = GetExtrude();
		if(pNode->m_blHeight)
			pNode->m_dHeight = m_coordpoint[0].m_altitude;

		// set polygon
		CGDM_Location3D w_Pt;
		int i;
		for (i = 0; i < m_coordpoint_array_size; i++)
		{
			w_Pt.m_tX = m_coordpoint[i].m_longitude * PAI / 180;
			w_Pt.m_tY = m_coordpoint[i].m_latitude * PAI / 180;
			w_Pt.m_tZ = m_coordpoint[i].m_altitude;

			pNode->AddPoint(w_Pt);
		}

		// check height
		double dHeight = 0;
		if(m_coordpoint_array_size > 0)
		{
			dHeight = m_coordpoint[0].m_altitude;
			for(i = 1; i < m_coordpoint_array_size; i++)
			{
				if(m_coordpoint[i].m_altitude != dHeight)
					dHeight = 0;
			}
		}
		pNode->m_dHeight = dHeight;

		pNode->Refresh();

		// add node
		pGisDoc->AddNode(pNode);
	}
	else if (m_type == KML_IT_POLYAREA)		// Polygon
	{
		CGIS_Area *pNode = new CGIS_Area();
		pNode->SetKey(tdata->key);

		if(m_style)
		{
			// set pen
			if(m_style->linestyle)
			{
				pNode->m_sPen.color		= m_style->linestyle->color;
				pNode->m_sPen.width		= m_style->linestyle->penwidth;
			}
			else
			{
				pNode->m_sPen.color		= 0xffffffff;
				pNode->m_sPen.width		= 1.0;
			}

			// set brush
			if(m_style->ploystyle)
			{
				pNode->m_sBrush.color	= m_style->ploystyle->color;

				if(m_style->ploystyle->fill && m_style->ploystyle->outline)
				{
					pNode->m_nDrawType = E_GISDRAWPOLY_ALL;
				}
				else if(m_style->ploystyle->fill)
				{
					pNode->m_nDrawType = E_GISDRAWPOLY_FILL;
				}
				else if(m_style->ploystyle->outline)
				{
					pNode->m_nDrawType = E_GISDRAWPOLY_LINE;
				}
				else
				{
					pNode->m_nDrawType = E_GISDRAWPOLY_NONE;
				}

				//by RSH 2013.8.1
				///*
				if (!m_style->ploystyle->polyTexture.isEmpty())
				{
					QStringToUShortArray(m_style->ploystyle->polyTexture, tdata->iconname,	TREEITEM_NAME_STR_MAX_LEN);
					memcpy(pNode->m_sTextureName, tdata->iconname, PMD_STR_MAX_SIZE);
				}
				else
					pNode->m_sTextureName[0] = 0;
				//*/
				//end edition by RSH
			}
			else
			{
				pNode->m_sBrush.color	= 0xffffffff;
				pNode->m_nDrawType		= E_GISDRAWPOLY_ALL;
			}
		}

		KmlAltitudeMode altitudeMode = GetAltitudeMode();
		switch (altitudeMode)
		{
		case KML_ALTITUDEMODE_CLAMPED_TO_GROUND:
		case KML_ALTITUDEMODE_CLAMPED_TO_SEA:
			pNode->m_nHeightType = E_GISPOLYHEIGHT_CLAMPED_TO_GROUND;
			break;
		case KML_ALTITUDEMODE_RELATIVE_TO_GROUND:
		case KML_ALTITUDEMODE_RELATIVE_TO_SEA:
			pNode->m_nHeightType = E_GISPOLYHEIGHT_RELATIVE_TO_GROUND;
			break;
		case KML_ALTITUDEMODE_ABSOLUTE:
			pNode->m_nHeightType = E_GISPOLYHEIGHT_ABSOLUTE;
			break;
		}

		//Setting Extrude
		pNode->m_blHeight = GetExtrude();
		if(pNode->m_blHeight)
			pNode->m_dHeight = m_coordpoint[0].m_altitude;

		// set polygon
		CGDM_Location3D w_Pt;
		int i;
		for (i = 0; i < m_coordpoint_array_size; i++)
		{
			w_Pt.m_tX = m_coordpoint[i].m_longitude * PAI / 180;
			w_Pt.m_tY = m_coordpoint[i].m_latitude * PAI / 180;
			w_Pt.m_tZ = m_coordpoint[i].m_altitude;

			pNode->AddPoint(w_Pt);
		}

		// check height
		double dHeight = 0;
		if(m_coordpoint_array_size > 0)
		{
			dHeight = m_coordpoint[0].m_altitude;
			for(i = 1; i < m_coordpoint_array_size; i++)
			{
				if(m_coordpoint[i].m_altitude != dHeight)
					dHeight = 0;
			}
		}
		pNode->m_dHeight = dHeight;

		pNode->Refresh();

		// add node
		pGisDoc->AddNode(pNode);
	}

	return 0;	// return ok
}

QString StyleUrlGenerator()
{
	static unsigned int count = 0;

	QString new_styleurl;
	QString rtn_str;
	QString strTemp;
	
	strTemp = strTemp.sprintf("%d", count);
	rtn_str.append(new_styleurl.fromUtf8(PLACEMARK_STYLE_URL_PREFIX_STRING));
	rtn_str.append(new_styleurl.fromUtf8(STYLE_URL_BASE_STRING));
	rtn_str.append(strTemp);

	count ++;

	return rtn_str;
}

QString GetNormalStyleUrl(QString StylemapUrl)
{
	QString new_styleurl;

	StylemapUrl= StylemapUrl.right(StylemapUrl.length()-strlen(PLACEMARK_STYLE_URL_PREFIX_STRING));
	StylemapUrl.prepend(new_styleurl.fromUtf8(NORMAL_STYLE_URL_PREFIX_STRING));

	return StylemapUrl;
}

QString GetHighlightStyleUrl(QString StylemapUrl)
{
	QString new_styleurl;

	StylemapUrl = StylemapUrl.right(StylemapUrl.length()-strlen(PLACEMARK_STYLE_URL_PREFIX_STRING));
	StylemapUrl.prepend(new_styleurl.fromUtf8(HIGHLIGHT_STYLE_URL_PREFIX_STRING));

	return StylemapUrl;
}

/********************************************************
	Convert TreeItem data ---> KmlTreeItem data
********************************************************/
int KmlTreeItem::ConvertFromTreeItemData(PLACEMARK_DATA* tdata)
{
	if(!tdata)
		return -1;

	// Type	
	if (tdata->itemtype == IT_FOLDER)			// Folder
	{
		m_type = KML_IT_FOLDER;
	}
	else if (tdata->itemtype == IT_DOC)			// Document
	{
		m_type = KML_IT_DOCUMET;
	}
	else if (tdata->itemtype == IT_SEARCH)		// Place (Search)
	{
		m_type = KML_IT_PLACESEARCH;
	}
	else if (tdata->itemtype == IT_PLACEMARK)	// Place (User Add)
	{
		m_type = KML_IT_PLACEMARK;

		//Set StyleUrl
		SetStyleUrl(StyleUrlGenerator());
	}
	else if (tdata->itemtype == IT_PATH)	    // Path(User Add)
	{
		m_type = KML_IT_PATH;

		//Set StyleUrl
		SetStyleUrl(StyleUrlGenerator());
	}
	else if (tdata->itemtype == IT_POLYAREA)	    // Polygon(User Add)
	{
		m_type = KML_IT_POLYAREA;

		//Set StyleUrl
		SetStyleUrl(StyleUrlGenerator());
	}
	else if (tdata->itemtype == IT_PAGE)		// Page
	{
		m_type = KML_IT_PAGE;
		return -1;	// return error (Page Object is ignored)
	}
	else										// Unkown
	{
		m_type = KML_IT_UNKNOWN;
	}

	// SubType
	m_subtype = (E_GDM_SUBDATA_TYPE)tdata->type;

	// Name, Description
	m_name = QString::fromUtf16(tdata->name,		TREEITEM_NAME_STR_MAX_LEN);
	m_desc = QString::fromUtf16(tdata->description, TREEITEM_DESC_STR_MAX_LEN);

	// Visibility (Check) Flag
	m_visibletype =  (tdata->showmark == SMT_HIDE)		?	(KML_VISTYPE_HIDE)
				  : ((tdata->showmark == SMT_PARTIALLY) ?	(KML_VISTYPE_PARTIALLY)
				  :											(KML_VISTYPE_SHOW));

	// Starting Location Flag
	m_startLocation = tdata->start;

	// LookAt		(snapshot or PlaceSearch or PlaceMark)
	if ((tdata->snapshot) || (m_type == KML_IT_PLACESEARCH) || 
		(m_type == KML_IT_PLACEMARK) || (m_type == KML_IT_POLYAREA) ||
		(m_type == KML_IT_PATH))
	{
		// LookAt
		if (m_lookat == NULL)
			m_lookat = new KmlLookAt();

		m_lookat->m_longitude	= tdata->view_longitude;
		m_lookat->m_latitude	= tdata->view_latitude;
		m_lookat->m_heading		= (-1) * tdata->view_heading;	// for GoogleEarth
		m_lookat->m_range		= tdata->view_range;
		m_lookat->m_tilt		= tdata->view_tilt;

		m_lookat->m_altitudeMode= KML_ALTITUDEMODE_RELATIVE_TO_GROUND; // FIXED		

		if (KML_IT_PLACESEARCH == m_type)
		{
			if(!m_coordpoint)
				m_coordpoint = new KmlCoordPoint();

			m_coordpoint->m_longitude	= m_lookat->m_longitude;
			m_coordpoint->m_latitude	= m_lookat->m_latitude;
			m_coordpoint->m_altitude	= m_lookat->m_altitude;
			SetCoordPointArraySize(1);
		}
	}

	// export gis object
	CGIS_Doc *pGisDoc = gdm_GetGISDoc();
	if (m_type == KML_IT_PLACEMARK)
	{
		CGIS_PlaceIcon *pNode = (CGIS_PlaceIcon*)pGisDoc->GetNodeFromKey(tdata->key);

		if(!pNode)
			return -1;

		//create kml style tree
		KmlStyle *new_style = new_kmlstyle();
		new_style->iconstyle = new KmlIconStyleElement;
		new_style->labelstyle = new KmlStyleElement;

		if(!new_style->iconstyle || !new_style->labelstyle)
			return -1;

		GetStyleUrl(new_style->styleid);
		GetStyleUrl(new_style->styleMapid);
		new_style->normal_styleurl = GetNormalStyleUrl(new_style->styleMapid);
		new_style->highlight_styleurl = GetHighlightStyleUrl(new_style->styleMapid);

		new_style->iconstyle->color = pNode->m_sPen.color;
		new_style->iconstyle->penwidth = pNode->m_sPen.width;
/*		new_style->iconstyle->colormode = pNode->;
		new_style->iconstyle->heading = pNode->;
		new_style->iconstyle->iconname = pNode->;*/
		new_style->iconstyle->scale = pNode->m_IconScale;	//by ugi 2013.8.1

		new_style->labelstyle->color = pNode->m_sPen.color;
/*		new_style->labelstyle->colormode = pNode->;
		new_style->labelstyle->penwidth = pNode->;
		new_style->labelstyle->scale = pNode->;	*/

		//by RSH 2013.7.22
		new_style->iconstyle->iconname.setUtf16(pNode->m_sIconName, PMD_STR_MAX_SIZE);
		//end by RSH
		new_style->iconstyle->videopath.setUtf16(pNode->m_sVideoPath, PMD_STR_MAX_SIZE);	//2014 2 6 by ugi
		new_style->iconstyle->htmlpath.setUtf16(pNode->m_sHtmlPath, PMD_STR_MAX_SIZE);	//2014 2 7 by ugi
		
		SetStyle(new_style);

		//Add to KmlStyleTree
		KmlStyle *added_style = new_kmlstyle();
		copy_kml_style(new_style, added_style);
		AddToKmlStyleTree(added_style);

		// set polygon
		KmlCoordPoint *p_coord = new KmlCoordPoint;
		if(!p_coord)
			return -1;

		CGDM_Location3D	pt = pNode->GetPt();
		p_coord->m_longitude = pt.m_tX * 180 / PAI;
		p_coord->m_latitude	= pt.m_tY * 180 / PAI;
		p_coord->m_altitude	= pt.m_tZ;

		SetCoordPoint(p_coord);
		SetCoordPointArraySize(1);
	}
	else if (m_type == KML_IT_PATH)			// Path
	{
		CGIS_Path *pNode = (CGIS_Path*)pGisDoc->GetNodeFromKey(tdata->key);

		if(!pNode)
			return -1;

		//create kml style tree
		KmlStyle *new_style = new_kmlstyle();
		//new_style->iconstyle = new KmlIconStyleElement;
		new_style->linestyle = new KmlLineStyleElement;
		new_style->ploystyle = new KmlPolyStyleElement;

		if(/*!new_style->iconstyle || */!new_style->linestyle || !new_style->ploystyle)
			return -1;

		SetExtrude(pNode->m_blHeight);

		GetStyleUrl(new_style->styleid);
		GetStyleUrl(new_style->styleMapid);
		new_style->normal_styleurl = GetNormalStyleUrl(new_style->styleMapid);
		new_style->highlight_styleurl = GetHighlightStyleUrl(new_style->styleMapid);

/*		new_style->iconstyle->color = pNode->m_sPen.color;
		new_style->iconstyle->penwidth = pNode->m_sPen.width;
		new_style->iconstyle->colormode = pNode->;
		new_style->iconstyle->heading = pNode->;
		new_style->iconstyle->iconname = pNode->;
		new_style->iconstyle->scale = pNode->;	*/

		new_style->linestyle->color		= pNode->m_sPen.color;
		new_style->linestyle->penwidth	= pNode->m_sPen.width;
		new_style->linestyle->colormode	= 0;

		new_style->ploystyle->color		= pNode->m_sBrush.color;
		/*new_style->ploystyle->colormode = 0;
		new_style->ploystyle->fill		= (E_GISDRAWPOLY_FILL == pNode->m_nDrawType || E_GISDRAWPOLY_ALL == pNode->m_nDrawType ) ? true : false;
		new_style->ploystyle->outline	= (E_GISDRAWPOLY_LINE == pNode->m_nDrawType || E_GISDRAWPOLY_ALL == pNode->m_nDrawType ) ? true : false;*/
		
		SetStyle(new_style);

		//Add to KmlStyleTree
		KmlStyle *added_style = new_kmlstyle();
		copy_kml_style(new_style, added_style);
		AddToKmlStyleTree(added_style);

		// set polygon
		int coordnum = pNode->m_PtArr.size();

		if(0 >= coordnum)
			return 0;

		KmlCoordPoint *p_coord = new KmlCoordPoint[coordnum];
		if(!p_coord)
			return -1;

		for(int i = 0; i < coordnum; i++)
		{
			p_coord[i].m_longitude = pNode->m_PtArr[i].m_tX * 180 / PAI;
			p_coord[i].m_latitude	= pNode->m_PtArr[i].m_tY * 180 / PAI;
			p_coord[i].m_altitude	= pNode->m_dHeight/*pNode->m_PtArr[i].m_tZ*/;
		}

		SetCoordPoint(p_coord);
		SetCoordPointArraySize(coordnum);

		switch (pNode->m_nHeightType)
		{
		case E_GISPOLYHEIGHT_CLAMPED_TO_GROUND:
			SetAltitudeMode(KML_ALTITUDEMODE_CLAMPED_TO_GROUND);
			break;
		case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
			SetAltitudeMode(KML_ALTITUDEMODE_RELATIVE_TO_GROUND);
			break;
		case E_GISPOLYHEIGHT_ABSOLUTE:
			SetAltitudeMode(KML_ALTITUDEMODE_ABSOLUTE);
			break;
		}
	}
	else if (m_type == KML_IT_POLYAREA)		// Polygon
	{
		CGIS_Area *pNode = (CGIS_Area*)pGisDoc->GetNodeFromKey(tdata->key);

		if(!pNode)
			return -1;

		//create kml style tree
		KmlStyle *new_style = new_kmlstyle();
		
		new_style->linestyle = new KmlLineStyleElement;
		new_style->ploystyle = new KmlPolyStyleElement;

		if(!new_style->linestyle || !new_style->ploystyle)
			return -1;
		
		SetExtrude(pNode->m_blHeight);

		GetStyleUrl(new_style->styleid);
		GetStyleUrl(new_style->styleMapid);
		new_style->normal_styleurl = GetNormalStyleUrl(new_style->styleMapid);
		new_style->highlight_styleurl = GetHighlightStyleUrl(new_style->styleMapid);

		new_style->linestyle->color		= pNode->m_sPen.color;
		new_style->linestyle->penwidth	= pNode->m_sPen.width;
		new_style->linestyle->colormode	= 0;

		new_style->ploystyle->color		= pNode->m_sBrush.color;
		new_style->ploystyle->colormode = 0;
		new_style->ploystyle->fill		= (E_GISDRAWPOLY_FILL == pNode->m_nDrawType || E_GISDRAWPOLY_ALL == pNode->m_nDrawType ) ? true : false;
		new_style->ploystyle->outline	= (E_GISDRAWPOLY_LINE == pNode->m_nDrawType || E_GISDRAWPOLY_ALL == pNode->m_nDrawType ) ? true : false;
		
		//by RSH 2013.8.1
		new_style->ploystyle->polyTexture.setUtf16(pNode->m_sTextureName, PMD_STR_MAX_SIZE);
		//end edition

		SetStyle(new_style);

		//Add to KmlStyleTree
		KmlStyle *added_style = new_kmlstyle();
		copy_kml_style(new_style, added_style);
		AddToKmlStyleTree(added_style);

		// set polygon
		int coordnum = pNode->m_PtArr.size();

		if(0 >= coordnum)
			return 0;

		KmlCoordPoint *p_coord = new KmlCoordPoint[coordnum];
		if(!p_coord)
			return -1;

		for(int i = 0; i < coordnum; i++)
		{
			p_coord[i].m_longitude = pNode->m_PtArr[i].m_tX * 180 / PAI;
			p_coord[i].m_latitude	= pNode->m_PtArr[i].m_tY * 180 / PAI;
			p_coord[i].m_altitude	= pNode->m_dHeight/*pNode->m_PtArr[i].m_tZ*/;
		}
	
		SetCoordPoint(p_coord);
		SetCoordPointArraySize(coordnum);

		switch (pNode->m_nHeightType)
		{
		case E_GISPOLYHEIGHT_CLAMPED_TO_GROUND:
			SetAltitudeMode(KML_ALTITUDEMODE_CLAMPED_TO_GROUND);
			break;
		case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
			SetAltitudeMode(KML_ALTITUDEMODE_RELATIVE_TO_GROUND);
			break;
		case E_GISPOLYHEIGHT_ABSOLUTE:
			SetAltitudeMode(KML_ALTITUDEMODE_ABSOLUTE);
			break;
		}

	}

	return 0;	// return ok
}
