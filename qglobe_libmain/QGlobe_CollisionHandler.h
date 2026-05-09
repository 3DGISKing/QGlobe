#pragma once
#include "QGlobe_Vector3D.h"
#include "QGlobe_Define.h"

class CQGlobe_CollisionHandler
{
public:
	CQGlobe_CollisionHandler(void);
	~CQGlobe_CollisionHandler(void);

	QGlobe_ERROR_NO	ValidatePos( INOUT CQGlobe_Point3DF *Pt);
	bool			IsInvaildPos(CQGlobe_Point3DF *Pt);
};
