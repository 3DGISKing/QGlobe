#pragma once

#include "GIS_Node.h"
#include "../qglobe_base/QGlobe_Location.h"
#include "GIS_PathData.h"

// gis node type
typedef enum 
{
	E_GISDRAWPOLY_LINE,		// draw only outline
	E_GISDRAWPOLY_FILL,		// draw only surface
	E_GISDRAWPOLY_ALL,		// draw outline and fill
	E_GISDRAWPOLY_NONE		// draw none
} E_GISDRAWPOLY_TYPE;

// gis node type
typedef enum 
{
	E_GISPOLYHEIGHT_CLAMPED_TO_GROUND,		// �������
	//E_GISPOLYHEIGHT_CLAMPED_TO_SEA,		// �ٴٸ����
	E_GISPOLYHEIGHT_RELATIVE_TO_GROUND,		// �������
	//E_GISPOLYHEIGHT_RELATIVE_TO_SEA,		// �ٴٸ����
	E_GISPOLYHEIGHT_ABSOLUTE				// �������
} E_GISPOLYHEIGHT_TYPE;

class CQGlobe_Camera;

class QGLOBE_GIS_EXPORT CGIS_Polygon : public CGIS_Node
{
public:
	CGIS_Polygon(void);
	~CGIS_Polygon(void);

public:
	void				AddPoint(CQGlobe_Location3D &pPt);
	void				InsertPoint(unsigned int index , CQGlobe_Location3D &cPt);
	void				AddPoints(CQGlobe_Location3D *pPts, int count);
	void				RemovePoint(int index);

	void				SetPoint(CQGlobe_Location3D &cPt, int index);
	void				Clear();
	unsigned int		GetCount()	{return m_PtArr.size();}
	virtual void		Refresh(bool clearAll = true);

	// track functions for select or modify
	virtual unsigned int	Get_TrackCount();
	virtual void			Get_TrackPt(unsigned int trackIndex, CQGlobe_Location3D *pPt);
	virtual void			Set_TrackPt(unsigned int trackIndex, CQGlobe_Location3D *pPt);

	// call calculation
	virtual void		OnCalculateGISInfo(void) {};

	// for select
	virtual bool		IsContain(MOUSE_POSITION &pos);

	// operator 
	CGIS_Polygon &		operator=(const CGIS_Polygon & cNode);
	virtual void		CopyFrom(const CGIS_Node & cNode);
	virtual CGIS_Node *	Clone();
	virtual void	    Write(QDataStream &out);
	virtual void	    Read(QDataStream &out);
	bool				IsHeight() {return m_blHeight;}
	bool				IsDrawVectorLine();

	bool				IsIntersect(CQGlobe_Rect2D &bound);
	bool				IsContain(CQGlobe_Rect2D &bound);
	bool				IsContain(CQGlobe_Location3D &pos);
	bool				IsContain(double x, double y);
	bool				IsFill();
	void				SetHeightValue(CQGlobe_Location3D &pt);
private:
	bool				IsContainNoCheck(double x, double y);
public:
	void				GetDrawSegPoints(CQGlobe_LocationArr &locArr, CQGlobe_Camera *pCamera);
	CQGlobe_MeshArr*		GetDrawCoverMesh(){return &m_cArrMesh;}
	void				SetMeasureMode(MeasureInfo measureInfo) {m_MeasureInfo = measureInfo;}
	QString				ConvertLengthToString();

private:
	void				GetDrawSegPoints(CQGlobe_LocationArr* pLocArr, CQGlobe_Location3D &pt1, CQGlobe_Location3D &pt2, double distEye);
	int					GetValidDivid(QGlobe_LOCATION *pStart, QGlobe_LOCATION *pEnd, double	&dHeight, double distEye);	
	void				GetSegPoints(CQGlobe_LocationArr* pLocationArr, QGlobe_LOCATION *pStart, QGlobe_LOCATION *pEnd, int nDiv);

private:
	void				ReCalcuLength();
	void				ReCalcuRange();
	void				ReCalcuAreaVolume();
	void				ResetDivisionPoints();
	void				ReCalcuCoverMesh();
	void				CheckPathData();


public:
	// setting member
	double						m_dHeight;		// �ع߰� (������)
	E_GISDRAWPOLY_TYPE			m_nDrawType;	// �׸��� ���� 
	E_GISPOLYHEIGHT_TYPE		m_nHeightType;	//���� ����

	// calculated GIS information value
	double				m_dLength;				// ���� 
	double				m_dArea;				// ���� 
	double				m_dVolume;				// ����ü��

	CQGlobe_LocationArr	m_PtArr;				// the points
	bool				m_blClose;				// the flag of close
	bool				m_blHeight;				// check display volume flag
	unsigned short	    m_sTextureName[PMD_STR_MAX_SIZE]; //by RSH 2013.7.30

protected:
	CQGlobe_LocationArr	m_cArrDivPt;			// DividedPt
	CQGlobe_MeshArr		m_cArrMesh;				// drawing covermesh

private:
	double				m_dMinDivLen;
	MeasureInfo			m_MeasureInfo;

	//kimyi add
private:
	GIS_PathData		m_PathData;
public:
	GIS_PathData*		GetPathData() { return &m_PathData; }
};
