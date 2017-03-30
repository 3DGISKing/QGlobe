#include "GIS_PlaceMark.h"

CGIS_PlaceMark::CGIS_PlaceMark(void)
{
	m_nGisType = E_GISNODE_TYPE_PLACEMARK;
}

CGIS_PlaceMark::~CGIS_PlaceMark(void)
{
}

CGIS_PlaceMark &	CGIS_PlaceMark::operator=(const CGIS_PlaceMark & cNode)
{
	if(this != &cNode)
	{
		CGIS_Point::operator = (cNode);
		
		m_blSearch			= cNode.m_blSearch;	
	}

	return *this;
}

CGIS_Node* CGIS_PlaceMark::Clone()
{
	CGIS_Node *pNode = new CGIS_PlaceMark();
	*pNode = *this;
	return pNode;
}
