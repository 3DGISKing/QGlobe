#include "kmlmanagerclient.h"
#include "kml_treeitem.h"
#include "kml_objects.h"
#include "kml_parser.h"
#include <QString>
/********************************************************
	Kml Default Starting Location Object
********************************************************/
#define ID_STR_DEFAULT_STARTING_LOCATION	"default_starting_location"
const char ID_STR_UNKNOWN_NAME_STRING[] = {0x5B, 0xEC, 0x9D, 0xB4, 0xEB, 0xA6, 0x84, 0xEC, 0x97, 0x86, 0xEC, 0x9D, 0x8C, 0x5D }; // "[이름없음]"

//--------------------------------------------------- Check Kml Default Starting Location Object
static bool IsKmlStaringtLocationObj(const kmldom::FeaturePtr feature)
{
	if (feature == NULL)
		return false;	// return NO

	// Check Id
	if (feature->has_id())
	{
		if (feature->get_id().compare(ID_STR_DEFAULT_STARTING_LOCATION) == 0)
		{
			return true;	// return YES
		}
	}

	return false;	// return NO
}

//--------------------------------------------------- Set Kml Default Starting Location Object
static int SetKmlStaringtLocationObj(const kmldom::FeaturePtr feature)
{
	if (feature == NULL)
		return -1;	// return error (Null pointer)

	// Set Id
	feature->set_id(ID_STR_DEFAULT_STARTING_LOCATION);

	return 0;	// return ok
}

/********************************************************
	Get/Set SubType of PlaceMark(Search) object
********************************************************/
//--------------------------------------------------- Check PlaceSearch Object and Get subtype
static bool GetPlaceSubType(const kmldom::FeaturePtr feature, E_GDM_SUBDATA_TYPE* subtype)
{
	if (feature == NULL)
		return false;	// return NO

	// Check Id
	if (feature->has_id())
	{
		if (feature->get_id().compare(ID_STR_DEFAULT_STARTING_LOCATION) != 0)
		{
			*subtype = (E_GDM_SUBDATA_TYPE)(atoi(feature->get_id().c_str()));

			return true;	// return YES
		}
	}

	return false;	// return NO
}

//--------------------------------------------------- Set Subtype
static int SetPlaceSubType(const kmldom::FeaturePtr feature, E_GDM_SUBDATA_TYPE subtype)
{
	if (feature == NULL)
		return -1;	// return error (Null pointer)

	// Set Id
	char strSubTypeId[10] = {0};
	sprintf_s(strSubTypeId, "%d", subtype);
	feature->set_id(strSubTypeId);

	return 0;	// return ok
}

/********************************************************
	Check Kml Page Dom Object
********************************************************/
//--------------------------------------------------- Check GDEM Page Object ?
// NOTE: This function is not still used. Because the GDEM Page object is ignored for saving.
static bool IsKmlPageObj(const QString name)
{
	QString name2(name);

	name2 = name2.toLower();

	if ((name2.indexOf("1 2 ") != -1) && (name2.indexOf(" >") != -1))
	{
		return true;	// return YES
	}

	return false;	// return NO
}

//--------------------------------------------------- Check GoogleEarth Page Object ?
static bool IsGEKmlPageObj(const QString name)
{
	QString name2(name);

	name2 = name2.toLower();

	if (name2.indexOf("a href") != -1)
	{
		return true;	// return YES
	}

	return false;	// return NO
}

void SelectAltitudeMode(int kattrmode, int gx_kattrmode, KmlAltitudeMode &attrmode)
{
	if(kmldom::ALTITUDEMODE_CLAMPTOGROUND == kattrmode && kmldom::GX_ALTITUDEMODE_CLAMPTOSEAFLOOR == gx_kattrmode)
		attrmode	= KML_ALTITUDEMODE_CLAMPED_TO_SEA;
	else if(kmldom::ALTITUDEMODE_RELATIVETOGROUND == kattrmode && kmldom::GX_ALTITUDEMODE_RELATIVETOSEAFLOOR == gx_kattrmode)
		attrmode	= KML_ALTITUDEMODE_RELATIVE_TO_SEA;
	else if ( -1 == kattrmode && -1 == gx_kattrmode)
		attrmode	= KML_ALTITUDEMODE_CLAMPED_TO_GROUND;
	else if ( kmldom::ALTITUDEMODE_RELATIVETOGROUND == kattrmode && -1 == gx_kattrmode)
		attrmode	= KML_ALTITUDEMODE_RELATIVE_TO_GROUND;
	else if ( kmldom::ALTITUDEMODE_ABSOLUTE == kattrmode && -1 == gx_kattrmode)
		attrmode	= KML_ALTITUDEMODE_ABSOLUTE;
	else
		attrmode	= KML_ALTITUDEMODE_CLAMPED_TO_GROUND;
}

