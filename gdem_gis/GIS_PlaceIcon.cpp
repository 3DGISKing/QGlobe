#include "GIS_PlaceIcon.h"

CGIS_PlaceIcon::CGIS_PlaceIcon(void)
{
	m_nGisType = E_GISNODE_TYPE_PLACEMARK;
	m_sIconName[0]=0;
	m_IconScale=1.0;
	m_sVideoPath[0]=0;//2014 2 6 by ugi
	m_sHtmlPath[0]=0; //2014 2 7 by ugi
}

CGIS_PlaceIcon::~CGIS_PlaceIcon(void)
{
}

CGIS_PlaceIcon &	CGIS_PlaceIcon::operator=(const CGIS_PlaceIcon & cNode)
{
	if(this != &cNode)
	{
		CopyFrom(cNode);
	}

	return *this;
}

void CGIS_PlaceIcon::CopyFrom(const CGIS_Node & cNode)
{
	if(cNode.m_nGisType == E_GISNODE_TYPE_PLACEMARK || cNode.m_nGisType == E_GISNODE_TYPE_PLACESEARCH)
	{
		CGIS_PlaceIcon *pNode = (CGIS_PlaceIcon *)&cNode;

		CGIS_Point::CopyFrom(cNode);

		m_blSearch= pNode->m_blSearch;
	}
}


CGIS_Node* CGIS_PlaceIcon::Clone()
{
	CGIS_Node *pNode = new CGIS_PlaceIcon();

	pNode->CopyFrom(*this);
	return pNode;
}
