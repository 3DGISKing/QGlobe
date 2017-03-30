#include "GDM_Coord3D.h"
#include "GDM_Math.h"
#include <qglobal.h>

// B,L,Hµá¹¢À¾ ¼é°¢»ô¶®Áì±ËµáÌ© ¼­Ãû
void gdm_GetPosition(IN GDM_LOCATION *pLoc , OUT CGDM_Point3DF* pPosF)
{
	double dist = pLoc->m_dDist + EARTH_RADIUS;
	double projXY = dist * cos(pLoc->m_dLatitude);
	pPosF->m_tZ = dist * sin(pLoc->m_dLatitude);
	pPosF->m_tX = projXY * cos(pLoc->m_dLongitude);
	pPosF->m_tY = projXY * sin(pLoc->m_dLongitude);
}

// ¼é°¢»ô¶®Áìµá¹¢À¾ B,L,HµáÌ© ¼­Ãû
void gdm_GetLocation(IN CGDM_Point3DF* pPosF, OUT GDM_LOCATION *pLoc)
{
	double projXY = sqrt(pPosF->m_tX * pPosF->m_tX + pPosF->m_tY * pPosF->m_tY);
	double dist = pPosF->getLength();
	pLoc->m_dDist = dist - EARTH_RADIUS;
	pLoc->m_dLatitude = asin(pPosF->m_tZ / dist);
	pLoc->m_dLongitude = atan2(pPosF->m_tY , pPosF->m_tX);
	if(pLoc->m_dLongitude > 0)
	{
		if(pPosF->m_tY < 0.0f)
			pLoc->m_dLongitude -= gdm_PI;
		else if(pPosF->m_tY == 0.0f)
		{
			pLoc->m_dLongitude = pPosF->m_tX > 0.0f ? 0 : gdm_PI;
		}
	}
	else
	{
		if(pPosF->m_tY > 0.0f)
			pLoc->m_dLongitude += gdm_PI;
		else if(pPosF->m_tY == 0.0f)
		{
			pLoc->m_dLongitude = pPosF->m_tX > 0.0f ? 0 : gdm_PI;
		}
	}
}

// ¼é°¢»ô¶®Áìµá¹¢À¾ B,L,HµáÌ© ¼­Ãû (Ë§Ì© ÂÜºãÌ® ÁÄµ¹¸¡À¾°¡ ³Þ¶£³Þ.)
void gdm_GetLocation(IN CGDM_Point3DF* pPosF, OUT CGDM_Location3D *pLoc)
{
	double projXY = sqrt(pPosF->m_tX * pPosF->m_tX + pPosF->m_tY * pPosF->m_tY);
	double dist = pPosF->getLength();
	pLoc->m_tZ = dist - EARTH_RADIUS;
	pLoc->m_tY = asin(pPosF->m_tZ / dist);
	pLoc->m_tX = atan2(pPosF->m_tY , pPosF->m_tX);
	if(pLoc->m_tX > 0)
	{
		if(pPosF->m_tY < 0.0f)
			pLoc->m_tX -= gdm_PI;
		else if(pPosF->m_tY == 0.0f)
		{
			pLoc->m_tX = pPosF->m_tX > 0.0f ? 0 : gdm_PI;
		}
	}
	else
	{
		if(pPosF->m_tY > 0.0f)
			pLoc->m_tX += gdm_PI;
		else if(pPosF->m_tY == 0.0f)
		{
			pLoc->m_tX = pPosF->m_tX > 0.0f ? 0 : gdm_PI;
		}
	}
}
////////////////////////////////////////////////////////
//	GDM_Coord3D

void gdm_RotateAroundVector(CGDM_Point3DF *pPt, double angle, CGDM_Vector3DF vt)
{
	CGDM_Quaternion q;
	CGDM_Matrix4 mat;
	vt.normalize();
	q.fromAngleAxis(angle , vt);
	q.getMatrix(mat);
	mat.transformVect(*pPt);
}

void gdm_GetAngleBetweenTwoVector(CGDM_Vector3DF vt1, CGDM_Vector3DF vt2 , CGDM_Vector3DF upVt, double &angle)
{
	vt1.normalize();
	vt2.normalize();
	upVt.normalize();
	vt1 = vt1 * EARTH_RADIUS;
	vt2 = vt2 * EARTH_RADIUS;
	upVt = upVt * EARTH_RADIUS;
	CGDM_Vector3DF vt = vt1.crossProduct(vt2);
	double fVal = vt.dotProduct(upVt);
	double fScalar;
	double value;
	fScalar = vt1.dotProduct(vt2);
	value = fScalar / (vt1.getLength() * vt2.getLength());
	if(value >= 1.0)
		angle = 0;
	else if(value <= -1.0)
		angle = gdm_PI;
	else
		angle = acos(value);
	if(fVal < 0)
	{
		angle = - angle;	
	}
}

