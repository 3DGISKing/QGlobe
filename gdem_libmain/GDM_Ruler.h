#ifndef GDM_RULER_H
#define GDM_RULER_H

#include "GDM_Vector3D.h"
#include "GDM_Location.h"
#include "GDM_Camera.h"

class CGDM_Ruler
{
public:
	CGDM_Ruler (CGDM_Camera	*pCamera)	{
		m_pRulerNode = NULL;
		m_pCamera = pCamera;
	}
	~CGDM_Ruler (void)	{}

	void Clear ();
	void Release (int nStep, bool blLine);
	void StepPoint(GDM_LOCATION *a_pLocation, int nStep, bool blLine = true);
	void MovePoint(GDM_LOCATION *a_pLocation, int nStep, bool blLine = true);
	void GetMeasureSceneNode(GDM_SceneManager* sceneMgr);

public:
	double	CountAllLength (bool blLine = true);
	double	CountAngle (bool blLine = true);
	bool	GetPointFromLocation (GDM_LOCATION *a_pLocation, CGDM_Point3DF	*a_pPoint, double dHeight = 0);
	int		GetRulerSegPoints(void *pPoint, GDM_LOCATION *pStart, GDM_LOCATION *pEnd);

protected:
	void	AddPoint (geo::core::array<geo::core::vector3df> *a_pPointAry, GDM_LOCATION *a_pLocation, double a_dHeight = 0, double a_fRatio = 1);
	int		GetValidDivid(GDM_LOCATION *pStart, GDM_LOCATION *pEnd, double	&dHeight);

protected:
	CGDM_Camera						*m_pCamera;
	geo::scene::CRulerSceneNode		*m_pRulerNode;
};

#endif
