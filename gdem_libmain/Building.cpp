
#include "Building.h"

#include "GDM_DataMgr.h"
#include "GeoMeshUtil.h"
#include "../gdem_client/gdemclient.h"
#include "../gdem_scene/GDM_SceneManager.h"
#include "../gdem_render/rendersrv.h"



struct Point
{
	double _X;
	double _Y;
};

struct SimpleBuildingHeader
{
	double _BoundingBoxMinX;
	double _BoundingBoxMinY;
	double _BoundingBoxMaxX;
	double _BoundingBoxMaxY;
	long   _Floor;
	long   _Unused;
	quint64   _ID;
	long   _NumParts;
	long   _NumPoints;
};

SimpleBuilding::SimpleBuilding()
{
	_Height=0.0;
	_Floor=1;
	_ID=-1;
	_pBottomPolygon=new UPolygon();
}

SimpleBuilding::SimpleBuilding(const SimpleBuilding& other)
{
	 _ID=other._ID;
	 this->_Height=other._Height;
	 this->_Floor=other._Floor;

	 this->_pBottomPolygon=new UPolygon;

	 this->_pBottomPolygon->m_cBound=other._pBottomPolygon->m_cBound;

	 for(unsigned int i=0;i<other._pBottomPolygon->RingCount();i++)
	 {
		 URing* newpring=_pBottomPolygon->NewRing();
		 URing*	otherring=other._pBottomPolygon->GetRing(i);

		 for(unsigned int j=0;j<otherring->PointCount();j++)
			 newpring->AddPt(otherring->_LocationArr[j]);
	 }
}

SimpleBuilding::~SimpleBuilding()
{
	delete _pBottomPolygon;
};

void SimpleBuilding::CalcCoverMesh()
{
	for(unsigned int i=0;i<_pBottomPolygon->RingCount();i++)
	{
		URing *pring=_pBottomPolygon->GetRing(i);
		pring->CalcCoverMesh();
	}
}

CGDM_Rect2D SimpleBuilding::GetBound()
{
	return _pBottomPolygon->m_cBound;
}

void SimpleBuilding::CalculateScreenRect(CGDM_Rect2D& screenrect)
{
	double screenminx=g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->VideoDriver()->getScreenSize().Width;
	double screenmaxx=0;
	double screenminy=g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->VideoDriver()->getScreenSize().Height;
	double screenmaxy=0;

	CGDM_Point3DF p;

	gdm_get3DWorldPoint(_pBottomPolygon->m_cBound.m_cMinPt.m_tX,
						_pBottomPolygon->m_cBound.m_cMinPt.m_tY,
						p,
						_Height+_Floor*3);

	gdm_WorldToScreen(&p);

	if(screenminx>p.m_tX ) screenminx=p.m_tX;
	if(screenmaxx<p.m_tX)  screenmaxx=p.m_tX;
	if(screenminy>p.m_tY)  screenminy=p.m_tY;
	if(screenmaxy<p.m_tY)  screenmaxy=p.m_tY;

	gdm_get3DWorldPoint(_pBottomPolygon->m_cBound.m_cMaxPt.m_tX,
						_pBottomPolygon->m_cBound.m_cMaxPt.m_tY,
						p,
						_Height+_Floor*3);

	gdm_WorldToScreen(&p);

	if(screenminx>p.m_tX ) screenminx=p.m_tX;
	if(screenmaxx<p.m_tX)  screenmaxx=p.m_tX;
	if(screenminy>p.m_tY)  screenminy=p.m_tY;
	if(screenmaxy<p.m_tY)  screenmaxy=p.m_tY;


	gdm_get3DWorldPoint(_pBottomPolygon->m_cBound.m_cMinPt.m_tX,
						_pBottomPolygon->m_cBound.m_cMinPt.m_tY,
						p,
						_Height);

	gdm_WorldToScreen(&p);

	if(screenminx>p.m_tX ) screenminx=p.m_tX;
	if(screenmaxx<p.m_tX)  screenmaxx=p.m_tX;
	if(screenminy>p.m_tY)  screenminy=p.m_tY;
	if(screenmaxy<p.m_tY)  screenmaxy=p.m_tY;

	gdm_get3DWorldPoint(_pBottomPolygon->m_cBound.m_cMaxPt.m_tX,
						_pBottomPolygon->m_cBound.m_cMaxPt.m_tY,
						p,
						_Height);

	gdm_WorldToScreen(&p);

	if(screenminx>p.m_tX ) screenminx=p.m_tX;
	if(screenmaxx<p.m_tX)  screenmaxx=p.m_tX;
	if(screenminy>p.m_tY)  screenminy=p.m_tY;
	if(screenmaxy<p.m_tY)  screenmaxy=p.m_tY;

	screenrect.m_cMinPt.m_tX=screenminx;
	screenrect.m_cMinPt.m_tY=screenminy;
	screenrect.m_cMaxPt.m_tX=screenmaxx;
	screenrect.m_cMaxPt.m_tY=screenmaxy;
  }

