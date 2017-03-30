#include <string>

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>

#include "kmlmanagerclient.h"
#include "kml_parser.h"
#include "kml_creator.h"


// static functions
static int OpenKmlFileForUtf8(QString kmlFileName,		std::string& kmlOutData);
static int SaveKmlFileForUtf8(std::string kmlInData,	QString kmlFileName);


KmlManagerClient::KmlManagerClient()
{

}

KmlManagerClient::~KmlManagerClient()
{

}

/********************************************************
					Parse KML file
********************************************************/
bool IsKml(const std::string kml_data) 
{
	if(KML_FILE_HEADER_STR_LEN > kml_data.length())
		return false;

	return kml_data.substr(0, KML_FILE_HEADER_STR_LEN) == KML_FILE_HEADER_STR_UTF8 ? true : false;
}


// Open Kml data From Kml File  AND  Convert Kml data Into Utf-8 data
static int OpenKmlFileForUtf8(QString kmlFileName, std::string& kmlOutData)
{
	QFile	kmlFile(kmlFileName);

	// Open kml file
	if (!kmlFile.open(QIODevice::ReadOnly))
		return -1;	// return error (file open error)

	QTextStream	kmlStream(&kmlFile);
	QString		strUnicode("");

	// Codec
	kmlStream.setCodec("UTF-8");
	kmlStream.setAutoDetectUnicode(true);

	// Set buffer size to read file
	QFileInfo fInfo(kmlFileName);
	qint64 fSize = fInfo.size();
	qint64 bufSize =   (fSize < FILE_LARGE_SIZE)	? (FILE_SMALL_BUFFER_MAX_SIZE) 
					: ((fSize < FILE_LARGE2_SIZE)	? (FILE_LARGE_BUFFER_MAX_SIZE)
													: (FILE_LARGE2_BUFFER_MAX_SIZE));

	if(!IsKml(QStringToKml(kmlStream.read(KML_FILE_HEADER_STR_LEN))))
	{
		kmlFile.close();
		return -1;
	}
	
	if(MAXIMUM_KML_FILE_SIZE < fSize)
	{
		kmlFile.close();
		return -1;
	}

	// Reserve buffer
	strUnicode.reserve(fSize + 1);
	if((fSize + 1) != strUnicode.capacity())
	{
		kmlFile.close();
		return -1;
	}

	// Read kml file
	kmlStream.seek(0);
	while (!kmlStream.atEnd())
	{		
		strUnicode.append(kmlStream.read(bufSize));
	}

	kmlFile.close();

	// Convert Unicode string To Utf-8 string
	kmlOutData = QStringToKml(strUnicode);		

	return 0;	// return ok
}

// Open KML file  AND  Parse it to kml data
int KmlManagerClient::ParseKmlFile(QString kmlFileName, KmlTreeItem* itemRoot)
{
	//---------------------------------------------------- Check parameters
	if ((kmlFileName == NULL) || (itemRoot == NULL))
		return -1;	// return error (parameter error)

	//---------------------------------------------------- Open Kml data From Kml File	
	std::string		kmlData;
	int				ret;
	if ((ret = OpenKmlFileForUtf8(kmlFileName, kmlData)) != 0)
		return ret;	// return error (file open or read error)

	//---------------------------------------------------- Parse Kml data
	return KmlParse_DataToKmlTree(kmlData, itemRoot);
}

/********************************************************
				Create KML file
********************************************************/
// Convert kml data into Utf-8	AND	 Write it to Kml File
static int SaveKmlFileForUtf8(std::string kmlInData, QString kmlFileName)
{
	QFile	kmlFile(kmlFileName);

	// Open kml file
	if (!kmlFile.open(QIODevice::WriteOnly))
		return -1;	// return error (file open error)

	QTextStream	kmlStream(&kmlFile);
	QString		strUnicode("");

	// Codec
	kmlStream.setCodec("UTF-8");
	kmlStream.setAutoDetectUnicode(true);

	// Write kml file
	strUnicode = KmlToQString(kmlInData);
			
	kmlStream << strUnicode;
	kmlFile.close();

	return 0;	// return ok
}

// Create kml data  AND  Write it to KML file
int KmlManagerClient::CreateKmlFile(KmlTreeItem* itemRoot, QString kmlFileName)
{
	//---------------------------------------------------- Check parameters
	if ((itemRoot == NULL) || (kmlFileName == NULL))
		return -1; // return error (parameter error)

	//---------------------------------------------------- Create Kml String
	std::string		kmlData;
	int				ret;

	if ((ret = KmlCreate_DataFromKmlTree(itemRoot, kmlData, kmlFileName)) != 0)
		return ret;	// return error (crate kml data error)

	//---------------------------------------------------- Save Kml data To Kml File
	return SaveKmlFileForUtf8(kmlData, kmlFileName);
}

/********************************************************
				Free libkml memory manually
********************************************************/
void KmlManagerClient::FreeLibkmlMemory()
{
	// Free memory
	FreeLibkmlMemoryManual();
}