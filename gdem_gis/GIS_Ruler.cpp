#include "GIS_Ruler.h"

CGIS_Ruler::CGIS_Ruler(void)
{
	m_nGisType		= E_GISNODE_TYPE_RULER;
	m_sPen.color	= 0xffffff00; // yellow
	m_sPen.width	= 2;
	m_blClose		= false;
}

CGIS_Ruler::~CGIS_Ruler(void)
{
}

CGIS_Ruler &	CGIS_Ruler::operator=(const CGIS_Ruler & cNode)
{
	if(this != &cNode)
	{
		CopyFrom(cNode);
	}

	return *this;
}
void CGIS_Ruler::CopyFrom(const CGIS_Node & cNode)
{
	if(cNode.m_nGisType == E_GISNODE_TYPE_RULER)
	{
		CGIS_Ruler *pNode = (CGIS_Ruler *)&cNode;

		CGIS_Polygon::CopyFrom(cNode);
		
	}
}

CGIS_Node* CGIS_Ruler::Clone()
{
	CGIS_Node *pNode = new CGIS_Ruler();
	pNode->CopyFrom(*this);
	return pNode;
}
