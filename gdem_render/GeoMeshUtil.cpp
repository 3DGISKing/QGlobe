#include "GeoMeshUtil.h"

#include "gdemclient.h"
#include "../gdem_libmain/GDM_DataMgr.h"
#include "../gds_common/GDSConfigMgr.h"
#include "../gdem_scene/GDM_SceneManager.h"
#include "../gdem_terrainengine/CImage.h"
#include "../gds_database/GDSDatabaseInterface.h"
#include "../gds_database/GDSGeoNameDBManager.h"

using namespace  core;

#define PIXEL_AREA_MAGNIFYER        5.0

inline void Read(void * dst, char*& src_pointer,size_t size)
{
	memcpy(dst,src_pointer,size);
	src_pointer+=size;
}

inline void GetXY(double longi,double lat,int level,int &x,int& y)
{
	double tilewidth=2*gdm_PI/(1<<level);

	x=((longi+gdm_PI)/tilewidth);
	y=((lat+gdm_PI)/tilewidth);
}

void ReadString(char* dest,char *& pRaw)
{
	int index = 0;

	Read(dest,pRaw, 1);

	while (*(dest + index++) != 0)
		Read(dest + index,pRaw, 1);
}

inline void ConvertToCharFromQString(QString &str, char* buffer)
{
	char *data;
	QByteArray byte = str.toAscii();
	data = byte.data();
	int len = strlen(data);

	memcpy(buffer, data, len + 1);
}


inline void MakeDirectory(QString path)
{
	QDir tmp;

	if(!tmp.exists(path)) 
		tmp.mkpath(path);
}


SAnimatedMesh* GeoMeshUtil::CreateGeoMeshsFromFile(QString filename)
{
	QFile file(filename);

	if(!file.open(QIODevice::ReadOnly))	return NULL;

	char * data=new char[file.size()];
	file.read(data,file.size());
	file.close();

	SAnimatedMesh * meshs=CreateGeoMeshsFromData(data);

	delete [] data;
	
	return meshs;
}

SAnimatedMesh* GeoMeshUtil::CreateGeoMeshsFromData(char * pRaw)
{
	//·ô»ÕÌ© ½ú±¶ºã
	int nummeshs;

	Read(&nummeshs,pRaw,sizeof(int));

	if(nummeshs==0) return NULL;

	SAnimatedMesh* meshs=new SAnimatedMesh();

	SGeoMesh* mesh;

	for(int i=0;i<nummeshs;i++)
	{
		mesh=new SGeoMesh();

		int meshbuffercount=ReadGeoMeshHeader(pRaw,mesh);

		for(int j=0;j<meshbuffercount;j++)
		{ 
			SMeshBuffer* meshbuffer=new SMeshBuffer;
			ReadMaterial(meshbuffer->Material,pRaw);

			int vertexcount;
			Read(&vertexcount,pRaw,sizeof(int));

			meshbuffer->Vertices.set_used(vertexcount);

			for(int k=0;k<vertexcount;k++)
			{
				S3DVertex* vertex=&meshbuffer->Vertices[k];

				Read(&vertex->Pos,pRaw,sizeof(float)*3);
				Read(&vertex->TCoords,pRaw,sizeof(float)*2);
				vertex->Color=meshbuffer->Material.DiffuseColor;
			}

			int indexcount;
			Read((char*)&indexcount,pRaw,sizeof(int));

			meshbuffer->Indices.set_used(indexcount);

			u16* index=meshbuffer->Indices.pointer();
			Read((char*)index,pRaw,indexcount*sizeof(u16));
			meshbuffer->recalculateBoundingBox();
			mesh->addMeshBuffer(meshbuffer);
			meshbuffer->drop();
		}

		mesh->recalculateBoundingBox();
		meshs->addMesh(mesh);
		mesh->drop();
	}

	return meshs;
}

