#ifndef KML_CREATOR_H
#define KML_CREATOR_H

#include <string>

#include "kml_treeitem.h"


/********************************************************** 
					Global Functions
**********************************************************/
int KmlCreate_DataFromKmlTree(KmlTreeItem* itemRoot, std::string& kmlData, QString kmlFileName);
void FreeLibkmlMemoryManual();		// Free libkml memory manually

#endif // KML_CREATOR_H
