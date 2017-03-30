#pragma once

#include "../gdem_base/GDM_Location.h"
#include "../gdem_base/GDM_Array.h"
#include "../gdem_base/GDM_Rect.h"
#include "../gdem_base/GDM_Vector3D.h"

class URing
{
public:
	~URing();

	CGDM_LocationArr        _LocationArr;
	CGDM_MeshArr            _CoverMeshArr; 
	
	void         CalcCoverMesh();
	unsigned int PointCount(){return _LocationArr.size();}
	void		 AddPt(CGDM_Location3D pt){_LocationArr.push_back(pt);}
};

class UPolygon
{  
	gdm_array<URing*> _pRingArr;
public:
	CGDM_Rect2D		m_cBound;
   
	UPolygon();
	~UPolygon();
	void Clear();
	inline void    AddRing(URing* ring){_pRingArr.push_back(ring);}
	inline URing*  GetRing(int i) {return _pRingArr[i];}
	inline URing*  NewRing()   {  URing * pring=new URing; _pRingArr.push_back(pring); return pring;}
	unsigned int   RingCount(){return	_pRingArr.size();}
	unsigned int   PointCount()
	{
		unsigned int res=0;
		for(unsigned int i=0;i<RingCount();i++)
		{
			res+=_pRingArr[i]->PointCount();
		}
		return res;
	}

	void RecalcBound();
};
	
