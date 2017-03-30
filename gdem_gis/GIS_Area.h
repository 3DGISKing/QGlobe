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
	GDM_POSITION	m_sMaxRatePos;			// �ִ�����
	GDM_POSITION	m_sMaxHeightPos;		// �ְ������
	GDM_POSITION	m_sMinHeightPos;		// ���ϳ�����
	double			m_dLengthOnSurface;		// �����������
	double			m_dAreaOnSurface;		// �����������
	double			m_dVolumeOnSurface;		// �������� �����ϴ°�� ü��

private:
	void			OnCalculateGISInfoWithoutTerrian(int m, int n, int level,
													double &dLengthOnSurface, double &dAreaOnSurface, double &dVolumeOnSurface,
													GDM_POSITION &sMaxRatePos, GDM_POSITION &sMaxHeightPos, GDM_POSITION &sMinHeightPos,
													CGDM_LocationArr	&arrUserPolygonPts); // Dem������ ���� ������ GIS Info���.
	double			GetAreaInMeshWithDem(GDEM_MESH &mesh);
	double          GetVolumeInMeshWithDem(GDEM_MESH &mesh);
};
