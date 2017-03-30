#include <QtCore/QFile>

#include "SGeoMesh.h"
#include "SAnimatedMesh.h"
#include "SMeshBuffer.h"
#include "building.h"

#define GEOOBJECT_COMPLEX_BUILDING_PATH     "/geoobject/building/complex/geomesh/%d/%d_%d_%d" 
#define GEOOBJECT_SIMPLE_BUILDING_PATH      "/geoobject/building/simple/%d/%d_%d_%d" 
#define SHP_FILE_STYLE                      "/%d_%d_%d.shp" 
#define GEOMESH_FILE_STYLE                  "/%d_%d_%d.geomeshs" 
#define GEOOBJECT_TEXTURE_PATH              "/geoobject/building/complex/textures/"

using namespace  geo;
using namespace  scene;
using namespace  video;

class GeoMeshUtil
{
public:
	static SAnimatedMesh* CreateGeoMeshsFromFile(QString filename);
	static SAnimatedMesh* CreateGeoMeshsFromData(char *pRaw);

	static void SaveToGeoMeshFile(QString filename,SAnimatedMesh* meshs,int level);
	static void SaveToGeoMeshFile(SMesh* mesh,SGeoMesh::GeoMeshHeader header,int level);
	static void AppendGeoMeshToGeoMeshFile(QString filename,SMesh* mesh,SGeoMesh::GeoMeshHeader header,int level);

	static void DeleteGeoMeshFromFile(QString filename,qint64 id);
	static void UpdateGeoMeshFile(QString filename,qint64 id,SGeoMesh* mesh);

	static bool AddGeoBuildingData(GeoBuildingProperty * p); 
	static bool UpdateGeoBuildingData(GeoBuildingProperty * p); 
	static bool DeleteGeoBuildingData(qint64 id);
	static bool GetGeoBuildingData(quint64 id,GeoBuildingProperty* p);
	static bool ExistGeoBuildingData(qint64 id);

	static void CalculateScreenRect(SGeoMesh* mesh,core::rect<s32>& screenrect);
	static void CalculateBoundingRect(SGeoMesh* mesh,core::rect<f32>& boundingrect);
	static void CalculateBoundingRect(SMesh* mesh,SGeoMesh::GeoMeshHeader header,core::rect<f32>& boundingrect);
	static quint64 GenerateGeoMeshID(double longitude, double latitude);

	static QString GetFileNameOfGeoMeshTile(int x,int y,int level);
	static bool IsTooBigFileSize(QString geomeshfilename);

private:
	static void WriteGeoMeshHeader(QFile& file,SGeoMesh * mesh);
	static void WriteGeoMeshHeader(QFile& file,SMesh * mesh,SGeoMesh::GeoMeshHeader header);
	static int  ReadGeoMeshHeader(char *& pRaw,SGeoMesh *mesh);

	static void WriteMaterial(QFile& file,SMaterial& material);
	static void ReadMaterial(SMaterial& material, char*& pRaw);

	static void WriteTexture(QFile& file,SMeshBuffer* meshbuffer,int level);
	static void GenerateTexture(SMeshBuffer * meshbuffer,int level);
	static bool IsTextureExist(const QString texturefilename,int level);
 
	static void WriteGeometry(QFile& file,SMeshBuffer* meshbuffer);
	static void WriteIndex(QFile& file,SMeshBuffer* meshbuffer);

	static double GetAreaInPixels(SMeshBuffer* meshbuffer,int level);
	static QString GetDirectoryNameOf3dTile(int x,int y,int level);
	
	static QString GetDirNameOfSmplBldTile(int x,int y,int level);
	static QString GetFileNameOfSmplBldTile(int x,int y,int level);
  };

inline void MakeDirectory(QString path);
inline void GetXY(double longi,double lat,int level,int &x,int& y);
inline void ConvertToCharFromQString(QString &str, char* buffer);









