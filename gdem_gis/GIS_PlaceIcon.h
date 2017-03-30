#pragma once
#include "GIS_Point.h"

class GDEM_GIS_EXPORT CGIS_PlaceIcon : 	public CGIS_Point
{
public:
	CGIS_PlaceIcon(void);
	~CGIS_PlaceIcon(void);

	// operator 
	CGIS_PlaceIcon &		operator=(const CGIS_PlaceIcon & cNode);
	virtual void			CopyFrom(const CGIS_Node & cNode);
	virtual CGIS_Node *		Clone();

public:
	bool	m_blSearch;	    // the place mark for search
	
	unsigned short	    m_sIconName[PMD_STR_MAX_SIZE]; //by RSH 2013.7.30
	double              m_IconScale;                   //by ugi 2013.8.1
	unsigned short	    m_sVideoPath[PMD_STR_MAX_SIZE]; //by ugi 2014.2.6
	unsigned short	    m_sHtmlPath[PMD_STR_MAX_SIZE]; //by ugi 2014.2.7
};