/********************************************************
	Parse	Kml Dom Object	->	Kml TreeItem
********************************************************/
int KmlParseDomObject(const kmldom::FeaturePtr feature, KmlTreeItem* item)
{
	if ((feature == NULL) || (item == NULL))
		return -1;	// return error (param error)

	// Check PlaceSearch Object and Set Subtype
	if (item->GetType() == KML_IT_PLACEMARK)
	{
		E_GDM_SUBDATA_TYPE subtype;

		if (GetPlaceSubType(feature, &subtype))
		{
			item->SetType(KML_IT_PLACESEARCH);
			item->SetSubType(subtype);
		}
	}

	// Name
	if (feature->has_name())
	{
		item->SetName(KmlToQString(feature->get_name()));

		// Check GoogleEarth Page Object ? (Only Folder)
		if (item->GetType() == KML_IT_FOLDER)
		{	
			QString fdname("");
			fdname = KmlToQString(feature->get_name());
			
			if (IsGEKmlPageObj(fdname))
			{
				return -2;	// return error (Page Object is ignored)
			}
		}
	}
	else // 이름이 없는 경우 "[이름없음]"으로
	{
		QString tempname;
		tempname = tempname.fromUtf8(ID_STR_UNKNOWN_NAME_STRING);
		item->SetName(tempname);
	}

	// Description
	if (feature->has_description())
		item->SetDescription(KmlToQString(feature->get_description()));

	// Visibility flag
	item->SetVisibleType((feature->get_visibility()) ? (KML_VISTYPE_SHOW) : (KML_VISTYPE_HIDE));

	// Starting Location Flag
	item->SetStartLocation(IsKmlStaringtLocationObj(feature));	

	// LookAt
	kmldom::LookAtPtr			klookat = NULL;

	if ((klookat = kmldom::AsLookAt(feature->get_abstractview())) != NULL)
	{
		KmlLookAt* lookat = new KmlLookAt();		
		lookat->m_longitude		= klookat->get_longitude();
		lookat->m_latitude		= klookat->get_latitude();
		lookat->m_altitude		= klookat->get_altitude();
		lookat->m_heading		= klookat->get_heading();
		lookat->m_range			= klookat->get_range();
		lookat->m_tilt			= klookat->get_tilt();

		int	kattrmode = -1;
		int	gx_kattrmode = -1;
		if(klookat->has_gx_altitudemode())
			kattrmode = (kmldom::AltitudeModeEnum)klookat->get_altitudemode();
		if(klookat->has_gx_altitudemode())
			gx_kattrmode = (kmldom::GxAltitudeModeEnum)klookat->get_gx_altitudemode();
		SelectAltitudeMode(kattrmode, gx_kattrmode, lookat->m_altitudeMode);

		// Set lookat
		item->SetLookAt(lookat);
	}

	if (const PlacemarkPtr placemark = kmldom::AsPlacemark(feature))
	{
/* ============= Get Coordinates ============= */
		kmldom::CoordinatesPtr kcoord = NULL;
		
		//getting CoordinatesPtr for AsPoint
		if ((item->GetType() == KML_IT_PLACESEARCH) || (item->GetType() == KML_IT_PLACEMARK))
		{
			kmldom::PointPtr kpoint = NULL;

			kpoint = kmldom::AsPoint(placemark->get_geometry());
			item->SetExtrude(kpoint->get_extrude());

			int	kattrmode = -1;
			int	gx_kattrmode = -1;
			KmlAltitudeMode		w_altitudeMode;
			if(kpoint->has_altitudemode())
				kattrmode = (kmldom::AltitudeModeEnum)kpoint->get_altitudemode();
			if(kpoint->has_gx_altitudemode())
				gx_kattrmode = (kmldom::GxAltitudeModeEnum)kpoint->get_gx_altitudemode();
			SelectAltitudeMode(kattrmode, gx_kattrmode, w_altitudeMode);
			item->SetAltitudeMode(w_altitudeMode);

			if(kpoint)
				kcoord = kpoint->get_coordinates();
		}
		//getting CoordinatesPtr for AsPolygon
		else if(KML_IT_POLYAREA == item->GetType())
		{
			kmldom::PolygonPtr			p_polygon		= NULL;
			kmldom::OuterBoundaryIsPtr	p_outboundaris	= NULL;
			kmldom::LinearRingPtr		p_linearring	= NULL;

			p_polygon = kmldom::AsPolygon(placemark->get_geometry());

			if(p_polygon)
			{
				p_outboundaris = p_polygon->get_outerboundaryis();
				item->SetExtrude(p_polygon->get_extrude());	

				int	kattrmode = -1;
				int	gx_kattrmode = -1;
				KmlAltitudeMode		w_altitudeMode;
				if(p_polygon->has_altitudemode())
					kattrmode = (kmldom::AltitudeModeEnum)p_polygon->get_altitudemode();
				if(p_polygon->has_gx_altitudemode())
					gx_kattrmode = (kmldom::GxAltitudeModeEnum)p_polygon->get_gx_altitudemode();
				SelectAltitudeMode(kattrmode, gx_kattrmode, w_altitudeMode);
				item->SetAltitudeMode(w_altitudeMode);

				if (p_outboundaris)
				{
					p_linearring = p_outboundaris->get_linearring();
					if(p_linearring)
						kcoord = p_linearring->get_coordinates();
				}
			}
		}
		//getting CoordinatesPtr for AsLineString
		else if(KML_IT_PATH == item->GetType())
		{
			kmldom::LineStringPtr		p_linestring	= NULL;

			p_linestring = kmldom::AsLineString(placemark->get_geometry());
			item->SetExtrude(p_linestring->get_extrude());	

			int	kattrmode = -1;
			int	gx_kattrmode = -1;
			KmlAltitudeMode		w_altitudeMode;
			if(p_linestring->has_altitudemode())
				kattrmode = (kmldom::AltitudeModeEnum)p_linestring->get_altitudemode();
			if(p_linestring->has_gx_altitudemode())
				gx_kattrmode = (kmldom::GxAltitudeModeEnum)p_linestring->get_gx_altitudemode();
			SelectAltitudeMode(kattrmode, gx_kattrmode, w_altitudeMode);
			item->SetAltitudeMode(w_altitudeMode);

			if (p_linestring)
				kcoord = p_linestring->get_coordinates();
		}

		if (NULL != kcoord)
		{
			kmlbase::Vec3	kv3;
			int coordpt_array_size = kcoord->get_coordinates_array_size();

			if(0 >= coordpt_array_size)
				return 0;

			if(KML_IT_POLYAREA == item->GetType())
				coordpt_array_size = coordpt_array_size - 1;

			KmlCoordPoint* coordpt = new KmlCoordPoint[coordpt_array_size];

			for(int pos = 0; pos < coordpt_array_size; pos ++)
			{
				kv3 = kcoord->get_coordinates_array_at(pos);

				coordpt[pos].m_longitude	= kv3.get_longitude();
				coordpt[pos].m_latitude		= kv3.get_latitude();
				coordpt[pos].m_altitude		= kv3.get_altitude();
			}

			// Set Coordinate Point
			item->SetCoordPoint(coordpt);			
			item->SetCoordPointArraySize(coordpt_array_size);			
		}
	}

	return 0;	// return ok
}

