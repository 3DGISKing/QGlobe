#ifndef KMLMANAGERCLIENT_H
#define KMLMANAGERCLIENT_H

#include <QString>

#include "kml_treeitem.h"


/********************************************************** 
					Global	Constatns
**********************************************************/
// Debug Mode
#define	KML_MGR_DEBUG_MODE				1		// 1: Debug Mode, 0: Release Mode

// Use Kml feature styles
#define	KML_USE_FEATURE_STYLES			1		// 1: Use, 0: Not Use (Default)

// File Read Max Buffer
#define FILE_SMALL_BUFFER_MAX_SIZE			10240		// bytes = 10  Kbytes	(Normal)

#define FILE_LARGE_SIZE						1024000		// bytes = 1   Mbytes	(Large)
#define FILE_LARGE_BUFFER_MAX_SIZE			102400		// bytes = 100 Kbytes

#define FILE_LARGE2_SIZE					5120000		// bytes = 5   Mbytes	(Too Large)
#define FILE_LARGE2_BUFFER_MAX_SIZE			1024000		// bytes = 1   Mbytes

#define MAXIMUM_KML_FILE_SIZE				51200000	// Our system is not support for KML file over 50 MBytes 

// Kml File Extension
#define KML_FILE_EXT					".kml"
#define KMZ_FILE_EXT					".kmz"

// Kml File Header
#define	KML_FILE_HEADER_STR_UTF8		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#define	KML_FILE_HEADER_STR_LEN			strlen(KML_FILE_HEADER_STR_UTF8)
#define GOOGLE_EARTH_ICON_IMAGE_URL		"http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png"

// File prefix string for MyPlaces
#define	MY_PLACES_FNAME_PREFIX			"myplaces"
#define	MY_PLACES_TEMP_FNAME_PREFIX		"myplaces.tmp"
#define	MY_PLACES_BACKUP_FNAME_PREFIX	"myplaces.backup"


/********************************************************** 
					Global	Macros
**********************************************************/
#define	KmlToQString(KML_STR)	QString::fromUtf8((KML_STR).data())	// Convert Kml String(Utf-8) ---> QString(Unicode)
#define	QStringToKml(Q_STR)		(Q_STR).toUtf8().data()				// Convert QString(Unicode)  ---> Kml String(Utf-8)


/********************************************************** 
		Class KmlManagerClient for External Interface
**********************************************************/
class KmlManagerClient
{
public:
	KmlManagerClient();
	~KmlManagerClient();

	// public static functions to Parse and Create KML file
	static int ParseKmlFile(QString kmlFileName, KmlTreeItem* itemRoot);	// Parse kml file to Kml Tree
	static int CreateKmlFile(KmlTreeItem* itemRoot, QString kmlFileName);	// Create kml file from Kml Tree
	static void FreeLibkmlMemory();											// Free libkml memory manually

private:

};

#endif // KMLMANAGERCLIENT_H
