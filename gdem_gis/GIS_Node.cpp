#include "stable.h"
#include "GIS_Node.h"
#include <QMetaType>

CGIS_Node::CGIS_Node(void)
{
	m_nGeoType      = E_GEONODE_TYPE_NONE;
	m_nGisType      = E_GISNODE_TYPE_NONE;

	memset(&m_sPen, 0, sizeof(m_sPen));
	memset(&m_sBrush, 0, sizeof(m_sBrush));

	m_nState		= E_GISNODE_STATE_NONE;
	m_blVisible		= true;
	m_sName[0]		= 0;
	m_nKey			= 0;
	m_blEditing		= false;
	m_blCalculated  = false;
	m_pProgress		= NULL;
	m_blEnable		= true;
}

CGIS_Node::~CGIS_Node(void)
{
}

CGIS_Node &	CGIS_Node::operator=(const CGIS_Node & cNode)
{
	if(this != &cNode)
	{
		CopyFrom(cNode);
	}

	return *this;
}
void CGIS_Node::CopyFrom(const CGIS_Node & cNode)
{
		m_nGisType		= cNode.m_nGisType;
		m_nGeoType		= cNode.m_nGeoType;	
		m_sPen			= cNode.m_sPen;		
		m_sBrush		= cNode.m_sBrush;
		m_blVisible		= cNode.m_blVisible;
		m_nKey			= cNode.m_nKey;	
		m_nState		= cNode.m_nState;
		m_cBound		= cNode.m_cBound;

		memcpy(m_sName, cNode.m_sName, sizeof(short) * PMD_STR_MAX_SIZE);
}

void CGIS_Node::Write(QDataStream &out)
{
	out << m_nGisType;
	out << m_nGeoType;
	out << m_sPen.color;
	out << m_sPen.width;
	out << m_sBrush.color;
	out << m_nKey;
//	out << m_sName;
}

void CGIS_Node::Read(QDataStream &in)
{
	unsigned int type;

	in >> type;
	m_nGisType = (E_GISNODE_TYPE)type;
	in >> type;
	m_nGeoType = (E_GEONODE_TYPE)type;

	in >> m_sPen.color;
	in >> m_sPen.width;
	in >> m_sBrush.color;
	in >> m_nKey;
//	in >> m_sName;
}

bool CGIS_Node::IsVisible() 
{
	if(m_blEnable && m_blVisible)
		return true;
	if(m_blEnable && GetState() == E_GISNODE_STATE_SELECT)
		return true;
	return false;
}

CGIS_Node* CGIS_Node::Clone()
{
	CGIS_Node *pNode = new CGIS_Node();
	pNode->CopyFrom(*this);
	return pNode;
}

void CGIS_Node::SetFocus(bool blFocus)
{
	if(m_nState == E_GISNODE_STATE_SELECT)
		return;

	if(blFocus)
		m_nState = E_GISNODE_STATE_FOCUS;
	else
		m_nState = E_GISNODE_STATE_NONE;
}