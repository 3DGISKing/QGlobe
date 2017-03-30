#pragma once
#include "GDM_Vector3D.h"
#include "GDM_Define.h"

class CGDM_CollisionHandler
{
public:
	CGDM_CollisionHandler(void);
	~CGDM_CollisionHandler(void);

	GDM_ERROR_NO	ValidatePos( INOUT CGDM_Point3DF *Pt);
	bool			IsInvaildPos(CGDM_Point3DF *Pt);
};
