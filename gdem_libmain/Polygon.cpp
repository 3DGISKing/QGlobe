#include "Polygon.h"
#include "GIS_funcs.h"

URing::~URing()
{
	_LocationArr.clear();
	_CoverMeshArr.clear();
}
void URing::CalcCoverMesh()
{
	_CoverMeshArr.clear();
	gdem_DivisionToTriangle(_LocationArr, _CoverMeshArr);
}

UPolygon::UPolygon(void)
{

}

UPolygon::~UPolygon(void)
{
	Clear();
}

void UPolygon::Clear()
{
	for(unsigned int i=0;i<RingCount();i++)
		delete _pRingArr[i];
	_pRingArr.clear();
}

void UPolygon::RecalcBound()
{
	m_cBound.m_cMinPt.m_tX = gdm_PI;
	m_cBound.m_cMinPt.m_tY = gdm_PI/2;
	m_cBound.m_cMaxPt.m_tX = -gdm_PI;
	m_cBound.m_cMaxPt.m_tY = -gdm_PI/2;

	for(unsigned int i=0;i<RingCount();i++)
	{
		URing* ring=GetRing(i);

		for(unsigned int j=0;j<ring->PointCount();j++)
		{
			CGDM_Location3D pt=ring->_LocationArr[j];

			m_cBound.addInternalPoint(pt.m_tX, pt.m_tY);
		}
	}
}



