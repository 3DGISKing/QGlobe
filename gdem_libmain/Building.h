#pragma once

#include <QString>

#include "../gdem_base/GDM_Array.h"
#include "../gdem_base/GDM_Location.h"
#include "../gdem_base/GDM_Rect.h"
#include "Polygon.h"

#define SHP_FILE_CODE 0x19831219
#define BUILDING_MAX_FLOOR 105
#define MAX_BAN            250
#define HEIGHT_PER_FLOOR   3.0

typedef struct tagGeoBuildingProperty
{
	quint64  _ID;
	int      _Loc_ID;
	QString  _Name;

	//one building may have many posts
	//each post name is separated by ","
	QString  _PostName;

	//one building may have many bans
	//each ban number is separated by ","
	QString  _Ban;

	float    _MinLongitude;
	float    _MaxLongitude;
	float    _MinLatitude;
	float    _MaxLatitude;
	int      _Floor;
	QString  _Misc;
	QString  _Address;
}GeoBuildingProperty;

class Building
{ 
public:
	void    SetName(QString q) {_Name=q;}
	QString GetName()          {return _Name;}
private:
	QString      _Name;
};

class SimpleBuilding :public Building
{
 public:
	SimpleBuilding();
	SimpleBuilding(const SimpleBuilding& other);
	~SimpleBuilding();

	UPolygon*       GetBottomPolygon()      {return _pBottomPolygon;}
	CGDM_Rect2D     GetBound();            
	double          GetFloor()				{return _Floor;}
	double          GetHeight()				{return _Height;}
	quint64         GetID()					{return _ID;}
	double          GetCenterLongi();       
	double          GetCenterLat()  ;
	void            SetFloor(int floor)		{_Floor=floor;}
 	void            SetHeight(double h)		{_Height=h;}
	void            SetID(quint64 id)			{_ID=id;}
	
	void            CalculateScreenRect(CGDM_Rect2D& rect);
	void            CalcCoverMesh();
	void            EstimateHeight();
	double          CalcAreaByPixel(int level);
private:
	double          _Height;
	double          _Floor;          
	quint64         _ID;
	UPolygon*       _pBottomPolygon; //°¼·ÏËæ ´ÝÂÙ ·ò´ªÂô;
};

class  BuildingArray
{
public:
	BuildingArray()
	{
	};

	~BuildingArray() ;	
	void AddBuilding(Building* b){_BuildingArray.push_back(b);}
	void Clear()     {_BuildingArray.clear();};
	void ConstructFromRawData(char* pRawData);
	bool ConstructFromFile(QString path);
	bool DeleteBulding(quint64 id);
	bool SaveToFile(QString path);
	
	inline unsigned  int Count()        {return _BuildingArray.size();}
	inline Building* GetBuilding(int i) {return _BuildingArray[i];}
	SimpleBuilding*  GetBuildingFromID (quint64 id); 



private:
	void ReadOneSimpleBuilding(char* shpFileData,int &offset);
	void WriteOneSimpleBuilding(QDataStream& out,SimpleBuilding*);
private:
	gdm_array<Building*>	_BuildingArray;
};


