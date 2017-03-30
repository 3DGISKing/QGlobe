#pragma once

#include "GIS_Node.h"

class GDEM_GIS_EXPORT CGIS_Point : public CGIS_Node
{
public:
	CGIS_Point(void);
	~CGIS_Point(void);

	// operator 
	CGIS_Point &		    operator=(const CGIS_Point & cNode);
	virtual void			CopyFrom(const CGIS_Node & cNode);
	virtual CGIS_Node *		Clone();
	virtual void	        Write(QDataStream &out);
	virtual void	        Read(QDataStream &out);

public:
	// track functions for select or modify
	virtual unsigned int	Get_TrackCount()  {return 1;}
	virtual void			Get_TrackPt(unsigned int trackIndex, CGDM_Location3D *pPt);
	virtual void			Set_TrackPt(unsigned int trackIndex, CGDM_Location3D *pPt);

	// for select
	virtual bool	        IsContain(MOUSE_POSITION &pos);

	void					SetScreenPos(double x, double y);
	void					SetPt(double x, double y, double z);
	CGDM_Location3D			GetPt(){return m_cPt;}
private:
	void					ResetBound();

protected:
	CGDM_Location3D			m_cPt;

private:
	double					m_dScreenX;
	double					m_dScreenY;
};