void GeoMeshUtil::SaveToGeoMeshFile(QString filename,SAnimatedMesh* meshs,int level)
{
	QFile file(filename);

	if(!file.open(QIODevice::WriteOnly)) 	return ;

	int meshnumber=meshs->Meshes.size();

	file.write((const char*)&meshnumber,sizeof(int));

	for(int i=0;i<meshs->Meshes.size();i++)
	{
		SGeoMesh* mesh=(SGeoMesh*)meshs->Meshes[i];

		WriteGeoMeshHeader(file,mesh);
		for(int j=0;j<mesh->getMeshBufferCount();j++)
		{
			SMeshBuffer *meshbuffer=(SMeshBuffer*)mesh->getMeshBuffer(j);
			WriteMaterial(file,meshbuffer->Material);
			WriteTexture(file,meshbuffer,level);

			int vertexcount=meshbuffer->getVertexCount();
			file.write((const char*)&vertexcount,sizeof(int));

			for(int k=0;k<vertexcount;k++)
			{
				float x,y,z,tx,ty;
				S3DVertex v=meshbuffer->Vertices[k];

				file.write((const char*)&v.Pos,sizeof(float)*3);
				file.write((const char*)&v.TCoords,sizeof(float)*2);
			}

			int indexcount=meshbuffer->getIndexCount();
			file.write((const char*)&indexcount,sizeof(int));

			file.write((const char*)meshbuffer->Indices.pointer(),indexcount*sizeof(u16));
		}
	}
	file.close();
}

void GeoMeshUtil::SaveToGeoMeshFile(SMesh* mesh,SGeoMesh::GeoMeshHeader header,int level)
{
	int x,y;

	GetXY(header.Longitude,header.Latitude,level,x,y);
	MakeDirectory(GetDirectoryNameOf3dTile(x,y,level));

	QString filename=GetFileNameOfGeoMeshTile(x,y,level);

	QFile file(filename);

	if(!file.open(QIODevice::WriteOnly)) 	return ;

	int meshnumber=1;

	file.write((const char*)&meshnumber,sizeof(int));

	WriteGeoMeshHeader(file,mesh,header);

	for(int j=0;j<mesh->getMeshBufferCount();j++)
	{
		SMeshBuffer *meshbuffer=(SMeshBuffer*)mesh->getMeshBuffer(j);

		WriteMaterial(file,meshbuffer->Material);
		WriteTexture(file,meshbuffer,level);
		WriteGeometry(file,meshbuffer);
		WriteIndex(file,meshbuffer);
	}

	file.close();
}

void GeoMeshUtil::AppendGeoMeshToGeoMeshFile(QString filename,SMesh* mesh,SGeoMesh::GeoMeshHeader header,int level)
{
	QFile file(filename);

	if(!file.open(QIODevice::ReadWrite )) 	return ;

	int meshnumber=0;

	file.read(( char*)&meshnumber,sizeof(int));
	meshnumber++;
	file.seek(0);

	file.write((const char*)&meshnumber,sizeof(int));
	file.seek(file.size());

	WriteGeoMeshHeader(file,mesh,header);

	for(int j=0;j<mesh->getMeshBufferCount();j++)
	{
		SMeshBuffer *meshbuffer=(SMeshBuffer*)mesh->getMeshBuffer(j);

		WriteMaterial(file,meshbuffer->Material);
		WriteTexture(file,meshbuffer,level);
		WriteGeometry(file,meshbuffer);
		WriteIndex(file,meshbuffer);
	}

	file.close();
}

void GeoMeshUtil::WriteGeoMeshHeader(QFile& file,SMesh * mesh,SGeoMesh::GeoMeshHeader header)
{
	/***************************************************************
	we do not use mesh name
	****************************************************************/
	//file.write((const char*)mesh->MeshName,strlen(mesh->MeshName)+1);

	file.write((const char*)&header,sizeof(SGeoMesh::GeoMeshHeader));

	int meshbuffercount=mesh->getMeshBufferCount();
	file.write((const char*)&meshbuffercount,sizeof(int));
}

void GeoMeshUtil::WriteGeoMeshHeader(QFile& file,SGeoMesh * mesh)
{
	/***************************************************************
	we do not use mesh name
	****************************************************************/
	//file.write((const char*)mesh->MeshName,strlen(mesh->MeshName)+1);

	file.write((const char*)&mesh->GeoInfo,sizeof(SGeoMesh::GeoMeshHeader));

	int meshbuffercount=mesh->getMeshBufferCount();
	file.write((const char*)&meshbuffercount,sizeof(int));
}

