#include "GIS_Doc.h"

CGIS_Doc *g_pGdm_GIS_doc;

CGIS_Doc* gdm_GetGISDoc()
{
	return g_pGdm_GIS_doc;
}

CGIS_Doc::CGIS_Doc(void)
{
	g_pGdm_GIS_doc = this;
	m_pSelectNode = NULL;
}

CGIS_Doc::~CGIS_Doc(void)
{
}

void CGIS_Doc::RemoveNode(CGIS_Node *pNode)
{
	int index = m_arrNode.binary_search(pNode);
	if(index >= 0)
	{
		m_arrNode.erase(index);
		SetModifyFlag(true);
	}
}

void CGIS_Doc::DestroyNodeFromType(E_GISNODE_TYPE type)
{
	CGIS_Node* pNode;
	int index;
	for(index = GetCount() - 1; index >= 0; index--)
	{
		pNode = m_arrNode[index];
		if(pNode->Get_GisType() == type)
		{
			delete pNode;
			m_arrNode.erase(index);
			SetModifyFlag(true);
		}
	}
}

void CGIS_Doc::SetSelectNode(CGIS_Node* pNode)
{
	int index;
	for(index = GetCount() - 1; index >= 0; index--)
	{
		m_arrNode[index]->SetState(E_GISNODE_STATE_NONE);
	}
	if(pNode)
		pNode->SetState(E_GISNODE_STATE_SELECT);
	m_pSelectNode = pNode;
}

CGIS_Node* CGIS_Doc::GetNodeFromIndex(unsigned int index)
{
	if(index > GetCount())
		return NULL;
	return m_arrNode[index];
}

void CGIS_Doc::RemoveNodeFromKey(unsigned int key)
{
	unsigned int index, count = GetCount();

	for(index = 0; index < count; index++)
	{
		if(m_arrNode[index]->GetKey() == key)
		{
			m_arrNode.erase(index);
			SetModifyFlag(true);
			return;
		}
	}
}

void CGIS_Doc::DestroyNodeFromKey(unsigned int key)
{
	unsigned int index, count = GetCount();
	CGIS_Node* pNode;

	for(index = 0; index < count; index++)
	{
		pNode = m_arrNode[index];
		if(pNode->GetKey() == key)
		{
			m_arrNode.erase(index);
			SetModifyFlag(true);
			if(pNode == m_pSelectNode)
				m_pSelectNode = NULL;
			delete pNode;
			return;
		}
	}
}


CGIS_Node* CGIS_Doc::GetNodeFromKey(unsigned int key)
{
	unsigned int index, count = GetCount();

	for(index = 0; index < count; index++)
	{
		if(m_arrNode[index]->GetKey() == key)
			return m_arrNode[index];
	}

	return NULL;
}

bool CGIS_Doc::AddNode(CGIS_Node *pNode)
{
	unsigned int key = pNode->GetKey();
	unsigned int index, count = GetCount();

	for(index = 0; index < count; index++)
	{
		if(m_arrNode[index]->GetKey() > 0  && m_arrNode[index]->GetKey() == key)
		{
			return false;
		}
	}

	m_arrNode.push_back(pNode);
	SetModifyFlag(true);
	return true;
}