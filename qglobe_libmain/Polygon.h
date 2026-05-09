#pragma once

#include "../qglobe_base/QGlobe_Location.h"
#include "../qglobe_base/QGlobe_Array.h"
#include "../qglobe_base/QGlobe_Rect.h"
#include "../qglobe_base/QGlobe_Vector3D.h"

class URing
{
public:
	~URing();

	CQGlobe_LocationArr        _LocationArr;
	CQGlobe_MeshArr            _CoverMeshArr; 
	
	void         CalcCoverMesh();
	unsigned int PointCount(){return _LocationArr.size();}
	void		 AddPt(CQGlobe_Location3D pt){_LocationArr.push_back(pt);}
};

class UPolygon
{  
	qglobe_array<URing*> _pRingArr;
public:
	CQGlobe_Rect2D		m_cBound;
   
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
	