void SimpleBuilding::EstimateHeight()
{
	double h=0;
	for(unsigned int i=0;i<_pBottomPolygon->RingCount();i++)
	{
		URing* pring=_pBottomPolygon->GetRing(i);
		for(unsigned int j=0;j<pring->PointCount();j++)
		{
			double longi=pring->_LocationArr[j].m_tX;
			double lat=pring->_LocationArr[j].m_tY;

			if(h<gdm_getMaxHeight(longi,lat))
				h=gdm_getMaxHeight(longi,lat);
		}
	}

#ifdef DEBUG
	if(h==0)
		qDebug("Warning SimpleBuilding::EstimateHeight");
#endif
	SetHeight(h);

}

double SimpleBuilding::GetCenterLongi()
{
	double res=_pBottomPolygon->m_cBound.m_cMinPt.m_tX;
	res+=_pBottomPolygon->m_cBound.m_cMaxPt.m_tX;
	res/=2.0;
	return res;
}

double SimpleBuilding::GetCenterLat()
{
	double res=_pBottomPolygon->m_cBound.m_cMinPt.m_tY;
	res+=_pBottomPolygon->m_cBound.m_cMaxPt.m_tY;
	res/=2.0;
	return res;
}

double SimpleBuilding::CalcAreaByPixel(int level)
{
	CGDM_Rect2D bound=GetBound();
   	double area=(bound.m_cMaxPt.m_tX-bound.m_cMinPt.m_tX)*(bound.m_cMaxPt.m_tY-bound.m_cMinPt.m_tY);
	double tileWidth = gdm_TwoPI / (1 << level);//À°ËÎÌ© ²¸¹¾¼¹¼³

	double pixelarea=(tileWidth/256.0)*(tileWidth/256);
	double numpixels=area/(pixelarea);
	return numpixels;
}

BuildingArray::~BuildingArray()
{
	for(unsigned int i=0;i<Count();i++)
		delete _BuildingArray[i];
	Clear();
}

void BuildingArray::ConstructFromRawData(char* pRawData)
{
	int* filecode;
	filecode=(int*)pRawData;

	if(*filecode!=SHP_FILE_CODE)  return;

	int* featurecnt;
	featurecnt=(int*)(pRawData+4);

	int offset=8;

	for(int i=0;i<*featurecnt;i++)
		ReadOneSimpleBuilding(pRawData,offset);
}

bool BuildingArray::ConstructFromFile(QString path)
{
	_BuildingArray.clear();
	QFile file(path);

	if(!file.open(QIODevice::ReadOnly))	return false;

	char * data=new char[file.size()];
	int size=file.read(data,file.size());
	Q_ASSERT(size==file.size());
	file.close();

	ConstructFromRawData(data);
		
	delete [] data;
	return true;
}

bool BuildingArray::SaveToFile(QString path)
{
	QFile file(path);           
	if(!file.open(QIODevice::WriteOnly)) 	return false;

	QDataStream out(&file);
	out.setByteOrder(QDataStream::LittleEndian);

	out<<SHP_FILE_CODE;
	out<<(long)_BuildingArray.size();

	for(unsigned int i=0;i<Count();i++)
		WriteOneSimpleBuilding(out,(SimpleBuilding*)GetBuilding(i));

	file.close();
	return true;
}

void BuildingArray::WriteOneSimpleBuilding(QDataStream& out,SimpleBuilding* pbuilding)
{
	out<<pbuilding->GetBound().m_cMinPt.m_tX;
	out<<pbuilding->GetBound().m_cMinPt.m_tY;
	out<<pbuilding->GetBound().m_cMaxPt.m_tX;
	out<<pbuilding->GetBound().m_cMaxPt.m_tY;

	out<<(long)pbuilding->GetFloor();
	out<<(long)0;	   //write unused
	out<<(quint64)(pbuilding->GetID());

	out<<pbuilding->GetBottomPolygon()->RingCount();
	out<<pbuilding->GetBottomPolygon()->PointCount();

	int startindex=0;

	for(unsigned int i=0;i<pbuilding->GetBottomPolygon()->RingCount();i++)
	{
		out<<(long)startindex;
		startindex+=pbuilding->GetBottomPolygon()->GetRing(i)->PointCount();
	}

	for(unsigned int i=0;i<pbuilding->GetBottomPolygon()->RingCount();i++)
	{
		URing* pring=pbuilding->GetBottomPolygon()->GetRing(i);

		for(unsigned int j=0;j<pring->PointCount();j++)
		{
			out<<pring->_LocationArr[j].m_tX;
			out<<pring->_LocationArr[j].m_tY;
		}
	}
}

