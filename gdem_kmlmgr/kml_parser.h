#ifndef KML_PARSER_H
#define KML_PARSER_H

#include <string>


/********************************************************** 
					Global Functions
**********************************************************/
int KmlParse_DataToKmlTree(std::string kmlData, KmlTreeItem* itemRoot);
void AddToKmlStyleTree(KmlStyle *new_style);
KmlStyle* new_kmlstyle();
void copy_kml_style(KmlStyle *src, KmlStyle *dest);
KmlStyle * FindStyle (KmlStyle *collection, QString StyleMapUrl);
KmlStyle * GetStyleTreeRoot ();
void  SetStyleTreeRoot (KmlStyle *root);
void DeleteKmlStyleTree();
#endif // KML_PARSER_H
