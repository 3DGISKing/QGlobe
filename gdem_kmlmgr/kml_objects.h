#ifndef KML_OBJECTS_H
#define KML_OBJECTS_H

#include "kml_treeitem.h"

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

//--------------------- Global functions
// Kml Dom Object	->	Kml TreeItem
int KmlParseDomObject(const kmldom::FeaturePtr feature, KmlTreeItem* item);

// Kml TreeItem		-> Kml Dom Object
int KmlCreateDomObject(KmlTreeItem* item, kmldom::FeaturePtr feature);

#endif // KML_OBJECTS_H
