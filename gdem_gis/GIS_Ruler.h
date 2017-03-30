#pragma once
#include "GIS_Polygon.h"

class GDEM_GIS_EXPORT CGIS_Ruler : 	public CGIS_Polygon
{
public:
	CGIS_Ruler(void);
	~CGIS_Ruler(void);

public:
	// operator 
	CGIS_Ruler &		operator=(const CGIS_Ruler & cNode);
	virtual void		CopyFrom(const CGIS_Node & cNode);
	virtual CGIS_Node *	Clone();

};
