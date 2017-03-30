#pragma once
#include <QtCore/QBitArray>

#include "GDM_Mem.h"

#include "gdem_protocol.h"
#include "gdem_libmain_global.h"

#define GDM_MAX_LEVEL		20
#define GDM_MAX_NODE_CHILD	4
#define GDM_MAX_NODE_COUNT	1000000

typedef char* GDM_NODE_NAME;

/* An enumerated type to specify the status of tile */
typedef enum {
	E_NODE_VALID_FLAG_NONE,		/* none */
	E_NODE_VALID_FLAG_INVALID,  /* invalid*/
	E_NODE_VALID_FLAG_VALID,    /* valid*/
} E_NODE_VALID_FLAG;


// Describe the quad tile in Quad Tree
class GDM_QUAD_NODE
{
public:
	GDM_QUAD_NODE()
	{
		valid=new QBitArray(E_GDM_SUBDATA_TYPE_MAX);
		invalid=new QBitArray(E_GDM_SUBDATA_TYPE_MAX);
		level=0;
		index=0;
		parent=NULL;
		child[0]=child[1]=child[2]=child[3]=NULL;
	}

	~GDM_QUAD_NODE();

	char		level;								// level number(0-20)
	char		index;								// child number(0-3)
	QBitArray*	valid;								// valid flag
	QBitArray*	invalid;							// invalid flag

	GDM_QUAD_NODE*	parent;						// pointer to parent
	GDM_QUAD_NODE*	child[GDM_MAX_NODE_CHILD];	// children pointer 
} ;


class GDEM_LIBMAIN_EXPORT CGDM_Node_Mgr
{
public:
	CGDM_Node_Mgr(void);
	~CGDM_Node_Mgr(void);

public:
	void				SetValidFlag(int nX, int nY, int nLevel, E_GDM_SUBDATA_TYPE type, E_NODE_VALID_FLAG flag);
	E_NODE_VALID_FLAG	GetValidFlag(int nX, int nY, int nLevel, E_GDM_SUBDATA_TYPE type);
	void				ClearAll();

private:
	GDM_QUAD_NODE*		m_root;	/* Callback function */
	int					m_Count;
	bool				m_blLock;

private:
	void				ClearNode(GDM_QUAD_NODE *pNode);
	GDM_QUAD_NODE*		NewNode(GDM_QUAD_NODE *parent, int index);
	void				GetNodeName(int nX, int nY, int nLevel, GDM_NODE_NAME name);
	GDM_QUAD_NODE*		GetNode(int nX, int nY, int nLevel);
	GDM_QUAD_NODE*		SetNode(int nX, int nY, int nLevel);
	void				GetPrintNodeName(int nX, int nY, int nLevel, GDM_NODE_NAME name);
};