/**
  2¼ê»ôµê shpFileDataÌ© offsetÌ¡¾·µá¹¢À¾
  ÂÙ±¶Ì© °¼·Ï»ôµê¶¦ psimplebuildingËæ ËÏÊÌ´ÉËÍ³Þ.
  offset²÷ ¸êÂÙ³Þ.

  ËÏ²÷ ³­Ë¦
  
  -°¼·Ï·ò´ªÂôËæ ´ÝÂÙ Áç¶®°ØÌ© Ì¡°Ò´ª»ô¶®Áì´ÉË¾ ËÏ²÷³Þ
  -°¼·ÏË¾ ËËµñ²÷ ·º´Ç ¼³¼°Ì© ±¶ºã
  -°¼·ÏË¾ ËËµñ²÷ Ãû´ÉÌ© ±¶ºã
  
  Áç¶®°ØË¾ °éºÂÂ×²÷ ·ô±¶ ÃûÌ© cap¶¦ 3°¢ÂôËºµá ¹¤ÂÚÂÙ³Þ ?? 3°¢ÂôËºµá ¹¤ÂÚÂ×¼è Ê±°Ö ³Þ°¢ÂôËºµá °û¶®·² ´õ°Ã °¯³Þ.
  ¼èÂôË§Ëæ À°¼è Ê±±¨ ÆÁ·ÍËË³Þ
  
  Áç¶®°ØË¾ °éºÂÂ×²÷ ·ô±¶ ÃûË¼ °é·¬ËË ÊÖ²÷ Ë÷¹¢ÃûËËµ¹°Ö °¡¼³ÂÙ³Þ.
*/ 

void BuildingArray::ReadOneSimpleBuilding(char* ShpFileData,int &offset)
{
    SimpleBuilding* psimplebuiling=new SimpleBuilding;

	SimpleBuildingHeader* buildingheader=(SimpleBuildingHeader*)(ShpFileData+offset);

	GeoBuildingProperty p;

	if(GeoMeshUtil::GetGeoBuildingData(buildingheader->_ID,&p))
		psimplebuiling->SetName(p._Name);

	double h=0;
	double tmph;

	psimplebuiling->SetFloor(buildingheader->_Floor);
	psimplebuiling->SetID(buildingheader->_ID);
	offset+=sizeof(SimpleBuildingHeader);

	long numparts=buildingheader->_NumParts;
	long numpoints=buildingheader->_NumPoints;	

	long* pIndexOfFirstPointInPart=(long*)(ShpFileData+offset);
	offset+=numparts*sizeof(long);

	Point* pPoints=(Point*)(ShpFileData+offset);
	
	psimplebuiling->GetBottomPolygon()->m_cBound.m_cMinPt.m_tX=buildingheader->_BoundingBoxMinX;
	psimplebuiling->GetBottomPolygon()->m_cBound.m_cMinPt.m_tY=buildingheader->_BoundingBoxMinY;
	psimplebuiling->GetBottomPolygon()->m_cBound.m_cMaxPt.m_tX=buildingheader->_BoundingBoxMaxX;
	psimplebuiling->GetBottomPolygon()->m_cBound.m_cMaxPt.m_tY=buildingheader->_BoundingBoxMaxY;

	int start_part_point_index,end_part_point_index;

	for(int indexpart=0; indexpart<numparts; indexpart++)
	{
		start_part_point_index=pIndexOfFirstPointInPart[indexpart];

		if(indexpart==numparts-1)
			end_part_point_index=numpoints;
		else
			end_part_point_index=pIndexOfFirstPointInPart[indexpart+1];

		URing *pring=new URing();
		for(int indexpoint=start_part_point_index;indexpoint<end_part_point_index;  indexpoint++)
		{
			CGDM_Location3D location;
			
			location.m_tX=pPoints[indexpoint]._X;
			location.m_tY=pPoints[indexpoint]._Y;
			location.m_tZ=0;

			tmph=  gdm_getHeight(location.m_tX,location.m_tY);
			if(h<tmph)
				h=tmph;

			pring->_LocationArr.push_back(location);
		}
		pring->CalcCoverMesh();
		psimplebuiling->GetBottomPolygon()->AddRing(pring);
	}

	offset+=numpoints*sizeof(double)*2;

	psimplebuiling->SetHeight(h);
	_BuildingArray.push_back(psimplebuiling);
}

bool BuildingArray::DeleteBulding(quint64 id)
{
	for(unsigned int i=0;i<Count();i++)
	{
		SimpleBuilding* p=(SimpleBuilding*)GetBuilding(i);

		if(p->GetID()==id)
		{
			_BuildingArray.erase(i);
			delete p;
			return true;
		}
	}

	return false;
}

SimpleBuilding* BuildingArray::GetBuildingFromID(quint64 id)
{
	for(unsigned int i=0;i<Count();i++)
	{
		SimpleBuilding* p=(SimpleBuilding*)GetBuilding(i);

		if(p->GetID()==id)
			return (SimpleBuilding*)p;
	}

	return NULL;
}





