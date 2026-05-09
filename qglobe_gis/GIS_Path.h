#pragma once
#include "GIS_Polygon.h"

class QGLOBE_GIS_EXPORT CGIS_Path : public CGIS_Polygon
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
										QGlobe_POSITION &sMaxRatePos, QGlobe_POSITION &sMaxHeightPos, QGlobe_POSITION &sMinHeightPos);

public:

	// calculated GIS information value
	QGlobe_POSITION	m_sMaxRatePos;			// �ִ�����
	QGlobe_POSITION	m_sMaxHeightPos;		// �ְ�������
	QGlobe_POSITION	m_sMinHeightPos;		// ���ϳ�����
	double			m_dLengthOnSurface;		// ������������
};
