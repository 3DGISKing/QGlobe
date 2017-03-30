#include "GDM_QuadNode.h"

#define ALPHA_BASE 0x30

GDM_QUAD_NODE::~GDM_QUAD_NODE()
{
	delete valid;
	delete invalid;
}


CGDM_Node_Mgr::CGDM_Node_Mgr(void)
{
	m_Count = 0;
	m_root = NewNode(NULL, 0);
	m_blLock = false;
}

CGDM_Node_Mgr::~CGDM_Node_Mgr(void)
{
	ClearNode(m_root);
}

void CGDM_Node_Mgr::ClearAll()
{
	m_blLock = true;
	ClearNode(m_root);
	m_root = NewNode(NULL, 0);
	m_blLock = false;
}

// set node flag
void CGDM_Node_Mgr::SetValidFlag(int nX, int nY, int nLevel, E_GDM_SUBDATA_TYPE type, E_NODE_VALID_FLAG flag)
{
	if(m_blLock)
		return;
	GDM_QUAD_NODE *pNode = SetNode(nX, nY, nLevel);

	if(pNode == NULL)
		return;
	
	switch(flag)
	{
	case E_NODE_VALID_FLAG_VALID:
		pNode->valid->setBit(type);
		break;
	case E_NODE_VALID_FLAG_INVALID:
		{
#ifdef DEBUG_LOG
			if(type == E_GDM_SUBDATA_TYPE_IMG)
			{
				char name[GDM_MAX_LEVEL];
				GetPrintNodeName(nX, nY, nLevel, name);
				qDebug("** ImageTile(X[%d],Y[%d],Level[%d], name[%s]) : no exist", nX, nY, nLevel, name);
			}
#endif
		}
		pNode->invalid->setBit(type) ;
		break;
	}
}

// get node flag
E_NODE_VALID_FLAG CGDM_Node_Mgr::GetValidFlag(int nX, int nY, int nLevel, E_GDM_SUBDATA_TYPE type)
{
	if(m_blLock)
		return E_NODE_VALID_FLAG_NONE;

	GDM_QUAD_NODE *pNode = GetNode(nX, nY, nLevel);

	if(pNode == NULL)
		return E_NODE_VALID_FLAG_NONE;

	long typeFlag = 1 << type;
	
	if(pNode->valid->testBit(type))
		return E_NODE_VALID_FLAG_VALID;
	else if(pNode->invalid->testBit(type))
		return E_NODE_VALID_FLAG_INVALID;

	return E_NODE_VALID_FLAG_NONE;
}

// retern node, if there is no node then create node and return it
GDM_QUAD_NODE * CGDM_Node_Mgr::SetNode(int nX, int nY, int nLevel)
{
	char name[GDM_MAX_LEVEL];
	GDM_QUAD_NODE *pNode = m_root;
	int level = 1, id;

	if(nLevel >= (GDM_MAX_LEVEL-1))
		return NULL;

	GetNodeName(nX, nY, nLevel, name);

	while(level <= nLevel)
	{
		if(pNode == NULL)
			return NULL;
		if(m_blLock)
			return NULL;
		id = name[level] - ALPHA_BASE;
		if(id < 0 || id > GDM_MAX_NODE_CHILD)
		{
#ifdef DEBUG
			qDebug("Invalide name: %s", name);
#endif
			return NULL;
		}

		if(pNode->child[id] == NULL)
		{
			NewNode(pNode, id);
		}

		pNode = pNode->child[id];
		level++;
	}

	return pNode;
}

// return node
GDM_QUAD_NODE * CGDM_Node_Mgr::GetNode(int nX, int nY, int nLevel)
{
	char name[GDM_MAX_LEVEL];
	GDM_QUAD_NODE *pNode = m_root;
	int level = 1, id;

	if(nLevel >= (GDM_MAX_LEVEL - 1))
		return NULL;

	GetNodeName(nX, nY, nLevel, name);

	while(level <= nLevel)
	{
		if(pNode == NULL)
			return NULL;

		if(m_blLock)
			return NULL;

		id = name[level] - ALPHA_BASE;

		if(id < 0 || id > GDM_MAX_NODE_CHILD)
		{
#ifdef DEBUG
			qDebug("Invalide name: %s", name);
#endif
			return NULL;
		}

		pNode = pNode->child[id];

		level++;
	}

	return pNode;
}

void CGDM_Node_Mgr::ClearNode(GDM_QUAD_NODE *pNode)
{
	Q_ASSERT(pNode!=NULL);

	int i = 0;

	// remove this from parent
	if(pNode->parent != NULL)
		pNode->parent->child[pNode->index] = NULL;

	// clear child node
	for(i = 0; i < GDM_MAX_NODE_CHILD; i++)
	{
		if(pNode->child[i] != NULL)
			ClearNode(pNode->child[i]);
	}

	// free self
	delete pNode;
}

GDM_QUAD_NODE* CGDM_Node_Mgr::NewNode(GDM_QUAD_NODE *parent, int index)
{
	Q_ASSERT(index >=0 && index<GDM_MAX_NODE_CHILD);

	if(parent)
		Q_ASSERT(parent->child[index]==NULL);

	if(m_Count > GDM_MAX_NODE_COUNT)
		return NULL;

	GDM_QUAD_NODE *pNode = NULL;

	m_Count++;

	pNode = new GDM_QUAD_NODE();

	pNode->index = (char)index;

	if(parent)
	{
		pNode->level			= parent->level + 1;
		pNode->parent			= parent;
		parent->child[index]	= pNode;
		pNode->index			= index;
	}
	else
		pNode->level = 0;

	return pNode;
}

void CGDM_Node_Mgr::GetNodeName(int nX, int nY, int nLevel, GDM_NODE_NAME name)
{
	name[nLevel + 1] = 0;

	while(nLevel >= 0)
	{
		name[nLevel] = ((nX & 0x01) + (nY & 0x01) * 2) + ALPHA_BASE;
		nLevel--;
		nX = nX >> 1;
		nY = nY >> 1;
	}
}

void CGDM_Node_Mgr::GetPrintNodeName(int nX, int nY, int nLevel, GDM_NODE_NAME name)
{
	name[nLevel + 1] = 0;

	while(nLevel >= 0)
	{
		if((nY & 0x01) == 0)
			name[nLevel] = (nX & 0x01) + ALPHA_BASE;
		else
			name[nLevel] = (3 - (nX & 0x01)) + ALPHA_BASE;

		nLevel--;
		nX = nX >> 1;
		nY = nY >> 1;
	}
}

