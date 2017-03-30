#pragma once

#include "GIS_Node.h"
#include "GDM_Array.h"
#include "gdem_gis_global.h"

#define	GIS_SELECT_RANGE	3

class CGIS_Doc;

GDEM_GIS_EXPORT CGIS_Doc* gdm_GetGISDoc();

class GDEM_GIS_EXPORT CGIS_Doc
{
public:
	CGIS_Doc(void);
	~CGIS_Doc(void);
public:
	bool			AddNode(CGIS_Node *pNode);
	void			RemoveNode(CGIS_Node *pNode);
	void			RemoveNodeFromKey(unsigned int key);
	void			DestroyNodeFromKey(unsigned int key);
	void			DestroyNodeFromType(E_GISNODE_TYPE type);
	CGIS_Node*		GetNodeFromIndex(unsigned int index);
	CGIS_Node*		GetNodeFromKey(unsigned int key);
	unsigned int	GetCount(void) {return m_arrNode.size();}

	// for select
	CGIS_Node*		GetSelectNode() {return m_pSelectNode;}
	void			SetSelectNode(CGIS_Node* pNode);
	void			SetModifyFlag(bool blModify = true) { m_blModified = blModify;}
	bool			IsModified() {return m_blModified;}
public:
	gdm_array<CGIS_Node*>	m_arrNode;

private:
	CGIS_Node*		m_pSelectNode;
	bool			m_blModified;
};