void GeoMeshUtil::WriteMaterial(QFile& file,SMaterial& material)
{
	unsigned int type=(int)material.MaterialType;
	file.write((const char*)&type,sizeof(unsigned int));

	file.write((const char*)&material.DiffuseColor.color,sizeof(u32));
	file.write((const char*)&material.AmbientColor.color,sizeof(u32));
	file.write((const char*)&material.SpecularColor.color,sizeof(u32));
	file.write((const char*)&material.EmissiveColor.color,sizeof(u32));

	file.write((const char*)&material.Shininess,sizeof(f32));
	file.write((const char*)&material.Lighting,sizeof(char));
}

void GeoMeshUtil::WriteTexture(QFile& file,SMeshBuffer* meshbuffer,int level)
{
	char textureexist=0;

	if(meshbuffer->Material.TextureLayer[0].Texture==NULL)
		textureexist=0;
	else
		textureexist=1;

	file.write((const char*)&textureexist,1);

	if(textureexist!=0)
	{	
		stringc imagename=meshbuffer->Material.TextureLayer[0].Texture->getName();

		QString filename=QString::fromAscii(imagename.c_str());
		filename=filename.right(filename.length()-filename.lastIndexOf("\\")-1);

		//bug fix 
		//from 15_car.bmp we get car.bmp
		QString tmp=filename.mid(3,filename.length()-3);

		if(IsTextureExist(tmp,15))
			filename=tmp;
		//end

		if(!IsTextureExist(filename,level))
			GenerateTexture(meshbuffer,level);
			
		filename=QString("%1_%2").arg(level).arg(filename);

		char strfilename[256];

		ConvertToCharFromQString(filename,strfilename);
		file.write((const char*)strfilename,strlen(strfilename)+1);
	}
}

void GeoMeshUtil::WriteGeometry(QFile& file,SMeshBuffer* meshbuffer)
{
	int vertexcount=meshbuffer->getVertexCount();

	file.write((const char*)&vertexcount,sizeof(int));
	for(int i=0;i<meshbuffer->getVertexCount();i++)
	{
		S3DVertex v=meshbuffer->Vertices[i];

		file.write((const char*)&v.Pos,sizeof(float)*3);
		file.write((const char*)&v.TCoords,sizeof(float)*2);
	}
}

void GeoMeshUtil::WriteIndex(QFile& file,SMeshBuffer* meshbuffer)
{
	int indexcount=meshbuffer->getIndexCount();
	file.write((const char*)&indexcount,sizeof(int));
	file.write((const char*)meshbuffer->Indices.pointer(),indexcount*sizeof(u16));
}


int GeoMeshUtil::ReadGeoMeshHeader(char *& pRaw,SGeoMesh *mesh)
{
	//ReadString(mesh->MeshName,pRaw);

	Read(( char*)&mesh->GeoInfo,pRaw,sizeof(SGeoMesh::GeoMeshHeader));

	int meshbuffercount;
	Read(( char*)&meshbuffercount,pRaw,sizeof(int));

	mesh->OldLongitude=mesh->GeoInfo.Longitude;
	mesh->OldLatitude=mesh->GeoInfo.Latitude;
	return meshbuffercount;
}

#include "../gdem_scene/GDM_TextureMgr.h"

void GeoMeshUtil::ReadMaterial(SMaterial& material, char*& pRaw)
{
	unsigned int type;
	Read(&type,pRaw,sizeof(unsigned int));

	material.MaterialType=(video::E_MATERIAL_TYPE)type;

	Read(&material.DiffuseColor,pRaw,sizeof(u32));
	Read(&material.AmbientColor,pRaw,sizeof(u32));
	Read(&material.SpecularColor,pRaw,sizeof(u32));
	Read(&material.EmissiveColor,pRaw,sizeof(u32));

	Read(&material.Shininess,pRaw,sizeof(f32));
	
	Read(&material.Lighting,pRaw,sizeof(char));

	char textureexist;

	Read((char*)&textureexist,pRaw,sizeof(char));

	if(textureexist!=0)
	{
		char strfilename[256];

		ReadString(strfilename,pRaw);

		Q_ASSERT(strlen(strfilename)<256);

		QString path=g_ServerConfig.GetFileDBPath()+GEOOBJECT_TEXTURE_PATH;

		char strpath[1024];

		ConvertToCharFromQString(path,strpath);

		strcat_s(strpath,strfilename);

		g_TextureListMutex.lock();
		ITexture *tex=g_app->GetRenderProxy()->
			GetRenderSrv()->m_pSceneMgr->VideoDriver()->getTexture(strpath);
		g_TextureListMutex.unlock();
		if(tex)
		{
			material.TextureLayer[0].Texture=tex;
			tex->grab();
		}
	}
}

