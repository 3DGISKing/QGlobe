#pragma once

#include "GIS_Node.h"
#include "../gdem_base/GDM_Location.h"
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
	E_GISPOLYHEIGHT_CLAMPED_TO_GROUND,		// 지면고정
	//E_GISPOLYHEIGHT_CLAMPED_TO_SEA,		// 바다면고정
	E_GISPOLYHEIGHT_RELATIVE_TO_GROUND,		// 지면기준
	//E_GISPOLYHEIGHT_RELATIVE_TO_SEA,		// 바다면기준
	E_GISPOLYHEIGHT_ABSOLUTE				// 절대고도
} E_GISPOLYHEIGHT_TYPE;

class CGDM_Camera;

class GDEM_GIS_EXPORT CGIS_Polygon : public CGIS_Node
{
public:
	CGIS_Polygon(void);
	~CGIS_Polygon(void);

public:
	void				AddPoint(CGDM_Location3D &pPt);
	void				InsertPoint(unsigned int index , CGDM_Location3D &cPt);
	void				AddPoints(CGDM_Location3D *pPts, int count);
	void				RemovePoint(int index);

	void				SetPoint(CGDM_Location3D &cPt, int index);
	void				Clear();
	unsigned int		GetCount()	{return m_PtArr.size();}
	virtual void		Refresh(bool clearAll = true);

	// track functions for select or modify
	virtual unsigned int	Get_TrackCount();
	virtual void			Get_TrackPt(unsigned int trackIndex, CGDM_Location3D *pPt);
	virtual void			Set_TrackPt(unsigned int trackIndex, CGDM_Location3D *pPt);

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

	bool				IsIntersect(CGDM_Rect2D &bound);
	bool				IsContain(CGDM_Rect2D &bound);
	bool				IsContain(CGDM_Location3D &pos);
	bool				IsContain(double x, double y);
	bool				IsFill();
	void				SetHeightValue(CGDM_Location3D &pt);
private:
	bool				IsContainNoCheck(double x, double y);
public:
	void				GetDrawSegPoints(CGDM_LocationArr &locArr, CGDM_Camera *pCamera);
	CGDM_MeshArr*		GetDrawCoverMesh(){return &m_cArrMesh;}
	void				SetMeasureMode(MeasureInfo measureInfo) {m_MeasureInfo = measureInfo;}
	QString				ConvertLengthToString();

private:
	void				GetDrawSegPoints(CGDM_LocationArr* pLocArr, CGDM_Location3D &pt1, CGDM_Location3D &pt2, double distEye);
	int					GetValidDivid(GDM_LOCATION *pStart, GDM_LOCATION *pEnd, double	&dHeight, double distEye);	
	void				GetSegPoints(CGDM_LocationArr* pLocationArr, GDM_LOCATION *pStart, GDM_LOCATION *pEnd, int nDiv);

private:
	void				ReCalcuLength();
	void				ReCalcuRange();
	void				ReCalcuAreaVolume();
	void				ResetDivisionPoints();
	void				ReCalcuCoverMesh();
	void				CheckPathData();


public:
	// setting member
	double						m_dHeight;		// 해발고 (설정값)
	E_GISDRAWPOLY_TYPE			m_nDrawType;	// 그리기 형태 
	E_GISPOLYHEIGHT_TYPE		m_nHeightType;	//높이 기준

	// calculated GIS information value
	double				m_dLength;				// 길이 
	double				m_dArea;				// 면적 
	double				m_dVolume;				// 절대체적

	CGDM_LocationArr	m_PtArr;				// the points
	bool				m_blClose;				// the flag of close
	bool				m_blHeight;				// check display volume flag
	unsigned short	    m_sTextureName[PMD_STR_MAX_SIZE]; //by RSH 2013.7.30

protected:
	CGDM_LocationArr	m_cArrDivPt;			// DividedPt
	CGDM_MeshArr		m_cArrMesh;				// drawing covermesh

private:
	double				m_dMinDivLen;
	MeasureInfo			m_MeasureInfo;

	//kimyi add
private:
	GIS_PathData		m_PathData;
public:
	GIS_PathData*		GetPathData() { return &m_PathData; }
};
