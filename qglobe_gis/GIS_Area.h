#pragma once
#include "GIS_Polygon.h"

class QGLOBE_GIS_EXPORT CGIS_Area : public CGIS_Polygon
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
	QGlobe_POSITION	m_sMaxRatePos;			// �ִ�����
	QGlobe_POSITION	m_sMaxHeightPos;		// �ְ�������
	QGlobe_POSITION	m_sMinHeightPos;		// ���ϳ�����
	double			m_dLengthOnSurface;		// ������������
	double			m_dAreaOnSurface;		// ������������
	double			m_dVolumeOnSurface;		// �������� �����ϴ°�� ü��

private:
	void			OnCalculateGISInfoWithoutTerrian(int m, int n, int level,
													double &dLengthOnSurface, double &dAreaOnSurface, double &dVolumeOnSurface,
													QGlobe_POSITION &sMaxRatePos, QGlobe_POSITION &sMaxHeightPos, QGlobe_POSITION &sMinHeightPos,
													CQGlobe_LocationArr	&arrUserPolygonPts); // Dem������ ���� ������ GIS Info���.
	double			GetAreaInMeshWithDem(QGLOBE_MESH &mesh);
	double          GetVolumeInMeshWithDem(QGLOBE_MESH &mesh);
};