void GeoMeshUtil::GenerateTexture(SMeshBuffer * meshbuffer,int level)
{
	CImage *im=(CImage*)meshbuffer->Material.TextureLayer[0].Texture->getImage();

	double pixels=GetAreaInPixels(meshbuffer,level);
	double scale=pixels/(im->getDimension().Height*im->getDimension().Width);

	scale=sqrt(scale)*PIXEL_AREA_MAGNIFYER;   

	if(level==THREED_MAX_LEVEL)		scale=1;

	if(scale>1.0 ) scale=1.0;

	int scaledwidth=scale*im->getDimension().Width;
	int scaledheight=scale*im->getDimension().Height;	

	QImage origim;
	origim.load(QString::fromAscii(meshbuffer->Material.TextureLayer[0].Texture->getName().c_str()));

	QImage scaledimage;
	scaledimage=origim.scaled(scaledwidth,scaledheight,Qt::KeepAspectRatio,Qt::SmoothTransformation);

	stringc imagename=meshbuffer->Material.TextureLayer[0].Texture->getName();

	QString filename=QString::fromAscii(imagename.c_str());

	filename=filename.right(filename.length()-filename.lastIndexOf("\\")-1);

	filename=QString::number(level)+"_"+filename;

	filename=g_ServerConfig.GetFileDBPath()+GEOOBJECT_TEXTURE_PATH+filename;

	if(!QFile::exists(filename))
		scaledimage.save(filename);
#ifdef DEBUG
	else
	{
		//gu_MessageBox(NULL,tr("Error"),"Duplicate texture name");
		QFile::remove(filename);
		scaledimage.save(filename);
		qDebug("Duplicate texture name");
	}
#endif
}

bool GeoMeshUtil::IsTooBigFileSize(QString geomeshfilename)
{
	QFile file(geomeshfilename);

	if(file.size()>g_pGDMDataMgr->GetMaxTileSize(E_GDM_MAINDATA_TYPE_3D))
		return true;
	else
		return false;
}

bool GeoMeshUtil::IsTextureExist(const QString texturefilename,int level)
{
	QString filename=QString("%1_%2").arg(level).arg(texturefilename);
	filename=g_ServerConfig.GetFileDBPath()+GEOOBJECT_TEXTURE_PATH+filename;
	if(QFile::exists(filename))
		return true;
	else
		return false;
}

double GeoMeshUtil::GetAreaInPixels(SMeshBuffer* meshbuffer,int level)
{
	double tileWidth = EARTH_RADIUS*gdm_TwoPI / (1 << level);
	double resolution=tileWidth/256.0;

	aabbox3df box=meshbuffer->getBoundingBox();
	const vector3df e=box.getExtent();
	double area= 2*(e.X*e.Y + e.X*e.Z + e.Y*e.Z)-e.X*e.Y;
	return area/(resolution*resolution);
}

//by RSH 2014.3.19
#define ID_TOLERANCE                     0.00000001
quint64 GeoMeshUtil::GenerateGeoMeshID(double longitude, double latitude)
{
	quint64 ix=longitude/ID_TOLERANCE;
	quint64 iy=latitude/ID_TOLERANCE;

	quint64 id;

	id=ix*pow(10.0,9);
	id=id+iy;

	return id;
}
//end RSH

QString GeoMeshUtil::GetDirectoryNameOf3dTile(int x,int y,int level)
{
	int m=x/128;   int n=y/128;

	QString path;
	return g_ServerConfig.GetFileDBPath()+path.sprintf(GEOOBJECT_COMPLEX_BUILDING_PATH,level,m,n,level); 
}

QString GeoMeshUtil::GetDirNameOfSmplBldTile(int x,int y,int level)
{
	int m=x/128;   int n=y/128;

	QString path;
	return g_ServerConfig.GetFileDBPath()+path.sprintf(GEOOBJECT_SIMPLE_BUILDING_PATH,level,m,n,level); 
}

QString GeoMeshUtil::GetFileNameOfGeoMeshTile(int x,int y,int level)
{
	QString directory_name=GetDirectoryNameOf3dTile(x,y,level);
	QString file_name;

	return directory_name+file_name.sprintf(GEOMESH_FILE_STYLE,x,y,level);
}

