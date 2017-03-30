#pragma once
#include "GIS_Polygon.h"

class GDEM_GIS_EXPORT CGIS_Area : public CGIS_Polygon
{
public:
	CGIS_Area(void);
	~CGIS_Area(void);

	// call calculation
	virtual void	OnCalculateGISInfo(void);

	// operator 
	CGIS_Area &		operator=(const CGIS_Area & cNode);
	virtual void	CopyFrom(const CGIS_Node & cNode);
	virtual CGIS_Node *		Clone();
	virtual void	Write(QDataStream &out);
	virtual void	Read(QDataStream &out);
	virtual void	Refresh(bool clearAll = true);

public:

	// calculated GIS information value
	GDM_POSITION	m_sMaxRatePos;			// 최대경사점
	GDM_POSITION	m_sMaxHeightPos;		// 최고높은점
	GDM_POSITION	m_sMinHeightPos;		// 최하낮은점
	double			m_dLengthOnSurface;		// 지형고려길이
	double			m_dAreaOnSurface;		// 지형고려면적
	double			m_dVolumeOnSurface;		// 지형까지 연장하는경우 체적

private:
	void			OnCalculateGISInfoWithoutTerrian(int m, int n, int level,
													double &dLengthOnSurface, double &dAreaOnSurface, double &dVolumeOnSurface,
													GDM_POSITION &sMaxRatePos, GDM_POSITION &sMaxHeightPos, GDM_POSITION &sMinHeightPos,
													CGDM_LocationArr	&arrUserPolygonPts); // Dem정보가 없는 지형의 GIS Info계산.
	double			GetAreaInMeshWithDem(GDEM_MESH &mesh);
	double          GetVolumeInMeshWithDem(GDEM_MESH &mesh);
};
