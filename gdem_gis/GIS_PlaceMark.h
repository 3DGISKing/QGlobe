#pragma once
#include "GIS_Point.h"

class GDEM_GIS_EXPORT CGIS_PlaceMark : 	public CGIS_Point
{
public:
	CGIS_PlaceMark(void);
	~CGIS_PlaceMark(void);

	// operator 
	CGIS_PlaceMark &		operator=(const CGIS_PlaceMark & cNode);
	virtual CGIS_Node *		Clone();

public:
	bool	m_blSearch;	// the place mark for search
};