QString GeoMeshUtil::GetFileNameOfSmplBldTile(int x,int y,int level)
{
	QString dirname=GetDirNameOfSmplBldTile(x,y,level);

	QString filename;

	return dirname+filename.sprintf(SHP_FILE_STYLE,x,y,level);
}

bool GeoMeshUtil::AddGeoBuildingData(GeoBuildingProperty * p)
{
	if(!g_DBManager) return false;

	QRegExp rx("(\\D+)");
	QStringList ban = p->_Ban.split(rx);
	ban.removeAll("");

	if(ban.size()>1)
	{
		p->_Ban = ban.join("|");
		p->_Ban =p->_Ban+"|";
	}

	return g_DBManager->AddGeoBuildingData(p);

}

bool GeoMeshUtil::UpdateGeoBuildingData(GeoBuildingProperty * p)
{
	if(!g_DBManager) return false;
	return g_DBManager->UpdateGeoBuildingData(p);
}

bool GeoMeshUtil::DeleteGeoBuildingData(qint64 id)
{
	if(!g_DBManager) return false;
	return g_DBManager->DeleteGeoBuildingData(id);
}

bool GeoMeshUtil::GetGeoBuildingData(quint64 id,GeoBuildingProperty *p)
{
	if(!g_DBManager) return false;
	return g_DBManager->GetGeoBuildingData(id,p);
}