void ConvertToKmldomAltitudeMode(int kml_mode, int & dom_altitudeMode, int & dom_gx_altitudeMode)
{
	switch(kml_mode)
	{
		case KML_ALTITUDEMODE_CLAMPED_TO_GROUND:
			dom_altitudeMode = -1;
			dom_gx_altitudeMode = -1;
			break;
		case KML_ALTITUDEMODE_RELATIVE_TO_GROUND:
			dom_altitudeMode = kmldom::ALTITUDEMODE_RELATIVETOGROUND;
			dom_gx_altitudeMode = -1;
			break;
		case KML_ALTITUDEMODE_ABSOLUTE:
			dom_altitudeMode = kmldom::ALTITUDEMODE_ABSOLUTE;
			dom_gx_altitudeMode = -1;
			break;
		case KML_ALTITUDEMODE_CLAMPED_TO_SEA:
			dom_altitudeMode = kmldom::ALTITUDEMODE_CLAMPTOGROUND;
			dom_gx_altitudeMode = kmldom::GX_ALTITUDEMODE_CLAMPTOSEAFLOOR;
			break;
		case KML_ALTITUDEMODE_RELATIVE_TO_SEA:
			dom_altitudeMode = kmldom::ALTITUDEMODE_RELATIVETOGROUND;
			dom_gx_altitudeMode = kmldom::GX_ALTITUDEMODE_RELATIVETOSEAFLOOR;
			break;

		default :
			dom_altitudeMode = -1;
			dom_gx_altitudeMode = -1;
			break;
	}
}