void gdm_GetPerpendPt(CGDM_Point3DF* pResult, const CGDM_Point3DF* pPt , const CGDM_Point3DF* pOrg , const CGDM_Vector3DF* pVt)
{
	CGDM_Vector3DF vt = *pPt - *pOrg;
	CGDM_Vector3DF vt1 = *pVt;
	vt1.normalize();
	double t;
	t = vt.m_tX * vt1.m_tX + vt.m_tY * vt1.m_tY + vt.m_tZ * vt1.m_tZ;
	*pResult = *pOrg + vt1 * t;
}

void gdm_RotateAroundVector(GDM_Coord3D *pCoord, double angle, CGDM_Vector3DF vt)
{
	CGDM_Quaternion q;
	CGDM_Matrix4 mat;
	vt.normalize();
	q.fromAngleAxis(angle , vt);
	q.getMatrix(mat);

	mat.transformVect(pCoord->m_org);
	mat.transformVect(pCoord->m_x);
	mat.transformVect(pCoord->m_y);
	mat.transformVect(pCoord->m_z);
}

void gdm_RotateAroundLine(CGDM_Point3DF *pPt, double angle, CGDM_Point3DF org , CGDM_Vector3DF vt)
{
	vt.normalize();
	CGDM_Quaternion q;
	CGDM_Matrix4 mat;
	q.fromAngleAxis(angle , vt);
	q.getMatrix(mat);
	CGDM_Vector3DF _vt;
	_vt = *pPt - org;
	mat.transformVect(_vt);
	*pPt = org + _vt;
}

void gdm_RotateAroundLine(GDM_Coord3D *pCoord, double angle, CGDM_Point3DF org , CGDM_Vector3DF vt)
{
	vt.normalize();
	CGDM_Quaternion q;
	CGDM_Matrix4 mat;
	q.fromAngleAxis(angle , vt);
	q.getMatrix(mat);
	mat.transformVect(pCoord->m_x);
	mat.transformVect(pCoord->m_y);
	mat.transformVect(pCoord->m_z);

	CGDM_Vector3DF _vt;
	_vt = pCoord->m_org - org;
	mat.transformVect(_vt);
	pCoord->m_org = org + _vt;
}

void gdm_GetQuaternion(IN GDM_Coord3D *pCoord, OUT CGDM_Quaternion* pQuaternion)
{
	CGDM_Matrix4 mat;
	gdm_MatrixSCStoTCS(pCoord , &mat);
	*pQuaternion = mat;
}

void gdm_SetQuaternion(CGDM_Quaternion* pQuaternion , GDM_Coord3D *pCoord)
{
	CGDM_Matrix4 mat;
	pQuaternion->getMatrix(mat);
	double* m = mat.pointer();
	pCoord->m_x.set(m[0], m[1] , m[2]);
	pCoord->m_y.set(m[4], m[5] , m[6]);
	pCoord->m_z.set(m[8], m[9] , m[10]);
}

void gdm_MatrixSCStoTCS(IN GDM_Coord3D *pCoord, OUT CGDM_Matrix4 *pMat)
{
	CGDM_Vector3DF mx , my , mz;
	CGDM_Vector3DF org;
	org = pCoord->m_org;
	mx = pCoord->m_x;
	my = pCoord->m_y;
	mz = pCoord->m_z;

	double* m = pMat->pointer();
	m[0] = mx.m_tX;
	m[1] = my.m_tX;
	m[2] = mz.m_tX;
	m[3] = 0;

	m[4] = mx.m_tY;
	m[5] = my.m_tY;
	m[6] = mz.m_tY;
	m[7] = 0;

	m[8] = mx.m_tZ;
	m[9] = my.m_tZ;
	m[10] = mz.m_tZ;
	m[11] = 0;

	m[12] = -(org.m_tX * mx.m_tX + org.m_tY * mx.m_tY + org.m_tZ * mx.m_tZ);
	m[13] = -(org.m_tX * my.m_tX + org.m_tY * my.m_tY + org.m_tZ * my.m_tZ);
	m[14] = -(org.m_tX * mz.m_tX + org.m_tY * mz.m_tY + org.m_tZ * mz.m_tZ);
	m[15] = 1;
}