void GeoMeshUtil::CalculateScreenRect(SGeoMesh* mesh,core::rect<s32>& screenrect)
{
	GDM_LOCATION loc;

	loc.m_dLongitude=mesh->GeoInfo.Longitude;
	loc.m_dLatitude=mesh->GeoInfo.Latitude;

	/*******************************
	DEMÌ© ²ÚËË¶¦ »ô´´¼¬Ëºµá °ÖµØÂ×±Ê²÷¼è²÷ ´¸°Ö¸ó»ô
	********************************/
	loc.m_dDist =mesh->GeoInfo.Height ;

	//³ÞËÁ ËË Ì¡¾·Ëæº· Áì¼Ó»ô¶®Áì±Ë¶¦ ÊÐ²÷³Þ.
	GDM_Coord3D standCoord;
	gdm_GetStandardCoord(&loc, &standCoord);

	//ËË »ô¶®Áì±Ë¶¦ ¸ÒÌ¡°¢¶í¿Ñ Ãä¼­»¤¿Ö³Þ.
	gdm_RotateAroundZAxis(&standCoord,mesh->GeoInfo.Azimuth*gdm_DEGTORAD);

	//Êï¹¶½¤Àâ¶¦ Ì»´Å»ô¶®Áì±ËËæµá ²¿±¨²÷ ¸êÃûÃÔµÛË¾ ÊÐ²÷³Þ.
	CGDM_Matrix4     Transform;
	gdm_MatrixTCStoSCS(&standCoord, &Transform );

	core::aabbox3df localbox=mesh->getBoundingBox();

	CGDM_Aabbox3DF worldbox(localbox.MinEdge.X,localbox.MinEdge.Y,localbox.MinEdge.Z,
		localbox.MaxEdge.X,localbox.MaxEdge.Y,localbox.MaxEdge.Z);

	CGDM_Vector3DF edges[8];

	worldbox.getEdges(edges);

	for(int i=0;i<8;i++)
		Transform.transformVect(edges[i]);

	CGDM_Point3DF p;

	double screenminx=g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->VideoDriver()->getScreenSize().Width;
	double screenmaxx=0;
	double screenminy=g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->VideoDriver()->getScreenSize().Height;
	double screenmaxy=0;

	for(int i=0;i<8;i++)
	{
		p.set(edges[i]);
		gdm_WorldToScreen(&p);

		if(screenminx>p.m_tX ) screenminx=p.m_tX;
		if(screenmaxx<p.m_tX)  screenmaxx=p.m_tX;
		if(screenminy>p.m_tY)  screenminy=p.m_tY;
		if(screenmaxy<p.m_tY)  screenmaxy=p.m_tY;
	}

	screenrect.UpperLeftCorner.X=screenminx;
	screenrect.UpperLeftCorner.Y=screenminy;
	screenrect.LowerRightCorner.X=screenmaxx;
	screenrect.LowerRightCorner.Y=screenmaxy;
}
void GeoMeshUtil::CalculateBoundingRect(SMesh* mesh,SGeoMesh::GeoMeshHeader header,core::rect<f32>& boundingrect)
{
	GDM_LOCATION loc;

	loc.m_dLongitude=header.Longitude;
	loc.m_dLatitude=header.Latitude;

	/*******************************
	DEMÌ© ²ÚËË¶¦ »ô´´¼¬Ëºµá °ÖµØÂ×±Ê²÷¼è²÷ ´¸°Ö¸ó»ô
	********************************/
	loc.m_dDist =header.Height ;

	//³ÞËÁ ËË Ì¡¾·Ëæº· Áì¼Ó»ô¶®Áì±Ë¶¦ ÊÐ²÷³Þ.
	GDM_Coord3D standCoord;
	gdm_GetStandardCoord(&loc, &standCoord);

	//ËË »ô¶®Áì±Ë¶¦ ¸ÒÌ¡°¢¶í¿Ñ Ãä¼­»¤¿Ö³Þ.
	gdm_RotateAroundZAxis(&standCoord,header.Azimuth*gdm_DEGTORAD);

	//Êï¹¶½¤Àâ¶¦ Ì»´Å»ô¶®Áì±ËËæµá ²¿±¨²÷ ¸êÃûÃÔµÛË¾ ÊÐ²÷³Þ.
	CGDM_Matrix4     Transform;
	gdm_MatrixTCStoSCS(&standCoord, &Transform );

	core::aabbox3df localbox=mesh->getBoundingBox();

	CGDM_Aabbox3DF worldbox(localbox.MinEdge.X,localbox.MinEdge.Y,localbox.MinEdge.Z,
		localbox.MaxEdge.X,localbox.MaxEdge.Y,localbox.MaxEdge.Z);

	CGDM_Vector3DF edges[8];

	worldbox.getEdges(edges);

	for(int i=0;i<8;i++)
		Transform.transformVect(edges[i]);

	CGDM_Point3DF p;

	float minlongitude=gdm_PI;
	float maxlongitude=-gdm_PI;
	float minlatitude=gdm_PI/2.0;
	float maxlatitude=-gdm_PI/2.0;;

	CGDM_Location3D loc1;

	for(int i=0;i<8;i++)
	{
		p.set(edges[i]);
		gdm_GetLocation(&p,&loc1);

		if(minlongitude>loc1.m_tX ) minlongitude=loc1.m_tX;
		if(maxlongitude<loc1.m_tX) maxlongitude=loc1.m_tX;
		if(minlatitude>loc1.m_tY)   minlatitude=loc1.m_tY;
		if(maxlatitude<loc1.m_tY)  maxlatitude=loc1.m_tY;
	}

	boundingrect.UpperLeftCorner.X=minlongitude;
	boundingrect.UpperLeftCorner.Y=minlatitude;
	boundingrect.LowerRightCorner.X=maxlongitude;
	boundingrect.LowerRightCorner.Y=maxlatitude;
}

