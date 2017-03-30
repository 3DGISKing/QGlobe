#pragma once
#include "GIS_Polygon.h"

class GDEM_GIS_EXPORT CGIS_Path : public CGIS_Polygon
{

public:
	CGIS_Path(void);
	~CGIS_Path(void);

public:

	// call calculation
	virtual void	OnCalculateGISInfo(void);

	// operator 
	CGIS_Path &		operator=(const CGIS_Path & cNode);
	virtual void	CopyFrom(const CGIS_Node & cNode);
	virtual CGIS_Node *		Clone();
	virtual void	Write(QDataStream &out);
	virtual void	Read(QDataStream &in); //asd 2014.3.31
	virtual void	Refresh(bool clearAll = true);
private:
	void OnCalculateGISInfoWithoutTerrian(int m, int n, int level, double &dLengthOnSurface,
										GDM_POSITION &sMaxRatePos, GDM_POSITION &sMaxHeightPos, GDM_POSITION &sMinHeightPos);

public:

	// calculated GIS information value
	GDM_POSITION	m_sMaxRatePos;			// 최대경사점
	GDM_POSITION	m_sMaxHeightPos;		// 최고높은점
	GDM_POSITION	m_sMinHeightPos;		// 최하낮은점
	double			m_dLengthOnSurface;		// 지형고려길이
};
