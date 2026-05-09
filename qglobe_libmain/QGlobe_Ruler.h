#ifndef QGlobe_RULER_H
#define QGlobe_RULER_H

#include "QGlobe_Vector3D.h"
#include "QGlobe_Location.h"
#include "QGlobe_Camera.h"

class CQGlobe_Ruler
{
public:
	CQGlobe_Ruler (CQGlobe_Camera	*pCamera)	{
		m_pRulerNode = NULL;
		m_pCamera = pCamera;
	}
	~CQGlobe_Ruler (void)	{}

	void Clear ();
	void Release (int nStep, bool blLine);
	void StepPoint(QGlobe_LOCATION *a_pLocation, int nStep, bool blLine = true);
	void MovePoint(QGlobe_LOCATION *a_pLocation, int nStep, bool blLine = true);
	void GetMeasureSceneNode(QGlobe_SceneManager* sceneMgr);

public:
	double	CountAllLength (bool blLine = true);
	double	CountAngle (bool blLine = true);
	bool	GetPointFromLocation (QGlobe_LOCATION *a_pLocation, CQGlobe_Point3DF	*a_pPoint, double dHeight = 0);
	int		GetRulerSegPoints(void *pPoint, QGlobe_LOCATION *pStart, QGlobe_LOCATION *pEnd);

protected:
	void	AddPoint (geo::core::array<geo::core::vector3df> *a_pPointAry, QGlobe_LOCATION *a_pLocation, double a_dHeight = 0, double a_fRatio = 1);
	int		GetValidDivid(QGlobe_LOCATION *pStart, QGlobe_LOCATION *pEnd, double	&dHeight);

protected:
	CQGlobe_Camera						*m_pCamera;
	geo::scene::CRulerSceneNode		*m_pRulerNode;
};

#endif
