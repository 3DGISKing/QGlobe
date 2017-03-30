#include "GIS_Point.h"
#include <QMetaType>

CGIS_Point::CGIS_Point(void)
{
	m_nGeoType  = E_GEONODE_TYPE_POINT;
}

CGIS_Point::~CGIS_Point(void)
{
}

CGIS_Point &	CGIS_Point::operator=(const CGIS_Point & cNode)
{
	if(this != &cNode)
	{
		CopyFrom(cNode);
	}

	return *this;
}

void CGIS_Point::CopyFrom(const CGIS_Node & cNode)
{
	if(cNode.m_nGeoType == E_GEONODE_TYPE_POINT)
	{
		CGIS_Point *pNode = (CGIS_Point *)&cNode;

		CGIS_Node::CopyFrom(cNode);
		
		m_cPt		= pNode->m_cPt;	
	}
}

void CGIS_Point::Write(QDataStream &out)
{
	CGIS_Node::Write(out);

	out << m_cPt.m_tX;
	out << m_cPt.m_tY;
	out << m_cPt.m_tZ;
}

void CGIS_Point::Read(QDataStream &in)
{
	CGIS_Node::Read(in);

	in >> m_cPt.m_tX;
	in >> m_cPt.m_tY;
	in >> m_cPt.m_tZ;

	ResetBound();
}

void CGIS_Point::SetPt(double x, double y, double z)
{
	m_cPt.m_tX = x;
	m_cPt.m_tY = y;
	m_cPt.m_tZ = z;
	ResetBound();
}

void CGIS_Point::Set_TrackPt(unsigned int trackIndex, CGDM_Location3D *pPt) 
{ 
	m_cPt = *pPt;
	ResetBound();
}	

void CGIS_Point::ResetBound()
{
	m_cBound.m_cMinPt.m_tX = m_cPt.m_tX;
	m_cBound.m_cMinPt.m_tY = m_cPt.m_tY;
	m_cBound.m_cMaxPt.m_tX = m_cPt.m_tX;
	m_cBound.m_cMaxPt.m_tY = m_cPt.m_tY;
}
void CGIS_Point::Get_TrackPt(unsigned int trackIndex, CGDM_Location3D *pPt)
{
	if(trackIndex == 0 && pPt)
		*pPt = m_cPt;
}

CGIS_Node* CGIS_Point::Clone()
{
	CGIS_Node *pNode = new CGIS_Point();
	pNode->CopyFrom(*this);
	return pNode;
}
void CGIS_Point::SetScreenPos(double x, double y)
{
	m_dScreenX	= x;
	m_dScreenY	= y;
}

bool CGIS_Point::IsContain(MOUSE_POSITION &pos)
{
	if(fabs(m_dScreenX  - pos.x) < pos.rangeInScreen && 
		fabs(m_dScreenY - pos.y) < pos.rangeInScreen)
		return true;
	return false;
}