void gdm_MatrixTCStoSCS(GDM_Coord3D *pCoord, CGDM_Matrix4 *pMat)
{
	CGDM_Vector3DF mx , my , mz;
	CGDM_Vector3DF org;
	org = pCoord->m_org;
	mx = pCoord->m_x;
	my = pCoord->m_y;
	mz = pCoord->m_z;

	double* m = pMat->pointer();
	m[0] = mx.m_tX;
	m[1] = mx.m_tY;
	m[2] = mx.m_tZ;
	m[3] = 0;

	m[4] = my.m_tX;
	m[5] = my.m_tY;
	m[6] = my.m_tZ;
	m[7] = 0;

	m[8] = mz.m_tX;
	m[9] = mz.m_tY;
	m[10] = mz.m_tZ;
	m[11] = 0;

	m[12] = org.m_tX;
	m[13] = org.m_tY;
	m[14] = org.m_tZ;
	m[15] = 1;
}

void gdm_RotateAroundXAxis(GDM_Coord3D *pCoord, double angle)
{
	GDM_Coord3D tcoord = *pCoord;
	tcoord.m_y *= (double)cos(angle);
	tcoord.m_z *= (double)sin(angle);

	pCoord->m_y = tcoord.m_y + tcoord.m_z;
	pCoord->m_y.normalize();
	pCoord->m_z = pCoord->m_x.crossProduct(pCoord->m_y);
}

void gdm_RotateAroundYAxis(GDM_Coord3D *pCoord, double angle)
{
	GDM_Coord3D tcoord = *pCoord;
	tcoord.m_z *= (double)cos(angle);
	tcoord.m_x *= (double)sin(angle);

	pCoord->m_z = tcoord.m_z + tcoord.m_x;
	pCoord->m_z.normalize();
	pCoord->m_x = pCoord->m_y.crossProduct(pCoord->m_z);
}

void gdm_RotateAroundZAxis(GDM_Coord3D *pCoord, double angle)
{
	GDM_Coord3D tcoord = *pCoord;
	tcoord.m_x *= (double)cos(angle);
	tcoord.m_y *= (double)sin(angle);

	pCoord->m_x = tcoord.m_x + tcoord.m_y;
	pCoord->m_x.normalize();
	pCoord->m_y = pCoord->m_z.crossProduct(pCoord->m_x);
}

void gdm_TCStoSCS(GDM_Coord3D *pCoord, CGDM_Point3DF *pnt)
{
	CGDM_Point3DF ret;
	ret.m_tX = pCoord->m_org.m_tX + pnt->m_tX * pCoord->m_x.m_tX + 
		pnt->m_tY * pCoord->m_y.m_tX + pnt->m_tZ * pCoord->m_z.m_tX;
	ret.m_tY = pCoord->m_org.m_tY + pnt->m_tX * pCoord->m_x.m_tY + 
		pnt->m_tY * pCoord->m_y.m_tY + pnt->m_tZ * pCoord->m_z.m_tY;
	ret.m_tZ = pCoord->m_org.m_tZ + pnt->m_tX * pCoord->m_x.m_tZ + 
		pnt->m_tY * pCoord->m_y.m_tZ + pnt->m_tZ * pCoord->m_z.m_tZ;
	*pnt = ret;
}

void gdm_VectorTCStoSCS(GDM_Coord3D *pCoord, CGDM_Vector3DF *pVtr)
{
	CGDM_Vector3DF ret;
	ret.m_tX = pVtr->m_tX * pCoord->m_x.m_tX + pVtr->m_tY * pCoord->m_y.m_tX + pVtr->m_tZ * pCoord->m_z.m_tX;
	ret.m_tY = pVtr->m_tX * pCoord->m_x.m_tY + pVtr->m_tY * pCoord->m_y.m_tY + pVtr->m_tZ * pCoord->m_z.m_tY;
	ret.m_tZ = pVtr->m_tX * pCoord->m_x.m_tZ + pVtr->m_tY * pCoord->m_y.m_tZ + pVtr->m_tZ * pCoord->m_z.m_tZ;
	*pVtr = ret;
}

void gdm_SCStoTCS(GDM_Coord3D *pCoord, CGDM_Point3DF* pnt)
{
	CGDM_Vector3DF vtr = *pnt - pCoord->m_org;
	pnt->m_tX = vtr.dotProduct(pCoord->m_x);
	pnt->m_tY = vtr.dotProduct(pCoord->m_y);
	pnt->m_tZ = vtr.dotProduct(pCoord->m_z);
}

void gdm_VectorSCStoTCS(GDM_Coord3D *pCoord, CGDM_Vector3DF *pVtr)
{
	CGDM_Vector3DF ret;
	ret.m_tX = pCoord->m_x.dotProduct(*pVtr);
	ret.m_tY = pCoord->m_y.dotProduct(*pVtr);
	ret.m_tZ = pCoord->m_z.dotProduct(*pVtr);
	*pVtr = ret;
}