void GeoMeshUtil::CalculateBoundingRect(SGeoMesh* mesh,core::rect<f32>& boundingrect)
{
	GDM_LOCATION loc;

	loc.m_dLongitude=mesh->GeoInfo.Longitude;
	loc.m_dLatitude=mesh->GeoInfo.Latitude;

	/*******************************
	DEMÌ© ²ÚËË¶¦ »ô´´¼¬Ëºµá °ÖµØÂ×±Ê²÷¼è²÷ ´¸°Ö¸ó»ô
	********************************/
	loc.m_dDist =mesh->GeoInfo.Height ;

	//³ÞËÁ ËË Ì¡¾·Ëæº· Áì¼Ó»ô¶®Áì±Ë¶¦ ÊÐ²÷³Þ.
	GDM_Coord3D standCoord;
	gdm_GetStandardCoord(&loc, &standCoord);

	//ËË »ô¶®Áì±Ë¶¦ ¸ÒÌ¡°¢¶í¿Ñ Ãä¼­»¤¿Ö³Þ.
	gdm_RotateAroundZAxis(&standCoord,mesh->GeoInfo.Azimuth*gdm_DEGTORAD);

	//Êï¹¶½¤Àâ¶¦ Ì»´Å»ô¶®Áì±ËËæµá ²¿±¨²÷ ¸êÃûÃÔµÛË¾ ÊÐ²÷³Þ.
	CGDM_Matrix4     Transform;
	gdm_MatrixTCStoSCS(&standCoord, &Transform );

	core::aabbox3df localbox=mesh->getBoundingBox();

	CGDM_Aabbox3DF worldbox(localbox.MinEdge.X,localbox.MinEdge.Y,localbox.MinEdge.Z,
		localbox.MaxEdge.X,localbox.MaxEdge.Y,localbox.MaxEdge.Z);

	CGDM_Vector3DF edges[8];

	worldbox.getEdges(edges);

	for(int i=0;i<8;i++)
		Transform.transformVect(edges[i]);

	CGDM_Point3DF p;

	float minlongitude=gdm_PI;
	float maxlongitude=-gdm_PI;
	float minlatitude=gdm_PI/2.0;
	float maxlatitude=-gdm_PI/2.0;;

	CGDM_Location3D loc1;

	for(int i=0;i<8;i++)
	{
		p.set(edges[i]);
		gdm_GetLocation(&p,&loc1);

		if(minlongitude>loc1.m_tX ) minlongitude=loc1.m_tX;
		if(maxlongitude<loc1.m_tX) maxlongitude=loc1.m_tX;
		if(minlatitude>loc1.m_tY)   minlatitude=loc1.m_tY;
		if(maxlatitude<loc1.m_tY)  maxlatitude=loc1.m_tY;
	}

	boundingrect.UpperLeftCorner.X=minlongitude;
	boundingrect.UpperLeftCorner.Y=minlatitude;
	boundingrect.LowerRightCorner.X=maxlongitude;
	boundingrect.LowerRightCorner.Y=maxlatitude;
}

void GeoMeshUtil::DeleteGeoMeshFromFile(QString filename,qint64 id)
{
	QFile meshfile(filename);

	bool ret=meshfile.open(QIODevice::ReadWrite);

	Q_ASSERT(ret);

	if(!ret)
		return;

	qint64 offset=0;

	qint64 startoffset=0;
	qint64 endoffset=0;

	int nummeshs=0;

	meshfile.read((char*)&nummeshs,sizeof(int));	offset=sizeof(int);

	for(int i=0;i<nummeshs;i++)
	{
		SGeoMesh::GeoMeshHeader header;

		meshfile.read((char*)&header,sizeof(SGeoMesh::GeoMeshHeader)); 

		if(header.ID==id)
			startoffset=offset;

		offset+=sizeof(SGeoMesh::GeoMeshHeader);

		int meshbuffercount=0; 

		meshfile.read((char*)&meshbuffercount,sizeof(int));	offset+=sizeof(int);

		for(int j=0;j<meshbuffercount;j++)
		{
			int bytes=sizeof(unsigned int)+//mattype
				      sizeof(int)*4+       //color
					  sizeof(float)+       //shiness
                      sizeof(char);        //lighting

			offset+=bytes;
			
			meshfile.seek(offset);
			
			char textureexist=0;

			meshfile.read((char*)&textureexist,sizeof(char));	offset+=sizeof(char);

			if(textureexist!=0)
			{
				char texturefilename[256];

				char ch;

				meshfile.read(&ch,sizeof(char));

				offset+=sizeof(char);

				int index=0;

				while(ch!=0)
				{
					texturefilename[index]=ch;
					meshfile.read(&ch,sizeof(char));

					offset+=sizeof(char);

					index++;

					Q_ASSERT(index<256);
				}

				texturefilename[index]=ch;
			}

			int vertexcount=0;

			meshfile.read((char*)&vertexcount,sizeof(int)); offset+=sizeof(int);

			// x,y,z, tx,ty 

		    offset=offset+sizeof(float)*vertexcount*5;
			
			meshfile.seek(offset);

			int indexcount=0;

			meshfile.read((char*)&indexcount,sizeof(int)); 	offset+=sizeof(int);
		
			offset+=sizeof(unsigned __int16)*indexcount;

			meshfile.seek(offset);
		}

		if(header.ID==id)
		{
			endoffset=offset;
			break;
		}
	}

	if(endoffset==0)
		Q_ASSERT(offset==meshfile.size());
	
	meshfile.seek(0);

	QByteArray alldata=meshfile.readAll();

	Q_ASSERT(alldata.size()==meshfile.size());

	meshfile.close();

	QFile::remove(filename);

	alldata.remove(startoffset,endoffset-startoffset);

	if(alldata.size()==sizeof(int))
	{
		//this file contain no mesh ,so we do not need to save this file
		Q_ASSERT(nummeshs==1);
		return;
	}

	if(!meshfile.open(QIODevice::ReadWrite))
		return;

	//update nummesh;

	nummeshs=nummeshs-1;

	meshfile.write((char*)&nummeshs,sizeof(int));

	meshfile.write(alldata.constData()+sizeof(int),alldata.size()-sizeof(int));

	meshfile.close();
}