/********************************************************
	Create	Kml TreeItem   ->   Kml Dom Object
********************************************************/
int KmlCreateDomObject(KmlTreeItem* item, kmldom::FeaturePtr feature)
{
	// Set PlaceSearch Object and SubType
	if (item->GetType() == KML_IT_PLACESEARCH)
	{
		SetPlaceSubType(feature, item->GetSubType());
	}

	// Check param
	if ((item == NULL) || (feature == NULL))
		return -1;	// return error (Param error)

	// Set name and Description
	QString name;
	item->GetName(name);
	if(!name.isEmpty())
		feature->set_name(QStringToKml(name));

	QString desc;
	item->GetDescription(desc);
	if(!desc.isEmpty())
		feature->set_description(QStringToKml(desc));

	// Set visibility flag
	if (item->GetVisibleType() == KML_VISTYPE_HIDE)
		feature->set_visibility(false);

	// Set Starting Location Flag
	if (item->GetStartLocation())
	{
		SetKmlStaringtLocationObj(feature);
	}	

	// Set LookAt
	KmlFactory* factory = KmlFactory::GetFactory();		
	KmlLookAt*	lookat	= NULL;

	lookat = item->GetLookAt();

	if (lookat != NULL)
	{
		LookAtPtr klookat = factory->CreateLookAt();	

		klookat->set_longitude(lookat->m_longitude);
		klookat->set_latitude(lookat->m_latitude);		
		klookat->set_altitude(lookat->m_altitude);
		klookat->set_range(lookat->m_range);	
		klookat->set_tilt(lookat->m_tilt);	
		klookat->set_heading(lookat->m_heading);		

		// Set AltitudeMode and GxAltitudeMode
		int altitudemode, gx_altitudemode;
		ConvertToKmldomAltitudeMode(lookat->m_altitudeMode, altitudemode, gx_altitudemode);
		if(-1 != altitudemode)
			klookat->set_altitudemode(altitudemode);
		if(-1 != gx_altitudemode)
			klookat->set_gx_altitudemode(gx_altitudemode);

		// Set
		feature->set_abstractview(klookat);
	}

	// Set CoordPoint (Only PlaceSearch and PlaceMark)
	if(KML_IT_PLACESEARCH == item->GetType())
	{
		if (const PlacemarkPtr placemark = kmldom::AsPlacemark(feature))
		{
			KmlCoordPoint* coordpt = item->GetCoordPoint();
			kmldom::CoordinatesPtr	kcoord	= factory->CreateCoordinates();
			kmldom::PointPtr		kpoint	= factory->CreatePoint();

			if (coordpt != NULL)
			{
				kmlbase::Vec3			kv3(coordpt->m_longitude, coordpt->m_latitude, coordpt->m_altitude);

				kcoord->add_vec3(kv3);
				kpoint->set_coordinates(kcoord);
			}

			placemark->set_geometry(kpoint);
		}
	}
	else if (item->GetType() == KML_IT_PLACEMARK)
	{	
		if (const PlacemarkPtr placemark = kmldom::AsPlacemark(feature))
		{
			if(NULL == item->GetStyle())
				return 0;

			// Set StyleUrl
			QString pre_symbol(STYLE_URL_PRIFIX_SYMBOL);
			placemark->set_styleurl(QStringToKml(item->GetStyle()->styleid.prepend(pre_symbol)));

			KmlCoordPoint* coordpt = item->GetCoordPoint();

			if (coordpt != NULL)
			{
				kmldom::CoordinatesPtr	kcoord	= factory->CreateCoordinates();
				kmldom::PointPtr		kpoint	= factory->CreatePoint();
				kmlbase::Vec3			kv3(coordpt->m_longitude, coordpt->m_latitude, coordpt->m_altitude);

				kcoord->add_vec3(kv3);
				kpoint->set_coordinates(kcoord);

				// Set AltitudeMode and GxAltitudeMode
				int altitudemode, gx_altitudemode;
				ConvertToKmldomAltitudeMode(item->GetAltitudeMode(), altitudemode, gx_altitudemode);
				if(-1 != altitudemode)
					kpoint->set_altitudemode(altitudemode);
				if(-1 != gx_altitudemode)
					kpoint->set_gx_altitudemode(gx_altitudemode);

				// Set
				placemark->set_geometry(kpoint);
			}
		}
	}
	else if(KML_IT_POLYAREA == item->GetType())
	{
		if (const PlacemarkPtr placemark = kmldom::AsPlacemark(feature))
		{
			kmldom::PolygonPtr polygon = factory->CreatePolygon();
			kmldom::OuterBoundaryIsPtr outerboundaryis = factory->CreateOuterBoundaryIs();
			kmldom::LinearRingPtr linearring = factory->CreateLinearRing();

			if(NULL == item->GetStyle())
				return 0;

			// Set StyleUrl
			QString pre_symbol(STYLE_URL_PRIFIX_SYMBOL);
			placemark->set_styleurl(QStringToKml(item->GetStyle()->styleid.prepend(pre_symbol)));

			if(item->GetExtrude())
				polygon->set_extrude(true);
			polygon->set_tessellate(true);

			// Set AltitudeMode and GxAltitudeMode
			int altitudemode, gx_altitudemode;
			ConvertToKmldomAltitudeMode(item->GetAltitudeMode(), altitudemode, gx_altitudemode);
			if(-1 != altitudemode)
				polygon->set_altitudemode(altitudemode);
			if(-1 != gx_altitudemode)
				polygon->set_gx_altitudemode(gx_altitudemode);

			KmlCoordPoint* coordpt = item->GetCoordPoint();
			kmldom::CoordinatesPtr	kcoord = factory->CreateCoordinates();

			if (coordpt != NULL)
			{
				for(int i = 0; i < item->GetCoordPointArraySize(); i++)
				{
					kmlbase::Vec3	kv3(coordpt[i].m_longitude, coordpt[i].m_latitude, coordpt[i].m_altitude);
					kcoord->add_vec3(kv3);
				}

				kmlbase::Vec3	kv3(coordpt[0].m_longitude, coordpt[0].m_latitude, coordpt[0].m_altitude);
				kcoord->add_vec3(kv3);
			
				linearring->set_coordinates(kcoord);
			}

			outerboundaryis->set_linearring(linearring);
			polygon->set_outerboundaryis(outerboundaryis);

			// Set
			placemark->set_geometry(polygon);
		}
	}
	else if(KML_IT_PATH == item->GetType())
	{
		if (const PlacemarkPtr placemark = kmldom::AsPlacemark(feature))
		{
			kmldom::LineStringPtr linestring = factory->CreateLineString();
			kmldom::OuterBoundaryIsPtr outerboundaryis = factory->CreateOuterBoundaryIs();
			kmldom::LinearRingPtr linearring = factory->CreateLinearRing();

			if(NULL == item->GetStyle())
				return 0;

			// Set StyleUrl
			QString pre_symbol(STYLE_URL_PRIFIX_SYMBOL);
			placemark->set_styleurl(QStringToKml(item->GetStyle()->styleid.prepend(pre_symbol)));

			if(item->GetExtrude())
				linestring->set_extrude(true);
			linestring->set_tessellate(true);

			// Set AltitudeMode and GxAltitudeMode
			int altitudemode, gx_altitudemode;
			ConvertToKmldomAltitudeMode(item->GetAltitudeMode(), altitudemode, gx_altitudemode);
			if(-1 != altitudemode)
				linestring->set_altitudemode(altitudemode);
			if(-1 != gx_altitudemode)
				linestring->set_gx_altitudemode(gx_altitudemode);

			KmlCoordPoint* coordpt = item->GetCoordPoint();
			kmldom::CoordinatesPtr	kcoord = factory->CreateCoordinates();

			if (coordpt != NULL)
			{
				for(int i = 0; i < item->GetCoordPointArraySize(); i++)
				{
					kmlbase::Vec3	kv3(coordpt[i].m_longitude, coordpt[i].m_latitude, coordpt[i].m_altitude);
					kcoord->add_vec3(kv3);
				}
			}

			linestring->set_coordinates(kcoord);

			// Set
			placemark->set_geometry(linestring);
		}
	}
	else
	{
		return 0;
	}

	return 0;	// return ok
}