void GeoMeshUtil::UpdateGeoMeshFile(QString filename,qint64 id,SGeoMesh *mesh)
{
	QFile meshfile(filename);

	bool ret=meshfile.open(QIODevice::ReadWrite);

	Q_ASSERT(ret);

	if(!ret)
		return;

	qint64 offset=0;

	qint64 startoffset=0;

	int nummeshs=0;

	meshfile.read((char*)&nummeshs,sizeof(int));	offset=sizeof(int);

	for(int i=0;i<nummeshs;i++)
	{
		SGeoMesh::GeoMeshHeader header;

		meshfile.read((char*)&header,sizeof(SGeoMesh::GeoMeshHeader)); 

		if(header.ID==id)
		{
			startoffset=offset;
			break;
		}

		offset+=sizeof(SGeoMesh::GeoMeshHeader);

		int meshbuffercount=0; 

		meshfile.read((char*)&meshbuffercount,sizeof(int));	offset+=sizeof(int);

		for(int j=0;j<meshbuffercount;j++)
		{
			int bytes=sizeof(unsigned int)+//mattype
				sizeof(int)*4+       //color
				sizeof(float)+       //shiness
				sizeof(char);        //lighting

			offset+=bytes;

			meshfile.seek(offset);

			char textureexist=0;

			meshfile.read((char*)&textureexist,sizeof(char));	offset+=sizeof(char);

			if(textureexist!=0)
			{
				char texturefilename[256];

				char ch;

				meshfile.read(&ch,sizeof(char));

				offset+=sizeof(char);

				int index=0;

				while(ch!=0)
				{
					texturefilename[index]=ch;
					meshfile.read(&ch,sizeof(char));

					offset+=sizeof(char);

					index++;

					Q_ASSERT(index<256);
				}

				texturefilename[index]=ch;
			}

			int vertexcount=0;

			meshfile.read((char*)&vertexcount,sizeof(int)); offset+=sizeof(int);

			// x,y,z, tx,ty 

			offset=offset+sizeof(float)*vertexcount*5;

			meshfile.seek(offset);

			int indexcount=0;

			meshfile.read((char*)&indexcount,sizeof(int)); 	offset+=sizeof(int);

			offset+=sizeof(unsigned __int16)*indexcount;

			meshfile.seek(offset);
		}
	}
	
	if(startoffset==0)
	{
		//this file does not contain mesh for given id
		meshfile.close();
		return;
	}

	meshfile.seek(startoffset);

	WriteGeoMeshHeader(meshfile,mesh,mesh->GeoInfo);

	for(int j=0;j<mesh->getMeshBufferCount();j++)
	{
		SMeshBuffer *meshbuffer=(SMeshBuffer*)mesh->getMeshBuffer(j);

		WriteMaterial(meshfile,meshbuffer->Material);
		char textureexist=0;

		if(meshbuffer->Material.TextureLayer[0].Texture==NULL)
			textureexist=0;
		else
			textureexist=1;

		meshfile.write((const char*)&textureexist,1);

		char texture[256];

		int index=0;
		meshfile.read(&texture[index],1);

		while(texture[index]!=0)
		{	
			index++;
			meshfile.read(&texture[index],1);
		}
		
		Q_ASSERT(index<256);

		WriteGeometry(meshfile,meshbuffer);
		//bug fix
		//if meshbuffer count is larger than 1, this case must be considered!
		WriteIndex(meshfile,meshbuffer);
	}

	meshfile.close();

}

bool GeoMeshUtil::ExistGeoBuildingData(qint64 id)
{
	if(!g_DBManager) return false;

	return g_DBManager->ExistGeoBuildingData(id); 
}